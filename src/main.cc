#include "application.h"

int main() {
    Trig::Application* app = new Trig::Application();

    // Init everything
    if (!app->InitGLFW()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    if (!app->InitGlad()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    if (!app->InitImGui()) {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        return -1;
    }

    app->Loop();

    app->Cleanup();
    return 0;
}
