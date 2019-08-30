#pragma once
#include "platform_cmsis.h"
#include "platform_constants.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_iwdg.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_usart.h"

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

#if defined(STM32F405xx)
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

/* SPI port */
typedef SPI_TypeDef   platform_spi_port_t;

/* I2C port */
typedef I2C_TypeDef   platform_i2c_port_t;

/* GPIO alternate function */
typedef uint8_t       platform_gpio_alternate_function_t;

/* Peripheral clock function */
typedef void (*platform_peripheral_clock_function_t)(uint32_t clock, FunctionalState state );

#ifdef __cplusplus
} /* extern "C" */
#endif

