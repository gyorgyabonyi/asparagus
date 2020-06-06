// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CACHE_H
#define ASPARAGUS_CACHE_H

#include "common.h"

#ifdef COLLECT_STATISTICS
#include <ostream>
#endif  // COLLECT_STATISTICS

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

        constexpr uint8_t type() const { return type_; }
        constexpr uint8_t depth() const { return depth_; }
        constexpr float value() const { return value_; }
        constexpr Cell best_move() const { return best_move_; }

        void Store(uint8_t type, uint8_t depth, float value, Cell best_move) {
            type_ = type & 0x3u;
            depth_ = depth;
            value_ = value;
            best_move_ = best_move;
        }

    private:
        uint64_t hash_;
        uint32_t type_       : 2;
        uint32_t depth_      : 8;
        uint32_t age_        : 10;
        uint32_t best_move_  : 10;
        float value_;

        DISALLOW_COPY_AND_ASSIGN(Entry);
    };

    Cache(uint64_t size);
    ~Cache();

    void Reset();
    void NewSearch();
    Entry* Find(uint64_t hash, bool* found);

    #ifdef COLLECT_STATISTICS
    void PrintStats(std::ostream& out);
    #endif  // COLLECT_STATISTICS

private:
    unsigned int ply_;
    uint64_t entry_num_;
    Entry* entries_;

    #ifdef COLLECT_STATISTICS
    uint64_t used_entries_;
    uint64_t lookup_count_;
    uint64_t collision_count_;
    uint64_t hit_count_;
    #endif  // COLLECT_STATISTICS

    DISALLOW_COPY_AND_ASSIGN(Cache);
};

}  // namespace asparagus

#endif  // ASPARAGUS_CACHE_H
