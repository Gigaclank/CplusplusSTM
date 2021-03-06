/**
 * @file api_system_startup.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-09
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#ifndef _API_SYSTEM_STARTUP
#define _API_SYSTEM_STARTUP

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "system_stm32f10x.h"

void api_system_init_tick(void);
void api_system_start(void);
void api_system_cntr(void);
void api_ms_delay(uint32_t count);

#endif