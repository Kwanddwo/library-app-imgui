#pragma once
#include "Page.hpp"

class LibrariansPage : public Page {

public:
    LibrariansPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};