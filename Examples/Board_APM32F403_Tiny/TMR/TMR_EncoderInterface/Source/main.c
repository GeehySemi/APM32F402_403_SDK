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
uint16_t encoderNum = 0;

/* Private function prototypes ********************************************/
void TMR_Config(void);
void TMR_GPIO_Config(void);
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

    BOARD_Delay_Config();
    BOARD_LED_Config(LED2);

    TMR_Config();
    TMR_GPIO_Config();
    COM_Config();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        BOARD_Delay_Ms(500);
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
    TMR_ICConfig_T   tmrICConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

    /* TMR clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 8;
    tmrBaseConfig.division = 119;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &tmrBaseConfig);

    TMR_ConfigEncodeInterface(TMR1, TMR_ENCODER_MODE_TI1, TMR_IC_POLARITY_RISING, TMR_IC_POLARITY_RISING);

    tmrICConfig.channel = TMR_CHANNEL_1;
    tmrICConfig.polarity = TMR_IC_POLARITY_RISING;
    tmrICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    tmrICConfig.prescaler = TMR_IC_PSC_1;
    tmrICConfig.filter = 0x0;
    TMR_ConfigIC(TMR1, &tmrICConfig);

    tmrICConfig.channel = TMR_CHANNEL_2;
    TMR_ConfigIC(TMR1, &tmrICConfig);

    TMR_ConfigCounter(TMR1, 4);
    TMR_ClearStatusFlag(TMR1, TMR_FLAG_UPDATE);

    TMR_EnableInterrupt(TMR1, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR1_UP_IRQn, 0xF, 0xF);

    TMR_Enable(TMR1);
}

/**
 * @brief   TMR GPIO configuration
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_GPIO_Config(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Config(GPIOA, &gpioConfig);
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

/**
 * @brief   TMR update interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_update_Irs(void)
{
    if (TMR_ReadIntFlag(TMR1, TMR_INT_UPDATE))
    {
        if((TMR1->CTRL1_B.CNTDIR) != 0)
        {
            encoderNum--;
        }
        else
        {
            encoderNum++;
        }
        TMR_ConfigCounter(TMR1, 4);
        TMR_ClearIntFlag(TMR1, TMR_INT_UPDATE);
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
