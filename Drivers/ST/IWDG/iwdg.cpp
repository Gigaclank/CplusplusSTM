/**
 * @file iwdg.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-12
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "iwdg.h"
/**
 * @brief Construct a new IWDG::IWDG object set up the time period
 * 
 * @param timeout 
 */
STM_IWDG::STM_IWDG(float timeout)
{
    uint8_t prescale = 0;
    uint16_t divider = 0;
    if (timeout >= 0.0001 && timeout <= 0.4096)
    {
        prescale = IWDG_Prescaler_4;
        divider = 4;
    }
    if (timeout < 0.8192)
    {
        prescale = IWDG_Prescaler_8;
        divider = 4;
    }
    if (timeout <= 1.6384)
    {
        prescale = IWDG_Prescaler_16;
        divider = 16;
    }
    if (timeout <= 3.2768)
    {
        prescale = IWDG_Prescaler_32;
        divider = 32;
    }
    if (timeout <= 6.5535)
    {
        prescale = IWDG_Prescaler_64;
        divider = 64;
    }
    if (timeout <= 13.104)
    {
        prescale = IWDG_Prescaler_128;
        divider = 128;
    }
    if (timeout <= 26.208)
    {
        prescale = IWDG_Prescaler_256;
        divider = 256;
    }
    if (timeout > 26.208)
        return;

    IWDG_SetPrescaler(prescale);
    IWDG_SetReload( (uint16_t)((40000/divider)*timeout) );
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/**
 * @brief reload the iwdg
 * 
 */
void STM_IWDG::reload(void)
{
    IWDG_ReloadCounter();
}

/**
 * @brief get the reset source
 * 
 */
uint8_t STM_IWDG::reset_source(void)
{
    uint8_t source = 0;
    cout<<"\r\nLast Reset Source ... ";
    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST))
    {
        printf("Software ");
        source = 1;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PORRST))
    {
        cout<<"POR/PDR  ";
        source = 2;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST))
    {
        cout<<"WWDog    ";
        source = 3;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST))
    {
        cout<<"IWDog    ";
        source = 4;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST))
    {
        cout<<"Pin      ";
        source = 5;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST))
    {
        cout<<"LowPower ";
        source = 6;
    }

    if (source == 0)
    {
        cout<<"None";
    }

    RCC_ClearFlag(); // Clear reset flags
    return source;
}
/**
 * @brief system reset
 * 
 */
void STM_IWDG::system_reset(void)
{
    NVIC_SystemReset();
}