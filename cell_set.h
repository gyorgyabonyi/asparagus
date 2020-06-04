// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CELL_SET_H
#define ASPARAGUS_CELL_SET_H

#include "common.h"

#include <vector>

namespace asparagus {

class CellSet final {
public:
    using iterator = std::vector<Cell>::iterator;

    iterator begin() { return cells_.begin(); }
    iterator end() { return cells_.end(); }

    constexpr void insert(Cell cell) {
        cells_.push_back(cell);
    }

private:
    std::vector<Cell> cells_;
};

}  // namespace asparagus

#endif  // ASPARAGUS_CELL_SET_H
