#include "errorhandling.h"

ErrMsg::ErrMsg(const std::string& msg) 
  : msg(msg) 
{}

const char* ErrMsg::what() const throw(){
  return msg.c_str();
}
/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */
