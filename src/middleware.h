#if !defined ARGENT_MIDDLEWARE
#define ARGENT_MIDDLEWARE


#include <stdarg.h>
#include <argent/core.h>


/**************************************************************************//**
 * @defgroup log Argent Middleware Logging Module
 * Runtime logging of events.
 *
 * Logging is an essential component for any sizable application. The Argent
 * Middleware Logging Module provides an interface for writing timestamped log
 * messages. These messages can be decorated as trace, warning or error messages
 * in order to make the context apparent and the message easily grep-able.
 *
 * The logging module attempts to be as thread-safe as possible, but this has
 * not been confirmed as yet. The logging module has been intentionally designed
 * to not raise any errors, and instead, handle failure conditions either
 * silently, or at most, with a warning message printed on to @c stdout.
 * @{
 */


/**
 * Open default log file.
 *
 * The @c agm_log_open() function opens a log file with a given fully qualified
 * @p path where all log messages will be saved by default. In case the log file
 * specified by @p path does not exist, then it will be created. If the log file
 * is opened with the flag @p flush set to @c true, then any existing data in
 * the log file will be erased. Conversely, if the flag @p flush is set to be @c
 * false, theny any existing data in the log file will be retained, and the new
 * log messages will be appended after them.
 *
 * @param path Fully qualifed path of log file.
 * @param flush Flag to flush log file on opening.
 *
 * @note There is no need to call @c agm_log_allow() after calling this function
 * as @c agm_log_open() allows logging by default. Also note that calling the @c
 * agm_log_open() function is safe even if a default log file is currently open;
 * in such a case, the currently open log file will be first closed before the
 * new one is opened. In the unlikely event of @c agm_log_open() failing to open
 * the log file, then an appropriate warning is printed onto @c stdout.
 *
 * @warning Be sure to close the log file opened with @c agm_log_open() once you
 * are done with it by making a call to @c agm_log_close() in order to prevent
 * resource leaks. Unlike dynamically allocated pointers on the heap, the Argent
 * Middleware Library does not manage the automatic clean-up of log files.
 *
 * @see agm_log_close()
 */
extern ag_hot void
agm_log_open(const char *path, bool flush);


/**
 * Close default log file.
 *
 * The @c agm_log_close() function closes the default log file that was opened
 * earlier by a call to @c agm_log_open(). Be sure to call this function after
 * you are done using the log file in order to prevent resource leaks.
 *
 * @note It is safe to call this function even if the default log file has not
 * been successfully opened. This is an intended feature to allow this function
 * to be safely called in case an error has occurred.
 *
 * @warning Unlike dynamically allocated pointers on the heap, the Argent
 * Middleware Library does not manage the automatic clean-up of log files.
 *
 * @see agm_log_open()
 */
extern void
agm_log_close(void);


/**
 * @private
 * Private helper function for the logging macros defined below.
 */
extern ag_hot void
agm_log_write__(const char, const char *, ...);


/**
 * Log trace message.
 *
 * The @c agm_log_trace() macro writes a formatted trace message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [T] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c agm_log_open(); however, no message will be logged.
 *
 * @see agm_log_warning()
 * @see agm_log_error()
 */
