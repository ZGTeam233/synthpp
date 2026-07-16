//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "synth_engine.h"
#include <algorithm>
#include <iostream>

namespace synthpp {
    SynthEngine::SynthEngine(double sample_rate) : sample_rate_(sample_rate) {}

    void SynthEngine::add_note(double frequency, int64_t start_ms, int duration_ms) {
        Voice v;
        v.start(frequency, start_ms, duration_ms);
        voices_.push_back(v);
    }

    void SynthEngine::advance_time(int64_t now_ms) {
        // 移除所有已结束的 Voice（可选，避免积累）
        voices_.erase(std::remove_if(voices_.begin(), voices_.end(),
            [now_ms](const Voice& v) { return !v.is_active(now_ms); }),
            voices_.end());
    }

    void SynthEngine::render_audio(float* output, int num_samples, int64_t block_start_ms) {
        std::fill(output, output + num_samples, 0.0f);

        // 不要提前返回！直接遍历所有采样
        for (int i = 0; i < num_samples; ++i) {
            int64_t sample_time_ms = static_cast<int64_t>(block_start_ms + static_cast<double>(i) * 1000.0 / sample_rate_);
            float mixed = 0.0f;
            int active_count = 0;
            for (auto& v : voices_) {
                if (v.is_active(sample_time_ms)) {
                    mixed += v.render_sample(sample_rate_, sample_time_ms);
                    ++active_count;
                }
            }
            if (active_count > 0) {
                output[i] = mixed / static_cast<float>(active_count);
            }
        }
    }

    void SynthEngine::clear() {
        voices_.clear();
    }
} // synthpp