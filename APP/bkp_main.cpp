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
#include "bkp.h"
using std::cout;

int main()
{
  
  api_system_start();
  STM_BKP bkp;
  uint16_t reg;
//  bkp = new STM_BKP();
  while (1)
  {   
    reg = bkp.read(BKP_DR1);
    if(reg == 0xAABB){
      cout<<reg<<"\r\n";
    }else{
      cout<<reg<<"\r\n";
      bkp.write(BKP_DR1,0xAABB);
    }
  }
}