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
using std::cout;
void button_handler(void);
GPIO btn(GPIOB, GPIO_Pin_9);
GPIO r(GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP);
GPIO g(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP);
GPIO b(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP);
GPIO o(GPIOD, GPIO_Pin_13, GPIO_Mode_Out_PP);
  
int main()
{
  
  api_system_start();
  btn.attach_interrupt(button_handler);
  while (1)
  {   
  }
}
void button_handler(void){
  cout<<"Button\r\n";
  r.write(!r.read());
  g.write(!g.read());
  b.write(!b.read());
  o.write(!o.read());
}