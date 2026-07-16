//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "audio_engine.h"
#include <numbers>

namespace synthpp {
    // 静态回调函数（PortAudio要求）
    int AudioEngine::paCallback(const void* input, void* output,
                            unsigned long frameCount,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData) {
        auto* engine = static_cast<AudioEngine*>(userData);
        return engine->renderSamples(output, frameCount);
    }

    // 实际的样本渲染方法
    int AudioEngine::renderSamples(void* output, unsigned long frameCount) {
        auto* out = static_cast<float*>(output);
        double phaseIncrement = currentFrequency * 2.0 * std::numbers::pi / sampleRate;

        for (unsigned int i = 0; i < frameCount; ++i) {
            float sample = static_cast<float>(std::sin(phase));  // 使用 std::sin
            *out++ = sample; // 左
            *out++ = sample; // 右
            phase += phaseIncrement;
            if (phase > 2.0 * std::numbers::pi)
                phase -= 2.0 * std::numbers::pi;
        }
        return paContinue;
    }

    bool AudioEngine::init() {
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }

        PaStreamParameters outputParams{};
        outputParams.device = Pa_GetDefaultOutputDevice();
        if (outputParams.device == paNoDevice) {
            std::cerr << "No default output device." << std::endl;
            return false;
        }
        outputParams.channelCount = 2;
        outputParams.sampleFormat = paFloat32;
        outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;

        err = Pa_OpenStream(&stream,  nullptr, // no input
                                &outputParams, sampleRate,
                                paFramesPerBufferUnspecified,
                                paClipOff, paCallback, this);
        if (err != paNoError) {
            std::cerr << "Pa_OpenStream error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
        return true;
    }

    bool AudioEngine::start() {
        PaError err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "Pa_StartStream error: " << Pa_GetErrorText(err) << std::endl;
            return false;
        }
        return true;
    }

    void AudioEngine::stop() {
        if (stream) {
            Pa_StopStream(stream);
            Pa_CloseStream(stream);
            stream = nullptr;
        }
    }

    void AudioEngine::setFrequency(double freq) {
        currentFrequency = freq;
    }

    void AudioEngine::resetPhase() {
        phase = 0.0;
    }
} // synthpp