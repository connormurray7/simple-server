#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <folly/MPMCQueue.h>

#include "../simple-server/ListeningSocket.h"
#include "../simple-server/Poller.h"

using std::string;
using std::cout;
using std::endl;
using std::make_shared;

void checkLoop(std::shared_ptr<folly::MPMCQueue<string>> queue) {
    string r;
    while(true) {
        queue->blockingRead(r);
        cout << "Got this: [" << r << "]" << endl;
    }
}


int main() {
    
    ListeningSocket socket("127.0.0.1", "8080");
    int local_socket = socket.get_socket_fd();

    auto queue = make_shared<folly::MPMCQueue<string>>(1024);

    std::thread(checkLoop, queue);
    
    KQueuePoller poller(queue);
    poller.loop_forever(local_socket);
    
    return 0;
}


