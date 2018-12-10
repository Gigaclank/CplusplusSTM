/**
 * @file uart.h
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#ifndef _UART_H
#define _UART_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define UART_RX_BUFFER_SIZE 255
#define UART_TX_BUFFER_SIZE 255
#define USART_IRQ_PREM_PRI 0
#define USART_IRQ_SUB_PRI 3



#define UART_RTS_POLARITY_GO		0
#define UART_CTS_POLARITY_GO		0
#define	UART_RTS_STOP_TRIG			60
#define	UART_RTS_GO_TRIG			30

#define UART_TX_TIMEOUT 			2000

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

class USART
{
  private:
    uint8_t rx_buffer_a_u8_glb[UART_RX_BUFFER_SIZE];
    uint8_t tx_buffer_a_u8_glb[UART_TX_BUFFER_SIZE];
    USART_TypeDef *_uart;
    GPIO_TypeDef *_cts_port, *_rts_port;
    uint16_t _cts_pin, _rts_pin;
    USART_InitTypeDef USART_InitStructure;
    GPIO *cts, *rts;
    struct
    {
        uint8_t wr_index_u8;
        uint8_t rd_index_u8;
        uint8_t cnt_u8;
        union {
            uint8_t byte;
            struct
            {
                uint8_t overflow : 1;
                uint8_t empty : 1;
                uint8_t flow_stop : 1;
                uint8_t nused : 5;
            } bits;
        } flags;
        volatile uint16_t timeout_timer_u16;
    } st_rx_glb, st_tx_glb;

    void get_pins(GPIO_TypeDef *tx_port, GPIO_TypeDef *rx_port, uint16_t *tx_pin, uint16_t *rx_pin);
    void setup_nvic(void);
    void init_rx_buf(void);
    void init_tx_buf(void);
    void isr(void);

  public:
//    USART(USART_TypeDef *uart,uint8_t flowControl = 0, uint32_t baud = 115200);
//    USART(USART_TypeDef *uart,uint8_t flowControl = 0, uint32_t baud= 115200, uint16_t WordLength = USART_WordLength_8b);
//    USART(USART_TypeDef *uart,uint8_t flowControl = 0, uint32_t baud= 115200, uint16_t WordLength= USART_WordLength_8b, uint16_t StopBits = USART_StopBits_1);
    USART(USART_TypeDef *uart,uint8_t flowControl = 0, uint32_t baud= 115200, uint16_t WordLength= USART_WordLength_8b, uint16_t StopBits = USART_StopBits_1, uint16_t parity =USART_Parity_No);

    void timer_control(void);
    uint8_t getchar(uint16_t wait_time_u16, uint8_t *rec_status_u8);
    void putchar(uint8_t tx_char_u8);
};
#endif
