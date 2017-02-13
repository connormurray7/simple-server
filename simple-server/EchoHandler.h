#pragma once

#include "RequestHandler"

#include <string>

class EchoHandler : public RequestHandler {
public:

    EchoHandler() {}

    void handle(std::string& request) {

    }
}
