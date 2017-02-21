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
    
    ///Takes a folly MPMC queue, a handler that inherits from
    ///RequestHandler and a number of threads.
    Dequeuer(std::shared_ptr<folly::MPMCQueue<Request>> queue,
             std::shared_ptr<RequestHandler> handler,
            int num_threads);

    ///Non-blocking call that kicks off each of the threads
    ///which will block on the MPMC queue waiting to service
    ///a request.
    void begin();

private:

    int num_threads;
    std::shared_ptr<folly::MPMCQueue<Request>> task_queue;
    std::shared_ptr<RequestHandler> handler;
    std::vector<std::thread> workers;

};
