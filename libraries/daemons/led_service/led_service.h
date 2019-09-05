/*
 * LED service daemon
 *
 *  1) Define 2 LEDs, their GPIOs in the header:
 *
 * in <platform>/platform.h:
 *    (see platforms/platform.h)
 *
 * Remove:
 * #define GPIO_LED_NOT_SUPPORTED
 *
 * Add:
 * #define PLATFORM_LED_COUNT               ( 2 )
 * #define HEXIN_LED1           ( HEXIN_GPIO_xx )
 * #define HEXIN_LED2           ( HEXIN_GPIO_xx )
 *
 * 2) Add table and code in <platform>/platform.c:
 *    (see platforms/platform.c)
 *
 * Add this table:
 * const hexin_gpio_t platform_gpio_leds[PLATFORM_LED_COUNT] =
 * {
 *     [HEXIN_LED_INDEX_1] = HEXIN_LED1,
 *     [HEXIN_LED_INDEX_2] = HEXIN_LED2,
 * };
 *
 * Add this code (pay attention to platform_led_set_state() to be your active state is correct):
 *
 *  // LEDs on this platform are active HIGH
 * platform_result_t platform_led_set_state(int led_index, int off_on )
 * {
 *     if ((led_index >= 0) && (led_index < PLATFORM_LED_COUNT))
 *     {
 *         switch (off_on)
 *         {
 *             case HEXIN_LED_OFF:
 *                 platform_gpio_output_low( &platform_gpio_pins[platform_gpio_leds[led_index]] );
 *                 break;
 *             case HEXIN_LED_ON:
 *                 platform_gpio_output_high( &platform_gpio_pins[platform_gpio_leds[led_index]] );
 *                 break;
 *         }
 *         return PLATFORM_SUCCESS;
 *     }
 *     return PLATFORM_BADARG;
 * }
 *
 * void platform_led_init( void )
 * {
 *     // Initialise LEDs and turn off by default
 *     platform_gpio_init( &platform_gpio_pins[HEXIN_LED1], OUTPUT_PUSH_PULL );
 *     platform_gpio_init( &platform_gpio_pins[HEXIN_LED2], OUTPUT_PUSH_PULL );
 *     platform_led_set_state(HEXIN_LED_INDEX_1, HEXIN_LED_OFF);
 *     platform_led_set_state(HEXIN_LED_INDEX_2, HEXIN_LED_OFF);
 *  }
 *
 *  Modify platform_init_external_devices() to call platform_led_init()
 *
 *
 * 3) Use pre-defined patterns, or define tables of LED flashing patterns
 *
 * See the pre-defined patterns for examples in libraries/daemons/led_service/led_service.c.
 *     and apps/snip/led_example/led_example.c
 *
 * Example: Use a pre-defined pattern to blinking LED 1 slowly
 *
 * {
 *  hexin_led_service_init();
 *
 *  hexin_led_service_start_pattern( HEXIN_LED_PATTERN_LED_1_SLOW_FLASH );
 *
 *  hexin_rtos_delay_milliseconds(10 * 1000);  // let it run 10 seconds
 *
 *  hexin_led_service_stop();
 * }
 *
 */

#include "platform.h"
#include "hexin_platform.h"

#pragma once

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


/*****************************************************************************/
/** @addtogroup LED       LED
 *  @ingroup platform
 *
 * API for programmable on/off sequences for visual indications using LEDs
 *
 *
 *  @{
 */
/*****************************************************************************/


/**
 *  Predefined LED Flash patterns
 */
