CXXFLAGS = -c -Wall -Werror -O2
DEBUG = -g
LDFLAGS =-L /usr/local/lib 
SOURCES = main.cpp State.cpp Core.cpp Utils.cpp\
		   Router.cpp Network.cpp SimulatedAnnealing.cpp Cost.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE=sa
#specify the directory that make should search
VPATH = src

all: $(EXECUTABLE)

debug: CXXFLAGS += $(DEBUG)
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

#create .o file from .cpp file
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -fr *.o *~ $(EXECUTABLE)