/**
 * @file main.cpp
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
#include <iostream>
#include "api_system_startup.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"

int main()
{
  api_system_start();

  GPIO red(GPIOF, GPIO_Pin_6, GPIO_Mode_AF_PP),green(GPIOF, GPIO_Pin_7, GPIO_Mode_AF_PP),blue(GPIOF, GPIO_Pin_8, GPIO_Mode_AF_PP);
  TIMER redT(TIM10,0.01),greenT(TIM11,0.01),blueT(TIM13,0.01);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_TIM10, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_TIM11, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_TIM13, ENABLE);

  redT.set_max_duty(1,255);
  greenT.set_max_duty(1,255);
  blueT.set_max_duty(1,255);
  
  redT.setup_pwm(1,0x06);
  greenT.setup_pwm(1,0xd8);
  blueT.setup_pwm(1,0x15);
  
  while (1)
  {  
    
   redT.update_pwm(1,(uint8_t)(0x06));
   greenT.update_pwm(1,(uint8_t)(0xd8));
   blueT.update_pwm(1,(uint8_t)(0x15));
   
   redT.delay(100); 
   
   redT.invert_pwm(1);
   greenT.invert_pwm(1);
   blueT.invert_pwm(1);
   
   redT.delay(100); 

   
  }
}

