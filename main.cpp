#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "vector"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

enum class Pages {
	MainTable,
	CurrentBorrows,
	BorrowHistory
};


bool checkedP = false;
bool checkedT = false;
bool checkedA = false;
bool checkedC = false;
bool checkedG = false;

void DrawMainTable(ImGuiTableFlags flags) {
	const int nb_col = 9;

	if (ImGui::Checkbox("Publish date", &checkedP)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("title", &checkedT)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Author", &checkedA)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Grenre", &checkedG)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Category", &checkedC)) {
	}
	char* wanted = (char*)malloc(sizeof(char));
	ImGui::InputText("##", wanted, 100);
	ImGui::SameLine();
	if (ImGui::Button("apply")) {

	}
	if (ImGui::BeginTable("tableLogBook", nb_col, flags)) {
		ImGui::TableSetupColumn("Title");
		ImGui::TableSetupColumn("Publish Date");
		ImGui::TableSetupColumn("Number of Copies Available");
		ImGui::TableSetupColumn("Number of Pages");
		ImGui::TableSetupColumn("Language");
		ImGui::TableSetupColumn("Editor");
		ImGui::TableSetupColumn("Authors");
		ImGui::TableSetupColumn("Genres");
		ImGui::TableSetupColumn("Categories");
		ImGui::TableHeadersRow();
		ImGui::EndTable();
	}
}

void DrawBorrowsTable(ImGuiTableFlags flags) {
	const int nb_col = 9;

	if (ImGui::Checkbox("Publish date", &checkedP)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("title", &checkedT)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Author", &checkedA)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Grenre", &checkedG)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Category", &checkedC)) {
	}
	char* wanted = (char*)malloc(sizeof(char));
	ImGui::InputText("##", wanted, 100);
	ImGui::SameLine();
	if (ImGui::Button("apply")) {

	}
	if (ImGui::BeginTable("tableBorrows", nb_col, flags)) {
		ImGui::TableSetupColumn("Title");
		ImGui::TableSetupColumn("Publish Date");
		ImGui::TableSetupColumn("Number of Copies Available");
		ImGui::TableSetupColumn("Number of Pages");
		ImGui::TableSetupColumn("Language");
		ImGui::TableSetupColumn("Editor");
		ImGui::TableSetupColumn("Authors");
		ImGui::TableSetupColumn("Genres");
		ImGui::TableSetupColumn("Categories");
		ImGui::TableHeadersRow();
		ImGui::EndTable();
	}
}


void DrawHistoryTable(ImGuiTableFlags flags) {
	const int nb_col = 9;
	
	if (ImGui::Checkbox("Publish date", &checkedP)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("title", &checkedT)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Author", &checkedA)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Grenre", &checkedG)) {
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Category", &checkedC)) {
	}
	char* wanted = (char*)malloc(sizeof(char));
	ImGui::InputText("##",wanted,100);
	ImGui::SameLine();
	if (ImGui::Button("apply")) {

	}
	if (ImGui::BeginTable("tableHistory", nb_col, flags)) {
		ImGui::TableSetupColumn("Title");
		ImGui::TableSetupColumn("Publish Date");
		ImGui::TableSetupColumn("Number of Copies Available");
		ImGui::TableSetupColumn("Number of Pages");
		ImGui::TableSetupColumn("Language");
		ImGui::TableSetupColumn("Editor");
		ImGui::TableSetupColumn("Genres");
		ImGui::TableSetupColumn("Authors");
		ImGui::TableSetupColumn("Categories");
		ImGui::TableHeadersRow();
		ImGui::EndTable();
	}
}


void MemberInterface() {
	ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	static Pages  current_page = Pages::MainTable;
	
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Books")) {
			if (ImGui::MenuItem("Home")) {
				current_page = Pages::MainTable;
			}
			if (ImGui::MenuItem("My current borrows")) {
				current_page = Pages::CurrentBorrows;
				}
		
			if (ImGui::MenuItem("History of borrows")) {
				current_page = Pages::BorrowHistory;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImVec2 size = ImVec2(1300, 1200);

	switch (current_page) {
	case Pages::MainTable:
		ImGui::Begin("Home");
		ImGui::SetWindowSize(size);
		DrawMainTable(flags);
		ImGui::End();
		ImGui::Render();
		break;
	case Pages::CurrentBorrows:
		ImGui::Begin("Borrows");
		ImGui::SetWindowSize(size);
		DrawBorrowsTable(flags);
		ImGui::End();
		ImGui::Render();
		break;
	case Pages::BorrowHistory:
		ImGui::Begin("History");
		ImGui::SetWindowSize(size);
		DrawHistoryTable(flags);
		ImGui::End();
		ImGui::Render();
		break;
	}
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

	GLFWwindow* window = glfwCreateWindow(800, 800, "window title", NULL, NULL);
	if (window == NULL) {
		std::cout << "GLFW couldn't create window" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, 800, 800);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	GLuint VBO, VAO; // usuaally a list of references we have 1 obj
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind to avoid accidental modification
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ImGui code here
		MemberInterface();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents(); // Processes the events
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}