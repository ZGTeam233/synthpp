//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <cmath>

namespace synthpp {

    inline double midi_to_frequency(int pitch) {
        // A4 = 440Hz, pitch 69
        return 440.0 * std::pow(2.0, (pitch - 69) / 12.0);
    }

} // namespace synthpp