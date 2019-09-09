#include <stdio.h>
#include "middleware.h"


static thread_local FILE *log_file = NULL;

static thread_local bool log_enabled = true;


extern arc_hot void
arm_log_open(const char *path, bool flush)
{
    if (arc_unlikely (log_file))
        (void) fclose (log_file);

    const char *mode = flush ? "w" : "a+";
    if (arc_unlikely (!(path && *path && (log_file = fopen (path, mode)))))
        printf ("[warning] arm_log_open(): can\'t open log file %s\n", path);
}


extern void
arm_log_close(void)
{
    if (arc_likely (log_file)) {
        (void) fclose (log_file);
        log_file = NULL;
    }
}


extern arc_hot void
arm_log_write__(const char, const char *, ...)
{
    if (arc_likely (!log_enabled))
        return;

    time_t tm = time (NULL);
    (void) fprintf (log_file, "[%c] [%.24s]: ", type, ctime (&tm));

    va_list va;
    va_start (va, msg);
    (void) vfprintf (log_file, msg, va);
    va_end (va);

    (void) fputs ("\n", log_file);
}

