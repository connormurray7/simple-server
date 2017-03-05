CXX        = clang++
SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(SOURCES:%.cpp=%.o)
EXECUTABLE = main
VALGRIND   = valgrind --leak-check=yes
CXXFLAGS   = -std=c++14 -stdlib=libc++ -Wall -Werror -Wextra -pedantic
LINKERFLAGS = -llzma -lz -lsnappy -llz4 -levent -ldouble-conversion -lssl -lgflags -lglog -lboost_system -lfolly
DEPENDENCIES = ../simple-server/*.o

release: CXXFLAGS += -O3
release: all

debug: CXXFLAGS += -g
debug: clean all

profile: CXXFLAGS += -pg
profile: clean all

run: all
run:
	chmod 755 main && ./main

all: $(OBJECTS) $(DEPENDENCIES)
	 $(CXX) $(CXXFLAGS) $(LINKERFLAGS) $(DEPENDENCIES) $(OBJECTS) -o $(EXECUTABLE)

test_server.o: test_server.cpp
test_socket.o: test_socket.cpp
test_dequeuer.o: test_dequeuer.cpp

main.o: main.cpp

%.o:
	 $(CXX) $(CXXFLAGS) -c $*.cpp

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all release debug profile clean

.SUFFIXES:
