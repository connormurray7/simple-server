#include <string>
#include <iostream>

#include "catch.hpp"
#include "../simple-server/ListeningSocket.h"

using std::string;
using std::cout;
using std::endl;

TEST_CASE("Can create multiple sockets", "[Socket]") {

    ListeningSocket socket("127.0.0.1", "8080");
    int fd = socket.get_socket_fd();
    cout << "Created socket with fd: " << fd << endl;

    ListeningSocket socket2("127.0.0.1", "8080");
    int fd2 = socket2.get_socket_fd();
    cout << "Created socket with fd: " << fd2 << endl;
}

