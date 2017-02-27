#pragma once

#include <string>

#include "../simple-server/RequestHandler.h"


class EchoHandler : public RequestHandler {
public:

    EchoHandler(){}

    Response handle(Request& request) {
        Response response(request.fd, request.msg);
        return response;
    }
};
