#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <folly/MPMCQueue.h>

#include "../simple-server/ListeningSocket.h"
#include "../simple-server/Poller.h"
#include "../simple-server/RequestHandler.h"
#include "../simple-server/Dequeuer.h"

using std::string;
using std::cout;
using std::endl;
using std::make_shared;
using folly::MPMCQueue;

int main() {
    
    ListeningSocket socket("127.0.0.1", "8080");
    int local_socket = socket.get_socket_fd();

    auto queue = make_shared<MPMCQueue<string>>(1024);
    auto handler = RequestHandler();
    auto dequeuer = Dequeuer(queue, handler, 1);

    auto t = std::thread(dequeuer.run);
    
    KQueuePoller poller(queue);
    poller.loop_forever(local_socket);
   
    return 0;
}


