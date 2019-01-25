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

include config.mk

CXXFLAGS+=-pthread
LDFLAGS+=
LDLIBS+=-ljack -lsndfile

PLAYREC_OBJ=jackclient.o jack_playrec.o jackiowav.o errorhandling.o cli.o
PAR_OBJ=jackclient.o jack_par.o errorhandling.o

all: jack_playrec jack_par

%.o: %.cc
	@$(CXX) $(CXXFLAGS) -c $<

jack_playrec: $(PLAYREC_OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

jack_par: $(PAR_OBJ)
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean exe

exe: install
	$(MAKE) -C packaging/exe

deb: install
	$(MAKE) -C packaging/deb

pkg: install
	$(MAKE) -C packaging/pkg

install: all
	@mkdir -p  $(DESTDIR)$(PREFIX)/bin
	@install -m 755 jack_playrec $(DESTDIR)$(PREFIX)/bin/.
	@install -m 755 jack_par $(DESTDIR)$(PREFIX)/bin/.

uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/jack_playrec
	@rm -f $(DESTDIR)$(PREFIX)/bin/jack_par

clean:
	@rm -rf *.o

distclean: clean
	@rm -f jack_playrec
	@rm -f jack_par
