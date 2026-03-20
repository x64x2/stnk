#include <stnik/config.h>
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <libexplain/connect.h>
#include <libexplain/fcntl.h>
#include <libexplain/getsockopt.h>
#include <libexplain/read.h>
#include <libexplain/socket.h>
#include <libexplain/write.h>

#include <stnik/logging.hpp>
#include <stnik/endpoint/client.hpp>


stn::endpoint_client::~endpoint_client()
{
    delete (struct sockaddr_in *)sock_addr;
}


stn::endpoint_client::endpoint_client(const std::string &a_hostname,int a_port):endpoint(-1),hostname(a_hostname),port(a_port),sock_addr(0),sock_addr_size(0),connecting(false),retry_after(0)
{
    char buf[100];
    snprintf(buf, sizeof(buf), "host \"%s\": port %d", hostname.c_str(), port);
    set_peer_name(buf);
    try_to_connect();
}


stn::endpoint_client::endpoint_client(const std::string &a_hostname,const std::string &a_port):endpoint(-1),hostname(a_hostname),port(0),sock_addr(0),sock_addr_size(0),connecting(false),retry_after(0)
{
    char buf[100];
    cout<<
    (
        buf,
        sizeof(buf),
        "host \"%s\": port \"%s\"",
        hostname.c_str(),
        a_port.c_str()
    )<<endl;
    set_peer_name(buf);
    parse_port_number_or_die(a_port);
    try_to_connect();
}


void stn::endpoint_client::write(const std::string &text)
{
    write_queue.push_back(text + "\n");
}


int stn::endpoint_client::get_write_file_descriptor()
{
    try_to_connect();
    if (connecting)
        return fd;
    return (write_queue.empty() ? -1 : fd);
}

void stn::endpoint_client::process_write()
{
    if(connecting)
    {
        assert(fd >= 0);
        connecting = false;
        int error = 0;
        socklen_t len = sizeof(error);
        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
        {
            fatal_error
            (
                "%s",
                explain_getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len)
            );
        }
    }

    if (fd < 0)
        return;
    if (write_queue.empty())
        return;
    std::string s = write_queue.front();
    ssize_t n = ::write(fd, s.c_str(), s.size());
    int err = errno;
    if (n != ssize_t(s.size()))
    {
        warning("%s", explain_errno_write(err, fd, s.c_str(), s.size()));
        close();
        kill_me_now();
        return;
    }
    write_queue.pop_front();
}


int stn::endpoint_client::get_read_file_descriptor()
{
    try_to_connect();
    if (connecting)
        return -1;
    return fd;
}


void stn::endpoint_client::process_read()
{
    assert(fd >= 0);
    if (fd < 0)
        return;

    char buffer[1 << 12];
    size_t nbytes = sizeof(buffer);
    ssize_t n = ::read(fd, buffer, nbytes);
    if (n == -1)
    {
        int err = errno;
        fatal_error("%s", explain_errno_read(err, fd, buffer, nbytes));
    }
    if (n == 0)
    {
        warning("connection closed by peer");
        data_received(0, 0);
        close();
        return;
    }
    data_received(buffer, n);
}


void stn::endpoint_client::try_to_connect()
{
    if (fd >= 0)
        return;
    if (connecting)
        return;
    time_t now;
    time(&now);
    if (now < retry_after)
        return;
    retry_after = now + 10;
    if (!sock_addr)
    {
        struct hostent *he = gethostbyname(hostname.c_str());
        if (!he)
        {
            warning("gethostbyname: %s", hstrerror(h_errno));
            kill_me_now();
            return;
        }
        struct sockaddr_in *sain_p = new struct sockaddr_in;
        memset(sain_p, 0, sizeof(*sain_p));
        sain_p->sin_addr.s_addr = *(in_addr_t *)he->h_addr;
        sain_p->sin_family = AF_INET;
        sain_p->sin_port = htons(port);
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        fatal_error("%s", explain_socket(AF_INET, SOCK_STREAM, 0));

    int flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        fatal_error("%s", explain_fcntl(fd, F_GETFL, 0));
    flags |= O_NONBLOCK;
    int err = fcntl(fd, F_SETFL, flags);
    if (err < 0)
        fatal_error("%s", explain_fcntl(fd, F_SETFL, flags));

    err = connect(fd, (const struct sockaddr *)sock_addr, sock_addr_size);
    if (err == 0)
    {
        connecting = false;
        return;
    }
    int error_num = errno;
    switch (error_num)
    {
    case EAGAIN:
    case EALREADY:
    case EINPROGRESS:
        // Cool damn heels and wait.
        return;

    case ECONNREFUSED:
    case ENETUNREACH:
        warning
        (
            "%s",
            explain_errno_connect
            (
                error_num,
                fd,
                (const struct sockaddr *)sock_addr,
                sock_addr_size
            )
        );
        close();
        kill_me_now();
        return;

    default:
        break;
    }

    fatal_error
    (
        "%s",
        explain_errno_connect
        (
            error_num,
            fd,
            (const struct sockaddr *)sock_addr,
            sock_addr_size
        )
    );
}