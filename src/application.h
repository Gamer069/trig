#pragma once

// Glad
#include <glad.h>

// GLFW
#include <GLFW/glfw3.h>

// NFD
#include <nfd.h>

// ImGui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// STD
#include <fstream>
#include <sstream>
#include <iostream>

namespace Trig {
    class Application {
        public:
            Application() = default;
            ~Application() { Cleanup(); }
            bool InitGLFW();
            bool InitGlad();
            bool InitImGui();
            void Loop();
            void RequestOpen();
            void Render();
            void Cleanup();
            const char* ReadFile(nfdchar_t* path);
        private:
            GLFWwindow* m_Window = nullptr;
    };
}
