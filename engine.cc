// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "engine.h"

#include <iostream>

#include "board.h"
#include "cell_set.h"
#include "config.h"
#include "statistics.h"

namespace asparagus {

constexpr float kInfinity = std::numeric_limits<float>::infinity();
constexpr float kWinValue = 1e20f;

Engine::Engine(const Config &config, Statistics* statistics)
    :   config_(config),
        statistics_(statistics) {
    for (const Pattern* pattern = kPatterns; pattern->pattern_; pattern++) {
        patterns_.AddPattern(pattern->pattern_, pattern->value_);
    }
}

void Engine::Start(int width, int height) {
    cache_.Reset();
}

Cell Engine::GetBestMove(const Board& board) {
    statistics_->Start();
    cache_.StartGeneration();
    Cell best_move = MakeCell(0, 0);
    if (board.empty()) {
        best_move = MakeCell(board.width() / 2, board.height() / 2);
    } else {
        Board node(board);
        for (int max_depth = 1; max_depth < config_.max_depth(); max_depth++) {
            std::cout << "depth: " << max_depth;
            NegaMax(&node, 0, max_depth, -kInfinity, kInfinity, 1.0f, 2, &best_move);
            std::cout << " " << GetX(best_move) << " " << GetY(best_move) << std::endl;
        }
    }
    statistics_->Stop();
    return best_move;
}

float Engine::NegaMax(Board* node, int depth, int max_depth, float alpha, float beta, float color,
                      int distance, Cell* best_move) {
    if (depth == max_depth) {
        return color * Evaluate(node);
    }
    CellSet moves;
    node->GetPossibleMoves(distance, &moves);
    // TODO(gyorgy): order moves.
    float best_value = -kInfinity;
    Cell local_best;
    const Stone stone = color > 0.0f ? kEngine : kPlayer;
    for (auto move : moves) {
        statistics_->AddNode();
        float value;
        if (node->IsTerminalMove(move, stone, config_.is_exact_five())) {
            statistics_->AddTerminal();
            value = -color * kWinValue;
        } else {
            node->Set(move, stone);
            value = -NegaMax(node, depth + 1, max_depth, -beta, -alpha, -color, 1, &local_best);
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
            statistics_->AddCutOff();
            break;
        }
    }
    return best_value;
}

float Engine::Evaluate(const Board* board) {
    static int kStrides[] = { Board::kUpRight, Board::kRight, Board::kDownRight, Board::kDown };
    statistics_->AddEval();
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