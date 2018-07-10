/**
  \file jackclient.cc
  \brief "jackclient" is a C++ wrapper for the JACK API.
  \author  Giso Grimm
  \date 2011
*/
  //TODO License


#include "jackclient.h"
#include <stdio.h>
#include "errorhandling.h"
#include <string.h>
#include <jack/thread.h>
#include <unistd.h>
#include <iostream>

static std::string errmsg("");

jackc_portless_t::jackc_portless_t(const std::string& clientname)
  : srate(0),active(false),xruns(0),xrun_latency(0)
{
  jack_status_t jstat;
  //jack_options_t opt(JackUseExactName |JackNoStartServer);
  jack_options_t opt((jack_options_t)(JackNoStartServer|JackUseExactName));
  jc = jack_client_open(clientname.c_str(),opt,&jstat);
  if( !jc ){
    std::string err("unable to open jack client: ");
    if( jstat & JackFailure )
      err += "Overall operation failed. ";
    if( jstat & JackInvalidOption )
      err += "The operation contained an invalid or unsupported option. ";
    if( jstat & JackNameNotUnique )
      err += "The desired client name was not unique. ";
    if( jstat & JackServerStarted )
      err += "The JACK server was started as a result of this operation. ";
    if( jstat & JackServerFailed )
      err += "Unable to connect to the JACK server. ";
    if( jstat & JackServerError )
      err += "Communication error with the JACK server. ";
    if( jstat & JackInitFailure )
      err += "Unable to initialize client. ";
    if( jstat & JackShmFailure )
      err += "Unable to access shared memory. ";
    if( jstat & JackVersionError )
      err += "Client's protocol version does not match. ";
    throw ErrMsg(err);
  }
  srate = jack_get_sample_rate(jc);
  fragsize = jack_get_buffer_size(jc);
  rtprio = jack_client_real_time_priority(jc);
  jack_set_xrun_callback(jc,jackc_portless_t::xrun_callback,this);
  jack_on_shutdown(jc,jackc_portless_t::on_shutdown,this);
}

jackc_portless_t::~jackc_portless_t()
{
  if( active )
    deactivate();
  int err(0);
  if( (err = jack_client_close(jc)) != 0 ){
    std::cerr << "Error: jack_client_close returned " << err << std::endl;
  }
}

uint32_t jackc_portless_t::tp_get_frame() const
{
  return jack_get_current_transport_frame(jc);
}

double jackc_portless_t::tp_get_time() const
{
  return (1.0/(double)srate)*(double)tp_get_frame();
}

void jackc_portless_t::activate()
{
  jack_activate(jc);
  active = true;
}

void jackc_portless_t::deactivate()
{
  if( active )
    jack_deactivate(jc);
  active = false;
}

void jackc_portless_t::connect(const std::string& src, const std::string& dest, bool bwarn)
{
  if( jack_connect(jc,src.c_str(),dest.c_str()) != 0 ){
    errmsg = std::string("unable to connect port '")+src + "' to '" + dest + "'.";
    if( bwarn )
      ;
    else
      throw ErrMsg(errmsg.c_str());
  }
}

jackc_t::jackc_t(const std::string& clientname)
  : jackc_portless_t(clientname)
{
  jack_set_process_callback(jc,process_,this);
}

jackc_t::~jackc_t()
{
  if( active ){
    deactivate();
    for(unsigned int k=0;k<inPort.size();k++)
      jack_port_unregister(jc,inPort[k]);
    for(unsigned int k=0;k<outPort.size();k++)
      jack_port_unregister(jc,outPort[k]);
  }
}

int jackc_t::process_(jack_nframes_t nframes, void *arg)
{
  return ((jackc_t*)(arg))->process_(nframes);
}

int jackc_t::process_(jack_nframes_t nframes)
{
  if( !active )
    return 0;
  for(unsigned int k=0;k<inBuffer.size();k++)
    inBuffer[k] = (float*)(jack_port_get_buffer(inPort[k],nframes));
  for(unsigned int k=0;k<outBuffer.size();k++)
    outBuffer[k] = (float*)(jack_port_get_buffer(outPort[k],nframes));
  return process(nframes,inBuffer,outBuffer);
}

