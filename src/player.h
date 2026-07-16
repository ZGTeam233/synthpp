//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#ifndef SYNTHPP_PLAYER_H
#define SYNTHPP_PLAYER_H

#include <thread>
#include <chrono>
#include "audio_engine.h"
#include "score.h"

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

#endif // SYNTHPP_PLAYER_H