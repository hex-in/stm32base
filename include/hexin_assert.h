#pragma once

#include "hexin_debug.h"
#include "platform_assert.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 * @cond       Macros
 ******************************************************/

#ifdef DEBUG
    #ifdef WPRINT_ENABLE_ERROR
        #define WPRINT_ERROR(args)                      do { WPRINT_MACRO(args); HEXIN_ASSERTION_FAIL_ACTION(); } while(0)
        #define hexin_assert( error_string, assertion ) do { if (!(assertion)) { HEXIN_ASSERTION_FAIL_ACTION(); } } while(0)
        #define hexin_minor_assert( error_string, assertion )   do { if ( !(assertion) ) WPRINT_MACRO( ( error_string ) ); } while(0)
    #else
        #define WPRINT_ERROR(args)                      do { HEXIN_ASSERTION_FAIL_ACTION();} while(0)
        #define hexin_assert( error_string, assertion ) do { if (!(assertion)) { HEXIN_ASSERTION_FAIL_ACTION();} } while(0)
        #define hexin_minor_assert( error_string, assertion )   do { (void)(assertion); } while(0)
    #endif
#else
    #define hexin_assert( error_string, assertion )         do { (void)(assertion); } while(0)
    #define hexin_minor_assert( error_string, assertion )   do { (void)(assertion); } while(0)
#endif

#ifdef __GNUC__
#define HEXIN_UNUSED_VAR __attribute__ ((unused))
#elif defined ( __CC_ARM )
/* Keil MDK Compiler */
#define HEXIN_UNUSED_VAR __attribute__ ((unused))
#else
#define HEXIN_UNUSED_VAR
#endif

#define hexin_static_assert( descr, expr ) \
{ \
    /* Make sure the expression is constant. */ \
    typedef enum { _STATIC_ASSERT_NOT_CONSTANT = (expr) } _static_assert_e HEXIN_UNUSED_VAR; \
    /* Make sure the expression is true. */ \
    typedef char STATIC_ASSERT_FAIL_##descr[(expr) ? 1 : -1] HEXIN_UNUSED_VAR; \
}

/** @endcond */

#ifdef __cplusplus
} /* extern "C" */
#endif
