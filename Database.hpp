#pragma once

#include <string>
#include "DatabaseConnection.hpp"
#include "Models.hpp"
//#include <exception>

class BaseDAO {
protected:
	DatabaseConnection db;
public:
	BaseDAO(DatabaseConnection& db) : db(db) {};
    DatabaseConnection& getDB() { return db; }
};

class UserDAO: BaseDAO {
public:
	UserDAO(DatabaseConnection& db) : BaseDAO(db) {};

    std::string getHash(const std::string& email) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT passwordHash FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        res->next();
        return res->getString("passwordHash");
    }
    void insertUser(const std::string& email, const std::string& firstName, const std::string& lastName, const std::string& passwordHash, const std::string& userRole) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "INSERT INTO users (email, firstName, lastName, passwordHash, userRole) VALUES (?, ?, ?, ?, ?)"));
        pstmt->setString(1, email);
        pstmt->setString(2, firstName);
        pstmt->setString(3, lastName);
        pstmt->setString(4, passwordHash);
        pstmt->setString(5, userRole);
        pstmt->execute();
    }

    void updateUser(int id, const std::string& email, const std::string& firstName, const std::string& lastName, const std::string& userRole) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
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
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
            "UPDATE users SET firstName = ?, lastName = ? WHERE email = ?"));
        pstmt->setString(1, firstName);
        pstmt->setString(2, lastName);
        pstmt->setString(3, email);
        pstmt->execute();
    }

    void deleteUser(int id) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM users WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
    }

    User findUserById(int id) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE id = ?"));
        pstmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        
        if (!res->next()) {
            throw "id not found";
        }
        
        User user(
            id,
            res->getString("email"),
            res->getString("firstName"),
            res->getString("lastName"),
            User::stringToUserRole(res->getString("role"))
        );

        return user;
    }

    User findUserByEmail(std::string email) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        if (!res->next()) {
            throw "id not found";
        }

        User user(
            res->getInt("id"),
            email,
            res->getString("firstName"),
            res->getString("lastName"),
            User::stringToUserRole(res->getString("role"))
        );

        return user;
    }

    std::vector<User> findAllUsers() {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users;"));
        std::vector<User> vUser;
        while (res->next()) {
            User user(
                res->getInt("id"),
                res->getString("email"),
                res->getString("firstName"),
                res->getString("lastName"),
                User::stringToUserRole(res->getString("role"))
            );
            vUser.push_back(user);
        }
        return vUser;
    }

    bool doesEmailExist(std::string email) {
        std::unique_ptr<sql::Connection> conn(db.getConnection());
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("SELECT * FROM users WHERE email = ?"));
        pstmt->setString(1, email);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        return res->next() ? true : false;
    }
};