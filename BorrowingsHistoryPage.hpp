#pragma once
#include "Page.hpp"

class BorrowingsHistoryPage : public Page {

public:
    BorrowingsHistoryPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};