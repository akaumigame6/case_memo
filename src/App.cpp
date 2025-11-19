// App.cpp
#include "App.h"
#include "TemplateManager.h"
#include "MarkdownEditor.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

App::App() { Init(); }
App::~App() { Shutdown(); }

void App::Init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW\n";
        return;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(800, 600, "Markdown Quick Editor", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ImGuiセットアップ
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // 日本語フォントを fonts フォルダから読み込む
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig font_cfg;
        font_cfg.OversampleH = 2;
        font_cfg.OversampleV = 2;
        font_cfg.PixelSnapH = true;

        const float fontSize = 18.0f; // 画面向けのデフォルトサイズ
        const char* candidates[] = {
            "fonts/NotoSansJP-Regular.ttf",
            "../fonts/NotoSansJP-Regular.ttf",
            "NotoSansJP-Regular.ttf",
            nullptr
        };

        bool fontLoaded = false;
        for (int i = 0; candidates[i] != nullptr; ++i)
        {
            const char* path = candidates[i];
            std::ifstream ifs(path);
            if (ifs.good())
            {
                io.Fonts->AddFontFromFileTTF(path, fontSize, &font_cfg, io.Fonts->GetGlyphRangesJapanese());
                std::cerr << "Loaded font: " << path << " for Japanese support.\n";
                fontLoaded = true;
                break;
            }
        }
        if (!fontLoaded)
        {
            std::cerr << "Warning: Japanese font not found in fonts/. Falling back to default font.\n";
            // フォントが見つからなければデフォルトフォントのまま
        }
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void App::Run()
{
    TemplateManager tm("templates");
    tm.LoadTemplates();

    static int selected = 0;
    TemplateData currentTemplate;
    bool templateLoaded = false;
    std::string generatedMarkdown;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Markdown Form Editor");

        const auto& names = tm.GetTemplateNames();
        if (!names.empty())
        {
            if (ImGui::BeginCombo("テンプレート", names[selected].c_str()))
            {
                for (int i = 0; i < names.size(); i++)
                {
                    bool isSel = (selected == i);
                    if (ImGui::Selectable(names[i].c_str(), isSel))
                    {
                        selected = i;
                        currentTemplate = tm.LoadTemplate(names[i]);
                        templateLoaded = true;
                    }
                    if (isSel)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }

        if (templateLoaded)
        {
            DrawTemplateEditor(currentTemplate);
            if (ImGui::Button("Export as Markdown"))
            {
                generatedMarkdown = GenerateMarkdown(currentTemplate);
            }
        }

        if (!generatedMarkdown.empty())
        {
            ImGui::Separator();
            ImGui::Text("出力結果:");
            ImGui::InputTextMultiline("##mdout", (char*)generatedMarkdown.c_str(),
                generatedMarkdown.size() + 1, ImVec2(-1, ImGui::GetTextLineHeight() * 10),
                ImGuiInputTextFlags_ReadOnly);
        }

        ImGui::End();
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void App::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}