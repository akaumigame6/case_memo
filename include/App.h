// App.h
// ==========================
// GLFW + ImGui を初期化・終了処理するアプリケーション枠組み。
// ==========================

#pragma once
#include "MarkdownEditor.h"

class App
{
public:
    App();
    ~App();
    void Run();

private:
    void Init();
    void Shutdown();

    struct GLFWwindow* window = nullptr;
};
