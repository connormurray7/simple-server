#pragma once

#include "../simple-server/RequestHandler.h"

#include <string>

class EchoHandler : public RequestHandler {
public:

    EchoHandler(){}

    Response handle(Request& request) {
        Response response(request.fd, request.msg);
        return response;
    }
};
