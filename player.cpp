//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "player.h"
#include <iostream>
#include <iomanip>

namespace synthpp {
void Player::play() {
    if (!engine.init())
        return;
    if (!engine.start())
        return;

    score.reset();
    while (score.hasMore()) {
        Note note = score.nextNote();
        engine.setFrequency(note.frequency);
        std::cout << std::fixed << std::setprecision(2) 
            << "Playing " << note.frequency << " Hz for " << note.durationMs << " ms\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(note.durationMs));
        }

        engine.stop();
        std::cout << "Playback finished." << std::endl;
    }
} // synthpp