#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    HEXIN_GPIO_1,
    HEXIN_GPIO_2,
    HEXIN_GPIO_3,
    HEXIN_GPIO_4,
    HEXIN_GPIO_5,
    HEXIN_GPIO_6,
    HEXIN_GPIO_7,
    HEXIN_GPIO_MAX, /* Denotes the total number of GPIO port aliases. Not a valid GPIO alias */
    HEXIN_GPIO_32BIT = 0x7FFFFFFF,
} hexin_gpio_t;

typedef enum
{
    HEXIN_SPI_1,
    HEXIN_SPI_MAX, /* Denotes the total number of SPI port aliases. Not a valid SPI alias */
    HEXIN_SPI_32BIT = 0x7FFFFFFF,
} hexin_spi_t;

typedef enum
{
    HEXIN_UART_1,
    HEXIN_UART_MAX, /* Denotes the total number of UART port aliases. Not a valid UART alias */
    HEXIN_UART_32BIT = 0x7FFFFFFF,
} hexin_uart_t;


/******************************************************
 *                    Constants
 ******************************************************/

/* UART port used for standard I/O */
#define STDIO_UART 							( HEXIN_UART_1 )

/* SPI flash is present */
#define HEXIN_PLATFORM_INCLUDES_SPI_FLASH
#define HEXIN_SPI_FLASH_CS 					( HEXIN_GPIO_2 )

/* Components connected to external I/Os */
#define PLATFORM_LED_COUNT               	( 1 )
#define HEXIN_LED1         					( HEXIN_GPIO_1 )


#ifdef __cplusplus
} /*extern "C" */
#endif
