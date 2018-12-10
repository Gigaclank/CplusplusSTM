/**
 * @file gpio.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"


class GPIO
{
private:
  GPIO_TypeDef *_port;
  int _pin;
  GPIO_InitTypeDef GPIO_InitStruct;
  void enable_clock();

public:
  /* Class Constructors */
  GPIO(GPIO_TypeDef *port, uint16_t pin);
  GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOSpeed_TypeDef speed);
  GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode);
  GPIO(GPIO_TypeDef *port, uint16_t pin, GPIOMode_TypeDef mode);
	

  /* Class functions */
  void speed(GPIOSpeed_TypeDef speed);
  void mode(GPIOMode_TypeDef mode);
  void write(uint8_t state);
  uint8_t read(void);
  void attach_interrupt(void (*callback)(void), EXTITrigger_TypeDef type = EXTI_Trigger_Falling);
};

#endif