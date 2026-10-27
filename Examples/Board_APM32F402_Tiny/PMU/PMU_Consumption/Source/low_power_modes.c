/*!
 * @file        low_power_modes.c
 *
 * @brief       LP modes program body
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

/* Includes */
#include "low_power_modes.h"
#include <stdio.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief     Consumption measure in sleep mode
 *              Wakeup using EINT line 1
 * @param     None
 *
 * @retval    None
 */
void ConsumptionMeasure_SleepMode(void)
{
    GPIO_Config_T  gpioConfigStruct;
    USART_Config_T usartConfigStruct;
    
    /* Enable GPIO clock*/
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB |
                              RCM_APB2_PERIPH_GPIOC | RCM_APB2_PERIPH_GPIOD);
    
    /* Setup all GPIO pin to Analog input mode*/
    gpioConfigStruct.pin = GPIO_PIN_ALL;
    gpioConfigStruct.mode = GPIO_MODE_ANALOG;
    gpioConfigStruct.speed = GPIO_SPEED_50MHz;
    
    GPIO_Config(GPIOA,&gpioConfigStruct);
    GPIO_Config(GPIOB,&gpioConfigStruct);
    GPIO_Config(GPIOC,&gpioConfigStruct);
    GPIO_Config(GPIOD,&gpioConfigStruct);

    /* Disable GPIO clock*/
    RCM_DisableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB |
                              RCM_APB2_PERIPH_GPIOC | RCM_APB2_PERIPH_GPIOD);

    /* Configure EINT line to wake up system*/
    BOARD_BUTTON_Config(BUTTON_KEY1,BUTTON_MODE_EINT);

    Delay(0x3FFFFF);
    /* Watting to enter SLEEP mode*/
    __WFI();
    
    BOARD_LED_Config(LED2);
    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &usartConfigStruct);
    
    printf("\r\nWakeup from sleep mode.\r\n");
    
    while(1)
    {
        BOARD_LED_Toggle(LED2);
        Delay(0x3FFFFF);
    }
}

/*!
 * @brief     Consumption measure in standby mode
 *
 * @param     None
 *
 * @retval    None
 */
void ConsumptionMeasure_StandbyMode(void)
{
    PMU_EnableWakeUpPin();

    PMU_ClearStatusFlag(PMU_FLAG_WUE);

    PMU_EnterSTANDBYMode(PMU_STOP_ENTRY_WFI);

    /*Input high logic voltage to PA0 WKUP pin to wake up system*/
    while(1)
    {
    }
}

