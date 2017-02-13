#pragma once

#include <string>

class RequestHandler {
public:

    virtual void handle(std::string request) = 0;

private:

};
