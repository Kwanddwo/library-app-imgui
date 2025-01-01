#pragma once

#include <iostream>
#include <string>
#include "Database.hpp"
#include "sha256/sha256.h"

using namespace std;

class Auth {
    UserDAO userdb;

    void setCurrUser(string email) {
        currUser = userdb.findUserByEmail(email);
    }

    void removeCurrUser() {
        currUser = User();
    }
public:
    User currUser;
    bool isLoggedIn;

    Auth(UserDAO& userdb) : userdb(userdb) { 
        isLoggedIn = false;
    };

    string getHash(string email) {
        return userdb.getHash(email);
    }

    string createHash(string password) {
        SHA256 sha;
        return sha(password);
    }

    bool testPassword(string password, string hash) {
        SHA256 sha256;
        return hash == sha256(password);
    };

    bool registerUser(string email, string password, string first, string last) {
        if (userdb.doesEmailExist(email))
            return false;
        SHA256 sha256;
        string hash = sha256(password);
        userdb.insertUser(email, first, last, hash, "member");
        return true;
    };

    bool login(string email, string password) {
        if (!userdb.doesEmailExist(email)) {
            return false;
        }
        std::cout << "email exists apparently...\n";
        string hash = this->getHash(email);
        if (!testPassword(password, hash))
            return false;
        hash = "";
        this->isLoggedIn = true;
        this->setCurrUser(email);
        return true;
    }
    void logout() {
        if (!this->isLoggedIn)
            return;
        this->removeCurrUser();
        this->isLoggedIn = false;
    }
};