#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <string>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"

class Dequeuer {
public:

    Dequeuer(std::shared_ptr<folly::MPMCQueue<std::string>> queue,
             std::shared_ptr<RequestHandler> handler,
            int num_threads);

    void begin();

private:

    int num_threads;
    std::shared_ptr<folly::MPMCQueue<std::string>> task_queue;
    std::shared_ptr<RequestHandler> handler;
    std::vector<std::thread> workers;

};
