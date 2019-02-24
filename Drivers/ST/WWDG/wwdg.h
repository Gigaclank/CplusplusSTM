/**
 * @file wwdg.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#ifndef _WWDG_H
#define _WWDG_H

#include "stm32f10x_wwdg.h"
class STM_WWDG
{

  private:
  public:
    STM_WWDG();
    ~STM_WWDG();
    uint8_t reset_source(void);
};
#endif
