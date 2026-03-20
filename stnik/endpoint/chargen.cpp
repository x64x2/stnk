#include <stnik/config.hpp>
#include <cstring>
#include <libexplain/write.h>
#include <stnik/endpoint/chargen.hpp>

stn::endpoint_chargen::endpoint_chargen(int a_fd):endpoint(a_fd),sequence(0),data_size(0),data_position(0)
{}

stn::endpoint::pointer stn::endpoint_chargen::create(int a_fd)
{
    return pointer(new endpoint_chargen(a_fd));
}


void stn::endpoint_chargen::process_read()
{
    char buffer[1024];
    ssize_t n = ::read(fd, buffer, sizeof(buffer));
    if (n == 0)
        kill_me_now();
}


int stn::endpoint_chargen::get_write_file_descriptor()
{
    return fd;
}


void stn::endpoint_chargen::process_write()
{
    if (data_position >= data_size)
    {
        for (int j = 0; j < 72; ++j)
        {
            data[j] = ' ' + (sequence + j) % 95;
        }
        data[72] = '\r';
        data[73] = '\n';
        ++sequence;
        data_size = 74;
        data_position = 0;
    }
    size_t nbytes = data_size - data_position;
    void *ptr = data + data_position;
    ssize_t n = ::write(fd, ptr, nbytes);
    if (n < 0)
    {
        fatal_error("%s", explain_write(fd, ptr, nbytes));
        kill_me_now();
        return;
    }
    if (n == 0)
    {
        kill_me_now();
        return;
    }
    data_position += n;
}
