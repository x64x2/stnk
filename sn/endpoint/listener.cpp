#include <stnik/config.hpp>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libexplain/accept.h>
#include <libexplain/bind.h>
#include <libexplain/fcntl.h>
#include <libexplain/listen.h>
#include <libexplain/setsockopt.h>
#include <libexplain/socket.h>

#include <stnik/reactor.hpp>
#include <stnik/endpoint/listener.hpp>


stn::endpoint_listener::~endpoint_listener()
{
}

stn::endpoint_listener::endpoint_listener(reactor &a_svc,int a_port_number, int a_repeat_count) :endpoint(-1),svc(a_svc),port_number(a_port_number),repeat_count(a_repeat_count)
{
}


stn::endpoint_listener::endpoint_listener(reactor &a_svc,const std::string &a_port_number, int a_repeat_count) :endpoint(-1),svc(a_svc),port_number(parse_port_number_or_die(a_port_number, true)),repeat_count(a_repeat_count)
{
}

int stn::endpoint_listener::get_read_file_descriptor()
{
    create_listening_socket();
    return endpoint::get_read_file_descriptor();
}


void stn::endpoint_listener::create_listening_socket()
{
    if (fd >= 0)
        return;
    if (port_number < 0)
    {
        port_number = get_default_port();
        assert(port_number > 0);
        assert(port_number < 65536);
    }
    fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
        fatal_error("%s", explain_socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));
    
        int one = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1)
    {
        fatal_error
        (
            "%s",
            explain_setsockopt
            (
                fd,
                SOL_SOCKET,
                SO_REUSEADDR,
                &one,
                sizeof(one)
            )
        );
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port_number);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fatal_error
        (
            "%s",
            explain_bind(fd, (struct sockaddr *)&addr, sizeof(addr))
        );
    }
    if (listen(fd, 5) < 0)
    {
        fatal_error("%s", explain_listen(fd, 5));
    }
    int flags = fcntl(fd, F_GETFD);
    if (flags < 0)
        fatal_error("%s", explain_fcntl(fd, F_GETFD, 0));
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFD, flags) < 0)
        fatal_error("%s", explain_fcntl(fd, F_SETFD, flags));
}


void stn::endpoint_listener::process_read()
{
    struct sockaddr addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t addrlen = sizeof(addr);
    int cfd = accept(fd, &addr, &addrlen);
    if (cfd < 0)
    {
        if (errno == EAGAIN)
            return;
        fatal_error("%s", explain_accept(fd, &addr, &addrlen));
    }

    if (repeat_count != -1)
    {
        --repeat_count;
        if (repeat_count <= 0)
        {
            kill_me_now();
            repeat_count = 0;
        }
    }
    endpoint::pointer ep = endpoint_factory(cfd, &addr, addrlen);
    add_endpoint(ep);
}


void stn::endpoint_listener::add_endpoint(const endpoint::pointer &ep)
{
    svc.add_endpoint(ep);
}

void stn::endpoint_listener::process_write()
{
    assert(false);
}