#pragma once
#include "Page.hpp"

class BorrowingsHistoryPage : public Page {
    BorrowingDAO& borrowingDB;
    Auth& auth;
    User currMember;
    std::vector<Borrowing> borrowings;

    void setBorrowings() {
        borrowings = borrowingDB.getMemberBorrowings(currMember.getId());
    }
public:
    BorrowingsHistoryPage(BorrowingDAO& borrowingDB, Auth& auth) : borrowingDB(borrowingDB), auth(auth) {
        currMember = auth.getCurrUser();
        strncpy_s(title, "Borrowings History", sizeof(title));
        setBorrowings();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Borrowings History List", 6, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Date Borrowed", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Intended Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actual Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Book Title", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& borrowing : borrowings)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", borrowing.getId());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getDateBorrowed().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getDateIntendedReturn().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getDateActualReturn().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getStatus().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getBook().getTitle().c_str());
            }
            ImGui::EndTable();
        }
    }
};