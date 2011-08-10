CFLAGS=-c -Wall -Werror -g
LDFLAGS=-L /usr/local/lib 
SOURCES=main.cpp RandomGenerator.cpp State.cpp Core.cpp \
		  Router.cpp Network.cpp SimulatedAnnealing.cpp Cost.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sa
#specify the directory that make should search
VPATH = src

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

#create .o file from .cpp file
%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

NET_SOURCES = testNetwork.cpp Network.cpp Core.cpp Router.cpp
NET_OBJECTS=$(NET_SOURCES:.cpp=.o)
NET_EXE = testNetwork

network: $(NET_SOURCES) $(NET_EXE)

$(NET_EXE): $(NET_OBJECTS)
	$(CXX) $(LDFLAGS) $(NET_OBJECTS) -o $@

clean:
	rm -fr *.o *~ $(EXECUTABLE)
