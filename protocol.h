// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_PROTOCOL_H
#define ASPARAGUS_PROTOCOL_H

#include <istream>
#include <ostream>

#include "common.h"

namespace asparagus {

class Protocol {
public:
    Protocol();
    virtual ~Protocol() = default;

    constexpr bool is_running() const { return is_running_; }

    void Quit();

    virtual bool HandleRequest(std::istream& request, std::ostream& response) = 0;

private:
    bool is_running_;

    DISALLOW_COPY_AND_ASSIGN(Protocol);
};

}  // namespace asparagus

#endif  // ASPARAGUS_PROTOCOL_H
