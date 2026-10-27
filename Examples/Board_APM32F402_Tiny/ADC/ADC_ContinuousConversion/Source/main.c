/**
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.0
 *
 * @date        2024-12-01
 *
 * @attention
 *
 *  Copyright (C) 2024-2025 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes ***************************************************************/
#include "main.h"

/* Private includes *******************************************************/
#include <stdio.h>

/* Private macro **********************************************************/
/* printf function configs to USART1 */
#define DEBUG_USART  USART1

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void ADC_Init(void);
void Delay(uint32_t count);

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    USART_Config_T USART_ConfigStruct;

    SystemClockConfig();

    /* USART config */
    USART_ConfigStructInit(&USART_ConfigStruct);
    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &USART_ConfigStruct);

    /* ADC1 initialization */
    ADC_Init();

    /* Infinite loop */
    while (1)
    {
        
    }
}

/*!
 * @brief       This function handles ADC global interrupt request
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void ADC_IRQRequest(void)
{
    uint16_t adcData = 0;
    uint16_t voltage = 0;

    /* Read ADC Conversion value */
    adcData = ADC_ReadConversionValue(ADC1);
    /* voltage(mV) = adcData * (3300mV / 4095) */
    voltage = (adcData * 3300) / 4095;
    printf("voltage : %d mV\r\n", voltage);
}

/*!
 * @brief     ADC Init
 *
 * @param     None
 *
 * @retval    None
 */
void ADC_Init(void)
{
    GPIO_Config_T GPIO_ConfigStruct;
    ADC_Config_T  ADC_ConfigStruct;

    /* Enable GPIOA clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    /* ADC channel 0 configuration */
    GPIO_ConfigStructInit(&GPIO_ConfigStruct);
    GPIO_ConfigStruct.mode    = GPIO_MODE_ANALOG;
    GPIO_ConfigStruct.pin     = GPIO_PIN_0;
    GPIO_Config(GPIOA, &GPIO_ConfigStruct);

    /* ADCCLK = PCLK2/4 */
    RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

    /* Enable ADC clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);

    /* ADC configuration */
    ADC_Reset(ADC1);
    ADC_ConfigStructInit(&ADC_ConfigStruct);
    ADC_ConfigStruct.mode                  = ADC_MODE_INDEPENDENT;
    ADC_ConfigStruct.scanConvMode          = DISABLE;
    ADC_ConfigStruct.continuousConvMode    = ENABLE;
    ADC_ConfigStruct.externalTrigConv      = ADC_EXT_TRIG_CONV_NONE;
    ADC_ConfigStruct.dataAlign             = ADC_DATA_ALIGN_RIGHT;
    /* channel number */
    ADC_ConfigStruct.nbrOfChannel          = 1;
    ADC_Config(ADC1, &ADC_ConfigStruct);

    /* ADC channel Convert configuration */
    ADC_ConfigRegularChannel(ADC1, ADC_CHANNEL_0, 1, ADC_SAMPLETIME_13CYCLES5);

    /* Enable complete conversion interupt */
    ADC_EnableInterrupt(ADC1, ADC_INT_EOC);

    /* NVIC configuration */
    NVIC_EnableIRQRequest(ADC1_2_IRQn, 1, 1);

    /* Enable ADC */
    ADC_Enable(ADC1);

    /* Enable ADC1 reset calibration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while (ADC_ReadResetCalibrationStatus(ADC1));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while (ADC_ReadCalibrationStartFlag(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_EnableSoftwareStartConv(ADC1);
}

/*!
 * @brief     Delay
 *
 * @param     count:  delay count
 *
 * @retval    None
 */
void Delay(uint32_t count)
{
    uint16_t i = 0;

    while (count--)
    {
        i = 7995;
        while (i--);
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, ch);

    /* wait for the data to be send */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
	UNUSED(file);

    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
