#pragma once
#include "platform_constants.h"
#include "platform_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/* Section where IRQ handlers are placed */
#define IRQ_SECTION ".text.irq"

/* Macro for defining an interrupt handler (non-RTOS-aware)
 *
 * @warning:
 * Do NOT call any RTOS primitive functions from here. If you need to call them,
 * define your interrupt handler using WWD_RTOS_DEFINE_ISR()
 *
 * @usage:
 * PLATFORM_DEFINE_ISR( my_irq )
 * {
 *     // Do something here
 * }
 *
 */
#if defined ( __GNUC__ )
/* GCC */
#define PLATFORM_DEFINE_NAKED_ISR( function ) \
        void function( void ); \
        __attribute__(( naked, interrupt, used, section( IRQ_SECTION ) )) void function( void )

#define PLATFORM_DEFINE_ISR( name ) \
        void name( void ); \
        __attribute__(( interrupt, used, section( IRQ_SECTION ) )) void name( void )

#elif defined ( __IAR_SYSTEMS_ICC__ )
/* IAR Systems */
#define PLATFORM_DEFINE_NAKED_ISR( function ) \
        __root __stackless void function( void ); \
        __root __stackless void function( void )

#define PLATFORM_DEFINE_ISR( name ) \
        __root void name( void ); \
        __root void name( void )

#elif defined ( __CC_ARM )
/* Keil MDK Compiler */

#define PLATFORM_DEFINE_NAKED_ISR( function ) \
        void function( void ); \
        __attribute__(( naked, interrupt, used, section( IRQ_SECTION ) )) void function( void )

#define PLATFORM_DEFINE_ISR( name ) \
        void name( void ); \
        __attribute__(( interrupt, used, section( IRQ_SECTION ) )) void name( void )

#else

#define PLATFORM_DEFINE_ISR( name )

#endif


/* Macro for mapping a defined function to an interrupt handler declared in
 * /HEXIN/platform/<Arch>/<Family>/platform_interrupt_handlers.h
 *
 * @usage:
 * PLATFORM_MAP_ISR( my_irq, USART1_irq )
 */
#if defined( __GNUC__ )

#define PLATFORM_MAP_ISR( function, irq_handler ) \
        extern void irq_handler( void ); \
        __attribute__(( alias( #function ))) void irq_handler ( void );

#elif defined ( __IAR_SYSTEMS_ICC__ )

#define PLATFORM_MAP_ISR( function, irq_handler ) \
        extern void irq_handler( void ); \
        _Pragma( TO_STRING( weak irq_handler=function ) )

#elif defined ( __CC_ARM )
/* Keil MDK Compiler */
#define PLATFORM_MAP_ISR( function, irq_handler ) \
        extern void irq_handler( void ); \
        __attribute__(( alias( #function ))) void irq_handler ( void );

#else

#define PLATFORM_MAP_ISR( function, irq_handler )

#endif


/* Macro for declaring a default handler for an unhandled interrupt
 *
 * @usage:
 * PLATFORM_SET_DEFAULT_ISR( USART1_irq, default_handler )
 */
#if defined( __GNUC__ )

#define PLATFORM_SET_DEFAULT_ISR( irq_handler, default_handler ) \
        __attribute__(( weak, alias( #default_handler ))) void irq_handler ( void );

#elif defined ( __IAR_SYSTEMS_ICC__ )

#define PLATFORM_SET_DEFAULT_ISR( irq_handler, default_handler ) \
        _Pragma( TO_STRING( weak irq_handler=default_handler ) )

#elif defined ( __CC_ARM )
/* Keil MDK Compiler */
#define PLATFORM_SET_DEFAULT_ISR( irq_handler, default_handler ) \
        __attribute__(( weak, alias( #default_handler ))) void irq_handler ( void );

#else

#define PLATFORM_SET_DEFAULT_ISR( irq_handler, default_handler )

#endif


/* Macro for defining the interrupt vector table for ARM Cortex-M.
 *
 * @usage:
 * PLATFORM_DEFINE_INTERRUPT_VECTOR_TABLE_ARRAY( interrupt_vector_table, PLATFORM_INTERRUPT_VECTOR_TABLE_SIZE_VARIABLE )
 */
#if defined( __GNUC__ )

#define PLATFORM_DEFINE_INTERRUPT_VECTOR_TABLE_ARRAY(_NAME_, _SIZE_) \
        const uint32_t _NAME_[_SIZE_]

#elif defined ( __IAR_SYSTEMS_ICC__ )

#define PLATFORM_DEFINE_INTERRUPT_VECTOR_TABLE_ARRAY(_NAME_, _SIZE_) \
        const uint32_t _NAME_ [_SIZE_] @ ".intvec"

#elif defined ( __CC_ARM )

		/* 	See: projects\scatter\hexin.sct	*/
#define PLATFORM_DEFINE_INTERRUPT_VECTOR_TABLE_ARRAY(_NAME_, _SIZE_) \
        const uint32_t _NAME_[_SIZE_] __attribute__((section("interrupt_vector"), used))

#else

//#error "Unsupported toolchain!"

#endif

#define PLATFORM_INTERRUPT_VECTOR_TABLE_HAS_VARIABLE_SIZE

#if defined( __GNUC__ ) || defined ( __IAR_SYSTEMS_ICC__ ) || defined ( __CC_ARM )

#ifdef HEXIN_NO_VECTORS

#define HEXIN_SAVE_INTERRUPTS(flags) \
    do { INLINE_ASM volatile("" : : : "memory"); (void)flags; } while(0)

#define HEXIN_RESTORE_INTERRUPTS(flags) \
    do { INLINE_ASM volatile("" : : : "memory"); } while(0)

#else

#define HEXIN_SAVE_INTERRUPTS(flags) \
    do { INLINE_ASM volatile("mrs %0, primask\ncpsid i" : "=r" (flags) : : "memory", "cc"); } while(0)

#define HEXIN_RESTORE_INTERRUPTS(flags) \
    do { INLINE_ASM volatile("msr primask, %0" : : "r" (flags) : "memory", "cc"); } while(0)

#endif

#endif

/******************************************************
 *                    Constants
 ******************************************************/

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
