#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include "interfaceManager.hpp"

using namespace std;

int main() {
    std::string host, user, password;
    std::cout << "host: "; std::cin >> host;
    std::cout << "username: "; std::cin >> user;
    std::cout << "password: "; std::cin >> password;
    DatabaseConnection dbconn(host, user, password, "library");
    if (!dbconn.isConnected()) {
        std::cerr << "Failed to connect to the database." << std::endl;
        return 1;
    }
    UserDAO userdb(dbconn);
    Auth auth = Auth(userdb);
    InterfaceApp uiManager(auth);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Library", NULL, NULL);
    if (window == NULL) {
        std::cout << "GLFW couldn't create window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    glViewport(0, 0, 600, 600);

    // ImGui integration
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Background Color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();                   // Process events
        ImGui_ImplOpenGL3_NewFrame();       // Prepare ImGui
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui interface
        uiManager.render(io.DisplaySize);

        // Finalize rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


// main.cpp
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <string>
//#include <vector>
//#include "DatabaseConnection.hpp"

// Global state for input fields
//struct AppState {
//    // User form
//    char userEmail[100] = "";
//    char userFirstName[50] = "";
//    char userLastName[50] = "";
//    char userPassword[64] = "";
//    char userRole[20] = "";
//    int selectedUserId = 0;
//
//    // Book form
//    char bookIsbn[14] = "";
//    char bookTitle[101] = "";
//    int bookPubYear = 2024;
//    int bookCopies = 0;
//    int bookPages = 0;
//    int selectedBookId = 0;
//
//    // Search buffers
//    char searchUserId[10] = "";
//    char searchBookId[10] = "";
//
//    // Modal flags
//    bool showErrorModal = false;
//    std::string errorMessage;
//};
//
//void ShowErrorModal(AppState& state) {
//    if (state.showErrorModal) {
//        ImGui::OpenPopup("Error");
//        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
//        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
//
//        if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
//            ImGui::Text("%s", state.errorMessage.c_str());
//            ImGui::Separator();
//
//            if (ImGui::Button("OK", ImVec2(120, 0))) {
//                state.showErrorModal = false;
//                ImGui::CloseCurrentPopup();
//            }
//            ImGui::EndPopup();
//        }
//    }
//}
//
//void ShowUserManagement(UserRepository& userRepo, AppState& state) {
//    ImGui::Begin("User Management");
//
//    // Create User Form
//    ImGui::Text("Create/Update User");
//    ImGui::InputText("Email", state.userEmail, IM_ARRAYSIZE(state.userEmail));
//    ImGui::InputText("First Name", state.userFirstName, IM_ARRAYSIZE(state.userFirstName));
//    ImGui::InputText("Last Name", state.userLastName, IM_ARRAYSIZE(state.userLastName));
//    ImGui::InputText("Password Hash", state.userPassword, IM_ARRAYSIZE(state.userPassword));
//    ImGui::InputText("Role", state.userRole, IM_ARRAYSIZE(state.userRole));
//
//    if (ImGui::Button("Create User")) {
//        try {
//            User user;
//            user.email = state.userEmail;
//            user.firstName = state.userFirstName;
//            user.lastName = state.userLastName;
//            user.passwordHash = state.userPassword;
//            user.userRole = state.userRole;
//
//            userRepo.create(user);
//
//            // Clear form
//            memset(state.userEmail, 0, sizeof(state.userEmail));
//            memset(state.userFirstName, 0, sizeof(state.userFirstName));
//            memset(state.userLastName, 0, sizeof(state.userLastName));
//            memset(state.userPassword, 0, sizeof(state.userPassword));
//            memset(state.userRole, 0, sizeof(state.userRole));
//        }
//        catch (const std::exception& e) {
//            state.errorMessage = e.what();
//            state.showErrorModal = true;
//        }
//    }
//
//    ImGui::Separator();
//
//    // Find User
//    ImGui::Text("Find User");
//    ImGui::InputText("User ID", state.searchUserId, IM_ARRAYSIZE(state.searchUserId));
//
//    if (ImGui::Button("Find")) {
//        try {
//            int userId = std::atoi(state.searchUserId);
//            bool isUser = false;
//            User user = userRepo.findById(userId, isUser);
//
//            if (isUser) {
//                state.selectedUserId = user.id;
//                strncpy_s(state.userEmail, user.email.c_str(), sizeof(state.userEmail) - 1);
//                strncpy_s(state.userFirstName, user.firstName.c_str(), sizeof(state.userFirstName) - 1);
//                strncpy_s(state.userLastName, user.lastName.c_str(), sizeof(state.userLastName) - 1);
//                strncpy_s(state.userRole, user.userRole.c_str(), sizeof(state.userRole) - 1);
//            }
//            else {
//                state.errorMessage = "User not found";
//                state.showErrorModal = true;
//            }
//        }
//        catch (const std::exception& e) {
//            state.errorMessage = e.what();
//            state.showErrorModal = true;
//        }
//    }
//
//    if (state.selectedUserId > 0) {
//        ImGui::SameLine();
//        if (ImGui::Button("Update")) {
//            try {
//                User user;
//                user.id = state.selectedUserId;
//                user.email = state.userEmail;
//                user.firstName = state.userFirstName;
//                user.lastName = state.userLastName;
//                user.passwordHash = state.userPassword;
//                user.userRole = state.userRole;
//
//                userRepo.update(user);
//            }
//            catch (const std::exception& e) {
//                state.errorMessage = e.what();
//                state.showErrorModal = true;
//            }
//        }
//
//        ImGui::SameLine();
//        if (ImGui::Button("Delete")) {
//            try {
//                userRepo.remove(state.selectedUserId);
//                state.selectedUserId = 0;
//                memset(state.userEmail, 0, sizeof(state.userEmail));
//                memset(state.userFirstName, 0, sizeof(state.userFirstName));
//                memset(state.userLastName, 0, sizeof(state.userLastName));
//                memset(state.userPassword, 0, sizeof(state.userPassword));
//                memset(state.userRole, 0, sizeof(state.userRole));
//            }
//            catch (const std::exception& e) {
//                state.errorMessage = e.what();
//                state.showErrorModal = true;
//            }
//        }
//    }
//
//    ImGui::End();
//}
//
//void ShowBookManagement(BookRepository& bookRepo, AppState& state) {
//    ImGui::Begin("Book Management");
//
//    // Create Book Form
//    ImGui::Text("Create/Update Book");
//    ImGui::InputText("ISBN", state.bookIsbn, IM_ARRAYSIZE(state.bookIsbn));
//    ImGui::InputText("Title", state.bookTitle, IM_ARRAYSIZE(state.bookTitle));
//    ImGui::InputInt("Publication Year", &state.bookPubYear);
//    ImGui::InputInt("Available Copies", &state.bookCopies);
//    ImGui::InputInt("Number of Pages", &state.bookPages);
//
//    if (ImGui::Button("Create Book")) {
//        try {
//            Book book;
//            book.isbn = state.bookIsbn;
//            book.title = state.bookTitle;
//            book.pubYear = state.bookPubYear;
//            book.numAvailableCopies = state.bookCopies;
//            book.nbrPages = state.bookPages;
//
//            bookRepo.create(book);
//
//            // Clear form
//            memset(state.bookIsbn, 0, sizeof(state.bookIsbn));
//            memset(state.bookTitle, 0, sizeof(state.bookTitle));
//            state.bookPubYear = 2024;
//            state.bookCopies = 0;
//            state.bookPages = 0;
//        }
//        catch (const std::exception& e) {
//            state.errorMessage = e.what();
//            state.showErrorModal = true;
//        }
//    }
//
//    ImGui::Separator();
//
//    // Find Book
//    ImGui::Text("Find Book");
//    ImGui::InputText("Book ID", state.searchBookId, IM_ARRAYSIZE(state.searchBookId));
//
//    if (ImGui::Button("Find")) {
//        try {
//            int bookId = std::atoi(state.searchBookId);
//            bool isBook = false;
//            Book book = bookRepo.findById(bookId, isBook);
//
//            if (isBook) {
//                state.selectedBookId = book.id;
//                strncpy_s(state.bookIsbn, book.isbn.c_str(), sizeof(state.bookIsbn) - 1);
//                strncpy_s(state.bookTitle, book.title.c_str(), sizeof(state.bookTitle) - 1);
//                state.bookPubYear = book.pubYear;
//                state.bookCopies = book.numAvailableCopies;
//                state.bookPages = book.nbrPages;
//            }
//            else {
//                state.errorMessage = "Book not found";
//                state.showErrorModal = true;
//            }
//        }
//        catch (const std::exception& e) {
//            state.errorMessage = e.what();
//            state.showErrorModal = true;
//        }
//    }
//
//    if (state.selectedBookId > 0) {
//        ImGui::SameLine();
//        if (ImGui::Button("Update")) {
//            try {
//                Book book;
//                book.id = state.selectedBookId;
//                book.isbn = state.bookIsbn;
//                book.title = state.bookTitle;
//                book.pubYear = state.bookPubYear;
//                book.numAvailableCopies = state.bookCopies;
//                book.nbrPages = state.bookPages;
//
//                bookRepo.update(book);
//            }
//            catch (const std::exception& e) {
//                state.errorMessage = e.what();
//                state.showErrorModal = true;
//            }
//        }
//
//        ImGui::SameLine();
//        if (ImGui::Button("Delete")) {
//            try {
//                bookRepo.remove(state.selectedBookId);
//                state.selectedBookId = 0;
//                memset(state.bookIsbn, 0, sizeof(state.bookIsbn));
//                memset(state.bookTitle, 0, sizeof(state.bookTitle));
//                state.bookPubYear = 2024;
//                state.bookCopies = 0;
//                state.bookPages = 0;
//            }
//            catch (const std::exception& e) {
//                state.errorMessage = e.what();
//                state.showErrorModal = true;
//            }
//        }
//    }
//
//    ImGui::End();
//}
//
//int main() {
//    // Initialize GLFW and OpenGL
//    if (!glfwInit()) {
//        return -1;
//    }
//
//    const char* glsl_version = "#version 130";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//
//    GLFWwindow* window = glfwCreateWindow(1280, 720, "Library Management System", NULL, NULL);
//    if (!window) {
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1); // Enable vsync
//
//    // Initialize ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void)io;
//
//    ImGui::StyleColorsDark();
//
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//    AppState state;
//
//    try {
//        std::string host;
//        std::string user;
//        std::string password;
//        std::cout << "host: "; std::cin >> host;
//        std::cout << "username: "; std::cin >> user;
//        std::cout << "password: "; std::cin >> password;
//        DatabaseConnection db(host, user, password, "library");
//        UserRepository userRepo(db);
//        BookRepository bookRepo(db);
//
//        while (!glfwWindowShouldClose(window)) {
//            glfwPollEvents();
//
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//            ImGui::NewFrame();
//
//            ShowUserManagement(userRepo, state);
//            ShowBookManagement(bookRepo, state);
//            ShowErrorModal(state);
//
//            ImGui::Render();
//            int display_w, display_h;
//            glfwGetFramebufferSize(window, &display_w, &display_h);
//            glViewport(0, 0, display_w, display_h);
//            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
//            glClear(GL_COLOR_BUFFER_BIT);
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//            glfwSwapBuffers(window);
//        }
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Fatal error: " << e.what() << std::endl;
//        return 1;
//    }
//
//    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}