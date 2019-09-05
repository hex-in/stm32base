
#include <string.h>
#include <stdlib.h>

#include "hexin_assert.h"
#include "hexin_debug.h"
#include "hexin_defaults.h"
#include "hexin_utilities.h"
#include "hexin_freertos.h"
#include "hexin_result.h"
#include "hexin_rtos_common.h"
#include "platform_config.h"
#include "cmw_rtos_interface.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define CPU_CYCLES_PER_MICROSECOND    ( CPU_CLOCK_HZ / 1000000 )

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    event_handler_t function;
    void* arg;
} hexin_event_message_t;
/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

hexin_result_t hexin_rtos_init( void )
{
    hexin_result_t result = HEXIN_SUCCESS;

    return result;
}

hexin_result_t hexin_rtos_deinit( void )
{
    hexin_result_t result = HEXIN_SUCCESS;

    return result;
}


hexin_result_t hexin_rtos_delay_milliseconds( uint32_t milliseconds )
{
    return (hexin_result_t) host_rtos_delay_milliseconds( milliseconds );
}

hexin_result_t hexin_rtos_push_to_queue( hexin_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return (hexin_result_t) host_rtos_push_to_queue( HEXIN_GET_QUEUE_HANDLE( queue ), message, timeout_ms );
}

hexin_result_t hexin_rtos_pop_from_queue( hexin_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return (hexin_result_t) host_rtos_pop_from_queue( HEXIN_GET_QUEUE_HANDLE( queue ), message, timeout_ms );
}


static void worker_thread_main( hexin_thread_arg_t arg )
{
    hexin_worker_thread_t* worker_thread = (hexin_worker_thread_t*) arg;

    while ( 1 )
    {
        hexin_event_message_t message;

        if ( hexin_rtos_pop_from_queue( &worker_thread->event_queue, &message, HEXIN_WAIT_FOREVER ) == HEXIN_SUCCESS )
        {
            message.function( message.arg );
        }
    }
}

hexin_result_t hexin_rtos_create_worker_thread( hexin_worker_thread_t* worker_thread, uint8_t priority, uint32_t stack_size, uint32_t event_queue_size )
{
    memset( worker_thread, 0, sizeof( *worker_thread ) );

    if ( hexin_rtos_init_queue( &worker_thread->event_queue, "worker queue", sizeof(hexin_event_message_t), event_queue_size ) != HEXIN_SUCCESS )
    {
        return HEXIN_ERROR;
    }

    if ( hexin_rtos_create_thread( &worker_thread->thread, priority , "worker thread", worker_thread_main, stack_size, (void*) worker_thread ) != HEXIN_SUCCESS )
    {
        hexin_rtos_deinit_queue( &worker_thread->event_queue );
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}

hexin_result_t hexin_rtos_delete_worker_thread( hexin_worker_thread_t* worker_thread )
{
    if ( hexin_rtos_delete_thread( &worker_thread->thread ) != HEXIN_SUCCESS )
    {
        return HEXIN_ERROR;
    }

    if ( hexin_rtos_deinit_queue( &worker_thread->event_queue ) != HEXIN_SUCCESS )
    {
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}

hexin_result_t hexin_rtos_send_asynchronous_event( hexin_worker_thread_t* worker_thread, event_handler_t function, void* arg )
{
    hexin_event_message_t message;

    message.function = function;
    message.arg = arg;

    return hexin_rtos_push_to_queue( &worker_thread->event_queue, &message, HEXIN_NO_WAIT );
}

static void timed_event_handler( void* arg )
{
    hexin_timed_event_t* event_object = (hexin_timed_event_t*) arg;
    hexin_event_message_t message;

    message.function = event_object->function;
    message.arg = event_object->arg;

    hexin_rtos_push_to_queue( &event_object->thread->event_queue, &message, HEXIN_NO_WAIT );
}
