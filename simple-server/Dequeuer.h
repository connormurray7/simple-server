#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <string>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"

///Contains a threadpool that handles
///requests off of the queue. Each thread
///writes make response. Folly MPMC queue
///is thread safe for multiple consumers.
class Dequeuer {
public:

    Dequeuer(std::shared_ptr<folly::MPMCQueue<Request>> queue,
             std::shared_ptr<RequestHandler> handler,
            int num_threads);

    void begin();

private:

    int num_threads;
    std::shared_ptr<folly::MPMCQueue<Request>> task_queue;
    std::shared_ptr<RequestHandler> handler;
    std::vector<std::thread> workers;

};
