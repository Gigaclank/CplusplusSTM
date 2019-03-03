/**
 * @file can.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-03-03
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "can.h"

void (*can_rx_handle[2])(void);
void (*can_tx_handle[2])(void);
CAN* _can[2];
/**
 * @brief Construct a new CAN::CAN object
 * 
 */
CAN::CAN(CAN_TypeDef *can, CAN_Baud baud, int id)
{
  
  this->_CANx = can;
  this->_baud = baud;
  this->_id = id;
  
  this->configureInterrupt();
  
  if (this->_CANx == CAN1){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    _can[0] = this;
  }
#ifdef STM32F10X_CL
  else if (this->_CANx == CAN2){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    _can[1] = this;
  }
#endif
  else
    return;
  CAN_DeInit(this->_CANx);
  CAN_StructInit(&this->_CAN_InitStructure);
  this->_CAN_InitStructure.CAN_TTCM = DISABLE;
  this->_CAN_InitStructure.CAN_ABOM = DISABLE;
  this->_CAN_InitStructure.CAN_AWUM = DISABLE;
  this->_CAN_InitStructure.CAN_NART = DISABLE;
  this->_CAN_InitStructure.CAN_RFLM = DISABLE;
  this->_CAN_InitStructure.CAN_TXFP = ENABLE;
  this->_CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  this->_CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  this->_CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  this->_CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  
  switch (this->_baud)
  {
  case BAUD_1MBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 6;
    break;
  case BAUD_500KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 12;
    break;
  case BAUD_250KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 24;
    break;
  case BAUD_125KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 48;
    break;
  case BAUD_100KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 60;
    break;
  case BAUD_50KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 120;
    break;
  case BAUD_20KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 300;
    break;
  case BAUD_10KBPS:
    this->_CAN_InitStructure.CAN_Prescaler = 600;
    break;
  default:
    return;
  }
  
  CAN_Init(this->_CANx, &this->_CAN_InitStructure);
  /* CAN1 filter init */
  if(this->_CANx == CAN1)this->_CAN_FilterInitStructure.CAN_FilterNumber = 1;
  else this->_CAN_FilterInitStructure.CAN_FilterNumber = 15;
  this->_CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  this->_CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  this->_CAN_FilterInitStructure.CAN_FilterIdHigh = this->_id;
  this->_CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  this->_CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  this->_CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  this->_CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  this->_CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&this->_CAN_FilterInitStructure);
  
  /* Transmit */
  this->_TxMessage.StdId = 0x321;
  this->_TxMessage.ExtId = 0x01;
  this->_TxMessage.RTR = CAN_RTR_DATA;
  this->_TxMessage.IDE = CAN_ID_STD;
  this->_TxMessage.DLC = 1;  
  CAN_ITConfig(this->_CANx, CAN_IT_FMP0, ENABLE);
}

/**
 * @brief send a message over can
 * 
 * @param data 
 */
void CAN::send(uint8_t *data, uint8_t size)
{
    memcpy(this->_TxMessage.Data,data,size);
    this->_TxMessage.DLC = size;
    CAN_Transmit(this->_CANx, &this->_TxMessage);
}

/**
 * @brief setup the interrupt for the selected can
 * 
 */
void CAN::configureInterrupt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
#ifdef STM32F10X_CL
    if (this->_CANx == CAN1)
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    else if (this->_CANx == CAN2)
        NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#else
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    InitRxMsg(&this->_RxMessage);
}

/**
 * @brief setup the rx message
 * 
 * @param RxMessage 
 */
void CAN::InitRxMsg(CanRxMsg *RxMessage)
{
    RxMessage->StdId = 0;
    RxMessage->ExtId = 0;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
    for (int i = 0; i < 8; i++)
        RxMessage->Data[i] = 0;
}
/**
 * @brief attach interrupt
 * 
 * @param rxcallback 
 */
void CAN::attach_interrupt(void (*rxcallback)(CanRxMsg msg), void (*txcallback)(CanTxMsg msg))
{
    this->_rxcallback = rxcallback;
    this->_txcallback = txcallback;
}

/**
 * @brief the interrupt used for the canbus
 * 
 */
void CAN::rx_interrupt(void)
{
    CAN_Receive(this->_CANx, CAN_FIFO0, &this->_RxMessage);
    if (this->_rxcallback)
        this->_rxcallback(this->_RxMessage);
}


void can_interrupt(CAN_TypeDef *can){
  if(can == CAN1){
    if(_can[0]) _can[0]->rx_interrupt();
  }else{
    if(_can[1]) _can[1]->rx_interrupt();
  }
}