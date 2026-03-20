#ifndef stnik_LOGGING_TEE_H
#define stnik_LOGGING_TEE_H

#include <stnik/logging.hpp>

namespace stn {
  class logging_tee:
  public logging
  {
    public:
    virtual ~logging_tee();
    logging_tee(logging *lhs, logging *rhs);
    
    protected:
    void deliver_message_text(const char *txt);
    
    private:
    logging *lhs;
    logging *rhs;
    logging_tee();
    logging_tee(const logging_tee &);
    logging_tee &operator=(const logging_tee &);
  }
}
