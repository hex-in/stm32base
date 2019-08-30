

#include <string.h>
#include "platform_init.h"
#include "platform_cmsis.h"
#include "platform_toolchain.h"

#define SCB_AIRCR_VECTKEY        ( (unsigned long)( 0x5FA << SCB_AIRCR_VECTKEY_Pos ))

extern uint32_t Image$$APP_CODE_FLASH$$RO$$Base;
extern uint32_t Image$$APP_CODE_FLASH$$RO$$Limit;

extern uint32_t Image$$SRAM$$ZI$$Base;
extern uint32_t Image$$SRAM$$ZI$$Limit;

extern uint32_t Image$$SRAM$$RW$$Base;
extern uint32_t Image$$SRAM$$RW$$Limit;

extern uint32_t Image$$STACK_SRAM$$Base;
extern uint32_t Image$$STACK_SRAM$$ZI$$Limit;

#define link_global_data_size   ((uint32_t)&Image$$SRAM$$RW$$Limit - (uint32_t)&Image$$SRAM$$RW$$Base)
#define link_bss_size           ((uint32_t)&Image$$SRAM$$ZI$$Limit - (uint32_t)&Image$$SRAM$$ZI$$Base )


//WEAK void _start( void ) __attribute__ (( naked )); /* Must be naked to avoid overwriting a program which is in RAM during function prologue when stack pointer is invalid */
WEAK void _exit( int status );

#define link_constructors_size   ((unsigned long)&link_constructors_end  -  (unsigned long)&link_constructors_location )

void reset_handler( void );

#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. This value must be a multiple of 0x200. */
void reset_handler( void )
{
	__set_MSP((uint32_t)&Image$$STACK_SRAM$$ZI$$Limit);
  
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif

	RCC->CR 	|= (uint32_t)0x00000001;
	RCC->CFGR 	 = (uint32_t)0x00000000;
	RCC->CR 	&= (uint32_t)0xFEF6FFFF;
	RCC->PLLCFGR = (uint32_t)0x24003010;
	RCC->CR 	&= (uint32_t)0xFFFBFFFF;
	RCC->CIR 	 = (uint32_t)0x00000000;

    /* Setup the interrupt vectors address */
#ifdef VECT_TAB_SRAM
	SCB->VTOR = SRAM_BASE  | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
	SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif

    /* Enable CPU Cycle counting */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; /* Global Enable for DWT */
    DWT->CYCCNT = 0;                                /* Reset the counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            /* Enable cycle counter */

    /* Initialise clocks and memory. init_clocks() and init_memory() must NOT depend on globals as global data and bss sections aren't initialised yet */
    platform_init_system_clocks();
    platform_init_memory();

    /* Copy initial values for global variables into RAM  */
	// if ( ( &Image$$SRAM$$RW$$Base != &Image$$APP_CODE_FLASH$$RO$$Limit ) && ( link_global_data_size != 0 ) )
	// {
	//      memcpy( &Image$$SRAM$$RW$$Base, &Image$$APP_CODE_FLASH$$RO$$Limit, (size_t) link_global_data_size );
	// }

    /* BSS segment is for zero initialised elements, so memset it to zero */
    // memset( &Image$$SRAM$$ZI$$Base, 0, (size_t) link_bss_size );

#if 0 /* was ifdef DEBUG */
    /* This is not a valid way to fill the stack, since it is currently in use - causes a problem in release with debug on - optimisation of active stack overwriting causes hardfault */
    memset( &link_stack_location, 0xA5, link_stack_size ); /* Fill stack with pattern to allow checking of stack usage */
#endif /* if 0 */

    /*
     * Run global C++ constructors if any
     */

    /* TODO: make this an unconditional goto?, so that return address stuff doesn't get put on the stack. (what happens if main returns in this case?) */
    platform_init_mcu_infrastructure( );
    platform_init_external_devices( );

    main( );

    /* the main loop has returned - there is now nothing to do - reboot. */

    /* Reset request */
    SCB->AIRCR = SCB_AIRCR_SYSRESETREQ_Msk | SCB_AIRCR_VECTKEY;

}

void _exit( int status )
{
    /* the main loop has returned - there is now nothing to do - reboot. */

    /* Allow some time for any printf calls to complete */
    volatile unsigned int i; /* try to make this not get optimized out by declaring the variable as volatile */
    volatile unsigned int j; /* try to make this not get optimized out by declaring the variable as volatile */

    (void) status; /* unused parameter */

    for ( i = 0; i < (unsigned int) 1000; i++ )
    {
        for ( j = 0; j < (unsigned int) 10000; j++ )
        {
            __asm("NOP");
        }
    }

    /* Reset request */
    SCB->AIRCR = SCB_AIRCR_SYSRESETREQ_Msk | SCB_AIRCR_VECTKEY;

    /* Should never get here, but this will get rid of error: "noreturn function does return" */
    while ( 1 == 1 )
    {
        /* do nothing */
    }
}

