#include "MarkdownEditor.h"
#include <imgui.h>
#include <sstream>

void DrawTemplateEditor(TemplateData& data)
{
    for (auto& field : data.fields)
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
}

std::string GenerateMarkdown(const TemplateData& data)
{
    std::stringstream out;
    out << "# " << data.name << "\n\n";
    for (auto& f : data.fields)
    {
        out << "## " << f.label << "\n" << f.value << "\n\n";
    }
    return out.str();
}
