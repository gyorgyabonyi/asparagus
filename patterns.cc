// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "patterns.h"

#include <cassert>

namespace asparagus {

Patterns::Patterns() {}

void Patterns::AddPattern(const char* pattern, float value) {
    Node* node = &root_;
    while (const char ch = *pattern++) {
        unsigned int index;
        if (ch == '+') {
            index = kEmpty;
        } else if (ch == 'O') {
            index = kEngine;
        } else if (ch == 'X') {
            index = kPlayer;
        } else {
            index = kForbidden;
        }
        if (!node->children_[index]) {
            node->children_[index] = new Node();
        }
        node = node->children_[index];
    }
    node->value_ = value;
}

float Patterns::GetValue(const Stone* cell, int stride) const {
    const Node* node = &root_;
    float value = node->value_;
    while (node && (*cell != kBoundary)) {
        value = node->value_;
        const unsigned int index = *cell & 3u;
        node = node->children_[index];
        cell += stride;
    }
    return value;
}

Patterns::Node::Node()
    :   children_{nullptr, nullptr, nullptr, nullptr},
        value_(kNeutralValue) {}

Patterns::Node::~Node() {
    for (auto child : children_) {
        delete child;
    }
}

}  // namespace asparagus
