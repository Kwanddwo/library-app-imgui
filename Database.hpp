#pragma once

#include <string>
#include <vector>
#include <memory>
#include "DatabaseConnection.hpp"
#include "Models.hpp"


class BaseDAO {
protected:
    DatabaseConnection db;
public:
    BaseDAO(DatabaseConnection& db) : db(db) {}
    DatabaseConnection& getDB() { return db; }
};

class UserDAO : public BaseDAO {
public:
    UserDAO(DatabaseConnection& db) : BaseDAO(db) {}

    std::string getHash(const std::string& email) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT passwordHash FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (res->next()) {
            return res->getString("passwordHash");
        }
        throw std::runtime_error("Email not found");
    }

    void insertUser(const std::string& email, const std::string& firstName, const std::string& lastName, const std::string& passwordHash, const std::string& userRole) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO users (email, firstName, lastName, passwordHash, userRole) VALUES (?, ?, ?, ?, ?)"));
        pstmt->setString(1, email);
        pstmt->setString(2, firstName);
        pstmt->setString(3, lastName);
        pstmt->setString(4, passwordHash);
        pstmt->setString(5, userRole);
        pstmt->execute();
    }

    void updateUser(int id, const std::string& email, const std::string& firstName, const std::string& lastName, const std::string& userRole) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE users SET email = ?, firstName = ?, lastName = ?, userRole = ? WHERE id = ?"));
        pstmt->setString(1, email);
        pstmt->setString(2, firstName);
        pstmt->setString(3, lastName);
        pstmt->setString(4, userRole);
        pstmt->setInt(5, id);
        pstmt->execute();
    }

    void updateUserName(const std::string& email, const std::string& firstName, const std::string& lastName) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE users SET firstName = ?, lastName = ? WHERE email = ?"));
        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, email);
        pstmt->execute();
    }

    void deleteUser(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");    

        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM users WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    User findUserById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            return User();
        }

        User user(
            id,
            res->getString("email"),
            res->getString("firstName"),
            res->getString("lastName"),
            User::stringToUserRole(res->getString("userRole"))
        );

        return user;
    }

    User findUserByEmail(const std::string& email) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (!res->next()) {
            throw std::runtime_error("Email not found");
        }

        User user(
            res->getInt("id"),
            email,
            res->getString("firstName"),
            res->getString("lastName"),
            User::stringToUserRole(res->getString("userRole"))
        );

        return user;
    }

    std::vector<User> findAllUsers() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users;"));
        std::vector<User> vUser;
        while (res->next()) {
            User user(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
            vUser.push_back(user);
        }
        return vUser;
    }

    bool doesEmailExist(const std::string& email) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        return res->next();
    }
    User most_activeUser(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(*) AS user_count "
                            "FROM users u "
                            "JOIN borrowings br ON u.id = br.clientId ";

        if (range == "past month") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        } else if (range == "past year") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        } else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY user_count DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
           std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        //cout testing
           
        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );

        }

        throw std::runtime_error("No active users found for the given range: " + range);
    }
    User least_activeUser(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(*) AS user_count "
            "FROM users u "
            "JOIN borrowings br ON u.id = br.clientId ";

        if (range == "past month") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "past year") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY user_count ASC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
        }

        return User();
    }

    User most_activeLibrarian(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(*) AS user_count "
            "FROM users u "
            "JOIN borrowings br ON u.id = br.librarianId ";

        if (range == "past month") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "past year") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY user_count DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
        }

        return User();
    }

    User least_activeLibrarian(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(*) AS user_count "
            "FROM users u "
            "JOIN borrowings br ON u.id = br.librarianId ";

        if (range == "past month") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "past year") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY user_count ASC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
        }

        return User();
    }

    User most_unfaithfulUser(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(u.id) AS unfaithful_count "
            "FROM users u "
            "JOIN borrowings br ON u.id = br.clientId "
            "WHERE (br.status = 'not returned' AND NOW() > br.dateIntendedReturn) "
            "OR br.dateActualReturn > br.dateIntendedReturn ";

        if (range == "past month") {
            query += "AND br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "past year") {
            query += "AND br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY unfaithful_count DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
        }

        return User();
    }

    User most_trustedUser(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT u.id, u.email, u.firstName, u.lastName, u.userRole, COUNT(u.id) AS borrow_count "
            "FROM users u "
            "JOIN borrowings br ON u.id = br.clientId "
            "WHERE (br.status = 'returned' AND br.dateActualReturn <= br.dateIntendedReturn) "
            "OR (br.status = 'not returned' AND NOW() < br.dateIntendedReturn) ";

        if (range == "past month") {
            query += "AND br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "past year") {
            query += "AND br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range value, using default (of all time): " + range);
        }

        query += "GROUP BY u.id ORDER BY borrow_count DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return User(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("userRole"))
            );
        }

        return User();
    }


 
};

