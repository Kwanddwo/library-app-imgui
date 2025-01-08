#pragma once
#include "Page.hpp"
#include "Models.hpp"

struct BookEditePageState : public PageState {
    Book book;
    BookEditePageState(Book book) : book(book) {};
};

class BookEditeFormPage : public Page {
    BookDAO& bookDB;
    Book book;
    int bookId = -1; 
    char isbn[20] = "";
    char title[100] = "";
    int pubYear = 0;
    int numAvailableCopies = 0;
    int nbrPages = 0;
    int languageId = 0;
    int editorId = 0;
    std::vector<Author> authors;
    std::vector<Category> categories;
    std::vector<Category> genres;
    std::function<void()> onEdit;

public:
    BookEditeFormPage(BookDAO& bookDB, std::function<void()> onEdit,Book book) : bookDB(bookDB),onEdit(onEdit),book(book) {
        strncpy_s(title, "Edit Book Info", sizeof(title));
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        ImGui::Text("Edit Book Information");

        // Input fields for book details
        ImGui::InputText("ISBN", isbn, sizeof(isbn));
        ImGui::InputText("Title", title, sizeof(title));
        ImGui::InputInt("Publication Year", &pubYear);
        ImGui::InputInt("Number of Available Copies", &numAvailableCopies);
        ImGui::InputInt("Number of Pages", &nbrPages);
        ImGui::InputInt("Language ID", &languageId);
        ImGui::InputInt("Editor ID", &editorId);

        // Update button
        if (ImGui::Button("Update Book")) {
            try {
                bookDB.updateBook(book.getId(), std::string(isbn), std::string(title), pubYear, numAvailableCopies, nbrPages, languageId, editorId);
                onEdit();
            }
            catch (const std::exception& e) {
                ImGui::Text("Error updating book: %s", e.what());
            }
        }

    }
};