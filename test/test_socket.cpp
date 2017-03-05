#include <string>
#include <iostream>
#include <unordered_set>

#include "catch.hpp"
#include "../simple-server/ListeningSocket.h"

using std::string;
using std::cout;
using std::endl;
using std::unordered_set;


TEST_CASE("Can create multiple sockets on same port", "[Socket]") {

    unordered_set<int> fds;

    for(int i = 0; i < 15; ++i) {
        ListeningSocket socket("127.0.0.1", "8080");
        int fd = socket.get_socket_fd();
        REQUIRE(fds.find(fd) == fds.end());
        fds.insert(fd);
        cout << "Created socket with fd: " << fd << endl;
    }
}

