/** @file
 * Defines macros for defining asserts for ARM-Cortex-M4 CPU
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#if defined ( __GNUC__ )

#if defined ( __clang__ )

static inline /*@noreturn@*/void HEXIN_TRIGGER_BREAKPOINT( void ) __attribute__((analyzer_noreturn))
{
    __asm__("bkpt");
}

#else

#if defined ( LINT ) /* Lint requires a prototype */

extern /*@noreturn@*/ void HEXIN_TRIGGER_BREAKPOINT( void );

#else /* #if defined ( LINT ) */

#define HEXIN_TRIGGER_BREAKPOINT( ) do { __asm__("bkpt");  } while (0)

#endif /* #if defined ( LINT ) */

#endif /* #if defined ( __clang__ ) */

#define HEXIN_ASSERTION_FAIL_ACTION() HEXIN_TRIGGER_BREAKPOINT()

#define HEXIN_DISABLE_INTERRUPTS() do { __asm__("CPSID i"); } while (0)

#define HEXIN_ENABLE_INTERRUPTS() do { __asm__("CPSIE i"); } while (0)

#elif defined ( __IAR_SYSTEMS_ICC__ )

#define HEXIN_TRIGGER_BREAKPOINT() do { __asm("bkpt 0"); } while (0)

#define HEXIN_ASSERTION_FAIL_ACTION() HEXIN_TRIGGER_BREAKPOINT()

#define HEXIN_DISABLE_INTERRUPTS() do { __asm("CPSID i"); } while (0)

#define HEXIN_ENABLE_INTERRUPTS() do { __asm("CPSIE i"); } while (0)

#elif defined ( __CC_ARM )

#define HEXIN_TRIGGER_BREAKPOINT() __asm("bkpt 0")

#define HEXIN_ASSERTION_FAIL_ACTION() HEXIN_TRIGGER_BREAKPOINT()

#define HEXIN_DISABLE_INTERRUPTS() { __asm("CPSID i"); }

#define HEXIN_ENABLE_INTERRUPTS() { __asm("CPSIE i"); }

#endif

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /*extern "C" */
#endif
