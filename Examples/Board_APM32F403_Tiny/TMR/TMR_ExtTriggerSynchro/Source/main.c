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

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void TMR_Config(void);
void TMR_GPIO_Config(void);

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

    TMR_Config();
    TMR_GPIO_Config();

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
    TMR_ICConfig_T   tmrICConfig;
    TMR_OCConfig_T   tmrOCConfig;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2 | RCM_APB1_PERIPH_TMR3 | RCM_APB1_PERIPH_TMR4);

    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.division = 0;
    tmrBaseConfig.repetitionCounter = 0;

    /* TMR2 frequency = 120MHz / (59 + 1) / 2 = 1MHz */
    tmrBaseConfig.period = 59;
    TMR_ConfigTimeBase(TMR2, &tmrBaseConfig);

    /* TMR3 frequency = 1MHz / (1 + 1) = 500KHz */
    tmrBaseConfig.period = 1;
    TMR_ConfigTimeBase(TMR3, &tmrBaseConfig);

    /* TMR3 frequency = 1MHz / (3 + 1) = 250KHz */
    tmrBaseConfig.period = 3;
    TMR_ConfigTimeBase(TMR4, &tmrBaseConfig);

    tmrICConfig.channel = TMR_CHANNEL_2;
    tmrICConfig.polarity = TMR_IC_POLARITY_RISING;
    tmrICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    tmrICConfig.prescaler = TMR_IC_PSC_1;
    tmrICConfig.filter = 0;
    TMR_ConfigIC(TMR2, &tmrICConfig);

    /* Configure TMR2, TMR3, TMR4 channel1 */
    tmrOCConfig.mode = TMR_OC_MODE_TOGGLE;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_DISABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_SET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_SET;
    tmrOCConfig.pulse = 0;

    TMR_ConfigOC1(TMR2, &tmrOCConfig);
    TMR_ConfigOC1(TMR3, &tmrOCConfig);
    TMR_ConfigOC1(TMR4, &tmrOCConfig);

    /* Enable TMR2 the Master Slave Mode */
    TMR_SelectSlaveMode(TMR2, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR2, TMR_TRIGGER_SOURCE_TI2FP2);
    TMR_SelectOutputTrigger(TMR2, TMR_TRGO_SOURCE_UPDATE);
    TMR_EnableMasterSlaveMode(TMR2);

    /* Enable TMR3 the Master Slave Mode */
    TMR_SelectSlaveMode(TMR3, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR3, TMR_TRIGGER_SOURCE_ITR1);
    TMR_SelectOutputTrigger(TMR3, TMR_TRGO_SOURCE_UPDATE);
    TMR_EnableMasterSlaveMode(TMR3);

    /* Enable TMR4 the Master Slave Mode */
    TMR_SelectSlaveMode(TMR4, TMR_SLAVE_MODE_GATED);
    TMR_SelectInputTrigger(TMR4, TMR_TRIGGER_SOURCE_ITR1);
    TMR_EnableMasterSlaveMode(TMR4);

    TMR_EnablePWMOutputs(TMR2);

    TMR_Enable(TMR2);
    TMR_Enable(TMR3);
    TMR_Enable(TMR4);
}

/**
 * @brief   TMR pin configuration
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_GPIO_Config(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    /* Configure TMR2 and TMR3 gpio */
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6;
    GPIO_Config(GPIOA, &gpioConfig);

    /* Configure TMR4 gpio */
    gpioConfig.pin = GPIO_PIN_6;
    GPIO_Config(GPIOB, &gpioConfig);
}
