
#include "FreeRTOS.h"
#include "task.h"
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

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t  xIdleStack[configMINIMAL_STACK_SIZE];

/******************************************************
 *               Variable Definitions
 ******************************************************/


/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 *  Main function - starts ThreadX
 *  Called from the crt0_KEIL reset_handler function
 *
 */

int main( void )
{
	volatile unsigned char i = 0;
    while (1) {
        i++;
    }
}

//static void application_thread_main( void *arg )
//{
//    UNUSED_PARAMETER( arg );
//    application_start( );

//    malloc_leak_check(NULL, LEAK_CHECK_THREAD);
//    vTaskDelete( NULL );
//}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
