#include <glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <fstream>
#include <sstream>

#include <iostream>

// Function prototypes
bool InitGLFW(GLFWwindow*& window);
bool InitGlad();
bool InitImGui(GLFWwindow* window);
void Loop(GLFWwindow* window);
void Render(GLFWwindow* window);
const char* ReadFile(nfdchar_t* path);

int main() {
    // Initialize GLFW, GLAD, ImGui, and create a window
    GLFWwindow* window = nullptr;

    if (!InitGLFW(window)) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    if (!InitGlad()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    if (!InitImGui(window)) {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        return 1;
    }

    // Main loop
    Loop(window);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

bool InitGLFW(GLFWwindow*& window) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Create a windowed mode window and its OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);    
    #endif

    window = glfwCreateWindow(1280, 720, "ImGui GLFW+GLAD Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    return true;
}

bool InitGlad() {
    // Initialize GLAD: This must be done after creating a window with GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

bool InitImGui(GLFWwindow* window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    return true;
}

void Loop(GLFWwindow* window) {
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Render(window);

        // Rendering
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_context);

        glfwSwapBuffers(window);
    }
}

void Render(GLFWwindow* window) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
                glfwDestroyWindow(window);
                glfwTerminate();
                std::exit(-1);
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                nfdchar_t* out = 0;
                nfdresult_t result = NFD_OpenDialog(0, 0, &out);

                if (result == NFD_OKAY) {
                    const char* contents = ReadFile(out);
                    std::cout << contents << std::endl;
                    free(out);
                } else if (result == NFD_CANCEL) {
                } else {
                    std::cerr << "ERR" << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

const char* ReadFile(nfdchar_t* path) {
    std::ifstream file(path, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return nullptr; 
    }

    std::stringstream ret("");
    std::string line;
    while (std::getline(file, line)) {
        ret << line << std::endl;
    }
    file.close();

    std::string retStr = ret.str();

    char* cstr = new char[retStr.length() + 1];
    std::strcpy(cstr, retStr.c_str());

    return cstr;
}
