#ifndef stnik_ENDPOINT_CLIENT_H
#define stnik_ENDPOINT_CLIENT_H

#include <ctime>
#include <deque>

#include <stnik/endpoint.hpp>

namespace stn {
  class endpoint_client:
  public endpoint
  {
    public:
    virtual ~endpoint_client();
    
    protected:
    endpoint_client(const std::string &hostname, int port);
    endpoint_client(const std::string &hostname, const std::string &port);
    
    public:
    void write(const std::string &text);
    bool more_to_send() const { return !write_queue.empty(); }
    bool is_connected() const { return (fd >= 0 && !connecting); }
    int get_port() const { return port; }
    const std::string &get_host() const { return hostname; }
    
    protected:
    int get_read_file_descriptor();
    void process_read();
    int get_write_file_descriptor();
    void process_write();
    virtual void data_received(const void *data, size_t nbytes) = 0;
    
    private:
    std::string hostname;
    int port;
    void *sock_addr;
    int sock_addr_size;
    bool connecting;
    
    time_t retry_after;
    typedef std::deque<std::string> write_queue_t;
    write_queue_t write_queue;
    void try_to_connect();

    endpoint_client();
    endpoint_client(const endpoint_client &);
    endpoint_client &operator=(const endpoint_client &);
  }
}
