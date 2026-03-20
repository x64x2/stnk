#include <cstdio>
#include <stnik/logging/stderr.hpp>
#include <stnik/name.hpp>

stn::logging_stderr::logging_stderr():logging()
{}


void stn::logging_stderr::deliver_message_text(const char *txt)
{
    fprintf(stderr, "%s: %s\n", progname_get().c_str(), txt);
}
