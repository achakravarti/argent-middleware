#include <stdio.h>
#include <threads.h>
#include <time.h>
#include "middleware.h"


    /* handle to log file */
static thread_local FILE *log_file = NULL;


    /* implement the ag_log_open() interface function */
extern ag_hot void
ag_log_open(const char *path, bool flush)
{
        /* close log file if it's alread open */
    if (ag_unlikely (log_file))
        (void) fclose (log_file);

        /* open log file in the appropriate mode; display an error message in
         * the unlikely event that the log file can't be opened */
    const char *mode = flush ? "w" : "a+";
    if (ag_unlikely (!(path && *path && (log_file = fopen (path, mode)))))
        printf ("[warning] ag_log_open(): can\'t open log file %s\n", path);
}


    /* implement the ag_log_close() interface function */
extern void
ag_log_close(void)
{
        /* close log file if it's already open */
    if (ag_likely (log_file)) {
        (void) fclose (log_file);
        log_file = NULL;
    }
}


    /* implement the private helper function ag_log_write__(); this function
     * performs the core process of writing timestamped log messages and is
     * called by the interface macros */
extern ag_hot void
ag_log_write__(const char type, const char *msg, ...)
{
        /* write log entry header with current timestamp and message type */
    time_t tm = time (NULL);
    (void) fprintf (log_file, "[%c] [%.24s]: ", type, ctime (&tm));

        /* write variable argument list */
    va_list va;
    va_start (va, msg);
    (void) vfprintf (log_file, msg, va);
    va_end (va);

        /* close entry with a newline character */
    (void) fputs ("\n", log_file);
}

