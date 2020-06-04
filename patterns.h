// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_PATTERNS_H
#define ASPARAGUS_PATTERNS_H

#include "common.h"

namespace asparagus {

class Patterns final {
public:
    static constexpr float kNeutralValue = 0.0f;

    Patterns();

    void AddPattern(const char* pattern, float value);
    float GetValue(const Stone* cell, int stride) const;

private:
    struct Node {
        Node();
        ~Node();

        Node* children_[4];
        float value_;
    };

    Node root_;
};

}  // namespace asparagus

#endif  // ASPARAGUS_PATTERNS_H
