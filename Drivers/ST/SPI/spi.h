/**
 * @file spi.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */
#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "gpio.h"

typedef enum spi_mode
{
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3,
} spi_mode;

class STM_SPI
{
  private:
    SPI_TypeDef *_spi;
    void set_up_pins();
    void setup_defaults();
    SPI_InitTypeDef spi_config;

  public:
    STM_SPI(SPI_TypeDef *spi_x);
    STM_SPI(SPI_TypeDef *spi_x, spi_mode mode);
    STM_SPI(SPI_TypeDef *spi_x, uint32_t speed);
    STM_SPI(SPI_TypeDef *spi_x, spi_mode mode, uint32_t speed);

    ~STM_SPI();

    void change_mode(spi_mode mode);
    void change_speed(uint32_t speed);
    uint8_t rdwr(uint8_t wr_byte);
};

#endif