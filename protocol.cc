// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#include "protocol.h"

namespace asparagus {

Protocol::Protocol()
    :   is_running_(true) {}

void Protocol::Quit() {
    is_running_ = false;
}

}  // namespace asparagus
