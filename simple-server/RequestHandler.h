#pragma once

#include <string>

class RequestHandler {
public:
    
    RequestHandler() {}
    
    ~RequestHandler() {}

    virtual Response handle(Request& request) = 0;

private:

};
