/**
   \file jack_par.cc
   \brief Print jack parameters (bufsize, samplerate)
   \author Giso Grimm, Carl-von-Ossietzky Universitaet Oldenburg
   \date 2016
*/

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
