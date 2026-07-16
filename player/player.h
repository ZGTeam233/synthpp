//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <thread>
#include <chrono>
#include "../core/audio_engine.h"
#include "../core/score.h"

namespace synthpp {
    class Player {
    private:
        AudioEngine& engine;
        Score& score;

    public:
        Player(AudioEngine& eng, Score& sc) : engine(eng), score(sc) {}
        void play();
    };
} // synthpp