/**
 * @file dac.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#ifndef _DAC_H
#define _DAC_H
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
//#include "stm32f10x_tim.h"
#include "timer.h"
#include "gpio.h"

#define DMA_CH1_ADDRESS 0x40007410
#define DMA_CH2_ADDRESS 0x40007414

class STM_DAC
{
  private:
    DAC_InitTypeDef DAC_InitStructure;
    uint8_t _channel;
    GPIO *_pin;
    TIMER  *_t;
    void setup_dma(uint32_t *buf, uint32_t len);
  public:
    STM_DAC(uint8_t ch);
    void go(void);
    void write(uint16_t val);
    void set_amplitude(uint32_t mode);
    void set_output_mode(uint32_t mode);
    void auto_update(uint32_t *buf, uint32_t len);
    void auto_update(uint32_t *buf, uint32_t len, uint32_t updateFreq);
};

#endif