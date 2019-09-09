#include <stdio.h>
#include "middleware.h"


    /* handle to log file */
static thread_local FILE *log_file = NULL;


    /* implement the arm_log_open() interface function */
extern arc_hot void
arm_log_open(const char *path, bool flush)
{
        /* close log file if it's alread open */
    if (arc_unlikely (log_file))
        (void) fclose (log_file);

        /* open log file in the appropriate mode; display an error message in
         * the unlikely event that the log file can't be opened */
    const char *mode = flush ? "w" : "a+";
    if (arc_unlikely (!(path && *path && (log_file = fopen (path, mode)))))
        printf ("[warning] arm_log_open(): can\'t open log file %s\n", path);
}


    /* implement the arm_log_close() interface function */
extern void
arm_log_close(void)
{
        /* close log file if it's already open */
    if (arc_likely (log_file)) {
        (void) fclose (log_file);
        log_file = NULL;
    }
}


    /* implement the private helper function arm_log_write__(); this function
     * performs the core process of writing timestamped log messages and is
     * called by the interface macros */
extern arc_hot void
arm_log_write__(const char type, const char *msg, ...)
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

