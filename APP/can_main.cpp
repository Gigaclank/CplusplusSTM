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
#include "can.h"
using std::cout;
/* Set up pins */

GPIO btn_user(GPIOB, GPIO_Pin_9,GPIO_Mode_IN_FLOATING);
GPIO btn_tamper(GPIOC, GPIO_Pin_13,GPIO_Mode_IN_FLOATING);

void can1_handle(CanRxMsg RxMessage);
void can2_handle(CanRxMsg RxMessage);
void setup_can_pins(void);
void setup_can_pins(void){
  
  GPIO can1rx(GPIOD, GPIO_Pin_0, GPIO_Mode_IPU);
  GPIO can2rx(GPIOB, GPIO_Pin_5, GPIO_Mode_IPU);
  
  GPIO can1tx(GPIOD, GPIO_Pin_1, GPIO_Mode_AF_PP);
  GPIO can2tx(GPIOB, GPIO_Pin_6, GPIO_Mode_AF_PP);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);
}
int main()
{
  uint8_t data[5] = {0};
  api_system_start();
  setup_can_pins();
  CAN *c[2];
  c[0] = new CAN(CAN1,BAUD_125KBPS,0x6420);
  c[1] = new CAN(CAN2,BAUD_125KBPS,0x2460);
  c[0]->attach_interrupt(can1_handle);
  c[1]->attach_interrupt(can2_handle);
  while (1)
  {   
    if(btn_user.read() == 0){
      cout<<"User Press\r\n";
      data[0] = 0x55;
      c[0]->send(data,1);
      while(btn_user.read() == 0);
    }
    if(btn_tamper.read() == 0){
      cout<<"Tamper Press\r\n";
      data[0] = 0xAA;
      c[1]->send(data,1);
      while(btn_tamper.read() == 0);
    }
  }
}


void can1_handle(CanRxMsg RxMessage){
   if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 1)&&(RxMessage.Data[0] == 0xAA))
  {
    cout<<"Can 1 Rx\r\n";
  }
  else
  {
    cout<<"Can 1 Error\r\n";
  }
}


void can2_handle(CanRxMsg RxMessage){
   if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 1)&&(RxMessage.Data[0] == 0x55))
  {
    cout<<"Can 2 Rx\r\n";
  }
  else
  {
    cout<<"Can 2 Error\r\n";
  }
}