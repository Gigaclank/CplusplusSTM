/**
 * @file dac.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-24
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "dac.h"

void (*write_val[2])(uint32_t, uint16_t) = {
    DAC_SetChannel1Data,
    DAC_SetChannel2Data};
/**
 * @brief Construct a new stm dac::stm dac object
 * 
 * @param ch 
 */
STM_DAC::STM_DAC(uint8_t ch)
{
    this->_channel = ch;
    uint32_t chSelect = DAC_Channel_1;
    if (this->_channel > 1)
        chSelect = DAC_Channel_2;

    if (this->_channel == 1)
        _pin = new GPIO(GPIOA, GPIO_Pin_4, GPIO_Mode_AIN);
    else
        _pin = new GPIO(GPIOA, GPIO_Pin_5, GPIO_Mode_AIN);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    DAC_StructInit(&this->DAC_InitStructure);
    this->DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
    this->DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    this->DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
    this->DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(chSelect, &this->DAC_InitStructure);

    write_val[this->_channel - 1](DAC_Align_12b_R, 4096);
    DAC_Cmd(chSelect, ENABLE);
}

/**
 * @brief move to next step of dac when not setup with software/no trigger
 * 
 */

void STM_DAC::go(void)
{
    uint32_t chSelect = DAC_Channel_1;
    if (this->_channel > 1)
        chSelect = DAC_Channel_2;
    DAC_SoftwareTriggerCmd(chSelect, ENABLE);
}

/**
 * @brief write a value onto dac line
 * 
 * @param val 
 */
void STM_DAC::write(uint16_t val)
{
    write_val[this->_channel - 1](DAC_Align_12b_R, val);
    go();
}
/**
 * @brief change the max amplitude of dac
 * 
 * @param mode 
 */
void STM_DAC::set_amplitude(uint32_t mode)
{
    uint32_t chSelect = DAC_Channel_1;
    if (this->_channel > 1)
        chSelect = DAC_Channel_2;
    this->DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = mode;
    DAC_Init(chSelect, &this->DAC_InitStructure);
}
/**
 * @brief change the waveform output of the dac
 * DAC_WaveGeneration_None -  write custom value out to dac
 * DAC_WaveGeneration_Noise - output noise
 * DAC_WaveGeneration_Triangle - output triangle wave
 * @param mode 
 */
void STM_DAC::set_output_mode(uint32_t mode)
{
    uint32_t chSelect = DAC_Channel_1;
    if (this->_channel > 1)
        chSelect = DAC_Channel_2;
    this->DAC_InitStructure.DAC_WaveGeneration = mode;
    DAC_Init(chSelect, &this->DAC_InitStructure);
}

void STM_DAC::auto_update(uint32_t *buf, uint32_t len)
{
  uint32_t chSelect = DAC_Channel_1;
  if (this->_channel > 1)
    chSelect = DAC_Channel_2;
  /* TIM2 Configuration */
  /* Time base configuration */
  this->_t = new TIMER(TIM2,0.000125);//8khz (audio)
  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  
  /* DAC channel1 Configuration */
  this->DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  this->DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  this->DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(chSelect, &this->DAC_InitStructure);
  setup_dma(buf,len);
}

void STM_DAC::auto_update(uint32_t *buf, uint32_t len, uint32_t updateFreq)
{
  uint32_t chSelect = DAC_Channel_1;
  if (this->_channel > 1)
    chSelect = DAC_Channel_2;
  /* TIM2 Configuration */
  /* Time base configuration */
  this->_t = new TIMER(TIM2,(1.0/updateFreq));//8khz (audio)
  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  
  /* DAC channel1 Configuration */
  this->DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  this->DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  this->DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(chSelect, &this->DAC_InitStructure);
  setup_dma(buf,len);
}

void STM_DAC::setup_dma(uint32_t *buf, uint32_t len){
  
  DMA_InitTypeDef            DMA_InitStructure;
  uint32_t chSelect = DAC_Channel_1;
  if (this->_channel > 1)
    chSelect = DAC_Channel_2;
#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  /* DMA2 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
#else
  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  /* DMA2 channel4 configuration */
  DMA_DeInit(DMA2_Channel4);
#else
  /* DMA1 channel4 configuration */
  DMA_DeInit(DMA1_Channel4);
#endif
  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_CH1_ADDRESS;
  if(this->_channel >1)
    DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_CH2_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buf;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = len;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
#if !defined STM32F10X_LD_VL && !defined STM32F10X_MD_VL
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);
  /* Enable DMA2 Channel4 */
  DMA_Cmd(DMA2_Channel4, ENABLE);
#else
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  /* Enable DMA1 Channel4 */
  DMA_Cmd(DMA1_Channel4, ENABLE);
#endif
  
  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
  automatically connected to the DAC converter. */
  DAC_Cmd(chSelect, ENABLE);
  
  /* Enable DMA for DAC Channel2 */
  DAC_DMACmd(chSelect, ENABLE);
}