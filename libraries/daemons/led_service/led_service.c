#include <string.h>
//#include "hexin.h"
#include "led_service.h"
#include "hexin_freertos.h"
#include "hexin_defaults.h"
#include "hexin_platform.h"
#include "platform_mcu_peripheral.h"

#ifndef GPIO_LED_NOT_SUPPORTED

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define LED_WORKER_STACK_SIZE               ( 1024 )
#define LED_WORKER_QUEUE_SIZE               (    4 )

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct
{
    hexin_worker_thread_t           worker_thread;
    hexin_bool_t                    worker_thread_started;

    hexin_timer_t                   led_timer;          /* timer to send asynch message to thread */
    hexin_bool_t                    led_timer_inited;

    hexin_led_service_parameters_t  params;
    int                             curr_state;         /* current state in the LED state table                         */
} led_service_context_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/
static hexin_result_t hexin_led_worker_thread_callback( void *arg);
static void led_timer_event_handler( void* arg );

/******************************************************
 *               Variable Definitions
 ******************************************************/

static led_service_context_t* led_ctx;

/* Predefined patterns */
static hexin_led_service_state_t  led_pattern_slow_led1[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 1000 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 1000 },
};

/* fast green blink */
static hexin_led_service_state_t  led_pattern_fast_led1[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 100 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 100 },

};

static hexin_led_service_state_t  led_pattern_one_led1[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_one_led1_standby[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 200 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 200 },
};

static hexin_led_service_state_t  led_pattern_two_led1[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_three_led1[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF }, 250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_led1_sos[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  500 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_slow_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON }, 500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 200 },
};

static hexin_led_service_state_t  led_pattern_fast_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
};

static hexin_led_service_state_t  led_pattern_one_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON }, 250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_two_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_three_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_led2_sos[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_ON },  500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  500 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF},  100 },
    { {HEXIN_LED_OFF, HEXIN_LED_ON },  250 },

    { {HEXIN_LED_OFF, HEXIN_LED_OFF}, 1000 },
};

static hexin_led_service_state_t  led_pattern_slow_both[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_ON  }, 500 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 200 },

};

/* fast green blink */
static hexin_led_service_state_t  led_pattern_fast_both[] =
{
    { {HEXIN_LED_ON,  HEXIN_LED_ON },  200 },
    { {HEXIN_LED_OFF, HEXIN_LED_OFF }, 100 },

};

static hexin_led_service_state_t  led_pattern_slow_led1_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON }, 500 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF}, 500 },
};

static hexin_led_service_state_t  led_pattern_fast_led1_led2[] =
{
    { {HEXIN_LED_OFF, HEXIN_LED_ON }, 250 },
    { {HEXIN_LED_ON,  HEXIN_LED_OFF}, 250 },
};

