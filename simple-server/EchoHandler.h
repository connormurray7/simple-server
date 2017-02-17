#pragma once

#include "RequestHandler.h"

#include <string>

class EchoHandler : public RequestHandler {
public:

    Response handle(Request& request) {
        Response response(request.fd, request.msg);
        return response;
    }
};
