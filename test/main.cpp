#include "../simple-server/ListeningSocket.h"
#include "../simple-server/Poller.h"

#include <folly/MPMCQueue.h>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int main() {
    
    ListeningSocket socket("127.0.0.1", "8080");
    int local_socket = socket.get_socket_fd();

    folly::MPMCQueue<string> queue(1024);
    
    KQueuePoller poller(&queue);
    poller.loop_forever(local_socket);
    
    return 0;
}


