CC=g++
CFLAGS=-c -Wall -Werror -g
LDFLAGS=-L /usr/local/lib 
SOURCES=main.cpp RandomGenerator.cpp State.cpp Core.cpp Router.cpp Network.cpp SimulatedAnnealing.cpp Cost.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fr *.o *~ $(EXECUTABLE)
