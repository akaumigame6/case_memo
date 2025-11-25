#include "TemplateManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
namespace fs = std::filesystem;

namespace model {

TemplateManager::TemplateManager(const string& dir)
    : directory(dir)
{
}

void TemplateManager::LoadTemplates()
{
    templateNames.clear();
    try {
        for (auto& e : fs::directory_iterator(directory)) {
            if (e.path().extension() == ".json") {
                templateNames.push_back(e.path().stem().string());
            }
        }
    }
    catch (exception& e) {
        cerr << "Template load error: " << e.what() << std::endl;
    }
}

const vector<string>& TemplateManager::GetTemplateNames() const
{
    return templateNames;
}

TemplateData TemplateManager::LoadTemplate(const std::string& name) const
{
    TemplateData data;
    string path = directory + "/" + name + ".json";
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        cerr << "Failed to open template file: " << path << std::endl;
        return data;
    }

    try {
        nlohmann::json j;
        ifs >> j;

        if (j.contains("name") && j["name"].is_string())
            data.name = j["name"].get<string>();

        // fields は配列であることを期待
        if (j.contains("fields") && j["fields"].is_array()) {
            for (const auto& item : j["fields"]) {
                if (!item.is_object()) continue;
                TemplateField field;
                field.label = item.value("label", std::string());
                field.type = item.value("type", std::string());
                field.value = item.value("value", std::string());
                data.fields.push_back(std::move(field));
            }
        }
        else {
            // 古いフォーマット互換： top-level に label/type が複数ある場合の簡易処理は省略
        }
    }
    catch (const nlohmann::json::parse_error& e) {
        cerr << "JSON parse error in " << path << ": " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        cerr << "Error reading template " << path << ": " << e.what() << std::endl;
    }

    return data;
}

} // namespace model