#define agm_log_trace(m, ...) \
    agm_log_write__('T', (m), ##__VA_ARGS__)


/**
 * Log warning message.
 *
 * The @c agm_log_warning() macro wirtes a formatted warning message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [W] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c agm_log_open(); however, no message will be logged.
 *
 * @see agm_log_trace()
 * @see agm_log_error()
 */
#define agm_log_warning(m, ...) \
    agm_log_write__('W', (m), ##__VA_ARGS__)


/**
 * Log error message.
 *
 * The @c agm_log_error() macro wirtes a formatted error message @p m in the
 * currently open log file. The message is decorated with the current local
 * system time and a [E] prefix to make it easily grep-able.
 *
 * @param m Formatted message to log.
 * @param ... Format tokens.
 *
 * @note It is safe to call this macro even if the log file has not been opened
 * by an earlier call to @c agm_log_open(); however, no message will be logged.
 *
 * @see agm_log_trace()
 * @see agm_log_warning()
 */
#define agm_log_error(m, ...) \
    agm_log_write__('E', (m), ##__VA_ARGS__)


/**
 * Log current error code.
 *
 * The @c agm_log_erno() macro logs the error code in the current context if it
 * is not AG_ERNO_NULL. The current function, file name and line number are
 * also logged.
 *
 * @note This is a convenience wrapper around @c agm_log_error(), and it is safe
 * to call this macro even if the log file has not been opened by an earlier
 * call to @c agm_log_open(); however, in that case, no message will be logged.
 *
 * @warning Since this macro makes a call to @c ag_erno_get(), it can only be
 * used within an @c AG_TRY, @c AG_CATCH, or @c AG_FINALLY block.
 *
 * @see agm_log_error()
 */
#define agm_log_erno()                                                 \
do {                                                                   \
    if (ag_likely (ag_erno_get ())) {                                \
        agm_log_error ("error 0x%x detected in %s() [%s:%d]",          \
                       ag_erno_get (), __func__, __FILE__, __LINE__); \
    }                                                                  \
} while (0)


/**
 * @example log.h
 * This is an example showing how to code against the PCR Logging Module
 * interface.
 * @}
 */


/**************************************************************************//**
 * @defgroup mpool Argent Middleware Memory Pool Module
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
 * The @c AGM_ERNO_MPOOL symbolic code is an error code which indicates that one
 * of the memory pool module interface functions has failed.
 *
 * @see agm_mpool_alloc()
 * @see agm_mpool_realloc()
 */
#define AGM_ERNO_MPOOL ((ag_erno) 0x00010001)


/**
 * Allocate heap memory from Boehm GC.
 *
 * The @c agm_mpool_alloc() function allocates @p sz bytes of heap memory to a
 * buffer @p bfr. The heap memory is allocated, and subsequently managed, by the
 * Boehm Garbage Collector.
 *
 * @param bfr Buffer to allocate heap memory.
 * @param sz Size in bytes of heap memory to allocate.
 *
 * @return AG_ERNO_NULL if successful.
 * @return AGM_ERNO_MPOOL if unsuccessful.
 *
 * @note @p bfr is expected to be a valid pointer to an unallocated buffer, and
 * @p sz is expected to be greater than 0.
 *
 * @warning Don't attempt to release the memory allocated to @p bfr through the
 * standard @c free() function; the heap memory allocated to @p bfr is managed
 * by the Boehm Garbage Collector.
 *
 * @see agm_mpool_realloc()
 */
extern ag_hot ag_erno
agm_mpool_alloc(void **bfr, size_t sz);


/**
 * Reallocate heap memory from Boehm GC.
 *
 * The @c agm_mpool_realloc() function reallocates @p sz bytes of heap memory to
 * a buffer @p bfr. This buffer is expected to have been allocated earlier by a
 * call to @c agm_mpool_alloc(). The heap memory is reallocated and managed by
 * the Boehm Garbage Collector.
 *
 * @param bfr Buffer to reallocate heap memory.
 * @param sz Size in bytes of heap memory to reallocate.
 *
 * @return AG_ERNO_NULL if successful.
 * @return AGM_ERNO_MPOOL if unsuccessful.
 *
 * @note @p bfr is expected to be a valid pointer to an allocated buffer, and @p
 * sz is expected to be greater than 0.
 *
 * @warning Don't attempt to release the memory allocated to @p bfr through the
 * standard @c free() function; the heap memory allocated to @p bfr is managed
 * by the Boehm Garbage Collector.
 *
 * @see agm_mpool_alloc()
 */
extern ag_hot ag_erno
agm_mpool_realloc(void **bfr, size_t sz);


/**
 * @example mpool.h
 * This is an example showing how to code against the Argent Middleware Memory
 * Pool Module interface.
 * @}
 */


#endif /* !defined ARGENT_MIDDLEWARE */

