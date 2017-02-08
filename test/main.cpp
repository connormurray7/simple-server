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
    cout << "Thread was created " << endl;
    string r;
    while(true) {
        queue->blockingRead(r);
        cout << "Got this: [" << r << "]" << endl;
    }
}

void printThis(string s) {
    cout << s << endl;
}


int main() {
    
    ListeningSocket socket("127.0.0.1", "8080");
    int local_socket = socket.get_socket_fd();

    auto queue = make_shared<folly::MPMCQueue<string>>(1024);
    auto t = std::thread(checkLoop, queue);
    
    KQueuePoller poller(queue);
    poller.loop_forever(local_socket);
   
    return 0;
}


