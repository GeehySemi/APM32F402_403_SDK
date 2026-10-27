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

/* Private macro **********************************************************/
#define DEBUG_USART USART1

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint32_t countValue = 0;

/* Private function prototypes ********************************************/
void TMR_Config(void);
void COM_Config(void);

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_Delay_Config();

    TMR_Config();
    COM_Config();

    /* Infinite loop */
    while (1)
    {
        countValue = (TMR_ReadCounter(TMR3) << 16) + TMR_ReadCounter(TMR2);
        printf("countValue = %ld\r\n", countValue);
        BOARD_Delay_Ms(1000);
    }
}

/**
 * @brief   TMR configuration
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_Config(void)
{
    TMR_BaseConfig_T tmrBaseConfig;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2 | RCM_APB1_PERIPH_TMR3);

    /* TMR1 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 0xFFFF;
    tmrBaseConfig.division = 59999;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR2, &tmrBaseConfig);
    TMR_EnableAutoReload(TMR2);

    TMR_SelectSlaveMode(TMR2, TMR_SLAVE_MODE_RESET);
    TMR_ConfigUpdateRequest(TMR2, TMR_UPDATE_SOURCE_REGULAR);
    TMR_SelectOutputTrigger(TMR2, TMR_TRGO_SOURCE_UPDATE);
    TMR_EnableMasterSlaveMode(TMR2);

    tmrBaseConfig.division = 0;
    TMR_ConfigTimeBase(TMR3, &tmrBaseConfig);
    TMR_ConfigPrescaler(TMR3, 0, TMR_PSC_RELOAD_UPDATE);
    TMR_EnableUpdate(TMR3);

    TMR_SelectInputTrigger(TMR3, TMR_TRIGGER_SOURCE_ITR1);
    TMR_SelectSlaveMode(TMR3, TMR_SLAVE_MODE_EXTERNAL1);
    TMR_EnableMasterSlaveMode(TMR3);
    TMR_EnableAutoReload(TMR3);

    TMR_Enable(TMR2);
    TMR_Enable(TMR3);
}

/**
 * @brief   Com configuration
 *
 * @param   None
 *
 * @retval  None
 */
void COM_Config(void)
{
    USART_Config_T usartConfig;

    usartConfig.baudRate = 115200;
    usartConfig.wordLength = USART_WORD_LEN_8B;
    usartConfig.stopBits = USART_STOP_BIT_1;
    usartConfig.parity = USART_PARITY_NONE;
    usartConfig.mode = USART_MODE_TX_RX;
    usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    BOARD_COM_Config(COM1, &usartConfig);
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
int fputc(int ch, FILE *f)
{
        /* send a byte of data to the serial port */
        USART_TxData(DEBUG_USART,(uint8_t)ch);

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

    /* wait for the data to be send  */
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
int _write(int file, char *ptr, int len)
{
    int i;
    UNUSED(file);
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
  #warning Not supported compiler type
#endif
