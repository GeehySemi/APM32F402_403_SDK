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
void MCO_Config(void);

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

    RCM_EnableCSS();
    RCM_ConfigHSE(RCM_HSE_OPEN);

    if(RCM_WaitHSEReady() == SUCCESS)
    {
        FMC_EnablePrefetchBuffer();
        FMC_ConfigLatency(FMC_LATENCY_2);

        RCM_ConfigAHB(RCM_AHB_DIV_1);
        RCM_ConfigAPB2(RCM_APB_DIV_1);
        RCM_ConfigAPB1(RCM_APB_DIV_2);

        RCM_ConfigPLL(RCM_PLLSEL_HSE, RCM_PLLMF_9);
        RCM_EnablePLL();
        if (RCM_WaitPLLReady() != SUCCESS)
        {
            while (1);
        }

        RCM_ConfigSYSCLK(RCM_SYSCLK_SEL_PLL);
        while(RCM_ReadSYSCLKSource() != RCM_SYSCLK_SEL_PLL);
    }
    else
    {
        while(1);
    }

    MCO_Config();
    BOARD_LED_Config(LED2);
    BOARD_Delay_Config();

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        BOARD_Delay_Ms(500);
    }
}

/**
 * @brief   Main clock output pin configuration
 *
 * @param   None
 *
 * @retval  None
 */
void MCO_Config(void)
{
    GPIO_Config_T gpioConfigStruct;
    /* Configure MCO pin */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);

    gpioConfigStruct.pin = GPIO_PIN_8;
    gpioConfigStruct.mode = GPIO_MODE_AF_PP;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOA, &gpioConfigStruct);
    RCM_ConfigMCO(RCM_MCOCLK_SYSCLK);
}
