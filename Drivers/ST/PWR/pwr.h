/**
 * @file pwr.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
#ifndef _PWR_H
#define _PWR_H

#include "stm32f10x_pwr.h"

typedef enum pwr_voltage
{
    PWR_2V2 = 0x00000000,
    PWR_2V3 = 0x00000020,
    PWR_2V4 = 0x00000040,
    PWR_2V5 = 0x00000060,
    PWR_2V6 = 0x00000080,
    PWR_2V7 = 0x000000A0,
    PWR_2V8 = 0x000000C0,
    PWR_2V9 = 0x000000E0
} pwr_voltage;

class STM_PWR
{
  private:
  public:
    STM_PWR();
    STM_PWR(pwr_voltage voltage);
    ~STM_PWR();

    void enable_wakeup_pin(void);
    void standby(void);
    void stop(void);
    void stop(uint8_t mode);
    void stop(uint8_t regulator, uint8_t mode);
    void clear_wakeup(void);
};
#endif