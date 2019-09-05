
#include "FreeRTOS.h"
#include "task.h"
#include "hexin_defaults.h"
#include "hexin_assert.h"
#include "hexin_utilities.h"
#include "platform_init.h"
#include "hexin_rtos_common.h"
#include "hexin_freertos.h"
#include "cmw_rtos.h"
#include "cmw_rtos_interface.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
extern const uint32_t ms_to_tick_ratio;

#ifndef TIMER_THREAD_STACK_SIZE
#define TIMER_THREAD_STACK_SIZE      1024
#endif
#define TIMER_QUEUE_LENGTH  5

#define SYSTEM_MONITOR_THREAD_STACK_SIZE   512


/* FreeRTOS Event Group definition is based on
 *  configUSE_16_BIT_TICKS
 *
 * See <FreeRTOS>/Source/event_roups.c for detail.
 */
#if configUSE_16_BIT_TICKS == 1
    #define eventEVENT_BITS_MASK    0xffU           /* 8-bit */
#else
    #define eventEVENT_BITS_MASK    0xffffffUL      /* 24-bit */
#endif

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef TimerCallbackFunction_t native_timer_handler_t;
typedef TaskFunction_t          native_thread_t;

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    event_handler_t function;
    void*           arg;
} timer_queue_message_t;

typedef struct
{
    event_handler_t function;
    void*           arg;
} hexin_event_message_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/
static void application_thread_main( void *arg );
/******************************************************
 *               Variable Definitions
 ******************************************************/
static TaskHandle_t app_thread_handle;

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t  xIdleStack[configMINIMAL_STACK_SIZE];


//static StackType_t 	AppStartTackStack[configMINIMAL_STACK_SIZE];
//static StaticTask_t AppStartTaskTCB;

/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 *  Main function - starts ThreadX
 *  Called from the crt0_KEIL reset_handler function
 *
 */

int main( void )
{
	platform_init_system_clocks();
    platform_init_external_devices( );

#if( configSUPPORT_STATIC_ALLOCATION == 1 ) &&  ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
	xTaskCreate( application_thread_main,
				 "app_thread",
                 configMINIMAL_STACK_SIZE, NULL,
                 HEXIN_APPLICATION_PRIORITY,
                 &app_thread_handle );
#endif

    /* Start the FreeRTOS scheduler - this call should never return */
    vTaskStartScheduler( );
}

