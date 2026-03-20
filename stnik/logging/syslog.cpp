#include <cstring>
#include <syslog.h>

#include <stnik/logging/syslog.hpp>
#include <stnik/name.hpp>

stn::logging_syslog::~logging_syslog()
{
    if (is_open)
    {
        closelog();
        is_open = false;
    }
}


stn::logging_syslog::logging_syslog() :
    logging(),
    is_open(false)
{
}


static int determine_level_from_text(const char *txt)
{
    struct table_t
    {
        const char *name;
        int level;
    };

    static const table_t table[] =
    {
        { "emerg", LOG_EMERG },
        { "emergency", LOG_EMERG },
        { "alert", LOG_ALERT },
        { "crit", LOG_CRIT },
        { "critical", LOG_CRIT },
        { "err", LOG_ERR },
        { "error", LOG_ERR },
        { "warn", LOG_WARNING },
        { "warning", LOG_WARNING },
        { "notice", LOG_NOTICE },
        { "info", LOG_INFO },
        { "information", LOG_INFO },
        { "debug", LOG_DEBUG },
        { 0, 0 }
    };

    for (const table_t *tp = table; tp->name; ++tp)
    {
        size_t len = strlen(tp->name);
        if (strncasecmp(tp->name, txt, len) == 0 && txt[len] == ':')
            return tp->level;
    }
    return LOG_ERR;
}


void stn::logging_syslog::deliver_message_text(const char *txt)
{
    int facility = LOG_USER;
    if (!is_open)
    {
        int option = LOG_PID;
        openlog(progname_get().c_str(), option, facility);
    }
    int level = determine_level_from_text(txt);
    int priority = facility | level;
    syslog(priority, "%s\n", txt);
}
