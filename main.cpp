//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "player.h"
#include "twinkle_star.h"
#include "audio_engine.h"

using namespace synthpp;

int main() {
    using namespace synthpp;

    TwinkleStar melody;
    Score score = melody.generateScore();  // 或直接 melody.fillScore(score);

    AudioEngine engine(44100);
    Player player(engine, score);
    player.play();

    return 0;
}