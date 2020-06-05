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
}

void Cache::NewSearch() {
    ply_ += 1u;
}

Cache::Entry* Cache::Find(uint64_t hash, bool* found) {
    Entry* entry = entries_ + (hash >> 32u) % entry_num_;
    if (entry->hash_ == hash) {
        *found = true;
    } else {
        *found = false;
        entry->hash_ = hash;
        entry->depth_ = ply_;
    }
    return entry;
}

}  // namespace asparagus
