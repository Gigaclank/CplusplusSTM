/**
 * @file rtc.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2018-12-11
 * 
 * @copyright Copyright (c) Aaron Lynn 2018
 * All Rights Reserved
 * 
 */
#include "rtc.h"

RTC_CL *rtc;
RTC_CL::RTC_CL(uint8_t source)
{
  uint32_t prescale;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE); // Enable PWR and BKP clocks
  
  BKP_DeInit(); //already read the register - no longer need to use the peripheral
  
  if (source == RTC_SRC_EXTERNAL)
  {
    prescale = 32767;
    RCC_LSEConfig(RCC_LSE_ON); // Enable LSE
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
      ; //wait until the flag is ready
    
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // Select LSE as RTC Clock Source
  }
  if (source == RTC_SRC_INTERNAL)
  { //use internal LSI CLK 40kHz
    prescale = 40001;
    RCC_LSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
      ; // Wait till LSI is ready
    
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //use internal LSI CLK 40kHZ
  }
  RCC_RTCCLKCmd(ENABLE);      // Enable RTC Clock
  RTC_WaitForSynchro();       // Wait for RTC registers synchronization
  RTC_WaitForLastTask();      // Wait until last write operation on RTC registers has finished
  RTC_SetPrescaler(prescale); //RTC period = RTCCLK/RTC_PR
  RTC_WaitForLastTask();      // Wait until last write operation on RTC registers has finished
  rtc = this;
  // Wait until last write operation on RTC registers has finished
  
}

/**
 * @brief attach an interrupt handler to the rtc interrupt
 * 
 * @param callback 
 */
void RTC_CL::attach_interrupt(void (*tick)(void),void (*alarm)(void))
{
  NVIC_InitTypeDef 	NVIC_InitStructure;
  
  if(tick){
    /* Enable the RTC Interrupt */
    
    RTC_ITConfig(RTC_IT_SEC, ENABLE);					// Enable the RTC Second
    
    NVIC_InitStructure.NVIC_IRQChannel						= RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    rtc_callback[0] = tick;		
  }
  if(alarm){
    
    RTC_ITConfig(RTC_IT_ALR, ENABLE);					// Enable the RTC ALARM
    NVIC_InitStructure.NVIC_IRQChannel						= RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    rtc_callback[1] = alarm;
    
  }
  
  RTC_WaitForLastTask();
}

/**
 * @brief write the time into the rtc clock
 * 
 * @param val 
 */
void RTC_CL::write(time_t val)
{
    RTC_WaitForLastTask();         //wait before writing
    RTC_SetCounter((uint32_t)val); //write the time
    RTC_WaitForLastTask();         //wait to ensure completion
}

/**
 * @brief read the time
 * 
 * @return time_t 
 */
time_t RTC_CL::read(void)
{
    return (time_t)RTC_GetCounter();
}

/**
 * @brief override the weak function time
 * 
 * @param timer 
 * @return time_t 
 */
time_t time(time_t *timer)
{
    if (timer != 0) rtc->write(*timer);
    return rtc->read();
}
