/**
 * @file pwr.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "pwr.h"

STM_PWR::STM_PWR()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
}
/**
 * @brief Construct a new PWR::PWR object
 * 
 * @param voltage 
 */
STM_PWR::STM_PWR(pwr_voltage voltage)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_PVDLevelConfig(voltage);
    PWR_PVDCmd(ENABLE);
}
/**
 * @brief Destroy the PWR::PWR object
 * 
 */
STM_PWR::~STM_PWR()
{
    PWR_DeInit();
}
/**
 * @brief enable the wakeup pin
 * 
 */
void STM_PWR::enable_wakeup_pin(void)
{
    PWR_WakeUpPinCmd(ENABLE);
}
/**
 * @brief enter standby mode
 * 
 */
void STM_PWR::standby(void)
{
    PWR_EnterSTANDBYMode();
}

/**
 * @brief setup stop mode
 * 
 */
void STM_PWR::stop(void)
{
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}
/**
 * @brief setup stop mode with entry option
 * 
 * @param mode 
 */
void STM_PWR::stop(uint8_t mode)
{
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, mode);
}

/**
 * @brief setup stop mode with regulator and entry options
 * 
 * @param regulator 
 * @param mode 
 */
void STM_PWR::stop(uint8_t regulator, uint8_t mode)
{
    PWR_EnterSTOPMode(regulator, mode);
}

/**
 * @brief clear wakeup flag
 * 
 */
void STM_PWR::clear_wakeup(void)
{
    /* Check if the Wake-Up flag is set */
    if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
    {
        /* Clear Wake Up flag */
        PWR_ClearFlag(PWR_FLAG_WU);
    }
}