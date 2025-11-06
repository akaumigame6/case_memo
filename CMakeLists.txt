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

# ==== 実行ファイル ====
add_executable(MemoApp
    src/main.cpp
    ${IMGUI_SOURCES}
)

# ==== インクルードパス ====
target_include_directories(MemoApp PRIVATE
    imgui
    imgui/backends
)

# ==== リンク設定 ====
target_link_libraries(MemoApp PRIVATE
    glfw
    OpenGL::GL
)

# ==== Windowsのコンソールを隠したい場合（任意） ====
if(WIN32)
    set_target_properties(MemoApp PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
