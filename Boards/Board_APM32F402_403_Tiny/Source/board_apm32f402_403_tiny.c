/**
 * @file        board_apm32f403_tiny.c
 *
 * @brief       This file provides firmware functions to manage Leds and key buttons
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
#include "board_apm32f402_403_tiny.h"

#if defined(BOARD_APM32F402_403_TINY)

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define LED_NUM                     2U
#define BUTTON_NUM                  2U
#define COM_NUM                     2U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
GPIO_T* LED_PORT[LED_NUM] = {LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t LED_PIN[LED_NUM] = {LED2_PIN, LED3_PIN};
const uint32_t LED_CLK[LED_NUM] = {LED2_GPIO_CLK, LED3_GPIO_CLK};

GPIO_T* BUTTON_PORT[BUTTON_NUM] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT};
const uint16_t BUTTON_PIN[BUTTON_NUM] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN};
const uint32_t BUTTON_CLK[BUTTON_NUM] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EINT_LINE[BUTTON_NUM] = {KEY1_BUTTON_EINT_LINE, KEY2_BUTTON_EINT_LINE};
const GPIO_PORT_SOURCE_T BUTTON_PORT_SOURCE[BUTTON_NUM] = {KEY1_BUTTON_EINT_PORT_SOURCE, KEY2_BUTTON_EINT_PORT_SOURCE};
const GPIO_PIN_SOURCE_T BUTTON_PIN_SOURCE[BUTTON_NUM] = {KEY1_BUTTON_EINT_PIN_SOURCE, KEY2_BUTTON_EINT_PIN_SOURCE};
const IRQn_Type BUTTON_IRQn[BUTTON_NUM] = {KEY1_BUTTON_EINT_IRQ_NUM, KEY2_BUTTON_EINT_IRQ_NUM};

USART_T* COM_PORT[COM_NUM] = {COM1_PORT, COM2_PORT};
const uint32_t COM_PORT_CLK[COM_NUM] = {COM1_PORT_CLK, COM2_PORT_CLK};
GPIO_T* COM_TX_PORT[COM_NUM] = {COM1_TX_GPIO_PORT, COM2_TX_GPIO_PORT};
GPIO_T* COM_RX_PORT[COM_NUM] = {COM1_RX_GPIO_PORT, COM2_RX_GPIO_PORT};
const uint16_t COM_TX_PIN[COM_NUM] = {COM1_TX_PIN, COM2_TX_PIN};
const uint16_t COM_RX_PIN[COM_NUM] = {COM1_RX_PIN, COM2_RX_PIN};
const uint32_t COM_TX_PORT_CLK[COM_NUM] = {COM1_TX_GPIO_CLK, COM2_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COM_NUM] = {COM1_RX_GPIO_CLK, COM2_RX_GPIO_CLK};


/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief   Board LED configuration
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_Config(BOARD_LED_T led)
{
    GPIO_Config_T GPIO_ConfigStruct = {0U};

    /* Enable the GPIO_LED Clock */
    RCM_EnableAPB2PeriphClock(LED_CLK[led]);

    /* Configure the GPIO_LED pin */
    GPIO_ConfigStruct.pin = LED_PIN[led];
    GPIO_ConfigStruct.mode = GPIO_MODE_OUT_PP;
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;

    GPIO_Config(LED_PORT[led], &GPIO_ConfigStruct);
    LED_PORT[led]->BC = LED_PIN[led];
}

/*!
 * @brief   Board button configuration
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *              @arg BUTTON_KEY2: Key2 Push Button
 *
 * @param       Button_Mode: Specifies Button mode.
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_MODE_GPIO: Button will be used as simple IO
 *              @arg BUTTON_MODE_EINT: Button will be connected to EINT line
 *                   with interrupt generation capability
 *
 * @retval  None
 */
