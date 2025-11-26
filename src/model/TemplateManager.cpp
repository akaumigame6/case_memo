#include "TemplateManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace model {

// --- JSONを安全に読み込む ---
static std::string LoadFileUTF8(const std::string& path)
{

    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open: " << path << std::endl;
        return "";
    }

    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

// --- 簡易パーサー（UTF-8対応） ---
static std::string ExtractString(const std::string& src, const std::string& key)
{
    size_t pos = src.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";

    pos = src.find(':', pos);
    if (pos == std::string::npos) return "";

    size_t q1 = src.find('"', pos + 1);
    size_t q2 = src.find('"', q1 + 1);
    if (q1 == std::string::npos || q2 == std::string::npos) return "";
    return src.substr(q1 + 1, q2 - q1 - 1);
}

TemplateManager::TemplateManager(const std::string& dir)
    : directory(dir)
{
}

void TemplateManager::LoadTemplates()
{
    templateNames.clear();
    try {
        for (auto& e : std::filesystem::directory_iterator(directory)) {
            if (e.path().extension() == ".json") {
                templateNames.push_back(e.path().stem().string());
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Template load error: " << e.what() << std::endl;
    }
}

const std::vector<std::string>& TemplateManager::GetTemplateNames() const
{
    return templateNames;
}

TemplateData TemplateManager::LoadTemplate(const std::string& name) const
{
    TemplateData data;
    std::string path = directory + "/" + name + ".json";
    std::string json = LoadFileUTF8(path);
    if (json.empty()) return data;

    data.name = ExtractString(json, "name");

    size_t pos = 0;
    while ((pos = json.find("\"label\"", pos)) != std::string::npos)
    {
        TemplateField field;
        field.label = ExtractString(json.substr(pos), "label");
        field.type = ExtractString(json.substr(pos), "type");
        data.fields.push_back(field);
        pos += 10;
    }

    return data;
}

} // namespace model
