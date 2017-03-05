#include <string>
#include <iostream>
#include <memory>
#include <folly/MPMCQueue.h>

#include "../simple-server/Request.h"
#include "../simple-server/Response.h"
#include "../simple-server/RequestHandler.h"
#include "../simple-server/Dequeuer.h"
#include "catch.hpp"

using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;
using folly::MPMCQueue;

class TestRequestHandler : public RequestHandler {
public:
    TestRequestHandler() {}

    Response handle(Request& request) {
        cout << "Handling request " << request.msg << endl;
        return Response(request.fd, "");
    }
};

TEST_CASE("Dequeuer handles requests on queue", "[Server]") {
    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);

    for(int i = 0; i < 10; ++i) {
        Request request(i, "request " + std::to_string(i));
        queue->blockingWrite(std::forward<Request>(request));
    }

    auto handler = shared_ptr<TestRequestHandler>(new TestRequestHandler());
    Dequeuer dequeuer(queue, handler, 1);

    dequeuer.begin();

}
