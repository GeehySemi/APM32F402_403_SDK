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
#include "apm32f402_403_wwdt.h"
#include "apm32f402_403_eint.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define DEBUG_USART  USART1

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
volatile uint8_t is_OverTime = 0;
static __IO u32 TimingDelay;

/* Private function prototypes ********************************************/
void SysTick_Init(void);
void SysTick_Delay_ms(__IO u32 nTime);
void TimingDelay_Decrement(void);

/** WWDT_Init */
void WWDT_Init(void);

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
    USART_Config_T usartConfigStruct;

    SystemClockConfig();

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &usartConfigStruct);

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);

    /* Clear WWDTRST flag */
    if (RCM_ReadStatusFlag(RCM_FLAG_WWDTRST)!=RESET)
    {
        BOARD_LED_On(LED3);
        RCM_ClearStatusFlag();
    }

    printf("\r\n System reset \r\n");

    printf("\r\n The overtime is from 25.85ms to 35.2ms \r\n");

    SysTick_Init();
    SysTick_Delay_ms(500);
    WWDT_Init();

    while (1)
    {
        if (is_OverTime == RESET)
        {
            BOARD_LED_Toggle(LED2);
            /* Delay 41ms , 25.85<28<35.2 ,Refresh allowed, system still running */
            SysTick_Delay_ms(28);
            WWDT_ConfigCounter(127);
            printf("running...\r\n");
        }

        if (is_OverTime == SET)
        {
            BOARD_LED_On(LED3);
            /* Delay 59ms  ,35.2<59 ,Refresh not allowed, system reset */
            SysTick_Delay_ms(59);
            WWDT_ConfigCounter(127);
        }

        if (!BOARD_BUTTON_GetState(BUTTON_KEY1))
        {
            SysTick_Delay_ms(5);
            if (!BOARD_BUTTON_GetState(BUTTON_KEY1))
            {
                is_OverTime = SET;
            }
        }
    }
}

/*!
 * @brief       WWDT configuration
 *
 * @param       None
 *
 * @retval      None
 */
void WWDT_Init(void)
{
    /* Enable WWDT clock */
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_WWDT);

    /* APM32F402: WWDT TimeBase =  PCLK1 (60MHz)/4096)/8 = 1831Hz (~0.55 ms)  */
    WWDT_ConfigTimebase(WWDT_TIME_BASE_8);

    /* Set Window value to 80; WWDT counter should be refreshed only when the counter
    is below 80 (and greater than 64) otherwise a reset will be generated  */
    WWDT_ConfigWindowData(80);

    /* Enable WWDG and set counter value to 127
     APM32F402:
     In this case the refresh window is: ~0.55 * (127-80)= 25.85ms < refresh window < ~0.55 * 64 = 35.2ms*/
    WWDT_Enable(127);
}

/*!
 * @brief       Start SysTick
 *
 * @param       None
 *
 * @retval      None
 */
void SysTick_Init(void)
{
    SystemCoreClockUpdate();
    /* SystemFrequency / 1000 = 1ms */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}

/*!
 * @brief       Precise Delay
 *
 * @param       nTime in milliseconds
 *
 * @retval      None
 */
void SysTick_Delay_ms(__IO u32 nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

/*!
 * @brief       Decrements the TimingDelay
 *
 * @param       None
 *
 * @retval      None
 */
void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0)
    {
        TimingDelay--;
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
