// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "statistics.h"

namespace asparagus {

Statistics::Statistics()
    :   thinking_time_(0.0001),
        node_count_(0),
        evaluate_count_(0),
        cutoff_count_(0),
        terminal_count_(0) {}

void Statistics::Start() {
    start_time_ = std::chrono::steady_clock::now();
    node_count_ = 0;
    evaluate_count_ = 0;
    cutoff_count_ = 0;
    terminal_count_ = 0;
}

void Statistics::Stop() {
    auto stop_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration =  stop_time - start_time_;
    thinking_time_ = duration.count();
    if (thinking_time_ < 0.0001) {
        thinking_time_ = 0.0001;
    }
}

void Statistics::Print(std::ostream& out) {
    out << "thinking time: " << thinking_time_ << std::endl;
    out << std::endl;
    out << "nodes    : " << node_count_ << std::endl;
    out << "evals    : " << evaluate_count_ << std::endl;
    out << "cuttoffs : " << cutoff_count_ << std::endl;
    out << "terminals: " << terminal_count_ << std::endl;
    const double nodes = node_count_;
    const double evals = evaluate_count_;
    const double cutoffs = cutoff_count_;
    out << std::endl;
    out << "node/sec: " << nodes / thinking_time_ << std::endl;
    out << "eval/sec: " << evals / thinking_time_ << std::endl;
    out << std::endl;
    out << "eval rate   : " << evals / nodes << std::endl;
    out << "cutoff rate : " << cutoffs / nodes << std::endl;
}

}  // namespace asparagus
