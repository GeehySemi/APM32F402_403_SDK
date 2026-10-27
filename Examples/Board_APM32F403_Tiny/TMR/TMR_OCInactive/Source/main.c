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
void OCPin_Config(void);

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

    OCPin_Config();
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

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR2);

    /* TMR2 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 999;
    tmrBaseConfig.division = 119;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR2, &tmrBaseConfig);

    tmrOCConfig.mode = TMR_OC_MODE_INACTIVE;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_DISABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_RESET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_RESET;

    tmrOCConfig.pulse = 800;
    TMR_ConfigOC1(TMR2, &tmrOCConfig);

    tmrOCConfig.pulse = 600;
    TMR_ConfigOC2(TMR2, &tmrOCConfig);

    tmrOCConfig.pulse = 400;
    TMR_ConfigOC3(TMR2, &tmrOCConfig);

    tmrOCConfig.pulse = 200;
    TMR_ConfigOC4(TMR2, &tmrOCConfig);

    TMR_EnableInterrupt(TMR2, TMR_INT_CC1 | TMR_INT_CC2 | TMR_INT_CC3 | TMR_INT_CC4);
    NVIC_EnableIRQRequest(TMR2_IRQn, 0xF, 0xF);

    TMR_EnablePWMOutputs(TMR2);
    TMR_Enable(TMR2);
}

/**
 * @brief   TMR output pin configuration
 *
 * @param   None
 *
 * @retval  None
 */
void OCPin_Config(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_OUT_PP;
    gpioConfig.pin = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIO_Config(GPIOA, &gpioConfig);

    /* generate raising edge */
    GPIO_WriteBitValue(GPIOA, GPIO_PIN_0, 1);
    GPIO_WriteBitValue(GPIOA, GPIO_PIN_1, 1);
    GPIO_WriteBitValue(GPIOA, GPIO_PIN_2, 1);
    GPIO_WriteBitValue(GPIOA, GPIO_PIN_3, 1);
}

/**
 * @brief   TMR update interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void TMR_update_Irs(void)
{
    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC1))
    {
        /* Clear the TMR2 CC1 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC1);

        /* delay 800us generate falling edge */
        GPIO_WriteBitValue(GPIOA, GPIO_PIN_0, 0);
    }

    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC2))
    {
        /* Clear the TMR2 CC2 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC2);

        /* delay 600us generate falling edge */
        GPIO_WriteBitValue(GPIOA, GPIO_PIN_1, 0);
    }

    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC3))
    {
        /* Clear the TMR2 CC3 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC3);

        /* delay 400us generate falling edge */
        GPIO_WriteBitValue(GPIOA, GPIO_PIN_2, 0);
    }

    if (TMR_ReadIntFlag(TMR2, TMR_INT_CC4))
    {
        /* Clear the TMR2 CC4 pending bit */
        TMR_ClearIntFlag(TMR2, TMR_INT_CC4);

        /* delay 200us generate falling edge */
        GPIO_WriteBitValue(GPIOA, GPIO_PIN_3, 0);
    }
}
