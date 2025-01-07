#pragma once
#include "Page.hpp"

class RegisterLibrarianPage : public Page {
    Auth& auth;
    std::function<void()> onReturn;

public:
    RegisterLibrarianPage(Auth& auth, std::function<void()> onReturn) : auth(auth), onReturn(onReturn) {
        strncpy_s(title, "Register Librarian", sizeof(title));
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        auto registerState = std::static_pointer_cast<RegisterPageState>(pageState);
        ImGui::InputText("First Name", registerState->userFirstName, 50);
        ImGui::InputText("Last Name", registerState->userLastName, 50);
        ImGui::InputText("Email", registerState->userEmail, 100);
        ImGui::InputText("Password", registerState->userPassword, 64);
        if (ImGui::Button("Register")) {
            try {
                if (!auth.registerUser(registerState->userEmail, registerState->userPassword, registerState->userFirstName, registerState->userLastName, "librarian")) {
                    state.errorMessage = "Registration failed. Email might already be in use.";
                    state.showErrorModal = true;
                }
                else {
                    onReturn();
                }
            }
            catch (const std::exception& e) {
                state.errorMessage = e.what();
                state.showErrorModal = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            onReturn();
        }
    }
};