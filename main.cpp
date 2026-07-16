//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "src/melodies/twinkle_star.h"
#include "src/renderer/wav_renderer.h"
#include <iostream>

int main() {
    synthpp::TwinkleStar melody;
    synthpp::Score score;
    melody.fill_score(score);

    std::cout << "Total notes: " << score.notes().size() << std::endl;
    int64_t totalMs = score.total_duration_ms();
    std::cout << "Total duration: " << totalMs << " ms" << std::endl;

    for (const auto& note : score.notes()) {
        std::cout << "freq=" << note.frequency
                  << " start=" << note.start_ms
                  << " dur=" << note.duration_ms
                  << " end=" << note.start_ms + note.duration_ms << std::endl;
    }

    if (synthpp::WavRenderer::render_to_file(score, "output.wav")) {
        std::cout << "Rendering successful. Play output.wav" << std::endl;
    } else {
        std::cerr << "Rendering failed." << std::endl;
        return 1;
    }
    return 0;
}