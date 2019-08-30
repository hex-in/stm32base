#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
/******************************************************
 *  MCU Constants and Options
 *
 *  NOTE: CubeMX is used to calculate these values
 *  Older STM clock configuration tool is no longer valid
 *  http://www.st.com/web/catalog/tools/FM147/CL1794/SC961/SS1743/LN1897
 *  Refer below link for CubeMx software
 *  http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/sw_development_suite/stm32cubemx.zip
 ******************************************************/

/*  CPU clock : 96 MHz */
#define CPU_CLOCK_HZ         ( 96000000 )

/*  Use external crystal */
#define HSE_SOURCE           ( RCC_HSE_ON )

/*  AHB clock  : System clock */
#define AHB_CLOCK_DIVIDER    ( RCC_SYSCLK_DIV1 )

/*  APB1 clock : AHB clock / 2 */
#define APB1_CLOCK_DIVIDER   ( RCC_HCLK_DIV2 )

/*  APB2 clock : AHB clock  */
#define APB2_CLOCK_DIVIDER   ( RCC_HCLK_DIV1 )

/*  PLL source : external crystal */
#define PLL_SOURCE           ( RCC_PLLSource_HSE )

/*  PLL Constants */
#define PLL_M_CONSTANT       ( 13 )
#define PLL_N_CONSTANT       ( 96 )
#define PLL_P_CONSTANT       ( RCC_PLLP_DIV2 )
#define PLL_Q_CONSTANT       ( 4 )

/*  System clock source  : PLL clock */
#define SYSTEM_CLOCK_SOURCE  ( RCC_SYSCLKSOURCE_PLLCLK )

/*  SysTick clock source : AHB clock  */
#define SYSTICK_CLOCK_SOURCE ( SYSTICK_CLKSOURCE_HCLK )

/*  Internal flash wait state : 2 cycles */
#define INT_FLASH_WAIT_STATE ( FLASH_LATENCY_2 )

/*  Internal flash voltage range : 2.7 to 3.6V */
#define PLATFORM_STM32_VOLTAGE_2V7_TO_3V6

#ifdef __cplusplus
} /* extern "C" */
#endif
