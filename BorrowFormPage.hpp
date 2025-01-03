#pragma once
#include "Page.hpp"

class BorrowFormPage : public Page {

public:
    BorrowFormPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};