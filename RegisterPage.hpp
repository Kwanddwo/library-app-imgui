#pragma once
#include "Page.hpp"

struct RegisterPageState : public PageState {
    char userEmail[100] = "";
    char userFirstName[50] = "";
    char userLastName[50] = "";
    char userPassword[64] = "";
};

class RegisterPage : public Page {
    Auth& auth;
    std::function<void(std::shared_ptr<PageState>)> onLoginClick;

public:
    RegisterPage(Auth& auth, std::function<void(std::shared_ptr<PageState>)> onLoginClick) : auth(auth), onLoginClick(onLoginClick) {
        strncpy_s(title, "Register Page", sizeof(title));
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        auto registerState = std::static_pointer_cast<RegisterPageState>(pageState);
        ImGui::InputText("First Name", registerState->userFirstName, 50);
        ImGui::InputText("Last Name", registerState->userLastName, 50);
        ImGui::InputText("Email", registerState->userEmail, 100);
        ImGui::InputText("Password", registerState->userPassword, 64);
        if (ImGui::Button("Register")) {
            try {
                if (!auth.registerUser(registerState->userEmail, registerState->userPassword, registerState->userFirstName, registerState->userLastName)) {
                    state.errorMessage = "Registration failed. Email might already be in use.";
                    state.showErrorModal = true;
                }
                else {
                    auto loginState = std::make_shared<LoginPageState>();
                    strncpy_s(loginState->userEmail, registerState->userEmail, sizeof(loginState->userEmail));
                    strncpy_s(loginState->userPassword, registerState->userPassword, sizeof(loginState->userPassword));
                    onLoginClick(loginState);
                }
            }
            catch (const std::exception& e) {
                state.errorMessage = e.what();
                state.showErrorModal = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Login Page")) {
            onLoginClick(pageState);
        }
    }
};