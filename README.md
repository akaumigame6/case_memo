# Case Memo — ImGuiで作るシンプルなメモツール（開発中）

## 概要
Dear ImGuiを使用した軽量な技術メモ・事例メモ管理ツールです。  
ローカルフォルダ内にMarkdown形式のメモを保存し、検索・タグ管理・テンプレート化をサポートします。

目的:
- シンプルで起動が早いメモアプリ
- ImGui を学びつつ、実用的な機能（検索、タグ付け、ファイル保存）を追加する

---

## 🚀 機能（予定を含む）

- メモの作成・検索・編集  
- タグの追加 / 削除 / 編集  
- テンプレート管理  
- 設定画面で参照フォルダを指定  

---

## 🛠 使用技術

- **C++17**
- **CMake**
- **Dear ImGui**（UIライブラリ）
- **GLFW**（ウィンドウ管理）
- **OpenGL**（描画バックエンド）

---

## ⚖️ ライセンス・クレジット

本ツールは以下のOSSを使用しています：

- [Dear ImGui](https://github.com/ocornut/imgui) — MIT License  
  © Omar Cornut and contributors

- [GLFW](https://github.com/glfw/glfw) — zlib/libpng License

本ツール自身は [MIT License](LICENSE) に基づいて公開しています。

---

## 🧩 ビルド方法

```bash
git clone https://github.com/あなたのユーザー名/MemoApp.git
cd MemoApp
mkdir build && cd build
cmake ..
cmake --build .