#ifndef INCLUDED_CMW_RTOS_INTERFACE_H_
#define INCLUDED_CMW_RTOS_INTERFACE_H_

#include <stdint.h>
#include "hexin_constants.h"
#include "cmw_rtos.h"
#include "cmw_constants.h"

#ifndef RTOS_HIGHER_PRIORTIY_THAN
#error cmw_rtos.h must define RTOS_HIGHER_PRIORTIY_THAN(x)
#endif
#ifndef RTOS_LOWER_PRIORTIY_THAN
#error cmw_rtos.h must define RTOS_LOWER_PRIORTIY_THAN(x)
#endif
#ifndef RTOS_LOWEST_PRIORITY
#error cmw_rtos.h must define RTOS_LOWEST_PRIORITY
#endif
#ifndef RTOS_HIGHEST_PRIORITY
#error cmw_rtos.h must define RTOS_HIGHEST_PRIORITY
#endif
#ifndef RTOS_DEFAULT_THREAD_PRIORITY
#error cmw_rtos.h must define RTOS_DEFAULT_THREAD_PRIORITY
#endif
#if !(defined(RTOS_USE_DYNAMIC_THREAD_STACK) || defined(RTOS_USE_STATIC_THREAD_STACK))
#error cmw_rtos.h must define either RTOS_USE_DYNAMIC_THREAD_STACK or RTOS_USE_STATIC_THREAD_STACK
#endif

#ifdef __x86_64__
typedef uint64_t cmw_thread_arg_t;
#else
typedef uint32_t cmw_thread_arg_t;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 * @cond       Constants
 ******************************************************/

#define NEVER_TIMEOUT ((uint32_t)0xffffffffUL)  /* Used with host_rtos_get_semaphore */

/******************************************************
 *             Structures & Typedefs
 ******************************************************/

/** @endcond */


/** @addtogroup rtosif RTOS Interface
 * Allows HEXIN to use use RTOS resources
 *  such as threads, semaphores & timing functions in an abstract way.
 *  @{
 */

/******************************************************
 *             Function declarations
 ******************************************************/

/* Thread management functions */

/**
 * Create a thread
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to create a new thread and start it running.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 *
 * @return CMW_SUCCESS or Error code
 */
extern cmw_result_t host_rtos_create_thread( /*@out@*/ host_thread_type_t* thread, void(*entry_function)( cmw_thread_arg_t arg ), const char* name, /*@null@*/ void* stack, uint32_t stack_size, uint32_t priority ) /*@modifies *thread@*/;


/**
 * Create a thread with specific thread argument
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 * @param arg            : the argument to pass to the new thread
 *
 * @return CMW result code
 */
extern cmw_result_t host_rtos_create_thread_with_arg( /*@out@*/ host_thread_type_t* thread, void(*entry_function)( cmw_thread_arg_t arg ), const char* name, /*@null@*/ void* stack, uint32_t stack_size, uint32_t priority, cmw_thread_arg_t arg );


/**
 * Note: different RTOS have different parameters for creating threads.
 * Use this function carefully if portability is important.
 * Create a thread with RTOS specific thread argument (E.g. specify time-slicing behavior)
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : pointer to a variable which will receive the new thread handle
 * @param entry_function : function pointer which points to the main function for the new thread
 * @param name           : a string thread name used for a debugger
 * @param stack_size     : the size of the thread stack in bytes
 * @param priority       : the priority of the thread
 * @param arg            : the argument to pass to the new thread
 * @param config        : os specific thread configuration
 * @return CMW result code
 */
extern cmw_result_t host_rtos_create_configed_thread(  /*@out@*/ host_thread_type_t* thread, void(*entry_function)( uint32_t ), const char* name, /*@null@*/ void* stack, uint32_t stack_size, uint32_t priority, host_rtos_thread_config_type_t *config );


