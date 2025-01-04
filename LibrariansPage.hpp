#pragma once
#include "Page.hpp"

struct LibrariansPageState : public PageState {
    
};

static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | 
    ImGuiTableFlags_RowBg | 
    ImGuiTableFlags_BordersOuter | 
    ImGuiTableFlags_BordersV | 
    ImGuiTableFlags_Resizable | 
    ImGuiTableFlags_Reorderable | 
    ImGuiTableFlags_Hideable;

class LibrariansPage : public Page {
    vector<User> librarians;
	UserDAO& userDB;

    void setLibrarians() {
        vector<User> users = userDB.findAllUsers();
        for (auto& user : users) {
            if (user.getRole() == UserRole::LIBRARIAN) {
                librarians.push_back(user);
            }
        }
    }
public:
    LibrariansPage(UserDAO& userDB): userDB(userDB) {
        strncpy_s(title, "Librarians", sizeof(title));
        setLibrarians();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Librarians List", 4, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("First Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Last Name", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& librarian : librarians)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", librarian.getId());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getEmail().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getFirstName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(librarian.getLastName().c_str());
            }
            ImGui::EndTable();
        }
    }
};