// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_ENGINE_H
#define ASPARAGUS_ENGINE_H

#include "cache.h"
#include "common.h"
#include "patterns.h"

#ifdef COLLECT_STATISTICS
#include <chrono>
#include <ostream>
#endif  // COLLECT_STATISTICS

namespace asparagus {

class Config;
class Board;

class Engine final {
public:
    explicit Engine(const Config &config);

    void Start();
    Cell GetBestMove(Board* board);
    #ifdef COLLECT_STATISTICS
    void PrintStats(std::ostream& out);
    #endif  // COLLECT_STATISTICS

private:
    struct Pattern {
        const char* pattern_;
        float value_;
    };

    static const Pattern kPatterns[];

    const Config &config_;
    Patterns patterns_;
    Cache cache_;
    #ifdef COLLECT_STATISTICS
    int node_count_;
    int eval_count_;
    int cutoff_count_;
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    double thinkig_time_;
    #endif  // COLLECT_STATISTICS

    float NegaMax(Board* node, int depth, float alpha, float beta, float color,
                  int distance, Cell* best_move);
    float Evaluate(const Board* board);

    DISALLOW_COPY_AND_ASSIGN(Engine);
};

}  // namespace asparagus

#endif  // ASPARAGUS_ENGINE_H
