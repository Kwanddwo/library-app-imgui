#pragma once
#include "Page.hpp"
#include "Models.hpp"

struct BookEditePageState : public PageState {
};

class BookEditeFormPage : public Page {
    BookDAO& bookDB;
    CategoryDAO& categoryDB;
    LanguageDAO& languageDB;
    EditorDAO& editorDB;
    AuthorDAO& authorDB;

    vector<Category> categories;
    vector<Category> genres;
    vector<Language> languages;
    vector<Editor> editors;
    vector<Author> authors;
    Book book;

    char isbn[20] = "";
    char bookTitle[100] = "";
    int pubYear = 0;
    int numAvailableCopies = 0;
    int nbrPages = 0;
    int selectedLanguageId = 0;
    int selectedEditorId = 0;
    std::vector<int> selectedAuthorIds;
    std::vector<int> selectedCategoryIds;
    std::vector<int> selectedGenreIds;
    std::function<void()> onEdit;

    void setCategories() {
        categories = categoryDB.findAllCategories("category");
    }
    void setGenres() {
        genres = categoryDB.findAllCategories("genre");
    }
    void setLanguages() {
        languages = languageDB.findAllLanguages();
    }
    void setEditors() {
        editors = editorDB.findAllEditors();
    }
    void setAuthors() {
        authors = authorDB.findAllAuthors();
    }

public:
    BookEditeFormPage(BookDAO& bookDB, std::function<void()> onEdit, Book book, CategoryDAO& categoryDB, LanguageDAO& languageDB, EditorDAO& editorDB, AuthorDAO& authorDB) :
        bookDB(bookDB), onEdit(onEdit), book(book), categoryDB(categoryDB), languageDB(languageDB), editorDB(editorDB), authorDB(authorDB) {
        strncpy_s(title, "Edit Book", sizeof(title));
        strncpy_s(bookTitle, book.getTitle().c_str(), sizeof(bookTitle));
        strncpy_s(isbn, book.getIsbn().c_str(), sizeof(isbn));
        pubYear = book.getPubYear();
        numAvailableCopies = book.getNumAvailableCopies();
        nbrPages = book.getNbrPages();
        selectedLanguageId = book.getLanguage().getId();
        selectedEditorId = book.getEditor().getId();
        for (const auto& author : book.getAuthors()) {
            selectedAuthorIds.push_back(author.getId());
        }
        for (const auto& category : book.getCategories()) {
            selectedCategoryIds.push_back(category.getId());
        }
        for (const auto& genre : book.getGenres()) {
            selectedGenreIds.push_back(genre.getId());
        }
        setCategories();
        setGenres();
        setLanguages();
        setEditors();
        setAuthors();
    }

    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        // Input fields for book details
        ImGui::InputText("ISBN", isbn, sizeof(isbn));
        ImGui::InputText("Title", bookTitle, sizeof(bookTitle));
        ImGui::InputInt("Publication Year", &pubYear);
        ImGui::InputInt("Number of Available Copies", &numAvailableCopies);
        ImGui::InputInt("Number of Pages", &nbrPages);

        // Dropdown for selecting language
        if (ImGui::BeginCombo("Language##form", selectedLanguageId == 0 ? "Select Language" : languageDB.findLanguageById(selectedLanguageId).getName().c_str())) {
            for (auto& language : languages) {
                bool isSelected = (language.getId() == selectedLanguageId);
                if (ImGui::Selectable(language.getName().c_str(), isSelected)) {
                    selectedLanguageId = language.getId();
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Dropdown for selecting editor
        if (ImGui::BeginCombo("Editor", selectedEditorId == 0 ? "Select Editor" : editorDB.findEditorById(selectedEditorId).getName().c_str())) {
            for (auto& editor : editors) {
                bool isSelected = (editor.getId() == selectedEditorId);
                if (ImGui::Selectable(editor.getName().c_str(), isSelected)) {
                    selectedEditorId = editor.getId();
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // Dropdown for selecting authors
        if (ImGui::BeginListBox("Authors")) {
            for (auto& author : authors) {
                bool isSelected = std::find(selectedAuthorIds.begin(), selectedAuthorIds.end(), author.getId()) != selectedAuthorIds.end();
                if (ImGui::Selectable(author.getFullName().c_str(), isSelected)) {
                    if (isSelected) {
                        selectedAuthorIds.erase(std::remove(selectedAuthorIds.begin(), selectedAuthorIds.end(), author.getId()), selectedAuthorIds.end());
                    }
                    else {
                        selectedAuthorIds.push_back(author.getId());
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        // Dropdown for selecting categories
        if (ImGui::BeginListBox("Categories")) {
            for (auto& category : categories) {
                bool isSelected = std::find(selectedCategoryIds.begin(), selectedCategoryIds.end(), category.getId()) != selectedCategoryIds.end();
                if (ImGui::Selectable(category.getName().c_str(), isSelected)) {
                    if (isSelected) {
                        selectedCategoryIds.erase(std::remove(selectedCategoryIds.begin(), selectedCategoryIds.end(), category.getId()), selectedCategoryIds.end());
                    }
                    else {
                        selectedCategoryIds.push_back(category.getId());
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        // Dropdown for selecting genres
        if (ImGui::BeginListBox("Genres")) {
            for (auto& genre : genres) {
                bool isSelected = std::find(selectedGenreIds.begin(), selectedGenreIds.end(), genre.getId()) != selectedGenreIds.end();
                if (ImGui::Selectable(genre.getName().c_str(), isSelected)) {
                    if (isSelected) {
                        selectedGenreIds.erase(std::remove(selectedGenreIds.begin(), selectedGenreIds.end(), genre.getId()), selectedGenreIds.end());
                    }
                    else {
                        selectedGenreIds.push_back(genre.getId());
                    }
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

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
                    selectedLanguageId,
                    selectedEditorId
                );

                // Clear existing associations before adding new ones
                bookDB.clearAuthorsFromBook(book.getId());
                bookDB.clearCategoriesFromBook(book.getId());

                bookDB.addAuthorsToBook(book.getId(), selectedAuthorIds);
                bookDB.addCategoriesToBook(book.getId(), selectedCategoryIds);
                bookDB.addCategoriesToBook(book.getId(), selectedGenreIds);
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
