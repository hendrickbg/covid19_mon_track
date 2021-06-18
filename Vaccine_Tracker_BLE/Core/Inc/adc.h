/*****************************************************************************
* File Name          : adc.h
* Author             : Hêndrick Bataglin Gonçalves
* Version            : V1.0.0
* Date               : 30-April-2021
* Description        : ADC config for Vaccine Tracker application.
*******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

/* Includes  ------------------------------------------------------------------*/
#include "main.h"
#include "bluenrg_lp_ll_gpio.h"

/* Defines  ------------------------------------------------------------------*/
#define USER_SAMPLERATE       (LL_ADC_SAMPLE_RATE_28)
#define USER_DATAWIDTH        (LL_ADC_DS_DATA_WIDTH_12_BIT)
#define USER_RATIO            (LL_ADC_DS_RATIO_128)

#define BUFFER_SIZE            2

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void MX_ADC_Init(void)
 * \brief Init ADC peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void MX_ADC_Init(void);

/*!******************************************************************
 * \fn void LL_ADC_MspInit(void)
 * \brief Init ADC pins.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void LL_ADC_MspInit(void);

/*!******************************************************************
 * \fn void HT_ADC_DeInit(void)
 * \brief DeInit ADC peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_ADC_DeInit(void);

/*!******************************************************************
 * \fn void HT_ADC_ConfigDMA(void)
 * \brief Configures DMA to receive adc converted values.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_ADC_ConfigDMA(void);

/*!******************************************************************
 * \fn void HT_ADC_ReadFloatAdc(float *v_out, float *batt_lvl)
 * \brief Get float ADC values after conversion.
 *
 * \param[in]  none
 * \param[out] float *v_out                 V_out read in float.
 * \param[out] float *batt_lvl              Battery level in float.
 *
 * \retval none
 *******************************************************************/
void HT_ADC_ReadFloatAdc(float *v_out, float *batt_lvl);

/*!******************************************************************
 * \fn void HT_ADC_ReadADC(uint32_t *vout, uint32_t *battery_lvl)
 * \brief Get ADC converted result.
 *
 * \param[in]  none
 * \param[out] uint32_t *v_out                 V_out read in uint32_t.
 * \param[out] uint32_t *batt_lvl              Battery level in uint32_t.
 *
 * \retval none
 *******************************************************************/
void HT_ADC_ReadADC(uint32_t *vout, uint32_t *battery_lvl);

#endif /* __ADC_H__ */

/************************ adc.h *****END OF FILE*****************************/
