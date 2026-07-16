//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <cstdint>

namespace synthpp {

    struct Note {
        double frequency;   // Hz
        int64_t start_ms;   // 绝对开始时间（毫秒）
        int duration_ms;    // 持续时长（毫秒）

        Note(double freq = 0.0, int64_t start = 0, int dur = 0)
            : frequency(freq), start_ms(start), duration_ms(dur) {}
    };

} // namespace synthpp