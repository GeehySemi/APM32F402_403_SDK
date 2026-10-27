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

/* Flash read write test address */
#define FLASH_READ_WRITE_TEST_ADDR    (FLASH_READ_WRITE_START_ADDR + APM32_FLASH_PAGE_SIZE)

/* test buffer size */
#define BUFFER_SIZE                   64

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void COM_Config(void);
void InitBuffer(void);
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint32_t size);

/* External variables *****************************************************/

/* test write buffer */
static uint8_t Write_Buffer[BUFFER_SIZE] = {0};

/* test read buffer */
static uint8_t Read_Buffer[BUFFER_SIZE] = {0};

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
    uint32_t addr = 0;

    SystemClockConfig();

    /* Device configuration */
    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    COM_Config();

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_FMC);

    /* Page aligned write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Page aligned write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Page aligned write operation fail!\r\n");
    }

    /* Page non-aligned write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR + 16;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Page non-aligned write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Page non-aligned write operation fail!\r\n");
    }

    /* Cross page write operation */
    addr = FLASH_READ_WRITE_TEST_ADDR - 8;
    InitBuffer();
    Flash_Write(addr, Write_Buffer, BUFFER_SIZE);
    Flash_Read(addr, Read_Buffer, BUFFER_SIZE);
    if (BufferCompare(Write_Buffer, Read_Buffer, BUFFER_SIZE) != 0U)
    {
        BOARD_LED_On(LED2);
        printf("Cross page write operation Successful!\r\n");
    }
    else
    {
        BOARD_LED_Off(LED2);
        printf("Cross page write operation fail!\r\n");
    }

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

/*!
 * @brief       Initialize Flash read write test buffers
 *
 * @param       None
 *
 * @retval      None
 */
void InitBuffer(void)
{
    for(uint32_t idx = 0U; idx < BUFFER_SIZE; idx++)
    {
        Write_Buffer[idx] = (uint8_t)idx;
        Read_Buffer[idx] = 0U;
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf1:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return 1 if buf1 = buf2. If not then return 0
 */
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint32_t size)
{
    uint32_t i = 0;

    for (i = 0; i < size; i++)
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
