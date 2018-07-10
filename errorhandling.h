#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H
#include <exception>
#include <string>
#define DEBUG(x) std::cerr << __FILE__ << ":" << __LINE__ << ": " << __PRETTY_FUNCTION__ << " " << #x << "=" << x << std::endl

  class ErrMsg : public std::exception {
  public:
    ErrMsg(const std::string& msg);
    virtual ~ErrMsg()=default;
    virtual const char* what() const throw();

  private:
    std::string msg;
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
