#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include "hexin_result.h"
#include "cmw_rtos.h"
#include "cmw_rtos_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#define LOWER_THAN_PRIORITY_OF( thread )            ((thread).handle.tx_thread_priority - 1)
#define HIGHER_THAN_PRIORITY_OF( thread )           ((thread).handle.tx_thread_priority + 1)

#define HEXIN_PRIORITY_TO_NATIVE_PRIORITY(priority) (uint8_t)(RTOS_HIGHEST_PRIORITY - priority)

#define HEXIN_END_OF_THREAD( thread )               malloc_leak_check(thread, LEAK_CHECK_THREAD); vTaskDelete(thread)
#define HEXIN_END_OF_CURRENT_THREAD( )              malloc_leak_check(NULL, LEAK_CHECK_THREAD); vTaskDelete(NULL)
#define HEXIN_END_OF_CURRENT_THREAD_NO_LEAK_CHECK( )   vTaskDelete(NULL)

#define HEXIN_TO_MALLOC_THREAD( x )                 ((malloc_thread_handle) *(x) )

#define HEXIN_GET_THREAD_HANDLE( thread )           ( thread )

#define HEXIN_GET_QUEUE_HANDLE( queue )             ( queue )

/******************************************************
 *                    Constants
 ******************************************************/

#define RTOS_NAME                     "FreeRTOS"
#define RTOS_VERSION                  "10.0.1"


/******************************************************
 *                   Enumerations
 ******************************************************/
typedef enum
{
    WAIT_FOR_ANY_EVENT,
    WAIT_FOR_ALL_EVENTS,
} hexin_event_flags_wait_option_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef cmw_thread_arg_t hexin_thread_arg_t;
typedef void (*hexin_thread_function_t)( hexin_thread_arg_t arg );

/******************************************************
 *                    Structures
 ******************************************************/

typedef EventGroupHandle_t hexin_event_flags_t;
typedef host_semaphore_type_t hexin_semaphore_t;

typedef SemaphoreHandle_t hexin_mutex_t;

typedef void (*timer_handler_t)( void* arg );

typedef struct
{
    TimerHandle_t    handle;
    timer_handler_t function;
    void*           arg;
}hexin_timer_t;

typedef TaskHandle_t hexin_thread_t;

typedef QueueHandle_t hexin_queue_t;

typedef struct
{
    hexin_thread_t thread;
    hexin_queue_t  event_queue;
} hexin_worker_thread_t;

typedef hexin_result_t (*event_handler_t)( void* arg );

typedef struct
{
    event_handler_t        function;
    void*                  arg;
    hexin_timer_t          timer;
    hexin_worker_thread_t* thread;
} hexin_timed_event_t;

/******************************************************
 *                 Global Variables
 ******************************************************/


/******************************************************
 *               Function Declarations
 ******************************************************/

