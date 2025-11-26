#include "TemplateManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace model {

/**
 * @brief Construct a TemplateManager for managing JSON templates in a directory.
 *
 * @param dir Filesystem path to the directory that contains template JSON files.
 */
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

/**
 * @brief Load a template by name from the manager's directory and parse its JSON representation.
 *
 * If the file cannot be opened or the JSON cannot be parsed, an empty TemplateData is returned and an error is logged.
 *
 * @return TemplateData The parsed template (fields and optional name). Returns an empty TemplateData on error or if the file is missing/invalid.
 */
TemplateData TemplateManager::LoadTemplate(const std::string& name) const
{
    TemplateData data;
    std::string path = directory + "/" + name + ".json";
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open template file: " << path << std::endl;
        return data;
    }

    try {
        nlohmann::json j;
        ifs >> j;

        if (j.contains("name") && j["name"].is_string())
            data.name = j["name"].get<std::string>();

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
        std::cerr << "JSON parse error in " << path << ": " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error reading template " << path << ": " << e.what() << std::endl;
    }

    return data;
}

} // namespace model