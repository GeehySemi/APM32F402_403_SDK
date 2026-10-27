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
#include "apm32f402_403_iwdt.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
volatile uint32_t tick = 0;

/* Private function prototypes ********************************************/

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

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_EINT);

    if(RCM_ReadStatusFlag(RCM_FLAG_IWDTRST) == SET)
    {
        BOARD_LED_On(LED3);
        RCM_ClearStatusFlag();
    }
    else
    {
        BOARD_LED_Off(LED3);
    }

    IWDT_EnableWriteAccess();
    IWDT_ConfigDivider(IWDT_DIVIDER_32);
    IWDT_ConfigReload(300);
    while (IWDT_ReadStatusFlag(IWDT_FLAG_PSCU) || IWDT_ReadStatusFlag(IWDT_FLAG_CNTU))
    {
    }
    IWDT_Refresh();
    IWDT_Enable();

    while(1)
    {
        BOARD_LED_Toggle(LED2);
        Delay();
        IWDT_Refresh();
    }
}

/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay(void)
{
    tick = 0;
    while(tick < 200);
}