void BOARD_BUTTON_Config(BOARD_BUTTON_T button, BOARD_BUTTON_MODE_T mode)
{
    GPIO_Config_T GPIO_ConfigStruct = {0U};
    EINT_Config_T EINT_ConfigStruct = {0U};
    
    /* Enable the BUTTON Clock */
    RCM_EnableAPB2PeriphClock(BUTTON_CLK[button] | RCM_APB2_PERIPH_AFIO);
    
    /* Configure Button pin as input floating */
    GPIO_ConfigStruct.mode = GPIO_MODE_IN_PU;
    GPIO_ConfigStruct.pin = BUTTON_PIN[button];
    GPIO_Config(BUTTON_PORT[button], &GPIO_ConfigStruct);
    
    if (mode == BUTTON_MODE_EINT)
    {
        /* Connect Button EINT Line to Button GPIO Pin */
        GPIO_ConfigEINTLine(BUTTON_PORT_SOURCE[button], BUTTON_PIN_SOURCE[button]);

        /* Configure Button EINT line */
        EINT_ConfigStruct.line = BUTTON_EINT_LINE[button];
        EINT_ConfigStruct.mode = EINT_MODE_INTERRUPT;
        EINT_ConfigStruct.trigger = EINT_TRIGGER_FALLING;
        EINT_ConfigStruct.lineCmd = ENABLE;
        EINT_Config(&EINT_ConfigStruct);

        /* Enable and set Button EINT Interrupt to the lowest priority */
        NVIC_EnableIRQRequest(BUTTON_IRQn[button], 0x0F, 0x0F);
    }
}

/*!
 * @brief   Board COM configuration
 *
 * @param   com: Specifies the COM to be configured
 *              This parameter can be one of following parameters:
 *              @arg COM1
 *              @arg COM2
 *
 * @param   configStruct: Pointer to a USART_Config_T structure that contains the configuration information for the specified USART peripheral.
 *
 * @retval  None
 */
void BOARD_COM_Config(BOARD_COM_T com, USART_Config_T* configStruct)
{
    GPIO_Config_T GPIO_ConfigStruct = {0U};

    /* Enable GPIO clock */
    RCM_EnableAPB2PeriphClock(COM_TX_PORT_CLK[com] | COM_RX_PORT_CLK[com]);

    if (com == COM1)
    {
        RCM_EnableAPB2PeriphClock(COM_PORT_CLK[com]);
    }
    else
    {
        RCM_EnableAPB1PeriphClock(COM_PORT_CLK[com]);
    }

    /* Configure USART Tx as alternate function push-pull */
    GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
    GPIO_ConfigStruct.pin = COM_TX_PIN[com];
    GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
    GPIO_Config(COM_TX_PORT[com], &GPIO_ConfigStruct);

    /* Configure USART Rx as input floating */
    GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
    GPIO_ConfigStruct.pin = COM_RX_PIN[com];
    GPIO_Config(COM_RX_PORT[com], &GPIO_ConfigStruct);

    /* USART configuration */
    USART_Config(COM_PORT[com], configStruct);

    /* Enable USART */
    USART_Enable(COM_PORT[com]);
}

/*!
 * @brief   Turn on the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_On(BOARD_LED_T led)
{
    LED_PORT[led]->BC = LED_PIN[led];
}

/**
 * @brief   Turn off the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_Off(BOARD_LED_T led)
{
    LED_PORT[led]->BSC = LED_PIN[led];
}

/**
 * @brief   Toggles the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_Toggle(BOARD_LED_T led)
{
    LED_PORT[led]->ODATA ^= LED_PIN[led];
}

/**
 * @brief   Returns the selected button state
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *              @arg BUTTON_KEY2: Key2 Push Button
 *
 * @retval  The button GPIO pin value.
 */
uint32_t BOARD_BUTTON_GetState(BOARD_BUTTON_T button)
{
    return GPIO_ReadInputBit(BUTTON_PORT[button], BUTTON_PIN[button]);
}

#endif /* BOARD_APM32F403_TINY */