/** Creates and starts a new thread with given priority, name and stack size.
 *
 * @param[out] thread     : Pointer to variable that will receive the thread handle
 * @param[in]  priority   : A priority number or HEXIN_APPLICATION_PRIORITY.
 *
 * @param[in]  name       : A text name for the thread (can be null)
 * @param[in]  function   : The main thread function
 * @param[in]  stack_size : Stack size for this thread
 * @param[in]  arg        : Argument which will be passed to thread function
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_create_thread( hexin_thread_t* thread, uint8_t priority, const char* name, hexin_thread_function_t function, uint32_t stack_size, void* arg );

/** Deletes a terminated thread
 *
 * @param[in] thread     : The handle of the thread to delete
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_delete_thread( hexin_thread_t* thread );

/** Sleep for a given period of milliseconds
 *
 * Causes the current thread to sleep for AT LEAST the
 * specified number of milliseconds. If the processor is heavily loaded
 * with higher priority tasks, the delay may be much longer than requested.
 *
 * @param[in] milliseconds : The time to sleep in milliseconds
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_delay_milliseconds( uint32_t milliseconds );

/** Initializes a RTOS timer
 *
 * Timer does not start running until @ref hexin_rtos_start_timer is called
 *
 * @param[in] timer    : A pointer to the timer handle to be initialized
 * @param[in] time_ms  : Timer period in milliseconds
 * @param[in] function : The callback handler function that is called each
 *                       time the timer expires
 * @param[in] arg      : An argument that will be passed to the callback
 *                       function
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_init_timer( hexin_timer_t* timer, uint32_t time_ms, timer_handler_t function, void* arg );

/** Starts a RTOS timer
 *
 * Timer must have been previously
 * initialized with @ref hexin_rtos_init_timer
 *
 * @param[in] timer    : A pointer to the timer handle to start
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_start_timer( hexin_timer_t* timer );

/** Stops a running RTOS timer
 *
 * Timer must have been previously
 * started with @ref hexin_rtos_start_timer
 *
 * @param[in] timer    : A pointer to the timer handle to stop
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_stop_timer( hexin_timer_t* timer );

/** De-initialize a RTOS timer
 *
 * Deletes a RTOS timer created with @ref hexin_rtos_init_timer
 *
 * @param[in] timer : A pointer to the RTOS timer handle
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_deinit_timer( hexin_timer_t* timer );

/** Sends an asynchronous event to the associated worker thread
 *
 *
 * @param[in] worker_thread : The worker thread in which context the callback should execute from
 * @param[in] function      : The callback function to be called from the worker thread
 * @param[in] arg           : The argument to be passed to the callback function
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_send_asynchronous_event( hexin_worker_thread_t* worker_thread, event_handler_t function, void* arg );

/*****************************************************************************/

/** Creates a worker thread
 *
 * A worker thread is a thread in whose context timed and asynchronous events
 * execute.
 *
 * @param[in] worker_thread    : A pointer to the worker thread to be created
 * @param[in] priority         : Thread priority
 * @param[in] stack_size       : Thread's stack size in number of bytes
 * @param[in] event_queue_size : Number of events can be pushed into the queue
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_create_worker_thread( hexin_worker_thread_t* worker_thread, uint8_t priority, uint32_t stack_size, uint32_t event_queue_size );

/** Deletes a worker thread
 *
 *
 * @param[in] worker_thread : A pointer to the worker thread to be created
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_delete_worker_thread( hexin_worker_thread_t* worker_thread );

/** Initializes a FIFO queue
 *
 *
 * @param[in] queue              : A pointer to the queue handle to be initialized
 * @param[in] name               : A text string name for the queue (NULL is allowed)
 * @param[in] message_size       : Size in bytes of objects that will be held in the queue
 * @param[in] number_of_messages : Depth of the queue - i.e. max number of objects in the queue
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_init_queue( hexin_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages );

/** Pushes an object onto a queue
 *
 *
 * @param[in] queue      : A pointer to the queue handle
 * @param[in] message    : The object to be added to the queue. Size is assumed to be
 *                         the size specified in @ref hexin_rtos_init_queue
 * @param[in] timeout_ms : The number of milliseconds to wait before returning
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error or timeout occurred
 */
hexin_result_t hexin_rtos_push_to_queue( hexin_queue_t* queue, void* message, uint32_t timeout_ms );


/** Pops an object off a queue
 *
 * Pops an object off a queue
 *
 * @param[in] queue      : A pointer to the queue handle
 * @param[out] message   : Pointer to a buffer that will receive the object being
 *                         popped off the queue. Size is assumed to be
 *                         the size specified in @ref hexin_rtos_init_queue, hence
 *                         you must ensure the buffer is long enough or memory
 *                         corruption will result
 * @param[in] timeout_ms : The number of milliseconds to wait before returning
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error or timeout occurred
 */
hexin_result_t hexin_rtos_pop_from_queue( hexin_queue_t* queue, void* message, uint32_t timeout_ms );

/** De-initialize a queue
 *
 * Deletes a queue created with @ref hexin_rtos_init_queue
 *
 * @param[in] queue : A pointer to the queue handle
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
hexin_result_t hexin_rtos_deinit_queue( hexin_queue_t* queue );




#ifdef __cplusplus
} /* extern "C" */
#endif
