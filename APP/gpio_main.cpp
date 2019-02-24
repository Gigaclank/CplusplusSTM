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
GPIO btn(GPIOC, GPIO_Pin_5);
GPIO r(GPIOF, GPIO_Pin_6, GPIO_Mode_Out_OD);
GPIO g(GPIOF, GPIO_Pin_7, GPIO_Mode_Out_OD);
GPIO b(GPIOF, GPIO_Pin_8, GPIO_Mode_Out_OD);
  
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
}