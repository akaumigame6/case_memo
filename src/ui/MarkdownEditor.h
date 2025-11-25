#pragma once
#include "model/TemplateManager.h"

namespace ui {

// UIの初期化/フレーム/描画/終了を分離
bool Init(void* window);
void NewFrame();
void Render();
void Shutdown();

// モデル（TemplateManager）を受け取りUIを描画する
void DrawTemplateUI(model::TemplateManager& tm);

} // namespace ui