class AuthorDAO : public BaseDAO {
public:
    AuthorDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertAuthor(const std::string& firstName, const std::string& lastName, const std::string& dateOfBirth) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO authors (firstName, lastName, dateOfBirth) VALUES (?, ?, ?)"));
        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, dateOfBirth);
        pstmt->execute();
    }

    void updateAuthor(int id, const std::string& firstName, const std::string& lastName, const std::string& dateOfBirth) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE authors SET firstName = ?, lastName = ?, dateOfBirth = ? WHERE id = ?"));
        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, dateOfBirth);
        pstmt->setInt(4, id);
        pstmt->execute();
    }

    void deleteAuthor(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM authors WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Author findAuthorById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM authors WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Author author(
            id,
            res->getString("firstName"),
            res->getString("lastName"),
            res->getString("dateOfBirth")
        );

        return author;
    }

    std::vector<Author> findAllAuthors() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM authors;"));
        std::vector<Author> vAuthor;
        while (res->next()) {
            Author author(
                res->getInt("id"),
                res->getString("firstName"),
                res->getString("lastName"),
                res->getString("dateOfBirth")
            );
            vAuthor.push_back(author);
        }
        return vAuthor;
    }
};

class EditorDAO : public BaseDAO {
public:
    EditorDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertEditor(const std::string& name) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO editors (name) VALUES (?)"));
        pstmt->setString(1, name);
        pstmt->execute();
    }

    void updateEditor(int id, const std::string& name) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE editors SET name = ? WHERE id = ?"));
        pstmt->setString(1, name);
        pstmt->setInt(2, id);
        pstmt->execute();
    }

    void deleteEditor(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM editors WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Editor findEditorById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM editors WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Editor editor(
            id,
            res->getString("name")
        );

        return editor;
    }
    std::vector<Editor> findAllEditors() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM editors;"));
        std::vector<Editor> vEditor;
        while (res->next()) {
            Editor editor(
                res->getInt("id"),
                res->getString("name")
            );
            vEditor.push_back(editor);
        }
        return vEditor;
    }
};

class LanguageDAO : public BaseDAO {
public:
    LanguageDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertLanguage(const std::string& name, const char code[2]) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO languages (name, code) VALUES (?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, std::string(code, 2));
        pstmt->execute();
    }

    void updateLanguage(int id, const std::string& name, const char code[2]) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE languages SET name = ?, code = ? WHERE id = ?"));
        pstmt->setString(1, name);
        pstmt->setString(2, std::string(code, 2));
        pstmt->setInt(3, id);
        pstmt->execute();
    }

    void deleteLanguage(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM languages WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Language findLanguageById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM languages WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Language language(
            id,
            res->getString("name"),
            res->getString("code").c_str()
        );

        return language;
    }

    std::vector<Language> findAllLanguages() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM languages;"));
        std::vector<Language> vLanguage;
        while (res->next()) {
            Language language(
                res->getInt("id"),
                res->getString("name"),
                res->getString("code").c_str()
            );
            vLanguage.push_back(language);
        }
        return vLanguage;
    }
};

class CategoryDAO : public BaseDAO {
public:
    CategoryDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertCategory(const std::string& name, const std::string& type) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO categories (name, type) VALUES (?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, type);
        pstmt->execute();
    }

    void updateCategory(int id, const std::string& name, const std::string& type) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE categories SET name = ?, type = ? WHERE id = ?"));
        pstmt->setString(1, name);
        pstmt->setString(2, type);
        pstmt->setInt(3, id);
        pstmt->execute();
    }

    void deleteCategory(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM categories WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Category findCategoryById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM categories WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Category category(
            id,
            res->getString("name"),
            res->getString("type")
        );

        return category;
    }

    std::vector<Category> findAllCategories(std::string type="category OR type = \'genre\'") {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM categories where type = ?;"));
        pstmt->setString(1, type);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::vector<Category> vCategory;
        while (res->next()) {
            Category category(
                res->getInt("id"),
                res->getString("name"),
                res->getString("type")
            );
            vCategory.push_back(category);
        }
        return vCategory;
    }
};

