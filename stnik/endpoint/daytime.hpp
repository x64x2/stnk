#ifndef stnik_ENDPOINT_DAYTIME_H
#define stnik_ENDPOINT_DAYTIME_H

#include <stnik/endpoint.hpp>

namespace stn {
  class endpoint_daytime:
  public endpoint
  {
    public:
    virtual ~endpoint_daytime();
    
    private:
    endpoint_daytime(int fd);
    
    public:
    static pointer create(int fd);
    
    protected:
    int get_write_file_descriptor();
    void process_read();
    void process_write();
    
    private:
    char data[100];
    size_t data_position;
    size_t data_size;
    endpoint_daytime();
    endpoint_daytime(const endpoint_daytime &rhs);
    endpoint_daytime &operator=(const endpoint_daytime &rhs);
  }
}