#pragma once

#include "Page.hpp"

struct LoginPageState : public PageState {
    char userEmail[100] = "";
    char userPassword[64] = "";
};

class LoginPage : public Page {
    Auth& auth;
    std::function<void(std::shared_ptr<PageState>)> onRegisterClick;
    std::function<void()> onLogin;

public:
    LoginPage(Auth& auth, std::function<void(std::shared_ptr<PageState>)> onRegisterClick, std::function<void()> onLogin) :
        auth(auth), onRegisterClick(onRegisterClick), onLogin(onLogin) {
        strncpy_s(title, "Login Page", sizeof(title));
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        auto loginState = std::static_pointer_cast<LoginPageState>(pageState);
        ImGui::InputText("Email", loginState->userEmail, 100);
        ImGui::InputText("Password", loginState->userPassword, 64, ImGuiInputTextFlags_Password);
        if (ImGui::Button("Login")) {
            try {
                if (!auth.login(loginState->userEmail, loginState->userPassword)) {
                    state.errorMessage = "Incorrect credentials";
                    state.showErrorModal = true;
                }
                else {
                    onLogin();
                }
            }
            catch (const std::exception& e) {
                state.errorMessage = e.what();
                state.showErrorModal = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Register")) {
            onRegisterClick(pageState);
        }
    }
};