class BookDAO : public BaseDAO {
public:
    BookDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertBook(const std::string& isbn, const std::string& title, int pubYear, int numAvailableCopies, int nbrPages, int languageId, int editorId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO books (isbn, title, pubYear, numAvailableCopies, nbrPages, languageId, editorId) VALUES (?, ?, ?, ?, ?, ?, ?)"));
        pstmt->setString(1, isbn);
        pstmt->setString(2, title);
        pstmt->setInt(3, pubYear);
        pstmt->setInt(4, numAvailableCopies);
        pstmt->setInt(5, nbrPages);
        pstmt->setInt(6, languageId);
        pstmt->setInt(7, editorId);
        pstmt->execute();
    }

    void updateBook(int id, const std::string& isbn, const std::string& title, int pubYear, int numAvailableCopies, int nbrPages, int languageId, int editorId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE books SET isbn = ?, title = ?, pubYear = ?, numAvailableCopies = ?, nbrPages = ?, languageId = ?, editorId = ? WHERE id = ?"));
        pstmt->setString(1, isbn);
        pstmt->setString(2, title);
        pstmt->setInt(3, pubYear);
        pstmt->setInt(4, numAvailableCopies);
        pstmt->setInt(5, nbrPages);
        pstmt->setInt(6, languageId);
        pstmt->setInt(7, editorId);
        pstmt->setInt(8, id);
        pstmt->execute();
    }

    void deleteBook(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM books WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Book findBookById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM books WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Book book(
            id,
            res->getString("isbn"),
            res->getString("title"),
            res->getInt("pubYear"),
            res->getInt("numAvailableCopies"),
            res->getInt("nbrPages"),
            LanguageDAO(db).findLanguageById(res->getInt("languageId")),
            EditorDAO(db).findEditorById(res->getInt("editorId")),
            findAuthorsByBookId(id),
            findCategoriesByBookId(id, "category"),
            findCategoriesByBookId(id, "genre")
        );

        return book;
    }

