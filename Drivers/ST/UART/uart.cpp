/**
 * @file uart.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "uart.h"
void assign_isr(uint8_t uart, void (*callback)(void));
void (*uart_callback[5])(void);

void assign_isr(uint8_t uart, void (*callback)(void))
{

    uart_callback[uart] = callback;
}

USART::USART(USART_TypeDef *uart, uint8_t flowControl, uint32_t baud, uint16_t WordLength, uint16_t StopBits, uint16_t parity)
{
    GPIO_TypeDef *_tx_port = 0, *_rx_port = 0;
    uint16_t _tx_pin, _rx_pin;
    _uart = uart;
    get_pins(_tx_port, _rx_port, &_tx_pin, &_rx_pin);
    GPIO tx(_tx_port, _tx_pin, GPIO_Mode_AF_PP), rx(_rx_port, _rx_pin, GPIO_Mode_AF_PP); //setup pins

    USART_InitStructure.USART_BaudRate = baud;                                      /* BaudRate = eg 57600 baud */
    USART_InitStructure.USART_WordLength = WordLength;                              /* Word Length = 8 Bits */
    USART_InitStructure.USART_StopBits = StopBits;                                  /* One Stop Bit */
    USART_InitStructure.USART_Parity = parity;                                      /* No parity */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Hardware flow control disabled (RTS and CTS signals)*/
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* Receive and transmit enabled */
    USART_Init(_uart, &USART_InitStructure);                                        /* USART configuration */
    USART_Cmd(_uart, ENABLE);
    if (flowControl)
    {
        cts = new GPIO(_cts_port, _cts_pin);
        rts = new GPIO(_rts_port, _rts_pin);
    }

    init_rx_buf();
    USART_ITConfig(_uart, USART_IT_RXNE, ENABLE); /* Enable the UART_COM1 Receive interrupt: this interrupt is generated when the UART_COM1 receive data register is not empty */
    init_tx_buf();
    setup_nvic();
}

/**
 * @brief find out the pins for the uart lines
 * 
 * @param tx_port 
 * @param rx_port 
 * @param tx_pin 
 * @param rx_pin 
 */
void USART::get_pins(GPIO_TypeDef *tx_port, GPIO_TypeDef *rx_port, uint16_t *tx_pin, uint16_t *rx_pin)
{
    if (_uart == USART1)
    {
        tx_port = GPIOA;
        rx_port = GPIOA;
        *tx_pin = GPIO_Pin_9;
        *rx_pin = GPIO_Pin_10;
    }
    else if (_uart == USART2)
    {
    }
    else if (_uart == USART3)
    {
    }
    else if (_uart == UART4)
    {
    }
    else if (_uart == UART5)
    {
    }
}
/**
 * @brief set up interrupts
 * 
 */
