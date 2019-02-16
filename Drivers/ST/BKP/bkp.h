/**
 * @file bkp.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#ifndef _BKP_H
#define _BKP_H
#include <iostream>
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"

class STM_BKP
{
  private:
  public:
    STM_BKP();
    ~STM_BKP();
    uint16_t read(uint16_t reg);
    void write(uint16_t reg, uint16_t val);
};

#endif