// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include <iostream>

#include "randoms.h"
#include "config.h"
#include "engine.h"
#include "controller.h"

using namespace asparagus;

constexpr int size = 19;
constexpr int rounds = 5;

int main(int argc, char** argv) {
    InitializeRandoms();
    Config config;
    config.Load(argc, argv);
    Engine engine_1(config);
    Engine engine_2(config);
    Controller controller_1(config, &engine_1);
    Controller controller_2(config, &engine_2);
    controller_1.Start(size, size);
    controller_2.Start(size, size);
    Cell move = MakeCell(0, 0);
    auto start_time_ = std::chrono::steady_clock::now();
    for (int i = 0; i < rounds; i++) {
        if (move) {
            controller_1.PlayerMove(move);
        }
        move = controller_1.GetEngineMove();
        std::cout << "player_1: " << GetX(move) << " " << GetY(move) << std::endl;
        if (controller_1.state() != Controller::kPlaying) {
            break;
        }
        controller_2.PlayerMove(move);
        move = controller_2.GetEngineMove();
        std::cout << "player_2: " << GetX(move) << " " << GetY(move) << std::endl;
        if (controller_1.state() != Controller::kPlaying) {
            break;
        }
    }
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time_;
    std::cout << "total time: " << duration.count() << std::endl;
    std::cout << "player_1:" << std::endl;
    controller_1.PrintStats(std::cout);
    std::cout << "player_2:" << std::endl;
    controller_2.PrintStats(std::cout);
    return 0;
}