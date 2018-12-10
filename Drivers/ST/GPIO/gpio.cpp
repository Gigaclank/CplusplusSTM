/**
 * @file gpio.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "gpio.h"


/**
 * @brief Construct a new GPIO::GPIO object
 * 
 * @param port 
 * @param pin 
 */
GPIO::GPIO(GPIO_TypeDef *port, uint16_t pin)
{
  _port = port;
  _pin = pin;
  enable_clock();
  GPIO_InitStruct.GPIO_Pin = pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(_port, &GPIO_InitStruct);
}

/**
 * @brief Construct a new GPIO::GPIO object
 * 
 * @param port 
 * @param pin 
 * @param speed 
 */
GPIO::GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOSpeed_TypeDef speed)
{
  _port = port;
  _pin = pin;
  enable_clock();
  GPIO_InitStruct.GPIO_Pin = pin;
  GPIO_InitStruct.GPIO_Speed = speed;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(_port, &GPIO_InitStruct);
}
/**
 * @brief Construct a new GPIO::GPIO object
 * 
 * @param port 
 * @param pin 
 * @param speed 
 * @param mode 
 */
GPIO::GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode)
{
  _port = port;
  _pin = pin;
  enable_clock();
  GPIO_InitStruct.GPIO_Pin = pin;
  GPIO_InitStruct.GPIO_Speed = speed;
  GPIO_InitStruct.GPIO_Mode = mode;
  GPIO_Init(_port, &GPIO_InitStruct);
}
/**
 * @brief Construct a new GPIO::GPIO object
 * 
 * @param port 
 * @param pin 
 * @param mode 
 */
GPIO::GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOMode_TypeDef mode)
{
  _port = port;
  _pin = pin;
  enable_clock();
  GPIO_InitStruct.GPIO_Pin = pin;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = mode;
  GPIO_Init(_port, &GPIO_InitStruct);
}

/**
 * @brief set the speed of the pin
 * 
 * @param speed 
 */
void GPIO::speed(GPIOSpeed_TypeDef speed)
{

  GPIO_InitStruct.GPIO_Speed = speed;
  GPIO_Init(_port, &GPIO_InitStruct);
}

/**
 * @brief set the mode of the pin
 * 
 * @param mode 
 */
void GPIO::mode(GPIOMode_TypeDef mode)
{
  GPIO_InitStruct.GPIO_Mode = mode;
  GPIO_Init(_port, &GPIO_InitStruct);
}

/**
 * @brief write logic level to pin
 * 
 * @param state 
 */
void GPIO::write(uint8_t state)
{
  if (state == 1)
    GPIO_SetBits(_port, _pin);
  else
    GPIO_ResetBits(_port, _pin);
}

void GPIO::enable_clock(void)
{
  uint32_t clock;
  if (_port == GPIOA)
  {
    clock = RCC_APB2Periph_GPIOA;
  }
  else if (_port == GPIOB)
  {
    clock = RCC_APB2Periph_GPIOB;
  }
  else if (_port == GPIOC)
  {
    clock = RCC_APB2Periph_GPIOC;
  }
  else if (_port == GPIOD)
  {
    clock = RCC_APB2Periph_GPIOD;
  }
  else if (_port == GPIOE)
  {
    clock = RCC_APB2Periph_GPIOE;
  }
  else if (_port == GPIOF)
  {
    clock = RCC_APB2Periph_GPIOF;
  }
  else if (_port == GPIOG)
  {
    clock = RCC_APB2Periph_GPIOG;
  }
  RCC_APB2PeriphClockCmd(clock, ENABLE);
}

/**
 * @brief read the state of the pin - works for both input and output
 * 
 * @return uint8_t 
 */
uint8_t GPIO::read(void)
{

  switch (GPIO_InitStruct.GPIO_Mode)
  {
  case GPIO_Mode_IN_FLOATING:
  case GPIO_Mode_IPD:
  case GPIO_Mode_IPU:
    return GPIO_ReadInputDataBit(_port, _pin);
  case GPIO_Mode_Out_OD:
  case GPIO_Mode_Out_PP:
  case GPIO_Mode_AF_OD:
  case GPIO_Mode_AF_PP:
    return GPIO_ReadOutputDataBit(_port, _pin);
  default:
    return 0;
  }
}

/**
 * @brief assign a callback and the type of interrupt for the pin
 * 
 * @param callback 
 * @param type 
 */
