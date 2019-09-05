#include "platform.h"
#include "platform_config.h"
#include "platform_init.h"
#include "platform_isr.h"
#include "platform_mcu_peripheral.h"
#include "hexin_defaults.h"
#include "hexin_platform.h"
// #include "gpio_button.h"

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

const platform_gpio_t platform_gpio_pins[] =
{
    [HEXIN_GPIO_1]  = { GPIOC,  7 },
    [HEXIN_GPIO_2]  = { GPIOA,  5 },
    [HEXIN_GPIO_3]  = { GPIOA,  6 },
    [HEXIN_GPIO_4]  = { GPIOA,  7 },
    [HEXIN_GPIO_5]  = { GPIOC,  5 },
    [HEXIN_GPIO_6]  = { GPIOA,  9 },
    [HEXIN_GPIO_7]  = { GPIOA, 10 },
};


/* UART peripherals and runtime drivers. Used by platform/hexin_platform_common.c */
// const platform_uart_t platform_uart_peripherals[] =
// {
//     [HEXIN_UART_1] =
//     {
//         .port               = USART1,
//         .tx_pin             = &platform_gpio_pins[HEXIN_GPIO_6 ],
//         .rx_pin             = &platform_gpio_pins[HEXIN_GPIO_7 ],
//         .cts_pin            = NULL,
//         .rts_pin            = NULL,
//         .tx_dma_config =
//         {
//             .controller     = DMA2,
//             .stream         = DMA2_Stream7,
//             .channel        = DMA_Channel_4,
//             .irq_vector     = DMA2_Stream7_IRQn,
//             .complete_flags = DMA_HISR_TCIF7,
//             .error_flags    = ( DMA_HISR_TEIF7 | DMA_HISR_FEIF7 ),
//         },
//         .rx_dma_config =
//         {
//             .controller     = DMA2,
//             .stream         = DMA2_Stream2,
//             .channel        = DMA_Channel_4,
//             .irq_vector     = DMA2_Stream2_IRQn,
//             .complete_flags = DMA_LISR_TCIF2,
//             .error_flags    = ( DMA_LISR_TEIF2 | DMA_LISR_FEIF2 | DMA_LISR_DMEIF2 ),
//         },
//     },
// };
// platform_uart_driver_t platform_uart_drivers[HEXIN_UART_MAX];


/* UART standard I/O configuration */
#ifndef HEXIN_DISABLE_STDIO
static const platform_uart_config_t stdio_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};
#endif

const hexin_gpio_t platform_gpio_leds[PLATFORM_LED_COUNT] =
{
     [HEXIN_LED_INDEX_1] = HEXIN_LED1,
};

/******************************************************
 *               Function Definitions
 ******************************************************/

void platform_init_peripheral_irq_priorities( void )
{
    /* Interrupt priority setup. Called by HEXIN/platform/MCU/STM32F2xx/platform_init.c */
    // NVIC_SetPriority( RTC_WKUP_IRQn    ,  1 ); /* RTC Wake-up event   */
    // NVIC_SetPriority( SDIO_IRQn        ,  2 ); /* SDIO                */
    // NVIC_SetPriority( DMA2_Stream3_IRQn,  3 ); /* SDIO DMA            */
    // NVIC_SetPriority( USART1_IRQn      ,  6 ); /* HEXIN_UART_1        */
    // NVIC_SetPriority( DMA2_Stream7_IRQn,  7 ); /* HEXIN_UART_1 TX DMA */
    // NVIC_SetPriority( DMA2_Stream2_IRQn,  7 ); /* HEXIN_UART_1 RX DMA */
}

/* LEDs on this platform are active HIGH */
platform_result_t platform_led_set_state(int led_index, int off_on )
{
    if ((led_index >= 0) && (led_index < PLATFORM_LED_COUNT))
    {
        switch (off_on)
        {
            case HEXIN_LED_OFF:
                platform_gpio_output_low( &platform_gpio_pins[platform_gpio_leds[led_index]] );
                break;
            case HEXIN_LED_ON:
                platform_gpio_output_high( &platform_gpio_pins[platform_gpio_leds[led_index]] );
                break;
        }
        return PLATFORM_SUCCESS;
    }
    return PLATFORM_BADARG;
}

void platform_led_init( void )
{
    /* Initialise LEDs and turn off by default */
    platform_gpio_init( &platform_gpio_pins[HEXIN_LED1], OUTPUT_PUSH_PULL );
    platform_led_set_state(HEXIN_LED_INDEX_1, HEXIN_LED_OFF);
 }


void platform_init_external_devices( void )
{
    /* Initialise LEDs and turn off by default */
    platform_led_init();

#ifndef HEXIN_DISABLE_STDIO
    /* Initialise UART standard I/O */
    platform_stdio_init( &platform_uart_drivers[STDIO_UART], &platform_uart_peripherals[STDIO_UART], &stdio_config );
#endif
}

uint32_t  platform_get_factory_reset_button_time ( uint32_t max_time )
{
    return 0;
}

/******************************************************
 *           Interrupt Handler Definitions
 ******************************************************/

// WWD_RTOS_DEFINE_ISR( usart1_irq )
// {
//     platform_uart_irq( &platform_uart_drivers[HEXIN_UART_1] );
// }

// WWD_RTOS_DEFINE_ISR( usart1_tx_dma_irq )
// {
//     platform_uart_tx_dma_irq( &platform_uart_drivers[HEXIN_UART_1] );
// }

// WWD_RTOS_DEFINE_ISR( usart1_rx_dma_irq )
// {
//     platform_uart_rx_dma_irq( &platform_uart_drivers[HEXIN_UART_1] );
// }

/******************************************************
 *            Interrupt Handlers Mapping
 ******************************************************/

/* These DMA assignments can be found STM32F2xx datasheet DMA section */
// WWD_RTOS_MAP_ISR( usart1_irq       , USART1_irq       )
// WWD_RTOS_MAP_ISR( usart1_rx_dma_irq, DMA2_Stream2_irq )
// WWD_RTOS_MAP_ISR( usart1_tx_dma_irq, DMA2_Stream7_irq )
