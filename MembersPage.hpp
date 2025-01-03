#pragma once
#include "Page.hpp"

class MembersPage : public Page {

public:
    MembersPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};