#include <stnik/config.hpp>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <libexplain/fork.h>
#include <stnik/reactor.hpp>
#include <stnik/endpoint/listener/daytime.hpp>
#include <stnik/logging/syslog.hpp>
#include <stnik/name.hpp>
#include <stnik/print_version.hpp>


static void usage()
{
    std::string prog = stn::progname_get();
    fprintf(stderr, "Usage: %s [ <option>... ]\n", prog.c_str());
    fprintf(stderr, "%s -V\n", prog.c_str());
    exit(1);
}

int main(int argc, char **argv)
{
    std::string port_num;
    stn::name_set(argv[0]);
    bool debug = false;
    int repeat_count = -1;
    for (;;)
    {
        int c = getopt(argc, argv, "adp:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'a':
            repeat_count = 1;
            break;

        case 'd':
            debug = true;
            break;

        case 'p':
            port_num = optarg;
            break;

        case 'V':
            stn::version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind != argc)
        usage();

    if (!debug)
    {
        if (explain_fork_or_die())
            return 0;
        stn::logging::register_stream(new stn::logging_syslog());
    }
    return 0;
}


//vim: set tw=8 sw=4 et
