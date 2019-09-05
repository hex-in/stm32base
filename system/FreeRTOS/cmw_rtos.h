
#ifndef INCLUDED_CMW_RTOS_H_
#define INCLUDED_CMW_RTOS_H_

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "hexin_constants.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define RTOS_HIGHER_PRIORTIY_THAN(x)     (x < RTOS_HIGHEST_PRIORITY ? x+1 : RTOS_HIGHEST_PRIORITY)
#define RTOS_LOWER_PRIORTIY_THAN(x)      (x > RTOS_LOWEST_PRIORITY ? x-1 : RTOS_LOWEST_PRIORITY)
#define RTOS_LOWEST_PRIORITY             (0)
#define RTOS_HIGHEST_PRIORITY            (configMAX_PRIORITIES-1)
#define RTOS_DEFAULT_THREAD_PRIORITY     (1)

#define RTOS_USE_DYNAMIC_THREAD_STACK

#ifndef CMW_LOGGING_STDOUT_ENABLE
#ifdef DEBUG
#define CMW_THREAD_STACK_SIZE        (1600 + 1400)   /* Stack checking requires a larger stack */
#else /* ifdef DEBUG */
#define CMW_THREAD_STACK_SIZE        (1400 + 1400)
#endif /* ifdef DEBUG */
#else /* if CMW_LOGGING_STDOUT_ENABLE */
#define CMW_THREAD_STACK_SIZE        (1400 + 4096 + 1400) /* CMW_LOG uses printf and requires a minimum of 4K stack */
#endif /* CMW_LOGGING_STDOUT_ENABLE */

/******************************************************
 *             Structures
 ******************************************************/

typedef SemaphoreHandle_t   /*@abstract@*/ /*@only@*/ host_semaphore_type_t;  /** FreeRTOS definition of a semaphore */
typedef TaskHandle_t        /*@abstract@*/ /*@only@*/ host_thread_type_t;     /** FreeRTOS definition of a thread handle */
typedef QueueHandle_t       /*@abstract@*/ /*@only@*/ host_queue_type_t;      /** FreeRTOS definition of a message queue */

/*@external@*/ extern void vApplicationMallocFailedHook( void );
/*@external@*/ extern void vApplicationIdleSleepHook( void );
/*@external@*/ extern hexin_bool_t host_platform_is_in_interrupt_context( void );
typedef struct
{
    uint8_t info;    /* not supported yet */
} host_rtos_thread_config_type_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_CMW_RTOS_H_ */
