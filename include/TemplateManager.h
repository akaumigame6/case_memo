// TemplateManager.h
// ==========================
// テンプレート構造(JSON)を読み込み、
// 項目(label, type)ごとの入力フォーム定義を保持する。
// ==========================

#pragma once
#include <string>
#include <vector>

using namespace std;

struct TemplateField {
    string label;
    string type; // "text" or "multiline"
    string value;
};

struct TemplateData {
    string name;
    vector<TemplateField> fields;
};

class TemplateManager {
public:
    TemplateManager(const string& dir);
    void LoadTemplates();

    const vector<string>& GetTemplateNames() const;
    TemplateData LoadTemplate(const string& name) const;
private:
    string directory;
    vector<string> templateNames;
};
