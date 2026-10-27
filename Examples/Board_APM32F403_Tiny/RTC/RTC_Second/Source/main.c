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
#define DEBUG_USART COM1_PORT

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void RTC_Config(void);
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
    COM_Config();
    RTC_Config();

    printf("This example is for RTC second.\r\n");
    /* Infinite loop */
    while (1)
    {
    }
}

/**
 * @brief   USART Configuration
 *
 * @param   None
 *
 * @retval  None
 */
void COM_Config(void)
{
    USART_Config_T usartConfigStruct;

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;

    BOARD_COM_Config(COM1, &usartConfigStruct);
}

/**
 * @brief   RTC Configuration
 *
 * @param   None
 *
 * @retval  None
 */
void RTC_Config(void)
{
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);

    /* unLock RTC write protection */
    PMU_EnableBackupAccess();
    RCM_EnableLSI();
    while(RCM_ReadStatusFlag(RCM_FLAG_LSIRDY) == RESET);
    RCM_ConfigRTCCLK(RCM_RTCCLK_LSI);
    RCM_EnableRTCCLK();

    /* Wait for RTC clock source synchro */
    RTC_WaitForSynchro();
    RTC_WaitForLastTask();

    RTC_ConfigPrescaler(50000);
    RTC_WaitForLastTask();

    /* Configure counter 0 */
    RTC_ConfigCounter(0);
    RTC_WaitForLastTask();

    /* Enable RTC alarm interrupt */
    RTC_EnableInterrupt(RTC_INT_SEC);
    RTC_WaitForLastTask();

    NVIC_EnableIRQRequest(RTC_IRQn, 0, 0);
}

/**
 * @brief   RTC interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void RTC_Isr(void)
{
    if(RTC_ReadIntFlag(RTC_INT_SEC))
    {
        uint32_t time, hour, minute, second;
        time = RTC_ReadCounter();
        hour = (time / 3600) % 24;
        minute = (time / 60) % 60;
        second = time % 60;
        printf("%ldh:%ldm:%lds\r\n", hour, minute, second);
        BOARD_LED_Toggle(LED2);
        RTC_ClearIntFlag(RTC_INT_SEC);
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
int fputc(int ch, FILE *f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
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
