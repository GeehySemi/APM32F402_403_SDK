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

    BOARD_LED_Config(LED2);

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

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR5);

    /* TMR5 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 999;
    tmrBaseConfig.division = 119;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR5, &tmrBaseConfig);

    /* 50% PWM */
    tmrOCConfig.mode = TMR_OC_MODE_PWM1;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_DISABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_RESET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    tmrOCConfig.pulse = 500;
    TMR_ConfigOC2(TMR5, &tmrOCConfig);

    TMR_EnablePWMOutputs(TMR5);
    TMR_Enable(TMR5);
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
    gpioConfig.pin = GPIO_PIN_1;
    GPIO_Config(GPIOA, &gpioConfig);
}
