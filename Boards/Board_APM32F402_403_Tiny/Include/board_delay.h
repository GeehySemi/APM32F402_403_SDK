/**
 * @file        board_delay.h
 *
 * @brief       This file contains definitions for APM32F403's delay function
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
#ifndef BOARD_DELAY_H
#define BOARD_DELAY_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f402_403.h"
#include "system_apm32f402_403.h"

/* Exported macro *********************************************************/

/* Exported typedef *******************************************************/

/* Exported variables *****************************************************/
extern volatile uint32_t delayCount;

/* Exported function prototypes *******************************************/
void BOARD_Delay_Config(void);
void BOARD_Delay_Ms(uint32_t count);
void BOARD_Delay_Us(uint32_t count);
void BOARD_Delay_IRQHandler(void);
uint32_t BOARD_ReadTick(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_DELAY_H */
