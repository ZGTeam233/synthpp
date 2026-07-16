cmake_minimum_required(VERSION 3.15)
project(synthpp)

# 启用 C++20 标准（因为用了 std::numbers）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 PortAudio 库（MSYS2 的包会自动提供 cmake 配置文件）
find_package(PortAudio REQUIRED)

# 调试输出
# message(STATUS "PortAudio_FOUND = ${PortAudio_FOUND}")
# message(STATUS "PortAudio_VERSION = ${PortAudio_VERSION}")

# 列出所有以 PortAudio 或 portaudio 开头的目标（如果是 IMPORTED 目标）
# get_cmake_property(_variableNames VARIABLES)
# foreach (_varName IN LISTS _variableNames)
#     if(_varName MATCHES "^PortAudio|^portaudio")
#         message(STATUS "Variable: ${_varName} = ${${_varName}}")
#     endif()
# endforeach()

# 列出所有目标（需要 CMake 3.10+）
get_property(_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
message(STATUS "Imported targets: ${_targets}")

# 包含你自己的头文件目录
include_directories(include)

# 添加可执行文件
add_executable(synthpp main.cpp
            third_party/json.hpp
        src/core/note.h
        src/core/score.cpp
        src/core/score.h
        src/core/melody.h
        src/synth/voice.cpp
        src/synth/voice.h
        src/synth/synth_engine.cpp
        src/synth/synth_engine.h
        src/renderer/wav_renderer.cpp
        src/renderer/wav_renderer.h
        src/utils/midi_utils.h
        src/melodies/twinkle_star.h)

# 链接 PortAudio
target_link_libraries(synthpp portaudio)

# （可选）为 Windows 生成控制台应用，避免弹出窗口
set_target_properties(synthpp PROPERTIES WIN32_EXECUTABLE FALSE)