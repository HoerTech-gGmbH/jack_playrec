/**
   \file jackiowav.h
   \brief simultaneously play a sound file and record from jack
   \author Giso Grimm
   \date 2012,2014,2017
*/

//TODO License
#ifndef JACKIOWAV_H
#define JACKIOWAV_H

#include <iostream>
#include <vector>
#include "errorhandling.h"
#include "jackclient.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

class jackio_t : public jackc_transport_t {
public:
  /**
     \param ifname Input file name
     \param ofname Output file name
     \param ports Output and Input ports (the first N ports are assumed to be output ports, N = number of channels in input file)
     \param jackname Jack client name
     \param freewheel Optionally use freewheeling mode
     \param autoconnect Automatically connect to hardware ports.
     \param verbose Show more infos on console.
  */
  jackio_t(const std::string& ifname,const std::string& ofname,
	   const std::vector<std::string>& ports,const std::string& jackname = "jackio",int freewheel = 0,int autoconnect = 0, bool verbose = false);
  jackio_t(double duration,const std::string& ofname,
	   const std::vector<std::string>& ports,const std::string& jackname = "jackio",int freewheel = 0,int autoconnect = 0, bool verbose = false);
  void set_transport_start(double start, bool wait);
  ~jackio_t();
  /**
     \brief start processing
  */
  void run();
private:
  SNDFILE* sf_in;
  SNDFILE* sf_out;
  SF_INFO sf_inf_in;
  SF_INFO sf_inf_out;
  float* buf_in;
  float* buf_out;
  unsigned int pos;
  bool b_quit;
  bool start;
  bool freewheel_;
  bool use_transport;
  uint32_t startframe;
  uint32_t nframes_total;
  std::vector<std::string> p;
  int process(jack_nframes_t nframes,const std::vector<float*>& inBuffer,const std::vector<float*>& outBuffer,uint32_t tp_frame, bool tp_rolling);
  void log(const std::string& msg);
  bool b_cb;
  bool b_verbose;
  bool wait_;
public:
  float cpuload;
  uint32_t xruns;
};

#endif

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */
