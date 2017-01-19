#include "ListeningSocket.h"

#include "Poller.h"

int main() {
    
    ListeningSocket socket;
    
    KQueuePoller poller;
    poller.loop_forever(local_socket);
    
    return 0;
}
