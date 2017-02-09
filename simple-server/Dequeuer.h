#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"

class Dequeuer {
public:

    Dequeuer(std::shared_ptr<folly::MPMCQueue<string>> queue,
            RequestHandler& handler,
            int num_threads);

    void begin();

private:

    std::shared_ptr<folly::MPMCQueue<string>> queue;
    RequestHandler handler;
    std::vector<std::thread> workers;

}
