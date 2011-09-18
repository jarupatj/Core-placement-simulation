CXX = mpicxx
CXXFLAGS = -c -O2
DEBUG = -g
LDFLAGS =-L /usr/local/lib 
SOURCES = mpiJob.cpp State.cpp Core.cpp Utils.cpp Router.cpp\
		   Network.cpp Simulator.cpp Cost.cpp Utilization.cpp
OBJECTS = $(SOURCES:.cpp=.o) SFMT.o
EXECUTABLE=mpiJob
#specify the directory that make should search
VPATH = ../src:../src/SFMT

all: $(EXECUTABLE)

debug: CXXFLAGS += $(DEBUG)
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

#create .o file from .cpp file
SFMT.o : SFMT.c
	$(CXX) -c -DMEXP=19937 $< -o $@
%.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -fr *.o *~ $(EXECUTABLE)