static void application_thread_main( void *arg )
{
    UNUSED_PARAMETER( arg );
    application_start( );

    malloc_leak_check(NULL, LEAK_CHECK_THREAD);
    vTaskDelete( NULL );
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
	static StaticTask_t xTimerTaskTCB;
	static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


hexin_result_t hexin_rtos_create_thread( hexin_thread_t* thread, uint8_t priority, const char* name, hexin_thread_function_t function, uint32_t stack_size, void* arg )
{
    /* Limit priority to default lib priority */
    if ( priority > RTOS_HIGHEST_PRIORITY ) {
        priority = RTOS_HIGHEST_PRIORITY;
    }

    return ( hexin_result_t )host_rtos_create_thread_with_arg( HEXIN_GET_THREAD_HANDLE( thread ), function, name, NULL, stack_size, priority, (uint32_t) arg );
}

hexin_result_t hexin_rtos_delete_thread(hexin_thread_t* thread)
{
    hexin_result_t result;

    malloc_leak_check( &thread, LEAK_CHECK_THREAD );

    result = (hexin_result_t)host_rtos_finish_thread( HEXIN_GET_THREAD_HANDLE( thread ) );

    if ( result != HEXIN_SUCCESS )
    {
        return result;
    }

    return (hexin_result_t)host_rtos_delete_terminated_thread( HEXIN_GET_THREAD_HANDLE( thread ) );
}

hexin_result_t hexin_rtos_init_mutex( hexin_mutex_t* mutex )
{
    hexin_assert("Bad args", mutex != NULL);

    /* Mutex uses priority inheritance */
    *mutex = xSemaphoreCreateRecursiveMutex( );
    if ( *mutex == NULL )
    {
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}

hexin_result_t hexin_rtos_lock_mutex( hexin_mutex_t* mutex )
{
    hexin_assert("Bad args", mutex != NULL);

    if ( xSemaphoreTakeRecursive( *mutex, HEXIN_WAIT_FOREVER ) != pdPASS )
    {
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_unlock_mutex( hexin_mutex_t* mutex )
{
    hexin_assert("Bad args", mutex != NULL);

    if ( xSemaphoreGiveRecursive( *mutex ) != pdPASS )
    {
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_deinit_mutex( hexin_mutex_t* mutex )
{
    hexin_assert("Bad args", mutex != NULL);

    vSemaphoreDelete( *mutex );
    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_init_queue( hexin_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages )
{
    UNUSED_PARAMETER( name );

    return (hexin_result_t)host_rtos_init_queue( HEXIN_GET_QUEUE_HANDLE( queue ), NULL, number_of_messages * message_size, message_size );
}

hexin_result_t hexin_rtos_deinit_queue( hexin_queue_t* queue )
{
    return (hexin_result_t)host_rtos_deinit_queue( HEXIN_GET_QUEUE_HANDLE( queue ) );
}

hexin_result_t hexin_rtos_is_queue_empty( hexin_queue_t* queue )
{
    signed portBASE_TYPE result;

    taskENTER_CRITICAL();
    result = xQueueIsQueueEmptyFromISR( *queue );
    taskEXIT_CRITICAL();

    return ( result != 0 ) ? HEXIN_SUCCESS : HEXIN_ERROR;
}

hexin_result_t hexin_rtos_is_queue_full( hexin_queue_t* queue )
{
    signed portBASE_TYPE result;

    taskENTER_CRITICAL();
    result = xQueueIsQueueFullFromISR( *queue );
    taskEXIT_CRITICAL();

    return ( result != 0 ) ? HEXIN_SUCCESS : HEXIN_ERROR;
}

static void timer_callback( TimerHandle_t handle )
{
    hexin_timer_t* timer = (hexin_timer_t*) pvTimerGetTimerID( handle );

    if ( timer->function )
    {
        timer->function( timer->arg );
    }
}

hexin_result_t hexin_rtos_init_timer( hexin_timer_t* timer, uint32_t time_ms, timer_handler_t function, void* arg )
{
    hexin_assert("Bad args", timer != NULL);

    timer->function = function;
    timer->arg      = arg;

    timer->handle = xTimerCreate(  (const char *)"", (TickType_t)( time_ms / ms_to_tick_ratio ), pdTRUE, timer, (native_timer_handler_t) timer_callback );
    if ( timer->handle == NULL )
    {
        return HEXIN_ERROR;
    }

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_start_timer( hexin_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return HEXIN_ERROR;
    }

    if (host_platform_is_in_interrupt_context() == HEXIN_TRUE)
    {   /* in Interrupt */
        signed portBASE_TYPE result;
        signed portBASE_TYPE xHigherPriorityTaskWoken;

        result = xTimerResetFromISR( timer->handle, &xHigherPriorityTaskWoken );
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
        if ( result != pdPASS )
        {
            return HEXIN_ERROR;
        }

        result = xTimerStartFromISR( timer->handle, &xHigherPriorityTaskWoken );
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
        if ( result != pdPASS )
        {
            return HEXIN_ERROR;
        }
    }
    else
    {   /* in Task */
        if ( xTimerReset( timer->handle, HEXIN_WAIT_FOREVER ) != pdPASS )
        {
            return HEXIN_ERROR;
        }

        if ( xTimerStart( timer->handle, HEXIN_WAIT_FOREVER ) != pdPASS )
        {
            return HEXIN_ERROR;
        }
    }

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_stop_timer( hexin_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return HEXIN_ERROR;
    }

    if (host_platform_is_in_interrupt_context() == HEXIN_TRUE)
    {   /* in Interrupt */
        signed portBASE_TYPE xHigherPriorityTaskWoken;

        if ( xTimerStopFromISR( timer->handle, &xHigherPriorityTaskWoken ) != pdPASS )
        {
            return HEXIN_ERROR;
        }
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
    else
    {   /* in Task */
        if ( xTimerStop( timer->handle, HEXIN_WAIT_FOREVER ) != pdPASS )
        {
            return HEXIN_ERROR;
        }
    }

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_deinit_timer( hexin_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return HEXIN_ERROR;
    }

    if ( xTimerDelete( timer->handle, HEXIN_WAIT_FOREVER ) != pdPASS )
    {
        return HEXIN_ERROR;
    }

    timer->handle = NULL;

    return HEXIN_SUCCESS;
}


hexin_result_t hexin_rtos_is_timer_running( hexin_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return HEXIN_ERROR;
    }

    return ( xTimerIsTimerActive( timer->handle ) != 0 ) ? HEXIN_SUCCESS : HEXIN_ERROR;
}


/* -------------------- Event Groups -------------------- */

hexin_result_t hexin_rtos_init_event_flags( hexin_event_flags_t* event_flags )
{
    *event_flags = xEventGroupCreateStatic( (StaticEventGroup_t *)pvPortMalloc( sizeof(StaticEventGroup_t) ) );

    return ((*event_flags == NULL) ? HEXIN_OUT_OF_HEAP_SPACE : HEXIN_SUCCESS);
}

hexin_result_t hexin_rtos_wait_for_event_flags( hexin_event_flags_t* event_flags, uint32_t flags_to_wait_for, uint32_t* flags_set, hexin_bool_t clear_set_flags, hexin_event_flags_wait_option_t wait_option, uint32_t timeout_ms )
{
    EventBits_t uxBitsToWaitFor = (flags_to_wait_for & eventEVENT_BITS_MASK);
    EventBits_t uxBits;

    if (*event_flags == NULL)
    {
        return HEXIN_ERROR;
    }

    /*
     * CAUTION: depending on assert (not error return)
     *          in hexin_rtos_set_event_flags
     * No error checking here as hexin_rtos_set_event_flags
     * should have asserted if it is out of supported range
     */

    uxBits = xEventGroupWaitBits(
        *event_flags,                               /* xEventGroup */
        uxBitsToWaitFor,                            /* uxBitsToWaitFor */
        ((clear_set_flags == HEXIN_TRUE) ? pdTRUE : pdFALSE),       /* xClearOnExit */
        ((wait_option == WAIT_FOR_ALL_EVENTS) ? pdTRUE : pdFALSE),  /* xWaitForAllBits */
        (timeout_ms / ms_to_tick_ratio)             /* xTicksToWait */
    );

    /* Timeout (nothing detected) */
    if ((uxBits & uxBitsToWaitFor) == 0)
        return HEXIN_TIMEOUT;

    *flags_set = uxBits;
    return HEXIN_SUCCESS;
}

hexin_result_t hexin_rtos_set_event_flags( hexin_event_flags_t* event_flags, uint32_t flags_to_set )
{
    EventBits_t uxBitsToSet = (flags_to_set & eventEVENT_BITS_MASK);

    /* Out of EventBits range supported by FreeRTOS */
    hexin_assert("Out of supported range", uxBitsToSet == flags_to_set);

    if (*event_flags == NULL)
    {
        return HEXIN_ERROR;
    }

    xEventGroupSetBits( *event_flags, uxBitsToSet );
    /* Ignore the return result (EventBits_t: all active bits) */

    /* WARNING: Do NOT compare return value with uxBitsToSet
     * for SUCCESS/FAIL.
     * The setting bit might had been cleared when this call returns
     */
    return HEXIN_SUCCESS;
}

hexin_result_t hexin_rtos_deinit_event_flags( hexin_event_flags_t* event_flags )
{
    if (*event_flags == NULL)
    {
        return HEXIN_ERROR;
    }

    vEventGroupDelete( *event_flags );  /* No return value */

    vPortFree( *event_flags );

    *event_flags = NULL;

    return HEXIN_SUCCESS;
}
