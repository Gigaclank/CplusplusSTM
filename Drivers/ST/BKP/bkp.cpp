/**
 * @file bkp.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
/**
 * Available Registers
 * BKP_DR1
 * BKP_DR2
 * BKP_DR3
 * BKP_DR4
 * BKP_DR5
 * BKP_DR6
 * BKP_DR7
 * BKP_DR8
 * BKP_DR9
 * BKP_DR10
 * BKP_DR11
 * BKP_DR12
 * BKP_DR13
 * BKP_DR14
 * BKP_DR15
 * BKP_DR16
 * BKP_DR17
 * BKP_DR18
 * BKP_DR19
 * BKP_DR20
 * BKP_DR21
 * BKP_DR22
 * BKP_DR23
 * BKP_DR24
 * BKP_DR25
 * BKP_DR26
 * BKP_DR27
 * BKP_DR28
 * BKP_DR29
 * BKP_DR30
 * BKP_DR31
 * BKP_DR32
 * BKP_DR33
 * BKP_DR34
 * BKP_DR35
 * BKP_DR36
 * BKP_DR37
 * BKP_DR38
 * BKP_DR39
 * BKP_DR40
 * BKP_DR41
 * BKP_DR42
 **/

#include "bkp.h"
/**
 * @brief Construct a new stm bkp::stm bkp object
 * 
 */
STM_BKP::STM_BKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // Enable PWR and BKP clocks
    PWR_BackupAccessCmd(ENABLE);                                             // Allow access to BKP Domain
}
/**
 * @brief Destroy the stm bkp::stm bkp object
 * 
 */
STM_BKP::~STM_BKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // Enable PWR and BKP clocks
    PWR_BackupAccessCmd(ENABLE);                                             // Allow access to BKP Domain
}
/**
 * @brief read a register from the backup register
 * @note available registers: 
 *
 * @param reg 
 * @return uint16_t 
 */
uint16_t STM_BKP::read(uint16_t reg)
{
    return BKP_ReadBackupRegister(reg);
}

/**
 * @brief write a value to the register
 * 
 * @param reg 
 * @param val 
 */
void STM_BKP::write(uint16_t reg, uint16_t val)
{
    BKP_WriteBackupRegister(reg, val);
}