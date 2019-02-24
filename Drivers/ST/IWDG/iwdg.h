/**
 * @file iwdg.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-12
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#ifndef _IWDG_H
#define _IWDG_H
#include <iostream>
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_iwdg.h"
using std::cout;

class STM_IWDG
{
  private:
  public:
    STM_IWDG(float timeout);
    void reload(void);
    uint8_t reset_source(void);
    void system_reset(void);
};
#endif
