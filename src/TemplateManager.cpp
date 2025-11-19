#include "TemplateManager.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace fs = filesystem;

// --- JSONを安全に読み込む ---
static string LoadFileUTF8(const string& path)
{
    ifstream ifs(path, ios::binary); // ロケール依存を避ける
    if (!ifs.is_open()) {
        cerr << "Failed to open: " << path << endl;
        return "";
    }

    ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str(); // UTF-8文字列として扱う
}

// --- 簡易パーサー（UTF-8対応） ---
static string ExtractString(const string& src, const string& key)
{
    size_t pos = src.find("\"" + key + "\"");
    if (pos == string::npos) return "";

    pos = src.find(':', pos);
    if (pos == string::npos) return "";

    size_t q1 = src.find('"', pos + 1);
    size_t q2 = src.find('"', q1 + 1);
    if (q1 == string::npos || q2 == string::npos) return "";
    // バイト単位の切り出しだが、UTF-8では問題ない（ImGuiはUTF-8前提）
    return src.substr(q1 + 1, q2 - q1 - 1);
}

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
    string json = LoadFileUTF8(path);
    if (json.empty()) return data;

    data.name = ExtractString(json, "name");

    // "label" と "type" を順に抽出
    size_t pos = 0;
    while ((pos = json.find("\"label\"", pos)) != string::npos)
    {
        TemplateField field;
        field.label = ExtractString(json.substr(pos), "label");
        field.type = ExtractString(json.substr(pos), "type");
        data.fields.push_back(field);
        pos += 10;
    }

    return data;
}
