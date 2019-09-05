
#include <stdint.h>
#include <string.h>
#include "hexin_assert.h"
#include "platform_peripheral.h"
#include "hexin_result.h"
#include "hexin_platform.h"
#include "platform_config.h"
#include "platform_mcu_peripheral.h"
#include "platform_init.h"

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
 *               Static Function Declarations
 ******************************************************/


/******************************************************
 *               Variable Definitions
 ******************************************************/

/* Externed from platforms/<Platform>/platform.c */
extern const platform_gpio_t       platform_gpio_pins[];

// extern const platform_uart_t       platform_uart_peripherals[];
// extern platform_uart_driver_t      platform_uart_drivers[];



/******************************************************
 *               Function Definitions
 ******************************************************/

hexin_result_t hexin_platform_init( void )
{
    // WPRINT_PLATFORM_INFO( ("Platform " PLATFORM " initialised\n") );

    // if ( HEXIN_TRUE == platform_watchdog_check_last_reset( ) )
    // {
    //     WPRINT_PLATFORM_ERROR( ("WARNING: Watchdog reset occured previously. Please see platform_watchdog.c for debugging instructions.\n") );
    // }

    return HEXIN_SUCCESS;
}

hexin_result_t hexin_gpio_init( hexin_gpio_t gpio, hexin_gpio_config_t configuration )
{
    return (hexin_result_t) platform_gpio_init( &platform_gpio_pins[gpio], configuration );
}

hexin_result_t hexin_gpio_deinit( hexin_gpio_t gpio )
{
    return (hexin_result_t) platform_gpio_deinit( &platform_gpio_pins[gpio] );
}

hexin_result_t hexin_gpio_output_high( hexin_gpio_t gpio )
{
    return (hexin_result_t) platform_gpio_output_high( &platform_gpio_pins[gpio] );
}

hexin_result_t hexin_gpio_output_low( hexin_gpio_t gpio )
{
    return (hexin_result_t) platform_gpio_output_low( &platform_gpio_pins[gpio] );
}

hexin_bool_t hexin_gpio_input_get( hexin_gpio_t gpio )
{
    return platform_gpio_input_get( &platform_gpio_pins[gpio] );
}

// hexin_result_t hexin_gpio_input_irq_enable( hexin_gpio_t gpio, hexin_gpio_irq_trigger_t trigger, hexin_gpio_irq_handler_t handler, void* arg )
// {
//     return (hexin_result_t) platform_gpio_irq_enable( &platform_gpio_pins[gpio], trigger, handler, (void*)arg );
// }

// hexin_result_t hexin_gpio_deepsleep_wakeup_enable( hexin_gpio_t gpio, hexin_gpio_irq_trigger_t trigger )
// {
//     return (hexin_result_t) platform_gpio_deepsleep_wakeup_enable( &platform_gpio_pins[ gpio ], trigger );
// }

// hexin_result_t hexin_gpio_input_irq_disable( hexin_gpio_t gpio )
// {
//     return (hexin_result_t) platform_gpio_irq_disable( &platform_gpio_pins[gpio] );
// }

hexin_result_t hexin_led_set_state( hexin_led_index_t led_index, hexin_led_state_t off_on )
{
#ifdef GPIO_LED_NOT_SUPPORTED
    UNUSED_PARAMETER(led_index);
    UNUSED_PARAMETER(off_on);
    return HEXIN_UNSUPPORTED;
#else
    return (hexin_result_t)platform_led_set_state( led_index, off_on );
#endif
}

// void hexin_platform_mcu_enable_powersave( void )
// {
// #ifndef HEXIN_DISABLE_MCU_POWERSAVE
//     platform_mcu_powersave_enable( );
// #endif
// }

// void hexin_platform_mcu_disable_powersave( void )
// {
// #ifndef HEXIN_DISABLE_MCU_POWERSAVE
//     platform_mcu_powersave_disable( );
// #endif
// }

// hexin_result_t hexin_platform_get_rtc_time( hexin_rtc_time_t* time )
// {
//     return (hexin_result_t) platform_rtc_get_time( time );
// }

// hexin_result_t hexin_platform_set_rtc_time( const hexin_rtc_time_t* time )
// {
//     return (hexin_result_t) platform_rtc_set_time( time );
// }

// hexin_result_t hexin_uart_init( hexin_uart_t uart, const hexin_uart_config_t* config, hexin_ring_buffer_t* optional_rx_buffer )
// {
// #ifndef HEXIN_DISABLE_STDIO
//     /* Interface is used by STDIO. Uncomment HEXIN_DISABLE_STDIO to overcome this */
//     if ( uart == STDIO_UART )
//     {
//         return HEXIN_ERROR;
//     }
// #endif

//     return (hexin_result_t) platform_uart_init( &platform_uart_drivers[uart], &platform_uart_peripherals[uart], config, optional_rx_buffer );
// }

// hexin_result_t hexin_uart_deinit( hexin_uart_t uart )
// {
//     return (hexin_result_t) platform_uart_deinit( &platform_uart_drivers[uart] );
// }

// hexin_result_t hexin_uart_transmit_bytes( hexin_uart_t uart, const void* data, uint32_t size )
// {
//     return (hexin_result_t) platform_uart_transmit_bytes( &platform_uart_drivers[uart], (const uint8_t*) data, size );
// }

// hexin_result_t hexin_uart_receive_bytes( hexin_uart_t uart, void* data, uint32_t* size, uint32_t timeout )
// {
//     return (hexin_result_t) platform_uart_receive_bytes( &platform_uart_drivers[uart], (uint8_t*)data, size, timeout );
// }

// hexin_result_t hexin_watchdog_kick( void )
// {
//     return (hexin_result_t) platform_watchdog_kick( );
// }

// uint64_t hexin_get_nanosecond_clock_value( void )
// {
//     return platform_get_nanosecond_clock_value( );
// }

// void hexin_deinit_nanosecond_clock( void )
// {
//     platform_deinit_nanosecond_clock( );
// }

// void hexin_reset_nanosecond_clock( void )
// {
//     platform_reset_nanosecond_clock( );
// }

// void hexin_init_nanosecond_clock( void )
// {
//     platform_init_nanosecond_clock( );
// }
