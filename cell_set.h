// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CELL_SET_H
#define ASPARAGUS_CELL_SET_H

#include "common.h"

#include <cassert>

namespace asparagus {

class CellSet final {
    static constexpr int kMaxSize = 30 * 30;
public:
    using iterator = Cell*;

    CellSet() : size_(0) {}

    constexpr iterator begin() { return cells_; }
    constexpr iterator end() { return cells_ + size_; }

    constexpr void insert(Cell cell) {
        assert(size_ < kMaxSize);
        cells_[size_++] = cell;
    }

private:
    int size_;
    Cell cells_[kMaxSize];
};

}  // namespace asparagus

#endif  // ASPARAGUS_CELL_SET_H
