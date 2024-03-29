#pragma once
#include <stdint.h>
#include "hexin_result.h"

#ifdef __cplusplus
extern "C" {
#endif

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
 * Main
 *
 * @param[in] : void
 * @return    : int
 *
 * @usage
 * \li Defined by RTOS or application and called by CRT0
 */
int main( void );


/**
 * Initialise the platform during hexin_init
 * @return    : result code
 *
 */
hexin_result_t hexin_platform_init( void );

/**
 * Initialise system clock(s)
 * This function includes initialisation of PLL and switching to fast clock
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage
 * \li Defined internally in platforms/MCU/<MCU>/platform_init.c and called by CRT0
 * \li Weakly defined in platforms/MCU/<MCU>/platform_init.c. Users may override it as desired
 */
extern void platform_init_system_clocks( void );

/**
 * Initialise memory subsystem
 * This function initialises memory subsystem such as external RAM
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage
 * \li Defined internally in platforms/MCU/<MCU>/platform_init.c and called by CRT0
 * \li Weakly defined in platforms/MCU/<MCU>/platform_init.c. Users may override it as desired
 */
extern void platform_init_memory( void );

/**
 * Initialise default MCU infrastructure
 * This function initialises default MCU infrastructure such as watchdog
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage
 * \li Defined and used internally in platforms/MCU/<MCU>/platform_init.c
 */
extern void platform_init_mcu_infrastructure( void );

/**
 * Initialise external devices
 * This function initialises and puts external peripheral devices on the board such as LEDs, buttons, sensors, etc into their reset state
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage     :
 * \li MUST be defined in platforms/<Platform>/platform.c
 * \li Called by @ref platform_init_mcu_infrastructure()
 */
extern void platform_init_external_devices( void );

/**
 * Initialise priorities of interrupts used by the platform peripherals
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage
 * \li MUST be defined in platforms/<Platform>/platform.c
 * \li Called by @ref platform_init_mcu_infrastructure()
 */
extern void platform_init_peripheral_irq_priorities( void );

/**
 * Initialise priorities of interrupts used by the RTOS
 *
 * @param[in] : void
 * @return    : void

 * @usage
 * \li MUST be defined by the RTOS
 * \li Called by @ref platform_init_mcu_infrastructure()
 */
extern void platform_init_rtos_irq_priorities( void );

/**
 * Used to run last step initialisation
 *
 * @param[in] : void
 * @return    : void
 *
 * @usage
 * \li Defined internally in platforms/MCU/<MCU>/platform_init.c and called by CRT0
 * \li Weakly defined in platforms/MCU/<MCU>/platform_init.c. Users may override it as desired
 */
extern void platform_init_complete( void );

#ifdef __cplusplus
} /*extern "C" */
#endif
