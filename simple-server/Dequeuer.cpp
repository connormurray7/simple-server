#include <memory>
#include <vector>
#include <string>
#include <netdb.h>
#include <folly/MPMCQueue.h>

#include "Dequeuer.h"

using std::string;
using std::vector;
using std::thread;
using std::shared_ptr;
using folly::MPMCQueue;


void send_response(Response& response) {
    int len = response.msg.size() + 1;
    char buf[len];
    
    response.msg.copy(buf, len);
    send(response.fd, buf, len, 0);
}

void run(shared_ptr<MPMCQueue<Request>> task_queue, shared_ptr<RequestHandler> handler) {
    Request request;
    Response response;
    while(true) {
        task_queue->blockingRead(request);
        response = handler->handle(request);
        send_response(response);
        //write response back
    }
}

Dequeuer::Dequeuer(shared_ptr<MPMCQueue<Request>> queue,
        shared_ptr<RequestHandler> req_handler,
        int num_threads_in)
{
    task_queue = queue;
    handler = req_handler;
    num_threads = num_threads_in;
}

void Dequeuer::begin() {
    for(int t = 0; t < num_threads; ++t) {
        workers.push_back(thread(run, task_queue, std::ref(handler)));
    }
}
