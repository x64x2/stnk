#include <stnik/config.h>
#include <cerrno>
#include <cstring>
#include <libexplain/fopen.h>
#include <libexplain/fflush.h>

#include <stnik/logging/file.hpp>
#include <stnik/name.hpp>

stn::logging_file::~logging_file()
{
    if (vfp)
    {
        FILE *fp = (FILE *)vfp;
        vfp = 0;
        fclose(fp);
    }
}

stn::logging_file::logging_file(const std::string &a_filename):logging(),filename(a_filename),vfp(0),since(0)
{}

void stn::logging_file::deliver_message_text(const char *txt)
{
    if (vfp)
    {
        time_t now;
        time(&now);
        if (now - since >= 60 * 60)
        {
            FILE *fp = (FILE *)vfp;
            vfp = 0;
            fclose(fp);
        }
    }

    FILE *fp = (FILE *)vfp;
    if (!fp)
    {
        fp = fopen(filename.c_str(), "a");
        if (!fp)
        {
            int err = errno;
            fprintf
            (
                stderr,
                "%s: %s\n",
                progname_get().c_str(),
                explain_errno_fopen(err, filename.c_str(), "a")
            );
            fp = stderr;
        }
        vfp = fp;
    }

    fprintf(fp, "%s\n", txt);
    if (fflush(fp))
    {
        int err = errno;
        fprintf
        (
            stderr,
            "%s: %s\n",
            progname_get().c_str(),
            explain_errno_fflush(err, fp)
        );
        fclose(fp);
        vfp = 0;
    }
}
