/*#pragma once

#include "Page.hpp"
#include <memory>
#include <string>
#include <imgui.h>
#include "Database.hpp"

struct StatisticsPageState : public PageState {
    std::string timeRange = "past month";
    std::string mostActiveUser;
    std::string leastActiveUser;
    std::string mostActiveLibrarian;
    std::string leastActiveLibrarian;
    std::string mostTrustedUser;
    std::string leastTrustedUser;
    std::string mostBorrowedBook;
    std::string mostBorrowedGenre;
    std::string mostBorrowedCategory;
    std::string mostBorrowedLanguage;
    bool isLoading = false;
    DatabaseConnection* db = nullptr;
};

class StatisticsPage : public Page {
private:
    void updateStatistics(StatisticsPageState* state) {
        if (!state || !state->db) return;

        state->isLoading = true;

        try {
            UserDAO userDao(*state->db);
            BookDAO bookDao(*state->db);

            auto activeUser = userDao.most_activeUser(state->timeRange);
            state->mostActiveUser = activeUser.getFullName();
            auto inactiveUser = userDao.least_activeUser(state->timeRange);
            state->leastActiveUser = inactiveUser.getFullName();
			auto activeLib = userDao.most_activeLibrarian(state->timeRange);// these don'ty have a row in the interface!!! include it!!!!
            state->mostActiveLibrarian = activeLib.getFullName();// these don'ty have a row in the interface!!! include it!!!!
            auto inactiveLib = userDao.least_activeLibrarian(state->timeRange);
            state->leastActiveLibrarian = inactiveLib.getFullName();
            auto trustedUser = userDao.most_trustedUser(state->timeRange);
            state->mostTrustedUser = trustedUser.getFullName();
            auto unfaithfulUser = userDao.most_unfaithfulUser(state->timeRange);
            state->leastTrustedUser = unfaithfulUser.getFullName();

            auto mostBorrowed = bookDao.most_borrowed_book_in_xperiod(state->timeRange);
            state->mostBorrowedBook = mostBorrowed.getTitle();

            // Temporary fixed data for testing
            state->mostBorrowedGenre = "Crime";
            state->mostBorrowedCategory = "Fiction";
            state->mostBorrowedLanguage = "English";

            state->isLoading = false;
        }
        catch (const std::exception& e) {
            state->isLoading = false;
            // Handle error (e.g., log or show an error message)
        }
    }

public:
    StatisticsPage() {
        strncpy_s(title, "Library Analytics Dashboard", sizeof(title));
    }

    void render(AppState& appState, std::shared_ptr<PageState> pageState) override {
        auto statsState = std::static_pointer_cast<StatisticsPageState>(pageState);
        if (!statsState) {
            appState.showErrorModal = true;
            appState.errorMessage = "Statistics page state is not properly initialized.";
            return;
        }

        ImGui::Begin("Library Analytics Dashboard", nullptr, ImGuiWindowFlags_NoCollapse);

        // Style and visuals
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.45f));

        // Time range selector
        static const char* ranges[] = { "Of All Time", "Past Year", "Past Month" };
        static int currentRange = 0;
        ImGui::Text("Select Time Range");//replace its position!!!!!!Don't put it on top of the table!!!
        if (ImGui::Combo("##TimeRange", &currentRange, ranges, IM_ARRAYSIZE(ranges))) {
            switch (currentRange) {
            case 0: statsState->timeRange = "of all time"; break;
            case 1: statsState->timeRange = "past year"; break;
            case 2: statsState->timeRange = "past month"; break;
            }
            updateStatistics(statsState.get());
        }

        ImGui::Separator();

        // User Activity Section
        if (ImGui::CollapsingHeader("User Activity Analysis", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(4, "UserStats");
            ImGui::Text("Metric"); ImGui::NextColumn();
            ImGui::Text("Of All Time"); ImGui::NextColumn();
            ImGui::Text("Past Year"); ImGui::NextColumn();
            ImGui::Text("Past Month"); ImGui::NextColumn();
            ImGui::Separator();

            ImGui::Text("Most Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Text("Least Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Columns(1);
        }

        // Book Statistics Section
        if (ImGui::CollapsingHeader("Book Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Most Borrowed Book: %s", statsState->mostBorrowedBook.c_str());
            ImGui::Text("Most Borrowed Genre: %s", statsState->mostBorrowedGenre.c_str());
            ImGui::Text("Most Borrowed Category: %s", statsState->mostBorrowedCategory.c_str());
            ImGui::Text("Most Borrowed Language: %s", statsState->mostBorrowedLanguage.c_str());
        }

        // User Reliability Section
        if (ImGui::CollapsingHeader("User Reliability", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(2);

            ImGui::Text("Most Trusted User");
            ImGui::Text("%s", statsState->mostTrustedUser.c_str());
            ImGui::NextColumn();

            ImGui::Text("Least Trusted User");
            ImGui::Text("%s", statsState->leastTrustedUser.c_str());
            ImGui::Columns(1);
        }

        // Pop style modifications
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        ImGui::End();
    }
};
// the data isn't showing up on the goddamed interface !! there is only empty table & rows!! whyy!??
*/
/*
#include "Page.hpp"
#include <memory>
#include <string>
#include <imgui.h>
#include "Database.hpp"


struct StatisticsPageState : public PageState {
    std::string timeRange = "of all time";
    std::string mostActiveUser;
    std::string leastActiveUser;
    std::string mostActiveLibrarian;
    std::string leastActiveLibrarian;
    std::string mostTrustedUser;
    std::string leastTrustedUser;
    std::string mostBorrowedBook;
    std::string mostBorrowedGenre;
    std::string mostBorrowedCategory;
    std::string mostBorrowedLanguage;

    
    
};
class StatisticsPage : public Page {
private:
    UserDAO& userDB;
    BookDAO& bookDB;
    std::string timeRange;

    void updateStatistics(StatisticsPageState* state) {
		if (!state) return;

        try {
            auto activeUser = userDB.most_activeUser(timeRange);
            state->mostActiveUser = activeUser.getFullName();

            auto inactiveUser = userDB.least_activeUser(timeRange);
            state->leastActiveUser = inactiveUser.getFullName();

            auto activeLib = userDB.most_activeLibrarian(timeRange);
            state->mostActiveLibrarian = activeLib.getFullName();

            auto inactiveLib = userDB.least_activeLibrarian(timeRange);
            state->leastActiveLibrarian = inactiveLib.getFullName();

            auto trustedUser = userDB.most_trustedUser(timeRange);
            state->mostTrustedUser = trustedUser.getFullName();

            auto unfaithfulUser = userDB.most_unfaithfulUser(timeRange);
            state->leastTrustedUser = unfaithfulUser.getFullName();

            auto mostBorrowed = bookDB.most_borrowed_book_in_xperiod(timeRange);
            state->mostBorrowedBook = mostBorrowed.getTitle();

            // Temporary fixed data for testing
            state->mostBorrowedGenre = "Crime";
            state->mostBorrowedCategory = "Fiction";
            state->mostBorrowedLanguage = "English";

        } catch (const std::exception& e) {
           
        }
    }

public:
    StatisticsPage(UserDAO& userDB, BookDAO& bookDB) : userDB(userDB), bookDB(bookDB), timeRange("of all time") {
        strncpy_s(title, "Library Analytics Dashboard", sizeof(title));
    }

    void render(AppState& appState, std::shared_ptr<PageState> pageState) override {
        auto statsState = std::static_pointer_cast<StatisticsPageState>(pageState);
        if (!statsState) {
            appState.showErrorModal = true;
            appState.errorMessage = "Statistics page state is not properly initialized.";
            return;
        }

        ImGui::Begin("Library Analytics Dashboard", nullptr, ImGuiWindowFlags_NoCollapse);

        // Style and visuals
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.45f));

        // Time range selector
        static const char* ranges[] = { "Of All Time", "Past Year", "Past Month" };
        static int currentRange = 0;
        ImGui::Text("Select Time Range");
        if (ImGui::Combo("##TimeRange", &currentRange, ranges, IM_ARRAYSIZE(ranges))) {
            switch (currentRange) {
            case 0: timeRange = "of all time"; break;
            case 1: timeRange = "past year"; break;
            case 2: timeRange = "past month"; break;
            }
            updateStatistics(statsState.get());
        }

        ImGui::Separator();

        // User Activity Section
        if (ImGui::CollapsingHeader("User Activity Analysis", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(4, "UserStats");
            ImGui::Text("Metric"); ImGui::NextColumn();
            ImGui::Text("Of All Time"); ImGui::NextColumn();
            ImGui::Text("Past Year"); ImGui::NextColumn();
            ImGui::Text("Past Month"); ImGui::NextColumn();
            ImGui::Separator();

            ImGui::Text("Most Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Text("Least Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Text("Most Active Librarian"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();

            ImGui::Text("Least Active Librarian"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveLibrarian.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveLibrarian.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveLibrarian.c_str()); ImGui::NextColumn();

            ImGui::Columns(1);
        }

        // Book Statistics Section
        if (ImGui::CollapsingHeader("Book Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Most Borrowed Book: %s", statsState->mostBorrowedBook.c_str());
            ImGui::Text("Most Borrowed Genre: %s", statsState->mostBorrowedGenre.c_str());
            ImGui::Text("Most Borrowed Category: %s", statsState->mostBorrowedCategory.c_str());
            ImGui::Text("Most Borrowed Language: %s", statsState->mostBorrowedLanguage.c_str());
        }

        // User Reliability Section
        if (ImGui::CollapsingHeader("User Reliability", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(2);

            ImGui::Text("Most Trusted User");
            ImGui::Text("%s", statsState->mostTrustedUser.c_str());
            ImGui::NextColumn();

            ImGui::Text("Least Trusted User");
            ImGui::Text("%s", statsState->leastTrustedUser.c_str());
            ImGui::Columns(1);
        }

        // Pop style modifications
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        ImGui::End();
    }
};
*/
#pragma once



