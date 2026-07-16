//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "wav_renderer.h"
#include "../core/score.h"
#include "../synth/synth_engine.h"
#include <fstream>
#include <iostream>
#include <cmath>

namespace synthpp {
    bool WavRenderer::render_to_file(const Score& score,
                                 const std::string& filename,
                                 int sample_rate) {
        if (score.notes().empty()) {
            std::cerr << "WavRenderer: Score is empty.\n";
            return false;
        }

        int64_t total_ms = score.total_duration_ms();
        if (total_ms == 0) {
            std::cerr << "WavRenderer: Total duration is zero.\n";
            return false;
        }

        // 计算总采样数
        int64_t total_samples = total_ms * sample_rate / 1000;
        std::vector<float> buffer(total_samples, 0.0f);

        SynthEngine engine(sample_rate);

        // 添加所有音符
        for (const auto& note : score.notes()) {
            engine.add_note(note.frequency, note.start_ms, note.duration_ms);
        }

        // 逐块渲染
        const int block_samples = 256;
        std::vector<float> block(block_samples);

        int64_t current_ms = 0;
        int64_t samples_rendered = 0;
        while (samples_rendered < total_samples) {
            int samples_to_render = std::min<int64_t>(block_samples, total_samples - samples_rendered);
            engine.render_audio(block.data(), samples_to_render, current_ms);
            // 复制数据
            std::copy(block.begin(), block.begin() + samples_to_render,
                      buffer.begin() + samples_rendered);
            samples_rendered += samples_to_render;
            current_ms += static_cast<int64_t>(samples_to_render * 1000.0 / sample_rate);
        }

        write_wav(filename, buffer, sample_rate);
        return true;
    }

    void WavRenderer::write_wav(const std::string& filename,
                            const std::vector<float>& samples,
                            int sample_rate) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to create WAV file: " << filename << std::endl;
            return;
        }

        // 修正：实际数据量 = 采样数 × 通道数 × 每个样本字节数
        int32_t num_samples = static_cast<int32_t>(samples.size());
        int16_t num_channels = 2;
        int32_t sample_rate_32 = sample_rate;
        int16_t bits_per_sample = 32;
        int16_t block_align = num_channels * (bits_per_sample / 8); // 4 字节/样本 × 2 通道 = 8
        int32_t byte_rate = sample_rate * block_align;  // 每秒字节数
        int32_t data_size = num_samples * block_align;  // 数据区总字节数
        int32_t chunk_size = 36 + data_size;

        // 写入 RIFF 头
        file.write("RIFF", 4);
        file.write(reinterpret_cast<const char*>(&chunk_size), 4);
        file.write("WAVE", 4);
        file.write("fmt ", 4);
        int32_t subchunk1_size = 16;
        file.write(reinterpret_cast<const char*>(&subchunk1_size), 4);
        int16_t audio_format = 3; // IEEE Float
        file.write(reinterpret_cast<const char*>(&audio_format), 2);
        file.write(reinterpret_cast<const char*>(&num_channels), 2);
        file.write(reinterpret_cast<const char*>(&sample_rate_32), 4);
        file.write(reinterpret_cast<const char*>(&byte_rate), 4);
        file.write(reinterpret_cast<const char*>(&block_align), 2);
        file.write(reinterpret_cast<const char*>(&bits_per_sample), 2);
        file.write("data", 4);
        file.write(reinterpret_cast<const char*>(&data_size), 4);

        // 写入音频数据（交错双声道）
        for (float s : samples) {
            file.write(reinterpret_cast<const char*>(&s), sizeof(float));
            file.write(reinterpret_cast<const char*>(&s), sizeof(float));
        }

        std::cout << "WAV file written: " << filename << " ("
                  << num_samples << " samples, " << data_size << " bytes)" << std::endl;
    }
} // synthpp