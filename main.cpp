#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "interfaceManager.hpp"

using namespace std;

int main() {
    try {
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
        InterfaceApp uiManager(auth, userdb);

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return 1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(600, 600, "Library", NULL, NULL);
        if (window == NULL) {
            std::cerr << "GLFW couldn't create window" << std::endl;
            glfwTerminate();
            return 1;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return 1;
        }
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
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }

    return 0;
}
