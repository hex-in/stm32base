#include "stdint.h"
#include "string.h"
#include "platform_peripheral.h"
#include "platform_isr.h"
#include "platform_isr_interface.h"
#include "hexin_assert.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_ll_system.h"

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

/* Structure of runtime GPIO IRQ data */
typedef struct
{
    platform_gpio_port_t*        owner_port; // GPIO port owning the IRQ line (line is shared across all GPIO ports)
    platform_gpio_irq_callback_t handler;    // User callback
    void*                        arg;        // User argument to be passed to the callbackA
} platform_gpio_irq_data_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/* GPIO peripheral clocks */
static const uint32_t gpio_peripheral_clocks[NUMBER_OF_GPIO_PORTS] =
{
    [0] = ((uint32_t)0x00000001),   // RCC_AHB1Periph_GPIOA
    [1] = ((uint32_t)0x00000002),   // RCC_AHB1Periph_GPIOB
    [2] = ((uint32_t)0x00000004),   // RCC_AHB1Periph_GPIOC
    [3] = ((uint32_t)0x00000008),   // RCC_AHB1Periph_GPIOD
    [4] = ((uint32_t)0x00000010),   // RCC_AHB1Periph_GPIOE
    [5] = ((uint32_t)0x00000020),   // RCC_AHB1Periph_GPIOF
    [6] = ((uint32_t)0x00000040),   // RCC_AHB1Periph_GPIOG
    [7] = ((uint32_t)0x00000080),   // RCC_AHB1Periph_GPIOH
};

/* Runtime GPIO IRQ data */
static volatile platform_gpio_irq_data_t gpio_irq_data[NUMBER_OF_GPIO_IRQ_LINES];

/******************************************************
 *            Platform Function Definitions
 ******************************************************/

platform_result_t platform_gpio_init( const platform_gpio_t* gpio, platform_pin_config_t config )
{
    GPIO_InitTypeDef  gpio_init_structure = { 0 };
    uint32_t          port_number;

    hexin_assert( "bad argument", ( gpio != NULL ) );

    port_number = platform_gpio_get_port_number( gpio->port );

    /* Enable peripheral clock for this port */
    RCC->AHB1ENR |= gpio_peripheral_clocks[port_number];

    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    switch( config ){
        case INPUT_PULL_UP:
            gpio_init_structure.Mode = GPIO_MODE_INPUT;
            gpio_init_structure.Pull = GPIO_PULLUP;
            break;

        case INPUT_PULL_DOWN:
            gpio_init_structure.Mode = GPIO_MODE_INPUT;
            gpio_init_structure.Pull = GPIO_PULLDOWN;
            break;

        case INPUT_HIGH_IMPEDANCE:
            gpio_init_structure.Mode = GPIO_MODE_INPUT;
            gpio_init_structure.Pull = GPIO_NOPULL;
            break;

        case OUTPUT_PUSH_PULL:
            gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
            gpio_init_structure.Pull = GPIO_PULLUP;
            break;

        case OUTPUT_OPEN_DRAIN_NO_PULL:
            gpio_init_structure.Mode = GPIO_MODE_OUTPUT_OD;
            gpio_init_structure.Pull = GPIO_NOPULL;
            break;
        
        case OUTPUT_OPEN_DRAIN_PULL_UP:
            gpio_init_structure.Mode = GPIO_MODE_OUTPUT_OD;
            gpio_init_structure.Pull = GPIO_PULLUP;
            break;

        default:
            gpio_init_structure.Mode = GPIO_MODE_INPUT;
            gpio_init_structure.Pull = GPIO_NOPULL;
            break;
    }

    gpio_init_structure.Pin = (uint32_t) ( 1 << gpio->pin_number );

    HAL_GPIO_Init( gpio->port, &gpio_init_structure );


    return PLATFORM_SUCCESS;
}

