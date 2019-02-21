/**
 * @file adc.cpp
 * @author Aaron Lynn ()
 * @brief 
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (c) Aaron Lynn 2019
 * All Rights Reserved
 * 
 */

#include "adc.h"
/**
 * @brief Construct a new stm adc::stm adc object
 * 
 * @param adc 
 */
STM_ADC::STM_ADC(ADC_TypeDef *adc)
{

    this->_ADCx = adc;
    this->_sample_time = ADC_SampleTime_239Cycles5;
    if (this->_ADCx == ADC1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); /* Enable ADC1 clock so that we can talk to it */
    else if (this->_ADCx == ADC2)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); /* Enable ADC1 clock so that we can talk to it */
    else if (this->_ADCx == ADC3)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); /* Enable ADC1 clock so that we can talk to it */
    else
        return;
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    ADC_DeInit(this->_ADCx); /* Put everything back to power-on defaults */

    /* ADC1 Configuration */
    this->_ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  /* Configures the ADC to operate in independent or dual mode */
    this->_ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       /* Specifies whether the conversion is performed in Scan (multichannels) or Single (one channel) mode */
    this->_ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 /* Don't do continuous conversions - do them on demand */
    this->_ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /* Start conversion by software, not an external trigger */
    this->_ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              /* Conversions are 12 bit - put them in the lower 12 bits of the result */
    this->_ADC_InitStructure.ADC_NbrOfChannel = 1;                             /* Say how many channels would be used by the sequencer */
    ADC_Init(this->_ADCx, &this->_ADC_InitStructure);                          /* Now do the setup */

    ADC_Cmd(this->_ADCx, ENABLE);      /* Enable ADC1 */
    ADC_ResetCalibration(this->_ADCx); /* Enable ADC1 reset calibaration register */
    while (ADC_GetResetCalibrationStatus(this->_ADCx))
        ;                              /* Check the end of ADC1 reset calibration register */
    ADC_StartCalibration(this->_ADCx); /* Start ADC1 calibaration */
    while (ADC_GetCalibrationStatus(this->_ADCx))
        ; /* Check the end of ADC1 calibration */
}
/**
 * @brief Destroy the stm adc::stm adc object
 * 
 */
STM_ADC::~STM_ADC()
{
    ADC_DeInit(this->_ADCx);
}
/**
 * @brief read an adc channel
 * 
 * @param ch 
 * @return uint16_t 
 */
uint16_t STM_ADC::read(uint8_t ch)
{
    ADC_RegularChannelConfig(this->_ADCx, ch, 1, this->_sample_time); /* Start the conversion */
    ADC_SoftwareStartConvCmd(this->_ADCx, ENABLE);                    /* Wait until conversion completion */
    while (ADC_GetFlagStatus(this->_ADCx, ADC_FLAG_EOC) == RESET)
        ; /* Get the conversion value		*/

    return ADC_GetConversionValue(this->_ADCx);
}

/**
 * @brief update the sample time for the channel
 * 
 * @param sample_time 
 */
void STM_ADC::update_sample_rate(uint8_t sample_time)
{
    this->_sample_time = sample_time;
}