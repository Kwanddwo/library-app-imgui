#pragma once
#include "Page.hpp"

struct LibrariansPageState : public PageState {
    
};

class LibrariansPage : public Page {
    vector<User> librarians;
	UserDAO& userDB;
    std::function<void()> onCreate;
    std::function<void(User)> onEdit;

    void setLibrarians() {
        vector<User> users = userDB.findAllUsers();
        for (auto& user : users) {
            if (user.getRole() == UserRole::LIBRARIAN) {
                librarians.push_back(user);
            }
        }
    }

    void deleteLibrarian(User librarian) {
        userDB.deleteUser(librarian.getId());
        setLibrarians();
    }

public:
    LibrariansPage(UserDAO& userDB, std::function<void(User)> onEdit, std::function<void()> onCreate): 
        userDB(userDB), onEdit(onEdit), onCreate(onCreate) {
        strncpy_s(title, "Librarians", sizeof(title));
        setLibrarians();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Librarians List", 5, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("First Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Last Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& librarian : librarians) {

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", librarian.getId());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getEmail().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getFirstName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getLastName().c_str());
                ImGui::TableNextColumn();
                if (ImGui::Button(("Delete##" + std::to_string(librarian.getId())).c_str()))
                    deleteLibrarian(librarian);
                ImGui::SameLine();
                if (ImGui::Button(("Edit##" + std::to_string(librarian.getId())).c_str()))
                    onEdit(librarian);
            }
            ImGui::EndTable();
        }

		if (ImGui::Button("Add Librarian")) {
            onCreate();
		}
    }
};