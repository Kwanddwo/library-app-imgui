#pragma once

enum class PageType { Login, Register, Welcome };

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