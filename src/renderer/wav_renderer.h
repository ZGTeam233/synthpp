//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace synthpp {
    class Score;

    class WavRenderer {
    public:
        // 渲染 Score 到 WAV 文件
        static bool render_to_file(const Score& score,
                                   const std::string& filename,
                                   int sample_rate = 44100);

    private:
        static void write_wav(const std::string& filename,
                              const std::vector<float>& samples,
                              int sample_rate);
    };
} // synthpp