    Book findBookByIsbn(std::string isbn) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM books WHERE isbn = ?"));
        pstmt->setString(1, isbn);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ISBN not found");
        }

		int id = res->getInt("id");

        Book book(
            id,
            isbn,
            res->getString("title"),
            res->getInt("pubYear"),
            res->getInt("numAvailableCopies"),
            res->getInt("nbrPages"),
            LanguageDAO(db).findLanguageById(res->getInt("languageId")),
            EditorDAO(db).findEditorById(res->getInt("editorId")),
            findAuthorsByBookId(id),
            findCategoriesByBookId(id, "category"),
            findCategoriesByBookId(id, "genre")
        );

        return book;
    }

    std::vector<Book> findAllBooks() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM books;"));
        std::vector<Book> vBook;
        while (res->next()) {
            Book book(
                res->getInt("id"),
                res->getString("isbn"),
                res->getString("title"),
                res->getInt("pubYear"),
                res->getInt("numAvailableCopies"),
                res->getInt("nbrPages"),
                LanguageDAO(db).findLanguageById(res->getInt("languageId")),
                EditorDAO(db).findEditorById(res->getInt("editorId")),
                findAuthorsByBookId(res->getInt("id")),
                findCategoriesByBookId(res->getInt("id"), "category"),
                findCategoriesByBookId(res->getInt("id"), "genre")
            );
            vBook.push_back(book);
        }
        return vBook;
    }


    int getNumberOfBooks(const std::string& lang = "", const std::string& cat = "", const std::string& gen = "") {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query = "SELECT COUNT(*) FROM books b";

        if (!cat.empty() || !gen.empty()) {
            query += " JOIN book_categories bc ON b.id = bc.bookId";
            query += " JOIN categories c ON c.id = bc.categoryId";
        }
        if (!lang.empty()) {
            query += " JOIN languages l ON l.id = b.languageId";
        }

        std::string conditions = "";
        if (!lang.empty()) {
            conditions += "l.name = ?";
        }
        if (!cat.empty()) {
            if (!conditions.empty()) conditions += " AND ";
            conditions += "c.type = 'category' AND c.name = ?";
        }
        if (!gen.empty()) {
            if (!conditions.empty()) conditions += " AND ";
            conditions += "c.type = 'genre' AND c.name = ?";
        }

        if (!conditions.empty()) {
            query += " WHERE " + conditions;
        }

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        int paramIndex = 1;

        if (!lang.empty()) {
            pstmt->setString(paramIndex++, lang);
        }
        if (!cat.empty()) {
            pstmt->setString(paramIndex++, cat);
        }
        if (!gen.empty()) {
            pstmt->setString(paramIndex++, gen);
        }

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        res->next();
        return res->getInt(1);
    }

    Book most_borrowed_book_in_xperiod(const std::string& range) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::string query =
            "SELECT b.*, COUNT(br.id) AS borrow_count "
            "FROM books b "
            "JOIN borrowings br ON b.id = br.bookId ";

        if (range == "last month") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 MONTH) ";
        }
        else if (range == "last year") {
            query += "WHERE br.dateBorrowed > DATE_SUB(NOW(), INTERVAL 1 YEAR) ";
        }
        else if (range != "of all time") {
            throw std::invalid_argument("Invalid range provided: " + range);
        }

        query += "GROUP BY b.id ORDER BY borrow_count DESC LIMIT 1";

        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(query));
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            return Book(
                res->getInt("id"),
                res->getString("isbn"),
                res->getString("title"),
                res->getInt("pubYear"),
                res->getInt("numAvailableCopies"),
                res->getInt("nbrPages"),
                LanguageDAO(db).findLanguageById(res->getInt("languageId")),
                EditorDAO(db).findEditorById(res->getInt("editorId")),
                findAuthorsByBookId(res->getInt("id")),
                findCategoriesByBookId(res->getInt("id"), "category"),
                findCategoriesByBookId(res->getInt("id"), "genre")
            );
        }

        return Book();

    }

    std::vector<Author> findAuthorsByBookId(int bookId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT a.id, a.firstName, a.lastName, a.dateOfBirth FROM authors a JOIN authored_by ab ON a.id = ab.authorId WHERE ab.bookId = ?"));
        pstmt->setInt(1, bookId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::vector<Author> authors;
        while (res->next()) {
            Author author(
                res->getInt("id"),
                res->getString("firstName"),
                res->getString("lastName"),
                res->getString("dateOfBirth")
            );
            authors.push_back(author);
        }
        return authors;
    }

    std::vector<Category> findCategoriesByBookId(int bookId, const std::string& type) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT c.id, c.name, c.type FROM categories c JOIN book_categories bc ON c.id = bc.categoryId WHERE bc.bookId = ? AND c.type = ?"));
        pstmt->setInt(1, bookId);
        pstmt->setString(2, type);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::vector<Category> categories;
        while (res->next()) {
            Category category(
                res->getInt("id"),
                res->getString("name"),
                res->getString("type")
            );
            categories.push_back(category);
        }
        return categories;
    }

	void addAuthorsToBook(int bookId, const std::vector<int>& authorIds) {
		if (!db.isConnected()) throw std::runtime_error("Database not connected");
		auto conn = db.getConnection();
		for (const auto& authorId : authorIds) {
			std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO authored_by (bookId, authorId) VALUES (?, ?)"));
			pstmt->setInt(1, bookId);
			pstmt->setInt(2, authorId);
			pstmt->execute();
		}
	}
	void addCategoriesToBook(int bookId, const std::vector<int>& categoryIds) {
		if (!db.isConnected()) throw std::runtime_error("Database not connected");
		auto conn = db.getConnection();
		for (const auto& categoryId : categoryIds) {
			std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO book_categories (bookId, categoryId) VALUES (?, ?)"));
			pstmt->setInt(1, bookId);
			pstmt->setInt(2, categoryId);
			pstmt->execute();
		}
	}
	void clearAuthorsFromBook(int bookId) {
		if (!db.isConnected()) throw std::runtime_error("Database not connected");
		auto conn = db.getConnection();
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM authored_by WHERE bookId = ?"));
		pstmt->setInt(1, bookId);
		pstmt->execute();
	}
	void clearCategoriesFromBook(int bookId) {
		if (!db.isConnected()) throw std::runtime_error("Database not connected");
		auto conn = db.getConnection();
		std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM book_categories WHERE bookId = ?"));
		pstmt->setInt(1, bookId);
		pstmt->execute();
	}
};

