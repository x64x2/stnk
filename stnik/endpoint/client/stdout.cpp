#include <stnik/config.hpp>
#include <libexplain/write.hpp>
#include <stnik/endpoint/client/stdout.hpp>
#include <stnik/logging.hpp>



stn::endpoint_client_stdout::endpoint_client_stdout(const std::string &a_hostname, int a_port_number):endpoint_client(a_hostname, a_port_number)
{}

stn::endpoint::pointer stn::endpoint_client_stdout::create(const std::string &a_hostname,int a_port_number)
{
    return pointer(new endpoint_client_stdout(a_hostname, a_port_number));
}


stn::endpoint_client_stdout::endpoint_client_stdout(const std::string &a_hostname, const std::string &a_port_number):endpoint_client(a_hostname, a_port_number)
{}


stn::endpoint::pointer stn::endpoint_client_stdout::create(const std::string &a_hostname,const std::string &a_port_number)
{
    return pointer(new endpoint_client_stdout(a_hostname, a_port_number));
}

void stn::endpoint_client_stdout::data_received(const void *data, size_t nbytes)
{
    if (nbytes == 0)
    {
        kill_me_now();
        return;
    }
    int ofd = fileno(stdout);
    if (ssize_t(nbytes) != ::write(ofd, data, nbytes))
    {
        log().fatal_error("%s", explain_write(ofd, data, nbytes));
        kill_me_now();
    }
}
