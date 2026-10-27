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

/* Buffer Size */
#define DATA_BUF_SIZE       128

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

uint8_t spi1RxBuf[DATA_BUF_SIZE] = {0};
uint8_t spi2RxBuf[DATA_BUF_SIZE] = {0};

/* Private function prototypes ********************************************/
void SPI_FullDuplex_Config(void);
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size);

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
    uint32_t i = 0;

    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    SPI_FullDuplex_Config();

    for (i = 0; i < DATA_BUF_SIZE; i++)
    {
        while(!SPI_I2S_ReadStatusFlag(SPI2, SPI_FLAG_TXBE));
        SPI_I2S_TxData(SPI2, i);
        while(!SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_TXBE));
        SPI_I2S_TxData(SPI1, i);

        while(!SPI_I2S_ReadStatusFlag(SPI1, SPI_FLAG_RXBNE));
        spi1RxBuf[i] = SPI_I2S_RxData(SPI1);
        while(!SPI_I2S_ReadStatusFlag(SPI2, SPI_FLAG_RXBNE));
        spi2RxBuf[i] = SPI_I2S_RxData(SPI2);
    }

    if (BufferCompare(spi1RxBuf, spi2RxBuf, DATA_BUF_SIZE))
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

/**
 * @brief   SPI full duplex configuration
 *
 * @param   None
 *
 * @retval  None
 */
void SPI_FullDuplex_Config(void)
{
    SPI_Config_T spiConfig;
    GPIO_Config_T gpioConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SPI1);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_SPI2);

    /* SPI1 gpio configuration */
    gpioConfig.pin = (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    GPIO_Config(GPIOA, &gpioConfig);

    /* SPI2 gpio configuration */
    gpioConfig.pin = (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    GPIO_Config(GPIOB, &gpioConfig);

    /* SPI1 Configuration */
    spiConfig.mode = SPI_MODE_MASTER;
    spiConfig.length = SPI_DATA_LENGTH_8B;
    spiConfig.phase = SPI_CLKPHA_1EDGE;
    spiConfig.polarity = SPI_CLKPOL_HIGH;
    spiConfig.nss = SPI_NSS_HARD;
    spiConfig.firstBit = SPI_FIRSTBIT_MSB;
    spiConfig.direction = SPI_DIRECTION_2LINES_FULLDUPLEX;
    spiConfig.baudrateDiv = SPI_BAUDRATE_DIV_2;
    SPI_Config(SPI1, &spiConfig);

    /* SPI2 Configuration */
    spiConfig.mode = SPI_MODE_SLAVE;
    SPI_Config(SPI2, &spiConfig);

    SPI_EnableSSOutput(SPI1);
    SPI_Enable(SPI1);

    SPI_EnableSSOutput(SPI2);
    SPI_Enable(SPI2);
}

/*!
 * @brief       Compares two buffers
 *
 * @param       buf1:    First buffer to be compared
 *
 * @param       buf2:    Second buffer to be compared
 *
 * @param       size:    Buffer size
 *
 * @retval      Return compare result. It can be one of value:
 *              @arg 1  : the two buffer are the same.
 *              @arg 0  : the two buffer are different.
 *
 */
uint8_t BufferCompare(uint8_t* buf1, uint8_t* buf2, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        if (buf1[i] != buf2[i])
        {
            return 0;
        }
    }

    return 1;
}
