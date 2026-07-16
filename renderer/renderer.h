//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <string>

namespace synthpp {
    class Score;
} // namespace synthpp

// 渲染 Score 到 WAV 文件
void renderScoreToWav(const synthpp::Score& score, const std::string& outputFile, int sampleRate = 44100);
