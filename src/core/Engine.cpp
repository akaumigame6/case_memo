#include "Engine.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace core {

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::InitWindow(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return true;
}

void Engine::Run(const std::function<void()>& onInit,
                 const std::function<void()>& onFrame,
                 const std::function<void()>& onShutdown)
{
    if (onInit) onInit();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (onFrame) onFrame();

        // Swap buffers is core responsibility (platform)
        glfwSwapBuffers(window);
    }

    if (onShutdown) onShutdown();

    // cleanup
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

} // namespace core
