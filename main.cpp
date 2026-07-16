//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "melodies/json_melody.h"
#include "core/score.h"
#include <iostream>
#include "player/player.h"
#include "core/audio_engine.h"
#include "renderer/renderer.h"

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // 可选：同时设置输入代码页
#endif // _WIN32

    try {
        // 1. 加载 JSON
        synthpp::JsonMelody melody("out.json");

        // 2. 创建 Score（已修改为支持绝对时间和滑音）
        synthpp::Score score;
        melody.fillScore(score);  // 这个函数需要改成往新 Score 里填 TimedNote

        // 3. 离线渲染成 WAV
        renderScoreToWav(score, "output.wav", 44100);

        std::cout << "渲染完成！用播放器打开 output.wav 听效果。\n";
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
    }
    return 0;
}
