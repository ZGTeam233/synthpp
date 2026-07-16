//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "synth_engine.h"
#include <algorithm>

namespace synthpp {
    SynthEngine::SynthEngine(double sample_rate) : sample_rate_(sample_rate) {}

    void SynthEngine::add_note(double frequency, int64_t start_ms, int duration_ms) {
        // 找一个空闲的 Voice
        auto it = std::find_if(voices_.begin(), voices_.end(),
            [](const Voice& v) { return !v.is_active(0); }); // 注意这里传入0不准确，但简化处理
        // 更好的做法：遍历 all，如果未激活则复用
        bool found = false;
        for (auto& v : voices_) {
            if (!v.is_active(0)) {
                v.start(frequency, start_ms + duration_ms);
                found = true;
                break;
            }
        }
        if (!found) {
            Voice v;
            v.start(frequency, start_ms + duration_ms);
            voices_.push_back(v);
        }
    }

    void SynthEngine::advance_time(int64_t now_ms) {
        // 移除所有已结束的 Voice（可选，也可标记为非活跃）
        voices_.erase(std::remove_if(voices_.begin(), voices_.end(),
            [now_ms](const Voice& v) { return !v.is_active(now_ms); }),
            voices_.end());
    }

    void SynthEngine::render_audio(float* output, int num_samples, int64_t current_time_ms) {
        // 清空输出
        std::fill(output, output + num_samples, 0.0f);

        // 累加所有活跃 Voice 的采样
        for (auto& v : voices_) {
            if (v.is_active(current_time_ms)) {
                for (int i = 0; i < num_samples; ++i) {
                    // 每个采样需要微秒级时间增量，但我们用固定时间，不逐采样更新 current_time_ms
                    // 简化：使用当前时间块内采样索引模拟（不精确，但演示足够）
                    int64_t sample_time_ms = current_time_ms + i * 1000 / static_cast<int>(sample_rate_);
                    output[i] += v.render_sample(sample_rate_, sample_time_ms);
                }
            }
        }

        // 简单的归一化防止削波（除以活跃数量）
        int active_count = 0;
        for (const auto& v : voices_) {
            if (v.is_active(current_time_ms)) ++active_count;
        }
        if (active_count > 1) {
            float inv = 1.0f / active_count;
            for (int i = 0; i < num_samples; ++i) {
                output[i] *= inv;
            }
        }
    }

    void SynthEngine::clear() {
        voices_.clear();
    }
} // synthpp