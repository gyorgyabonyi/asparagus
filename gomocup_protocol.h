// Copyright (c) 2020 Gyorgy Abonyi. All Rights Reserved.

#ifndef ASPARAGUS_GOMOCUP_PROTOCOL_H
#define ASPARAGUS_GOMOCUP_PROTOCOL_H

#include "protocol.h"

namespace asparagus {

class GomocupProtocol final : public Protocol {
public:
    bool HandleRequest(std::istream& request, std::ostream& response) override;
};

}  // namespace asparagus

#endif  // ASPARAGUS_GOMOCUP_PROTOCOL_H
