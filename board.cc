// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "board.h"

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
    memset(stones_, 0, kStorageSize);
}

Board::Board(const Board& board)
    :   width_(board.width_),
        height_(board.height_),
        hash_(board.hash_) {
    memcpy(stones_, board.stones_, kStorageSize);
}

void Board::Initialize(int width, int height) {
    width_ = width;
    height_ = height;
    hash_ = 0;
    memset(stones_, kBoundary, kStorageSize);
    for (int y = 1; y <= height; y++) {
        memset(stones_ + MakeCell(1, y), kEmpty, width);
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
    for (Cell cell = 0; cell < kStorageSize; cell++) {
        if (flags[cell]) {
            cells->insert(cell);
        }
    }
}

void Board::GetPossibleMoves(int dist, CellSet* cells) const {
    uint8_t flags[kStorageSize];
    memset(flags, 0, sizeof(flags));
    for (int y = 1; y <= height_; y++) {
        for (int x = 1; x <= width_; x++) {
            if (IsStone(stones_[MakeCell(x, y)])) {
                for (int sy = std::max(1, y - dist); sy <= std::min(height_, y + dist); sy++) {
                    for (int sx = std::max(1, x - dist); sx <= std::min(width_, x + dist); sx++) {
                        flags[MakeCell(sx, sy)] = 1;
                    }
                }
            }
        }
    }
    for (Cell cell = 0; cell < kStorageSize; cell++) {
        if (flags[cell] && !stones_[cell]) {
            cells->insert(cell);
        }
    }
}

}  // namespace asparagus
