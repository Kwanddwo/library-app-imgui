#pragma once

#include "imgui.h"
#include "Authentification.hpp"
#include <memory>

#include "LoginPage.hpp"
#include "RegisterPage.hpp"

// Anyone who is logged in
#include "BooksPage.hpp"
#include "ProfilePage.hpp"

// Member pages
#include "BorrowingsHistoryPage.hpp"
#include "BorrowFormPage.hpp"

// Admin & librarian pages
#include "BorrowingsPage.hpp"
#include "MembersPage.hpp"

// Admin pages
#include "LibrariansPage.hpp"
#include "StatisticsPage.hpp"

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

        if (auth.getCurrUser().canViewBooks()) {
            ImGui::SameLine();
            if (ImGui::Button("Books")) {
                auth.logout();
                this->setPage(PageType::Books, nullptr);
            }
        }

		if (auth.getCurrUser().canReserveBooks()) {
			ImGui::SameLine();
			if (ImGui::Button("Borrowings History")) {
				this->setPage(PageType::BorrowingsHistory, nullptr);
			}
			ImGui::SameLine();
			if (ImGui::Button("Borrow Form")) {
				this->setPage(PageType::BorrowForm, nullptr);
			}
		}

		if (auth.getCurrUser().canManageMembers()) {
            ImGui::SameLine();
            if (ImGui::Button("Borrowings")) {
                this->setPage(PageType::Borrowings, nullptr);
            }
			ImGui::SameLine();
			if (ImGui::Button("Members")) {
				this->setPage(PageType::Members, nullptr);
			}
			
		}

        if (auth.getCurrUser().canManageAllUsers()) {
            ImGui::SameLine();
            if (ImGui::Button("Librarians")) {
                this->setPage(PageType::Librarians, nullptr);
            }
        }

        if (auth.getCurrUser().canViewStatistics()) {
            ImGui::SameLine();
            if (ImGui::Button("Statistics")) {
                this->setPage(PageType::Statistics, nullptr);
            }
        }

		ImGui::SameLine();
		if (ImGui::Button("Profile")) {
			this->setPage(PageType::Profile, nullptr);
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
                [this]() { this->setPage(PageType::Books, nullptr); }
            );
            break;
        case PageType::Register:
            currentPage = std::make_unique<RegisterPage>(
                auth,
                [this](std::shared_ptr<PageState> state) { this->setPage(PageType::Login, state); }
            );
            break;
        case PageType::Profile:
            currentPage = std::make_unique<ProfilePage>(auth);
            break;
        case PageType::Books:
            currentPage = std::make_unique<BooksPage>();
            break;


        case PageType::BorrowForm:
            currentPage = std::make_unique<BorrowFormPage>();
            break;
        case PageType::BorrowingsHistory:
            currentPage = std::make_unique<BorrowingsHistoryPage>();
            break;


		case PageType::Borrowings:
			currentPage = std::make_unique<BorrowingsPage>();
			break;
		case PageType::Members:
			currentPage = std::make_unique<MembersPage>();
			break;


		case PageType::Librarians:
            currentPage = std::make_unique<LibrariansPage>();
			break;
		case PageType::Statistics:
			currentPage = std::make_unique<StatisticsPage>();
			break;
        }
        
    }

    void render(ImVec2 DisplaySize) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(DisplaySize.x, DisplaySize.y));
        ImGui::Begin(currentPage->title, NULL, PARENT_FLAGS);
        
        if (currentPageType > PageType::Register) {
            this->renderNavBar();
        }
        currentPage->render(state, currentPageState);
		
        ImGui::End();
        ShowErrorModal();
    }
};