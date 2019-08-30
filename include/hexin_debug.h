
#ifndef INCLUDED_HEXIN_DEBUG_H
#define INCLUDED_HEXIN_DEBUG_H

#include <stdio.h>
#include "hexin_defaults.h"

#ifdef PLATFORM_TRACE
#include "platform_trace.h"
#endif
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

#ifdef DEBUG
#include "platform_assert.h"
    #define HEXIN_BREAK_IF_DEBUG( ) HEXIN_ASSERTION_FAIL_ACTION()
#else
    #define HEXIN_BREAK_IF_DEBUG( )
#endif

#ifdef WPRINT_PLATFORM_PERMISSION
int platform_wprint_permission(void);
#define WPRINT_PLATFORM_PERMISSION_FUNC() platform_wprint_permission()
#else
#define WPRINT_PLATFORM_PERMISSION_FUNC() 1
#endif

/******************************************************
 *             Print declarations
 ******************************************************/

#define WPRINT_MACRO(args) do {if (WPRINT_PLATFORM_PERMISSION_FUNC()) printf args;} while(0==1)

/* HEXIN printing macros for general SDK/Library functions*/
#ifdef WPRINT_ENABLE_LIB_INFO
    #define WPRINT_LIB_INFO(args) WPRINT_MACRO(args)
#else
    #define WPRINT_LIB_INFO(args)
#endif
#ifdef WPRINT_ENABLE_LIB_DEBUG
    #define WPRINT_LIB_DEBUG(args) WPRINT_MACRO(args)
#else
    #define WPRINT_LIB_DEBUG(args)
#endif
#ifdef WPRINT_ENABLE_LIB_ERROR
    #define WPRINT_LIB_ERROR(args) { WPRINT_MACRO(args); HEXIN_BREAK_IF_DEBUG(); }
#else
    #define WPRINT_LIB_ERROR(args) { HEXIN_BREAK_IF_DEBUG(); }
#endif

/* HEXIN printing macros for Applications*/
#ifdef WPRINT_ENABLE_APP_INFO
    #define WPRINT_APP_INFO(args) WPRINT_MACRO(args)
#else
    #define WPRINT_APP_INFO(args)
#endif
#ifdef WPRINT_ENABLE_APP_DEBUG
    #define WPRINT_APP_DEBUG(args) WPRINT_MACRO(args)
#else
    #define WPRINT_APP_DEBUG(args)
#endif
#ifdef WPRINT_ENABLE_APP_ERROR
    #define WPRINT_APP_ERROR(args) { WPRINT_MACRO(args); HEXIN_BREAK_IF_DEBUG(); }
#else
    #define WPRINT_APP_ERROR(args) { HEXIN_BREAK_IF_DEBUG(); }
#endif

/* HEXIN printing macros for the RTOS*/
#ifdef WPRINT_ENABLE_RTOS_INFO
    #define WPRINT_RTOS_INFO(args) WPRINT_MACRO(args)
#else
    #define WPRINT_RTOS_INFO(args)
#endif
#ifdef WPRINT_ENABLE_RTOS_DEBUG
    #define WPRINT_RTOS_DEBUG(args) WPRINT_MACRO(args)
#else
    #define WPRINT_RTOS_DEBUG(args)
#endif
#ifdef WPRINT_ENABLE_RTOS_ERROR
    #define WPRINT_RTOS_ERROR(args) { WPRINT_MACRO(args); HEXIN_BREAK_IF_DEBUG(); }
#else
    #define WPRINT_RTOS_ERROR(args) { HEXIN_BREAK_IF_DEBUG(); }
#endif

/* HEXIN printing macros for Platforms*/
#ifdef WPRINT_ENABLE_PLATFORM_INFO
    #define WPRINT_PLATFORM_INFO(args) WPRINT_MACRO(args)
#else
    #define WPRINT_PLATFORM_INFO(args)
#endif
#ifdef WPRINT_ENABLE_PLATFORM_DEBUG
    #define WPRINT_PLATFORM_DEBUG(args) WPRINT_MACRO(args)
#else
    #define WPRINT_PLATFORM_DEBUG(args)
#endif
#ifdef WPRINT_ENABLE_PLATFORM_ERROR
    #define WPRINT_PLATFORM_ERROR(args) { WPRINT_MACRO(args); HEXIN_BREAK_IF_DEBUG(); }
#else
    #define WPRINT_PLATFORM_ERROR(args) { HEXIN_BREAK_IF_DEBUG(); }
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_HEXIN_DEBUG_H */
