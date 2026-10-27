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
/* printf function configs to USART1*/
#define DEBUG_USART  USART1

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void I2C_Init(void);

/* External variables *****************************************************/

/* External functions *****************************************************/

/** Buffer compare*/
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size);

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    uint8_t dataToSend[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t receivedData[10] = {0};
    USART_Config_T USART_ConfigStruct;

    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);

    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX_RX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &USART_ConfigStruct);

    I2C_Init();

    while(1)
    {
        if (I2C_ReadStatusFlag(I2C1, I2C_FLAG_ADDR))
        {
            (void)I2C1->STS2; 

            for (uint8_t i = 0; i < 10; i++) 
            {
                while (!I2C_ReadStatusFlag(I2C1, I2C_FLAG_RXBNE)); 
                receivedData[i] = I2C_RxData(I2C1);
            }

            I2C_EnableGenerateStop(I2C1);
            
            if (BufferCompare(receivedData, dataToSend, 10))
            {
                BOARD_LED_On(LED2);
            }
            else
            {
                BOARD_LED_On(LED3);
            }
        }
    }
}

/*!
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 *
 */
void Delay(void)
{
    volatile uint32_t delay = 0xffff3;

    while(delay--);
}

/*!
 * @brief   I2C Init
 *
 * @param   None
 *
 * @retval  None
 *
 */
void I2C_Init(void)
{
   GPIO_Config_T gpioConfigStruct;
   I2C_Config_T i2cConfigStruct;

   /* Enable I2C related Clock */
   RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO);
   RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);

   /* Free I2C_SCL and I2C_SDA */
   gpioConfigStruct.mode = GPIO_MODE_AF_OD;
   gpioConfigStruct.speed = GPIO_SPEED_50MHz;
   gpioConfigStruct.pin = GPIO_PIN_6;
   GPIO_Config(GPIOB, &gpioConfigStruct);

   gpioConfigStruct.mode = GPIO_MODE_AF_OD;
   gpioConfigStruct.speed = GPIO_SPEED_50MHz;
   gpioConfigStruct.pin = GPIO_PIN_7;
   GPIO_Config(GPIOB, &gpioConfigStruct);

   /* Config I2C1 */
   I2C_Reset(I2C1);
   i2cConfigStruct.mode = I2C_MODE_I2C;
   i2cConfigStruct.dutyCycle = I2C_DUTYCYCLE_2;
   i2cConfigStruct.ackAddress = I2C_ACK_ADDRESS_7BIT;
   i2cConfigStruct.ownAddress1 = 0XB0;
   i2cConfigStruct.ack = I2C_ACK_ENABLE;
   i2cConfigStruct.clockSpeed = 100000;
   I2C_Config(I2C1,&i2cConfigStruct);

   /* Enable I2Cx */
   I2C_Enable(I2C1);
}

/*!
 * @brief   Buffer Compare
 *
 * @param   buf1  Buffer1
 *
 * @param   buf2  Buffer2
 *
 * @param   size  Buffer size
 *
 * @retval  0 or 1
 *
 */
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i]) 
        {
            return 0;
        }
    }
    return 1;
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
