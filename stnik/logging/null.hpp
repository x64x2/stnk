#ifndef stnik_LOGGING_NULL_H
#define stnik_LOGGING_NULL_H

#include <stnik/logging.hpp>

namespace stn {
  class logging_null:
  public logging
  {
    public: 
    virtual ~logging_null();
    logging_null();
    
    protected:
    void deliver_message_text(const char *txt);
    private:
    logging_null(const logging_null &rhs);
    logging_null &operator=(const logging_null &rhs);
  }
}
