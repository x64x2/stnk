#ifndef stnik_ENDPOINT_CLIENT_STDOUT_H
#define stnik_ENDPOINT_CLIENT_STDOUT_H

#include <stnik/endpoint/client.hpp>

namespace stn {
  
  class endpoint_client_stdout:
  
  public endpoint_client
  {
    public:
    virtual ~endpoint_client_stdout();
    
    private:
    endpoint_client_stdout(const std::string &hostname, int port);
    
    public:
    static pointer create(const std::string &hostname, int port);
    
    private:
    endpoint_client_stdout(const std::string &hostname,
      const std::string &port);
      
      public:
      static pointer create(const std::string &hostname, const std::string &port);
      
      protected:
      void data_received(const void *data, size_t nbytes);
      
      private:
      endpoint_client_stdout();
      endpoint_client_stdout(const endpoint_client_stdout &);
      endpoint_client_stdout &operator=(const endpoint_client_stdout &);
    }
}