#pragma once

#include "imgui.h"
#include "Authentification.hpp"

#ifndef PARENT_FLAGS
#define PARENT_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
#endif

typedef enum { login_page, register_page } page;

struct AppState {
    page curr_page = login_page;

    // User form
    char userEmail[100] = "";
    char userFirstName[50] = "";
    char userLastName[50] = "";
    char userPassword[64] = "";
    char userRole[20] = "";
    int selectedUserId = -1;

    // Book form
    char bookIsbn[14] = "";
    char bookTitle[101] = "";
    int bookPubYear = 2024;
    int bookCopies = 0;
    int bookPages = 0;
    int selectedBookId = -1;

    // Search buffers
    char searchUserId[10] = "";
    char searchBookId[10] = "";

    // Modal flags
    bool showErrorModal = false;
    std::string errorMessage;
};

class InterfaceApp {
    Auth& auth;
    AppState state;

    void ShowErrorModal(AppState& state) {
        if (state.showErrorModal) {
            ImGui::OpenPopup("Error");
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("%s", state.errorMessage.c_str());
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    state.showErrorModal = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }

public:
    InterfaceApp(Auth& auth) : auth(auth) {}

    void renderLoginRegister(ImVec2 DisplaySize) {
        if (!auth.getIsLoggedIn()) {
            if (state.curr_page == login_page) {
                ImGui::Begin("Login Page", NULL, PARENT_FLAGS);
                ImGui::InputText("Email", state.userEmail, 100);
                ImGui::InputText("Password", state.userPassword, 64, ImGuiInputTextFlags_Password);
                if (ImGui::Button("Login")) {
                    try {
                        if (!auth.login(state.userEmail, state.userPassword)) {
                            state.errorMessage = "Incorrect credentials";
                            state.showErrorModal = true;
                        }
                    }
                    catch (const std::exception& e) {
                        state.errorMessage = e.what();
                        state.showErrorModal = true;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Register")) {
                    state.curr_page = register_page;
                }
                ImGui::End();
            }
            if (state.curr_page == register_page) {
                ImGui::Begin("Register Page", NULL, PARENT_FLAGS);
                ImGui::InputText("First Name", state.userFirstName, 50);
                ImGui::InputText("Last Name", state.userLastName, 50);
                ImGui::InputText("Email", state.userEmail, 100);
                ImGui::InputText("Password", state.userPassword, 64);
                if (ImGui::Button("Register")) {
                    try {
                        if (!auth.registerUser(state.userEmail, state.userPassword, state.userFirstName, state.userLastName)) {
                            state.errorMessage = "Registration failed. Email might already be in use.";
                            state.showErrorModal = true;
                        }
                        else {
                            state.curr_page = login_page;
                        }
                    }
                    catch (const std::exception& e) {
                        state.errorMessage = e.what();
                        state.showErrorModal = true;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Login Page")) {
                    state.curr_page = login_page;
                }
                ImGui::End();
            }
        }
        else {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(DisplaySize.x, DisplaySize.y));
            ImGui::Begin("Welcome Page", NULL, PARENT_FLAGS);
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
            ImGui::End();
        }
    }

    void render(ImVec2 DisplaySize) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(DisplaySize.x, DisplaySize.y));
        this->renderLoginRegister(DisplaySize);
        this->ShowErrorModal(state);
    }
};
