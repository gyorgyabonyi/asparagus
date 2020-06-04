// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_SIMPLE_PROTOCOL_H
#define ASPARAGUS_SIMPLE_PROTOCOL_H

#include <string>
#include <vector>

#include "board.h"
#include "engine.h"
#include "protocol.h"
#include "statistics.h"

namespace asparagus {

class Config;

class SimpleProtocol final : public Protocol {
public:
    explicit SimpleProtocol(Config* config);

    bool HandleRequest(std::istream& request, std::ostream& response) override;

private:
    Config* config_;
    Board board_;
    Engine engine_;
    Statistics statistics_;

    void HandleQuit(std::ostream& response);
    void HandleStart(const std::vector<std::string>& args, std::ostream& response);
    void HandleMove(const std::vector<std::string>& args, std::ostream& response);
    void HandleGo(std::ostream& response);
    void HandleSet(const std::vector<std::string>& args, std::ostream& response);
    void HandleGet(const std::vector<std::string>& args, std::ostream& response);
    void HandleBoard(const std::vector<std::string>& args, std::ostream& response);
    void HandlePrint(std::ostream& response);
    void HandleStats(std::ostream& response);
};

}  // namespace asparagus

#endif  // ASPARAGUS_SIMPLE_PROTOCOL_H
