;******************** (C) COPYRIGHT 2018 Hexin ********************
;* File Name          : startup_stm32f405xx.s
;* Author             : MCD Application Team
;* Description        : STM32F405xx devices vector table for MDK-ARM toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; 

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
				GLOBAL  __initial_sp
__initial_sp


Heap_Size       EQU     0x00010000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
                GLOBAL  __heap_base
                GLOBAL  __heap_limit
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

				 END

;************************ (C) COPYRIGHT Hexin *****END OF FILE*****
