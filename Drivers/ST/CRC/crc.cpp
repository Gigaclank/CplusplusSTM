/**
 * @file crc.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "crc.h"

/**
 * @brief CRC calculated upon 0x4C11DB7 polynomial
 * 
 */

/**
 * @brief Construct a new stm crc::stm crc object
 * 
 */
STM_CRC::STM_CRC()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    CRC_ResetDR();
}

/**
 * @brief Destroy the stm crc::stmc crc object
 * 
 */
STM_CRC::~STM_CRC()
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
}

/**
 * @brief calculate crc from a buffer
 * 
 * @param buf 
 * @return uint32_t 
 */
uint32_t STM_CRC::calc_crc(void *buf, uint32_t len)
{
  CRC_ResetDR();
    return CRC_CalcBlockCRC((uint32_t *)buf, len);
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint32_t 
 */
uint32_t STM_CRC::calc_crc(uint32_t data)
{
  CRC_ResetDR();
    return CRC_CalcCRC(data);
//  return 0;
}

/**
 * @brief return current crc
 * 
 * @return uint32_t 
 */
uint32_t STM_CRC::get_crc()
{
    return CRC_GetCRC();
}
