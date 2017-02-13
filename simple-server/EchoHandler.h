#pragma once

#include "RequestHandler.h"

#include <string>

class EchoHandler : public RequestHandler {
public:

    EchoHandler() {}

    void handle(std::string& request) {

    }
};
