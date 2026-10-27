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
volatile uint8_t PWMStep;

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

    BOARD_Delay_Config();

    TMR_GPIO_Config();
    TMR_Config();

    /* Infinite loop */
    while (1)
    {
        TMR_GenerateEvent(TMR1, TMR_EVENT_COM);
        BOARD_Delay_Ms(100);
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
    TMR_BDTConfig_T tmrBDTConfig;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR1);

    /* TMR1 clock source frequency = 120MHz */
    tmrBaseConfig.countMode = TMR_COUNTER_MODE_UP;
    tmrBaseConfig.clockDivision = TMR_CLOCK_DIV_1;
    tmrBaseConfig.period = 2000;
    tmrBaseConfig.division = 59;
    tmrBaseConfig.repetitionCounter = 0;
    TMR_ConfigTimeBase(TMR1, &tmrBaseConfig);

    /* Configure output channel */
    tmrOCConfig.mode = TMR_OC_MODE_PWM1;
    tmrOCConfig.outputState = TMR_OC_STATE_ENABLE;
    tmrOCConfig.outputNState = TMR_OC_NSTATE_ENABLE;
    tmrOCConfig.polarity = TMR_OC_POLARITY_HIGH;
    tmrOCConfig.nPolarity = TMR_OC_NPOLARITY_HIGH;
    tmrOCConfig.idleState = TMR_OC_IDLE_STATE_SET;
    tmrOCConfig.nIdleState = TMR_OC_NIDLE_STATE_SET;

    tmrOCConfig.pulse = 1000;
    TMR_ConfigOC1(TMR1, &tmrOCConfig);
    TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
    TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

    tmrOCConfig.pulse = 500;
    TMR_ConfigOC2(TMR1, &tmrOCConfig);
    TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
    TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

    tmrOCConfig.pulse = 250;
    TMR_ConfigOC3(TMR1, &tmrOCConfig);
    TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
    TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);

    /* Configure the Break feature, dead time */
    tmrBDTConfig.RMOS = TMR_RMOS_STATE_ENABLE;
    tmrBDTConfig.IMOS = TMR_IMOS_STATE_ENABLE;
    tmrBDTConfig.lockLevel = TMR_LOCK_LEVEL_OFF;
    tmrBDTConfig.deadTime = 0x01;
    tmrBDTConfig.BRKState = TMR_BRK_STATE_ENABLE;
    tmrBDTConfig.BRKPolarity = TMR_BRK_POLARITY_HIGH;
    tmrBDTConfig.automaticOutput = TMR_AUTOMATIC_OUTPUT_ENABLE;
    TMR_ConfigBDT(TMR1, &tmrBDTConfig);

    /* Enable TMR1 commutation interrupt */
    TMR_EnableInterrupt(TMR1, TMR_INT_COM);
    TMR_ClearIntFlag(TMR1, TMR_INT_COM);
    NVIC_EnableIRQRequest(TMR1_TRG_COM_IRQn, 0xF, 0xF);

    /* Enable capture compare preload control */
    TMR_EnableCCPreload(TMR1);

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

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB);

    /* Configure TMR1 GPIO for complementary output PWM */
    /* CH1->PA8, CH2->PA9, CH3->PA10 */
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.mode = GPIO_MODE_AF_PP;
    gpioConfig.pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_Config(GPIOA, &gpioConfig);

    /* BKIN->PB12, CH1N->PB13, CH2N->PB14 , CH3N->PB15 */
    gpioConfig.pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_Config(GPIOB, &gpioConfig);
}


/**
 * @brief   TMR trigger and commutation interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void TMR1_TRG_COM_Isr(void)
{
    PWMStep++;

    switch(PWMStep)
    {
        case 1:
            /* Configure Channel1 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
        break;
        case 2:
            /* Configure Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
        break;
        case 3:
            /* Configure Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
        break;
        case 4:
            /* Configure Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_3);
        break;
        case 5:
            /* Configure Channel1 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_3);
        break;
        case 6:
            /* Configure Channel1 */
            TMR_EnableCCxChannel(TMR1, TMR_CHANNEL_1);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_1);

            /* Configure Channel2 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_2);
            TMR_DisableCCxNChannel(TMR1, TMR_CHANNEL_2);

            /* Configure Channel3 */
            TMR_DisableCCxChannel(TMR1, TMR_CHANNEL_3);
            TMR_EnableCCxNChannel(TMR1, TMR_CHANNEL_3);

            PWMStep = 0;
        break;
        default:
            PWMStep = 0;
        break;
    }
    TMR_ClearIntFlag(TMR1, TMR_INT_COM);
}
