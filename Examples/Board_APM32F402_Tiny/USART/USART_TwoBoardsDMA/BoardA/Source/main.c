/**
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.1
 *
 * @date        2026-01-05
 *
 * @attention
 *
 *  Copyright (C) 2024-2026 Geehy Semiconductor
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

DMA_Config_T dmaConfigStruct;

volatile uint32_t rxCountUSART1 = 0;
volatile uint32_t txCountUSART1 = 0;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/
/** Delay */
void Delay(uint32_t count);

/** USART Initialization */
void USART_Init(void);

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
    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);

    for(int i = 0; i < DATA_BUF_SIZE; i++)
    {
        txDataBufUSART1[i] = i;
    }
    
    /* USART Initialization */
    USART_Init();

    /* Enable DMA USART TX stream*/
    DMA_Enable(DMA1_Channel4);
    
    /* Wait KEY1 button is pressed */
    while(BOARD_BUTTON_GetState(BUTTON_KEY1) != RESET)
    {
    }
    
    /* Wait KEY1 button is released */
    while(BOARD_BUTTON_GetState(BUTTON_KEY1) == RESET)
    {
    }
    
    /* Enable USART DMA TX request*/
    USART_EnableDMA(USART1,USART_DMA_TX);
    
    /* Waitting the end of data transfer*/
    while(USART_ReadStatusFlag(USART1, USART_FLAG_TXC) == RESET)
    {
    }
    
    while(DMA_ReadStatusFlag(DMA1_FLAG_TC4) == RESET)
    {
    }
    
    /* Clear USART DMA flags*/
    DMA_ClearStatusFlag(DMA1_FLAG_TC4);
    USART_ClearStatusFlag(USART1, USART_FLAG_TXC);

    while(1)
    {
    }
}

/*!
 * @brief       USARTS Initialization
 *
 * @param       None
 *
 * @retval      None
 */
void USART_Init(void)
{
    USART_Config_T usartConfigStruct;

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &usartConfigStruct);

    /* Configure USART DMA*/
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);
    
    dmaConfigStruct.bufferSize          = DATA_BUF_SIZE;
    dmaConfigStruct.memoryDataSize      = DMA_MEMORY_DATA_SIZE_BYTE;
    dmaConfigStruct.memoryInc           = DMA_MEMORY_INC_ENABLE;
    dmaConfigStruct.loopMode            = DMA_MODE_NORMAL;
    dmaConfigStruct.peripheralBaseAddr  = (uint32_t) (&(USART1->DATA));
    dmaConfigStruct.peripheralDataSize  = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    dmaConfigStruct.peripheralInc       = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfigStruct.priority            = DMA_PRIORITY_HIGH;
    
    /* USART TX DMA*/
    dmaConfigStruct.dir                 = DMA_DIR_PERIPHERAL_DST;
    dmaConfigStruct.memoryBaseAddr      = (uint32_t)txDataBufUSART1;
    dmaConfigStruct.M2M                 = DMA_M2MEN_DISABLE;
    DMA_Config(DMA1_Channel4, &dmaConfigStruct);
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
