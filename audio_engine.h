//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#ifndef SYNTHPP_AUDIO_ENGINE_H
#define SYNTHPP_AUDIO_ENGINE_H

#include <portaudio.h>
#include <cmath>
#include <iostream>

namespace synthpp {
    class AudioEngine {
        PaStream* stream = nullptr;
        double sampleRate = 44100.0;
        double phase = 0.0;
        double currentFrequency = 440.0; // 默认A4

        // 静态回调函数（PortAudio要求）
        static int paCallback(const void*, void*, unsigned long,
                            const PaStreamCallbackTimeInfo*,
                            PaStreamCallbackFlags, void*);

        // 实际的样本渲染方法
        int renderSamples(void*, unsigned long);

    public:
        AudioEngine(double rate = 44100.0) : sampleRate(rate) {}
        ~AudioEngine() { stop(); Pa_Terminate(); }
        bool init();
        bool start();
        void stop();

        // 线程安全地改变频率（回调中会读取）
        void setFrequency(double);

        // 重置相位（可选，用于避免音符间相位不连续）
        void resetPhase();
    };
} // synthpp

#endif // SYNTHPP_AUDIO_ENGINE_H