static const hexin_led_service_parameters_t predefined_led_flash_patterns[] =
{
        [ HEXIN_LED_PATTERN_LED_1_SLOW_FLASH ] =
        {
            .led_states = led_pattern_slow_led1,
            .num_states = sizeof(led_pattern_slow_led1) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_FAST_FLASH ] =
        {
            .led_states = led_pattern_fast_led1,
            .num_states = sizeof(led_pattern_fast_led1) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_ONE_FLASH ] =
        {
            .led_states = led_pattern_one_led1,
            .num_states = sizeof(led_pattern_one_led1) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_ONE_STANDBY ] =
        {
            .led_states = led_pattern_one_led1_standby,
            .num_states = sizeof(led_pattern_one_led1_standby) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_TWO_FLASH ] =
        {
            .led_states = led_pattern_two_led1,
            .num_states = sizeof(led_pattern_two_led1) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_THREE_FLASH ] =
        {
            .led_states = led_pattern_three_led1,
            .num_states = sizeof(led_pattern_three_led1) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_SOS ] =
        {
            .led_states = led_pattern_led1_sos,
            .num_states = sizeof(led_pattern_led1_sos) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_SLOW_FLASH ] =
        {
            .led_states = led_pattern_slow_led2,
            .num_states = sizeof(led_pattern_slow_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_FAST_FLASH ] =
        {
            .led_states = led_pattern_fast_led2,
            .num_states = sizeof(led_pattern_fast_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_ONE_FLASH ] =
        {
            .led_states = led_pattern_one_led2,
            .num_states = sizeof(led_pattern_one_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_TWO_FLASH ] =
        {
            .led_states = led_pattern_two_led2,
            .num_states = sizeof(led_pattern_two_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_THREE_FLASH ] =
        {
            .led_states = led_pattern_three_led2,
            .num_states = sizeof(led_pattern_three_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_2_SOS ] =
        {
            .led_states = led_pattern_led2_sos,
            .num_states = sizeof(led_pattern_led2_sos) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_LED_2_SLOW_FLASH ] =
        {
            .led_states = led_pattern_slow_both,
            .num_states = sizeof(led_pattern_slow_both) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_LED_1_LED_2_FAST_FLASH ] =
        {
            .led_states = led_pattern_fast_both,
            .num_states = sizeof(led_pattern_fast_both) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_ALTERNATE_SLOW_FLASH ] =
        {
            .led_states = led_pattern_slow_led1_led2,
            .num_states = sizeof(led_pattern_slow_led1_led2) / sizeof(hexin_led_service_state_t)
        },
        [ HEXIN_LED_PATTERN_ALTERNATE_FAST_FLASH ] =
        {
            .led_states = led_pattern_fast_led1_led2,
            .num_states = sizeof(led_pattern_fast_led1_led2) / sizeof(hexin_led_service_state_t)
        },
};
/******************************************************
 *               Internal Function Definitions
 ******************************************************/

/******************************************************
 *               Timer Event Handler
 ******************************************************/
static void hexin_led_stop_timer(led_service_context_t*  led_ctx)
{
    if (led_ctx->led_timer_inited == HEXIN_TRUE)
    {
        hexin_rtos_deinit_timer( &led_ctx->led_timer );
        led_ctx->led_timer_inited = HEXIN_FALSE;
    }
}

static void hexin_led_start_timer(led_service_context_t*  led_ctx)
{
    hexin_led_stop_timer(led_ctx);
    if (hexin_rtos_init_timer( &led_ctx->led_timer, led_ctx->params.led_states[led_ctx->curr_state].state_length_ms,
                           led_timer_event_handler, (void*) led_ctx ) == HEXIN_SUCCESS)
    {
        led_ctx->led_timer_inited = HEXIN_TRUE;
        hexin_rtos_start_timer( &led_ctx->led_timer );
    }
}

static hexin_result_t hexin_led_worker_thread_callback( void *arg)
{
    led_service_context_t*  led_ctx = (led_service_context_t*)arg;
    int                     led;

    if (led_ctx == NULL)
    {
        return HEXIN_BADARG;
    }

    /* move on to next state */
    led_ctx->curr_state++;
    if (led_ctx->curr_state >= led_ctx->params.num_states)
    {
        /* reset to state 0 */
        led_ctx->curr_state = 0;
    }

    /* turn on/off LEDs */
    for (led = 0; led < HEXIN_LED_INDEX_MAX; led++)
    {
        hexin_led_set_state(led, led_ctx->params.led_states[led_ctx->curr_state].off_on[led] );
    }

    /* restart timer (we may have a new value for the timer) */
    hexin_led_start_timer(led_ctx);

    return HEXIN_SUCCESS;
}

static void led_timer_event_handler( void* arg )
{
    led_service_context_t*  led_ctx = (led_service_context_t*)arg;

    if ((led_ctx != NULL) && (led_ctx->worker_thread_started == HEXIN_TRUE))
    {
        /* send async event to worker thread */
        hexin_rtos_send_asynchronous_event( &led_ctx->worker_thread, hexin_led_worker_thread_callback, led_ctx );
    }
}

/******************************************************
 *               External Function Definitions
 ******************************************************/

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
 *          HEXIN_ERROR     - unable to start worker thread or timer
 *
 */
hexin_result_t hexin_led_service_init( void )
{
    int             led;
    hexin_result_t  result;

    if (led_ctx != NULL)
    {
        return HEXIN_ERROR;
    }

    led_ctx = (led_service_context_t*)calloc_named("led_service", 1, sizeof(led_service_context_t));
    if (led_ctx == NULL)
    {
        return HEXIN_ERROR;
    }
    memset(led_ctx, 0x00, sizeof(led_service_context_t));

    /* start the worker thread */
    result = hexin_rtos_create_worker_thread( &led_ctx->worker_thread, HEXIN_DEFAULT_WORKER_PRIORITY, LED_WORKER_STACK_SIZE, LED_WORKER_QUEUE_SIZE );
    if (result != HEXIN_SUCCESS)
    {
        goto _led_service_init_fail;
    }
    led_ctx->worker_thread_started = HEXIN_TRUE;

    /* Turn off LEDs */
    for (led = 0; led < HEXIN_LED_INDEX_MAX; led++)
    {
        hexin_led_set_state( led, HEXIN_LED_OFF );
    }
    return HEXIN_SUCCESS;

_led_service_init_fail:

    if (led_ctx != NULL)
    {
        free(led_ctx);
    }
    led_ctx = NULL;

    return HEXIN_ERROR;
}

/**
 * De-Initialize LED service
 *
 * Deletes the worker thread and turns off the LEDs
 * After calling hexin_led_service_deinit(), you can
 * still turn on individual LEDs using hexin_led_set_state()
 *
 */
void hexin_led_service_deinit( void )
{
    if (led_ctx == NULL)
    {
        return;
    }

    hexin_led_service_stop();

    if (led_ctx->worker_thread_started == HEXIN_TRUE)
    {
        hexin_rtos_delete_worker_thread( &led_ctx->worker_thread );
        led_ctx->worker_thread_started = HEXIN_FALSE;
    }
    free(led_ctx);
    led_ctx = NULL;
}

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
hexin_result_t hexin_led_service_start(const hexin_led_service_parameters_t* params)
{
    int             led;

    if (params == NULL)
    {
        return HEXIN_BADARG;
    }

    /* Do we have an LED context ? */
    if (led_ctx == NULL)
    {
        return HEXIN_ERROR;
    }

    /* stop any currently running timer */
    hexin_led_stop_timer(led_ctx);

    /* copy over parameters */
    memcpy(&led_ctx->params, params, sizeof(hexin_led_service_parameters_t));

    /* turn on/off LEDs for first state */
    led_ctx->curr_state = 0;
    for ( led = 0; led < HEXIN_LED_INDEX_MAX; led++ ) {
        hexin_led_set_state( led, led_ctx->params.led_states[led_ctx->curr_state].off_on[led] );
    }

    /* initialize and start the timer */
    hexin_led_start_timer(led_ctx);

    return HEXIN_SUCCESS;

}

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
hexin_result_t hexin_led_service_start_pattern( hexin_led_pattern_t led_pattern )
{
    if ( led_pattern < HEXIN_LED_PATTERN_MAX ) {
        return hexin_led_service_start( &predefined_led_flash_patterns[led_pattern] );
    }

    return HEXIN_BADARG;
}

/**
 * Stop the LED Service playback
 *
 * NOTES:   Sets all LEDs off
 *          You must call hexin_led_service_deinit() if you want to free system resources
 *
 * @return  HEXIN_SUCCESS
 */
hexin_result_t hexin_led_service_stop( void )
{
    int led;

    if (led_ctx != NULL)
    {
        hexin_led_stop_timer(led_ctx);
    }

    /* Turn off LEDs */
    for (led = 0; led < HEXIN_LED_INDEX_MAX; led++)
    {
        hexin_led_set_state(led, HEXIN_LED_OFF );
    }

    return HEXIN_SUCCESS;
}

#else
/* dummy functions for when there are no LEDs defined */
hexin_result_t hexin_led_service_init( void )
{
    return HEXIN_ERROR;
}
void hexin_led_service_deinit( void )
{
}
hexin_result_t hexin_led_service_start(const hexin_led_service_parameters_t* params)
{
    return HEXIN_ERROR;
}
hexin_result_t hexin_led_service_start_pattern(hexin_led_pattern_t led_pattern)
{
    return HEXIN_ERROR;
}
hexin_result_t hexin_led_service_stop( void )
    return HEXIN_ERROR;
}
#endif  /* ifndef GPIO_LED_NOT_SUPPORTED */
