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
void TMR_Config(void);
void TMR_GPIO_Config(void);
void COM_Config(void);

/* External variables *****************************************************/
uint8_t captureState = 0;
uint32_t captureValue[2] = {0};
uint32_t frequency = 0;
volatile uint32_t updateTime = 0;

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

    TMR_GPIO_Config();
    TMR_Config();
    COM_Config();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
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
    TMR_ICConfig_T tmrICConfig;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

    /* TMR clock source frequency = 60MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 999;
    tmrBaseConfig.division = 0;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR2, &tmrBaseConfig);

    tmrICConfig.channel = TMR_CHANNEL_2;
    tmrICConfig.polarity = TMR_IC_POLARITY_RISING;
    tmrICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    tmrICConfig.prescaler = TMR_IC_PSC_1;
    tmrICConfig.filter = 0;
    TMR_ConfigIC(TMR2, &tmrICConfig);

    TMR_EnableInterrupt(TMR2, TMR_INT_CC2);
    TMR_EnableInterrupt(TMR2, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR2_IRQn, 0xF, 0xF);

    TMR_Enable(TMR2);
}

/**
 * @brief   PWM output pin configuration
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
    gpioConfig.pin = GPIO_PIN_1;
    GPIO_Config(GPIOA, &gpioConfig);
}

/**
 * @brief   USART configuration
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
 * @brief   TMR compare or capture interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_CC_Irs(void)
{
    uint32_t tempValue = 0;
    uint32_t pclk1 = 0;

    RCM_ReadPCLKFreq(&pclk1, 0);
  
    if (TMR_ReadIntFlag(TMR2, TMR_INT_UPDATE))
    {
        updateTime++;
        TMR_ClearIntFlag(TMR2, TMR_INT_UPDATE);
    }

    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC2))
    {
        /* Read CC2 value */
        captureValue[captureState] = TMR_ReadCaputer2(TMR2);

        if (captureState != 0)
        {
            tempValue = captureValue[1] + (1000 * updateTime) - captureValue[0];
            frequency = (pclk1 * 2) / tempValue;
            printf("frequency = %ld Hz\r\n", frequency);
            captureState = 0;
        }
        else
        {
            captureState++;
        }

        updateTime = 0;
        TMR_ClearIntFlag(TMR2, TMR_INT_CC2);
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
