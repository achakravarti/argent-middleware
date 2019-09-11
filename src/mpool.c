#include <gc.h>
#include "middleware.h"


    /* flag to check whether Boehm GC has been initialised */
static thread_local bool gc_flag = false;


    /* checks whether Boehm GC has been initialised */
static inline void
gc_init(void)
{
        /* initialise Boehm GC if it has already not been done so */
    if (agc_unlikely (!gc_flag)) {
        GC_INIT ();
        gc_flag = true;
    }
}


    /* implement the agm_mpool_alloc() interface function */
extern agc_hot agc_erno
agm_mpool_alloc(void **bfr, size_t sz)
{
AGC_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an
         * unallocated memory buffer and sz needs to be > 0 */
    agc_assert_handle (bfr && !*bfr);
    agc_assert_range (sz);

        /* allocate heap memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in allocating memory */
    gc_init ();
    *bfr = GC_MALLOC (sz);
    agc_assert_handle (*bfr);

AGC_CATCH:
        /* log and update current error code */
    agm_log_erno ();
    agm_log_error ("failed to allocate %lu bytes of memory", sz);
    agc_erno_set (AGM_ERNO_MPOOL);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement the agm_mpool_realloc() interface function */
extern agc_hot agc_erno
agm_mpool_realloc(void **bfr, size_t sz)
{
AGC_TRY:
        /* assert preconditions; bfr needs to be a valid pointer to an already
         * allocated memory buffer and sz needs to be > 0 */
    agc_assert_handle (bfr);
    agc_assert_range (sz);

        /* reallocate hep memory to bfr through the Boehm GC, ensuring that the
         * GC has been initialised and is successful in reallocating memory */
    gc_init ();
    *bfr = GC_REALLOC (*bfr, sz);
    agc_assert_handle (*bfr);

AGC_CATCH:
        /* log and update current error code */
    agm_log_erno ();
    agm_log_error ("failed to allocate %lu bytes of memory", sz);
    agc_erno_set (AGM_ERNO_MPOOL);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}

