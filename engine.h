// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_ENGINE_H
#define ASPARAGUS_ENGINE_H

#include "cache.h"
#include "common.h"
#include "patterns.h"

namespace asparagus {

class Config;
class Board;

class Engine final {
public:
    explicit Engine(const Config &config);

    void Start();
    Cell GetBestMove(Board* board);

private:
    struct Pattern {
        const char* pattern_;
        float value_;
    };

    static const Pattern kPatterns[];

    const Config &config_;
    Patterns patterns_;
    Cache cache_;

    float NegaMax(Board* node, int depth, int max_depth, float alpha, float beta, float color,
                  int distance, Cell* best_move);
    float Evaluate(const Board* board);

    DISALLOW_COPY_AND_ASSIGN(Engine);
};

}  // namespace asparagus

#endif  // ASPARAGUS_ENGINE_H
