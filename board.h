// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_BOARD_H
#define ASPARAGUS_BOARD_H

#include "common.h"

namespace asparagus {

class CellSet;

class Board final {
public:
    static constexpr int kMinSize = 5;
    static constexpr int kMaxSize = 30;

    static constexpr int kStride = 32;
    static constexpr int kStorageSize = kStride * kStride;
    static constexpr int kUp = -kStride;
    static constexpr int kRight = 1;
    static constexpr int kDown = kStride;
    static constexpr int kLeft = -1;
    static constexpr int kUpRight = kUp + kRight;
    static constexpr int kDownRight = kDown + kRight;
    static constexpr int kDownLeft = kDown + kLeft;
    static constexpr int kUpLeft = kUp + kLeft;

    Board();

    constexpr int width() const { return width_; }
    constexpr int height() const { return height_; }
    constexpr bool empty() const { return !hash_; }
    constexpr Stone stone(Cell cell) const { return stones_[cell]; }
    constexpr const Stone* cell(Cell cell) const { return stones_ + cell; }

    bool IsInside(Cell cell) const;
    bool IsEmptyCell(Cell move) const;
    bool IsTerminalMove(Cell move, Stone stone, bool is_exact_five) const;

    void Initialize(int width, int height);
    void Set(Cell cell, Stone stone);
    void GetCellsToEvaluate(int dist, CellSet* cells) const;
    void GetPossibleMoves(int dist, CellSet* cells) const;

private:
    int width_;
    int height_;
    uint64_t hash_;
    Stone stones_[kStride * kStride];

    DISALLOW_COPY_AND_ASSIGN(Board);
};

}  // namespace asparagus

#endif  // ASPARAGUS_BOARD_H
