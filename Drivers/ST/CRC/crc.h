/**
 * @file crc.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#ifndef _CRC_H
#define _CRC_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_crc.h"

class STM_CRC
{
  private:
  public:
    STM_CRC();
    ~STM_CRC();
    uint32_t calc_crc(void *buf, uint32_t len);
    uint32_t calc_crc(uint32_t data);
    uint32_t get_crc();
};

#endif