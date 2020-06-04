// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_STATISTICS_H
#define ASPARAGUS_STATISTICS_H

#include <chrono>
#include <ostream>

namespace asparagus {

class Statistics {
public:
    Statistics();

    constexpr void AddNode() { node_count_ += 1; }
    constexpr void AddEval() { evaluate_count_ += 1; }
    constexpr void AddCutOff() { cutoff_count_ += 1; }
    constexpr void AddTerminal() { terminal_count_ += 1; }

    void Start();
    void Stop();
    void Print(std::ostream& out);

private:
    std::chrono::time_point<std::chrono::steady_clock> start_time_;
    double thinking_time_;
    int node_count_;
    int evaluate_count_;
    int cutoff_count_;
    int terminal_count_;
};

}  // namespace asparagus

#endif  // ASPARAGUS_STATISTICS_H
