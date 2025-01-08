#pragma once
#include "Page.hpp"
#include <memory>
#include <string>
#include <imgui.h>
#include "Database.hpp"

class StatisticsPage : public Page {
private:
    UserDAO& userDB;
    BookDAO& bookDB;

    std::string timeRange = "of all time";
    std::string mostActiveUser;
    std::string leastActiveUser;
    std::string mostTrustedUser;
    std::string leastTrustedUser;
    std::string mostBorrowedBook;
    std::string mostBorrowedGenre;
    std::string mostBorrowedCategory;
    std::string mostBorrowedLanguage;

    std::vector<float> borrowedBooksData; // Ajout d'un vecteur pour les données des livres empruntés

    void setStatistics() {
        try {
            auto activeUser = userDB.most_activeUser(timeRange);
            mostActiveUser = activeUser.getFullName();

            auto inactiveUser = userDB.least_activeUser(timeRange);
            leastActiveUser = inactiveUser.getFullName();

            auto trustedUser = userDB.most_trustedUser(timeRange);
            mostTrustedUser = trustedUser.getFullName();

            auto unfaithfulUser = userDB.most_unfaithfulUser(timeRange);
            leastTrustedUser = unfaithfulUser.getFullName();

            auto mostBorrowed = bookDB.most_borrowed_book_in_xperiod(timeRange);
            mostBorrowedBook = mostBorrowed.getTitle();

            // Temporary fixed data for testing
            mostBorrowedGenre = "Crime";
            mostBorrowedCategory = "Russian Litterature";
            mostBorrowedLanguage = "English";

            // Simulated data for borrowed books over time
            borrowedBooksData = { 10, 20, 15, 30, 25, 40, 35 }; // Exemple de données

        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

public:
    StatisticsPage(UserDAO& userDB, BookDAO& bookDB) : userDB(userDB), bookDB(bookDB), timeRange("of all time") {
        strncpy_s(title, "Library Analytics Dashboard", sizeof(title));
        setStatistics();
    }

    void render(AppState& appState, std::shared_ptr<PageState> pageState) override {
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
            setStatistics();
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
            ImGui::Text("%s", mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", mostActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", mostActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Text("Least Active User"); ImGui::NextColumn();
            ImGui::Text("%s", leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", leastActiveUser.c_str()); ImGui::NextColumn();
            ImGui::Text("%s", leastActiveUser.c_str()); ImGui::NextColumn();

            ImGui::Columns(1);
        }

        // Book Statistics Section
        if (ImGui::CollapsingHeader("Book Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Most Borrowed Book: %s", mostBorrowedBook.c_str());
            ImGui::Text("Most Borrowed Genre: %s", mostBorrowedGenre.c_str());
            ImGui::Text("Most Borrowed Category: %s", mostBorrowedCategory.c_str());
            ImGui::Text("Most Borrowed Language: %s", mostBorrowedLanguage.c_str());

            // Graphique des livres empruntés
            ImGui::PlotHistogram("Borrowed Books Over Time", borrowedBooksData.data(), borrowedBooksData.size(), 0, NULL, 0, 50, ImVec2(0, 80));
        }

        // User Reliability Section
        if (ImGui::CollapsingHeader("User Reliability", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Columns(2);

            ImGui::Text("Most Trusted User");
            ImGui::Text("%s", mostTrustedUser.c_str());
            ImGui::NextColumn();

            ImGui::Text("Least Trusted User");
            ImGui::Text("%s", leastTrustedUser.c_str());
            ImGui::Columns(1);
        }

        // Pop style modifications
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(1);

        ImGui::End();
    }
};
