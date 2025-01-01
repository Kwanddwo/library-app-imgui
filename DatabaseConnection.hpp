#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <stdlib.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

class DatabaseConnection {
private:
    sql::Driver* driver;
    sql::Connection* conn;

    const std::string host;
    const std::string user;
    const std::string password;
    const std::string database;

public:
    DatabaseConnection(
        const std::string& host,
        const std::string& user,
        const std::string& password,
        const std::string& database
    ) : host(host), user(user), password(password), database(database) {
        try {
            driver = get_driver_instance();
            conn = driver->connect(host, user, password);
            conn->setSchema(database);
        }
        catch (sql::SQLException& e) {
            std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
            system("pause");
            exit(1);
        }
    }

    DatabaseConnection(const DatabaseConnection& other)
        : host(other.host), user(other.user), password(other.password), database(other.database) {
        try {
            driver = get_driver_instance();
            conn = driver->connect(host, user, password);
            conn->setSchema(database);
        }
        catch (sql::SQLException& e) {
            std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
            system("pause");
            exit(1);
        }
    }

    ~DatabaseConnection() {
        if (conn) {
            delete conn;
        }
    }

    sql::Connection* getConnection() { return conn; }
};