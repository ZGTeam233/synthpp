//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "src/melodies/twinkle_star.h"
#include "src/renderer/wav_renderer.h"
#include <iostream>

#include "src/melodies/json_melody.h"
#include "src/renderer/wav_renderer.h"
#include <iostream>

int main() {
    try {
        synthpp::JsonMelody melody("out.json");  // 你的 JSON 文件
        synthpp::Score score;
        melody.fill_score(score);

        std::cout << "Total notes: " << score.notes().size() << std::endl;
        std::cout << "Total duration: " << score.total_duration_ms() << " ms" << std::endl;

        if (synthpp::WavRenderer::render_to_file(score, "output.wav")) {
            std::cout << "Rendering successful. Play output.wav" << std::endl;
        } else {
            std::cerr << "Rendering failed." << std::endl;
            return 1;
        }

        std::cout << "Last note end: "
                  << score.notes().back().start_ms + score.notes().back().duration_ms << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}