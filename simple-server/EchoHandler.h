#pragma once

#include "RequestHandler.h"

#include <string>

class EchoHandler : public RequestHandler {
public:

    Response handle(Request& request) {
        //TODO Implement the handler.
        Response response(request.fd, request.request);
        return response;
    }
};
