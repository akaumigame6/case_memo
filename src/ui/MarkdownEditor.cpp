#include "MarkdownEditor.h"
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <filesystem>
#include <cctype>

namespace ui {

bool Init(void* windowPtr)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(windowPtr);
    if (!window) return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // フォント読み込みはここで行う
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig font_cfg;
    font_cfg.OversampleH = 2;
    font_cfg.OversampleV = 2;
    font_cfg.PixelSnapH = true;

    const float fontSize = 18.0f; // 画面向けのデフォルトサイズ

    // フォントディレクトリを走査して .ttf/.otf を探す
    std::vector<std::string> searchDirs = {"fonts", "..\\fonts", "../fonts"};
    bool fontLoaded = false;
    ImFont* loadedFont = nullptr;
    for (const auto& dir : searchDirs)
    {
        std::filesystem::path p(dir);
        if (!std::filesystem::exists(p) || !std::filesystem::is_directory(p))
            continue;

        for (auto& e : std::filesystem::directory_iterator(p))
        {
            if (!e.is_regular_file()) continue;
            auto ext = e.path().extension().string();
            for (auto & c : ext) c = (char)std::tolower(c);
            if (ext == ".ttf" || ext == ".otf")
            {
                std::string fp = e.path().string();
                ImFont* f = io.Fonts->AddFontFromFileTTF(fp.c_str(), fontSize, &font_cfg, io.Fonts->GetGlyphRangesJapanese());
                if (f)
                {
                    std::cerr << "Loaded font: " << fp << " for Japanese support.\n";
                    loadedFont = f;
                    fontLoaded = true;
                    break;
                }
                else
                {
                    std::cerr << "Failed to load font file: " << fp << "\n";
                }
            }
        }
        if (fontLoaded) break;
    }

    if (fontLoaded && loadedFont)
    {
        // フォントをデフォルトに設定
        io.FontDefault = loadedFont;
        // フォントテクスチャはバックエンドが最初の NewFrame() 時に自動生成する（新しい ImGui 実装）
    }
    else
    {
        std::cerr << "Warning: Japanese font not found in fonts/. Falling back to default font.\n";
    }

    return true;
}

void NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Render()
{
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// --- UI 描画（TemplateManager を受け取り内部でテンプレート読み込み/編集を行う） ---
void DrawTemplateUI(model::TemplateManager& tm)
{
    static int selected = 0;
    static model::TemplateData currentTemplate;
    static bool templateLoaded = false;
    static std::string generatedMarkdown;

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
        // 各フィールドの入力
        for (auto& field : currentTemplate.fields)
        {
            ImGui::Text("%s", field.label.c_str());
            if (field.type == "text")
            {
                char buf[256];
                std::snprintf(buf, sizeof(buf), "%s", field.value.c_str());
                if (ImGui::InputText(("##" + field.label).c_str(), buf, sizeof(buf)))
                    field.value = buf;
            }
            else if (field.type == "multiline")
            {
                char buf[2048];
                strncpy(buf, field.value.c_str(), sizeof(buf));
                if (ImGui::InputTextMultiline(("##" + field.label).c_str(), buf, sizeof(buf), ImVec2(-1, ImGui::GetTextLineHeight() * 6)))
                    field.value = buf;
            }
            ImGui::Separator();
        }

        if (ImGui::Button("Export as Markdown"))
        {
            std::stringstream out;
            out << "# " << currentTemplate.name << "\n\n";
            for (auto& f : currentTemplate.fields)
            {
                out << "## " << f.label << "\n" << f.value << "\n\n";
            }
            generatedMarkdown = out.str();
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
}

} // namespace ui
