//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include <vector>
#include <cstdint>

namespace synthpp {
    struct Note {
        double freqStart;   // 起始频率（Hz）
        double freqEnd;     // 结束频率（Hz），若没有滑音则等于 freqStart
        int startMs;        // 从乐曲开头计算的毫秒
        int durationMs;     // 持续毫秒

        Note(double startFreq, double endFreq, int start, int dur)
            : freqStart(startFreq), freqEnd(endFreq), startMs(start), durationMs(dur) {}
    };

    class Score {
        std::vector<Note> notes;
    public:
        void addNote(const Note& n) { notes.push_back(n); }
        const std::vector<Note>& getNotes() const { return notes; }
        // 可选：按 startMs 排序
        void sortByTime();
    };
} // namespace synthpp