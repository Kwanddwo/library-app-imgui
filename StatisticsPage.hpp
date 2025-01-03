#pragma once
#include "Page.hpp"

class StatisticsPage : public Page {

public:
    StatisticsPage() {}

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Example Page");
    }
};