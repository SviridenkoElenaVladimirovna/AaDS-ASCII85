
PROJECT = ascii85

IDIR = .
CXX=g++

CXXFLAGS = -I$(IDIR)

ODIR = OBJ

LDIR = ../lib

LIBS = -lm

DEPS = ascii85.h 

OBJ = main.o ascii85.o

.PHONY: default
default: all

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(PROJECT): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)


all: $(PROJECT)

.PHONY: clean
clean:
	rm -f *.o *~ core

cleanall: clean
	rm -f $(PROJECT)
