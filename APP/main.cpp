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

void button_handler(void);
void timer_intterupt(void);
TIMER *tim;
GPIO r(GPIOF, GPIO_Pin_6, GPIO_Mode_Out_OD);
GPIO g(GPIOF, GPIO_Pin_7, GPIO_Mode_Out_OD);
GPIO b(GPIOF, GPIO_Pin_8, GPIO_Mode_Out_OD);
  
int main()
{
  uint8_t current_timer = 0;
  TIM_TypeDef * timers[15]={
    TIM1,
    TIM2,
    TIM3,
    TIM4,
    TIM5,
    TIM6,
    TIM7,
    TIM8,
    TIM9,
    TIM10,
    TIM11,
    TIM12,
    TIM13,
    TIM14,
    0
  };
  SystemStart();
    
  GPIO btn(GPIOC,GPIO_Pin_5,GPIO_Mode_IPU);
  btn.attach_interrupt(button_handler);
  
  
  while (1)
  {   
   
    tim = new TIMER(timers[current_timer],0.1);//100us
    tim->attach_intterupt(timer_intterupt);
    tim->delay(1);
    r.write(!r.read()); g.write(!g.read());b.write(!b.read());
    
    printf("%d\r\n",current_timer+1);
    if(timers[current_timer+1] == 0) current_timer = 0;
    else current_timer ++;
    delete tim;
    
    
  }
}
void timer_intterupt(void){
//  cout<<"Tick\r\n";
//   r.write(!r.read());
}
void button_handler(void){
  cout<<"Button\r\n";
 
}