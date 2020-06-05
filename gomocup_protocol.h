// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_GOMOCUP_PROTOCOL_H
#define ASPARAGUS_GOMOCUP_PROTOCOL_H

#include "protocol.h"

#include "common.h"

namespace asparagus {

class GomocupProtocol final : public Protocol {
public:
    GomocupProtocol() = default;

    bool HandleRequest(std::istream& request, std::ostream& response) override;

private:
    DISALLOW_COPY_AND_ASSIGN(GomocupProtocol);
};

}  // namespace asparagus

#endif  // ASPARAGUS_GOMOCUP_PROTOCOL_H
