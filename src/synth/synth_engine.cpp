//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "synth_engine.h"
#include <algorithm>

namespace synthpp {
    SynthEngine::SynthEngine(double sample_rate) : sample_rate_(sample_rate) {}

    void SynthEngine::add_note(double frequency, int64_t start_ms, int duration_ms) {
        // 查找一个空闲的 Voice（即当前不活跃的）
        for (auto& v : voices_) {
            if (!v.is_active(start_ms)) {
                v.start(frequency, start_ms, duration_ms);
                return;
            }
        }
        // 若没有空闲，新建
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
        // 清空输出
        std::fill(output, output + num_samples, 0.0f);

        // 如果没有活跃 Voice，直接返回（全零）
        bool has_active = false;
        for (const auto& v : voices_) {
            if (v.is_active(block_start_ms) || v.is_active(block_start_ms + num_samples * 1000 / sample_rate_)) {
                has_active = true;
                break;
            }
        }
        if (!has_active) {
            return;
        }

        // 对每个采样累加活跃 Voice 的贡献
        for (int i = 0; i < num_samples; ++i) {
            // 计算当前采样对应的精确时间（毫秒）
            int64_t sample_time_ms = block_start_ms + static_cast<int64_t>(i * 1000.0 / sample_rate_);
            float mixed = 0.0f;
            int active_count = 0;
            for (auto& v : voices_) {
                if (v.is_active(sample_time_ms)) {
                    mixed += v.render_sample(sample_rate_, sample_time_ms);
                    ++active_count;
                }
            }
            if (active_count > 0) {
                // 归一化防止削波
                output[i] = mixed / active_count;
            }
        }
    }

    void SynthEngine::clear() {
        voices_.clear();
    }
} // synthpp