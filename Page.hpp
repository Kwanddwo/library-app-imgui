#pragma once
#include <string>
#include <memory>
#include "imgui.h"
#include "Database.hpp"

enum class PageType { 
    Login, 
    Register, 
    Profile, 
    BorrowingsHistory,
    BorrowForm,
    Books,
    Borrowings, 
    Members, 
    Librarians,
    Statistics
};

struct AppState {
    bool showErrorModal = false;
    std::string errorMessage;
};

struct PageState {
    virtual ~PageState() = default;
};

class Page {
public:
    char title[100] = "Library App";
    virtual void render(AppState& state, std::shared_ptr<PageState> pageState) = 0;
};