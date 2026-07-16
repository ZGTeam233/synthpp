//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "../core/score.h"
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdint>
#include <iostream>
#include "renderer.h"

// 写入 WAV 文件（44字节头 + PCM数据）
void writeWav(const std::string& filename, const std::vector<float>& samples, int sampleRate) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "无法创建 WAV 文件\n";
        return;
    }

    int32_t numSamples = static_cast<int32_t>(samples.size());
    int32_t dataSize = numSamples * sizeof(float);  // 32-bit float
    int32_t byteRate = sampleRate * sizeof(float) * 2; // 双声道，但这里简化成单声道

    // 写入 RIFF 头
    file.write("RIFF", 4);
    int32_t chunkSize = 36 + dataSize;
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    int32_t subchunk1Size = 16;
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    int16_t audioFormat = 3; // IEEE Float
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    int16_t numChannels = 2; // 双声道，左右相同
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    int32_t sampleRate32 = sampleRate;
    file.write(reinterpret_cast<const char*>(&sampleRate32), 4);
    int32_t byteRate32 = sampleRate * numChannels * sizeof(float);
    file.write(reinterpret_cast<const char*>(&byteRate32), 4);
    int16_t blockAlign = numChannels * sizeof(float);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    int16_t bitsPerSample = 32;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    int32_t dataSize32 = static_cast<int32_t>(dataSize);
    file.write(reinterpret_cast<const char*>(&dataSize32), 4);

    // 写入音频数据（双声道交错）
    for (float s : samples) {
        file.write(reinterpret_cast<const char*>(&s), sizeof(float)); // 左
        file.write(reinterpret_cast<const char*>(&s), sizeof(float)); // 右
    }
    std::cout << "WAV 文件已生成: " << filename << "\n";
}

// 核心渲染函数
void renderScoreToWav(const synthpp::Score& score, const std::string& outputFile, int sampleRate = 44100) {
    // 1. 计算总时长（最后一个音符结束时间）
    int totalMs = 0;
    for (const auto& note : score.getNotes()) {
        int end = note.startMs + note.durationMs;
        if (end > totalMs) totalMs = end;
    }
    if (totalMs == 0) {
        std::cerr << "没有音符，无法渲染\n";
        return;
    }

    int totalSamples = static_cast<int>((static_cast<long long>(totalMs) * sampleRate) / 1000);
    std::vector<float> buffer(totalSamples, 0.0f);

    // 2. 遍历每个音符，叠加波形到缓冲区
    for (const auto& note : score.getNotes()) {
        int startSample = static_cast<int>((static_cast<long long>(note.startMs) * sampleRate) / 1000);
        int numSamples = static_cast<int>((static_cast<long long>(note.durationMs) * sampleRate) / 1000);

        double phase = 0.0;
        double freqStart = note.freqStart;
        double freqEnd = note.freqEnd;
        bool isSlide = (std::abs(freqStart - freqEnd) > 0.01);

        for (int i = 0; i < numSamples && (startSample + i) < totalSamples; ++i) {
            double t = (double)i / numSamples; // 0→1
            double currentFreq;
            if (isSlide) {
                // 线性插值滑音（你也可以换成指数或贝塞尔曲线）
                currentFreq = freqStart + (freqEnd - freqStart) * t;
            } else {
                currentFreq = freqStart;
            }
            double phaseIncrement = 2.0 * 3.141592653589793 * currentFreq / sampleRate;
            phase += phaseIncrement;
            if (phase > 2.0 * 3.141592653589793) phase -= 2.0 * 3.141592653589793;

            // 关键：叠加！多个音符同时响起时，波形直接相加
            buffer[startSample + i] += static_cast<float>(std::sin(phase));
        }
    }

    // 3. 归一化，防止削波
    float maxAmp = 0.0f;
    for (float s : buffer) {
        if (std::abs(s) > maxAmp) maxAmp = std::abs(s);
    }
    if (maxAmp > 0.01f) {
        for (float& s : buffer) s /= (maxAmp * 1.2f); // 留一点余量
    }

    // 4. 写入 WAV
    writeWav(outputFile, buffer, sampleRate);
}