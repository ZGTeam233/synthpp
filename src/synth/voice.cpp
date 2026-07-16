//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "voice.h"
#include <cmath>

namespace synthpp {
    void Voice::start(double frequency, int64_t start_time_ms, int duration_ms) {
        freq_ = frequency;
        start_time_ms_ = start_time_ms;
        duration_ms_ = duration_ms;
        end_time_ms_ = start_time_ms + duration_ms;
        active_ = true;
        // 重要：相位保留以实现连续（但可能导致起始相位跳跃，为了平滑应保留）
        // 我们保留 phase_ 不变
    }

    void Voice::stop() {
        active_ = false;
    }

    bool Voice::is_active(int64_t current_time_ms) const {
        return active_ && current_time_ms >= start_time_ms_ && current_time_ms < end_time_ms_;
    }

    float Voice::render_sample(double sample_rate, int64_t current_time_ms) {
        if (!is_active(current_time_ms)) {
            return 0.0f;
        }

        // 计算相对时间（毫秒）
        int64_t elapsed = current_time_ms - start_time_ms_;
        if (elapsed < 0) return 0.0f;

        // 淡入淡出（5ms）
        const int fade_ms = 5;
        float gain = 1.0f;
        if (elapsed < fade_ms) {
            gain = static_cast<float>(elapsed) / fade_ms;
        } else if (duration_ms_ - elapsed < fade_ms) {
            gain = static_cast<float>(duration_ms_ - elapsed) / fade_ms;
        }

        // 相位累加（连续）
        double phase_increment = 2.0 * 3.141592653589793 * freq_ / sample_rate;
        phase_ += phase_increment;
        if (phase_ > 2.0 * 3.141592653589793) {
            phase_ -= 2.0 * 3.141592653589793;
        }

        return gain * static_cast<float>(std::sin(phase_));
    }
} // synthpp