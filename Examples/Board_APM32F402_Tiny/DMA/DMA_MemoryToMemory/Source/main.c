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
#define BUF_SIZE  32

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
const uint32_t SRC_Const_Buffer[BUF_SIZE] =
{
    0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
    0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
    0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
    0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
    0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
    0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
    0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
    0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};
uint32_t DST_Buffer[BUF_SIZE];

volatile uint32_t CurrDataCounterBegin = 0;
volatile uint32_t CurrDataCounterEnd = 1;
/* Private function prototypes ********************************************/
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);

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
    DMA_Config_T DMA_ConfigStruct;

    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA2);

    DMA_Reset(DMA2_Channel1);

    DMA_ConfigStructInit(&DMA_ConfigStruct);
    DMA_ConfigStruct.peripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
    DMA_ConfigStruct.memoryBaseAddr = (uint32_t)DST_Buffer;
    DMA_ConfigStruct.dir = DMA_DIR_PERIPHERAL_SRC;
    DMA_ConfigStruct.bufferSize = BUF_SIZE;
    DMA_ConfigStruct.peripheralInc = DMA_PERIPHERAL_INC_ENABLE;
    DMA_ConfigStruct.memoryInc = DMA_MEMORY_INC_ENABLE;
    DMA_ConfigStruct.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_WORD;
    DMA_ConfigStruct.memoryDataSize = DMA_MEMORY_DATA_SIZE_WORD;
    DMA_ConfigStruct.loopMode = DMA_MODE_NORMAL;
    DMA_ConfigStruct.priority = DMA_PRIORITY_HIGH;
    DMA_ConfigStruct.M2M = DMA_M2MEN_ENABLE;

    DMA_Config(DMA2_Channel1, &DMA_ConfigStruct);

    NVIC_EnableIRQRequest(DMA2_Channel1_IRQn, 0, 0);

    DMA_EnableInterrupt(DMA2_Channel1, DMA_INT_TC);

    CurrDataCounterBegin = DMA_ReadDataNumber(DMA2_Channel1);

    DMA_Enable(DMA2_Channel1);

    while (CurrDataCounterEnd);

    if (Buffercmp(SRC_Const_Buffer, DST_Buffer, BUF_SIZE))
    {
        BOARD_LED_On(LED2);
    }
    else
    {
        BOARD_LED_On(LED3);
    }

    /* Infinite loop */
    while (1)
    {
        
    }
}

/*!
 * @brief     This function handles DMA1 channel IRQ request
 *
 * @param     None
 *
 * @retval    None
 *
 */
void DMA2_ChannelIRQRequest(void)
{
    if (DMA_ReadIntFlag(DMA2_INT_FLAG_TC1))
    {
        CurrDataCounterEnd = DMA_ReadDataNumber(DMA2_Channel1);

        DMA_ClearIntFlag(DMA2_INT_FLAG_TC1);
    }
}

/*!
 * @brief       Compares two buffers
 *
 * @param       pBuffer
 *
 * @param       pBuffer

 * @retval      0: FAILED
  *             1: PASSED
 *
 * @note
 */
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer != *pBuffer1)
        {
            return 0;
        }

        pBuffer++;
        pBuffer1++;
    }

    return 1;
}
