#pragma once
#include <string>
#include "Database.hpp"

class UIManager {
private:
    Library lib;
    DatabaseConnection db;
    User* currentUser;

    // UI state
    bool showLoginWindow;
    bool showBookList;
    bool showBorrowingManagement;
    bool showAdminDashboard;

public:
    UIManager(Library lib, DatabaseConnection db) : lib(lib), db(db) {
    };

    void render();

private:
    void renderLoginWindow();
    void renderBookList();
    void renderBorrowingManagement();
    void renderAdminDashboard();
    void renderStatistics();
};