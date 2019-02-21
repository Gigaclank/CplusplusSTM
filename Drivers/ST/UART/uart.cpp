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
void assign_isr(uint8_t uart, USART *_class);
USART *isr_uart[5];
/**
 * @brief assign an interrupt to the call back with this specific
 * 
 * @param uart 
 * @param callback 
 */
void assign_isr(uint8_t uart, USART *_class)
{

    isr_uart[uart - 1] = _class;
}
/**
 * @brief Construct a new USART::USART object
 * 
 * @param uart 
 * @param flowControl 
 * @param baud 
 * @param WordLength 
 * @param StopBits 
 * @param parity 
 */
USART::USART(USART_TypeDef *uart, uint8_t flowControl, uint32_t baud, uint16_t WordLength, uint16_t StopBits, uint16_t parity)
{

    this->_uart = uart;
    
    if(this->_uart == USART1)
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(this->_uart == USART2)
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(this->_uart == USART3)
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    else if(this->_uart == UART4)
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    else if(this->_uart == UART5)
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    
    this->USART_InitStructure.USART_BaudRate = baud;                                      /* BaudRate = eg 57600 baud */
    this->USART_InitStructure.USART_WordLength = WordLength;                              /* Word Length = 8 Bits */
    this->USART_InitStructure.USART_StopBits = StopBits;                                  /* One Stop Bit */
    this->USART_InitStructure.USART_Parity = parity;                                      /* No parity */
    this->USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* Hardware flow control disabled (RTS and CTS signals)*/
    this->USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 /* Receive and transmit enabled */
    USART_Init(this->_uart, &this->USART_InitStructure);                                  /* USART configuration */
    USART_Cmd(this->_uart, ENABLE);
    this->cts = 0;
    this->rts = 0;
   
    init_rx_buf();
    USART_ITConfig(this->_uart, USART_IT_RXNE, ENABLE); /* Enable the UART_COM1 Receive interrupt: this interrupt is generated when the UART_COM1 receive data register is not empty */
    init_tx_buf();
    if (this->_uart == USART1)
        assign_isr(1, this);
    else if (this->_uart == USART2)
        assign_isr(2, this);
    else if (this->_uart == USART3)
        assign_isr(3, this);
    else if (this->_uart == UART4)
        assign_isr(4, this);
    else if (this->_uart == UART5)
        assign_isr(5, this);
    setup_nvic();
}
/**
 * @brief add flow control pins to the uart
 * 
 * @param rt 
 * @param ct 
 */
void USART::flowcontrol(GPIO *rt, GPIO *ct)
{
    this->cts = ct;
    this->rts = rt;
}

/**
 * @brief set up interrupts
 * 
 */
void USART::setup_nvic(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    IRQn_Type type;
    if (this->_uart == USART1)
        type = USART1_IRQn;
    else if (this->_uart == USART2)
        type = USART2_IRQn;
    else if (this->_uart == USART3)
        type = USART3_IRQn;
    else if (this->_uart == UART4)
        type = UART4_IRQn;
    else if (this->_uart == UART5)
        type = UART5_IRQn;

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
    this->st_rx_glb.wr_index_u8 = 0;
    this->st_rx_glb.rd_index_u8 = 0;
    this->st_rx_glb.cnt_u8 = 0;
    this->st_rx_glb.flags.bits.overflow = 0;
    this->st_rx_glb.flags.bits.empty = 1; //need to flag hi to show buf is empty
    this->st_rx_glb.flags.bits.flow_stop = 0;
    this->st_rx_glb.timeout_timer_u16 = 0;
    memset(this->rx_buffer_a_u8_glb, 0, sizeof(this->rx_buffer_a_u8_glb));

    return;
}

/**
 * @brief clear tx buffer
 * 
 */
