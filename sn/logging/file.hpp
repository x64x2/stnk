#ifndef stnik_LOGGING_FILE_H
#define stnik_LOGGING_FILE_H

#include <ctime>
#include <string>

#include <stnik/logging.hpp>

namespace stn {
  class logging_file:
  public logging
  {
    public:
    virtual ~logging_file();
    logging_file(const std::string &filename);
    
    protected:
    void deliver_message_text(const char *txt);
    
    private:
    std::string filename;
    void *vfp;
    time_t since;
    logging_file();
    logging_file(const logging_file &);
    logging_file &operator=(const logging_file &);
  }
}