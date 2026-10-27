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
enum {FAILED, PASSED};

/* Debug Information output */

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
const uint16_t I2S2_Buffer_Tx[32] =
{
    0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C,
    0x0D0E, 0x0F10, 0x1112, 0x1314, 0x1516, 0x1718,
    0x191A, 0x1B1C, 0x1D1E, 0x1F20, 0x2122, 0x2324,
    0x2526, 0x2728, 0x292A, 0x2B2C, 0x2D2E, 0x2F30,
    0x3132, 0x3334, 0x3536, 0x3738, 0x393A, 0x3B3C,
    0x3D3E, 0x3F40
};

uint8_t TransferStatus = FAILED;
volatile uint32_t txCnt = 0;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/** Buffer compare*/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    I2S_Config_T I2S_ConfigStruct;

    SystemClockConfig();

    RCM_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);

    while (BOARD_BUTTON_GetState(BUTTON_KEY1) != RESET);
    
    SPI_I2S_Reset(SPI2);

    I2S_ConfigStruct.standard = I2S_STANDARD_PHILLIPS;
    I2S_ConfigStruct.length = I2S_DATA_LENGTH_16BEX;
    I2S_ConfigStruct.MCLKOutput = I2S_MCLK_OUTPUT_DISABLE;
    I2S_ConfigStruct.audioDiv = I2S_AUDIO_DIV_48K;
    I2S_ConfigStruct.polarity = I2S_CLKPOL_LOW;

    I2S_ConfigStruct.mode = I2S_MODE_MASTER_TX;
    I2S_Config(SPI2, &I2S_ConfigStruct);

    I2S_Enable(SPI2);
    SPI_I2S_EnableInterrupt(SPI2, SPI_I2S_INT_TXBE);
    
    SPI_I2S_ClearIntFlag(SPI2, SPI_I2S_INT_TXBE);

    while(txCnt < 32);

    while (1)
    {
    }
}

/*!
 * @brief       Configures the different system clocks
 *
 * @param       None
 *
 * @retval      None
 *
 */
void RCM_Configuration(void)
{
    RCM_Reset();

    RCM_ConfigHSE(RCM_HSE_OPEN);
    if(RCM_WaitHSEReady() == SUCCESS)
    {
        FMC_EnablePrefetchBuffer();
        FMC_ConfigLatency(FMC_LATENCY_2);

        RCM_ConfigAHB(RCM_AHB_DIV_1);
        RCM_ConfigAPB2(RCM_APB_DIV_1);
        RCM_ConfigAPB1(RCM_APB_DIV_2);
        RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);
        RCM_ConfigPLL(RCM_PLLSEL_HSE, RCM_PLLMF_9);

        RCM_EnablePLL();
        if (RCM_WaitPLLReady() != SUCCESS)
        {
            while (1);
        }

        RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_PLL);
        while(RCM_ReadSYSCLKSource() != RCM_SYSCLK_SEL_PLL);
    }

    RCM_EnableAPB2PeriphClock((RCM_APB2_PERIPH_T)(RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO));
    RCM_EnableAPB1PeriphClock((RCM_APB1_PERIPH_T)(RCM_APB1_PERIPH_SPI2));
}

/*!
 * @brief       Configures the different GPIO ports.
 *
 * @param       None
 *
 * @retval      None
 *
 */
void GPIO_Configuration(void)
{
    GPIO_Config_T GPIO_ConfigStruct;

    GPIO_ConfigStruct.pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_ConfigStruct);
}

/*!
 * @brief       Configure the nested vectored interrupt controller.
 *
 * @param       None
 *
 * @retval      None
 */
void NVIC_Configuration(void)
{
    NVIC_EnableIRQRequest(SPI2_IRQn, 1, 0);
}

/*!
 * @brief       Compares two buffers.
 *
 * @param       pBuffer1:       buffer1
 *
 * @param       pBuffer2:       buffer2
 *
 * @param       BufferLength:   buffer length
 *
 * @retval      None
 */
uint8_t Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
    uint16_t i;

    for(i = 0; i < BufferLength; i++)
    {
        if(pBuffer1[i] != pBuffer2[i])
        {
            return FAILED;
        }
    }

    return PASSED;
}

/*!
 * @brief       Compares two buffers.
 *
 * @param       pBuffer1:       buffer1
 *
 * @param       pBuffer2:       buffer2
 *
 * @param       BufferLength:   buffer length
 *
 * @retval      None
 */
uint8_t Buffercmp24bits(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
    uint16_t i;

    for(i = 0; i < BufferLength; i++)
    {
        if(pBuffer1[i] != pBuffer2[i])
        {
            if(pBuffer1[i] != (pBuffer2[i] & 0XFF00))
            {
                return FAILED;
            }
        }
    }

    return PASSED;
}
