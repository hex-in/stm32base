#pragma once

#include "hexin_result.h"
#include "hexin_utilities.h"
#include "cmw_constants.h"
#include "platform_peripheral.h"
#include "platform.h" /* This file is unique for each platform */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
/** @cond !ADDTHIS*/
#define HEXIN_I2C_START_FLAG                    (1U << 0)
#define HEXIN_I2C_REPEATED_START_FLAG           (1U << 1)
#define HEXIN_I2C_STOP_FLAG                     (1U << 2)

#define HEXIN_GPIO_NONE ((hexin_gpio_t)0x7fffffff)

/** @endcond */
/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    HEXIN_ACTIVE_LOW   = 0,     /**< ACTIVE LOW    */
    HEXIN_ACTIVE_HIGH  = 1,     /**< ACTIVE HIGH   */
} hexin_active_state_t;

typedef enum
{
    HEXIN_LED_OFF   = 0,        /**< OFF    */
    HEXIN_LED_ON,               /**< ON     */
} hexin_led_state_t;

typedef enum
{
    HEXIN_LED_INDEX_1 = 0,      /**< LED INDEX 1    */

    HEXIN_LED_INDEX_MAX         /**< denotes end of list - do not use */
} hexin_led_index_t;

/******************************************************
 *         Type Definitions
 ******************************************************/

typedef platform_pin_config_t                   hexin_gpio_config_t;            /**< GPIO configuration     */

//typedef platform_gpio_irq_trigger_t             hexin_gpio_irq_trigger_t;       /**< GPIO IRQ trigger       */

//typedef platform_gpio_irq_callback_t            hexin_gpio_irq_handler_t;       /**< GPIO IRQ callback      */

//typedef platform_uart_config_t                  hexin_uart_config_t;            /**< UART configuration     */

//typedef platform_i2c_bus_address_width_t        hexin_i2c_bus_address_width_t;  /**< I2C bus address width  */

//typedef platform_i2c_speed_mode_t               hexin_i2c_speed_mode_t;         /**< I2C speed mode         */

//typedef platform_i2c_message_t                  hexin_i2c_message_t;            /**< I2C message            */

//typedef platform_spi_message_segment_t          hexin_spi_message_segment_t;    /**< SPI message            */

//typedef platform_rtc_time_t                     hexin_rtc_time_t;               /**< RTC time               */

//typedef platform_spi_slave_config_t             hexin_spi_slave_config_t;       /**< SPI Slave configuration*/

//typedef platform_spi_slave_transfer_direction_t hexin_spi_slave_transfer_direction_t; /**< SPI transfer direction (Read/Write) */

//typedef platform_spi_slave_transfer_status_t    hexin_spi_slave_transfer_status_t;    /**< SPI transfer status */

//typedef platform_spi_slave_command_t            hexin_spi_slave_command_t;      /**< SPI slave command */

//typedef platform_spi_slave_data_buffer_t        hexin_spi_slave_data_buffer_t;  /**< SPI slave data */

/******************************************************
 *                    Structures
 ******************************************************/


/**
 * Specifies details of an external SPI slave device which is connected to the HEXIN system
 */
//typedef struct
//{
//    hexin_spi_t                   port;          /**< Which SPI peripheral of the platform to use for the SPI device being specified */
//    hexin_gpio_t                  chip_select;   /**< Which hardware pin to use for Chip Select of the SPI device being specified */
//    uint32_t                      speed;         /**< SPI device access speed in Hertz */
//    uint8_t                       mode;          /**< Mode of operation for SPI port See HEXIN/platform/include/platform_peripheral.h  SPI mode constants */
//    uint8_t                       bits;          /**< Number of data bits - usually 8, 16 or 32 */
//} hexin_spi_device_t;

/******************************************************
 *                     Variables
 ******************************************************/

//#ifdef HEXIN_PLATFORM_INCLUDES_SPI_FLASH
//extern const hexin_spi_device_t hexin_spi_flash;
//#endif

/******************************************************
 *              Function Declarations
 ******************************************************/

/*****************************************************************************/
/** @defgroup platform       Platform functions
 *
 *  HEXIN hardware platform functions
 *  These functions provide interface to hardware present in the
 *  underlying platform.
 *  Most of these functions will invoke platform-specific functions
 *  which are implemented in the platform specific code.
 *
 *  These functions generally have no upper/lower bounds, speeds,
 *  or other configuration parameters specified, as these parameters
 *  are specified by the underlying platform specific code.
 *
 */
