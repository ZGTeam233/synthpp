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

        void start(double frequency, int64_t end_time_ms);
        void stop();
        bool is_active(int64_t current_time_ms) const;
        float render_sample(double sample_rate, int64_t current_time_ms, bool slide = false);

        // 如果启用滑音，可设置目标频率和起始时间
        void set_slide_target(double target_freq, int64_t start_slide_ms, int64_t end_slide_ms);

    private:
        bool active_ = false;
        double freq_ = 0.0;
        double phase_ = 0.0;
        int64_t end_time_ms_ = 0;

        // 滑音参数（暂未实现，留作扩展）
        bool sliding_ = false;
        double freq_start_ = 0.0;
        double freq_end_ = 0.0;
        int64_t slide_start_ms_ = 0;
        int64_t slide_end_ms_ = 0;
    };
} // synthpp
