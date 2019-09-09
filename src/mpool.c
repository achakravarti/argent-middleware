#include <gc.h>
#include "middleware.h"


static thread_local bool gc_flag = false;


static inline void
gc_init(void)
{
    if (arc_unlikely (!gc_flag)) {
        GC_INIT ();
        gc_flag = true;
    }
}


extern arc_hot arc_erno
arm_mpool_alloc(void **bfr, size_t sz)
{
ARC_TRY:
    arc_assert_handle (bfr && !*bfr);
    arc_assert_range (sz);

    gc_init ();
    *bfr = GC_MALLOC (sz);
    arc_assert_handle (*bfr);

ARC_CATCH:
    arc_erno_set (ARM_ERNO_MPOOL);

ARC_FINALLY:
    return arc_erno_get ();
}


extern arc_hot rush_erno
arm_mpool_realloc(void **bfr, size_t sz)
{
ARC_TRY:
    arc_assert_handle (bfr);
    arc_assert_range (sz);

    gc_init ();
    *bfr = GC_REALLOC (*bfr, sz);
    arc_assert_handle (*bfr);

ARC_CATCH:
    arc_erno_set (ARC_ERNO_MPOOL);

ARC_FINALLY:
    return arc_erno_get ();
}