/*User mostActiveUser;
User leastActiveUser;
User mostActiveLibrarian;
User leastActiveLibrarian;
User mostTrustedUser;
User leastTrustedUser;

// Book statistics
Book mostBorrowedBook;
std::string mostBorrowedGenre;
std::string mostBorrowedCategory;
char selectedFilterType[32] = "Language";  // Language, Category, or Genre
char selectedFilterValue[64] = "";
int bookCountByFilter = 0;

*/
#pragma once
#include "Page.hpp"
#include <memory>
#include <string>
#include <imgui.h>

// Define the state for the StatisticsPage
struct StatisticsPageState : public PageState {
    // User statistics
    std::string mostActiveUser = "John Doe";
    std::string leastActiveUser = "Jane Smith";
    std::string mostActiveLibrarian = "Robert Johnson";
    std::string leastActiveLibrarian = "Sarah Wilson";
    std::string mostTrustedUser = "Michael Brown";
    std::string leastTrustedUser = "Emma Davis";

    // Book statistics
    std::string mostBorrowedBook = "The Great Gatsby";
    std::string mostBorrowedGenre = "Fiction";
    std::string mostBorrowedCategory = "Literature";

    // Book counting
    char selectedFilterType[32] = "Language";
    char selectedFilterValue[64] = "";
    int bookCountByFilter = 42; // Dummy value

