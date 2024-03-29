#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "hexin_result.h"

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
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

/* HEXIN <-> RTOS API */
extern hexin_result_t hexin_rtos_init  ( void );
extern hexin_result_t hexin_rtos_deinit( void );

/* Entry point for user Application */
extern void application_start          ( void );


#ifdef __cplusplus
} /* extern "C" */
#endif
