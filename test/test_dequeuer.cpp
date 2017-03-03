#include <string>
#include <iostream>
#include <folly/MPMCQueue.h>

#include "../simple-server/Request.h"
#include "../simple-server/Response.h"
#include "../simple-server/RequestHandler.h"
#include "catch.hpp"

using std::cout;
using std::endl;
using std::string;
using folly::MPMCQueue;

class TestRequestHandler : public RequestHandler {
public:
    TestRequestHandler() {}

    Response handle(Request& request) {
        cout << "Handling request " << requestsHandled++ << endl;
        return Response(request.fd, "");
    }
    
    int requestsHandled = 0;
};

TEST_CASE("Dequeuer handles requests on queue", "[Server]") {
    MPMCQueue<Request> queue = MPMCQueue<Request>(16);

    for(int i = 0; i < 10; ++i) {
        Request request(0, "request 0");
        queue.blockingWrite(std::forward<Request>(request));
    }

}
