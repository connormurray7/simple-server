CXX		     = g++
DEPENDENCIES = $(wildcard ../simple-server/*.o)
SOURCES		 = $(wildcard *.cpp)
OBJECTS		 = $(SOURCES:%.cpp=%.o)
EXECUTABLE	 = main
VALGRIND	 = valgrind --leak-check=yes
CXXFLAGS	 = -std=c++14 -Wall -Wextra -pedantic -llzma -lz -lsnappy -llz4 -levent -ldouble-conversion -lssl -lgflags -lglog -lboost_system -lfolly

release: CXXFLAGS += -O3
release: all

debug: CXXFLAGS += -g
debug: clean all

profile: CXXFLAGS += -pg
profile: clean all

run: all
run:
	chmod 755 main && ./main

all: $(OBJECTS) 
	 $(MAKE) -C ../simple-server/ 
	 $(CXX) $(CXXFLAGS) $(DEPENDENCIES) $(OBJECTS) -o $(EXECUTABLE)

main.o: main.cpp

%.o:
	 $(CXX) $(CXXFLAGS) -c $*.cpp

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

valgrind: debug
valgrind:
	$(VALGRIND) ./$(EXECUTABLE)

.PHONY: all release debug profile clean valgrind

.SUFFIXES:
