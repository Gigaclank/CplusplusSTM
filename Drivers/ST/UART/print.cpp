/**
 * @file print.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
#include "print.h"

/**
 * @brief Construct a new PRINT::PRINT object
 * 
 */
PRINT::PRINT()
{
  this->u_print = 0;
}

void PRINT::assign_uart(USART *u)
{
  this->u_print = 0;
  this->u_print = u;
}




PRINT p;
#ifdef __ICCARM__ /* IAR compiler being used */
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
      if(p.u_print)p.u_print->write(*buffer);
        *buffer++;
    }
    return (i);
}

size_t __read(int handle, unsigned char *buffer, size_t size)
{
    uint8_t err_u8 = 0;
    size_t i;
    uint8_t recChar;
    for (i = 0; i < size; i++)
    {
       if(p.u_print)p.u_print->read(0,&err_u8);
        if (err_u8 != 0)
        {
            *buffer = recChar;
            buffer++;
        }
    }

    return (i);
}

#endif

#ifdef __GNUC__
int __io_putchar(int ch)
{
  if(p.u_print)p.u_print->write(ch);
  return (ch);
}

int __io_getchar(void)
{
    int ch;
    uint8_t status;
    if(p.u_print)p.u_print->read(0,&status);
    return (ch);
}

#endif