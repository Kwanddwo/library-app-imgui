#pragma once
#include "Page.hpp"
#include "Authentification.hpp"


//std::string isbn;
//std::string title;
//int pubYear;
//int numAvailableCopies;
//int nbrPages;
//Language language;
//Editor editor;
//std::vector<Author> authors;
//std::vector<Category> categories;
//std::vector<Category> genres;
class BooksPage : public Page {
    Auth auth;
	vector<Book> books;
	BookDAO& bookDB;
    Borrowing borrow;
    BorrowingDAO borrowDB;

	void setBooks() {
		books = bookDB.findAllBooks();
	}

public:
    BooksPage(BookDAO& bookDB , Auth auth,BorrowingDAO borrowDB): bookDB(bookDB), auth(auth), borrowDB(borrowDB) {
		strncpy_s(title, "Books", sizeof(title));
		setBooks();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Books List", 11, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("ISBN", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Number of pages", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Available copies", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Language", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Author(s)", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Genres", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Categories", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            for (auto& book : books)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%d", book.getId());
                ImGui::TableNextColumn();
                ImGui::Text(book.getIsbn().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(book.getTitle().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%d", book.getNbrPages());
                ImGui::TableNextColumn();
                ImGui::Text("%d", book.getNumAvailableCopies());
                ImGui::TableNextColumn();
                ImGui::Text(book.getLanguage().getName().c_str());
                ImGui::TableNextColumn();
                ImGui::Text(book.getEditor().getName().c_str());

                ImGui::TableNextColumn();
                std::string authorsCol = "";
                for (auto& author : book.getAuthors()) {
					authorsCol += author.getFullName() + ", ";
                }
                ImGui::Text(authorsCol.c_str());

                std::string categoriesCol = "";
                std::string genresCol = "";

                for (auto& category : book.getCategories()) {
						categoriesCol += category.getName() + ", ";
                }
                for (auto& genre : book.getGenres()) {
                    genresCol += genre.getName() + ", ";
                }
                ImGui::TableNextColumn();
                ImGui::Text(genresCol.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(categoriesCol.c_str());
                if (auth.getCurrUser().canReserveBooks() && book.getNumAvailableCopies()>0) {
                    ImGui::TableNextColumn();
                    std::string buttonLabel = "reserve##" + std::to_string(book.getId());
                    if (ImGui::Button(buttonLabel.c_str())) {
                        book.setNumAvailableCopies(book.getNumAvailableCopies() - 1)  ;
                        bookDB.updateBook(book.getId(), book.getIsbn(), book.getTitle(), book.getPubYear(), book.getNumAvailableCopies(), book.getNbrPages(), book.getLanguage().getId(), book.getEditor().getId());
                        borrow.setClient(auth.getCurrUser());
                        borrow.setStatus("reserved");
                        borrow.setId(rand() % 10000);
                        borrow.setBook(book);
                        const string& a = NULL;
                        borrow.setDateBorrowed(a);
                        borrow.setDateIntendedReturn(a);
                        borrow.setDateActualReturn(a);
                        borrowDB.insertBorrowing(borrow.getDateBorrowed(), borrow.getDateIntendedReturn(), borrow.getDateActualReturn(), borrow.getStatus(), borrow.getClient().getId(), 1, borrow.getBook().getId());
                    }
                }
                
            }
            ImGui::EndTable();
        }
    }
};