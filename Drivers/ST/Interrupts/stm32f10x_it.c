/**
 * @file stm32f10x_it.c
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-09
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "stm32f10x_it.h"

uint32_t SysTicTimer = 0;
void (*exti_callback[16])(void);
void (*timer_callback[18])(void);
TIMER *timer[16];

void NMIException(void)
{
  while (1)
  {
  }
}
void HardFaultException(void)
{
  while (1)
  {
    NVIC_SystemReset(); //reset system cant afford to be stuck
  }
}
void MemManageException(void)
{
  while (1)
  {
  }
}

void BusFaultException(void)
{
  while (1)
  {
  }
}
void UsageFaultException(void)
{
  while (1)
  {
  }
}
void SVCHandler(void)
{
}
void DebugMonitor(void)
{
}
void PendSVC(void)
{
}
void SysTick_Handler(void)
{
  SysTicTimer++;
}
/* Window Watch Dog Interrupts */
void WWDG_IRQHandler(void)
{
}
void PVD_IRQHandler(void)
{
}
void TAMPER_IRQHandler(void)
{
}
void FLASH_IRQHandler(void)
{
}

/* EXTERNAL LINE INTTERUPTS */
void EXTI0_IRQHandler(void)
{
  if (exti_callback[0])
    exti_callback[0]();
  EXTI_ClearITPendingBit(EXTI_Line0);
}
void EXTI1_IRQHandler(void)
{
  if (exti_callback[1])
    exti_callback[1]();
  EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI2_IRQHandler(void)
{
  if (exti_callback[2])
    exti_callback[2]();
  EXTI_ClearITPendingBit(EXTI_Line2);
}
void EXTI3_IRQHandler(void)
{
  if (exti_callback[3])
    exti_callback[3]();
  EXTI_ClearITPendingBit(EXTI_Line3);
}
void EXTI4_IRQHandler(void)
{
  if (exti_callback[4])
    exti_callback[4]();
  EXTI_ClearITPendingBit(EXTI_Line4);
}
void EXTI9_5_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line5))
  {
    if (exti_callback[5])
      exti_callback[5]();
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
  if (EXTI_GetITStatus(EXTI_Line6))
  {
    if (exti_callback[6])
      exti_callback[6]();
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
  if (EXTI_GetITStatus(EXTI_Line7))
  {
    if (exti_callback[7])
      exti_callback[7]();
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
  if (EXTI_GetITStatus(EXTI_Line8))
  {
    if (exti_callback[8])
      exti_callback[8]();
    EXTI_ClearITPendingBit(EXTI_Line8);
  }
  if (EXTI_GetITStatus(EXTI_Line9))
  {
    if (exti_callback[9])
      exti_callback[9]();
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}
void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line10))
  {
    if (exti_callback[10])
      exti_callback[10]();
    EXTI_ClearITPendingBit(EXTI_Line10);
  }
  if (EXTI_GetITStatus(EXTI_Line11))
  {
    if (exti_callback[11])
      exti_callback[11]();
    EXTI_ClearITPendingBit(EXTI_Line11);
  }
  if (EXTI_GetITStatus(EXTI_Line12))
  {
    if (exti_callback[12])
      exti_callback[12]();
    EXTI_ClearITPendingBit(EXTI_Line12);
  }
  if (EXTI_GetITStatus(EXTI_Line13))
  {
    if (exti_callback[13])
      exti_callback[13]();
    EXTI_ClearITPendingBit(EXTI_Line13);
  }
  if (EXTI_GetITStatus(EXTI_Line14))
  {
    if (exti_callback[14])
      exti_callback[14]();
    EXTI_ClearITPendingBit(EXTI_Line14);
  }
  if (EXTI_GetITStatus(EXTI_Line15))
  {
    if (exti_callback[15])
      exti_callback[15]();
    EXTI_ClearITPendingBit(EXTI_Line15);
  }
}
/* DMA intterupts */
void DMA1_Channel1_IRQHandler(void)
{
}
void DMA1_Channel2_IRQHandler(void)
{
}
void DMA1_Channel3_IRQHandler(void)
{
}
void DMA1_Channel4_IRQHandler(void)
{
}
void DMA1_Channel5_IRQHandler(void)
{
}
void DMA1_Channel6_IRQHandler(void)
{
}
void DMA1_Channel7_IRQHandler(void)
{
}
void DMA2_Channel1_IRQHandler(void)
{
}
void DMA2_Channel2_IRQHandler(void)
{
}
void DMA2_Channel3_IRQHandler(void)
{
}
void DMA2_Channel4_5_IRQHandler(void)
{
}

/* ADC Interrupts */
void ADC1_2_IRQHandler(void)
{
}
void ADC3_IRQHandler(void)
{
}

/* USB/CAN interrupts */
void USBWakeUp_IRQHandler(void)
{
}
void USB_HP_CAN_TX_IRQHandler(void)
{
}
void USB_LP_CAN_RX0_IRQHandler(void)
{
}
void USB_HP_CAN1_TX_IRQHandler(void)
{
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
}
void CAN1_RX1_IRQHandler(void)
{
}
void CAN1_SCE_IRQHandler(void)
{
}
void CAN_RX1_IRQHandler(void)
{
}
void CAN_SCE_IRQHandler(void)
{
}

