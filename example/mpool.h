/* Be sure to have first built and installed the Argent Core and Argent
 * Middleware Libraries. Including <argent/middleware.h> automatically includes
 * <argent/core.h>. */
#include <argent/middleware.h>


/* This is an example of how you would use the ag_mpool_alloc() function
 * without the Argent Core error handling mechanism. */
static void
alloc(void)
{
    int *bfr = NULL;
    ag_erno e = ag_mpool_alloc ((void **) &bfr, sizeof *bfr);
}



/* This is an example of how you would use the ag_mpool_realloc() function
 * with the Argent Core error handling mechanism. */
static ag_erno
realloc(void)
{
AG_TRY:
    ;int *bfr = NULL;
    ag_try (ag_mpool_alloc ((void **) &bfr, sizeof *bfr));


    const size_t newsize = sizeof *bfr * 5;
    ag_try (ag_mpool_realloc ((void **) &bfr, newsize ));

AG_CATCH:
AG_FINALLY:
    return ag_erno_get ();
}


