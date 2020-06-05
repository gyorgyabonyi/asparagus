// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include <cstring>

#include "cache.h"

namespace asparagus {

Cache::Cache(uint64_t size)
    :   ply_(0) {
    entry_num_ = size / sizeof(Entry);
    entries_ = new Entry[entry_num_];
}

Cache::~Cache() {
    delete [] entries_;
}

void Cache::Reset() {
    ply_ = 0;
    memset(entries_, 0, entry_num_ * sizeof(Entry));

    #ifdef COLLECT_STATISTICS
    used_entries_ = 0;
    lookup_count_ = 0;
    collision_count_ = 0;
    hit_count_ = 0;
    #endif  // COLLECT_STATISTICS
}

void Cache::NewSearch() {
    ply_ += 1u;
}

Cache::Entry* Cache::Find(uint64_t hash, bool* found) {
    #ifdef COLLECT_STATISTICS
    lookup_count_ += 1ull;
    #endif  // COLLECT_STATISTICS
    Entry* entry = entries_ + (hash >> 32u) % entry_num_;
    if (entry->hash_ == hash) {
        #ifdef COLLECT_STATISTICS
        lookup_count_ += 1ull;
        hit_count_ += 1ull;
        #endif  // COLLECT_STATISTICS
        *found = true;
    } else {
        #ifdef COLLECT_STATISTICS
        if (entry->hash_) {
            if (entry->age_ > (ply_ - 1)) {
                collision_count_ += 1ull;
            }
        } else {
            used_entries_ += 1ull;
        }
        #endif  // COLLECT_STATISTICS
        *found = false;
        entry->hash_ = hash;
    }
    entry->age_ = ply_;
    return entry;
}

#ifdef COLLECT_STATISTICS
void Cache::PrintStats(std::ostream& out) {
    out << "cache stats:" << std::endl;
    out << "  entries       : " << used_entries_ << std::endl;
    out << "  usage         : " << 100.0 * double(used_entries_) / double(entry_num_) << " %" << std::endl;
    out << "  hit rate      : " << 100.0 * double(hit_count_) / double(lookup_count_) << " %" << std::endl;
    out << "  collision rate: " << 100.0 * double(collision_count_) / double(lookup_count_) << " %" << std::endl;
}
#endif  // COLLECT_STATISTICS

}  // namespace asparagus
