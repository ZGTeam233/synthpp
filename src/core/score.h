//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <vector>
#include <algorithm>
#include "note.h"

namespace synthpp {
    class Score {
    public:
        void add_note(const Note& note);
        void sort_by_time();
        const std::vector<Note>& notes() const;
        int64_t total_duration_ms() const;
        void clear();

    private:
        std::vector<Note> notes_;
    };
} // synthpp