/* Timers */
void TIM1_BRK_TIM9_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Break) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_FLAG_Break);
  }
  if (TIM_GetFlagStatus(TIM9, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[8])
      timer_callback[8]();
    if (timer[8])
      timer[8]->cntr();
    TIM_ClearFlag(TIM9, TIM_FLAG_Update);
  }
}
void TIM1_UP_TIM10_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[0])
      timer_callback[0]();
    if (timer[0])
      timer[0]->cntr();
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  }
  if (TIM_GetFlagStatus(TIM10, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[9])
      timer_callback[9]();
    if (timer[9])
      timer[9]->cntr();
    TIM_ClearFlag(TIM10, TIM_FLAG_Update);
  }
}
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_COM) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_FLAG_COM);
  }
  if (TIM_GetFlagStatus(TIM11, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[10])
      timer_callback[10]();
    if (timer[10])
      timer[10]->cntr();
    TIM_ClearFlag(TIM11, TIM_FLAG_Update);
  }
}

void TIM1_BRK_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Break) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_FLAG_Break);
  }
}
void TIM1_UP_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[0])
      timer_callback[0]();
    if (timer[0])
      timer[0]->cntr();
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  }
}
void TIM1_TRG_COM_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_COM) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_FLAG_COM);
  }
}
void TIM1_CC_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_IT_CC1);
  }
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC2) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_IT_CC2);
  }
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC3) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_IT_CC3);
  }
  if (TIM_GetFlagStatus(TIM1, TIM_FLAG_CC4) != RESET)
  {
    TIM_ClearFlag(TIM1, TIM_IT_CC4);
  }
}
void TIM2_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[1])
      timer_callback[1]();
    if (timer[1])
      timer[1]->cntr();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); /* Clear the TIM2 Update pending bit */
  }
}
void TIM3_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[2])
      timer_callback[2]();
    if (timer[2])
      timer[2]->cntr();
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /* Clear the TIM3 Update pending bit */
  }
}
void TIM4_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[3])
      timer_callback[3]();
    if (timer[3])
      timer[3]->cntr();
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear the TIM4 Update pending bit */
  }
}
void TIM5_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM5, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[4])
      timer_callback[4]();
    if (timer[4])
      timer[4]->cntr();
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}
void TIM6_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) != RESET)
  {
     if (timer_callback[5])
      timer_callback[5]();
    if (timer[5])
      timer[5]->cntr();
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}
void TIM7_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[6])
      timer_callback[6]();
    if (timer[6])
      timer[6]->cntr();
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}

void TIM8_CC_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_CC1); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_CC2); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_CC3); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_CC4) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_CC4); /* Clear the TIM5 Update pending bit */
  }
}
void TIM8_BRK_TIM12_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_Break) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_Break); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM12, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[11])
      timer_callback[11]();
    if (timer[11])
      timer[11]->cntr();
    TIM_ClearITPendingBit(TIM12, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}
void TIM8_UP_TIM13_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[7])
      timer_callback[7]();
    if (timer[7])
      timer[7]->cntr();
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM13, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[12])
      timer_callback[12]();
    if (timer[12])
      timer[12]->cntr();
    TIM_ClearITPendingBit(TIM13, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  if (TIM_GetFlagStatus(TIM8, TIM_FLAG_COM) != RESET)
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_COM); /* Clear the TIM5 Update pending bit */
  }
  if (TIM_GetFlagStatus(TIM14, TIM_FLAG_Update) != RESET)
  {
    if (timer_callback[13])
      timer_callback[13]();
    if (timer[13])
      timer[13]->cntr();
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update); /* Clear the TIM5 Update pending bit */
  }
}

/* I2C Interrupts */
void I2C1_EV_IRQHandler(void)
{
}
void I2C1_ER_IRQHandler(void)
{
}
void I2C2_EV_IRQHandler(void)
{
}
void I2C2_ER_IRQHandler(void)
{
}
/* SPI Interrupts */
void SPI1_IRQHandler(void)
{
}
void SPI2_IRQHandler(void)
{
}
void SPI3_IRQHandler(void)
{
}
/* USART Interrupts */
void USART1_IRQHandler(void)
{
}
void USART2_IRQHandler(void)
{
}
void USART3_IRQHandler(void)
{
}
void UART4_IRQHandler(void)
{
}
void UART5_IRQHandler(void)
{
}
/* RCC Interrupts */
void RCC_IRQHandler(void)
{
}

void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_SEC); /* Clear the RTC Second interrupt */
  }

  if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
  {
    RTC_ClearITPendingBit(RTC_IT_ALR); /* Clear the RTC Alarm interrupt */
  }

  return;
}
/* RTC Interrupts */
void RTCAlarm_IRQHandler(void)
{
}
/* FSMC Interrupts*/
void FSMC_IRQHandler(void)
{
}
/* SDIO Interrupts*/
void SDIO_IRQHandler(void)
{
}

/*
*********************************************************************************************************
*                                           End of stm32f10x_it.c
*********************************************************************************************************
*/
