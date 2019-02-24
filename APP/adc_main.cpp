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
#include "adc.h"
#include "gpio.h"
using std::cout;
STM_ADC adc(ADC1);
GPIO adcPin(GPIOA,GPIO_Pin_1,GPIO_Mode_AIN);
int main()
{
  api_system_start();
  while (1)
  {  
    cout<<adc.read(1)<<"\r\n";
  }
}

