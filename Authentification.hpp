#pragma once

#include <iostream>
#include <string>
#include "Database.hpp"
#include "sha256/sha256.h"

using namespace std;

class Auth {
    UserDAO userdb;
    User currUser;
    bool isLoggedIn;

    void setCurrUser(const string& email) {
        currUser = userdb.findUserByEmail(email);
    }

    void removeCurrUser() {
        currUser = User();
    }

public:
    Auth(UserDAO& userdb) : userdb(userdb), isLoggedIn(false) {}

	UserDAO& getUserDAO() {
		return userdb;
	}

    bool getIsLoggedIn() const {
        return isLoggedIn;
    }

    User getCurrUser() const {
        return currUser;
    }

    string getHash(const string& email) {
        return userdb.getHash(email);
    }

    string createHash(const string& password) const {
        SHA256 sha;
        return sha(password);
    }

    bool testPassword(const string& password, const string& hash) const {
        SHA256 sha256;
        return hash == sha256(password);
    }

    bool registerUser(const string& email, const string& password, const string& first, const string& last, const string& role="member") {
        if (userdb.doesEmailExist(email))
            return false;
        string hash = createHash(password);
        userdb.insertUser(email, first, last, hash, role);
        return true;
    }

    bool login(const string& email, const string& password) {
        if (!userdb.doesEmailExist(email)) {
            return false;
        }
        string hash = getHash(email);
        if (!testPassword(password, hash)) {
            return false;
        }
        isLoggedIn = true;
        setCurrUser(email);
        return true;
    }

    void logout() {
        if (!isLoggedIn)
            return;
        removeCurrUser();
        isLoggedIn = false;
    }
};
