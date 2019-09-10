#include <gc.h>
#include "middleware.h"


    /* flag to check whether Boehm GC has been initialised */
static thread_local bool gc_flag = false;


    /* checks whether Boehm GC has been initialised */
static inline void
gc_init(void)
{
        /* initialise Boehm GC if it has already not been done so */
    if (arc_unlikely (!gc_flag)) {
        GC_INIT ();
        gc_flag = true;
    }
}


    /* implement the arm_mpool_alloc() interface function */
extern arc_hot arc_erno
arm_mpool_alloc(void **bfr, size_t sz)
{
ARC_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an
         * unallocated memory buffer and sz needs to be > 0 */
    arc_assert_handle (bfr && !*bfr);
    arc_assert_range (sz);

        /* allocate heap memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in allocating memory */
    gc_init ();
    *bfr = GC_MALLOC (sz);
    arc_assert_handle (*bfr);

ARC_CATCH:
        /* log and update current error code */
    arm_log_erno ();
    arm_log_error ("failed to allocate %lu bytes of memory", sz);
    arc_erno_set (ARM_ERNO_MPOOL);

ARC_FINALLY:
        /* return current error code */
    return arc_erno_get ();
}


    /* implement the arm_mpool_realloc() interface function */
extern arc_hot arc_erno
arm_mpool_realloc(void **bfr, size_t sz)
{
ARC_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an already
         * allocated memory buffer and sz needs to be > 0 */
    arc_assert_handle (bfr);
    arc_assert_range (sz);

        /* reallocate hep memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in reallocating memory */
    gc_init ();
    *bfr = GC_REALLOC (*bfr, sz);
    arc_assert_handle (*bfr);

ARC_CATCH:
        /* log and update current error code */
    arm_log_erno ();
    arm_log_error ("failed to allocate %lu bytes of memory", sz);
    arc_erno_set (ARM_ERNO_MPOOL);

ARC_FINALLY:
        /* return current error code */
    return arc_erno_get ();
}

