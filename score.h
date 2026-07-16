//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#ifndef SYNTHPP_SCORE_H
#define SYNTHPP_SCORE_H

#include <vector>
#include <cstdint>

namespace synthpp {
    struct Note {
        double frequency; // Hz
        int durationMs; // 毫秒

        Note(double freq, int dur) : frequency(freq), durationMs(dur) {}
    };

    class Score {
        std::vector<Note> notes;
        size_t currentIndex = 0;

    public:
        Score() = default;
        explicit Score(const std::vector<Note>& n) : notes(n) {}
        void addNote(const Note& note);
        bool hasMore() const;
        Note nextNote();
        void reset();
    };
} // synthpp

#endif // SYNTHPP_SCORE_H