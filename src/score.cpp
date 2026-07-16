//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "../score.h"
#include <vector>

namespace synthpp {
    void Score::addNote(const Note& note) {
        notes.push_back(note);
    }

    bool Score::hasMore() const {
        return currentIndex < notes.size();
    }

    Note Score::nextNote() {
        if (!hasMore()) return Note(0, 0);
            return notes[currentIndex++];
    }

    void Score::reset() {
        currentIndex = 0;
    }
} // synthpp