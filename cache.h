// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CACHE_H
#define ASPARAGUS_CACHE_H

#include "common.h"

namespace asparagus {

class Cache final {
public:
    class Entry {
        friend class Cache;
    public:
        static constexpr uint32_t kEmpty        = 0u;
        static constexpr uint32_t kLowerBound   = 1u;
        static constexpr uint32_t kUpperBound   = 2u;
        static constexpr uint32_t kExact        = 3u;

        Entry() = default;

        constexpr int type() const { return type_; }

    private:
        uint64_t hash_;
        uint32_t type_       : 2;
        uint32_t depth_      : 10;
        uint32_t best_move_  : 10;
        float value_;

        DISALLOW_COPY_AND_ASSIGN(Entry);
    };

    Cache(uint64_t size);
    ~Cache();

    void Reset();
    void NewSearch();
    Entry* Find(uint64_t hash, bool* found);

private:
    unsigned int ply_;
    uint64_t entry_num_;
    Entry* entries_;

    DISALLOW_COPY_AND_ASSIGN(Cache);
};

}  // namespace asparagus

#endif  // ASPARAGUS_CACHE_H
