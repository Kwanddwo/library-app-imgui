#pragma once
#include "Page.hpp"

class BooksPage : public Page {

public:
    BooksPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};