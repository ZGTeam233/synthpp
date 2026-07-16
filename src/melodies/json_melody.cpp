//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "json_melody.h"
#include "../core/note.h"
#include "../utils/midi_utils.h"
#include "../../third_party/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace synthpp {
    JsonMelody::JsonMelody(const std::string& filepath) : filepath_(filepath) {}

    void JsonMelody::fill_score(Score& score) const {
        std::ifstream file(filepath_);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open JSON file: " + filepath_);
        }

        json data;
        try {
            file >> data;
        } catch (const json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }

        if (!data.is_array()) {
            throw std::runtime_error("JSON root must be an array of notes");
        }

        for (const auto& item : data) {
            if (!item.contains("pitch") || !item.contains("start_ms") || !item.contains("duration_ms")) {
                std::cerr << "Warning: skipping invalid note entry\n";
                continue;
            }

            int pitch = item["pitch"].get<int>();
            int64_t start_ms = item["start_ms"].get<int64_t>();
            int duration_ms = item["duration_ms"].get<int>();

            double freq = midi_to_frequency(pitch);
            score.add_note(Note(freq, start_ms, duration_ms));
        }

        score.sort_by_time();  // 确保按时间排序
    }
} // synthpp