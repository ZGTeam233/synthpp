//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include "../core/melody.h"
#include "../core/note.h"

namespace synthpp {

    class TwinkleStar : public Melody {
    public:
        void fill_score(Score& score) const override {
            const int dur = 400;
            // 使用频率常量（在 melody.h 基类中已有，但这里为了方便直接写数字）
            // 也可以用 midi_to_frequency，但 TwinkleStar 用于测试，直接用常数
            double C4 = 261.63;
            double D4 = 293.66;
            double E4 = 329.63;
            double F4 = 349.23;
            double G4 = 392.00;
            double A4 = 440.00;

            int64_t start = 0;
            auto add = [&](double freq, int dur_ms) {
                score.add_note(Note(freq, start, dur_ms));
                start += dur_ms;
            };

            // 小星星旋律（C C G G A A G, F F E E D D C）
            add(C4, dur);
            add(C4, dur);
            add(G4, dur);
            add(G4, dur);
            add(A4, dur);
            add(A4, dur);
            add(G4, dur * 2);

            add(F4, dur);
            add(F4, dur);
            add(E4, dur);
            add(E4, dur);
            add(D4, dur);
            add(D4, dur);
            add(C4, dur * 2);
        }
    };

} // namespace synthpp