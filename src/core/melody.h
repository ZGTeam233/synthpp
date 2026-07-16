//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include "score.h"

namespace synthpp {

    // 抽象基类：旋律生成器
    class Melody {
    public:
        virtual void fill_score(Score& score) const = 0;
        virtual ~Melody() = default;
    };

} // namespace synthpp