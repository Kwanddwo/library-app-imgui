#pragma once
#include "Page.hpp"

class EditUserPage : public Page {
    User user;
    UserDAO& userDB;
    std::function<void(User)> onReturn;
    char userFirstName[50];
    char userLastName[50];
    char userEmail[100];
    char userPassword[64] = "";

public:
    EditUserPage(UserDAO& userDB, std::function<void(User)> onReturn, User user) : onReturn(onReturn), user(user), userDB(userDB) {
        strncpy_s(title, "Edit User", sizeof(title));
        
        strncpy_s(userEmail, user.getEmail().c_str(), 100);
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::InputText("First Name", userFirstName, 50);
        ImGui::InputText("Last Name", userLastName, 50);
        ImGui::InputText("Email", userEmail, 100);
        ImGui::InputText("Password", userPassword, 64);
        if (ImGui::Button("Update")) {
            try {
                userDB.updateUser(
                    user.getId(),
                    userEmail,
                    userFirstName,
                    userLastName,
                    User::userRoleToString(user.getRole())
                );
                onReturn(user);
            }
            catch (const std::exception& e) {
                state.errorMessage = e.what();
                state.showErrorModal = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            onReturn(user);
        }
    }
};