class BorrowingDAO : public BaseDAO {
public:
    BorrowingDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertBorrowing(const std::string dateBorrowed, const std::string& dateIntendedReturn, const std::string& dateActualReturn, const std::string& status, int clientId, int librarianId, int bookId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO borrowings (dateBorrowed, dateIntendedReturn, dateActualReturn, status, clientId, librarianId, bookId) VALUES (?, ?, ?, ?, ?, ?, ?)"));
        if (dateBorrowed == "null")
            pstmt->setNull(1, sql::DataType::DATE);
        else
            pstmt->setString(1, dateBorrowed);
        if (dateIntendedReturn == "null")
            pstmt->setNull(2, sql::DataType::DATE);
        else
            pstmt->setString(2, dateIntendedReturn);
        if (dateActualReturn == "null")
            pstmt->setNull(3, sql::DataType::DATE);
        else
            pstmt->setString(3, dateActualReturn);
        pstmt->setString(4, status);
        pstmt->setInt(5, clientId);
        if (librarianId == -1)
            pstmt->setNull(6, sql::DataType::INTEGER);
        else
            pstmt->setInt(6, librarianId);
        pstmt->setInt(7, bookId);
        pstmt->execute();
    }

    void updateBorrowing(int id, const std::string& dateBorrowed, const std::string& dateIntendedReturn, const std::string& dateActualReturn, const std::string& status, int clientId, int librarianId, int bookId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE borrowings SET dateBorrowed = ?, dateIntendedReturn = ?, dateActualReturn = ?, status = ?, clientId = ?, librarianId = ?, bookId = ? WHERE id = ?"));
        pstmt->setString(1, dateBorrowed);
        pstmt->setString(2, dateIntendedReturn);
        pstmt->setString(3, dateActualReturn);
        pstmt->setString(4, status);
        pstmt->setInt(5, clientId);
        pstmt->setInt(6, librarianId);
        pstmt->setInt(7, bookId);
        pstmt->setInt(8, id);
        pstmt->execute();
    }

    void updateBorrowingStatus(int id, const std::string status, const std::string dateborrow, const std::string dateExpectedreturn, const std::string datereturn) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE borrowings SET status = ? , dateBorrowed=STR_TO_DATE(?, '%Y-%m-%d') ,dateIntendedReturn= STR_TO_DATE(?, '%Y-%m-%d') ,dateActualReturn= STR_TO_DATE(?, '%Y-%m-%d') WHERE id = ?"));
        pstmt->setString(1, status);
        pstmt->setString(2, dateborrow);
        pstmt->setString(3, dateExpectedreturn);
        pstmt->setString(4, datereturn);
        pstmt->setInt(5, id);
        pstmt->execute();
    }

    void deleteBorrowing(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM borrowings WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    Borrowing findBorrowingById(int id) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM borrowings WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (!res->next()) {
            throw std::runtime_error("ID not found");
        }

        Borrowing borrowing(
            id,
            res->getString("dateBorrowed"),
            res->getString("dateIntendedReturn"),
            res->getString("dateActualReturn"),
            res->getString("status"),
            UserDAO(db).findUserById(res->getInt("clientId")),
            UserDAO(db).findUserById(res->getInt("librarianId")),
            BookDAO(db).findBookById(res->getInt("bookId"))
        );

        return borrowing;
    }

    std::vector<Borrowing> findAllBorrowings() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM borrowings;"));
        std::vector<Borrowing> vBorrowing;
        while (res->next()) {
            Borrowing borrowing(
                res->getInt("id"),
                res->getString("dateBorrowed"),
                res->getString("dateIntendedReturn"),
                res->getString("dateActualReturn"),
                res->getString("status"),
                UserDAO(db).findUserById(res->getInt("clientId")),
                UserDAO(db).findUserById(res->getInt("librarianId")),
                BookDAO(db).findBookById(res->getInt("bookId"))
            );
            vBorrowing.push_back(borrowing);
        }
        return vBorrowing;
    }

    std::vector<Borrowing> getMemberBorrowings(const int clientId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "SELECT * FROM borrowings WHERE clientId = ?"));
        pstmt->setInt(1, clientId);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        std::vector<Borrowing> borrowings;
        while (res->next()) {
            int librarianId = res->isNull("librarianId") ? -1 : res->getInt("librarianId");

            User client = UserDAO(db).findUserById(res->getInt("clientId"));
            User librarian = (librarianId != -1) ? UserDAO(db).findUserById(librarianId) : User(); 
            Book book = BookDAO(db).findBookById(res->getInt("bookId"));
            Borrowing borrowing(
                res->getInt("id"),
                res->getString("dateBorrowed"),
                res->getString("dateIntendedReturn"),
                res->getString("dateActualReturn"),
                res->getString("status"),
                client,
                librarian,
                book
            );
            borrowings.push_back(borrowing);
        }
        return borrowings;
    }
};
