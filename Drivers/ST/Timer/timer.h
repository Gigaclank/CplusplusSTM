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

using std::cout;

typedef enum ScaleFactor{
  Microseconds = 0xf4240,
  Milliseconds = 0x3e8,
  Nanoseconds = 0x3b9aca00
}ScaleFactor;

//typedef enum Clock{
//  APB1 = 36000000,
//  APB2 = 72000000,
//}Clock;

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
  void calculate_values(TIM_TypeDef *TIMx,float freq,uint16_t *arr, uint16_t *psc);
  unsigned int PeriodFromFrequency( double f, ScaleFactor *scale);
  int which_source(int timer);
  int which_source(TIM_TypeDef *timer);

public:
  TIMER(TIM_TypeDef *TIMx, double S);
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