/*****************************************************************************/

/*****************************************************************************/
/** @addtogroup uart       UART
 *  @ingroup platform
 *
 * Universal Asynchronous Receiver Transmitter (UART) Functions
 *
 * Configuration and specifications of the UART depend on
 * underlying platform, please refer to
 * <HEXIN_SDK>/platforms/<platform_name>/platform.h and
 * <HEXIN_SDK>/platforms/<platform_name>/platform.c for details.
 *
 * Example Usage: (Check for return values in actual implementation)
 *
 *      // Ring buffer (optional) is used to hold data received from UART
 *      ring_buffer_init(&rx_buffer, rx_data, RX_BUFFER_SIZE);
 *
 *      // HEXIN_UART_1 is the UART Port
 *      // Uart ports are enumerated from HEXIN_UART_1 to HEXIN_UART_MAX
 *      // in <HEXIN_SDK>/platforms/<platform_name>/platform.h
 *      // Example uart_config
 *      hexin_uart_config_t uart_config =
 *      {
 *          .baud_rate    = 115200,
 *          .data_width   = DATA_WIDTH_8BIT,
 *          .parity       = NO_PARITY,
 *          .stop_bits    = STOP_BITS_1,
 *          .flow_control = FLOW_CONTROL_DISABLED,
 *      };
 *      // rx_buffer is optional buffer used to hold data received from UART
 *      hexin_uart_init( HEXIN_UART_1, &uart_config, &rx_buffer);
 *
 *      // Done with initialization, now we can send/receive data
 *      hexin_uart_transmit_bytes( HEXIN_UART_1, tx_data, tx_data_size );
 *      hexin_uart_receive_bytes( HEXIN_UART_1, &rx_data, &rx_data_size, timeout_in_ms )
 *
 * Refer to <WICD_SDK>/apps/snip/uart/uart.c for example usage.
 *
 *  @{
 */
/*****************************************************************************/

/** Initializes a UART interface
 *
 * Prepares an UART hardware interface for communications
 *
 * @param[in]  uart               : The interface which should be initialized,
 *                                  platform header file enumerates interfaces from HEXIN_UART_0 to HEXIN_UART_MAX.
 * @param[in]  config             : UART configuration structure
 *                                  defined in HEXIN/platform/include/platform_peripheral.h
 * @param[in]  optional_rx_buffer : Pointer to an optional RX ring buffer
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_uart_init( hexin_uart_t uart, const hexin_uart_config_t* config, hexin_ring_buffer_t* optional_rx_buffer );


/** Deinitializes a UART interface
 *
 * @param[in]  uart : The interface which should be deinitialized
 *                    platform header file enumerates interfaces from HEXIN_UART_0 to HEXIN_UART_MAX.
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_uart_deinit( hexin_uart_t uart );


/** Transmit data on a UART interface
 *
 * @param[in]  uart     : The UART interface, platform header file enumerates UART interfaces
 *                        from HEXIN_UART_0 to HEXIN_UART_MAX.
 * @param[in]  data     : Pointer to the start of data
 * @param[in]  size     : Number of bytes to transmit
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_uart_transmit_bytes( hexin_uart_t uart, const void* data, uint32_t size );


/** Receive data on a UART interface
 *
 * @param[in]     uart    : The UART interface, platform header file enumerates UART interfaces
 *                          from HEXIN_UART_0 to HEXIN_UART_MAX.
 * @param[out]    data    : Pointer to the buffer which will store incoming data
 * @param[in,out] size    : Number of bytes to receive, function return in same parameter number of actually received bytes
 * @param[in]     timeout : Timeout in milliseconds
 *                          HEXIN_WAIT_FOREVER and HEXIN_NO_WAIT can be specified for infinite and no wait.
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_uart_receive_bytes( hexin_uart_t uart, void* data, uint32_t* size, uint32_t timeout );

/** @} */
/*****************************************************************************/
/** @addtogroup spi       SPI
 *  @ingroup platform
 *
 * Serial Peripheral Interface (SPI) Functions
 *
 * For SPI specification and configuration for the underlying platform,
 * please refer to
 * <HEXIN_SDK>/platforms/<platform_name>/platform.c and
 * <HEXIN_SDK>/platforms/<platform_name>/platform.h
 *
 * Example usage: (Check for return values in actual implementation)
 *
 *      // <HEXIN_SDK>/platforms/<platform_name>/platform.h
 *      // hexin_spi_device_t defines a SPI slave device connected to the MCU
 *      hexin_spi_init( &hexin_spi_device );
 *
 *      // Define SPI message segment
 *      // hexin_spi_message_segment_t spi_segment[NUM_SPI_MSG_SEGMENTS];
 *      // spi_message_segment structure consists of
 *      // {
 *      //     const void* tx_buffer;
 *      //     void*       rx_buffer;
 *      //     uint32_t    length; //Length of data to be sent
 *      // }
 *
 *      // Initialize all the spi_message segments
 *
 *      spi_segment[0].tx_buffer = tx_buffer;
 *      spi_segment[0].rx_buffer = rx_buffer;
 *      spi_segment[0].length = message_length; //Length of data in tx_buffer
 *
 *      //Continue to add spi_segment[1], spi_segment[2] ...
 *
 *      // Transfer the segments
 *      hexin_spi_transfer( &hexin_spi_device, spi_segment, number_of_segments);
 *
 *      // if hexin_spi_transfer returns HEXIN_SUCCESS, rx_buffer(If non-NULL) should be populated
 *
 * HEXIN SPI Functions can be divided into
 *  SPI Master Functions
 *      For platforms where MCU is used as SPI master
 *
 *      hexin_spi_init()
 *      hexin_spi_transmit()
 *      hexin_spi_transfer()
 *      hexin_spi_deinit()
 *
 *  SPI Slave Functions
 *      For platforms where MCU can be used as SPI Slave
 *      Please refer to <HEXIN_SDK>/apps/snip/spi_slave For example usage
 *
 *      hexin_spi_slave_init()
 *      hexin_spi_slave_deinit()
 *      hexin_spi_slave_receive_command()
 *      hexin_spi_slave_transfer_data()
 *      hexin_spi_slave_send_error_status()
 *      hexin_spi_slave_generate_interrupt()
 *
 *  @{
 */