    bool isLoading = false;
};

// Define the StatisticsPage class
class StatisticsPage : public Page {
public:
    // Constructor to initialize the title of the page
    StatisticsPage() {
        strncpy_s(title, "Statistics Dashboard", sizeof(title));
    }

    // Render function to display the statistics
    void render(AppState& state, std::shared_ptr<PageState> pageState) override {
        // Cast the pageState to StatisticsPageState
        auto statsState = std::static_pointer_cast<StatisticsPageState>(pageState);

        // Display user activity statistics
        ImGui::Text("User Activity Statistics");
        ImGui::Separator();

        // Create a table for user statistics
        if (ImGui::BeginTable("user_statistics", 2, ImGuiTableFlags_Borders)) {
            // Most/Least Active Users
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Active User");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Least Active User");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str());

            // Most/Least Active Librarians
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Active Librarian");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostActiveLibrarian.c_str());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Least Active Librarian");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->leastActiveLibrarian.c_str());

            // Most/Least Trusted Users
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Trusted User");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostTrustedUser.c_str());

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Least Trusted User");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->leastTrustedUser.c_str());

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Text("Book Statistics");
        ImGui::Separator();

        // Create a table for book statistics
        if (ImGui::BeginTable("book_statistics", 2, ImGuiTableFlags_Borders)) {
            // Most Borrowed Book
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Borrowed Book");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostBorrowedBook.c_str());

            // Most Borrowed Genre
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Borrowed Genre");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostBorrowedGenre.c_str());

            // Most Borrowed Category
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Most Borrowed Category");
            ImGui::TableNextColumn();
            ImGui::Text("%s", statsState->mostBorrowedCategory.c_str());

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Text("Book Count by Filter");
        ImGui::Separator();

        // Filter type combo box
        if (ImGui::BeginCombo("Filter Type", statsState->selectedFilterType)) {
            if (ImGui::Selectable("Language")) {
                strncpy_s(statsState->selectedFilterType, "Language", sizeof(statsState->selectedFilterType));
            }
            if (ImGui::Selectable("Category")) {
                strncpy_s(statsState->selectedFilterType, "Category", sizeof(statsState->selectedFilterType));
            }
            if (ImGui::Selectable("Genre")) {
                strncpy_s(statsState->selectedFilterType, "Genre", sizeof(statsState->selectedFilterType));
            }
            ImGui::EndCombo();
        }

        // Input text for filter value
        ImGui::InputText("Filter Value", statsState->selectedFilterValue, sizeof(statsState->selectedFilterValue));

        // Button to count books
        if (ImGui::Button("Count Books")) {
            statsState->isLoading = true;
            // Dummy count - will be replaced with real data later
            statsState->bookCountByFilter = 42;
            statsState->isLoading = false;
        }

        ImGui::SameLine();
        ImGui::Text("Count: %d", statsState->bookCountByFilter);

        // Display loading text if isLoading is true
        if (statsState->isLoading) {
            ImGui::SameLine();
            ImGui::Text("Loading...");
        }
    }
};
/*public:
    StatisticsPage(UserDAO,BookDAO) {
        strncpy_s(title, "Library Analytics Dashboard", sizeof(title));
    }

    void render(AppState& appState, std::shared_ptr<PageState> pageState) override {
        auto statsState = std::static_pointer_cast<StatisticsPageState>(pageState);
        if (!statsState) {
            appState.showErrorModal = true;
            appState.errorMessage = "Statistics page state is not properly initialized.";
            return;
        }

        ImGui::Begin("Library Analytics Dashboard", nullptr, ImGuiWindowFlags_NoCollapse);

        // Style and visuals
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.8f, 0.45f));

        // Table headers
        if (ImGui::CollapsingHeader("Library Analytics Overview", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(4, "UserStats");
            ImGui::Text("Good Oms!");
                  ImGui::Columns(4, "UserStats");
            ImGui::Text("Metric"); ImGui::NextColumn();
            ImGui::Text("Of All Time"); ImGui::NextColumn();
            ImGui::Text("Past Year"); ImGui::NextColumn();
            ImGui::Text("Past Month"); ImGui::NextColumn();
            ImGui::Separator();
            ImGui::Text("Most Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->mostActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Text("Least Active User"); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", statsState->leastActiveUser.c_str()); ImGui::NextColumn();


            ImGui::Text("Most Active Librarian"); ImGui::NextColumn();
			ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();
			ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();
			ImGui::Text("%s", statsState->mostActiveLibrarian.c_str()); ImGui::NextColumn();
            

            ImGui::Columns(1);
        }

        // Time range selector placed after the table
        static const char* ranges[] = { "Of All Time", "Past Year", "Past Month" };
        static int currentRange = 0;
        ImGui::Text("Select Time Range");
        if (ImGui::Combo("##TimeRange", &currentRange, ranges, IM_ARRAYSIZE(ranges))) {
            printf("Time range changed: %s\n", ranges[currentRange]);
            switch (currentRange) {
            case 0: statsState->timeRange = "of all time"; break;
            case 1: statsState->timeRange = "past year"; break;
            case 2: statsState->timeRange = "past month"; break;
            }
            updateStatistics(statsState.get());
        }

        ImGui::Separator();

        // Book Statistics Section
        if (ImGui::CollapsingHeader("Book Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Most Borrowed Book: %s", statsState->mostBorrowedBook.c_str());
            ImGui::Text("Most Borrowed Genre: %s", statsState->mostBorrowedGenre.c_str());
            ImGui::Text("Most Borrowed Category: %s", statsState->mostBorrowedCategory.c_str());
            ImGui::Text("Most Borrowed Language: %s", statsState->mostBorrowedLanguage.c_str());
        }


        // User Reliability Section
        if (ImGui::CollapsingHeader("User Reliability", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(2);

            ImGui::Text("Most Trusted User");
            ImGui::Text("%s", statsState->mostTrustedUser.empty() ? "N/A" : statsState->mostTrustedUser.c_str());
            ImGui::NextColumn();

            ImGui::Text("Least Trusted User");
            ImGui::Text("%s", statsState->leastTrustedUser.empty() ? "N/A" : statsState->leastTrustedUser.c_str());
            ImGui::Columns(1);
        }

        // Restore style modifications
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        ImGui::End();
    }*/