platform_result_t platform_gpio_deinit( const platform_gpio_t* gpio )
{
    hexin_assert( "bad argument", ( gpio != NULL ) );

    /* Configure IO Direction in Input Floating Mode */
    HAL_GPIO_DeInit( gpio->port, (uint32_t)( gpio->pin_number ) );

    return PLATFORM_SUCCESS;
}

platform_result_t platform_gpio_output_high( const platform_gpio_t* gpio )
{
    hexin_assert( "bad argument", ( gpio != NULL ) );
	
	gpio->port->BSRR = (uint16_t) ( 1 << gpio->pin_number );

    return PLATFORM_SUCCESS;
}

platform_result_t platform_gpio_output_low( const platform_gpio_t* gpio )
{
    hexin_assert( "bad argument", ( gpio != NULL ) );

    gpio->port->BSRR = ((uint32_t)( 1 << gpio->pin_number ) ) << 16U;

    return PLATFORM_SUCCESS;
}

hexin_bool_t platform_gpio_input_get( const platform_gpio_t* gpio )
{
    hexin_bool_t result;

    hexin_assert( "bad argument", ( gpio != NULL ) );

    result = ( ( gpio->port->IDR & (uint32_t) ( gpio->pin_number ) ) != 0 ) ? HEXIN_TRUE : HEXIN_FALSE;

    return result;
}

platform_result_t platform_gpio_toggle( const platform_gpio_t* gpio )
{
    hexin_assert( "bad argument", ( gpio != NULL ) );

    gpio->port->ODR ^= (uint32_t) ( gpio->pin_number );

    return PLATFORM_SUCCESS;
}

// platform_result_t platform_gpio_irq_enable( const platform_gpio_t* gpio, platform_gpio_irq_trigger_t trigger, platform_gpio_irq_callback_t handler, void* arg )
// {
//     // uint32_t            interrupt_line;
//     // EXTITrigger_TypeDef exti_trigger;

//     // hexin_assert( "bad argument", ( gpio != NULL ) && ( handler != NULL ) );

//     // interrupt_line = (uint32_t) ( 1 << gpio->pin_number );

//     // switch ( trigger )
//     // {
//     //     case IRQ_TRIGGER_RISING_EDGE:
//     //     {
//     //         exti_trigger = EXTI_Trigger_Rising;
//     //         break;
//     //     }
//     //     case IRQ_TRIGGER_FALLING_EDGE:
//     //     {
//     //         exti_trigger = EXTI_Trigger_Falling;
//     //         break;
//     //     }
//     //     case IRQ_TRIGGER_BOTH_EDGES:
//     //     {
//     //         exti_trigger = EXTI_Trigger_Rising_Falling;
//     //         break;
//     //     }
//     //     default:
//     //     {
//     //         return PLATFORM_BADARG;
//     //     }
//     // }

//     // platform_mcu_powersave_disable();

//     // if ( ( EXTI->IMR & interrupt_line ) == 0 )
//     // {
//     //     EXTI_InitTypeDef exti_init_structure;
//     //     IRQn_Type        interrupt_vector = ( IRQn_Type ) 0;

//     //     SYSCFG_EXTILineConfig( platform_gpio_get_port_number( gpio->port ), gpio->pin_number );

//     //     exti_init_structure.EXTI_Trigger = exti_trigger;
//     //     exti_init_structure.EXTI_Line    = interrupt_line;
//     //     exti_init_structure.EXTI_Mode    = EXTI_Mode_Interrupt;
//     //     exti_init_structure.EXTI_LineCmd = ENABLE;
//     //     EXTI_Init( &exti_init_structure );

//     //     if ( ( interrupt_line & 0x001F ) != 0 )
//     //     {
//     //         /* Line 0 to 4 */
//     //         interrupt_vector = (IRQn_Type) ( EXTI0_IRQn + gpio->pin_number );
//     //     }
//     //     else if ( ( interrupt_line & 0x03E0 ) != 0 )
//     //     {
//     //         /* Line 5 to 9 */
//     //         interrupt_vector = EXTI9_5_IRQn;
//     //     }
//     //     else if ( ( interrupt_line & 0xFC00 ) != 0 )
//     //     {
//     //         /* Line 10 to 15 */
//     //         interrupt_vector = EXTI15_10_IRQn;
//     //     }

