/**
 * @file api_system_startup.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-09
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */

/**
 * @brief Start up STM32 with system tick and vector table
 * 
 */
#include "api_system_startup.h"

void SystemStart(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    init_SysTick_Configuration();

    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* Configure the Priority Group to 2 bits */
}

/**
 * @brief Set up system tick to 1ms
 * 
 */
void init_SysTick_Configuration(void)
{
    /* Code */
    NVIC_SetPriority(SysTick_IRQn, 0x0);  /* Configure the SysTick handler priority */
    SysTick->CTRL |= SysTick_CTRL_ENABLE; /* Enable the SysTick Counter */
    SysTick->VAL = (uint32_t)0x0;         /* Clear the SysTick Counter */

    if (SysTick_Config(SystemCoreClock / 1000))
    { /* Setup SysTick Timer for 1 msec interrupts  */
        while (1)
            ; /* Capture error */
    }

    return;
}