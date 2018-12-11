/**
 * @file timer.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-08
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */

/* Not working :TIM8,9,10,11,12,13 */

#ifndef _TIMER_H
#define _TIMER_H
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"
#include "misc.h"

class TIMER
{
  private:
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TypeDef *_TIMx;
    uint32_t cnt;
    uint32_t runTime;
    uint32_t freq ;

  public:
    TIMER(TIM_TypeDef *TIMx,float ms);
    ~TIMER();
    void attach_intterupt(void (*callback)(void));
    void delay(uint16_t delay_value);
    void cntr(void);
};

extern TIMER *timer[16];

#endif