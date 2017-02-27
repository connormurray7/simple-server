#pragma once

#include <string>

#include "Request.h"
#include "Response.h"

///Abstract class that should be extended to
///provide functionality for parsing the request.
///See example, EchoHandler that responds back.
class RequestHandler {
public:

    virtual Response handle(Request& request) = 0;

private:

};
