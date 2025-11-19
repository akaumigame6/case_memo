#pragma once
#include "TemplateManager.h"

// 入力フォームを描画して、入力結果をTemplateDataに格納する
void DrawTemplateEditor(TemplateData& data);

// 入力結果をMarkdownテキストに変換
std::string GenerateMarkdown(const TemplateData& data);