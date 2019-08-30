#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define WPRINT_ENABLE_APP_INFO           /* Application prints */
//#define WPRINT_ENABLE_APP_DEBUG
//#define WPRINT_ENABLE_APP_ERROR
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define WPRINT_ENABLE_LIB_INFO           /* General library prints */
//#define WPRINT_ENABLE_LIB_DEBUG
//#define WPRINT_ENABLE_LIB_ERROR
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define WPRINT_ENABLE_RTOS_INFO          /* RTOS prints */
//#define WPRINT_ENABLE_RTOS_DEBUG
//#define WPRINT_ENABLE_RTOS_ERROR
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define WPRINT_ENABLE_PLATFORM_INFO      /* Platform prints */
//#define WPRINT_ENABLE_PLATFORM_DEBUG
#define WPRINT_ENABLE_PLATFORM_ERROR


/************************************************************************/

#define HEXIN_DEFAULT_WORKER_PRIORITY           (5)
#define HEXIN_DEFAULT_LIBRARY_PRIORITY          (5)
#define HEXIN_APPLICATION_PRIORITY              (7)

/************************************************************************
 * Application thread stack size */
#define HEXIN_DEFAULT_APPLICATION_STACK_SIZE    (6144)

/************************************************************************
 * Uncomment to disable watchdog. For debugging only */
#define HEXIN_DISABLE_WATCHDOG

/************************************************************************
 * Uncomment to disable standard IO, i.e. printf(), etc. */
#define HEXIN_DISABLE_STDIO

/************************************************************************
 * Uncomment to disable MCU powersave API functions */
#define HEXIN_DISABLE_MCU_POWERSAVE

/************************************************************************
 * Uncomment to enable MCU real time clock */
//#define HEXIN_ENABLE_MCU_RTC


#ifdef __cplusplus
} /*extern "C" */
#endif
