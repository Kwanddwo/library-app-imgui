#pragma once

#include "imgui.h"
#include "Authentification.hpp"

#ifndef PARENT_FLAGS
#define PARENT_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
#endif

typedef enum { login_page, register_page } page;

// State machines for interfaces? Think about later
//class Interface {
//public:
//    virtual void render() = 0;
//};

struct AppState {
    page curr_page;

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
    Auth auth;
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
    InterfaceApp(Auth& auth): auth(auth){}

    void renderLoginRegister(ImVec2 DisplaySize) {
        if (!auth.isLoggedIn) {
            if (state.curr_page == login_page) {
                ImGui::Begin("Login Page", NULL, PARENT_FLAGS);
                ImGui::InputText("Email", state.userEmail, 100);
                ImGui::InputText("Password", state.userPassword, 50, ImGuiInputTextFlags_Password);
                if (ImGui::Button("Login")) {
                    if (!auth.login(state.userEmail, state.userPassword)) {
                        state.errorMessage = "Incorrect credentials";
                        state.showErrorModal = true;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Register"))
                    state.curr_page = register_page;
               /* if (is_login_error)
                    ImGui::Text("Error: Credentials are incorrect");*/
                ImGui::End();
            }
            if (state.curr_page == register_page) {
                ImGui::Begin("Register Page", NULL, PARENT_FLAGS);
                ImGui::InputText("First Name", state.userFirstName, 100);
                ImGui::InputText("Last Name", state.userLastName, 100);
                ImGui::InputText("Email", state.userEmail, 100);
                ImGui::InputText("Password", state.userPassword, 50);
                if (ImGui::Button("Register")) {
                    auth.registerUser(
                        state.userEmail, 
                        state.userPassword, 
                        state.userFirstName, 
                        state.userLastName
                    );
                    state.curr_page = login_page;
                }
                ImGui::SameLine();
                if (ImGui::Button("Login Page")) {
                    state.curr_page = login_page;
                }
                ImGui::End();
            }
        }
        if (auth.isLoggedIn) {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(
                DisplaySize.x, DisplaySize.y
            ));
            ImGui::Begin("Welcome Page", NULL, PARENT_FLAGS);
            ImGui::Text("Hello!");
            ImGui::Text("First name: ");
            ImGui::SameLine();
            ImGui::Text("%s", auth.currUser.getFirstName().c_str());
            ImGui::Text("Last name: ");
            ImGui::SameLine();
            ImGui::Text("%s", auth.currUser.getLastName().c_str());
            ImGui::Text("Email: ");
            ImGui::SameLine();
            ImGui::Text("%s", auth.currUser.getEmail().c_str());
            ImGui::Text("Role: ");
            ImGui::SameLine();
            ImGui::Text("%s", User::userRoleToString(auth.currUser.getRole()).c_str());
            ImGui::End();
        }

    }
    void render(ImVec2 DisplaySize) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(
            DisplaySize.x, DisplaySize.y
        ));
        //ImGui::ShowDemoWindow(&show_demo_window);
        this->renderLoginRegister(DisplaySize);
        this->ShowErrorModal(state);
    }
};