//     //     /* Must be lower priority than the value of configMAX_SYSCALL_INTERRUPT_PRIORITY otherwise FreeRTOS will not be able to mask the interrupt */
//     //     NVIC_EnableIRQ( interrupt_vector );

//     //     gpio_irq_data[gpio->pin_number].owner_port = gpio->port;
//     //     gpio_irq_data[gpio->pin_number].handler    = handler;
//     //     gpio_irq_data[gpio->pin_number].arg        = arg;

//     //     platform_mcu_powersave_enable();

//     //     return PLATFORM_SUCCESS;
//     // }

//     // platform_mcu_powersave_enable();

//     return PLATFORM_NO_EFFECT;
// }

// platform_result_t platform_gpio_irq_disable( const platform_gpio_t* gpio )
// {
//     // uint16_t interrupt_line;

//     // hexin_assert( "bad argument", ( gpio != NULL ) );

//     // interrupt_line = (uint16_t) ( 1 << gpio->pin_number );

//     // platform_mcu_powersave_disable();

//     // if ( ( EXTI->IMR & interrupt_line ) && gpio_irq_data[gpio->pin_number].owner_port == gpio->port )
//     // {
//     //     hexin_bool_t     interrupt_line_used = WICED_FALSE;
//     //     IRQn_Type        interrupt_vector    = ( IRQn_Type ) 0;
//     //     EXTI_InitTypeDef exti_init_structure;

//     //     /* Disable EXTI interrupt line */
//     //     exti_init_structure.EXTI_Line    = (uint32_t)interrupt_line;
//     //     exti_init_structure.EXTI_LineCmd = DISABLE;
//     //     exti_init_structure.EXTI_Mode    = EXTI_Mode_Interrupt;
//     //     exti_init_structure.EXTI_Trigger = EXTI_Trigger_Rising;
//     //     EXTI_Init( &exti_init_structure );
//     //     exti_init_structure.EXTI_Trigger = EXTI_Trigger_Falling;
//     //     EXTI_Init( &exti_init_structure );

//     //     /* Disable NVIC interrupt */
//     //     if ( ( interrupt_line & 0x001F ) != 0 )
//     //     {
//     //         /* Line 0 to 4 */
//     //         interrupt_vector = (IRQn_Type) ( EXTI0_IRQn + gpio->pin_number );
//     //         interrupt_line_used = WICED_FALSE;
//     //     }
//     //     else if ( ( interrupt_line & 0x03E0 ) != 0 )
//     //     {
//     //         /* Line 5 to 9 */
//     //         interrupt_vector = EXTI9_5_IRQn;
//     //         interrupt_line_used = ( ( EXTI->IMR & 0x3e0U ) != 0 ) ? WICED_TRUE : WICED_FALSE;
//     //     }
//     //     else if ( ( interrupt_line & 0xFC00 ) != 0 )
//     //     {
//     //         /* Line 10 to 15 */
//     //         interrupt_vector = EXTI15_10_IRQn;
//     //         interrupt_line_used = ( ( EXTI->IMR & 0xfc00U ) != 0 ) ? WICED_TRUE : WICED_FALSE;
//     //     }

//     //     /* Some IRQ lines share a vector. Disable vector only if not used */
//     //     if ( interrupt_line_used == WICED_FALSE )
//     //     {
//     //         NVIC_DisableIRQ( interrupt_vector );
//     //     }

//     //     gpio_irq_data[gpio->pin_number].owner_port = 0;
//     //     gpio_irq_data[gpio->pin_number].handler    = 0;
//     //     gpio_irq_data[gpio->pin_number].arg        = 0;

//     //     platform_mcu_powersave_enable();

