//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <string>
#include <vector>
#include "../core/melody.h"

namespace synthpp {
    // 从 JSON 文件加载音符数据的 Melody 实现
    class JsonMelody : public Melody {
    public:
        // 构造函数：传入 JSON 文件路径，立即解析
        explicit JsonMelody(const std::string& jsonFilePath);

        // 重写纯虚函数：将解析的音符填充到 Score
        void fillScore(Score& score) const override;

    private:
        // 内部音符数据结构（仅存储必要字段）
        struct NoteData {
            int pitch;         // MIDI 音高编号（0-127）
            double duration;   // 持续时间（秒）
            // 注意：我们忽略 start，因为 Score 不支持绝对时间
        };

        std::vector<NoteData> notes_;  // 存储所有音符

        // 辅助函数：将 MIDI 编号转为频率（Hz）
        static double midiToFrequency(int pitch);
    };
} // synthpp
