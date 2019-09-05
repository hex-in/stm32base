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

typedef enum
{
    INPUT_PULL_UP,             /**< Input with an internal pull-up resistor - use with devices that actively drive the signal low - e.g. button connected to ground */
    INPUT_PULL_DOWN,           /**< Input with an internal pull-down resistor - use with devices that actively drive the signal high - e.g. button connected to a power rail */
    OUTPUT_PUSH_PULL,          /**< Output actively driven high and actively driven low - must not be connected to other active outputs - e.g. LED output */
    INPUT_HIGH_IMPEDANCE,      /**< Input - must always be driven, either actively or by an external pullup resistor */
    OUTPUT_OPEN_DRAIN_NO_PULL, /**< Output actively driven low but is high-impedance when set high - can be connected to other open-drain/open-collector outputs. Needs an external pull-up resistor */
    OUTPUT_OPEN_DRAIN_PULL_UP, /**< Output actively driven low and is pulled high with an internal resistor when set high - can be connected to other open-drain/open-collector outputs. */
} platform_pin_config_t;

typedef enum
{
    IRQ_TRIGGER_RISING_EDGE  = 0x1, /**< Interrupt triggered at input signal's rising edge  */
    IRQ_TRIGGER_FALLING_EDGE = 0x2, /**< Interrupt triggered at input signal's falling edge */
    IRQ_TRIGGER_BOTH_EDGES   = IRQ_TRIGGER_RISING_EDGE | IRQ_TRIGGER_FALLING_EDGE, /**< Interrupt triggered at input signal's rising and falling edge */
    IRQ_TRIGGER_LEVEL_HIGH   = 0x4, /**< Interrupt triggered when input signal's level is high */
    IRQ_TRIGGER_LEVEL_LOW    = 0x8, /**< Interrupt triggered when input signal's level is low  */
} platform_gpio_irq_trigger_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/**
 * GPIO interrupt callback handler
 */
typedef void (*platform_gpio_irq_callback_t)( void* arg );      /**< GPIO interrupt callback handler */

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

/**
 * Initialise the specified GPIO pin
 *
 * @param[in] gpio   : gpio pin
 * @param[in] config : pin configuration
 *
 * @return @ref platform_result_t
 */
platform_result_t platform_gpio_init( const platform_gpio_t* gpio, platform_pin_config_t config );

/**
 * Deinitialise the specified GPIO pin
 *
 * @param[in] gpio : gpio pin
 *
 * @return @ref platform_result_t
 */
platform_result_t platform_gpio_deinit( const platform_gpio_t* gpio );


/**
 * Toggle the specified GPIO pin output high
 *
 * @param[in] gpio : gpio pin
 *
 * @return @ref platform_result_t
 */
platform_result_t platform_gpio_output_high( const platform_gpio_t* gpio );


/**
 * Toggle the specified GPIO pin output low
 *
 * @param[in] gpio : gpio pin
 *
 * @return @ref platform_result_t
 */
platform_result_t platform_gpio_output_low( const platform_gpio_t* gpio );


/**
 * Retrieve logic level of the GPIO input pin specified
 *
 * @param[in] gpio : gpio pin
 *
 * @return @ref platform_result_t
 */
hexin_bool_t platform_gpio_input_get( const platform_gpio_t* gpio );


/**
 * Toggle the specified GPIO pin
 *
 * @param[in] gpio : gpio pin
 *
 * @return @ref platform_result_t
 */

platform_result_t platform_gpio_toggle( const platform_gpio_t* gpio );

#ifdef __cplusplus
} /*"C" */
#endif
