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

/* Private typedef ********************************************************/
typedef enum {FALSE, TRUE} BOOL;

/* Private variables ******************************************************/
/** USART1 receive buffer*/
uint8_t rxDataBufUSART1[DATA_BUF_SIZE] = {0};
/** USART1 transmit buffer*/
uint8_t txDataBufUSART1[DATA_BUF_SIZE] = {0};

/* Private function prototypes ********************************************/
volatile uint32_t rxCountUSART1 = 0;
volatile uint32_t txCountUSART1 = 0;

/* External variables *****************************************************/

/* External functions *****************************************************/
/** Delay */
void Delay(uint32_t count);

/** Buffer compare*/
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size);

/**
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

    NVIC_EnableIRQRequest(USART1_IRQn, 1, 0);

    /* Init txDataBufUSART1 */
    for(int i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufUSART1[i] = i;
    }

    /* Wait KEY1 button is pressed */
    while(BOARD_BUTTON_GetState(BUTTON_KEY1) != RESET)
    {
    }
    
    /* Wait KEY1 button is released */
    while(BOARD_BUTTON_GetState(BUTTON_KEY1) == RESET)
    {
    }
    
    USART_EnableInterrupt(USART1, USART_INT_TXBE);

    while(1)
    {
    }
}

/*!
 * @brief       This function handles USART1 interrupt Handler
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into  void USART1_IRQHandler(void)
 */
void  USART_Isr(void)
{
    /* USART1 Send Data */
    if(USART_ReadIntFlag(USART1, USART_INT_TXBE) == SET)
    {
        if(txCountUSART1 < DATA_BUF_SIZE)
        {
            USART_TxData(USART1,txDataBufUSART1[txCountUSART1++]);
        }
        else
        {
            USART_DisableInterrupt(USART1, USART_INT_TXBE);
        }
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
 * @retval      Return SET if buf1 = buf2. If not then return RESET
 */
BOOL BufferCompare(uint8_t *buf1, uint8_t *buf2, uint8_t size)
{
    uint8_t i;

    for(i = 0; i < size; i++)
    {
        if(buf1[i] != buf2[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!
 * @brief       Delay
 *
 * @param       count:  delay count
 *
 * @retval      None
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;
    while(delay--);
}
