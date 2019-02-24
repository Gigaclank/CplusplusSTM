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
#include "dac.h"
using std::cout;
#define DAC_USE_DMA
const uint16_t Sine12bit[32] = {
  2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
  3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
  599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
int main()
{
#ifndef DAC_USE_DMA
  int i = 0;
#endif 
  api_system_start();
  STM_DAC d(2);  
#ifdef DAC_USE_DMA
  d.auto_update((uint32_t*)Sine12bit,sizeof(Sine12bit)/2,500);
#endif
  while (1)
  {
#ifndef DAC_USE_DMA
    d.write(Sine12bit[i%32]);  
    api_ms_delay(1);
    i++;
#endif
  }
  
}