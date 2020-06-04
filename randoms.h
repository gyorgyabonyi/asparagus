// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_RANDOMS_H
#define ASPARAGUS_RANDOMS_H

#include <cstdint>

namespace asparagus {

extern uint64_t kRandoms[1024][4];

void InitializeRandoms();

}  // namespace asparagus

#endif  // ASPARAGUS_RANDOMS_H
