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

    TMR_GPIO_Config();
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
    TMR_ICConfig_T tmrICConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

    /* TMR1 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 999;
    tmrBaseConfig.division = 119;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &tmrBaseConfig);

    tmrICConfig.channel = TMR_CHANNEL_2;
    tmrICConfig.polarity = TMR_IC_POLARITY_RISING;
    tmrICConfig.selection = TMR_IC_SELECTION_DIRECT_TI;
    tmrICConfig.prescaler = TMR_IC_PSC_1;
    tmrICConfig.filter = 0;
    TMR_ConfigIC(TMR1, &tmrICConfig);

    tmrOCConfig.mode = TMR_OC_MODE_PWM2;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_DISABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_SET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    tmrOCConfig.pulse = 500;
    TMR_ConfigOC1(TMR1, &tmrOCConfig);

    TMR_ConfigSinglePulseMode(TMR1, TMR_SPM_SINGLE);

    /* Selects the Slave Mode and Input Trigger source */
    TMR_SelectSlaveMode(TMR1, TMR_SLAVE_MODE_TRIGGER);
    TMR_SelectInputTrigger(TMR1, TMR_TRIGGER_SOURCE_TI2FP2);

    /* Enable the specified TMR peripheral */
    TMR_EnablePWMOutputs(TMR1);
    TMR_Enable(TMR1);
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
    gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Config(GPIOA, &gpioConfig);
}
