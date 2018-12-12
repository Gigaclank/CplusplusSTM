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
#include "rtc.h"
void tick(void);

RTC_CL *rc;
int main()
{
 
  api_system_start();
  
  rc = new RTC_CL(RTC_SRC_INTERNAL);
  rc->attach_interrupt(tick);
  while (1)
  {   
  }
}
void tick(void){
  
   cout<<rc->read()<<"\r\n";
}