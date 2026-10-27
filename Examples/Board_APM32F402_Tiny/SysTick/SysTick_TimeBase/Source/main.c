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
#include "board_apm32f402_403_tiny.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/
void Delay(uint32_t cnt);

/* External variables *****************************************************/
volatile uint32_t delayCnt = 0;

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

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    SysTick_Config(SystemCoreClock / 1000);

    /* Infinite loop */
    while (1)
    {
        BOARD_LED_Toggle(LED2);
        BOARD_LED_Toggle(LED3);
        Delay(1000);
    }
}

void Delay(uint32_t cnt)
{
    delayCnt = cnt;
    while(delayCnt);
}
