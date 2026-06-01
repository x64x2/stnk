#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stnik/reactor.h>
#include <stnik/endpoint/client/stdout.h>
#include <stnik/logging.h>
#include <stnik/progname.h>
#include <stnik/version_print.h>


static void usage()
{
    std::string prog = stn::progname_get();
    fprintf(stderr,"Usage: %s <hostname> <port>\n", prog.c_str());
    fprintf(stderr,"%s -V\n", prog.c_str());
    exit(1);
}


int main(int argc, char **argv)
{
    stn::name_set(argv[0]);
    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            stn::print_version();
            return 0;

        default:
            usage();
        }
    }
    std::string hostname;
    std::string port;
    switch (argc - optind)
    {
    case 2:
        hostname = argv[optind];
        port = argv[optind + 1];
        break;

    default:
        usage();
    }
    return 0;
}


// im: set tw=8 sw=4 et
