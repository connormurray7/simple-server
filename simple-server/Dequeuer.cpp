#include <memory>
#include <vector>
#include <string>
#include <folly/MPMCQueue.h>

#include "Dequeuer.h"

using std::string;
using std::vector;
using std::thread;
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

void Dequeuer::begin() {
    for(auto& worker: workers) {
        worker = thread(run);
    }
}

void Dequeuer::run() {
    string r;
    while(true) {
        queue->blockingRead(r);
        handler.handle(r);
    }
}
        
