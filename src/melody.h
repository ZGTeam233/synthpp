//
// Created by wsnzg6 on 2026/7/16.
// Copyright(c) 2026 ZGTeam233.
//

#ifndef SYNTHPP_MELODY_H
#define SYNTHPP_MELODY_H

#include <vector>
#include "../score.h"

namespace synthpp {

    // 抽象基类：表示一首旋律/乐谱
    struct Melody {
        // 提供所有音符频率常量（静态，可在子类中直接使用）
        // 第 3 八度
        static constexpr double C3  = 130.81;
        static constexpr double Cs3 = 138.59;
        static constexpr double D3  = 146.83;
        static constexpr double Ds3 = 155.56;
        static constexpr double E3  = 164.81;
        static constexpr double F3  = 174.61;
        static constexpr double Fs3 = 185.00;
        static constexpr double G3  = 196.00;
        static constexpr double Gs3 = 207.65;
        static constexpr double A3  = 220.00;
        static constexpr double As3 = 233.08;
        static constexpr double B3  = 246.94;

        // 第 4 八度（中央C）
        static constexpr double C4  = 261.63;
        static constexpr double Cs4 = 277.18;
        static constexpr double D4  = 293.66;
        static constexpr double Ds4 = 311.13;
        static constexpr double E4  = 329.63;
        static constexpr double F4  = 349.23;
        static constexpr double Fs4 = 369.99;
        static constexpr double G4  = 392.00;
        static constexpr double Gs4 = 415.30;
        static constexpr double A4  = 440.00;
        static constexpr double As4 = 466.16;
        static constexpr double B4  = 493.88;

        // 第 5 八度
        static constexpr double C5  = 523.25;
        static constexpr double Cs5 = 554.37;
        static constexpr double D5  = 587.33;
        static constexpr double Ds5 = 622.25;
        static constexpr double E5  = 659.25;
        static constexpr double F5  = 698.46;
        static constexpr double Fs5 = 739.99;
        static constexpr double G5  = 783.99;
        static constexpr double Gs5 = 830.61;
        static constexpr double A5  = 880.00;
        static constexpr double As5 = 932.33;
        static constexpr double B5  = 987.77;

        // 第 6 八度
        static constexpr double C6  = 1046.50;

        // 纯虚函数：子类必须实现，填充传入的 Score 对象
        virtual void fillScore(Score& score) const = 0;

        // 可选：返回一个完整 Score 的便利方法
        Score generateScore() const;

        virtual ~Melody() = default;
    };

} // namespace synthpp

#endif // SYNTHPP_MELODY_H