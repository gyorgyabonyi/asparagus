// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "controller.h"

#include "config.h"
#include "engine.h"

namespace asparagus {

Controller::Controller(const Config& config, Engine* engine)
    :   config_(config),
        engine_(engine),
        state_(kUnknown) {}

void Controller::Start(int width, int height) {
    board_.Initialize(width, height);
    engine_->Start();
}

void Controller::SetCell(Cell cell, Stone stone) {
    board_.Set(cell, stone);
}

void Controller::PlayerMove(Cell move) {
    board_.Set(move, kPlayer);
    if (board_.IsTerminalMove(move, kPlayer, config_.is_exact_five())) {
        state_ = kWon;
    }
}

Cell Controller::GetEngineMove() {
    Cell move = engine_->GetBestMove(&board_);
    if (!GetX(move)) {
        state_ = kDraw;
    } else {
        board_.Set(move, kEngine);
        if (board_.IsTerminalMove(move, kEngine, config_.is_exact_five())) {
            state_ = kWon;
        }
    }
    return move;
}

}  // namespace asparagus
