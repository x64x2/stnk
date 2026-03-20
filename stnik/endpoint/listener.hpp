#ifndef stnik_ENDPOINT_LISTENER_H
#define stnik_ENDPOINT_LISTENER_H

#include <stnik/endpoint.hpp>

namespace stn {
  class reactor; 
  
  class endpoint_listener:
  public endpoint
  {
    public:
    virtual ~endpoint_listener();
    
    protected:
    endpoint_listener(reactor &svc, int port_number, int repeat_count);
    endpoint_listener(reactor &svc, const std::string &port_number,int repeat_count);
    
    protected:
    void process_read();
    void process_write();
    void add_endpoint(const endpoint::pointer &ep);
    
    virtual int get_default_port() const = 0;
    virtual endpoint::pointer endpoint_factory(int fd, void *adr,int adrlen) = 0;
    int get_read_file_descriptor();
    
    private:
    int port_number;
    int repeat_count;
    void create_listening_socket();
    endpoint_listener();
    endpoint_listener(const endpoint_listener &);
    endpoint_listener &operator=(const endpoint_listener &);
  }
}