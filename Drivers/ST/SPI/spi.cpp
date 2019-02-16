/**
 * @file spi.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-16
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "spi.h"

/**
 * @brief Construct a new stm spi::stm spi object
 * 
 * @param spi_x 
 */
STM_SPI::STM_SPI(SPI_TypeDef *spi_x)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}

STM_SPI::STM_SPI(SPI_TypeDef *spi_x, spi_mode mode)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    change_mode(mode);
    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}

STM_SPI::STM_SPI(SPI_TypeDef *spi_x, spi_mode mode, uint16_t speed)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    change_mode(mode);
    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}

/**
 * @brief Destroy the stm spi::stm spi object
 * 
 */
STM_SPI::~STM_SPI()
{
    if (this->_spi == SPI1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
    else
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE); /* SPI Periph clock enable */
    SPI_Cmd(this->_spi, DISABLE);
}

/**
 * @brief Construct a new stm spi::set up pins object
 * 
 */
void STM_SPI::set_up_pins()
{
    GPIO *p;
    if (this->_spi == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        p = new GPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_AF_PP);
        p = new GPIO(GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP);
        p = new GPIO(GPIOA, GPIO_Pin_7, GPIO_Mode_AF_PP);
    }
    else if (this->_spi == SPI2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); /* SPI Periph clock enable */
        p = new GPIO(GPIOB, GPIO_Pin_13, GPIO_Mode_AF_PP);
        p = new GPIO(GPIOB, GPIO_Pin_14, GPIO_Mode_AF_PP);
        p = new GPIO(GPIOB, GPIO_Pin_15, GPIO_Mode_AF_PP);
    }
    else
        cout << "unknwon spi\r\n";
}

/**
 * @brief initalize class defaults
 * 
 */
void STM_SPI::setup_defaults()
{
    this->spi_config.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    this->spi_config.SPI_Mode = SPI_Mode_Master;
    this->spi_config.SPI_DataSize = SPI_DataSize_8b;
    this->spi_config.SPI_CPOL = SPI_CPOL_Low;
    this->spi_config.SPI_CPHA = SPI_CPHA_1Edge;
    this->spi_config.SPI_NSS = SPI_NSS_Soft;
    this->spi_config.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    this->spi_config.SPI_FirstBit = SPI_FirstBit_MSB;
}

/**
 * @brief 
 * 
 * @param mode 
 */
void STM_SPI::change_mode(spi_mode mode)
{
    switch (mode)
    {
    case SPI_MODE_0:
        this->spi_config.SPI_CPOL = SPI_CPOL_Low;
        this->spi_config.SPI_CPHA = SPI_CPHA_1Edge;

        break;
    case SPI_MODE_1:
        this->spi_config.SPI_CPOL = SPI_CPOL_Low;
        this->spi_config.SPI_CPHA = SPI_CPHA_2Edge;
        break;
    case SPI_MODE_2:
        this->spi_config.SPI_CPOL = SPI_CPOL_High;
        this->spi_config.SPI_CPHA = SPI_CPHA_1Edge;
        break;
    case SPI_MODE_3:
        this->spi_config.SPI_CPOL = SPI_CPOL_High;
        this->spi_config.SPI_CPHA = SPI_CPHA_2Edge;
        break;
    }
}