typedef enum
{
    HEXIN_LED_PATTERN_LED_1_SLOW_FLASH = 0,     /**< Slow flash - PLATFORM_LED_1                            */
    HEXIN_LED_PATTERN_LED_1_FAST_FLASH,         /**< Fast flash - PLATFORM_LED_1                            */
    HEXIN_LED_PATTERN_LED_1_ONE_FLASH,          /**< One fast flash then 1 second off - PLATFORM_LED_1      */
    HEXIN_LED_PATTERN_LED_1_ONE_STANDBY,        /**< One standby for Dongle - PLATFORM_LED_1                                */
    HEXIN_LED_PATTERN_LED_1_TWO_FLASH,          /**< Two fast flashes then 1 second off - PLATFORM_LED_1    */
    HEXIN_LED_PATTERN_LED_1_THREE_FLASH,        /**< Three fast flashes then 1 second off - PLATFORM_LED_1  */
    HEXIN_LED_PATTERN_LED_1_SOS,                /**< Morse code SOS - PLATFORM_LED_1                        */
    HEXIN_LED_PATTERN_LED_2_SLOW_FLASH,         /**< Slow flash - PLATFORM_LED_2                            */
    HEXIN_LED_PATTERN_LED_2_FAST_FLASH,         /**< Fast flash - PLATFORM_LED_2                            */
    HEXIN_LED_PATTERN_LED_2_ONE_FLASH,          /**< One fast flash then 1 second off - PLATFORM_LED_2      */
    HEXIN_LED_PATTERN_LED_2_TWO_FLASH,          /**< Two fast flashes then 1 second off - PLATFORM_LED_2    */
    HEXIN_LED_PATTERN_LED_2_THREE_FLASH,        /**< Three fast flashes then 1 second off - PLATFORM_LED_2  */
    HEXIN_LED_PATTERN_LED_2_SOS,                /**< Morse code SOS - PLATFORM_LED_2                        */

    HEXIN_LED_PATTERN_LED_1_LED_2_SLOW_FLASH,   /**< Slow Flashes both Platform_LED_1 and PLATFORM_LED_2    */
    HEXIN_LED_PATTERN_LED_1_LED_2_FAST_FLASH,   /**< Fast flashes both Platform_LED_1 and PLATFORM_LED_2    */
    HEXIN_LED_PATTERN_ALTERNATE_SLOW_FLASH,     /**< Slow Flashes Platform_LED_1 then PLATFORM_LED_2        */
    HEXIN_LED_PATTERN_ALTERNATE_FAST_FLASH,     /**< Fast flashes Platform_LED_1 then PLATFORM_LED_2        */

    HEXIN_LED_PATTERN_MAX                       /**< Denotes the end of the list */
} hexin_led_pattern_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/**
 *  Structure to define one state of a flashing LED sequence
 */
typedef struct
{
        hexin_led_state_t               off_on[HEXIN_LED_INDEX_MAX+1];   	/**< One entry per LED, HEXIN_LED_OFF or HEXIN_LED_ON */
        uint32_t                        state_length_ms;               		/**< How long to display this LED state */
} hexin_led_service_state_t;

typedef struct
{
        uint8_t                         num_states;                 /**< number of states in the table        */
        hexin_led_service_state_t*      led_states;                 /**< array of states, one per num_states  */
} hexin_led_service_parameters_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
/**
 * Initialize LED GPIOs for this platform
 *
 * Initializes the platform's LEDs for use and turns them off
 * To turn on/off individual LEDs use hexin_led_set_state()
 *
 * To use patterns, you must use hexin_led_service_init().
 *
 */
void hexin_led_init( void );

/**
 * Set the on/off state for the LED
 *
 * NOTE: call hexin_led_init() or hexin_led_service_init() before calling this function.
 *
 * @param      led     : LED enumeration for the platform
 * @param      off_on  : off / on state for the LED
 *
 * @return      HEXIN_SUCCESS
 *              HEXIN_ERROR
 */
hexin_result_t hexin_led_set_state(hexin_led_index_t led, hexin_led_state_t off_on );

/**
 * Initialize LED Service
 *
 * Initializes the platform's LEDs for use and turns them off
 *
 * Starts a worker thread for use in flashing LED patterns
 * Start a pattern using hexin_led_start() or hexin_led_start_pattern()
 * To turn on individual LEDs use hexin_led_set_state()
 *
 * @return  HEXIN_SUCCESS
 *          HEXIN_ERROR
 *
 */
hexin_result_t hexin_led_service_init( void );

/**
 * De-Initialize LED service and free system resources
 *
 * Calls hexin_led_service_stop().
 * After calling hexin_led_service_deinit(), you can
 * still turn on individual LEDs using hexin_led_set_state()
 *
 */
void hexin_led_service_deinit( void );

/**
 * Start the LED service
 *
 * NOTE: This is non-blocking, and will continue until
 *       user calls hexin_led_service_stop() or
 *       another call to hexin_led_service_start() or hexin_led_service_start_pattern()
 *       which will cancel the previous pattern
 *
 * @param params      : parameter structure
 *
 * @return  HEXIN_SUCCESS
 *          HEXIN_BADARG
 *          HEXIN_ERROR
 */
hexin_result_t hexin_led_service_start(const hexin_led_service_parameters_t* params);

/**
 * Start the LED service using a predefined pattern
 *
 * NOTE: This is non-blocking, and will continue until
 *       user calls hexin_led_service_stop() or
 *       another call to hexin_led_service_start() or hexin_led_service_start_pattern()
 *       which will cancel the previous pattern
 *
 * @param pattern      : which pattern to display
 *
 * @return  HEXIN_SUCCESS
 *          HEXIN_BADARG
 *          HEXIN_ERROR
 */
hexin_result_t hexin_led_service_start_pattern(hexin_led_pattern_t led_pattern);

/**
 * Stop the LED Service playback
 *
 * Sets all LEDs off
 * You must call hexin_led_service_deinit() if you want to free system resources
 *
 * @return  HEXIN_SUCCESS
 *
 */
hexin_result_t hexin_led_service_stop( void );

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

