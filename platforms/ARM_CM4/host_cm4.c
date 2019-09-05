#include "stm32f4xx.h"
#include "hexin_constants.h"

uint32_t host_platform_get_cycle_count( void )
{
    /* From the ARM Cortex-M3 Techinical Reference Manual
     * 0xE0001004  DWT_CYCCNT  RW  0x00000000  Cycle Count Register */
    return DWT->CYCCNT;
}

hexin_bool_t host_platform_is_in_interrupt_context( void )
{
    /* From the ARM Cortex-M3 Techinical Reference Manual
     * 0xE000ED04   ICSR    RW [a]  Privileged  0x00000000  Interrupt Control and State Register */
    return ( ( SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk ) != 0 ) ? HEXIN_TRUE : HEXIN_FALSE;
}
