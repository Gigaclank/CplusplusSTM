/**
 * @file adc.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
class STM_ADC
{
  private:
    ADC_TypeDef *_ADCx;
    ADC_InitTypeDef _ADC_InitStructure;
    uint8_t _sample_time;

  public:
    STM_ADC(ADC_TypeDef *adc);
    ~STM_ADC();
    uint16_t read(uint8_t ch);
    void update_sample_rate(uint8_t sample_time);
};

#endif