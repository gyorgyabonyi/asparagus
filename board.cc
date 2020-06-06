// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "board.h"

#include <algorithm>
#include <cstring>

#include "cell_set.h"
#include "randoms.h"

namespace asparagus {

static int CountSimilar(Stone stone, const Stone* cell, int stride) {
    int count = 0;
    while (*cell == stone) {
        cell += stride;
        count += 1;
    }
    return count;
}

Board::Board()
    :   width_(0),
        height_(0),
        hash_(0) {
    memset(stones_, 0, sizeof(stones_));
    memset(axes_, 0, sizeof(axes_));
}

void Board::Initialize(int width, int height) {
    // static const int kStrides[4] = { kDownLeft, kLeft, kUpLeft, kUp };
    static const int kStrides[8] = {
        kDownLeft, kLeft, kUpLeft, kUp, kUpRight, kRight, kDownRight, kDown,
    };

    width_ = width;
    height_ = height;
    hash_ = 0;

    last_ = kStride * (height + 2) + 1;

    memset(stones_, kBoundary, kStorageSize);
    memset(axes_, 0, sizeof(axes_));
    for (int y = 1; y <= height; y++) {
        memset(stones_ + MakeCell(1, y), kEmpty, width);
    }
    for (int y = 0; y < kStride; y++) {
        for (int x = 0; x < kStride; x++) {
            const Cell base = MakeCell(x, y);
            if (!IsInside(base)) {
                continue;
            }
            for (int stride = 0; stride < 8; stride++) {
                Cell value = base + kStrides[stride];
                const Stone* board = cell(base);
                for (int i = 0; *board == kEmpty && i < kMaxAxisLength; i++) {
                    axes_[base][stride][i] = value;
                    value += kStrides[stride];
                }
            }
        }
    }
}

bool Board::IsInside(Cell cell) const {
    const unsigned int x = GetX(cell);
    const unsigned int y = GetY(cell);
    return x > 0 && x <= width_ && y > 0 && y < height_;
}

bool Board::IsEmptyCell(Cell move) const {
    return IsInside(move) && !stones_[move];
}

bool Board::IsTerminalMove(Cell move, Stone stone, bool is_exact_five) const {
    static int kStrides[4][2] = {
        { kUp, kDown },
        { kUpRight, kDownLeft },
        { kRight, kLeft },
        { kDownRight, kUpLeft }
    };
    const Stone* origin = stones_ + move;
    for (auto & directions : kStrides) {
        const auto count = CountSimilar(stone, origin + directions[0], directions[0]) +
                           CountSimilar(stone, origin + directions[1], directions[1]);
        if ((is_exact_five && count == 4) || (!is_exact_five && count >= 4)) {
            return true;
        }
    }
    return false;
}

void Board::Set(Cell cell, Stone stone) {
    hash_ ^= kRandoms[cell][stones_[cell]];
    hash_ ^= kRandoms[cell][stone];
    stones_[cell] = stone;
}

void Board::GetCellsToEvaluate(int dist, CellSet* cells) const {
    static const int kStrides[4] = { kDownLeft, kLeft, kUpLeft, kUp };
    uint8_t flags[kStorageSize];
    memset(flags, 0, sizeof(flags));

    /*
    for (Cell cell = kStride + 1; cell < last_; cell++) {
        if (IsStone(stones_[cell])) {
            for (int direction = 0; direction < 4; direction++) {
                for (int i = 0; i < dist; i++) {
                    flags[axes_[cell][direction][i]] = 1;
                }
            }
        }
    }
    */

    for (int y = 1; y <= height_; y++) {
        for (int x = 1; x <= width_; x++) {
            const Cell base = MakeCell(x, y);
            if (IsStone(stones_[base])) {
                for (auto stride : kStrides) {
                    Cell cell = base;
                    for (int i = 0; i < dist && stones_[cell] != kBoundary; i++, cell += stride) {
                        flags[cell] = 1u;
                    }
                }
            }
        }
    }

    for (Cell cell = kStride + 1; cell < last_; cell++) {
        if (flags[cell] && stones_[cell] != kBoundary) {
            cells->insert(cell);
        }
    }
}

void Board::GetPossibleMoves(int dist, CellSet* cells) const {
    uint8_t flags[kStorageSize];
    memset(flags, 0, sizeof(flags));

    for (Cell cell = kStride + 1; cell < last_; cell++) {
        if (IsStone(stones_[cell])) {
            for (int direction = 0; direction < 8; direction++) {
                for (int i = 0; i < dist; i++) {
                    flags[axes_[cell][direction][i]] = 1;
                }
            }
        }
    }

    for (Cell cell = kStride + 1; cell < last_; cell++) {
        if (flags[cell] && !stones_[cell]) {
            cells->insert(cell);
        }
    }
}

}  // namespace asparagus
