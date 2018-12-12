/**
 * @file rtc.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-11
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#ifndef _RTC_H
#define _RTC_H

#include <time.h>
#include <stdint.h>
#include "stm32f10x_rtc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_it.h"

#define  RTC_SRC_EXTERNAL 0
#define  RTC_SRC_INTERNAL 1
    
class RTC_CL
{
  public:
    RTC_CL(uint8_t source = RTC_SRC_EXTERNAL);
    void attach_interrupt(void (*tick)(void),void (*alarm)(void) =0);
    void write(time_t val);
    time_t read(void);
};

#endif