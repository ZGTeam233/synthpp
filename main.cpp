//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "src/json_melody.h"
#include "src/score.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // 可选：同时设置输入代码页
#endif // _WIN32
    try {
        // 1. 从 JSON 加载旋律
        synthpp::JsonMelody melody("out.json");

        // 2. 创建 Score 并填充
        synthpp::Score score;
        melody.fillScore(score);

        // 3. 现在 score 里存储了所有音符（频率+时长），可按顺序播放
        // 例如：遍历播放
        score.reset();
        while (score.hasMore()) {
            auto note = score.nextNote();
            // 输出或播放 note.frequency, note.durationMs
            std::cout << "频率: " << note.frequency << " Hz, 时长: " << note.durationMs << " ms\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
