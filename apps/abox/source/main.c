#include <stdint.h>
#include "led_service.h"
#include "hexin_freertos.h"


static hexin_thread_t           dongleMonitorthread;
uint32_t dongle_tcp_server = 1;

void do_dongle_monitor_loop( uint32_t arg )
{
	volatile unsigned char j = 0;
    while (1) {
		j++;
		hexin_rtos_delay_milliseconds( 300 );
    }
}

void application_start( void )
{
	volatile unsigned char i = 0;
	hexin_rtos_create_thread( &dongleMonitorthread,
                                            6,
                                            "Dongle monitor thread",
                                            do_dongle_monitor_loop,
                                            1024,
                                            &dongle_tcp_server );
    hexin_led_service_init();
    hexin_led_service_start_pattern( HEXIN_LED_PATTERN_LED_1_ONE_FLASH );

	//platform_gpio_init( &led, OUTPUT_PUSH_PULL );
    while (1) {
        i++;
		hexin_rtos_delay_milliseconds( 300 );	
    }
}
                  