//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include "../core/melody.h"

namespace synthpp {

    class TwinkleStar : public Melody {
    public:
        void fillScore(Score& score) const override;
    };

} // namespace synthpp