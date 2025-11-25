// TemplateManager.h (model)
#pragma once
#include <string>
#include <vector>

namespace model {

struct TemplateField {
    std::string label;
    std::string type; // "text" or "multiline"
    std::string value;
};

struct TemplateData {
    std::string name;
    std::vector<TemplateField> fields;
};

class TemplateManager {
public:
    TemplateManager(const std::string& dir);
    void LoadTemplates();

    const std::vector<std::string>& GetTemplateNames() const;
    TemplateData LoadTemplate(const std::string& name) const;
private:
    std::string directory;
    std::vector<std::string> templateNames;
};

} // namespace model
