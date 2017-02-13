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

void run(shared_ptr<MPMCQueue<string>> task_queue, RequestHandler& handler) {
    string r;
    while(true) {
        task_queue->blockingRead(r);
        handler.handle(r);
    }
}

Dequeuer::Dequeuer(shared_ptr<MPMCQueue<string>> queue, 
        RequestHandler req_handler,
        int num_threads_in)
{
    task_queue = queue;
    handler = handler;
    num_threads = num_threads_in;
}

void Dequeuer::begin() {
    for(int t = 0; t < num_threads; ++t) {
        workers.push_back(thread(run, task_queue, std::ref(handler)));
    }
}

        
