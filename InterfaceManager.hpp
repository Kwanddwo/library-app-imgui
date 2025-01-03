#pragma once

#include "imgui.h"
#include "Authentification.hpp"
#include <memory>

#include "LoginPage.hpp"
#include "RegisterPage.hpp"
#include "WelcomePage.hpp"

#ifndef PARENT_FLAGS
#define PARENT_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
#endif

class InterfaceApp {
    Auth& auth;
    AppState state;
    PageType currentPageType;
    std::unique_ptr<Page> currentPage;
    std::shared_ptr<PageState> currentPageState;

    void ShowErrorModal() {
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

	void renderNavBar() {
		if (ImGui::Button("Logout")) {
			auth.logout();
			this->setPage(PageType::Login, std::make_shared<LoginPageState>());
		}
		ImGui::SameLine();
		if (ImGui::Button("Welcome")) {
			this->setPage(PageType::Welcome, nullptr);
		}
	}

public:
    InterfaceApp(Auth& auth) : auth(auth) {
        this->setPage(PageType::Login, std::make_shared<LoginPageState>());
    }

    void setPage(PageType pageType, std::shared_ptr<PageState> pageState) {
        currentPageType = pageType;
        currentPageState = pageState;
        switch (pageType) {
        case PageType::Login:
            currentPage = std::make_unique<LoginPage>(
                auth,
                [this](std::shared_ptr<PageState> state) { this->setPage(PageType::Register, std::make_shared<RegisterPageState>()); },
                [this]() { this->setPage(PageType::Welcome, nullptr); }
            );
            break;
        case PageType::Register:
            currentPage = std::make_unique<RegisterPage>(
                auth,
                [this](std::shared_ptr<PageState> state) { this->setPage(PageType::Login, state); }
            );
            break;
        case PageType::Welcome:
            currentPage = std::make_unique<WelcomePage>(auth);
            break;
        }
    }

    void render(ImVec2 DisplaySize) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(DisplaySize.x, DisplaySize.y));
        ImGui::Begin(currentPage->title, NULL, PARENT_FLAGS);
        
        if (currentPageType >= PageType::Welcome) {
            this->renderNavBar();
        }
        currentPage->render(state, currentPageState);
		
        ImGui::End();
        ShowErrorModal();
    }
};