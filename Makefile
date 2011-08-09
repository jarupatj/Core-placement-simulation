CC=g++
CFLAGS=-c -Wall -Werror -g
LDFLAGS=-L /usr/local/lib 
SOURCES=main.cpp RandomGenerator.cpp State.cpp Core.cpp Router.cpp Network.cpp SimulatedAnnealing.cpp Cost.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
NET_SOURCES = testNetwork.cpp Network.cpp Core.cpp Router.cpp
NET_OBJECTS=$(NET_SOURCES:.cpp=.o)
NET_EXE = testNetwork

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

network: $(NET_SOURCES) $(NET_EXE)

$(NET_EXE): $(NET_OBJECTS)
	$(CC) $(LDFLAGS) $(NET_OBJECTS) -o $@


clean:
	rm -fr *.o *~ $(EXECUTABLE)
