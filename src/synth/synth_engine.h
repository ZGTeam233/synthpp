//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <vector>
#include <cstdint>
#include "voice.h"

namespace synthpp {
    class SynthEngine {
    public:
        explicit SynthEngine(double sample_rate = 44100.0);

        void add_note(double frequency, int64_t start_ms, int duration_ms);
        void advance_time(int64_t now_ms);  // 移除结束的 Voice
        void render_audio(float* output, int num_samples, int64_t current_time_ms);

        void clear();

    private:
        double sample_rate_;
        std::vector<Voice> voices_;
    };
} // synthpp
