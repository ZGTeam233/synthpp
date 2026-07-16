cmake_minimum_required(VERSION 3.15)
project(synthpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- 使用 pkg-config 查找 PortAudio ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

# ---- 生成可执行文件 ----
add_executable(synthpp main.cpp
    score.h
    score.cpp
    audio_engine.h
    audio_engine.cpp
    player.h
    player.cpp
    melody.h
    twinkle_star.h
    melody.cpp)

# ---- 链接 PortAudio（通过 pkg-config 提供的变量） ----
target_include_directories(synthpp PRIVATE ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(synthpp PRIVATE ${PORTAUDIO_LIBRARIES})
target_compile_options(synthpp PRIVATE ${PORTAUDIO_CFLAGS_OTHER})