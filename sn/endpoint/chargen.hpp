#ifndef stnik_ENDPOINT_CHARGEN_H
#define stnik_ENDPOINT_CHARGEN_H

#include <boost/shared_ptr.hpp>
#include <stnik/endpoint.hpp>

namespace stn {
  class endpoint_chargen:
  public endpoint
  {
    public:
    virtual ~endpoint_chargen();
    
    private:
    endpoint_chargen(int fd);
    
    public:
    static pointer create(int fd);
    
    protected:
    int get_write_file_descriptor();
    void process_read();
    void process_write();
    
    private:
    size_t sequence;
    char data[74];
    size_t data_size;
    size_t data_position;
    endpoint_chargen();
    endpoint_chargen(const endpoint_chargen &rhs);
    endpoint_chargen &operator=(const endpoint_chargen &rhs);
  }
}