//     //     return PLATFORM_SUCCESS;
//     // }

//     // platform_mcu_powersave_enable();

//     return PLATFORM_NO_EFFECT;
// }

// platform_result_t platform_gpio_deepsleep_wakeup_enable( const platform_gpio_t* gpio, platform_gpio_irq_trigger_t trigger )
// {
//     return PLATFORM_UNSUPPORTED;
// }

// /******************************************************
//  *      STM32F2xx Internal Function Definitions
//  ******************************************************/

// platform_result_t platform_gpio_irq_manager_init( void )
// {
//     // memset( (void*)gpio_irq_data, 0, sizeof( gpio_irq_data ) );

//     // /* Switch on SYSCFG peripheral clock to allow writing into SYSCFG registers */
//     // RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE );

//     return PLATFORM_SUCCESS;
// }

// platform_result_t platform_gpio_set_alternate_function( platform_gpio_port_t* gpio_port, uint8_t pin_number, GPIOOType_TypeDef output_type, GPIOPuPd_TypeDef pull_up_down_type, uint8_t alternation_function )
// {
//     uint32_t            interrupt_line;
//     LL_EXTI_InitTypeDef exti_trigger;

//     hexin_assert( "bad argument", ( gpio != NULL ) && ( handler != NULL ) );

//     interrupt_line = (uint32_t) ( gpio->pin_number );

//     switch ( trigger ) {
//         case IRQ_TRIGGER_RISING_EDGE:
//             exti_trigger.Trigger = LL_EXTI_TRIGGER_RISING;
//             break;

//         case IRQ_TRIGGER_FALLING_EDGE:
//             exti_trigger.Trigger = LL_EXTI_TRIGGER_FALLING;
//             break;

//         case IRQ_TRIGGER_BOTH_EDGES:
//             exti_trigger.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
//             break;

//         default:
//             return PLATFORM_BADARG;
//     }

//     platform_mcu_powersave_disable();

//     if ( ( EXTI->IMR & interrupt_line ) == 0 )
//     {
//         LL_EXTI_InitTypeDef exti_init_structure;
//         IRQn_Type           interrupt_vector = ( IRQn_Type ) 0;

//         // SYSCFG_EXTILineConfig( platform_gpio_get_port_number( gpio->port ), gpio->pin_number );

//     //     exti_init_structure.EXTI_Trigger = exti_trigger;
//     //     exti_init_structure.EXTI_Line    = interrupt_line;
//     //     exti_init_structure.EXTI_Mode    = EXTI_Mode_Interrupt;
//     //     exti_init_structure.EXTI_LineCmd = ENABLE;
//     //     EXTI_Init( &exti_init_structure );

//     //     if ( ( interrupt_line & 0x001F ) != 0 )
//     //     {
//     //         /* Line 0 to 4 */
//     //         interrupt_vector = (IRQn_Type) ( EXTI0_IRQn + gpio->pin_number );
//     //     }
//     //     else if ( ( interrupt_line & 0x03E0 ) != 0 )
//     //     {
//     //         /* Line 5 to 9 */
//     //         interrupt_vector = EXTI9_5_IRQn;
//     //     }
//     //     else if ( ( interrupt_line & 0xFC00 ) != 0 )
//     //     {
//     //         /* Line 10 to 15 */
//     //         interrupt_vector = EXTI15_10_IRQn;
//     //     }

//     //     /* Must be lower priority than the value of configMAX_SYSCALL_INTERRUPT_PRIORITY otherwise FreeRTOS will not be able to mask the interrupt */
//     //     NVIC_EnableIRQ( interrupt_vector );

//     //     gpio_irq_data[gpio->pin_number].owner_port = gpio->port;
//     //     gpio_irq_data[gpio->pin_number].handler    = handler;
//     //     gpio_irq_data[gpio->pin_number].arg        = arg;

//         platform_mcu_powersave_enable();

//         return PLATFORM_SUCCESS;
//     }

