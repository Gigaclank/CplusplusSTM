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
    SPI_Init(this->_spi, &this->spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}

/**
 * @brief Construct a new stm spi::stm spi object
 * 
 * @param spi_x 
 * @param mode 
 */
STM_SPI::STM_SPI(SPI_TypeDef *spi_x, spi_mode mode)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    change_mode(mode);

    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &this->spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}
/**
 * @brief Construct a new stm spi::stm spi object
 * 
 * @param spi_x 
 * @param speed 
 */
STM_SPI::STM_SPI(SPI_TypeDef *spi_x, uint32_t speed)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    change_speed(speed);

    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &this->spi_config);
    SPI_Cmd(this->_spi, ENABLE);
}

/**
 * @brief Construct a new stm spi::stm spi object
 * 
 * @param spi_x 
 * @param mode 
 * @param speed 
 */
STM_SPI::STM_SPI(SPI_TypeDef *spi_x, spi_mode mode, uint32_t speed)
{
    this->_spi = spi_x;
    set_up_pins();
    setup_defaults();
    change_mode(mode);
    change_speed(speed);

    SPI_Cmd(this->_spi, DISABLE);
    SPI_I2S_DeInit(this->_spi);
    SPI_Init(this->_spi, &this->spi_config);
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
    _Pragma("diag_suppress=Pe177,Pe550");
    GPIO *p;
    _Pragma("diag_default=Pe177,Pe550");

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
 * @brief change spi mode
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

    SPI_Init(this->_spi, &this->spi_config);
}

/**
 * @brief change spi speed
 * 
 * @param speed 
 */
void STM_SPI::change_speed(uint32_t speed)
{
    uint8_t i;
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    uint32_t last_spi1_speed_u32 = RCC_Clocks.PCLK1_Frequency / 2;
    for (i = 0; i < 10; i++)
    {
        if (last_spi1_speed_u32 <= speed)
        {
            i = 10;
        }
        else
        {
            last_spi1_speed_u32 /= 2;
        }
    }
    this->spi_config.SPI_BaudRatePrescaler = i << 3;

    SPI_Init(this->_spi, &this->spi_config);
}

/**
 * @brief read/write data onto spi bus
 * 
 * @param wr_byte 
 * @return uint8_t 
 */
uint8_t STM_SPI::rdwr(uint8_t wr_byte)
{
    uint16_t timeout = 0;
    while (SPI_I2S_GetFlagStatus(this->_spi, SPI_I2S_FLAG_TXE) == RESET && timeout++ != 0xFFFF)
        ; /*loop while DR register is not empty */
    SPI_I2S_SendData(this->_spi, wr_byte);
    timeout = 0;
    while (SPI_I2S_GetFlagStatus(this->_spi, SPI_I2S_FLAG_RXNE) == RESET && timeout++ != 0xFFFF)
        ; /*loop while DR register is not empty */
    return SPI_I2S_ReceiveData(this->_spi);
}
