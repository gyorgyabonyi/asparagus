// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_ENGINE_H
#define ASPARAGUS_ENGINE_H

#include "cache.h"
#include "common.h"
#include "patterns.h"

namespace asparagus {

class Config;
class Board;
class Statistics;

class Engine final {
public:
    explicit Engine(const Config &config, Statistics* statistics);

    void Start(int width, int height);
    Cell GetBestMove(const Board &board);

private:
    struct Pattern {
        const char* pattern_;
        float value_;
    };

    static const Pattern kPatterns[];

    const Config &config_;
    Statistics* statistics_;
    Patterns patterns_;
    Cache cache_;

    float NegaMax(Board* node, int depth, int max_depth, float alpha, float beta, float color,
                  int distance, Cell* best_move);
    float Evaluate(const Board* board);
};

}  // namespace asparagus

#endif  // ASPARAGUS_ENGINE_H
