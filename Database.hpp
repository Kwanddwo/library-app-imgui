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
            throw std::runtime_error("ID not found");
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

    std::vector<Category> findAllCategories() {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM categories;"));
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
            std::vector<Author>(), // Authors will be fetched separately
            std::vector<Category>(), // Categories will be fetched separately
            std::vector<Category>() // Genres will be fetched separately
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
                std::vector<Author>(), // Authors will be fetched separately
                std::vector<Category>(), // Categories will be fetched separately
                std::vector<Category>() // Genres will be fetched separately
            );
            vBook.push_back(book);
        }
        return vBook;
    }
};

class BorrowingDAO : public BaseDAO {
public:
    BorrowingDAO(DatabaseConnection& db) : BaseDAO(db) {}

    void insertBorrowing(const std::string& dateBorrowed, const std::string& dateIntendedReturn, const std::string& dateActualReturn, const std::string& status, int clientId, int librarianId, int bookId) {
        if (!db.isConnected()) throw std::runtime_error("Database not connected");
        auto conn = db.getConnection();
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO borrowings (dateBorrowed, dateIntendedReturn, dateActualReturn, status, clientId, librarianId, bookId) VALUES (?, ?, ?, ?, ?, ?, ?)"));
        pstmt->setString(1, dateBorrowed);
        pstmt->setString(2, dateIntendedReturn);
        pstmt->setString(3, dateActualReturn);
        pstmt->setString(4, status);
        pstmt->setInt(5, clientId);
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
};
