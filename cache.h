// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CACHE_H
#define ASPARAGUS_CACHE_H

#include "common.h"

namespace asparagus {

class Cache final {
public:
    Cache();
    ~Cache();

    void Reset();
    void StartGeneration();

private:
    class Entry {
    public:
    private:
        uint64_t hash_;
        uint32_t type_       : 2;
        uint32_t depth_      : 10;
        uint32_t best_move_  : 10;
        uint32_t age_        : 10;
        float value_;
    };
};

}  // namespace asparagus

#endif  // ASPARAGUS_CACHE_H
