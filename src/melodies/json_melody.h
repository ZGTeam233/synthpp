//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#pragma once
#include "../core/melody.h"
#include <string>

namespace synthpp {
    class JsonMelody : public Melody {
    public:
        explicit JsonMelody(const std::string& filepath);

        void fill_score(Score& score) const override;

    private:
        std::string filepath_;
    };
} // synthpp
