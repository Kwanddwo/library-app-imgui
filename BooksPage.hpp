#pragma once
#include "Page.hpp"
#include "Authentification.hpp"

static string stringToLower(string s) {
    string str = s;
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

static bool filterSearch(string entry, string search) {
    return stringToLower(entry).find(stringToLower(search)) != string::npos ? true : false;
}

class BooksPage : public Page {
    Auth auth;
	vector<Book> books;
    vector<Book> filteredBooks;
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
        filteredBooks = books;
    }

    void clearFilter() {
        filteredBooks = books;
    }

    vector<Book> filterBooks(int radio, string search_input) {
        filteredBooks.clear();
        if (radio == 0) {
            for (auto& book : books) {
                if (filterSearch(book.getTitle(), search_input)) {
                    filteredBooks.push_back(book);
                }
            }  
        }
        if (radio == 1) {
            for (auto& book : books) {
                for(auto& categ : book.getCategories())
                if (categ.getType() =="genre" && filterSearch(categ.getName(), search_input)) {
                    filteredBooks.push_back(book);
                }
            }
        }
        if (radio == 2) {
            for (auto& book : books) {
                if (to_string(book.getPubYear()) == search_input) {
                    filteredBooks.push_back(book);
                }
            }
        }
        if (radio == 3) {
            for (auto& book : books) {
                for (auto& author : book.getAuthors())
                    if (filterSearch(author.getFullName(), search_input)) {
                        filteredBooks.push_back(book);
                    }
            }
        }
        return filteredBooks;
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

        static int e = 0;
        ImGui::RadioButton("Title", &e, 0); ImGui::SameLine();
        ImGui::RadioButton("Genre", &e, 1); ImGui::SameLine();
        ImGui::RadioButton("Year", &e, 2);ImGui::SameLine();
        ImGui::RadioButton("Author", &e, 3);ImGui::SameLine();
        if (ImGui::Button("Reset")) { clearFilter(); };ImGui::SameLine();

        static char search_input[128] = "";
        ImGui::InputTextWithHint("", "enter text here", search_input, IM_ARRAYSIZE(search_input));ImGui::SameLine();

        if (ImGui::Button("search")) {
            ImGui::Text("button clicked");
            filteredBooks = filterBooks(e, search_input);
            
        }
            if (ImGui::BeginTable("Books List", 12, flags, outer_size))
            {
                ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("ISBN", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Pub Year", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Number of pages", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Available copies", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Language", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Editor", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Author(s)", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Genres", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Categories", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                for (auto& book : filteredBooks)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", book.getId());
                    ImGui::TableNextColumn();
                    ImGui::Text(book.getIsbn().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text(book.getTitle().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", book.getPubYear());
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
                        borrow.setDateBorrowed("1970-01-01");
                        borrow.setDateIntendedReturn("1970-01-01");
                        borrow.setDateActualReturn("1970-01-01");
                        borrowDB.insertBorrowing(borrow.getDateBorrowed(), borrow.getDateIntendedReturn(), borrow.getDateActualReturn(), borrow.getStatus(), borrow.getClient().getId(), 1, borrow.getBook().getId());
                    }
                }
                
            }
            ImGui::EndTable();
        }
    }
};