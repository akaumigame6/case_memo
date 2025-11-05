# Case Memo — ImGuiで作るシンプルなメモツール（開発中）

## 概要

このリポジトリは、C++ と Dear ImGui を使って開発する軽量なメモ（ノート）ツールの雛形です。
開発中の段階で、まずはローカルで素早くメモが取れるデスクトップアプリを目指します。

目的:
- シンプルで起動が早いメモアプリ
- ImGui を学びつつ、実用的な機能（検索、タグ付け、ファイル保存）を追加する

## 現在の状態

- 状態: ひな型README作成（開発初期）
- プラットフォーム: Windows（主開発環境）

## 目標と想定機能

- テキストメモの作成/編集/削除
- メモの保存（ローカルファイル、JSON/Plain）
- 検索（全文検索の簡易版）
- タグ/カテゴリ管理（将来）
- 簡易的な同期（将来・オプション）

## 技術スタック

- 言語: C++17 以上を想定
- GUI: Dear ImGui (https://github.com/ocornut/imgui)
- ウィンドウ/入力/レンダラー: GLFW または SDL2 + OpenGL（プロジェクトで選択）
- ビルド: CMake

## 依存関係（参考）

- Dear ImGui
- GLFW または SDL2
- OpenGL (もしくは選択したレンダラー)
- CMake（3.10 以上推奨）

## Windows（PowerShell）での簡単なビルド手順（サンプル）

以下は一般的な手順の例です。プロジェクトを初期化する際に、CMakeLists.txt に依存の取り込み方法を書いておくと良いです。

1) リポジトリをクローン/作業ディレクトリへ移動

```powershell
cd C:\path\to\workspace
git clone <repo-url> case_memo
cd case_memo
```

2) ビルドディレクトリ作成、CMake 実行

```powershell
mkdir build; cd build
cmake .. -G "Visual Studio 17 2022" -A x64
# または MinGW/MSYS2 を使う場合は適切なジェネレータを指定
cmake --build . --config Release
```

3) 実行ファイルを起動（Visual Studio の場合は .\Release\case_memo.exe 等）

```powershell
.\Release\case_memo.exe
```

※ 上記コマンドはプロジェクトのCMake設定や使用するライブラリによって変わります。

## プロジェクト構成（例）

- src/             : アプリ本体のソース
- include/         : ヘッダ
- third_party/     : ImGuiやラッパーをサブモジュールで置く場合
- resources/       : アイコンやデフォルト設定
- CMakeLists.txt

## ロードマップ（短期・中期）

短期:
- メモの基本作成/編集/削除、ファイル保存機能
- シンプルな検索UI

中期:
- タグ付けとフィルタ
- 設定の永続化
- 単体テストやCIの導入

長期:
- オプションでクラウド同期
- プラグインやエクスポート機能

## 貢献

- Issue や Pull Request を歓迎します。
- 大きな変更を入れる場合は先に Issue で相談してください。

## ライセンス

初期状態では明示的なライセンスは設定していません。公開・共有する場合は適切なOSSライセンス（例: MIT）を追加してください。

---

## デプロイ（CMakeでexeを作る／配布する際の注意）

このプロジェクトを配布する際は CMake を使って実行ファイル（.exe）を生成する想定です。以下は一般的な手順と注意点のメモです。

- ビルド構成: 開発時は Debug、配布時は Release ビルドを使います。Release は最適化とデバッグ情報削減を行います。
- 依存の取り扱い: ImGui 自体はヘッダ＋ソースをプロジェクトに組み込めますが、GLFW/SDL2 や OpenGL ランタイムは別途必要です。vcpkg や MSYS2 を使うと依存管理が楽になります。
- 実行ファイル単体にしたい場合: Windowsで完全に単一exe化（ランタイムを静的リンク）は、使用ライブラリのライセンスやビルド設定に依存します。Visual C++ のランタイムは静的リンクも可能ですが、再頒布ライセンスに注意してください。
- DLL 配布: Visual Studio で動かす場合、多くは exe と同じフォルダに必要な DLL（例: glfw3.dll、SDL2.dll、OpenGLドライバはOS側）を配置します。
- インストーラ/パッケージング: CPack（CMake のパッケージ機能）や NSIS / Inno Setup でインストーラを作成できます。単純な配布なら exe と必要なDLL群を zip にまとめるだけでも十分です。

PowerShell（Windows）での例：

```powershell
# プロジェクトルートで
mkdir build; cd build
# Visual Studio ジェネレータを使う例（例: VS2022 x64）
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
# インストール先へ配置（CMakeLists に install を定義している場合）
cmake --install . --config Release --prefix ../dist
```

CPack でインストーラを作る（CMakeLists.txt 内で CPack を設定している場合）:

```powershell
# build フォルダから
# cpack -G NSIS -C Release
```

配布時のチェックリスト:

- Release ビルドで動作確認を行う
- 実行に必要な DLL が同梱されているか確認（またはランタイムの配布方法を明記）
- 必要なら Visual C++ 再頒布パッケージの案内を README に書く
- ライセンス（使用したライブラリのライセンス）を確認し、配布物にライセンス表記を含める

（注）単体exe化や完全なスタティックリンクは環境依存かつ設定が多いため、まずは「exe + 必要DLL のポータブルフォルダ」か「インストーラ」を推奨します。

---

（注）このREADMEはひな型です。プロジェクトの進行に合わせ、ビルド手順や依存関係の具体的な指示（サブモジュールの初期化、パッケージマネージャ経由の導入など）を追記していきます。