/*****************************************************************************/

/** initializes the SPI interface for a given SPI device
 *
 * Prepares a SPI hardware interface for communication as a master
 *
 * @param[in]  spi : The SPI device to be initialized
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if the SPI device could not be initialized
 */
//hexin_result_t hexin_spi_init( const hexin_spi_device_t* spi );


/** Transmits data to a SPI device
 *
 * @param[in]  spi                : The SPI device to be initialized
 * @param[in]  segments           : A pointer to an array of segments
 * @param[in]  number_of_segments : The number of segments to transfer
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
//hexin_result_t hexin_spi_transmit( const hexin_spi_device_t* spi, const hexin_spi_message_segment_t* segments, uint16_t number_of_segments );


/** Transmits and/or receives data from a SPI device
 *
 * @param[in]  spi                : The SPI device to be initialized
 * @param[in]  segments           : A pointer to an array of segments
 * @param[in]  number_of_segments : The number of segments to transfer
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
//hexin_result_t hexin_spi_transfer( const hexin_spi_device_t* spi, const hexin_spi_message_segment_t* segments, uint16_t number_of_segments );


/** De-initializes a SPI interface
 *
 * Turns off a SPI hardware interface
 *
 * @param[in]  spi : The SPI device to be de-initialized
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred
 */
//hexin_result_t hexin_spi_deinit( const hexin_spi_device_t* spi );


/** initializes a SPI slave interface
 *
 * @param[in]  spi    : The SPI slave interface to be initialized
 * @param[in]  config : SPI slave configuration
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_init( hexin_spi_t spi, const hexin_spi_slave_config_t* config );


/** De-initializes a SPI slave interface
 *
 * @param[in]  spi : The SPI slave interface to be de-initialized
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_deinit( hexin_spi_t spi );


/** Receive command from the remote SPI master
 *
 * @param[in]   spi         : The SPI slave interface
 * @param[out]  command     : Pointer to the variable which will contained the received command
 * @param[in]   timeout_ms  : Timeout in milliseconds
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_receive_command( hexin_spi_t spi, hexin_spi_slave_command_t* command, uint32_t timeout_ms );


/** Transfer data to/from the remote SPI master
 *
 * @param[in]  spi         : The SPI slave interface
 * @param[in]  direction   : Transfer direction
 * @param[in]  buffer      : The buffer which contain the data to transfer
 * @param[in]  timeout_ms  : timeout in milliseconds
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_transfer_data( hexin_spi_t spi, hexin_spi_slave_transfer_direction_t direction, hexin_spi_slave_data_buffer_t* buffer, uint32_t timeout_ms );


/** Send an error status over the SPI slave interface
 *
 * @param[in]  spi          : The SPI slave interface
 * @param[in]  error_status : SPI slave error status
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_send_error_status( hexin_spi_t spi, hexin_spi_slave_transfer_status_t error_status );


/** Generate an interrupt on the SPI slave interface
 *
 * @param[in]  spi               : The SPI slave interface
 * @param[in]  pulse_duration_ms : Interrupt pulse duration in milliseconds
 *
 * @return @ref hexin_result_t
 */
