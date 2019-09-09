/* Be sure to have first built and installed the Argent Core and Argent
 * Middleware Libraries. Including <argent/middleware.h> automatically includes
 * <argent/core.h>. */
#include <argent/middleware.h>


/* This is an example of how you would use the arm_mpool_alloc() function
 * without the Argent Core error handling mechanism. */
static void
alloc(void)
{
    int *bfr = NULL;
    int erno = arm_mpool_alloc ((void *) &bfr, sizeof *bfr);
}



/* This is an example of how you would use the arm_mpool_realloc() function
 * with the Argent Core error handling mechanism. */
static arc_erno
realloc(void)
{
ARC_TRY:
    ;int *bfr = NULL;
    arc_try (arm_mpool_alloc ((void *) &bfr, sizeof *bfr));


    const size_t newsize = sizeof *bfr * 5;
    arc_try (arm_mpool_realloc ((void *) &bfr, newsize ));

ARC_CATCH:
ARC_FINALLY:
    return arc_erno_get ();
}


