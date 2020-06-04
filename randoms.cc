// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include <random>
#include "randoms.h"

namespace asparagus {

static std::random_device device;
static std::mt19937_64 generator(device());
static std::uniform_int_distribution<uint64_t> distribution;

uint64_t kRandoms[1024][4];

void InitializeRandoms() {
    for (auto & randoms : kRandoms) {
        randoms[0] = 0;
        randoms[1] = distribution(generator);
        randoms[2] = distribution(generator);
        randoms[3] = distribution(generator);
    }
}

}  // namespace asparagus
