#include <stnik/config.hpp>
#include <cstring>
#include <libexplain/write.h>
#include <stnik/endpoint/daytime.hpp>

stn::endpoint_daytime::endpoint_daytime(int a_fd):endpoint(a_fd),data_position(0),data_size(0)
{
    time_t now;
    time(&now);
    struct tm *tmp = localtime(&now);
    strftime(data, sizeof(data), "%c\n", tmp);
    strlen(data);
}

stn::endpoint::pointer stn::endpoint_daytime::create(int a_fd)
{
    return pointer(new endpoint_daytime(a_fd));
}

void stn::endpoint_daytime::process_read()
{
    char buffer[1024];
    ssize_t n = ::read(fd, buffer, sizeof(buffer));
    if (n == 0)
        kill_me_now();
}

int stn::endpoint_daytime::get_write_file_descriptor()
{
    return fd;
}

void stn::endpoint_daytime::process_write()
{
    if (data_position >= data_size)
    {
        kill_me_now();
    }
    else
    {
        size_t nbytes = data_size - data_position;
        void *ptr = data + data_position;
        ssize_t n = ::write(fd, ptr, nbytes);
        if (n < 0)
            fatal_error("%s", explain_write(fd, ptr, nbytes));
        data_position += n;
    }
}