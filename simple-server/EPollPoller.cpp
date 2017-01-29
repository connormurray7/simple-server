#pragma once
#if defined(unix) || defined(__unix__) || defined(__unix)

#include "Poller.h"

#include <string>
#include <iostream>

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::runtime_error;


EPollPoller::EPollPoller() {}
void EPollPoller::loop_forever(int local_socket) {}
void EPollPoller::handle_request(int event) {}


#endif
