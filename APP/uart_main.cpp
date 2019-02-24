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
using std::cout;

GPIO tx1(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP),rx1(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
GPIO tx2(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP),rx2(GPIOA,GPIO_Pin_3,GPIO_Mode_IN_FLOATING);
USART *uart[2];//(USART1);

int main()
{
  api_system_start();
  uart[0] = new USART(USART1);
  uart[1] = new USART(USART2);
  p.assign_uart(uart[1]); 
  
  while (1)
  {  
    cout<<'a';
  }
}
