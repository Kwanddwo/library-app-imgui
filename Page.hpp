#pragma once
#include <string>
#include <memory>
#include "imgui.h"

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
    virtual void render(AppState& state, std::shared_ptr<PageState> pageState) = 0;
    char title[100];
};