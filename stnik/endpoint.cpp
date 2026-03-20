#include <stnik/config.hpp>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <libexplain/close.h>
#include <libexplain/getpeername.h>

#include <stnik/reactor.hpp>
#include <stnik/endpoint.hpp>
#include <stnik/logging.hpp>
#include <stnik/time_value.hpp>


stn::endpoint::~endpoint()
{
    close();
}


void stn::endpoint::close()
{
    for (;;)
    {
        if (::close(fd) == 0)
            break;
        if (errno == EBADF)
            break;
        log().warning("%s", explain_errno_close(errno, fd));
        usleep(10);
    }
    fd = -1;
}


stn::endpoint::endpoint(int a_fd):fd(a_fd),ready_to_die(false)
{
    set_peer_name();
}

stn::endpoint::endpoint(int a_fd, const void *sadr, int sadr_len):fd(a_fd),ready_to_die(false)
{
    set_peer_name(sadr, sadr_len);
}


void stn::endpoint::set_peer_name(const char *s)
{
    if(s && *s)
}

void stn::endpoint::set_peer_name(const std::string &s)
{
    if (!s.empty())
        peer_name = s;
}

void stn::endpoint::set_peer_name(const void *addr_p, int addr_len)
{
    if (addr_len != sizeof(sockaddr_in))
        return;
    sockaddr_in *addr = (sockaddr_in *)addr_p;
    in_addr_t *ap = &addr->sin_addr.s_addr;
    hostent *he = gethostbyaddr(ap, sizeof(*ap), AF_INET);
    if (!he)
    {
        set_peer_name(inet_ntoa(addr->sin_addr));
        return;
    }
    set_peer_name(he->h_name);
}

void stn::endpoint::set_peer_name()
{
    if (fd < 0)
        return;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    unsigned sa_len = sizeof(sa);
    if (getpeername(fd, (struct sockaddr *)&sa, &sa_len))
    {
        if (errno != ENOTSOCK)
        {
            warning
            (
                "%s",
                explain_errno_getpeername
                (
                    errno,
                    fd,
                    (struct sockaddr *)&sa,
                    &sa_len
                )
            );
        }
    }
    set_peer_name(&sa, sa_len);
}

bool stn::endpoint::is_ready_to_die()
    const
{
    return ready_to_die;
}

void stn::endpoint::kill_me_now()
{
    ready_to_die = true;
}

int stn::endpoint::get_read_file_descriptor()
{
    return fd;
}

int stn::endpoint::get_write_file_descriptor()
{
    return -1;
}

void stn::endpoint::process_read()
{
    assert(!"derived class stnikst override this method");
}

void stn::endpoint::process_write()
{
    assert(!"derived class stnikst override this method");
}

void stn::endpoint::fatal_error(const char *fmt, ...)
{
    va_start(ap, fmt);
    fatal_error_v(fmt, ap);
    va_end(ap);
}

void stn::endpoint::fatal_error_v(const char *fmt, va_list ap)
{
    const char *name = get_peer_name();
    if (!name || !*name)
    {
        log().fatal_error_v(fmt, ap);
        return;
    }

    char buffer[1000];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    log().fatal_error("%s: %s", name, buffer);
}

void stn::endpoint::warning(const char *fmt, ...)
{
    va_start(ap,fmt);
    warning_v(fmt,ap);
    va_end(ap);
}

void stn::endpoint::warning_v(const char *fmt, va_list ap)
{
    const char *name = get_peer_name();
    if (!name || !*name)
    {
        log().warning_v(fmt, ap);
        return;
    }
    char buffer[1000];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    log().warning("%s: %s", name, buffer);
}

stn::time_value stn::endpoint::get_maximum_sleep()const
{
    return time_value(60);
}

void stn::endpoint::process_timeout()
{}

int stn::endpoint::parse_port_number(const std::string &text)
{
    struct servent *sp = getservbyname(text.c_str(), "tcp");
    if (sp)
        return ntohs(sp->s_port);
    char *ep = 0;
    long n = strtol(text.c_str(), &ep, 0);
    if (ep == text.c_str() || *ep)
        return -1;
    return n;
}

int stn::endpoint::parse_port_number_or_die(const std::string &text,bool empty_is_ok)
{
    if (text.empty() && empty_is_ok)
        return -1;
    int n = parse_port_number(text);
    if (n < 0)
    {
        log().fatal_error
        (
            "unable to interpret \"%s\" as a port number",
            text.c_str()
        );
    }
    return n;
}
