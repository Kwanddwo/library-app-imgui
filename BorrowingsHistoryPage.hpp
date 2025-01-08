#pragma once
#include "Page.hpp"

class BorrowingsHistoryPage : public Page {
    BorrowingDAO& borrowingDB;
    BookDAO& bookDB;
    Auth& auth;
    User currMember;
    std::vector<Borrowing> borrowings;

    void setBorrowings() {
        borrowings = borrowingDB.getMemberBorrowings(currMember.getId());
    }

    void incrementAvailableCopies(Book book) {
        book.setNumAvailableCopies(book.getNumAvailableCopies() + 1);
        bookDB.updateBook(book.getId(), book.getIsbn(), book.getTitle(), book.getPubYear(), book.getNumAvailableCopies(), book.getNbrPages(), book.getLanguage().getId(), book.getEditor().getId());
    }

public:
    BorrowingsHistoryPage(BorrowingDAO& borrowingDB, Auth& auth, BookDAO& bookDB) : borrowingDB(borrowingDB), auth(auth), bookDB(bookDB) {
        currMember = auth.getCurrUser();
        strncpy_s(title, "Borrowings History", sizeof(title));
        setBorrowings();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 12);
        if (ImGui::BeginTable("Borrowings History List", 7, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Date Borrowed", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Intended Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actual Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Book Title", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_None);
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
                if (borrowing.getStatus() == "reserved") {
                    ImGui::TableNextColumn();
                    if (ImGui::Button( ("Cancel##" + std::to_string(borrowing.getId())).c_str() )) {
                        borrowingDB.updateBorrowingStatus(borrowing.getId(), "cancelled");
						incrementAvailableCopies(borrowing.getBook());
                        setBorrowings();
                    }
                }
            }
            ImGui::EndTable();
        }
    }
};