//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "voice.h"

namespace synthpp {
    void Voice::start(double frequency, int64_t end_time_ms) {
        freq_ = frequency;
        end_time_ms_ = end_time_ms;
        phase_ = 0.0;
        active_ = true;
        sliding_ = false;
    }

    void Voice::stop() {
        active_ = false;
    }

    bool Voice::is_active(int64_t current_time_ms) const {
        return active_ && current_time_ms < end_time_ms_;
    }

    float Voice::render_sample(double sample_rate, int64_t current_time_ms, bool /*slide*/) {
        if (!is_active(current_time_ms)) {
            return 0.0f;
        }

        // 简单正弦波
        double phase_increment = 2.0 * 3.141592653589793 * freq_ / sample_rate;
        phase_ += phase_increment;
        if (phase_ > 2.0 * 3.141592653589793) {
            phase_ -= 2.0 * 3.141592653589793;
        }
        return static_cast<float>(std::sin(phase_));
    }

    void Voice::set_slide_target(double target_freq, int64_t start_slide_ms, int64_t end_slide_ms) {
        // 暂不实现，留作扩展
    }
} // synthpp