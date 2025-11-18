cmake_minimum_required(VERSION 3.16)
project(MemoApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==== 必要ライブラリ ====
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# ==== Dear ImGui のソース ====
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# --- 自動でsrc以下のcppを全て取得（main.cpp含む）---
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# --- includeディレクトリを登録 ---
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# ==== 実行ファイル ====
add_executable(MemoApp
    ${SRC_FILES}
    ${IMGUI_SOURCES}
)

# ==== リンク設定 ====
target_link_libraries(MemoApp PRIVATE
    glfw
    OpenGL::GL
)

target_link_libraries(MemoApp PUBLIC imm32.lib)

if(WIN32)
    # Link Windows IME library for Imm* functions used in ImeHelper
    target_link_libraries(MemoApp PRIVATE imm32)
endif()

# ==== Windowsのコンソールを隠したい場合（任意） ====
if(WIN32)
    set_target_properties(MemoApp PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
