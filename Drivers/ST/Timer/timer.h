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
  TIM_OCInitTypeDef TIM_OCInitStructure[4];
  TIM_TypeDef *_TIMx;
  uint32_t cnt;
  uint32_t runTime;
  uint32_t freq;
  uint8_t duty_max[4];

public:
  TIMER(TIM_TypeDef *TIMx, float ms);
  ~TIMER();
  void attach_intterupt(void (*callback)(void));
  void delay(uint16_t delay_value);
  void cntr(void);
  void setup_pwm(void);
  void setup_pwm(uint8_t ch, uint8_t duty);
  void invert_pwm(uint8_t ch);
  void update_pwm(uint8_t ch, uint8_t duty);
  void update_pwm(uint8_t ch, float duty);
  void set_max_duty(uint8_t max);
  void set_max_duty(uint8_t ch, uint8_t max);
};

extern TIMER *timer[16];

#endif