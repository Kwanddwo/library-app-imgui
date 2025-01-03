#pragma once
#include "Page.hpp"

class WelcomePage : public Page {
    Auth& auth;

public:
    WelcomePage(Auth& auth) : auth(auth) {
        strncpy_s(title, "Welcome Page", sizeof(title));
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Hello!");
        ImGui::Text("First name: ");
        ImGui::SameLine();
        ImGui::Text("%s", auth.getCurrUser().getFirstName().c_str());
        ImGui::Text("Last name: ");
        ImGui::SameLine();
        ImGui::Text("%s", auth.getCurrUser().getLastName().c_str());
        ImGui::Text("Email: ");
        ImGui::SameLine();
        ImGui::Text("%s", auth.getCurrUser().getEmail().c_str());
        ImGui::Text("Role: ");
        ImGui::SameLine();
        ImGui::Text("%s", User::userRoleToString(auth.getCurrUser().getRole()).c_str());
    }
};