void GPIO::attach_interrupt(void (*callback)(void), EXTITrigger_TypeDef type)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  uint8_t PortSource;
  uint8_t PinSource;
  uint32_t ExtiLine;
  uint8_t irqChannel;
  switch (_pin)
  {
  case GPIO_Pin_0:
    ExtiLine = EXTI_Line0;
    PinSource = GPIO_PinSource0;
    irqChannel = EXTI0_IRQn;
    exti_callback[0] = callback;
    break;
  case GPIO_Pin_1:
    ExtiLine = EXTI_Line1;
    PinSource = GPIO_PinSource1;
    irqChannel = EXTI1_IRQn;
    exti_callback[1] = callback;
    break;
  case GPIO_Pin_2:
    ExtiLine = EXTI_Line2;
    PinSource = GPIO_PinSource2;
    irqChannel = EXTI2_IRQn;
    exti_callback[2] = callback;
    break;
  case GPIO_Pin_3:
    ExtiLine = EXTI_Line3;
    PinSource = GPIO_PinSource3;
    irqChannel = EXTI3_IRQn;
    exti_callback[3] = callback;
    break;
  case GPIO_Pin_4:
    ExtiLine = EXTI_Line4;
    PinSource = GPIO_PinSource4;
    irqChannel = EXTI4_IRQn;
    exti_callback[4] = callback;
    break;
  case GPIO_Pin_5:
    ExtiLine = EXTI_Line5;
    PinSource = GPIO_PinSource5;
    irqChannel = EXTI9_5_IRQn;
    exti_callback[5] = callback;
    break;
  case GPIO_Pin_6:
    ExtiLine = EXTI_Line6;
    PinSource = GPIO_PinSource6;
    irqChannel = EXTI9_5_IRQn;
    exti_callback[6] = callback;
    break;
  case GPIO_Pin_7:
    ExtiLine = EXTI_Line7;
    PinSource = GPIO_PinSource7;
    irqChannel = EXTI9_5_IRQn;
    exti_callback[7] = callback;
    break;
  case GPIO_Pin_8:
    ExtiLine = EXTI_Line8;
    PinSource = GPIO_PinSource8;
    irqChannel = EXTI9_5_IRQn;
    exti_callback[8] = callback;
    break;
  case GPIO_Pin_9:
    ExtiLine = EXTI_Line9;
    PinSource = GPIO_PinSource9;
    irqChannel = EXTI9_5_IRQn;
    exti_callback[9] = callback;
    break;
  case GPIO_Pin_10:
    ExtiLine = EXTI_Line10;
    PinSource = GPIO_PinSource10;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[10] = callback;
    break;
  case GPIO_Pin_11:
    ExtiLine = EXTI_Line11;
    PinSource = GPIO_PinSource11;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[11] = callback;
    break;
  case GPIO_Pin_12:
    ExtiLine = EXTI_Line12;
    PinSource = GPIO_PinSource12;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[12] = callback;
    break;
  case GPIO_Pin_13:
    ExtiLine = EXTI_Line13;
    PinSource = GPIO_PinSource13;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[13] = callback;
    break;
  case GPIO_Pin_14:
    ExtiLine = EXTI_Line14;
    PinSource = GPIO_PinSource14;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[14] = callback;
    break;
  case GPIO_Pin_15:
    ExtiLine = EXTI_Line15;
    PinSource = GPIO_PinSource15;
    irqChannel = EXTI15_10_IRQn;
    exti_callback[15] = callback;
    break;
  default:
    break;
  }
  
  
  if (_port == GPIOA)
  {
    PortSource = GPIO_PortSourceGPIOA;
  }
  else if (_port == GPIOB)
  {
    PortSource = GPIO_PortSourceGPIOB;
  }
  else if (_port == GPIOC)
  {
    PortSource = GPIO_PortSourceGPIOC;
  }
  else if (_port == GPIOD)
  {
    PortSource = GPIO_PortSourceGPIOD;
  }
  else if (_port == GPIOE)
  {
    PortSource = GPIO_PortSourceGPIOE;
  }
  else if (_port == GPIOF)
  {
    PortSource = GPIO_PortSourceGPIOF;
  }
  else if (_port == GPIOG)
  {
    PortSource = GPIO_PortSourceGPIOG;
  }
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);

  GPIO_EXTILineConfig(PortSource, PinSource); /* Connect GPIO to EXTI */
  
  EXTI_InitStructure.EXTI_Line = ExtiLine; /* GPIO pin 0 must be on EXTI line 0 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = type;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitTypeDef 	NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel				    =  irqChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=  2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority			=  2;
  NVIC_InitStructure.NVIC_IRQChannelCmd				=  ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}