//hexin_result_t hexin_spi_slave_generate_interrupt( hexin_spi_t spi, uint32_t pulse_duration_ms );

/** @} */
/*****************************************************************************/
/** @addtogroup gpio       GPIO
 *  @ingroup platform
 *
 * General Purpose Input/Output pin (GPIO) Functions
 *
 * The HEXIN GPIO functions can be divided into the following categories
 *
 *      // Initialization/De-Initialization functions
 *      hexin_gpio_init();
 *      hexin_gpio_deinit();
 *
 *      // GPIO I/O functions
 *      hexin_gpio_output_high()
 *      hexin_gpio_output_low()
 *      hexin_gpio_input_get()
 *
 *      // Manage interrupt from GPIO pins
 *      hexin_gpio_input_irq_enable()
 *      hexin_gpio_input_irq_disable()
 *
 * hexin_gpio_t : Refers to the GPIO Pin, for details refer to
 *      <HEXIN_SDK>/platforms/<platform_name>/platform.h
 *      <HEXIN_SDK>/platforms/<platform_name>/platform.c
 *
 * hexin_gpio_config_t : GPIO configuration, refer to :
 *      <HEXIN_SDK>/platform/include/platform_peripheral.h : platform_pin_config_t
 *
 *  @{
 */
/*****************************************************************************/

/** Initializes a GPIO pin
 *
 * Prepares a GPIO pin for use.
 *
 * @param[in] gpio          : The gpio pin which should be initialized
 * @param[in] configuration : A structure containing the required
 *                            gpio configuration
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_gpio_init( hexin_gpio_t gpio, hexin_gpio_config_t configuration );

/** De-initializes a GPIO pin
 *
 * Clears a GPIO pin from use.
 *
 * @param[in] gpio          : The gpio pin which should be de-initialized
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_gpio_deinit( hexin_gpio_t gpio );

/** Sets an output GPIO pin high
 *
 * Sets an output GPIO pin high. Using this function on a
 * gpio pin which is set to input mode is undefined.
 *
 * @param[in] gpio          : The gpio pin which should be set high
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_gpio_output_high( hexin_gpio_t gpio );


/** Sets an output GPIO pin low
 *
 * Sets an output GPIO pin low. Using this function on a
 * gpio pin which is set to input mode is undefined.
 *
 * @param[in] gpio          : The gpio pin which should be set low
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_gpio_output_low( hexin_gpio_t gpio );


/** Get the state of an input GPIO pin
 *
 * Get the state of an input GPIO pin. Using this function on a
 * gpio pin which is set to output mode will return an undefined value.
 *
 * @param[in] gpio          : The gpio pin which should be read
 *
 * @return    HEXIN_TRUE  : if high
 * @return    HEXIN_FALSE : if low
 */
hexin_bool_t   hexin_gpio_input_get( hexin_gpio_t gpio );


