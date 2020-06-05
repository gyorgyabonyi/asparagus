// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include <iostream>

#include "config.h"
#include "controller.h"
#include "gomocup_protocol.h"
#include "randoms.h"
#include "simple_protocol.h"
#include "engine.h"

int main(int argc, char** argv) {
    asparagus::InitializeRandoms();
    asparagus::Config config;
    config.Load(argc, argv);
    asparagus::Engine engine(config);
    asparagus::Controller controller(config, &engine);
    asparagus::Protocol* protocol;
    if (config.use_gomocup_protocol()) {
        protocol = new asparagus::GomocupProtocol();
    } else {
        protocol = new asparagus::SimpleProtocol(&config, &controller);
    }
    while (protocol->is_running()) {
        if (protocol->HandleRequest(std::cin, std::cout)) {
            std::cout << std::endl;
            std::cout.flush();
        }
    }
    delete protocol;
    return 0;
}
