#if !defined ARGENT_MIDDLEWARE
#define ARGENT_MIDDLEWARE


#include <argent/core.h>


extern arc_hot void
arm_log_open(const char *path, bool flush);


extern void
arm_log_close(void);


extern arc_hot void
arm_log_write__(const char, const char *, ...);


#define arm_log_trace(m, ...) \
    arm_log_write__('T', (m), ##__VA_ARGS__)


#define arm_log_warning(m, ...) \
    arm_log_write__('W', (m), ##__VA_ARGS__)


#define arm_log_error(m, ...) \
    arm_log_write__('E', (m), ##__VA_ARGS__)


/**************************************************************************//**
 * @defgroup mpool Argent Memory Pool Module
 * Garbage-collected heap memory management.
 *
 * Compared to manual memory management, garbage collection provides a few
 * advantages, namely the prevention of potential memory leaks and reduced heap
 * memory fragmentation. Apart from the collection cycle, garbage collection may
 * also improve performance by negating the need for frequent memory releasing
 * operations.
 *
 * The Memory Pool Module is a wrapper around the Boehm Garbage Collector. The
 * Boehm GC is one of the most mature and widely used garbage collectors in the
 * C/C++ ecosystem.
 * @{
 */


/**
 * Indicate memory pool error.
 *
 * The @c ARM_ERNO_MPOOL symbolic code is an error code which indicates that one
 * of the memory pool module interface functions has failed.
 *
 * @see arm_mpool_alloc()
 * @see arm_mpool_realloc()
 */
#define ARM_ERNO_MPOOL ((arc_erno) 0x00010001)


/**
 * Allocate heap memory from Boehm GC.
 *
 * The @c arm_mpool_alloc() function allocates @p sz bytes of heap memory to a
 * buffer @p bfr. The heap memory is allocated, and subsequently managed, by the
 * Boehm Garbage Collector.
 *
 * @param bfr Buffer to allocate heap memory.
 * @param sz Size in bytes of heap memory to allocate.
 *
 * @return ARC_ERNO_NULL if successful.
 * @return ARM_ERNO_MPOOL if unsuccessful.
 *
 * @note @p bfr is expected to be a valid pointer to an unallocated buffer, and
 * @p sz is expected to be greater than 0.
 *
 * @warning Don't attempt to release the memory allocated to @p bfr through the
 * standard @c free() function; the heap memory allocated to @p bfr is managed
 * by the Boehm Garbage Collector.
 *
 * @see arm_mpool_realloc()
 */
extern arc_hot arc_erno
arm_mpool_alloc(void **bfr, size_t sz);


/**
 * Reallocate heap memory from Boehm GC.
 *
 * The @c arm_mpool_realloc() function reallocates @p sz bytes of heap memory to
 * a buffer @p bfr. This buffer is expected to have been allocated earlier by a
 * call to @c arm_mpool_alloc(). The heap memory is reallocated and managed by
 * the Boehm Garbage Collector.
 *
 * @param bfr Buffer to reallocate heap memory.
 * @param sz Size in bytes of heap memory to reallocate.
 *
 * @return ARC_ERNO_NULL if successful.
 * @return ARC_ERNO_MPOOL if unsuccessful.
 *
 * @note @p bfr is expected to be a valid pointer to an allocated buffer, and @p
 * sz is expected to be greater than 0.
 *
 * @warning Don't attempt to release the memory allocated to @p bfr through the
 * standard @c free() function; the heap memory allocated to @p bfr is managed
 * by the Boehm Garbage Collector.
 *
 * @see arm_mpool_alloc()
 */
extern arc_hot rush_erno
arm_mpool_realloc(void **bfr, size_t sz);


/**
 * @example mpool.h
 * This is an example showing how to code against the Argent Middleware Memory
 * Pool Module interface.
 * @}
 */


#endif /* !defined ARGENT_MIDDLEWARE */

