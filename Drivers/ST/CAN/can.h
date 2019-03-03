/**
 * @file can.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-03-03
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
#ifndef _CAN_H
#define _CAN_H
#include "stm32f10x_rcc.h"
#include "stm32f10x_can.h"
#include <stdio.h>
#include <string.h>

typedef enum CAN_Baud
{
  BAUD_1MBPS = 1000,
  BAUD_500KBPS = 500,
  BAUD_250KBPS = 250,
  BAUD_125KBPS = 125,
  BAUD_100KBPS = 100,
  BAUD_50KBPS = 50,
  BAUD_20KBPS = 20,
  BAUD_10KBPS = 10
} CAN_Baud;

class CAN
{
private:
  CAN_TypeDef *_CANx;
  CAN_InitTypeDef _CAN_InitStructure;
  CAN_FilterInitTypeDef _CAN_FilterInitStructure;
  CAN_Baud _baud;
  int _id;
  CanRxMsg _RxMessage;
  CanTxMsg _TxMessage;
  void (*_rxcallback)(CanRxMsg msg);
  void (*_txcallback)(CanTxMsg msg);
  void configureInterrupt(void);

public:
  CAN(CAN_TypeDef *can, CAN_Baud baud, int id);
  void send(uint8_t *data, uint8_t size);
  void InitRxMsg(CanRxMsg *RxMessage);
  void attach_interrupt(void (*rxcallback)(CanRxMsg msg), void (*txcallback)(CanTxMsg msg) = 0);
  void rx_interrupt(void);
};

void can_interrupt(CAN_TypeDef *can);

#endif