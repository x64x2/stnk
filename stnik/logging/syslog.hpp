#ifndef stnik_LOGGING_SYSLOG_H
#define stnik_LOGGING_SYSLOG_H

#include <stnik/logging.hpp>

namespace stn {
  class logging_syslog:
  public logging
  {
    public:
    virtual ~logging_syslog(); 
    logging_syslog();
    
    protected:
    void deliver_message_text(const char *txt);
    
    private:
    bool is_open;
    logging_syslog(const logging_syslog &);
    logging_syslog &operator=(const logging_syslog &);
  }
}
