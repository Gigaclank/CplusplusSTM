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
#include "spi.h"



int main()
{
  api_system_start();
  STM_SPI s(SPI2,115200);
  while (1)
  {   
    cout<<s.rdwr(0x55)<<"\r\n";
  }
}