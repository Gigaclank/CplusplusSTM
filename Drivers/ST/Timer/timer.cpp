/**
* @file timer.cpp
* @author Aaron Lynn ()
* @brief 
* @version 0.1
* @date 2018-12-08
* 
* @copyright Copyright (c) Aaron Lynn 2018
* All Rights Reserved
* 
*/
#include "timer.h"

TIMER *timer[16];
/**
* @brief Construct a new TIMER::TIMER object
* 
* @param TIMx 
* @param S 
*/
TIMER::TIMER(TIM_TypeDef *TIMx, double S)
{
  RCC_ClocksTypeDef RCC_Clocks;
  NVIC_InitTypeDef NVIC_InitStructure;

  uint32_t timerPeriph;
  //  uint16_t division = 0;
  uint8_t IrqChannel = 0;
  this->_TIMx = TIMx;
  RCC_GetClocksFreq(&RCC_Clocks);

  this->cnt = 0;
  this->runTime = 0;
  this->duty_max[0] = 100;
  this->duty_max[1] = 100;
  this->duty_max[2] = 100;
  this->duty_max[3] = 100;

  if (this->_TIMx == TIM1)
  {
    timerPeriph = RCC_APB2Periph_TIM1;
#if !defined(STM32F10X_MD) && !defined(STM32F10X_HD) && !defined(STM32F10X_XL)
    IrqChannel = TIM1_UP_IRQn;
#else
    IrqChannel = TIM1_UP_TIM10_IRQn;
#endif
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (this->_TIMx == TIM2)
  {
    timerPeriph = RCC_APB1Periph_TIM2;
    IrqChannel = TIM2_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (this->_TIMx == TIM3)
  {
    timerPeriph = RCC_APB1Periph_TIM3;
    IrqChannel = TIM3_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
#if defined(STM32F10X_MD) || defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (this->_TIMx == TIM4)
  {
    timerPeriph = RCC_APB1Periph_TIM4;
    IrqChannel = TIM4_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (this->_TIMx == TIM5)
  {
    timerPeriph = RCC_APB1Periph_TIM5;
    IrqChannel = TIM5_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (this->_TIMx == TIM6)
  {
    timerPeriph = RCC_APB1Periph_TIM6;
    IrqChannel = TIM6_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (this->_TIMx == TIM7)
  {
    timerPeriph = RCC_APB1Periph_TIM7;
    IrqChannel = TIM7_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
#if defined(STM32F10X_XL)
  else if (this->_TIMx == TIM8)
  {
    timerPeriph = RCC_APB2Periph_TIM8;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (this->_TIMx == TIM9)
  {
    timerPeriph = RCC_APB2Periph_TIM9;
    IrqChannel = TIM1_BRK_TIM9_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (this->_TIMx == TIM10)
  {
    timerPeriph = RCC_APB2Periph_TIM10;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (this->_TIMx == TIM11)
  {
    timerPeriph = RCC_APB2Periph_TIM11;
    IrqChannel = TIM1_TRG_COM_TIM11_IRQn;
    this->freq = RCC_Clocks.PCLK2_Frequency;
  }
  else if (this->_TIMx == TIM12)
  {
    timerPeriph = RCC_APB1Periph_TIM12;
    IrqChannel = TIM8_BRK_TIM12_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (this->_TIMx == TIM13)
  {
    timerPeriph = RCC_APB1Periph_TIM13;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
  else if (this->_TIMx == TIM14)
  {
    timerPeriph = RCC_APB1Periph_TIM14;
    IrqChannel = TIM8_TRG_COM_TIM14_IRQn;
    this->freq = RCC_Clocks.PCLK1_Frequency;
  }
#endif
#endif
#endif

  //  if (this->_TIMx == TIM1 || this->_TIMx == TIM8 || this->_TIMx == TIM9 || this->_TIMx == TIM10 || this->_TIMx == TIM11 || this->_TIMx == TIM15 || this->_TIMx == TIM16)
  if (which_source(this->_TIMx))
  {
    RCC_APB2PeriphClockCmd(timerPeriph, ENABLE); /* TIM2 Periph clock enable */
  }
  else
  {
    RCC_APB1PeriphClockCmd(timerPeriph, ENABLE);
  }

  uint16_t u16_PrescalerValue = 0;
  uint16_t u16_PeriodValue = 0;
  calculate_values(this->_TIMx, (1 / S), &u16_PeriodValue, &u16_PrescalerValue);

  TIM_TimeBaseStructInit(&this->TIM_TimeBaseStructure);
  this->TIM_TimeBaseStructure.TIM_Prescaler = u16_PrescalerValue - 1; /* Prescale to 1Mhz */
  this->TIM_TimeBaseStructure.TIM_Period = (uint16_t)(u16_PeriodValue)-1;
  this->TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  this->TIM_TimeBaseStructure.TIM_ClockDivision = 0;

  TIM_TimeBaseInit(this->_TIMx, &this->TIM_TimeBaseStructure);

  NVIC_InitStructure.NVIC_IRQChannel = IrqChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ClearFlag(this->_TIMx, TIM_FLAG_Update);      /* Clear TIM1 update pending flag */
  TIM_ITConfig(this->_TIMx, TIM_IT_Update, ENABLE); /* Enable the TIM1 Update Interrupt */
  TIM_Cmd(this->_TIMx, ENABLE);                     /* TIM1 counter enable */

  this->attach_intterupt(0);
}

/**
* @brief Deconstruct TIMER object
* 
* @param TIMx 
* @param ms 
*/
TIMER::~TIMER()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t timerPeriph;
  uint8_t IrqChannel = 0;

  this->cnt = 0;
  this->runTime = 0;

  if (this->_TIMx == TIM1)
  {
    timerPeriph = RCC_APB2Periph_TIM1;

#if !defined(STM32F10X_MD) && !defined(STM32F10X_HD) && !defined(STM32F10X_XL)
    IrqChannel = TIM1_UP_IRQn;
#else
    IrqChannel = TIM1_UP_TIM10_IRQn;

#endif
    timer_callback[0] = 0;
    timer[0] = 0;
  }
  else if (this->_TIMx == TIM2)
  {
    timerPeriph = RCC_APB1Periph_TIM2;
    IrqChannel = TIM2_IRQn;
    timer_callback[1] = 0;
    timer[1] = 0;
  }
  else if (this->_TIMx == TIM3)
  {
    timerPeriph = RCC_APB1Periph_TIM3;
    IrqChannel = TIM3_IRQn;
    timer_callback[2] = 0;
    timer[2] = 0;
  }
#if defined(STM32F10X_MD) || defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (this->_TIMx == TIM4)
  {
    timerPeriph = RCC_APB1Periph_TIM4;
    IrqChannel = TIM4_IRQn;
    timer_callback[3] = 0;
    timer[3] = 0;
  }
#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (this->_TIMx == TIM5)
  {
    timerPeriph = RCC_APB1Periph_TIM5;
    IrqChannel = TIM5_IRQn;
    timer_callback[4] = 0;
    timer[4] = 0;
  }
  else if (this->_TIMx == TIM6)
  {
    timerPeriph = RCC_APB1Periph_TIM6;
    IrqChannel = TIM6_IRQn;
    timer_callback[5] = 0;
    timer[5] = 0;
  }
  else if (this->_TIMx == TIM7)
  {
    timerPeriph = RCC_APB1Periph_TIM7;
    IrqChannel = TIM7_IRQn;
    timer_callback[6] = 0;
    timer[6] = 0;
  }
#if defined(STM32F10X_XL)
  else if (this->_TIMx == TIM8)
  {
    timerPeriph = RCC_APB2Periph_TIM8;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    timer_callback[7] = 0;
    timer[7] = 0;
  }
  else if (this->_TIMx == TIM9)
  {
    timerPeriph = RCC_APB2Periph_TIM9;
    IrqChannel = TIM1_BRK_TIM9_IRQn;
    timer_callback[8] = 0;
    timer[8] = 0;
  }
  else if (this->_TIMx == TIM10)
  {
    timerPeriph = RCC_APB2Periph_TIM10;
    IrqChannel = TIM1_UP_TIM10_IRQn;
    timer_callback[9] = 0;
    timer[9] = 0;
  }
  else if (this->_TIMx == TIM11)
  {
    timerPeriph = RCC_APB2Periph_TIM11;
    IrqChannel = TIM1_TRG_COM_TIM11_IRQn;
    timer_callback[10] = 0;
    timer[10] = 0;
  }
  else if (this->_TIMx == TIM12)
  {
    timerPeriph = RCC_APB1Periph_TIM12;
    IrqChannel = TIM8_BRK_TIM12_IRQn;
    timer_callback[11] = 0;
    timer[11] = 0;
  }
  else if (this->_TIMx == TIM13)
  {
    timerPeriph = RCC_APB1Periph_TIM13;
    IrqChannel = TIM8_UP_TIM13_IRQn;
    timer_callback[12] = 0;
    timer[12] = 0;
  }
  else if (this->_TIMx == TIM14)
  {
    timerPeriph = RCC_APB1Periph_TIM14;
    IrqChannel = TIM8_TRG_COM_TIM14_IRQn;
    timer_callback[13] = 0;
    timer[13] = 0;
  }
#endif
#endif
#endif

  //  if (this->_TIMx == TIM1 || this->_TIMx == TIM12 || this->_TIMx == TIM15 || this->_TIMx == TIM16)
  if (which_source(this->_TIMx))
    RCC_APB2PeriphClockCmd(timerPeriph, DISABLE); /* TIM2 Periph clock enable */
  else
    RCC_APB1PeriphClockCmd(timerPeriph, DISABLE);

  NVIC_InitStructure.NVIC_IRQChannel = IrqChannel;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_ClearFlag(this->_TIMx, TIM_FLAG_Update);       /* Clear TIM1 update pending flag */
  TIM_ITConfig(this->_TIMx, TIM_IT_Update, DISABLE); /* Enable the TIM1 Update Interrupt */
  TIM_Cmd(this->_TIMx, DISABLE);                     /* TIM1 counter enable */

  this->_TIMx = 0;
}

/**
* @brief attach a callback handler to the timer
* 
* @param callback 
*/
void TIMER::attach_intterupt(void (*callback)(void))
{

  if (this->_TIMx == TIM1)
  {
    timer_callback[0] = callback;
    timer[0] = this;
  }
  else if (this->_TIMx == TIM2)
  {
    timer_callback[1] = callback;
    timer[1] = this;
  }
  else if (this->_TIMx == TIM3)
  {
    timer[2] = this;
    timer_callback[2] = callback;
  }
  else if (this->_TIMx == TIM4)
  {
    timer[3] = this;
    timer_callback[3] = callback;
  }
  else if (this->_TIMx == TIM5)
  {
    timer[4] = this;
    timer_callback[4] = callback;
  }
  else if (this->_TIMx == TIM6)
  {
    timer[5] = this;
    timer_callback[5] = callback;
  }
  else if (this->_TIMx == TIM7)
  {
    timer[6] = this;
    timer_callback[6] = callback;
  }
  else if (this->_TIMx == TIM8)
  {
    timer[7] = this;
    timer_callback[7] = callback;
  }
  else if (this->_TIMx == TIM9)
  {
    timer[8] = this;
    timer_callback[8] = callback;
  }
  else if (this->_TIMx == TIM10)
  {
    timer[9] = this;
    timer_callback[9] = callback;
  }
  else if (this->_TIMx == TIM11)
  {
    timer[10] = this;
    timer_callback[10] = callback;
  }
  else if (this->_TIMx == TIM12)
  {
    timer[11] = this;
    timer_callback[11] = callback;
  }
  else if (this->_TIMx == TIM13)
  {
    timer[12] = this;
    timer_callback[12] = callback;
  }
  else if (this->_TIMx == TIM14)
  {
    timer[13] = this;
    timer_callback[13] = callback;
  }
  else if (this->_TIMx == TIM15)
  {
    timer[14] = this;
    timer_callback[14] = callback;
  }
  else if (this->_TIMx == TIM16)
  {
    timer[15] = this;
    timer_callback[15] = callback;
  }
}
/**
* @brief delay based on timer
* 
* @param delay_value 
*/
void TIMER::delay(uint16_t delay_value)
{
  this->cnt = delay_value;
  while (this->cnt > 0)
    ;
}
/**
* @brief Timer called in timer interrupt
* 
*/
void TIMER::cntr(void)
{
  if (this->cnt > 0)
    this->cnt--;
  this->runTime++;
}
/**
* @brief setup basic pwm
* 
*/
void TIMER::setup_pwm(void)
{
  TIM_CtrlPWMOutputs(this->_TIMx, ENABLE);
}

/**
* @brief setup a pwm timer on a specific channel
* 
* @param ch 
*/
void TIMER::setup_pwm(uint8_t ch, uint8_t duty)
{

  void (*init[4])(TIM_TypeDef *, TIM_OCInitTypeDef *) = {
      TIM_OC1Init,
      TIM_OC2Init,
      TIM_OC3Init,
      TIM_OC4Init,
  };

  TIM_OCStructInit(&this->TIM_OCInitStructure[ch - 1]);

  this->TIM_OCInitStructure[ch - 1].TIM_OCMode = TIM_OCMode_PWM1;
  this->TIM_OCInitStructure[ch - 1].TIM_OutputState = TIM_OutputState_Enable; // TIM_OutputState_Enable 	TIM_OutputState_Disable
  this->TIM_OCInitStructure[ch - 1].TIM_OutputNState = TIM_OutputNState_Disable;
  this->TIM_OCInitStructure[ch - 1].TIM_Pulse = (uint16_t)(this->TIM_TimeBaseStructure.TIM_Period * duty / duty_max[ch - 1]);
  this->TIM_OCInitStructure[ch - 1].TIM_OCPolarity = TIM_OCPolarity_Low;      //	TIM_OCPolarity_High			TIM_OCPolarity_Low
  this->TIM_OCInitStructure[ch - 1].TIM_OCNPolarity = TIM_OCPolarity_High;    //	TIM_OCNPolarity_High		TIM_OCNPolarity_Low
  this->TIM_OCInitStructure[ch - 1].TIM_OCIdleState = TIM_OCIdleState_Reset;  //	TIM_OCIdleState_Set			TIM_OCIdleState_Reset
  this->TIM_OCInitStructure[ch - 1].TIM_OCNIdleState = TIM_OCIdleState_Reset; //	TIM_OCNIdleState_Set		TIM_OCNIdleState_Reset

  init[ch - 1](this->_TIMx, &this->TIM_OCInitStructure[ch - 1]);

  TIM_CtrlPWMOutputs(this->_TIMx, ENABLE);
}
/**
* @brief invert the pwm signal for a specfic channel
* 
* @param ch 
*/
void TIMER::invert_pwm(uint8_t ch)
{
  void (*init[4])(TIM_TypeDef *, TIM_OCInitTypeDef *) = {
      TIM_OC1Init,
      TIM_OC2Init,
      TIM_OC3Init,
      TIM_OC4Init,
  };

  if (this->TIM_OCInitStructure[ch - 1].TIM_OCPolarity == TIM_OCPolarity_Low)
    this->TIM_OCInitStructure[ch - 1].TIM_OCPolarity = TIM_OCPolarity_High;
  else
    this->TIM_OCInitStructure[ch - 1].TIM_OCPolarity = TIM_OCPolarity_Low;

  init[ch - 1](this->_TIMx, &this->TIM_OCInitStructure[ch - 1]);
}

/**
* @brief updae duty cycle of pwm
* 
* @param ch 
* @param duty 
*/
void TIMER::update_pwm(uint8_t ch, uint8_t duty)
{
  void (*update[4])(TIM_TypeDef *, uint16_t) = {
      TIM_SetCompare1,
      TIM_SetCompare2,
      TIM_SetCompare3,
      TIM_SetCompare4,
  };
  update[ch - 1](this->_TIMx, (uint16_t)(this->TIM_TimeBaseStructure.TIM_Period * duty / duty_max[ch - 1]));
}
/**
* @brief update duty cycle of pwm
* 
* @param ch 
* @param duty 
*/
void TIMER::update_pwm(uint8_t ch, float duty)
{
  void (*update[4])(TIM_TypeDef *, uint16_t) = {
      TIM_SetCompare1,
      TIM_SetCompare2,
      TIM_SetCompare3,
      TIM_SetCompare4,
  };
  update[ch - 1](this->_TIMx, (uint16_t)(this->TIM_TimeBaseStructure.TIM_Period * duty / duty_max[ch - 1]));
}
/**
* @brief set the max duty for all channels
* 
* @param max 
*/
void TIMER::set_max_duty(uint8_t max)
{
  this->duty_max[0] = max;
  this->duty_max[1] = max;
  this->duty_max[2] = max;
  this->duty_max[3] = max;
}
/**
* @brief set the max duty for a sepcfic channel
* 
* @param ch 
* @param max 
*/
void TIMER::set_max_duty(uint8_t ch, uint8_t max)
{
  this->duty_max[ch - 1] = max;
}

/**
 * @brief calculate the values required to obtain the frequency
 * 
 * @param TIMx 
 * @param freq 
 * @param arr 
 * @param psc 
 */
void TIMER::calculate_values(TIM_TypeDef *TIMx, float freq, uint16_t *arr, uint16_t *psc)
{

  RCC_ClocksTypeDef RCC_Clocks;
  ScaleFactor m_scale;
  unsigned int period = PeriodFromFrequency(freq, &m_scale);
  ;

  unsigned int scale = ((unsigned int)m_scale);
  unsigned int clk;
  unsigned int prescaler;

  unsigned int PWM1_CLK_HZ;
  unsigned int PWM2_CLK_HZ;
  unsigned int ONE_MHZ = 1000000;
  unsigned int PWM1_CLK_MHZ = 72;
  unsigned int PWM2_CLK_MHZ = 36;
  unsigned int PWM_MAX_CLK_MHZ;

  unsigned int SYSTEM_CYCLE_CLOCK_HZ = 72000000;
  unsigned int SYSTEM_APB1_CLOCK_HZ = 72000000;
  unsigned int SYSTEM_APB2_CLOCK_HZ = 36000000;

  RCC_GetClocksFreq(&RCC_Clocks);
  SYSTEM_CYCLE_CLOCK_HZ = RCC_Clocks.SYSCLK_Frequency;
  SYSTEM_APB1_CLOCK_HZ = RCC_Clocks.PCLK1_Frequency;
  SYSTEM_APB2_CLOCK_HZ = RCC_Clocks.PCLK2_Frequency;
  PWM1_CLK_MHZ = SYSTEM_APB1_CLOCK_HZ / ONE_MHZ;
  PWM2_CLK_MHZ = SYSTEM_APB2_CLOCK_HZ / ONE_MHZ;

  if (SYSTEM_APB1_CLOCK_HZ == SYSTEM_CYCLE_CLOCK_HZ)
    PWM1_CLK_HZ = (SYSTEM_APB1_CLOCK_HZ);
  else
  {
    PWM1_CLK_HZ = (SYSTEM_APB1_CLOCK_HZ * 2);
    PWM1_CLK_MHZ = (PWM1_CLK_HZ / ONE_MHZ);
  }
  if (SYSTEM_APB2_CLOCK_HZ == SYSTEM_CYCLE_CLOCK_HZ)
    PWM2_CLK_HZ = (SYSTEM_APB2_CLOCK_HZ);
  else
  {
    PWM2_CLK_HZ = (SYSTEM_APB2_CLOCK_HZ * 2);
    PWM2_CLK_MHZ = (PWM2_CLK_HZ / ONE_MHZ);
  }
  if (PWM2_CLK_MHZ > PWM1_CLK_MHZ)
    PWM_MAX_CLK_MHZ = PWM2_CLK_MHZ;
  else
    PWM_MAX_CLK_MHZ = PWM1_CLK_MHZ;

  // APB2
  //determine which clock the timer uses
  if (which_source(TIMx))
    clk = PWM2_CLK_HZ;
  else
    clk = PWM1_CLK_HZ;

  prescaler = clk / scale;
  // scale in MHz
  unsigned int sm = scale / ONE_MHZ;
  if (which_source(TIMx))
    clk = PWM2_CLK_MHZ;
  else
    clk = PWM1_CLK_MHZ;

  cout << "Freq: " << freq << " Hz";

  if (prescaler == 0)
  {
    if (period > 0xFFFFFFFF / PWM_MAX_CLK_MHZ)
    {
      // avoid overflow
      prescaler = clk;
      period /= sm;
    }
    else
    {
      prescaler = 1;
      period = period * clk / sm;
    }
  }
  else
  {
    while (prescaler > 0x10000)
    {
      prescaler >>= 1;
      period <<= 1;
    }
  }
  if (TIMx != TIM2 && TIMx != TIM5)
  //  if (timer != 2 && timer != 5)
  {
    // 16 bit timer
    while (period >= 0x10000)
    {
      // period too large
      prescaler <<= 1;
      period >>= 1;
    }
  }
  cout << " PSC: " << prescaler;
  cout << " ARR: " << (period + 1) << "\r\n";

  *arr = (period + 1);
  *psc = prescaler;
}

/**
 * @brief Calculate the Period required for the frequency.
 * 
 * @param f 
 * @param scale 
 * @return unsigned int 
 */
unsigned int TIMER::PeriodFromFrequency(double f, ScaleFactor *scale)
{
  if (f >= 1000.0)
  {
    *scale = Nanoseconds;
    return (unsigned int)((1000000000.0 / f) + 0.5);
  }
  if (f >= 1.0)
  {
    *scale = Microseconds;
    return (unsigned int)((1000000.0 / f) + 0.5);
  }
  *scale = Milliseconds;
  return (unsigned int)((1000.0 / f) + 0.5);
}

/**
 * @brief determine which source should be used for clock source.
 * 
 * @param timer 
 * @return int 
 */
int TIMER::which_source(int timer)
{
  if (timer == 1)
    return 1;
  else if (timer == 2)
    return 0;
  else if (timer == 3)
    return 0;
#if defined(STM32F10X_MD) || defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (timer == 4)
    return 0;
#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (timer == 5)
    return 0;
  else if (timer == 6)
    return 0;
  else if (timer == 7)
    return 0;
#if defined(STM32F10X_XL)
  else if (timer == 8)
    return 1;
  else if (timer == 9)
    return 1;
  else if (timer == 10)
    return 1;
  else if (timer == 11)
    return 1;
  else if (timer == 12)
    return 0;
  else if (timer == 13)
    return 0;
  else if (timer == 14)
    return 0;
#endif
#endif
#endif
  return 0;
}

/**
 * @brief determine which source should be used for clock source.
 * 
 * @param timer 
 * @return int 
 */
int TIMER::which_source(TIM_TypeDef *timer)
{
  if (timer == TIM1)
    return 1;
  else if (timer == TIM2)
    return 0;
  else if (timer == TIM3)
    return 0;
#if defined(STM32F10X_MD) || defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (timer == TIM4)
    return 0;
#if defined(STM32F10X_HD) || defined(STM32F10X_XL)
  else if (timer == TIM5)
    return 0;
  else if (timer == TIM6)
    return 0;
  else if (timer == TIM7)
    return 0;
#if defined(STM32F10X_XL)
  else if (timer == TIM8)
    return 1;
  else if (timer == TIM9)
    return 1;
  else if (timer == TIM10)
    return 1;
  else if (timer == TIM11)
    return 1;
  else if (timer == TIM12)
    return 0;
  else if (timer == TIM13)
    return 0;
  else if (timer == TIM14)
    return 0;
#endif
#endif
#endif
  return 0;
}