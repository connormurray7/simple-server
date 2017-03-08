#include <string>
#include <iostream>

#include "catch.hpp"
#include "../simple-server/Server.h"

using std::string;
using std::cout;
using std::endl;

class TestRequestHandler : public RequestHandler {
public:
    TestRequestHandler() {}

    Response handle(Request& request) {
        cout << "Received request [" << request.msg << "]" << endl;
        Response response(request.fd, request.msg);
        return response;
    }
};

TEST_CASE("Server starts fine on local addr", "[Server]") {

    Server<TestRequestHandler> server("127.0.0.1", "8080", 1);
    server.run();
}
