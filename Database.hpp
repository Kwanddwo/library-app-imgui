#pragma once

#include <string>
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