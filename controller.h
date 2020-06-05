// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_CONTROLLER_H
#define ASPARAGUS_CONTROLLER_H

#include "board.h"
#include "common.h"

namespace asparagus {

class Config;
class Engine;

class Controller {
public:
    enum State { kUnknown, kPlaying, kWon, kDraw };

    Controller(const Config& config, Engine* engine);

    constexpr State state() const { return state_; }
    constexpr const Board& board() const { return board_; }

    void Start(int width, int height);
    void SetCell(Cell cell, Stone stone);
    void PlayerMove(Cell move);
    Cell GetEngineMove();

private:
    const Config& config_;
    Board board_;
    Engine* engine_;
    State state_;

    DISALLOW_COPY_AND_ASSIGN(Controller);
};

}  // namespace asparagus

#endif  // ASPARAGUS_CONTROLLER_H
