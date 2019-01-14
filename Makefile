# This file is part of jack_playrec
# Copyright (C) 2018  Hörtech gGmbH
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -pthread
LDFLAGS=
LDLIBS=-ljack -lsndfile

PLAYREC_OBJ=jackclient.o jack_playrec.o jackiowav.o errorhandling.o cli.o
PAR_OBJ=jackclient.o jack_par.o errorhandling.o

CODENAME=$(shell lsb_release -sc)
FULLVERSION=$(shell date +%Y%m%d)

all: jack_playrec jack_par

%.o: %.cc
	@$(CXX) $(CXXFLAGS) -c $<

jack_playrec: $(PLAYREC_OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

jack_par: $(PAR_OBJ) 
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean exe

exe: all
	./find_and_copy_dependencies.sh
	makensis jack_playrec.nsi

deb: all
	rm -Rf hoertech
	mkdir -p hoertech/$(CODENAME)
	htchdebian-mkdeb jack_playrec.csv $(FULLVERSION)
	mv *.deb hoertech/$(CODENAME)

upload: deb
	rsync -a hoertech jenkins@mha.physik.uni-oldenburg.de:workspace/htchdebian/repos/

clean:
	@rm -rf *.o

distclean: clean
	@rm -f jack_playrec
	@rm -f jack_par
	@rm -f *.deb
