#pragma once

#include <string>

#include "Request.h"
#include "Response.h"

class RequestHandler {
public:
    
    RequestHandler() {}
    
    ~RequestHandler() {}

    virtual Response handle(Request& request) = 0;

private:

};
