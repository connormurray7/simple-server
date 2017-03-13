#include <string>
#include <iostream>

#include <folly/MPMCQueue.h>

#include "catch.hpp"
#include "../simple-server/Poller.h"
#include "../simple-server/Request.h"

using std::string;
using std::cout;
using std::endl;
using folly::MPMCQueue;


TEST_CASE("Poller starts and stops without exception", "[Poller]") {
    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);
    Poller* poller = new KQueuePoller(queue);
    delete poller;
}