//     platform_mcu_powersave_enable();

//     return PLATFORM_NO_EFFECT;
// }

uint32_t platform_gpio_get_port_number( platform_gpio_port_t* gpio_port )
{
    switch ( (uint32_t) gpio_port )
    {
        case GPIOA_BASE:
            return LL_SYSCFG_EXTI_PORTA;
        case GPIOB_BASE:
            return LL_SYSCFG_EXTI_PORTB;
        case GPIOC_BASE:
            return LL_SYSCFG_EXTI_PORTC;
        case GPIOD_BASE:
            return LL_SYSCFG_EXTI_PORTD;
        case GPIOE_BASE:
            return LL_SYSCFG_EXTI_PORTE;
        case GPIOF_BASE:
            return LL_SYSCFG_EXTI_PORTF;
        case GPIOG_BASE:
            return LL_SYSCFG_EXTI_PORTG;
        case GPIOH_BASE:
            return LL_SYSCFG_EXTI_PORTH;
        case GPIOI_BASE:
            return LL_SYSCFG_EXTI_PORTI;
        default:
            return INVALID_GPIO_PORT_NUMBER;
    }
}

// /******************************************************
//  *               IRQ Handler Definitions
//  ******************************************************/

// /* Common IRQ handler for all GPIOs */
// // WWD_RTOS_DEFINE_ISR( gpio_irq )
// // {
// //     uint32_t active_interrupt_vector = (uint32_t) ( ( SCB->ICSR & 0x3fU ) - 16 );
// //     uint32_t gpio_number;
// //     uint32_t interrupt_line;

// //     switch ( active_interrupt_vector )
// //     {
// //         case EXTI0_IRQn:
// //             interrupt_line = EXTI_Line0;
// //             gpio_number = 0;
// //             break;
// //         case EXTI1_IRQn:
// //             interrupt_line = EXTI_Line1;
// //             gpio_number = 1;
// //             break;
// //         case EXTI2_IRQn:
// //             interrupt_line = EXTI_Line2;
// //             gpio_number = 2;
// //             break;
// //         case EXTI3_IRQn:
// //             interrupt_line = EXTI_Line3;
// //             gpio_number = 3;
// //             break;
// //         case EXTI4_IRQn:
// //             interrupt_line = EXTI_Line4;
// //             gpio_number = 4;
// //             break;
// //         case EXTI9_5_IRQn:
// //             interrupt_line = EXTI_Line5;
// //             for ( gpio_number = 5; gpio_number < 10 && ( EXTI->PR & interrupt_line ) == 0; gpio_number++ )
// //             {
// //                 interrupt_line <<= 1;
// //             }
// //             hexin_assert( "interrupt pin not found", gpio_number < 10 );
// //             break;
// //         case EXTI15_10_IRQn:
// //             interrupt_line = EXTI_Line10;
// //             for ( gpio_number = 10; gpio_number < 16 && ( EXTI->PR & interrupt_line ) == 0; gpio_number++ )
// //             {
// //                 interrupt_line <<= 1;
// //             }
// //             hexin_assert( "interrupt pin not found", gpio_number < 16 );
// //             break;
// //         default:
// //             return;
// //     }

// //     /* Clear interrupt flag */
// //     EXTI->PR = interrupt_line;

// //     /* Call the respective GPIO interrupt handler/callback */
// //     if ( gpio_irq_data[gpio_number].handler != NULL )
// //     {
// //         void * arg = gpio_irq_data[gpio_number].arg; /* Avoids undefined order of access to volatiles */
// //         gpio_irq_data[gpio_number].handler( arg );
// //     }
// // }

// /******************************************************
//  *               IRQ Handler Mapping
//  ******************************************************/

// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI0_irq     )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI1_irq     )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI2_irq     )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI3_irq     )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI4_irq     )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI9_5_irq   )
// // WWD_RTOS_MAP_ISR( gpio_irq , EXTI15_10_irq )
