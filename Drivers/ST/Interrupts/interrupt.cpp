/**
 * @file interrupt.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-09
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "stm32f10x_it.h"
#pragma language = extended
#pragma segment = "CSTACK"

extern "C" void __iar_program_start(void);

typedef void (*intfunc)(void);
typedef union {
  intfunc __fun;
  void *__ptr;
} intvec_elem;

// The vector table is normally located at address 0.
// When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
// If you need to define interrupt service routines,
// make a copy of this file and include it in your project.
// The name "__vector_table" has special meaning for C-SPY:
// it is where the SP start value is found, and the NVIC vector
// table register (VTOR) is initialized to this address if != 0.

#pragma location = ".intvec"
extern "C" const intvec_elem __vector_table[] =
    {
        {.__ptr = __sfe("CSTACK")},
        ResetHandler,        //                  ; Reset Handler
        NMIException,        //                  ; NMI Handler
        HardFaultException,  //            ; Hard Fault Handler
        MemManageException,  //            ; MPU Fault Handler
        BusFaultException,   //             ; Bus Fault Handler
        UsageFaultException, //           ; Usage Fault Handler
        0,                   //                             ; Reserved
        0,                   //                             ; Reserved
        0,                   //                             ; Reserved
        0,                   //                             ; Reserved
        SVCHandler,          //                    ; SVCall Handler
        DebugMonitor,        //                  ; Debug Monitor Handler
        0,                   //                             ; Reserved
        PendSVC,             //                       ; PendSV Handler
        SysTick_Handler,     //               ; SysTick Handler

        // ; External Interrupts
        WWDG_IRQHandler,               //               ; Window Watchdog
        PVD_IRQHandler,                //                ; PVD through EXTI Line detect
        TAMPER_IRQHandler,             //             ; Tamper
        RTC_IRQHandler,                //                ; RTC
        FLASH_IRQHandler,              //              ; Flash
        RCC_IRQHandler,                //                ; RCC
        EXTI0_IRQHandler,              //              ; EXTI Line 0
        EXTI1_IRQHandler,              //              ; EXTI Line 1
        EXTI2_IRQHandler,              //              ; EXTI Line 2
        EXTI3_IRQHandler,              //              ; EXTI Line 3
        EXTI4_IRQHandler,              //              ; EXTI Line 4
        DMA1_Channel1_IRQHandler,      //      ; DMA1 Channel 1
        DMA1_Channel2_IRQHandler,      //      ; DMA1 Channel 2
        DMA1_Channel3_IRQHandler,      //      ; DMA1 Channel 3
        DMA1_Channel4_IRQHandler,      //      ; DMA1 Channel 4
        DMA1_Channel5_IRQHandler,      //      ; DMA1 Channel 5
        DMA1_Channel6_IRQHandler,      //      ; DMA1 Channel 6
        DMA1_Channel7_IRQHandler,      //      ; DMA1 Channel 7
        ADC1_2_IRQHandler,             //             ; ADC1 & ADC2
        USB_HP_CAN1_TX_IRQHandler,     //     ; USB High Priority or CAN1 TX
        USB_LP_CAN1_RX0_IRQHandler,    //    ; USB Low  Priority or CAN1 RX0
        CAN1_RX1_IRQHandler,           //           ; CAN1 RX1
        CAN1_SCE_IRQHandler,           //           ; CAN1 SCE
        EXTI9_5_IRQHandler,            //            ; EXTI Line 9..5
        TIM1_BRK_TIM9_IRQHandler,      //      ; TIM1 Break and TIM9
        TIM1_UP_TIM10_IRQHandler,      //      ; TIM1 Update and TIM10
        TIM1_TRG_COM_TIM11_IRQHandler, // ; TIM1 Trigger and Commutation and TIM11
        TIM1_CC_IRQHandler,            //            ; TIM1 Capture Compare
        TIM2_IRQHandler,               //               ; TIM2
        TIM3_IRQHandler,               //               ; TIM3
        TIM4_IRQHandler,               //               ; TIM4
        I2C1_EV_IRQHandler,            //            ; I2C1 Event
        I2C1_ER_IRQHandler,            //            ; I2C1 Error
        I2C2_EV_IRQHandler,            //            ; I2C2 Event
        I2C2_ER_IRQHandler,            //            ; I2C2 Error
        SPI1_IRQHandler,               //               ; SPI1
        SPI2_IRQHandler,               //               ; SPI2
        USART1_IRQHandler,             //             ; USART1
        USART2_IRQHandler,             //             ; USART2
        USART3_IRQHandler,             //             ; USART3
        EXTI15_10_IRQHandler,          //          ; EXTI Line 15..10
        RTCAlarm_IRQHandler,           //           ; RTC Alarm through EXTI Line
        USBWakeUp_IRQHandler,          //          ; USB Wakeup from suspend
        TIM8_BRK_TIM12_IRQHandler,     //     ; TIM8 Break and TIM12
        TIM8_UP_TIM13_IRQHandler,      //      ; TIM8 Update and TIM13
        TIM8_TRG_COM_TIM14_IRQHandler, // ; TIM8 Trigger and Commutation and TIM14
        TIM8_CC_IRQHandler,            //            ; TIM8 Capture Compare
        ADC3_IRQHandler,               //               ; ADC3
        FSMC_IRQHandler,               //               ; FSMC
        SDIO_IRQHandler,               //               ; SDIO
        TIM5_IRQHandler,               //               ; TIM5
        SPI3_IRQHandler,               //               ; SPI3
        UART4_IRQHandler,              //              ; UART4
        UART5_IRQHandler,              //              ; UART5
        TIM6_IRQHandler,               //               ; TIM6
        TIM7_IRQHandler,               //               ; TIM7
        DMA2_Channel1_IRQHandler,      //      ; DMA2 Channel1
        DMA2_Channel2_IRQHandler,      //      ; DMA2 Channel2
        DMA2_Channel3_IRQHandler,      //      ; DMA2 Channel3
        DMA2_Channel4_5_IRQHandler,    // ,

};

extern "C" void __cmain(void);
extern "C" __weak void __iar_init_core(void);
extern "C" __weak void __iar_init_vfp(void);

void ResetHandler(void)
{
  SystemInit();
  __iar_program_start();
}

#pragma required = __vector_table
void __iar_program_start(void)
{
  __iar_init_core();
  __iar_init_vfp();
  __cmain();
}
