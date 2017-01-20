CXX			= clang++
SOURCES 	= $(wildcard *.cpp)
OBJECTS		= $(SOURCES:%.cpp=%.o)
EXECUTABLE 	= main 

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic 
#-Werror 
release: CXXFLAGS += -O3 -DNDEBUG
release: all

debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

profile: CXXFLAGS += -pg
profile: clean all

all: $(OBJECTS)
	 $(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

Poller.o: Poller.h
ListeningSocket.o: ListeningSocket.h

main.o: main.cpp ../simple-server/ListeningSocket.h ../simple-server/Poller.h

%.o:
	 $(CXX) $(CXXFLAGS) -c $*.cpp

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all release debug profile clean
 	# disable built-in rules
.SUFFIXES:

