// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "config.h"

namespace asparagus {

Config::Config()
    :   use_gomocup_protocol_(false),
        cache_size_(1ull * 1024ull * 1024ull * 1024ull),
        is_exact_five_(false),
        max_depth_(5) {}

void Config::Load(int argc, char **argv) {
    // TODO(gyorgy): Implement it.
}

int Config::Get(const std::string& key) const {
    if (key == "is_exact_five") {
        return is_exact_five_ ? 1 : 0;
    } else if (key == "max_depth") {
        return max_depth_;
    }
    return 0;
}

void Config::Set(const std::string& key, int value) {
    if (key == "is_exact_five") {
        is_exact_five_ = value;
    } else if (key == "max_depth") {
        max_depth_ = value;
    }
}

}  // namespace asparagus
