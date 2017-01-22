CXX		    = clang++
SOURCES		= $(wildcard *.cpp)
OBJECTS		= $(SOURCES:%.cpp=%.o)
EXECUTABLE	= main
VALGRIND	= valgrind --leak-check=yes
CXXFLAGS	= -std=c++14 -stdlib=libc++ -Wall -Wextra -pedantic
#-Werror 
release: CXXFLAGS += -O3
release: all

debug: CXXFLAGS += -g
debug: clean all

profile: CXXFLAGS += -pg
profile: clean all

all: $(OBJECTS)

ListeningSocket.o: ListeningSocket.h
Poller.o: Poller.h

%.o:
	 $(CXX) $(CXXFLAGS) -c $*.cpp

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

valgrind: debug
valgrind:
	$(VALGRIND) ./$(EXECUTABLE)

.PHONY: all release debug profile clean valgrind

.SUFFIXES: