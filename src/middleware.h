#if !defined ARGENT_MIDDLEWARE
#define ARGENT_MIDDLEWARE


#include <argent/core.h>


#define ARM_ERNO_MPOOL ((arc_erno) 0x00010001)


extern arc_hot arc_erno
arm_mpool_alloc(void **bfr, size_t sz);


extern arc_hot rush_erno
arm_mpool_realloc(void **bfr, size_t sz);


#endif /* !defined ARGENT_MIDDLEWARE */