/** Enables an interrupt trigger for an input GPIO pin
 *
 * Enables an interrupt trigger for an input GPIO pin.
 * Using this function on an uninitialized gpio pin or
 * a gpio pin which is set to output mode is undefined.
 *
 * @param[in] gpio    : The gpio pin which will provide the interrupt trigger
 * @param[in] trigger : The type of trigger (rising/falling edge, high/low level)
 * @param[in] handler : A function pointer to the interrupt handler
 * @param[in] arg     : An argument that will be passed to the
 *                      interrupt handler
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_gpio_input_irq_enable( hexin_gpio_t gpio, hexin_gpio_irq_trigger_t trigger, hexin_gpio_irq_handler_t handler, void* arg );

/** Enables an input GPIO pin to wakeup from Deep-Sleep
 *
 * @param[in] gpio    : The gpio pin which will provide the interrupt trigger
 * @param[in] trigger : The type of trigger (rising/falling edge, high/low level)
 *                      interrupt handler
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_gpio_deepsleep_wakeup_enable(hexin_gpio_t gpio, hexin_gpio_irq_trigger_t trigger);

/** Disables an interrupt trigger for an input GPIO pin
 *
 * Disables an interrupt trigger for an input GPIO pin.
 * Using this function on a gpio pin which has not been set up
 * using @ref hexin_gpio_input_irq_enable is undefined.
 *
 * @param[in] gpio    : The gpio pin which provided the interrupt trigger
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_gpio_input_irq_disable( hexin_gpio_t gpio );

/**
 * Set status of an LED (Off or On)
 *
 * NOTES: LED initialization is done in platform_init_external_devices() in each platform.c file
 *        Platform code knows the polarity of the LEDs and sets the output high/low as appropriate.
 *        If there are no LEDs on a platform, provide a dummy function in platform.c.
 *
 * @param[in]   led_index  : hexin_led_t LED
 * @param[in]   off_on     : HEXIN_LED_OFF or HEXIN_LED_ON
 *
 * @return  HEXIN_SUCCESS
 *          HEXIN_BADARG
 */
hexin_result_t hexin_led_set_state(hexin_led_index_t led_index, hexin_led_state_t off_on );

/** @} */
/*****************************************************************************/
/** @addtogroup watchdog       Watchdog
 *  @ingroup platform
 *
 * Watchdog Timer Functions
 *
 * Watchdog is initialized during boot-up
 * Unless HEXIN_DISABLE_WATCHDOG is defined.
 *
 * In HEXIN System Monitor thread periodically kicks/feeds
 * the watchdog.
 *  @{
 */
/*****************************************************************************/

/** Kick the system watchdog.
 *
 * Resets (kicks) the timer of the system watchdog.
 * This is called periodically by system_monitor thread
 * <HEXIN_SDK/HEXIN/internal/system_monitor.c>
 *
 * Applications need not call this function, unless
 * there is a use case where System monitor thread may
 * not be scheduled for long period of time
 * greater than DEFAULT_SYSTEM_MONITOR_PERIOD (5sec)
 *
 * For example: If the application has a thread which
 * is running at the highest priority and doing a while(1).
 * Such use case would be rare.
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
hexin_result_t hexin_watchdog_kick( void );

/** @} */
/*****************************************************************************/
/** @addtogroup mcupowersave       Powersave
 *
 * MCU Powersave Functions
 *
 * Functions to enable MCU to enter/leave lower power consumption
 * states when it is idle.
 *
 * By default Powersave is not enabled in HEXIN.
 *
 *  @{
 */
/*****************************************************************************/

/** Enables the MCU to enter powersave mode.
 *
 * @warning   If the MCU drives the sleep clock input pin of the WLAN chip,   \n
 *            ensure the 32kHz clock output from the MCU is maintained while  \n
 *            the MCU is in powersave mode. The WLAN sleep clock reference is \n
 *            typically configured in the file:                               \n
 *            <HEXIN-SDK>/platforms/<PLATFORM_NAME>/platform.h
 * @return    void
 */
void hexin_platform_mcu_enable_powersave( void );


/** Stops the MCU entering powersave mode.
 *
 * @return    void
 */
void hexin_platform_mcu_disable_powersave( void );

/** @} */

/**
 * This function will return the value of time read from the on board CPU real time clock. Time value must be given in the format of
 * the structure hexin_rtc_time_t
 *
 * @param[out] time        : Pointer to a time structure
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_platform_get_rtc_time( hexin_rtc_time_t* time );


/**
 * This function will set MCU RTC time to a new value. Time value must be given in the format of
 * the structure hexin_rtc_time_t
 *
 * @param[in] time        : Pointer to a time structure
 *
 * @return    HEXIN_SUCCESS : on success.
 * @return    HEXIN_ERROR   : if an error occurred with any step
 */
//hexin_result_t hexin_platform_set_rtc_time( const hexin_rtc_time_t* time );

#ifdef __cplusplus
} /*extern "C" */
#endif
