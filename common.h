// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_COMMON_H
#define ASPARAGUS_COMMON_H

#include <cstdint>

namespace asparagus {

#define COLLECT_STATISTICS  1
// #define USE_CACHE           1
// #define ITERATIVE_DEEPENING 1

#define DISALLOW_COPY_AND_ASSIGN(clazz) \
    clazz(const clazz&) = delete;       \
    clazz& operator =(const clazz&) = delete

using Stone = uint8_t;

constexpr uint8_t kEmpty     = 0x00u;
constexpr uint8_t kEngine    = 0x01u;
constexpr uint8_t kForbidden = 0x02u;
constexpr uint8_t kPlayer    = 0x03u;
constexpr uint8_t kBoundary  = 0x04u;

constexpr bool IsStone(Stone stone) { return stone & 1u; }

using Cell = uint16_t;

constexpr Cell MakeCell(unsigned int x, unsigned int y) {
    return (x & 0x1fu) | ((y & 0x1fu) << 5u);
}
constexpr unsigned int GetX(Cell cell) { return cell & 0x1fu; }
constexpr unsigned int GetY(Cell cell) { return cell >> 5u; }

}  // namespace asparagus

#endif  // ASPARAGUS_COMMON_H
