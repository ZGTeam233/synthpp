//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#include "melody.h"

namespace synthpp {
// 可选：返回一个完整 Score 的便利方法
    Score Melody::generateScore() const {
        Score s;
        fillScore(s);
        return s;
    }
} // namespace synthpp
