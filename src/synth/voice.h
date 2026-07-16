//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <cstdint>

namespace synthpp {
    // 单个发声单元（复音）
    class Voice {
    public:
        Voice() = default;

        void start(double frequency, int64_t start_time_ms, int64_t duration_ms);
        void stop();
        bool is_active(int64_t current_time_ms) const;
        float render_sample(double sample_rate, int64_t current_time_ms);

    private:
        bool active_ = false;
        double freq_ = 0.0;
        double phase_ = 0.0;
        int64_t start_time_ms_ = 0;
        int64_t duration_ms_ = 0;
        int64_t end_time_ms_ = 0;
    };
} // synthpp
