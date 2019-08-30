/** @file
 *  Defines generic data and enumerated types used by Platform Peripheral API
 *  Declares function prototypes for Platform peripheral API
 *
 *
 *   ------------------------------------------------------------------------------------------------------------
 *   Porting Notes
 *   ------------------------------------------------------------------------------------------------------------
 *   * This file defines and consolidates various standardized data types required by the Platform Peripheral API
 *   * Generic Platform Peripheral data types are defined in this file
 *   * MCU-specific data types are defined in <HEXIN-SDK>/platform/MCU/<MCU_family>/platform_mcu_peripheral.h
 *   * platform_mcu_peripheral.h may include declarations, definitions, and function prototypes which are local to the MCU family
 *   * The following table outlines the structures that need to be defined in platform_mcu_peripheral.h:
 *   +------------------------+-----------------------+
 *   | Name                   | Description           |
 *   +------------------------+-----------------------+
 *   | platform_gpio_t        | GPIO interface        |
 *   | platform_uart_t        | UART interface        |
 *   | platform_uart_driver_t | UART driver interface |
 *   | platform_spi_t         | SPI  interface        |
 *   | platform_i2c_t         | I2C  interface        |
 *   | platform_pwm_t         | PWM  interface        |
 *   | platform_adc_t         | ADC  interface        |
 *   +------------------------+-----------------------+
 *
 *
*/

#pragma once
#include "stdint.h"
#include "platform_mcu_peripheral.h" /* Include MCU-specific types */
#include "platform_toolchain.h"
#include "ring_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif



/******************************************************
 *                      Macros
 ******************************************************/

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

/**
 * performs complete reset operation
 */
void platform_mcu_reset( void ) NORETURN;


#ifdef __cplusplus
} /*"C" */
#endif
