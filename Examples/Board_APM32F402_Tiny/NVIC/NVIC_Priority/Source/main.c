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
#include "apm32f402_403_misc.h"
#include "board_apm32f402_403_tiny.h"
#include "board_delay.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

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

    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_EINT);
    BOARD_BUTTON_Config(BUTTON_KEY2, BUTTON_MODE_EINT);
    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    NVIC_ConfigPriorityGroup(NVIC_PRIORITY_GROUP_1);
    NVIC_EnableIRQRequest(KEY1_BUTTON_EINT_IRQ_NUM, 1, 1);
    NVIC_EnableIRQRequest(KEY2_BUTTON_EINT_IRQ_NUM, 0, 1);

    /* Infinite loop */
    while (1)
    {
    }
}

/**
 * @brief   Button1 interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void Button1_Isr(void)
{
    if(EINT_ReadIntFlag(KEY1_BUTTON_EINT_LINE) == SET)
    {
        EINT_ClearIntFlag(KEY1_BUTTON_EINT_LINE);
        BOARD_LED_On(LED2);
        while(1);
    }
}

/**
 * @brief   Button2 interrupt server
 *
 * @param   None
 *
 * @retval  None
 */
void Button2_Isr(void)
{
    if(EINT_ReadIntFlag(KEY2_BUTTON_EINT_LINE) == SET)
    {
        EINT_ClearIntFlag(KEY2_BUTTON_EINT_LINE);
        BOARD_LED_On(LED3);
        while(1);
    }
}
