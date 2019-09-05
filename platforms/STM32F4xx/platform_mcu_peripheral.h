#pragma once
#include "platform_cmsis.h"
#include "platform_constants.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_iwdg.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_usart.h"
#include "cmw_constants.h"
#include "cmw_rtos_interface.h"
#include "ring_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/* GPIOA to I */
#define NUMBER_OF_GPIO_PORTS      (8)

/* Interrupt line 0 to 15. Each line is shared among the same numbered pins across all GPIO ports */
#define NUMBER_OF_GPIO_IRQ_LINES  (16)

/* USART1 to 8 where applicable */
#if defined(STM32F446xx) || defined(STM32F40_41xxx)
    #define NUMBER_OF_UART_PORTS      (6)
#endif
#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
    #define NUMBER_OF_UART_PORTS      (8)
#endif
#if defined(STM32F401xx) || defined(STM32F411xE)
    #define NUMBER_OF_UART_PORTS      (3)    // USART1, USART2, and USART6
#endif
#if defined(STM32F412xG)
    #define NUMBER_OF_UART_PORTS      (4)    // USART1, USART2, USART3 and USART6
#endif

/* Invalid UART port number */
#define INVALID_UART_PORT_NUMBER  (0xff)

/* Invalid GPIO port number */
#define INVALID_GPIO_PORT_NUMBER  (0xff)

/* Default STDIO buffer size */
#ifndef STDIO_BUFFER_SIZE
#define STDIO_BUFFER_SIZE         (64)
#endif

/* SPI1 to SPI6 where applicable */
#if defined(STM32F40_41xxx)
#define NUMBER_OF_SPI_PORTS       (3)
#endif
#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
#define NUMBER_OF_SPI_PORTS       (6)
#endif
#if defined(STM32F401xx) || defined(STM32F446xx)
#define NUMBER_OF_SPI_PORTS       (4)
#endif
#if defined(STM32F411xE) || defined(STM32F412xG)
#define NUMBER_OF_SPI_PORTS       (5)
#endif

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/* GPIO port */
typedef GPIO_TypeDef  platform_gpio_port_t;

/* UART port */
typedef USART_TypeDef platform_uart_port_t;

/* GPIO alternate function */
typedef uint8_t       platform_gpio_alternate_function_t;

/* Peripheral clock function */
typedef void (*platform_peripheral_clock_function_t)(uint32_t clock, FunctionalState state );

typedef DMA_TypeDef     dma_registers_t;
typedef FunctionalState functional_state_t;
typedef uint32_t        peripheral_clock_t;
typedef IRQn_Type       irq_vector_t;

/******************************************************
 *                    Structures
 ******************************************************/

// typedef struct
// {
//     DMA_TypeDef*        controller;
//     DMA_Stream_TypeDef* stream;
//     uint32_t            channel;
//     IRQn_Type           irq_vector;
//     uint32_t            complete_flags;
//     uint32_t            error_flags;
// } platform_dma_config_t;

typedef struct
{
    platform_gpio_port_t* port;       /* GPIO port. platform_gpio_port_t is defined in <HEXIN-SDK>/MCU/<MCU>/platform_mcu_interface.h */
    uint8_t               pin_number; /* pin number. Valid range is defined in <HEXIN-SDK>/MCU/<MCU>/platform_mcu_interface.h         */
} platform_gpio_t;

// typedef struct
// {
//     platform_uart_port_t*           port;
//     const platform_gpio_t*          tx_pin;
//     const platform_gpio_t*          rx_pin;
//     const platform_gpio_t*          cts_pin;
//     const platform_gpio_t*          rts_pin;
//     platform_dma_config_t           tx_dma_config;
//     platform_dma_config_t           rx_dma_config;
// } platform_uart_t;

// typedef struct
// {
//     platform_uart_t*           peripheral;
//     hexin_ring_buffer_t*       rx_buffer;
//     host_semaphore_type_t      rx_complete;
//     host_semaphore_type_t      tx_complete;
//     volatile uint32_t          tx_size;
//     volatile uint32_t          rx_size;
//     volatile platform_result_t last_receive_result;
//     volatile platform_result_t last_transmit_result;
// } platform_uart_driver_t;

// typedef struct
// {
//     DMA_TypeDef*                         dma_register;
//     DMA_Stream_TypeDef*                  stream;
//     uint32_t                             channel;
//     peripheral_clock_t                   peripheral_clock;
//     platform_peripheral_clock_function_t peripheral_clock_func;
//     irq_vector_t                         irq;
// } platform_dma_t;


/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

platform_result_t platform_gpio_irq_manager_init      ( void );
uint32_t          platform_gpio_get_port_number       ( platform_gpio_port_t* gpio_port );
// platform_result_t platform_gpio_set_alternate_function( platform_gpio_port_t* gpio_port, uint8_t pin_number, GPIOOType_TypeDef output_type, GPIOPuPd_TypeDef pull_up_down_type, uint8_t alternation_function );

platform_result_t platform_watchdog_init              ( void );
platform_result_t platform_mcu_powersave_init         ( void );

platform_result_t platform_rtc_init                   ( void );
platform_result_t platform_rtc_enter_powersave        ( void );
platform_result_t platform_rtc_abort_powersave        ( void );
platform_result_t platform_rtc_exit_powersave         ( uint32_t requested_sleep_time, uint32_t *cpu_sleep_time );

//uint8_t           platform_uart_get_port_number       ( platform_uart_port_t* uart );
//void              platform_uart_irq                   ( platform_uart_driver_t* driver );
//void              platform_uart_tx_dma_irq            ( platform_uart_driver_t* driver );
//void              platform_uart_rx_dma_irq            ( platform_uart_driver_t* driver );

#ifdef __cplusplus
} /* extern "C" */
#endif

