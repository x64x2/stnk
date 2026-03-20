#ifndef stnik_LOGGING_STDERR_H
#define stnik_LOGGING_STDERR_H

#include <stnik/logging.hpp>

namespace stn {
  class logging_stderr:
  public logging
  {
    public:
    virtual ~logging_stderr();
    logging_stderr();
    
    protected:
    void deliver_message_text(const char *txt);
    
    private:
    logging_stderr(const logging_stderr &);
    logging_stderr &operator=(const logging_stderr &);
  }
}
