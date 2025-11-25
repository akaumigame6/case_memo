// Engine.h
#pragma once
#include <functional>

struct GLFWwindow;

namespace core {

class Engine {
public:
    Engine();
    ~Engine();

    // ウィンドウ初期化（内部で glfwInit などを行う）
    bool InitWindow(int width, int height, const char* title);

    // メインループ。コールバックはそれぞれ初期化・フレーム描画・終了処理を行う
    void Run(const std::function<void()>& onInit,
             const std::function<void()>& onFrame,
             const std::function<void()>& onShutdown);

    // GLFWwindow* を外部（UIなど）が使えるよう取得
    GLFWwindow* GetWindow() const { return window; }

private:
    GLFWwindow* window = nullptr;
};

} // namespace core
