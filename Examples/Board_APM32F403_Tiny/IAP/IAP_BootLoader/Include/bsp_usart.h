/*!
 * @file        bsp_usart.h
 *
 * @brief       Header for bsp_usart.c module
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

/* Define to prevent recursive inclusion */
#ifndef __BSP_USART_H
#define __BSP_USART_H

/* Includes */
#include "apm32f402_403.h"
#include "board_apm32f402_403_tiny.h"
#include "apm32f402_403_gpio.h"
#include "apm32f402_403_usart.h"


/* Exported macro *********************************************************/
#define DEBUG_USART                         USART1
#define DEBUG_USART_CLK                     RCM_APB2_PERIPH_USART1
#define DEBUG_USART_TX_CLK                  RCM_APB2_PERIPH_GPIOA
#define DEBUG_USART_TX_PORT                 GPIOA
#define DEUBG_USART_TX_SOURCE               GPIO_PIN_SOURCE_9
#define DEBUG_USART_TX_PIN                  GPIO_PIN_9
#define DEBUG_USART_RX_CLK                  RCM_APB2_PERIPH_GPIOA
#define DEBUG_USART_RX_PORT                 GPIOA
#define DEUBG_USART_RX_SOURCE               GPIO_PIN_SOURCE_10
#define DEBUG_USART_RX_PIN                  GPIO_PIN_10
#define DEBUG_USART_BAUDRATE                115200

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
void USART_Init(uint32_t baudRate);

#endif
