#pragma once
#include "Page.hpp"
#include "Models.hpp"

struct BookEditePageState : public PageState {
};

class BookEditeFormPage : public Page {
    BookDAO& bookDB;
    Book book;
    char isbn[20] = "";
    char bookTitle[100] = "";
    int pubYear = 0;
    int numAvailableCopies = 0;
    int nbrPages = 0;
    Language language;
    Editor editor;
    std::vector<Author> authors;
    std::vector<Category> categories;
    std::vector<Category> genres;
    std::function<void()> onEdit;

public:
    BookEditeFormPage(BookDAO& bookDB, std::function<void()> onEdit, Book book) :
        bookDB(bookDB), onEdit(onEdit), book(book) {
        strncpy_s(title, "Edit Book", sizeof(title));
        strncpy_s(bookTitle, book.getTitle().c_str(), sizeof(bookTitle));
        strncpy_s(isbn, book.getIsbn().c_str(), sizeof(isbn));
        pubYear = book.getPubYear();
        numAvailableCopies = book.getNumAvailableCopies();
        nbrPages = book.getNbrPages();
        language = book.getLanguage();
        editor = book.getEditor();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        // Input fields for book details
        ImGui::InputText("ISBN", isbn, sizeof(isbn));
        ImGui::InputText("Title", bookTitle, sizeof(title));
        ImGui::InputInt("Publication Year", &pubYear);
        ImGui::InputInt("Number of Available Copies", &numAvailableCopies);
        ImGui::InputInt("Number of Pages", &nbrPages);
        // Change with selection option
        /*ImGui::InputInt("Language ID", &language);
        ImGui::InputInt("Editor ID", &editor);*/
		// Add selection option
		// Language
		// Editor
		// Add authors
		// Add categories

        // Update button
        if (ImGui::Button("Update Book")) {
            try {
                bookDB.updateBook(
                    book.getId(), 
                    std::string(isbn), 
                    std::string(bookTitle), 
                    pubYear, 
                    numAvailableCopies,
                    nbrPages, 
                    book.getLanguage().getId(), // Change this
					book.getEditor().getId() // Change this
                ); 
                onEdit();
            }
            catch (const std::exception& e) {
                ImGui::Text("Error updating book: %s", e.what());
            }
        }

		if (ImGui::Button("Cancel")) {
			onEdit();
		}

    }
};