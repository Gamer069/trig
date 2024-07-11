cmake_minimum_required(VERSION 3.29.6)
project(Trig CXX C OBJC OBJCXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
file(GLOB SOURCES src/*.cc src/*.cpp)

# Set cocoa
set(GLFW_BUILD_COCOA ON)

# Add lib/glfw as a subdirectory
add_subdirectory(lib/glfw)

add_definitions(-DGLFW_INCLUDE_NONE)

# Add imgui source files using file(GLOB)
file(GLOB IMGUI_SRC lib/imgui/*.cc lib/imgui/*.cpp lib/imgui/backends/imgui_impl_opengl3.cpp lib/imgui/backends/imgui_impl_glfw.cpp)

# Set glad source file
set(GLAD_SRC lib/glad/glad.c)
# Set nfd source files
# set(NFD_SRC lib/nfd/src/nfd_cocoa.m lib/nfd/src/nfd_common.c)
set(NFD_SRC lib/nfd/src/nfd_cocoa.m lib/nfd/src/nfd_common.c)

# Add executable target
add_executable(Trig ${NFD_SRC} ${SOURCES} ${IMGUI_SRC} ${GLAD_SRC})

# Include directories for glad and imgui
target_include_directories(Trig PUBLIC
    lib/glad/
    lib/imgui
    lib/nfd/src/include
)

# Link libraries
target_link_libraries(Trig
    glfw
)