void jackc_t::add_input_port(const std::string& name)
{
  if( (int)(strlen(jack_get_client_name(jc))+name.size()+2) >= jack_port_name_size())
    throw ErrMsg(std::string("Port name "+name+" is to long."));
  jack_port_t* p;
  p = jack_port_register(jc,name.c_str(),JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput,0);
  if( !p )
    throw ErrMsg(std::string("unable to register input port \""+name+"\"."));
  inPort.push_back(p);
  inBuffer.push_back(NULL);
  input_port_names.push_back(std::string(jack_get_client_name(jc))+":"+name);
}

void jackc_t::add_output_port(const std::string& name)
{
  if( (int)(strlen(jack_get_client_name(jc))+name.size()+2) >= jack_port_name_size())
    throw ErrMsg(std::string("Port name "+name+" is to long."));
  jack_port_t* p;
  p = jack_port_register(jc,name.c_str(),JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
  if( !p )
    throw ErrMsg(std::string("unable to register output port \""+name+"\"."));
  outPort.push_back(p);
  outBuffer.push_back(NULL);
  output_port_names.push_back(std::string(jack_get_client_name(jc))+":"+name);
}


int jackc_portless_t::xrun_callback(void *arg)
{
  ((jackc_portless_t*)arg)->xruns++;
  return 0;
}

void jackc_portless_t::on_shutdown(void *arg)
{
  ((jackc_portless_t*)arg)->active = false;
}

void jackc_t::connect_in(unsigned int port,const std::string& pname,bool bwarn)
{
  if( inPort.size() <= port ){
    DEBUG(port);
    DEBUG(inPort.size());
    throw ErrMsg("Input port number not available (connect_in).");
  }
  connect(pname,jack_port_name(inPort[port]),bwarn);
}

void jackc_t::connect_out(unsigned int port,const std::string& pname,bool bwarn)
{
  if( outPort.size() <= port ){
    DEBUG(port);
    DEBUG(outPort.size());
    throw ErrMsg("Output port number not available (connect_out).");
  }
  connect(jack_port_name(outPort[port]),pname,bwarn);
}

jackc_transport_t::jackc_transport_t(const std::string& clientname)
  : jackc_t(clientname)
{
}

int jackc_transport_t::process(jack_nframes_t nframes,const std::vector<float*>& inBuffer,const std::vector<float*>& outBuffer)
{ 
  jack_position_t pos;
  jack_transport_state_t jstate;
  jstate = jack_transport_query(jc,&pos);
  return process(nframes,inBuffer,outBuffer,pos.frame,jstate == JackTransportRolling);
}

void jackc_portless_t::tp_locate(double p)
{
  jack_transport_locate(jc,p*srate); 
}

void jackc_portless_t::tp_locate(uint32_t p)
{
  jack_transport_locate(jc,p); 
}

void jackc_portless_t::tp_start()
{
  jack_transport_start(jc); 
}

void jackc_portless_t::tp_stop()
{
  jack_transport_stop(jc); 
}


std::string jackc_portless_t::get_client_name()
{
  return jack_get_client_name(jc);
}

void * jackc_db_t::service(void* h)
{
  ((jackc_db_t*)h)->service();
  return NULL;
}

void jackc_db_t::service()
{
  pthread_mutex_lock( &mtx_inner_thread );
  while( !b_exit_thread ){
    usleep(10);
    if( active ){
      for(uint32_t kb=0;kb<2;kb++){
        if( pthread_mutex_trylock( &(mutex[kb]) ) == 0 ){
          if( buffer_filled[kb] ){
            inner_process(inner_fragsize, dbinBuffer[kb], dboutBuffer[kb] );
            buffer_filled[kb] = false;
          }
          pthread_mutex_unlock( &(mutex[kb]) );
        }
      }
    }
  }
  pthread_mutex_unlock( &mtx_inner_thread );
}

jackc_db_t::jackc_db_t(const std::string& clientname,jack_nframes_t infragsize)
  : jackc_t(clientname),
    inner_fragsize(infragsize),
    inner_is_larger(inner_fragsize>(jack_nframes_t)fragsize),
    current_buffer(0),
    b_exit_thread(false),
    inner_pos(0)
{
  buffer_filled[0] = false;
  buffer_filled[1] = false;
  if( inner_is_larger ){
    // check for integer ratio:
    ratio = inner_fragsize/fragsize;
    if( ratio*(jack_nframes_t)fragsize != inner_fragsize )
      throw ErrMsg("Inner fragsize is not an integer multiple of fragsize.");
    // create extra thread:
    pthread_mutex_init( &mtx_inner_thread, NULL );
    pthread_mutex_init( &mutex[0], NULL );
    pthread_mutex_init( &mutex[1], NULL );
    pthread_mutex_lock( &mutex[0] );
    if( 0 != jack_client_create_thread(jc,&inner_thread,std::max(-1,rtprio-1),(rtprio>0),service,this) )
      throw ErrMsg("Unable to create inner processing thread.");
  }else{
    // check for integer ratio:
    ratio = fragsize/inner_fragsize;
    if( (jack_nframes_t)fragsize != ratio*inner_fragsize )
      throw ErrMsg("Fragsize is not an integer multiple of inner fragsize.");
    
  }
}

jackc_db_t::~jackc_db_t()
{
  b_exit_thread = true;
  if( inner_is_larger ){
    pthread_mutex_lock( &mtx_inner_thread );
    pthread_mutex_unlock( &mtx_inner_thread );
    pthread_mutex_destroy( &mtx_inner_thread );
    for(uint32_t kb=0;kb<2;kb++){
      pthread_mutex_destroy( &(mutex[kb]) );
      for(uint32_t k=0;k<dbinBuffer[kb].size();k++)
        delete [] dbinBuffer[kb][k];
      for(uint32_t k=0;k<dboutBuffer[kb].size();k++)
        delete [] dboutBuffer[kb][k];
    }
  }
}  

void jackc_db_t::add_input_port(const std::string& name)
{
  if( inner_is_larger ){
    // allocate buffer:
    for(uint32_t k=0;k<2;k++)
      dbinBuffer[k].push_back(new float[inner_fragsize]);
  }else{
    for(uint32_t k=0;k<2;k++)
      dbinBuffer[k].push_back(NULL);
  }
  jackc_t::add_input_port(name);
}

void jackc_db_t::add_output_port(const std::string& name)
{
  if( inner_is_larger ){
    // allocate buffer:
    for(uint32_t k=0;k<2;k++)
      dboutBuffer[k].push_back(new float[inner_fragsize]);
  }else{
    for(uint32_t k=0;k<2;k++)
      dboutBuffer[k].push_back(NULL);
  }
  jackc_t::add_output_port(name);
}

int jackc_db_t::process(jack_nframes_t nframes,const std::vector<float*>& inBuffer,const std::vector<float*>& outBuffer)
{
  if( !active )
    return 0;
  int rv(0);
  if( inner_is_larger ){
    // copy data to buffer
    for( uint32_t k=0;k<inBuffer.size();k++)
      memcpy(&(dbinBuffer[current_buffer][k][inner_pos]),inBuffer[k],sizeof(float)*fragsize);
    // copy data from buffer
    for( uint32_t k=0;k<outBuffer.size();k++)
      memcpy(outBuffer[k],&(dboutBuffer[current_buffer][k][inner_pos]),sizeof(float)*fragsize);
    inner_pos += fragsize;
    // if buffer is full, lock other buffer and unlock current buffer
    if( inner_pos >= inner_fragsize ){
      uint32_t next_buffer((current_buffer+1)%2);
      pthread_mutex_lock(&(mutex[next_buffer]));
      buffer_filled[current_buffer] = true;
      pthread_mutex_unlock(&(mutex[current_buffer]));
      current_buffer = next_buffer;
      inner_pos = 0;
    }
  }else{
    for(uint32_t kr=0;kr<ratio;kr++){
      for(uint32_t k=0;k<inBuffer.size();k++)
        dbinBuffer[0][k] = &(inBuffer[k][kr*fragsize]);
      for(uint32_t k=0;k<outBuffer.size();k++)
        dboutBuffer[0][k] = &(outBuffer[k][kr*fragsize]);
      rv = inner_process(inner_fragsize,dbinBuffer[0],dboutBuffer[0]);
    }
  }
  return rv;
}

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */

