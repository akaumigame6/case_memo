#include "App.h"
#include "model/TemplateManager.h"
#include "ui/MarkdownEditor.h"
#include <iostream>

// App.cpp (refactored)

App::App() {}
App::~App() {}

void App::Run()
{
    if (!engine.InitWindow(800, 600, "Markdown Quick Editor"))
    {
        std::cerr << "Failed to initialize window\n";
        return;
    }

    model::TemplateManager tm("templates");
    tm.LoadTemplates();

    // コアエンジンにコールバックを渡す（UIとモデルはこの中で使う）
    engine.Run(
        // onInit
        [&]() {
            ui::Init(engine.GetWindow());
        },
        // onFrame
        [&]() {
            ui::NewFrame();
            ui::DrawTemplateUI(tm);
            ui::Render();
        },
        // onShutdown
        [&]() {
            ui::Shutdown();
        }
    );
}