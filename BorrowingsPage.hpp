#pragma once
#include "Page.hpp"

struct BorrowingsPageState : public PageState {

};

class BorrowingsPage : public Page {
    std::vector<Borrowing> borrowings;
    BorrowingDAO& borrowingDB;
	BookDAO& bookDB;

    void setBorrowings() {
        borrowings = borrowingDB.findAllBorrowings();
    }

    void incrementAvailableCopies(Book book) {
        book.setNumAvailableCopies(book.getNumAvailableCopies() + 1);
        bookDB.updateBook(book.getId(), book.getIsbn(), book.getTitle(), book.getPubYear(), book.getNumAvailableCopies(), book.getNbrPages(), book.getLanguage().getId(), book.getEditor().getId());
    }

    void verifyReservation(Borrowing b) {
        borrowingDB.updateBorrowingStatus(b.getId(), "not returned");
        setBorrowings();
    }
    void cancelBorrowing(Borrowing b) {
		incrementAvailableCopies(b.getBook());
        borrowingDB.updateBorrowingStatus(b.getId(), "cancelled");
        setBorrowings();
    }
    void verifyReturn(Borrowing b) {
		incrementAvailableCopies(b.getBook());
        borrowingDB.updateBorrowingStatus(b.getId(), "returned");
        setBorrowings();
    }
public:
    BorrowingsPage(BorrowingDAO& borrowingDB, BookDAO& bookDB) : borrowingDB(borrowingDB), bookDB(bookDB) {
        strncpy_s(title, "Borrowings", sizeof(title));
        setBorrowings();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Borrowings List", 8, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Date Borrowed", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Intended Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actual Return Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Client", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Book", ImGuiTableColumnFlags_None);
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
                ImGui::Text(borrowing.getClient().getFullName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(borrowing.getBook().getTitle().c_str());
                ImGui::TableNextColumn();
                if (borrowing.getStatus() == "reserved") {
                    if (ImGui::Button(("Verify##" + std::to_string(borrowing.getId())).c_str()))
                        verifyReservation(borrowing);
                    ImGui::SameLine();
                    if (ImGui::Button(("Cancel##" + std::to_string(borrowing.getId())).c_str()))
                        cancelBorrowing(borrowing);
                }
                if (borrowing.getStatus() == "not returned") {
                    if (ImGui::Button(("Mark Returned##" + std::to_string(borrowing.getId())).c_str()))
                        verifyReturn(borrowing);
                }
            }
            ImGui::EndTable();
        }
    }
};
