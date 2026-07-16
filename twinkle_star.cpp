//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "twinkle_star.h"

namespace synthpp {
    void TwinkleStar::fillScore(Score& score) const override {
        // 小星星前两句（C C G G A A G, F F E E D D C）
        // 每个音符持续 400ms
        const int dur = 400;
        score.addNote(Note(C4, dur));
        score.addNote(Note(C4, dur));
        score.addNote(Note(G4, dur));
        score.addNote(Note(G4, dur));
        score.addNote(Note(A4, dur));
        score.addNote(Note(A4, dur));
        score.addNote(Note(G4, dur * 2)); // 延长

        score.addNote(Note(F4, dur));
        score.addNote(Note(F4, dur));
        score.addNote(Note(E4, dur));
        score.addNote(Note(E4, dur));
        score.addNote(Note(D4, dur));
        score.addNote(Note(D4, dur));
        score.addNote(Note(C4, dur * 2));
    }
} // namespace synthpp