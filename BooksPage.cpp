#include "BooksPage.hpp"
#include "imgui.h"

BooksPage::BooksPage(BookDAO& bookDB, Auth& auth, BorrowingDAO& borrowDB, CategoryDAO& categoryDB, LanguageDAO& languageDB, EditorDAO& editorDB, AuthorDAO& authorDB, std::function<void(Book)> onEdit) :
    bookDB(bookDB), auth(auth), borrowDB(borrowDB), onEdit(onEdit), categoryDB(categoryDB), languageDB(languageDB), editorDB(editorDB), authorDB(authorDB) {
    strncpy_s(title, "Books", sizeof(title));
    setBooks();
    setAuthors();
    setCategories();
    setGenres();
    setEditors();
    setLanguages();
}

void BooksPage::clearFilter() {
    filteredBooks = books;
}

vector<Book> BooksPage::filterBooks(int radio, string search_input) {
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
            for (auto& categ : book.getCategories())
                if (categ.getType() == "genre" && filterSearch(categ.getName(), search_input)) {
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

void BooksPage::render(AppState& state, std::shared_ptr<PageState> pageState) {
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

    static int e = 0;
    ImGui::RadioButton("Title##r", &e, 0); ImGui::SameLine();
    ImGui::RadioButton("Genre", &e, 1); ImGui::SameLine();
    ImGui::RadioButton("Year", &e, 2); ImGui::SameLine();
    ImGui::RadioButton("Author", &e, 3); ImGui::SameLine();
    if (ImGui::Button("Reset")) { clearFilter(); }; ImGui::SameLine();

    static char search_input[128] = "";
    ImGui::InputTextWithHint("", "enter text here", search_input, IM_ARRAYSIZE(search_input)); ImGui::SameLine();

    if (ImGui::Button("Search")) {
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
            if (auth.getCurrUser().canReserveBooks() && book.getNumAvailableCopies() > 0) {
                ImGui::TableNextColumn();
                std::string buttonLabel = "Reserve##" + std::to_string(book.getId());
                if (ImGui::Button(buttonLabel.c_str())) {
                    book.setNumAvailableCopies(book.getNumAvailableCopies() - 1);
                    bookDB.updateBook(book.getId(), book.getIsbn(), book.getTitle(), book.getPubYear(), book.getNumAvailableCopies(), book.getNbrPages(), book.getLanguage().getId(), book.getEditor().getId());

                    Borrowing borrow;
                    borrow.setClient(auth.getCurrUser());
                    borrow.setStatus("reserved");

                    borrow.setBook(book);
                    borrow.setDateBorrowed("null");
                    borrow.setDateIntendedReturn("null");
                    borrow.setDateActualReturn("null");
                    borrowDB.insertBorrowing(borrow.getDateBorrowed(), borrow.getDateIntendedReturn(), borrow.getDateActualReturn(), borrow.getStatus(), borrow.getClient().getId(), -1, borrow.getBook().getId());
                }
            }
            if (auth.getCurrUser().canManageBooks()) {
                ImGui::TableNextColumn();
                std::string buttonLabel2 = "Delete##" + std::to_string(book.getId());
                std::string buttonLabel3 = "Edit##" + std::to_string(book.getId());
                if (ImGui::Button(buttonLabel2.c_str())) {
                    bookDB.deleteBook(book.getId());
                    setBooks();
                }
                ImGui::SameLine();
                if (ImGui::Button(buttonLabel3.c_str())) {
                    onEdit(book);
                }
            }
        }

        ImGui::EndTable();
    }

    if (auth.getCurrUser().canManageBooks()) {
        if (ImGui::CollapsingHeader("Add Books")) {
            this->renderBookForm();
        }
        if (ImGui::CollapsingHeader("Add Categories/Genres")) {
            this->renderCategoryForm();
        }
        if (ImGui::CollapsingHeader("Add Editors")) {
            this->renderEditorForm();
        }
        if (ImGui::CollapsingHeader("Add Authors")) {
            this->renderAuthorForm();
        }
        if (ImGui::CollapsingHeader("Add Languages")) {
            this->renderLanguageForm();
        }
    }
}

void BooksPage::renderBookForm() {
    static char isbn[20] = "";
    static char bookTitle[100] = "";
    static int pubYear = 0;
    static int numAvailableCopies = 0;
    static int nbrPages = 0;
    static int selectedLanguageId = 0;
    static int selectedEditorId = 0;
    static std::vector<int> selectedAuthorIds;
    static std::vector<int> selectedCategoryIds;
    static std::vector<int> selectedGenreIds;

    ImGui::InputText("ISBN", isbn, sizeof(isbn));
    ImGui::InputText("Title##form", bookTitle, sizeof(bookTitle));
    ImGui::InputInt("Publication Year", &pubYear);
    ImGui::InputInt("Number of Available Copies", &numAvailableCopies);
    ImGui::InputInt("Number of Pages", &nbrPages);

    // Dropdown for selecting language
    if (ImGui::BeginCombo("Language##form", selectedLanguageId == 0 ? "Select Language" : languageDB.findLanguageById(selectedLanguageId).getName().c_str())) {
        for (auto& language : languages) {
            bool isSelected = (language.getId() == selectedLanguageId);
            if (ImGui::Selectable(language.getName().c_str(), isSelected)) {
                //ImGui::SameLine();
                std::string deleteButtonLabel = "Delete##Language" + std::to_string(language.getId());
                if (ImGui::Button(deleteButtonLabel.c_str())) {
                    languageDB.deleteLanguage(language.getId());
                    setLanguages();
                }
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
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete##Editor" + std::to_string(editor.getId());
            if (ImGui::Button(deleteButtonLabel.c_str())) {
                editorDB.deleteEditor(editor.getId());
                setEditors();
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
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete##Author" + std::to_string(author.getId());
            if (ImGui::Button(deleteButtonLabel.c_str())) {
                authorDB.deleteAuthor(author.getId());
                setAuthors();
            }
        }
        ImGui::EndListBox();
    }

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
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete##Category" + std::to_string(category.getId());
            if (ImGui::Button(deleteButtonLabel.c_str())) {
                categoryDB.deleteCategory(category.getId());
                setCategories();
                setGenres();
            }
        }
        ImGui::EndListBox();
    }

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
            ImGui::SameLine();
            std::string deleteButtonLabel = "Delete##Genre" + std::to_string(genre.getId());
            if (ImGui::Button(deleteButtonLabel.c_str())) {
                categoryDB.deleteCategory(genre.getId());
                setGenres();
            }
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Add Book")) {
        try {
            bookDB.insertBook(
                std::string(isbn),
                std::string(bookTitle),
                pubYear,
                numAvailableCopies,
                nbrPages,
                selectedLanguageId,
                selectedEditorId
            );
            int bookId = bookDB.findBookByIsbn(std::string(isbn)).getId();
            bookDB.addAuthorsToBook(bookId, selectedAuthorIds);
            bookDB.addCategoriesToBook(bookId, selectedCategoryIds);
            bookDB.addCategoriesToBook(bookId, selectedGenreIds);
            setBooks();
        }
        catch (const std::exception& e) {
            ImGui::Text("Error adding book: %s", e.what());
        }
    }
}

