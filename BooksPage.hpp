#pragma once
#include "Page.hpp"

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
	vector<Book> books;
	BookDAO& bookDB;

	void setBooks() {
		books = bookDB.findAllBooks();
	}

public:
    BooksPage(BookDAO& bookDB): bookDB(bookDB) {
		strncpy_s(title, "Books", sizeof(title));
		setBooks();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
        if (ImGui::BeginTable("Books List", 10, flags, outer_size))
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
                    if (category.getType() == "genre") {
						genresCol += category.getName() + ", ";
                    }
                    else {
						categoriesCol += category.getName() + ", ";
                    }
                }
                ImGui::TableNextColumn();
                ImGui::Text(genresCol.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(categoriesCol.c_str());
            }
            ImGui::EndTable();
        }
    }
};