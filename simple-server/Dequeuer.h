#pragma once

#include <memory>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"

class Dequeuer {
public:

    Dequeuer(std::shared_ptr<folly::MPMCQueue<string>> queue,
            RequestHandler& handler);

private:
    std::shared_ptr<folly::MPMCQueue<string>> queue;
    RequestHandler handler;

}