void BooksPage::renderAuthorForm() {
    static char firstName[128] = "";
    static char lastName[128] = "";
    static char dateOfBirth[128] = "";

    ImGui::InputText("First Name", firstName, sizeof(firstName));
    ImGui::InputText("Last Name", lastName, sizeof(lastName));
    ImGui::InputTextWithHint("Date of Birth", "YYYY-MM-DD", dateOfBirth, sizeof(dateOfBirth));

    if (ImGui::Button("Add Author")) {
        try {
            authorDB.insertAuthor(
                std::string(firstName),
                std::string(lastName),
                std::string(dateOfBirth)
            );
            setAuthors();
        }
        catch (const std::exception& e) {
            ImGui::Text("Error adding author: %s", e.what());
        }
    }
}

void BooksPage::renderEditorForm() {
    static char editorName[128] = "";

    ImGui::InputText("Editor Name", editorName, sizeof(editorName));

    if (ImGui::Button("Add Editor")) {
        try {
            editorDB.insertEditor(std::string(editorName));
            setEditors();
        }
        catch (const std::exception& e) {
            ImGui::Text("Error adding editor: %s", e.what());
        }
    }
}

void BooksPage::renderLanguageForm() {
    static char languageName[128] = "";
    static char languageCode[3] = "";

    ImGui::InputTextWithHint("Language Name", "English", languageName, sizeof(languageName));
    ImGui::InputTextWithHint("Language Code", "EN", languageCode, sizeof(languageCode));

    if (ImGui::Button("Add Language")) {
        try {
            languageDB.insertLanguage(std::string(languageName), languageCode);
            setLanguages();
        }
        catch (const std::exception& e) {
            ImGui::Text("Error adding language: %s", e.what());
        }
    }
}

void BooksPage::renderCategoryForm() {
    static char categoryName[128] = "";
    static char categoryType[10] = "";

    ImGui::InputText("Category Name", categoryName, sizeof(categoryName));
    
    const char* categoryTypes[] = { "category", "genre" };
    static int selectedCategoryType = 0;
    ImGui::Combo("Category Type", &selectedCategoryType, categoryTypes, IM_ARRAYSIZE(categoryTypes));
    strncpy_s(categoryType, categoryTypes[selectedCategoryType], sizeof(categoryType));

    if (ImGui::Button("Add Category")) {
        try {
            categoryDB.insertCategory(std::string(categoryName), std::string(categoryType));
            setCategories();
            setGenres();
        }
        catch (const std::exception& e) {
            ImGui::Text("Error adding category: %s", e.what());
        }
    }
}

void BooksPage::setBooks() {
    books = bookDB.findAllBooks();
    filteredBooks = books;
}

void BooksPage::setCategories() {
    categories = categoryDB.findAllCategories("category");
}

void BooksPage::setGenres() {
    genres = categoryDB.findAllCategories("genre");
}

void BooksPage::setLanguages() {
    languages = languageDB.findAllLanguages();
}

void BooksPage::setEditors() {
    editors = editorDB.findAllEditors();
}

void BooksPage::setAuthors() {
    authors = authorDB.findAllAuthors();
}

