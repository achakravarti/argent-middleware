#include <gc.h>
#include <threads.h>
#include "middleware.h"


    /* flag to check whether Boehm GC has been initialised */
static thread_local bool gc_flag = false;


    /* checks whether Boehm GC has been initialised */
static inline void
gc_init(void)
{
        /* initialise Boehm GC if it has already not been done so */
    if (ag_unlikely (!gc_flag)) {
        GC_INIT ();
        gc_flag = true;
    }
}


    /* implement the ag_mpool_alloc() interface function */
extern ag_hot ag_erno
ag_mpool_alloc(void **bfr, size_t sz)
{
AG_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an
         * unallocated memory buffer and sz needs to be > 0 */
    ag_assert_handle (bfr && !*bfr);
    ag_assert_range (sz);

        /* allocate heap memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in allocating memory */
    gc_init ();
    *bfr = GC_MALLOC (sz);
    ag_assert_handle (*bfr);

AG_CATCH:
        /* log and update current error code */
    ag_log_erno ();
    ag_log_error ("failed to allocate %lu bytes of memory", sz);
    ag_erno_set (AG_ERNO_MPOOL);

AG_FINALLY:
        /* return current error code */
    return ag_erno_get ();
}


    /* implement the ag_mpool_realloc() interface function */
extern ag_hot ag_erno
ag_mpool_realloc(void **bfr, size_t sz)
{
AG_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an already
         * allocated memory buffer and sz needs to be > 0 */
    ag_assert_handle (bfr);
    ag_assert_range (sz);

        /* reallocate hep memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in reallocating memory */
    gc_init ();
    *bfr = GC_REALLOC (*bfr, sz);
    ag_assert_handle (*bfr);

AG_CATCH:
        /* log and update current error code */
    ag_log_erno ();
    ag_log_error ("failed to allocate %lu bytes of memory", sz);
    ag_erno_set (AG_ERNO_MPOOL);

AG_FINALLY:
        /* return current error code */
    return ag_erno_get ();
}

