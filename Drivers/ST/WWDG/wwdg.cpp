/**
 * @file wwdg.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "wwdg.h"

/**
 * @brief Construct a new stm wwdg::stm wwdg object
 * 
 */
STM_WWDG::STM_WWDG(float timeout)
{
    //     uint8_t prescale = 0;
    //     uint16_t divider = 0;
    //     RCC_ClocksTypeDef RCC_Clocks;
    //     RCC_GetClocksFreq(&RCC_Clocks);
    //     uint32_t pclk = RCC_Clocks.PCLK1_Frequency;
    // #if !defined(STM32F10X_LD_VL) && !defined(STM32F10X_MD_VL) && !defined(STM32F10X_HD_VL)

    // #else

    // #endif
    //     RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
}
/**
 * @brief Destroy the stm wwdg::stm wwdg object
 * 
 */
STM_WWDG::~STM_WWDG()
{
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/**
 * @brief get the reset source
 * 
 */
uint8_t STM_WWDG::reset_source(void)
{
    uint8_t source = 0;
    cout << "\r\nLast Reset Source ... ";
    if (RCC_GetFlagStatus(RCC_FLAG_SFTRST))
    {
        printf("Software ");
        source = 1;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PORRST))
    {
        cout << "POR/PDR  ";
        source = 2;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST))
    {
        cout << "WWDog    ";
        source = 3;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST))
    {
        cout << "IWDog    ";
        source = 4;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST))
    {
        cout << "Pin      ";
        source = 5;
    }
    else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST))
    {
        cout << "LowPower ";
        source = 6;
    }

    if (source == 0)
    {
        cout << "None";
    }

    RCC_ClearFlag(); // Clear reset flags
    return source;
}