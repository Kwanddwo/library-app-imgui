#pragma once
#include "Page.hpp"

class BorrowingsPage : public Page {

public:
    BorrowingsPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};