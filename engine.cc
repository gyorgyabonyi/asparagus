// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "engine.h"

#include "board.h"
#include "cell_set.h"
#include "config.h"


namespace asparagus {

constexpr float kInfinity = std::numeric_limits<float>::infinity();
constexpr float kWinValue = 1e20f;

Engine::Engine(const Config &config)
    :   config_(config),
        cache_(config.cache_size()) {
    for (const Pattern* pattern = kPatterns; pattern->pattern_; pattern++) {
        patterns_.AddPattern(pattern->pattern_, pattern->value_);
    }
}

void Engine::Start() {
    #ifdef USE_CACHE
    cache_.Reset();
    #endif  // USE_CACHE
    #ifdef AGGREGATED_STATISTICS
    aggregated_node_count_ = 0;
    aggregated_eval_count_ = 0;
    aggregated_cutoff_count_ = 0;
    #endif  // AGGREGATED_STATISTICS
}

Cell Engine::GetBestMove(Board* board) {
    #ifdef COLLECT_STATISTICS
    start_time_ = std::chrono::steady_clock::now();
    node_count_ = 0;
    eval_count_ = 0;
    cutoff_count_ = 0;
    #endif  // COLLECT_STATISTICS
    cache_.NewSearch();
    Cell best_move = MakeCell(0, 0);
    if (board->empty()) {
        best_move = MakeCell(board->width() / 2, board->height() / 2);
    } else {
        #ifdef ITERATIVE_DEEPENING
        for (int depth = 1; depth <= config_.max_depth(); depth++) {
            NegaMax(board, depth, -kInfinity, kInfinity, 1.0f, 2, &best_move);
        }
        #else  // ITERATIVE_DEEPENING
        NegaMax(board, config_.max_depth(), -kInfinity, kInfinity, 1.0f, 2, &best_move);
        #endif  // ITERATIVE_DEEPENING
    }
    #ifdef COLLECT_STATISTICS
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time_;
    thinkig_time_ = duration.count();
    #ifdef AGGREGATED_STATISTICS
    aggregated_node_count_ += node_count_;
    aggregated_eval_count_ += eval_count_;
    aggregated_cutoff_count_ += cutoff_count_;
    #endif  // AGGREGATED_STATISTICS
    #endif  // COLLECT_STATISTICS

    return best_move;
}

#ifdef COLLECT_STATISTICS
void Engine::PrintStats(std::ostream& out) {
    out << "engine stats:" << std::endl;
    out << " thinking time: " << thinkig_time_ << std::endl;
    out << " nodes        : " << node_count_ << std::endl;
    out << " evals        : " << eval_count_ << std::endl;
    out << " cutoff rate  : " << 100.0 * double(cutoff_count_) / double(node_count_) << std::endl;
    out << " nodes/sec    : " << double(node_count_) / thinkig_time_ << std::endl;
    out << " evals/sec    : " << double(eval_count_) / thinkig_time_ << std::endl;
    out << std::endl;
    cache_.PrintStats(out);

    #ifdef AGGREGATED_STATISTICS
    out << "aggregated stats:" << std::endl;
    out << " nodes:        " << aggregated_node_count_ << std::endl;
    out << " eval rate   : " << 100.0 * double(aggregated_eval_count_) / double(aggregated_node_count_) << std::endl;
    out << " cutoff rate : " << 100.0 * double(aggregated_cutoff_count_) / double(aggregated_node_count_) << std::endl;
    #endif  // AGGREGATED_STATISCTICS
}
#endif  // COLLECT_STATISTICS

float Engine::NegaMax(Board* node, int depth, float alpha, float beta, float color, int distance,
                      Cell* best_move) {
    #ifdef COLLECT_STATISTICS
    node_count_ += 1ull;
    #endif  // COLLECT_STATISTICS

    #ifdef USE_CACHE
    const float original_alpha = alpha;
    bool found;
    Cache::Entry* entry = cache_.Find(node->hash(), &found);
    if (found && entry->depth() >= depth) {
        uint8_t type = entry->type();
        if (type == Cache::Entry::kExact) {
            *best_move = entry->best_move();
            return entry->value();
        } else if (type == Cache::Entry::kLowerBound) {
            alpha = std::max(alpha, entry->value());
        } else if (type == Cache::Entry::kUpperBound) {
            beta = std::min(beta, entry->value());
        }
        if (alpha >= beta) {
            *best_move = entry->best_move();
            return entry->value();
        }
    }
    #endif  // USE_CACHE

    if (depth == 0) {
        return color * Evaluate(node);
    }

    CellSet moves;
    Cell cached_best_move = entry->best_move();
    if (found & node->IsEmptyCell(cached_best_move)) {
        moves.insert(cached_best_move);
    }
    node->GetPossibleMoves(distance, &moves);
    // TODO(gyorgy): order moves.
    float best_value = -kInfinity;
    Cell local_best_move = MakeCell(0, 0);
    const Stone stone = color > 0.0f ? kEngine : kPlayer;
    for (auto move : moves) {
        // TODO(gyorgy): ignore the cached best move at second time.
        float value;
        if (node->IsTerminalMove(move, stone, config_.is_exact_five())) {
            value = -color * kWinValue;
        } else {
            node->Set(move, stone);
            value = -NegaMax(node, depth - 1, -beta, -alpha, -color, 1, &local_best_move);
            node->Set(move, kEmpty);
        }

        if (value > best_value) {
            best_value = value;
            *best_move = move;
        }
        if (best_value > alpha) {
            alpha = best_value;
        }
        if (alpha >= beta) {
            #ifdef COLLECT_STATISTICS
            cutoff_count_ += 1ull;
            #endif  // COLLECT_STATISTICS
            break;
        }
    }

    #ifdef USE_CACHE
    uint8_t type;
    if (best_value <= original_alpha) {
        type = Cache::Entry::kUpperBound;
    } else if (best_value >= beta) {
        type = Cache::Entry::kLowerBound;
    } else {
        type = Cache::Entry::kExact;
    }
    entry->Store(type, depth, best_value, *best_move);
    #endif  // USE_CACHE

    return best_value;
}

float Engine::Evaluate(const Board* board) {
    static int kStrides[] = { Board::kUpRight, Board::kRight, Board::kDownRight, Board::kDown };
    #ifdef COLLECT_STATISTICS
    eval_count_ += 1ull;
    #endif  // COLLECT_STATISTICS
    CellSet cells;
    board->GetCellsToEvaluate(3, &cells);
    float value = 0.0f;
    for (auto cell : cells) {
        for (auto stride : kStrides) {
            value += patterns_.GetValue(board->cell(cell), stride);
        }
    }
    return value;
}

constexpr float kValue0     = 0.0f;
constexpr float kValue2     = 1e1f;
constexpr float kValue2Open = 2e1f;
constexpr float kValue3     = 1e2f;
constexpr float kValue3Open = 2e2f;
constexpr float kValue4     = 2e2f;
constexpr float kValue4Open = 1e8f;
constexpr float kValue5     = 1e10f;

const Engine::Pattern Engine::kPatterns[] = {
    { "OO+++", kValue2 }, { "XX+++", -kValue2 },
    { "O+O++", kValue2 }, { "X+X++", -kValue2 },
    { "O++O+", kValue2 }, { "X++X+", -kValue2 },
    { "O+++O", kValue2 }, { "X+++X", -kValue2 },
    { "++O+O", kValue2 }, { "++X+X", -kValue2 },
    { "+++OO", kValue2 }, { "+++XX", -kValue2 },

    { "+OO++", kValue2Open }, { "+XX++", -kValue2Open },
    { "+O+O+", kValue2Open }, { "+X+X+", -kValue2Open },
    { "+O++O+", kValue2Open }, { "+X++X+", -kValue2Open },
    { "++OO+", kValue2Open }, { "++XX+", -kValue2Open },

    { "OOO++", kValue3 }, { "XXX++", -kValue3 },
    { "OO+O+", kValue3 }, { "XX+X+", -kValue3 },
    { "OO++O", kValue3 }, { "XX++X", -kValue3 },
    { "O+OO+", kValue3 }, { "X+XX+", -kValue3 },
    { "O+O+O", kValue3 }, { "X+X+X", -kValue3 },
    { "O++OO", kValue3 }, { "X++XX", -kValue3 },
    { "+OO+O", kValue3 }, { "+XX+X", -kValue3 },
    { "+O+OO", kValue3 }, { "+X+XX", -kValue3 },
    { "++OOO", kValue3 }, { "++XXX", -kValue3 },

    { "+OOO+", kValue3Open }, { "+XXX+", -kValue3Open },

    { "OOOO+", kValue4 }, { "XXXX+", -kValue4 },
    { "OOO+O", kValue4 }, { "XXX+X", -kValue4 },
    { "OO+OO", kValue4 }, { "XX+XX", -kValue4 },
    { "O+OOO", kValue4 }, { "X+XXX", -kValue4 },
    { "+OOOO", kValue4 }, { "+XXXX", -kValue4 },

    { "+OOOO+", kValue4Open }, { "+XXXX+", -kValue4Open },

    { nullptr, kValue0 }
};

}  // namespace asparagus
