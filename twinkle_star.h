//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#ifndef SYNTHPP_TWINKLE_STAR_H
#define SYNTHPP_TWINKLE_STAR_H

#include "melody.h"

namespace synthpp {

    class TwinkleStar : public Melody {
    public:
        void fillScore(Score& score) const override;
    };

} // namespace synthpp

#endif // SYNTHPP_TWINKLE_STAR_H