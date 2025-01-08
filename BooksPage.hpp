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
    Auth& auth;
    BookDAO& bookDB;
    BorrowingDAO& borrowDB;
    CategoryDAO& categoryDB;
    LanguageDAO& languageDB;
    EditorDAO& editorDB;
    AuthorDAO& authorDB;

    vector<Book> books;
    vector<Book> filteredBooks;
    vector<Category> categories;
    vector<Category> genres;
    vector<Language> languages;
    vector<Editor> editors;
    vector<Author> authors;

    std::function<void(Book)> onEdit;

    void renderBookForm();
    void renderCategoryForm();
    void renderEditorForm();
    void renderAuthorForm();
    void renderLanguageForm();

    void setBooks();
    void setCategories();
    void setGenres();
    void setLanguages();
    void setEditors();
    void setAuthors();

public:
    BooksPage(BookDAO& bookDB, Auth& auth, BorrowingDAO& borrowDB, CategoryDAO& categoryDB, LanguageDAO& languageDB, EditorDAO& editorDB, AuthorDAO& authorDB, std::function<void(Book)> onEdit);

    void clearFilter();
    vector<Book> filterBooks(int radio, string search_input);
    void render(AppState& state, std::shared_ptr<PageState> pageState) override;
};
