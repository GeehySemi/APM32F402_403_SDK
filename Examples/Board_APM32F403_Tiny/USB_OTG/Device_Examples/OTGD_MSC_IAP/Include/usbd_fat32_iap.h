/*!
 * @file        usbd_fat32_iap.h
 *
 * @brief       Header for usbd_fat32_iap.c module
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
#ifndef USBD_FAT32_IAP_H
#define USBD_FAT32_IAP_H

/* Includes ***************************************************************/
#include "main.h"

/* Exported macro *********************************************************/

/* APM32F403xB - 128KB Flash Size */
#define FLASH_START_ADDR                0x08000000
#define FLASH_SIZE                      (128 * 1024)

/* 16KB max, bootloader size */
#define BOOTLOADER_SIZE                 0xC800
#define USER_APP_ADDR                   (0X800C800)
#define USER_APP_SIZE                   (0X8020000 - USER_APP_ADDR)

/* Exported typedef *******************************************************/

/*!
 * @brief    IAP operation type define
 */
typedef enum
{
    IAP_OP_OK,      /*!< IAP operation success */
    IAP_OP_BUSY,
    IAP_OP_ERROR    /*!< IAP operation error */
} IAP_OP_T;

/*!
 * @brief    IAP state
 */
typedef enum
{
    IAP_IDLE,
    IAP_READY,
    IAP_BUSY,
    IAP_OK,
    IAP_ERROR,
} IAP_STATE_T;

/*!
 * @brief    IAP information
 */
typedef struct
{
    uint8_t     state;
    uint32_t    fwBegin;
    uint32_t    fwEnd;
} IAP_INFO_T;

/*!
 * @brief    function hook
 */
typedef void (*pFunction)(void);

extern IAP_INFO_T iapInfo;

/* Exported function prototypes *******************************************/

USER_STATUS_T IAP_Init(void);
USER_STATUS_T IAP_FAT32_SetStatusFileName(const char * name);
void IAP_Jump2App(uint32_t addr);
USER_STATUS_T IAP_CheckAppCodeExist(void);
USER_STATUS_T IAP_FAT32_Read(uint8_t *buffer, uint32_t addr);
USER_STATUS_T IAP_FAT32_Write(uint8_t *buffer, uint32_t addr);

#endif /* USBD_FAT32_IAP_H */