void USART::setup_nvic(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    IRQn_Type type;
    if (_uart == USART1)
    {
        type = USART1_IRQn;
    }
    else if (_uart == USART2)
    {
        type = USART2_IRQn;
    }
    else if (_uart == USART3)
    {
        type = USART3_IRQn;
    }
    else if (_uart == UART4)
    {
        type = UART4_IRQn;
    }
    else if (_uart == UART5)
    {
        type = UART5_IRQn;
    }
    // the interrupt is enabled when first byte is txed

    NVIC_InitStructure.NVIC_IRQChannel = type;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_IRQ_PREM_PRI;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_IRQ_SUB_PRI;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief clear rx buffer
 * 
 */
void USART::init_rx_buf(void)
{
    st_rx_glb.wr_index_u8 = 0;
    st_rx_glb.rd_index_u8 = 0;
    st_rx_glb.cnt_u8 = 0;
    st_rx_glb.flags.bits.overflow = 0;
    st_rx_glb.flags.bits.empty = 1; //need to flag hi to show buf is empty
    st_rx_glb.flags.bits.flow_stop = 0;
    st_rx_glb.timeout_timer_u16 = 0;
    memset(rx_buffer_a_u8_glb, 0, sizeof rx_buffer_a_u8_glb);

    return;
}

/**
 * @brief clear tx buffer
 * 
 */
void USART::init_tx_buf(void)
{
    st_tx_glb.wr_index_u8 = 0;
    st_tx_glb.rd_index_u8 = 0;
    st_tx_glb.cnt_u8 = 0;
    st_tx_glb.flags.bits.overflow = 0;
    st_tx_glb.flags.bits.empty = 1;
    st_tx_glb.flags.bits.flow_stop = 0;
    st_tx_glb.timeout_timer_u16 = 0;
    memset(tx_buffer_a_u8_glb, 0, sizeof tx_buffer_a_u8_glb);

    return;
}

/**
 * @brief uart interrupt serivce routine
 * 
 */
void USART::isr(void)
{
    /* Local Variables */
    uint16_t rec_char_u16;

    /* Code */
    if (USART_GetITStatus(_uart, USART_IT_FE) != RESET)
    {
        USART_ClearITPendingBit(_uart, USART_IT_FE);
        rec_char_u16 = USART_ReceiveData(_uart);
    }

    if (USART_GetITStatus(_uart, USART_IT_PE) != RESET)
    {
        while (USART_GetFlagStatus(_uart, USART_FLAG_RXNE) == RESET)
        {
        }
        USART_ClearITPendingBit(_uart, USART_IT_PE);
        rec_char_u16 = USART_ReceiveData(_uart);
    }
    if (USART_GetITStatus(_uart, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(_uart, USART_IT_ORE);
        rec_char_u16 = USART_ReceiveData(_uart);
    }

    if (USART_GetITStatus(_uart, USART_IT_NE) != RESET)
    {
        USART_ClearITPendingBit(_uart, USART_IT_NE);
        rec_char_u16 = USART_ReceiveData(_uart);
    }

    if (USART_GetITStatus(_uart, USART_IT_RXNE) != RESET)
    {
        rec_char_u16 = USART_ReceiveData(_uart);
        if (st_rx_glb.cnt_u8 < UART_RX_BUFFER_SIZE)
        {
            rx_buffer_a_u8_glb[st_rx_glb.wr_index_u8] = (uint8_t)rec_char_u16;
            if (++st_rx_glb.wr_index_u8 == UART_RX_BUFFER_SIZE)
            {
                st_rx_glb.wr_index_u8 = 0;
            }
            if (++st_rx_glb.cnt_u8 == UART_RX_BUFFER_SIZE)
            {
                st_rx_glb.flags.bits.overflow = 1;
            }
        }
        else
        {
            st_rx_glb.flags.bits.overflow = 1;
        }
        if (rts != NULL)
        { /* Check if RX flow control enabled */
            if (st_rx_glb.cnt_u8 >= UART_RTS_STOP_TRIG)
            {
                st_rx_glb.flags.bits.flow_stop = 1;
                rts->write(1);
            }
        }
    }

    if (USART_GetITStatus(_uart, USART_IT_TXE) != RESET)
    {
        if (st_tx_glb.cnt_u8 > 0)
        {
            if (cts != NULL)
            {
                if (cts->read() != UART_CTS_POLARITY_GO)
                {
                    USART_ITConfig(_uart, USART_IT_TXE, DISABLE);
                    st_tx_glb.flags.bits.flow_stop = 1;
                }
                else
                {
                    st_tx_glb.flags.bits.empty = 0;
                    st_tx_glb.cnt_u8--;
                    USART_SendData(_uart, tx_buffer_a_u8_glb[st_tx_glb.rd_index_u8]);
                    if (++st_tx_glb.rd_index_u8 == UART_TX_BUFFER_SIZE)
                    {
                        st_tx_glb.rd_index_u8 = 0;
                    }
                }
            }
        }
        else
        {
            st_tx_glb.flags.bits.empty = 1;
            USART_ITConfig(_uart, USART_IT_TXE, DISABLE);
        }
    }
}

/**
 * @brief uart timer handler
 * 
 */
void USART::timer_control(void)
{
    if (st_rx_glb.timeout_timer_u16)
    {
        st_rx_glb.timeout_timer_u16--;
    }
    if (st_tx_glb.timeout_timer_u16)
    {
        st_tx_glb.timeout_timer_u16--;
    }

    if (st_tx_glb.flags.bits.flow_stop == 1)
    {
        if (cts->read() == UART_CTS_POLARITY_GO)
        {
            USART_ITConfig(_uart, USART_IT_TXE, ENABLE);
            st_tx_glb.flags.bits.flow_stop = 0;
        }
    }
    return;
}

/**
 * @brief get rx character from buffer
 * 
 * @param wait_time_u16 
 * @param rec_status_u8 
 */
uint8_t USART::getchar(uint16_t wait_time_u16, uint8_t *rec_status_u8)
{
    /* Local Variables */
    uint8_t data_byte_u8 = 0;

    /* Initialise variable */
    *rec_status_u8 = 0;
    st_rx_glb.timeout_timer_u16 = wait_time_u16;
    while ((st_rx_glb.timeout_timer_u16) && (st_rx_glb.cnt_u8 == 0))
    {
    }

    if (st_rx_glb.cnt_u8 != 0)
    {
        data_byte_u8 = rx_buffer_a_u8_glb[st_rx_glb.rd_index_u8];
        *rec_status_u8 = 1;

        if (++st_rx_glb.rd_index_u8 == UART_RX_BUFFER_SIZE)
        {
            st_rx_glb.rd_index_u8 = 0;
        }
        USART_ITConfig(_uart, USART_IT_RXNE, DISABLE);
        --st_rx_glb.cnt_u8;
        USART_ITConfig(_uart, USART_IT_RXNE, ENABLE);

        if (rts != NULL)
        { /* Check if RX flow control enabled */
            if (st_rx_glb.cnt_u8 <= UART_RTS_GO_TRIG)
            {
                st_rx_glb.flags.bits.flow_stop = 0;
                rts->write(0);
            }
        }
    }

    return (data_byte_u8);
}

/**
 * @brief transmit character over uart
 * 
 * @param tx_char_u8 
 */
void USART::putchar(uint8_t tx_char_u8)
{
    /* Local Variables */
    uint8_t CTS_status = UART_CTS_POLARITY_GO;

    if (cts != NULL)
    {
        st_tx_glb.timeout_timer_u16 = UART_TX_TIMEOUT;
        do
        {
            CTS_status = cts->read(); /* Read state of CTS pin */
        } while ((CTS_status != UART_CTS_POLARITY_GO) && (st_tx_glb.timeout_timer_u16));
    }

    while (st_tx_glb.cnt_u8 >= UART_TX_BUFFER_SIZE)
    {
        //wait until buffer is reduced
    }

    if (st_tx_glb.cnt_u8 < UART_TX_BUFFER_SIZE)
    {
        tx_buffer_a_u8_glb[st_tx_glb.wr_index_u8] = tx_char_u8;
        if (++st_tx_glb.wr_index_u8 == UART_TX_BUFFER_SIZE)
        {
            st_tx_glb.wr_index_u8 = 0;
        }
        USART_ITConfig(_uart, USART_IT_TXE, DISABLE);
        st_tx_glb.cnt_u8++;
        USART_ITConfig(_uart, USART_IT_TXE, ENABLE);
        if (st_tx_glb.cnt_u8 == UART_TX_BUFFER_SIZE)
        {
            st_tx_glb.flags.bits.overflow = 1;
        }
    }
    else
    {
        st_tx_glb.flags.bits.overflow = 1;
    }

    //if buffer was empty then we need to restart interupt
    if (st_tx_glb.flags.bits.empty == 1)
    {
        USART_ITConfig(_uart, USART_IT_TXE, ENABLE);
    }

    return;
}