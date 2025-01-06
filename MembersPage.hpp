#pragma once
#include "Page.hpp"

struct MembersPageState : public PageState {

};

class MembersPage : public Page {
    vector<User> members;
    UserDAO& userDB;
    std::function<void(User)> onEdit;

    void setMembers() {
        vector<User> users = userDB.findAllUsers();
        for (auto& user : users) {
            if (user.getRole() == UserRole::MEMBER) {
                members.push_back(user);
            }
        }
    }

    void deleteMember(User member) {
        userDB.deleteUser(member.getId());
        setMembers();
    }


public:
    MembersPage(UserDAO& userDB, std::function<void(User)> onEdit) : userDB(userDB), onEdit(onEdit) {
        strncpy_s(title, "Members", sizeof(title));
        setMembers();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Members List", 5, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("First Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Last Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& member : members)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", member.getId());
                ImGui::TableNextColumn();
                ImGui::Text(member.getEmail().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(member.getFirstName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(member.getLastName().c_str());
                ImGui::TableNextColumn();
                if (ImGui::Button(("Delete##" + std::to_string(member.getId())).c_str()))
                    deleteMember(member);
                ImGui::SameLine();
                if (ImGui::Button(("Edit##" + std::to_string(member.getId())).c_str()))
                    onEdit(member);
            }
            ImGui::EndTable();
        }
    }
};
