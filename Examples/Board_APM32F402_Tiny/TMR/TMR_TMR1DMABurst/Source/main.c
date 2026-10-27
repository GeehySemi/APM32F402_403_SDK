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
#define BUFFER_LENGTH 3

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint16_t configBuffer[BUFFER_LENGTH] = {800, 700, 600};

/* Private function prototypes ********************************************/
void TMR_Config(void);
void TMR_GPIO_Config(void);
void TMR_DMA_Config(void);

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

    TMR_GPIO_Config();
    TMR_DMA_Config();
    TMR_Config();

    /* Infinite loop */
    while (1)
    {

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
    TMR_OCConfig_T tmrOCConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

    /* TMR1 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 999;
    tmrBaseConfig.division = 119;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &tmrBaseConfig);

    tmrOCConfig.mode = TMR_OC_MODE_PWM1;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_ENABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_RESET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_RESET;

    /* TMR1 CH1 config */
    tmrOCConfig.pulse = 500;
    TMR_ConfigOC1(TMR1, &tmrOCConfig);

    /* TMR1 CH2 config */
    tmrOCConfig.pulse = 500;
    TMR_ConfigOC2(TMR1, &tmrOCConfig);

    /* TMR1 CH3 config */
    tmrOCConfig.pulse = 500;
    TMR_ConfigOC3(TMR1, &tmrOCConfig);

    /* TMR1 DMA Base register and DMA Burst Length Config */
    TMR_ConfigDMA(TMR1, TMR_DMA_BASE_CC1, TMR_DMA_BURSTLENGTH_3TRANSFERS);

    /* TMR1 Update DMA Request enable */
    TMR_EnableDMASoure(TMR1, TMR_DMA_SOURCE_UPDATE);

    TMR_EnablePWMOutputs(TMR1);
    TMR_Enable(TMR1);
}

/**
 * @brief   DMA configuration
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_DMA_Config(void)
{
    DMA_Config_T dmaConfig;

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    dmaConfig.peripheralBaseAddr = (uint32_t)(&TMR1->DMADDR);
    dmaConfig.memoryBaseAddr = (uint32_t)configBuffer;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_DST;
    dmaConfig.bufferSize = BUFFER_LENGTH;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_HALFWORD;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_HALFWORD;
    dmaConfig.loopMode = DMA_MODE_NORMAL;
    dmaConfig.priority = DMA_PRIORITY_HIGH;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;

    /* Config TMR1 UP DMA channel */
    DMA_Config(DMA1_Channel5, &dmaConfig);

    /* Enable DMA */
    DMA_Enable(DMA1_Channel5);
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

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;

    /* TMR_CH1N(PB13), TMR1_CH2N(PB14), TMR1_CH3N(PB15) GPIO configuration */
    gpioConfig.pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Config(GPIOB, &gpioConfig);

    /* TMR1_CH1(PA8), TMR1_CH2(PA9), TMR1_CH3(PA10) GPIO configuration */
    gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Config(GPIOA, &gpioConfig);
}
