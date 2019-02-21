/**
 * @file print.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#ifndef _PRINT_H
#define _PRINT_H
#include <stdio.h>
#include <iostream>
#include <yfuns.h>
#include "uart.h"

class PRINT
{
  private:

    

  public:
    USART *u_print;
    PRINT();
    void assign_uart(USART *u);
    
};
extern PRINT p;
#endif