/**
 * Exit a thread
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to exit the current thread just before its main
 * function would otherwise return. Some RTOSs allow threads to exit by simply returning
 * from their main function. If this is the case, then the implementation of
 * this function should be empty.
 *
 * @param thread         : Pointer to the current thread handle
 *
 * @return CMW_SUCCESS or Error code
 */
extern cmw_result_t host_rtos_finish_thread( host_thread_type_t* thread ) /*@modifies *thread@*/;


/**
 * Deletes a terminated thread
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * Some RTOS implementations require that another thread deletes any terminated thread
 * If RTOS does not require this, leave empty
 *
 * @param thread         : handle of the terminated thread to delete
 *
 * @returns CMW_SUCCESS on success, Error code otherwise
 */
extern cmw_result_t host_rtos_delete_terminated_thread( host_thread_type_t* thread );


/**
 * Waits for a thread to complete
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 *
 * @param thread         : handle of the thread to wait for
 *
 * @returns CMW_SUCCESS on success, Error code otherwise
 */
extern cmw_result_t host_rtos_join_thread( host_thread_type_t* thread );

/* Semaphore management functions */

/**
 * Create a Semaphore
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to create a semaphore
 *
 * @param semaphore         : Pointer to the semaphore handle to be initialized
 *
 * @return CMW_SUCCESS or Error code
 */
extern cmw_result_t host_rtos_init_semaphore(  /*@special@*/ /*@out@*/ host_semaphore_type_t* semaphore ) /*@allocates *semaphore@*/  /*@defines **semaphore@*/;

/**
 * Get a semaphore
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to get a semaphore.
 * - If the semaphore value is greater than zero, the sempahore value is decremented and the function returns immediately.
 * - If the semaphore value is zero, the current thread is put on a queue of threads waiting on the
 *   semaphore, and sleeps until another thread sets the semaphore and causes it to wake. Upon waking, the
 *   semaphore is decremented and the function returns.
 *
 * @note : This function must not be called from an interrupt context as it may block.
 *
 * @param semaphore   : Pointer to the semaphore handle
 * @param timeout_ms  : Maximum number of milliseconds to wait while attempting to get
 *                      the semaphore. Use the NEVER_TIMEOUT constant to wait forever.
 * @param will_set_in_isr : True if the semaphore will be set in an ISR. Currently only used for NoOS/NoNS
 *
 * @return cmw_result_t : CMW_SUCCESS if semaphore was successfully acquired
 *                     : HEXIN_TIMEOUT if semaphore was not acquired before timeout_ms period
 */
extern cmw_result_t host_rtos_get_semaphore( host_semaphore_type_t* semaphore, uint32_t timeout_ms, hexin_bool_t will_set_in_isr ) /*@modifies internalState@*/;

/**
 * Set a semaphore
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to set a semaphore.
 * The value of the semaphore is incremented, and if there are any threads waiting on the semaphore,
 * then the first waiting thread is woken.
 *
 * Some RTOS implementations require different processing when setting a semaphore from within an ISR.
 * A parameter is provided to allow this.
 *
 * @param semaphore       : Pointer to the semaphore handle
 * @param called_from_ISR : Value of HEXIN_TRUE indicates calling from interrupt context
 *                          Value of HEXIN_FALSE indicates calling from normal thread context
 *
 * @return cmw_result_t : CMW_SUCCESS if semaphore was successfully set
 *                        : Error code if an error occurred
 *
 */
extern cmw_result_t host_rtos_set_semaphore( host_semaphore_type_t* semaphore, hexin_bool_t called_from_ISR );

/**
 * Deletes a semaphore
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to delete a semaphore.
 *
 * @param semaphore         : Pointer to the semaphore handle
 *
 * @return cmw_result_t : CMW_SUCCESS if semaphore was successfully deleted
 *                        : Error code if an error occurred
 */
extern cmw_result_t host_rtos_deinit_semaphore( /*@special@*/ host_semaphore_type_t* semaphore ) /*@releases *semaphore@*/;


/* Time management functions */

