CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -pthread
LDFLAGS=
LDLIBS=-ljack -lsndfile

PLAYREC_OBJ=jackclient.o jack_playrec.o jackiowav.o errorhandling.o cli.o
PAR_OBJ=jackclient.o jack_par.o errorhandling.o

all: jack_playrec jack_par

%.o: %.cc
	@$(CXX) $(CXXFLAGS) -c $<

jack_playrec: $(PLAYREC_OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

jack_par: $(PAR_OBJ) 
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	@rm -rf *.o

distclean: clean
	@rm -f jack_playrec
	@rm -f jack_par
