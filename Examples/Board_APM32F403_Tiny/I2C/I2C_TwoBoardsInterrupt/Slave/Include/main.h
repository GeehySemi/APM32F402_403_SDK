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
#include "apm32f402_403_i2c.h"
#include "apm32f402_403_misc.h"
#include <stdio.h>

/* Exported macro *********************************************************/

enum {FAILED, PASSED};

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/

void Delay(void);
void USART_Init(void);

/* I2C Init */
void I2C_Init(void);
/* I2C Write */
uint8_t I2C_Write(char * pBuffer);
/* I2C Interrupt */
void APM_TINY_PB_I2C_Isr(void);
void I2C_Isr(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
