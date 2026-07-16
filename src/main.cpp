//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "../player.h"
#include "../twinkle_star.h"
#include "audio_engine.h"
#include <fstream>

using namespace synthpp;

int main(int argc, char *argv[]) {
    std::ofstream log("debug.log");
    log << "main entered" << std::endl;
    log.close();
    std::cout << "main entered, argc=" << argc << std::endl;
    std::fflush(stdout);
    using namespace synthpp;

    TwinkleStar melody;
    Score score = melody.generateScore();  // 或直接 melody.fillScore(score);

    AudioEngine engine(44100);
    Player player(engine, score);
    player.play();

    return 0;
}