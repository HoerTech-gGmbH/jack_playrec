// jack_par prints jack parameters (bufsize, samplerate)
// This file is part of jack_playrec
// Copyright (C) 2018  Hörtech gGmbH
// Copyright (C) 2014 2015 2016 Giso Grimm, Carl-von-Ossietzky Universitaet Oldenburg
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "jackclient.h"
#include <iostream>

int main(int argc, char** argv)
{
  try{
    jackc_portless_t jc("jack_par");
    std::cout << jc.get_fragsize() << " " << jc.get_srate() << "\n";
  }
  catch( const std::exception& msg ){
    std::cerr << "Error: " << msg.what() << std::endl;
    return 1;
  }
  catch( const char* msg ){
    std::cerr << "Error: " << msg << std::endl;
    return 1;
  }
  return 0;
}

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */
