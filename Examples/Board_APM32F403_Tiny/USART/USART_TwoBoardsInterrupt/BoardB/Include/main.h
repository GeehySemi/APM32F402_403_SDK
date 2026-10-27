/**
 * @file        main.h
 *
 * @brief       Header for main.c module
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
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f402_403.h"
#include "board_apm32f402_403_tiny.h"
#include "apm32f402_403_usart.h"
#include "apm32f402_403_misc.h"
#include <stdio.h>

/* Exported macro *********************************************************/
#define DATA_BUF_SIZE       (32)

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
extern volatile uint32_t rxCountUSART1;
extern volatile uint32_t txCountUSART1;
extern uint8_t txDataBufUSART1[DATA_BUF_SIZE];
extern uint8_t rxDataBufUSART1[DATA_BUF_SIZE];

void  USART_Isr(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
