    /* be sure to have first built and installed the Argent Core and Argent
     * Middleware Libraries. Including <argent/middleware.h> automatically
     * includes <argent/core.h> */
#include <argent/middleware.h>


    /* this is an example of a function that makes a call to ag_log_erno() when
     * it encounters an error; note that ag_log_erno() can only be called in
     * either the AG_TRY, AG_CATCH, or AG_FINALLY blocks */
static ag_erno
foo(void)
{
AG_TRY:
    void *bfr = NULL;
    ag_assert_handle (bfr);

AG_CATCH:
    ag_log_erno ();

AG_FINALLY:
    return ag_erno_get ();
}


int main(void)
{
        /* this is how you would open a log file, flushing out its existing
         * contents */
    const char *path = "/path/to/log/file";
    ag_log_open (path, true);

        /* this is how you would write a formatted trace message to the opened
         * log file */
    ag_log_trace ("There are %d days in %d year.\n", 365, 1);

        /* this is how you would write an unformatted warning message to the
         * opened log file */
    ag_log_warning ("Warning! You need to be careful!");

        /* calling ag_log_open() again closes the currently open log file and
         * opens a new one, setting the latter as the default; in this case, the
         * existing contents of the new log file are preserved */
    ag_log_open ("/another/log/file", false);

        /* this is how you would write a formatted error message to the
         * currently open log file */
    ag_log_error ("Exception 0x%x caught in %s\n", 0xDEADBEEF, __func__);

        /* this is how you would close the currently open log file after you are
         * done */
    ag_log_close ();

    return 0;
}

