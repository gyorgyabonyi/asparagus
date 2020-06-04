// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "simple_protocol.h"

#include <iomanip>

#include "config.h"

namespace asparagus {

static int ReadTokens(std::istream& in, std::vector<std::string>* tokens) {
    std::string token;
    while (int ch = in.get()) {
        if (ch == '\r' || ch == '\n') {
            break;
        }
        if (isspace(ch)) {
            if (!token.empty()) {
                tokens->push_back(token);
                token.clear();
            }
        } else {
            token.push_back(ch);
        }
    }
    if (!token.empty()) {
        tokens->push_back(token);
    }
    return tokens->size();
}

SimpleProtocol::SimpleProtocol(Config *config)
    :   config_(config),
        engine_(*config, &statistics_) {}

bool SimpleProtocol::HandleRequest(std::istream& request, std::ostream& response) {
    std::vector<std::string> tokens;
    if (ReadTokens(request, &tokens)) {
        const std::string command = tokens.front();
        tokens.erase(tokens.begin());
        if (command == "quit") {
            HandleQuit(response);
        } else if (command == "start") {
            HandleStart(tokens, response);
        } else if (command == "move") {
            HandleMove(tokens, response);
        } else if (command == "go") {
            HandleGo(response);
        } else if (command == "set") {
            HandleSet(tokens, response);
        } else if (command == "get") {
            HandleGet(tokens, response);
        } else if (command == "board") {
            HandleBoard(tokens, response);
        } else if (command == "print") {
            HandlePrint(response);
        } else if (command == "stats") {
            HandleStats(response);
        } else {
            response << "error: unknown command: " << command;
        }
        return true;
    }
    return false;
}

void SimpleProtocol::HandleQuit(std::ostream& response) {
    Quit();
    response << "bye";
}

void SimpleProtocol::HandleStart(const std::vector<std::string>& args, std::ostream& response) {
    if (args.empty() || args.size() > 2) {
        response << "error: bad arguments";
        return;
    }
    const int width = std::stoi(args[0]);
    const int height = args.size() == 2 ? std::stoi(args[1]) : width;
    if (width < Board::kMinSize || width > Board::kMaxSize ||
        height < Board::kMinSize || height > Board::kMaxSize) {
        response << "error: illegal size: " << width << "x" << height;
        return;
    }
    board_.Initialize(width, height);
    engine_.Start(width, height);
    response << "ok";
}

void SimpleProtocol::HandleMove(const std::vector<std::string>& args, std::ostream& response) {
    if (args.size() != 2) {
        response << "error: bad arguments";
        return;
    }
    const int x = std::stoi(args[0]);
    const int y = std::stoi(args[1]);
    const Cell move = MakeCell(x, y);
    if (!board_.IsEmptyCell(move)) {
        response << "error: illegal move: " << x << " " << y;
        return;
    }
    board_.Set(move, kPlayer);
    if (board_.IsTerminalMove(move, kPlayer, config_->is_exact_five())) {
        response << " player won";
    } else {
        response << "ok";
    }
}

void SimpleProtocol::HandleGo( std::ostream& response) {
    Cell move = engine_.GetBestMove(board_);
    if (board_.IsEmptyCell(move)) {
        board_.Set(move, kEngine);
    }
    response << GetX(move) << " " << GetY(move);
    if (board_.IsTerminalMove(move, kEngine, config_->is_exact_five())) {
        response << " engine won";
    }
}

void SimpleProtocol::HandleSet(const std::vector<std::string>& args, std::ostream& response) {
    if (args.size() == 2) {
        config_->Set(args[0], std::stoi(args[1]));
    }
}

void SimpleProtocol::HandleGet(const std::vector<std::string>& args, std::ostream& response) {
    if (!args.empty()) {
        response << config_->Get(args[0]);
    }
}

void SimpleProtocol::HandleBoard(const std::vector<std::string>& args, std::ostream& response) {
    if (args.size() != 3) {
        response << "error: bad arguments";
        return;
    }
    const int x = std::stoi(args[0]);
    const int y = std::stoi(args[1]);
    const Cell cell = MakeCell(x, y);
    if (!board_.IsInside(cell)) {
        response << "error: illegal cell: " << x << " " << y;
        return;
    }
    Stone stone = kBoundary;
    if (args[2] == "empty") {
        stone = kEmpty;
    } else if (args[2] == "player") {
        stone = kPlayer;
    } else if (args[2] == "engine") {
        stone = kEngine;
    } else if (args[2] == "forbidden") {
        stone = kForbidden;
    }
    if (stone == kBoundary) {
        response << "error: unknown value " << args[2];
        return;
    }
    board_.Set(cell, stone);
    response << "ok";
}

void SimpleProtocol::HandlePrint(std::ostream& response) {
    const int width = board_.width();
    const int height = board_.height();
    response << "   ";
    for (int x = 1; x <= width; x++) {
        int digit = x / 10;
        if (digit) {
            response << digit << " ";
        } else {
            response << "  ";
        }
    }
    response << std::endl << "   ";
    for (int x = 1; x <= width; x++) {
            response << x % 10 << " ";
    }
    response << std::endl;
    for (int y = 1; y <= height; y++) {
        response << std::setw(2) << y << " ";
        for (int x = 1; x <= width; x++) {
            const Stone stone = board_.stone(MakeCell(x, y));
            if (stone == kEmpty) {
                response << "+";
            } else if (stone == kEngine) {
                response << "O";
            } else if (stone == kPlayer) {
                response << "X";
            } else {
                response << "?";
            }
            if (x < width) {
                response << " ";
            }
        }
        response << std::endl;
    }
}

void SimpleProtocol::HandleStats(std::ostream& response) {
    statistics_.Print(response);
}

}  // namespace asparagus
