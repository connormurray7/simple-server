#include <string>
#include <memory>

#include "Dequeuer.h"

using std::shared_ptr;
using folly::MPMCQueue;

Dequeuer::Dequeuer(shared_ptr<MPMCQueue<string>> queue, RequestHandler& handler) 
    : queue(queue), handler(handler) {}

Dequeuer::run() {
    string r;
    while(true) {
        queue->blockingRead(r);
        handler.handle(r);
    }
}
        
