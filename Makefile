CC=gcc
CXX=g++
CXXFLAGS=-std=c++14 -pthread
LDFLAGS=
LDLIBS=-ljack -lsndfile

OBJ=jackclient.o jack_playrec.o jackiowav.o errorhandling.o cli.o

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

jack_playrec: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	@rm -rf *.o
	@rm -f jack_playrec
