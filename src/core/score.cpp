//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "score.h"

namespace synthpp {
    void Score::add_note(const Note& note) {
        notes_.push_back(note);
    }

    void Score::sort_by_time() {
        std::sort(notes_.begin(), notes_.end(),
            [](const Note& a, const Note& b) {
                return a.start_ms < b.start_ms;
            });
    }

    const std::vector<Note>& Score::notes() const {
        return notes_;
    }

    int64_t Score::total_duration_ms() const {
        if (notes_.empty()) return 0;
        int64_t max_end = 0;
        for (const auto& n : notes_) {
            int64_t end = n.start_ms + n.duration_ms;
            if (end > max_end) max_end = end;
        }
        return max_end;
    }

    void Score::clear() {
        notes_.clear();
    }
} // synthpp