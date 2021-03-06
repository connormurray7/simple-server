#include <string>
#include <iostream>

//#include <folly/MPMCQueue.h>

#include "catch.hpp"
#include "../simple-server/Poller.h"
#include "../simple-server/Server.h"
#include "../simple-server/Request.h"
#include "../simple-server/ListeningSocket.h"

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
//using folly::MPMCQueue;


TEST_CASE("Poller starts and stops without exception", "[Poller]") {
    ListeningSocket socket("127.0.0.1", "8080");
    int fd = socket.get_socket_fd();

    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);
    KQueuePoller poller(queue);

    poller.loop_forever(fd);
}