/**
 * Gets time in milliseconds since RTOS start
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to retrieve the current time.
 *
 * @note: Since this is only 32 bits, it will roll over every 49 days, 17 hours, 2 mins, 47.296 seconds
 *
 * @returns Time in milliseconds since the RTOS started.
 */
extern cmw_time_t host_rtos_get_time( void )  /*@modifies internalState@*/;

/**
 * Delay for a number of milliseconds
 *
 * Implemented in the HEXIN RTOS interface which is specific to the
 * RTOS in use.
 * HEXIN uses this function to delay processing
 * Processing of this function depends on the minimum sleep
 * time resolution of the RTOS.
 * The current thread should sleep for the longest period possible which
 * is less than the delay required, then makes up the difference
 * with a tight loop
 *
 * @return cmw_result_t : CMW_SUCCESS if delay was successful
 *                        : Error code if an error occurred
 *
 */
extern cmw_result_t host_rtos_delay_milliseconds( uint32_t num_ms );


/* Message queue management functions */

extern cmw_result_t host_rtos_init_queue( /*@special@*/ /*@out@*/ host_queue_type_t* queue, void* buffer, uint32_t buffer_size, uint32_t message_size ) /*@allocates *queue@*/  /*@defines **queue@*/;

extern cmw_result_t host_rtos_push_to_queue( host_queue_type_t* queue, void* message, uint32_t timeout_ms );

extern cmw_result_t host_rtos_pop_from_queue( host_queue_type_t* queue, void* message, uint32_t timeout_ms );

extern cmw_result_t host_rtos_deinit_queue( /*@special@*/host_queue_type_t* queue ) /*@releases *queue@*/;


unsigned long host_rtos_get_tickrate( void );

/* Support timers for CMW */
typedef void (host_rtos_timer_handler_t)( void * );
typedef int (host_rtos_init_timer_t)( void* timer, uint32_t time_ms, host_rtos_timer_handler_t function, void* arg );
typedef int (host_rtos_start_timer_t)( void* timer );
typedef int (host_rtos_stop_timer_t)( void* timer );
typedef int (host_rtos_deinit_timer_t)( void* timer );
typedef int (host_rtos_is_timer_running_t)( void* timer );
typedef int (host_rtos_alloc_timer_t)( void** timer );
typedef int (host_rtos_free_timer_t)( void** timer );

typedef struct
{
    /* order of operations should be: first allocate, then init, then start/stop/is_running any number of times, then deinit, then free */
    host_rtos_alloc_timer_t      *host_rtos_alloc_timer;

    /* These functions behave identically to the hexin_rtos.h functions */
    host_rtos_init_timer_t       *host_rtos_init_timer;

    host_rtos_start_timer_t      *host_rtos_start_timer;
    host_rtos_stop_timer_t       *host_rtos_stop_timer;
    host_rtos_is_timer_running_t *host_rtos_is_timer_running;

    host_rtos_deinit_timer_t     *host_rtos_deinit_timer;

    /* call after successful deinit */
    host_rtos_free_timer_t       *host_rtos_free_timer;
} host_rtos_timer_ifc_t;

extern const host_rtos_timer_ifc_t *host_rtos_timer_ifc;

/**
 * Register a timer interface for CMW to use.
 *
 * @return cmw_result_t : CMW_SUCCESS if successful
 *                        : Error code if an error occurred
 *
 */
extern cmw_result_t host_rtos_register_timer_ifc( const host_rtos_timer_ifc_t *host_rtos_timer_ifc );

/**
 * Unregister a timer interface CMW was using.
 *
 * @return cmw_result_t : CMW_SUCCESS if successful
 *                        : Error code if an error occurred
 *
 */
extern cmw_result_t host_rtos_unregister_timer_ifc( const host_rtos_timer_ifc_t *host_rtos_timer_ifc );

#define HOST_RTOS_TIMER_IFC host_rtos_timer_ifc
#define HOST_RTOS_TIMER_IFC_CALL(func) ( *(HOST_RTOS_TIMER_IFC->func) )

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_CMW_RTOS_INTERFACE_H_ */
