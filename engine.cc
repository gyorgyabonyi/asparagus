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
    cache_.Reset();
}

Cell Engine::GetBestMove(Board* board) {
    cache_.NewSearch();
    Cell best_move = MakeCell(0, 0);
    if (board->empty()) {
        best_move = MakeCell(board->width() / 2, board->height() / 2);
    } else {
        for (int depth = 1; depth <= config_.max_depth(); depth++) {
            NegaMax(board, depth, -kInfinity, kInfinity, 1.0f, 2, &best_move);
        }
    }
    return best_move;
}

float Engine::NegaMax(Board* node, int depth, float alpha, float beta, float color, int distance,
                      Cell* best_move) {
    const float original_alpha = alpha;
    bool found;
    Cache::Entry* entry = cache_.Find(node->hash(), &found);
    if (found && entry->depth() >= depth) {
        uint8_t type = entry->type();
        if (type == Cache::Entry::kExact) {
            return entry->value();
        } else if (type == Cache::Entry::kLowerBound) {
            alpha = std::max(alpha, entry->value());
        } else if (type == Cache::Entry::kUpperBound) {
            beta = std::min(beta, entry->value());
        }
        if (alpha >= beta) {
            return entry->value();
        }
    }

    if (depth == 0) {
        return color * Evaluate(node);
    }

    CellSet moves;
    node->GetPossibleMoves(distance, &moves);
    // TODO(gyorgy): order moves.
    float best_value = -kInfinity;
    Cell local_best_move = MakeCell(0, 0);
    const Stone stone = color > 0.0f ? kEngine : kPlayer;
    for (auto move : moves) {
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
            break;
        }
    }

    uint8_t type;
    if (best_value <= original_alpha) {
        type = Cache::Entry::kUpperBound;
    } else if (best_value >= beta) {
        type = Cache::Entry::kLowerBound;
    } else {
        type = Cache::Entry::kExact;
    }
    entry->Store(type, depth, best_value, local_best_move);
    return best_value;
}

float Engine::Evaluate(const Board* board) {
    static int kStrides[] = { Board::kUpRight, Board::kRight, Board::kDownRight, Board::kDown };
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