void USART::init_tx_buf(void)
{
    this->st_tx_glb.wr_index_u8 = 0;
    this->st_tx_glb.rd_index_u8 = 0;
    this->st_tx_glb.cnt_u8 = 0;
    this->st_tx_glb.flags.bits.overflow = 0;
    this->st_tx_glb.flags.bits.empty = 1;
    this->st_tx_glb.flags.bits.flow_stop = 0;
    this->st_tx_glb.timeout_timer_u16 = 0;
    memset(this->tx_buffer_a_u8_glb, 0, sizeof(this->tx_buffer_a_u8_glb));

    return;
}

/**
 * @brief uart timer handler
 * 
 */
void USART::timer_control(void)
{
    if (this->st_rx_glb.timeout_timer_u16)
        this->st_rx_glb.timeout_timer_u16--;
    if (this->st_tx_glb.timeout_timer_u16)
        this->st_tx_glb.timeout_timer_u16--;

    if (this->st_tx_glb.flags.bits.flow_stop == 1)
    {
        if (this->cts->read() == UART_CTS_POLARITY_GO)
        {
            USART_ITConfig(this->_uart, USART_IT_TXE, ENABLE);
            this->st_tx_glb.flags.bits.flow_stop = 0;
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
uint8_t USART::read(uint16_t wait_time_u16, uint8_t *rec_status_u8)
{
    /* Local Variables */
    uint8_t data_byte_u8 = 0;

    /* Initialise variable */
    *rec_status_u8 = 0;
    this->st_rx_glb.timeout_timer_u16 = wait_time_u16;
    while ((this->st_rx_glb.timeout_timer_u16) && (this->st_rx_glb.cnt_u8 == 0))
        ;

    if (this->st_rx_glb.cnt_u8 != 0)
    {
        data_byte_u8 = this->rx_buffer_a_u8_glb[this->st_rx_glb.rd_index_u8];
        *rec_status_u8 = 1;

        if (++this->st_rx_glb.rd_index_u8 == UART_RX_BUFFER_SIZE)
            this->st_rx_glb.rd_index_u8 = 0;

        USART_ITConfig(this->_uart, USART_IT_RXNE, DISABLE);
        --this->st_rx_glb.cnt_u8;
        USART_ITConfig(this->_uart, USART_IT_RXNE, ENABLE);

        if (this->rts != NULL)
        { /* Check if RX flow control enabled */
            if (this->st_rx_glb.cnt_u8 <= UART_RTS_GO_TRIG)
            {
                this->st_rx_glb.flags.bits.flow_stop = 0;
                this->rts->write(0);
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
void USART::write(uint8_t tx_char_u8)
{
    /* Local Variables */
    uint8_t CTS_status = UART_CTS_POLARITY_GO;

    if (this->cts != NULL)
    {
        this->st_tx_glb.timeout_timer_u16 = UART_TX_TIMEOUT;
        do
        {
            CTS_status = this->cts->read(); /* Read state of CTS pin */
        } while ((CTS_status != UART_CTS_POLARITY_GO) && (this->st_tx_glb.timeout_timer_u16));
    }

    while (this->st_tx_glb.cnt_u8 >= UART_TX_BUFFER_SIZE)
        ; //wait until buffer is reduced

    if (this->st_tx_glb.cnt_u8 < UART_TX_BUFFER_SIZE)
    {
        this->tx_buffer_a_u8_glb[this->st_tx_glb.wr_index_u8] = tx_char_u8;
        if (++this->st_tx_glb.wr_index_u8 == UART_TX_BUFFER_SIZE)
            this->st_tx_glb.wr_index_u8 = 0;

        USART_ITConfig(this->_uart, USART_IT_TXE, DISABLE);
        this->st_tx_glb.cnt_u8++;
        USART_ITConfig(this->_uart, USART_IT_TXE, ENABLE);
        if (this->st_tx_glb.cnt_u8 == UART_TX_BUFFER_SIZE)
            this->st_tx_glb.flags.bits.overflow = 1;
    }
    else
        this->st_tx_glb.flags.bits.overflow = 1;

    //if buffer was empty then we need to restart interupt
    if (this->st_tx_glb.flags.bits.empty == 1)
        USART_ITConfig(this->_uart, USART_IT_TXE, ENABLE);

    return;
}

/**
 * @brief uart interrupt serivce routine
 * 
 */
void uart_isr(uint8_t uartCh)
{
    /* Local Variables */
    uint16_t rec_char_u16;

    /* Code */
    USART *u = isr_uart[uartCh - 1];
    if (USART_GetITStatus(u->_uart, USART_IT_FE) != RESET)
    {
        USART_ClearITPendingBit(u->_uart, USART_IT_FE);
        rec_char_u16 = USART_ReceiveData(u->_uart);
    }

    if (USART_GetITStatus(u->_uart, USART_IT_PE) != RESET)
    {
        while (USART_GetFlagStatus(u->_uart, USART_FLAG_RXNE) == RESET)
            ;

        USART_ClearITPendingBit(u->_uart, USART_IT_PE);
        rec_char_u16 = USART_ReceiveData(u->_uart);
    }
    if (USART_GetITStatus(u->_uart, USART_IT_ORE) != RESET)
    {
        USART_ClearITPendingBit(u->_uart, USART_IT_ORE);
        rec_char_u16 = USART_ReceiveData(u->_uart);
    }

    if (USART_GetITStatus(u->_uart, USART_IT_NE) != RESET)
    {
        USART_ClearITPendingBit(u->_uart, USART_IT_NE);
        rec_char_u16 = USART_ReceiveData(u->_uart);
    }

    if (USART_GetITStatus(u->_uart, USART_IT_RXNE) != RESET)
    {
        rec_char_u16 = USART_ReceiveData(u->_uart);
        if (u->st_rx_glb.cnt_u8 < UART_RX_BUFFER_SIZE)
        {
            u->rx_buffer_a_u8_glb[u->st_rx_glb.wr_index_u8] = (uint8_t)rec_char_u16;
            if (++u->st_rx_glb.wr_index_u8 == UART_RX_BUFFER_SIZE)
                u->st_rx_glb.wr_index_u8 = 0;
            if (++u->st_rx_glb.cnt_u8 == UART_RX_BUFFER_SIZE)
                u->st_rx_glb.flags.bits.overflow = 1;
        }
        else
            u->st_rx_glb.flags.bits.overflow = 1;

        if (u->rts != NULL)
        { /* Check if RX flow control enabled */
            if (u->st_rx_glb.cnt_u8 >= UART_RTS_STOP_TRIG)
            {
                u->st_rx_glb.flags.bits.flow_stop = 1;
                u->rts->write(1);
            }
        }
    }

    if (USART_GetITStatus(u->_uart, USART_IT_TXE) != RESET)
    {
        if (u->st_tx_glb.cnt_u8 > 0)
        {
            if (u->cts != NULL)
            {
                if (u->cts->read() != UART_CTS_POLARITY_GO)
                {
                    USART_ITConfig(u->_uart, USART_IT_TXE, DISABLE);
                    u->st_tx_glb.flags.bits.flow_stop = 1;
                }
                else
                {
                    u->st_tx_glb.flags.bits.empty = 0;
                    u->st_tx_glb.cnt_u8--;
                    USART_SendData(u->_uart, u->tx_buffer_a_u8_glb[u->st_tx_glb.rd_index_u8]);
                    if (++u->st_tx_glb.rd_index_u8 == UART_TX_BUFFER_SIZE)
                        u->st_tx_glb.rd_index_u8 = 0;
                }
            }else{
                u->st_tx_glb.flags.bits.empty = 0;
                u->st_tx_glb.cnt_u8--;
                USART_SendData(u->_uart, u->tx_buffer_a_u8_glb[u->st_tx_glb.rd_index_u8]);
                if (++u->st_tx_glb.rd_index_u8 == UART_TX_BUFFER_SIZE)
                  u->st_tx_glb.rd_index_u8 = 0;
            }
        }
        else
        {
            u->st_tx_glb.flags.bits.empty = 1;
            USART_ITConfig(u->_uart, USART_IT_TXE, DISABLE);
        }
    }
}