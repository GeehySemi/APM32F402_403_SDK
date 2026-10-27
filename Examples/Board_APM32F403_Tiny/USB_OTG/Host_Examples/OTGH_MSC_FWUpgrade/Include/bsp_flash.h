/*!
 * @file        bsp_flash.h
 *
 * @brief       Header for bsp_flash.c module
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
#ifndef BSP_FLASH_H
#define BSP_FLASH_H

/* Includes ***************************************************************/
#include "apm32f402_403.h"
#include "apm32f402_403_fmc.h"

/* Exported macro *********************************************************/

/* flash memory start address */
#define USER_FLASH_START_ADDRESS    ((uint32_t)0x08000000)

/* user application address.
   Note: the 1st sector 0x08000000-0x0800F000 is reserved for the Firmware upgrade bootloader code */
#define APPLICATION_ADDRESS         ((uint32_t)0x0800F000)

/* flash memory end address */
#define USER_FLASH_END_ADDRESS      ((uint32_t)0x0801FFFF)

/* user application size */
#define USER_FLASH_SIZE             (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)

/* remain flash size */
#define REMAIN_FLASH_SIZE           (USER_FLASH_END_ADDRESS - USER_FLASH_START_ADDRESS + 1)

/* Exported typedef *******************************************************/
/*!
 * @brief    FMC operation type define
 */
typedef enum
{
    FMC_OP_OK,      /*!< Flash operation success */
    FMC_OP_ERROR    /*!< Flash operation error */
} FMC_OP_T;

/* Exported function prototypes *******************************************/

void FLASH_FlashInit(void);
uint8_t FLASH_ReadOutProtectionStatus(void);
uint8_t FLASH_EraseSectors(uint32_t address);
FMC_STATUS_T FLASH_ProgramWord(uint32_t address, uint32_t data);

#endif /* BSP_FLASH_H */
