#include "application.h"

namespace Trig {
    bool Application::InitGLFW() {
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

        m_Window = glfwCreateWindow(1920, 1080, "Trig", 0, 0);
        if (!m_Window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSwapInterval(1); // Enable vsync

        return true;
    }
    bool Application::InitGlad() {
        // Initialize GLAD: This must be done after creating a window with GLFW
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }
        return true;
    }
    bool Application::InitImGui() {
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
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        return true;
    }
    void Application::Loop() {
        // Main loop
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Render();

            // Rendering
            glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);

            glfwSwapBuffers(m_Window);
        }
    }
    void Application::RequestOpen() {
        nfdchar_t* out = 0;
        nfdresult_t result = NFD_OpenDialog(0, 0, &out);

        if (result == NFD_OKAY) {
            const char* contents = ReadFile(out);
            std::cout << contents << std::endl;
            free(out);
        } else if (result == NFD_CANCEL) {
            std::cout << "Cancel" << std::endl;
        } else {
            std::cerr << "ERR" << std::endl;
        }

    }
    void Application::Render() {
        // for some reason on mac ctrl is swapped with command, but ig its fine.
        ImGuiIO& io = ImGui::GetIO();

        bool ctrlOrSuper = io.KeyCtrl || (io.ConfigMacOSXBehaviors && io.KeySuper);
        if (ctrlOrSuper && ImGui::IsKeyPressed(ImGuiKey_O)) {
            RequestOpen();
        }

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
                    Cleanup();
                    std::exit(0);
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    RequestOpen();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

    }
    const char* Application::ReadFile(nfdchar_t* path) {
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
    void Application::Cleanup() {
        // shutdown imgui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // shutdown glfw
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }
}
