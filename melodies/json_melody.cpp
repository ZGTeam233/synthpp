//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "json_melody.h"
#include "../third_party/json.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

using json = nlohmann::json;

namespace synthpp {

    JsonMelody::JsonMelody(const std::string& jsonFilePath) {
        // 1. 读取 JSON 文件
        std::ifstream file(jsonFilePath);
        if (!file.is_open()) {
            throw std::runtime_error("无法打开 JSON 文件: " + jsonFilePath);
        }

        json j;
        try {
            file >> j;
        } catch (const json::parse_error& e) {
            throw std::runtime_error("JSON 解析错误: " + std::string(e.what()));
        }

        // 2. 确保 JSON 是数组
        if (!j.is_array()) {
            throw std::runtime_error("JSON 根元素必须是数组");
        }

        // 3. 遍历数组，提取音符数据（只取 pitch 和 duration）
        notes_.reserve(j.size());
        for (const auto& item : j) {
            // 每个对象应有 pitch 和 duration 字段（start 可忽略）
            if (!item.contains("pitch") || !item.contains("duration")) {
                std::cerr << "警告: 某条记录缺少 pitch 或 duration 字段，已跳过\n";
                continue;
            }
            NoteData nd;
            nd.pitch = item["pitch"].get<int>();
            nd.duration = item["duration"].get<double>();

            // 简单有效性检查
            if (nd.duration < 0) nd.duration = 0;
            if (nd.pitch < 0 || nd.pitch > 127) {
                std::cerr << "警告: 音高 " << nd.pitch << " 超出 MIDI 范围 (0-127)，已跳过\n";
                continue;
            }
            notes_.push_back(nd);
        }

        std::cout << "从 JSON 加载了 " << notes_.size() << " 个音符\n";
    }

    double JsonMelody::midiToFrequency(int pitch) {
        // 标准公式：A4 = 440Hz, pitch = 69
        return 440.0 * std::pow(2.0, (pitch - 69) / 12.0);
    }

    void JsonMelody::fillScore(Score& score) const {
        // 将每个音符添加到 Score（忽略 start 时间，按顺序连续播放）
        for (const auto& note : notes_) {
            double freq = midiToFrequency(note.pitch);
            int durationMs = static_cast<int>(note.duration * 1000 + 0.5); // 四舍五入
            if (durationMs < 0) durationMs = 0;
            score.addNote(Note(freq, durationMs));
        }
    }

} // namespace synthpp