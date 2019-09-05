/** @file
 * Define default STM32F4xx initialisation functions
 */
#include "platform_init.h"
#include "platform_isr.h"
#include "platform_peripheral.h"
#include "platform_config.h"
#include "platform_toolchain.h"

#include "stm32f4xx_hal_cortex.h"

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
TIM_HandleTypeDef        htim1;
/******************************************************
 *               Function Definitions
 ******************************************************/
void _Error_Handler( char *file, int line )
{
    /* Loop forever */
    while(1)
    {
    }
}

void platform_mcu_reset( void )
{
    NVIC_SystemReset( );

    /* Loop forever */
    while ( 1 )
    {
    }
}

HAL_StatusTypeDef platform_hal_init_tick( uint32_t TickPriority )
{
	RCC_ClkInitTypeDef    clkconfig;
	uint32_t              uwTimclock = 0;
	uint32_t              uwPrescalerValue = 0;
	uint32_t              pFLatency;

	/*Configure the TIM1 IRQ priority */
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, TickPriority ,0); 

	/* Enable the TIM1 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); 

	/* Enable TIM1 clock */
	__HAL_RCC_TIM1_CLK_ENABLE();

	/* Get clock configuration */
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	/* Compute TIM1 clock */
	uwTimclock = 2*HAL_RCC_GetPCLK2Freq();

	/* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
	uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000) - 1);

	/* Initialize TIM1 */
	htim1.Instance = TIM1;

	htim1.Init.Period = (1000000 / 1000) - 1;
	htim1.Init.Prescaler = uwPrescalerValue;
	htim1.Init.ClockDivision = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	if( HAL_TIM_Base_Init(&htim1) == HAL_OK ) {
	/* Start the TIM time Base generation in interrupt mode */
		return HAL_TIM_Base_Start_IT(&htim1);
	}

	/* Return function status */
	return HAL_ERROR;
	
}

void platform_hal_init( void )
{
  /* Configure Flash prefetch, Instruction cache, Data cache */ 
#if (INSTRUCTION_CACHE_ENABLE != 0U)
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
#endif /* INSTRUCTION_CACHE_ENABLE */

#if (DATA_CACHE_ENABLE != 0U)
	__HAL_FLASH_DATA_CACHE_ENABLE();
#endif /* DATA_CACHE_ENABLE */

#if (PREFETCH_ENABLE != 0U)
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

	/* Set Interrupt Group Priority */
	HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

	/* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
	platform_hal_init_tick( TICK_INT_PRIORITY );

	/* Init the low level hardware */
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_NVIC_SetPriority( PendSV_IRQn, 15, 0 );
}

WEAK void platform_init_system_clocks( void )
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	platform_hal_init();
	
    /* Configure Clocks */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the CPU, AHB and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = HSE_SOURCE;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = PLL_M_CONSTANT;
    RCC_OscInitStruct.PLL.PLLN = PLL_N_CONSTANT;
    RCC_OscInitStruct.PLL.PLLP = PLL_P_CONSTANT;
    RCC_OscInitStruct.PLL.PLLQ = PLL_Q_CONSTANT;
#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) ||\
    defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx) ||\
    defined(STM32F413xx) || defined(STM32F423xx)
    RCC_OscInitStruct.PLL.PLLR = PLL_R_CONSTANT;
#endif
    if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK ) {
        _Error_Handler(__FILE__, __LINE__);
    }

    /* Initializes the CPU, AHB and APB busses clocks  */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = SYSTEM_CLOCK_SOURCE;
    RCC_ClkInitStruct.AHBCLKDivider  = AHB_CLOCK_DIVIDER;
    RCC_ClkInitStruct.APB1CLKDivider = APB1_CLOCK_DIVIDER;
    RCC_ClkInitStruct.APB2CLKDivider = APB2_CLOCK_DIVIDER;

    if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, INT_FLASH_WAIT_STATE ) != HAL_OK ) {
        _Error_Handler(__FILE__, __LINE__);
    }

}

WEAK void platform_init_memory( void )
{

}

void platform_init_mcu_infrastructure( void )
{
    uint8_t i;

#ifdef INTERRUPT_VECTORS_IN_RAM
    SCB->VTOR = 0x20000000; /* Change the vector table to point to start of SRAM */
#endif /* ifdef INTERRUPT_VECTORS_IN_RAM */

    /* Initialise interrupt priorities */
    for ( i = 0; i < 81; i++ ) {
        NVIC_SetPriority( (IRQn_Type) i, 0x0F );
    }

    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    platform_init_rtos_irq_priorities();

}

WEAK void platform_init_external_devices( void )
{

}
