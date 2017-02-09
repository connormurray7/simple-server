#include <string>
#include <memory>

#include "Dequeuer.h"

using std::shared_ptr;
using folly::MPMCQueue;

Dequeuer::Dequeuer(shared_ptr<MPMCQueue<string>> queue, 
        RequestHandler& req_handler,
        int num_threads) 
{
    task_queue = queue;
    handler = handler;
    workers = vector<thread>(num_threads, thread());
}

Dequeuer::begin() {
    for(auto& worker: workers) {
        worker = thread(run);
    }
}

Dequeuer::run() {
    string r;
    while(true) {
        queue->blockingRead(r);
        handler.handle(r);
    }
}
        
