/*!
 * @file        flash_read_write.h
 *
 * @brief       Header for flash_read_write.c module
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

/* The following is the Flash region planning for this example. */

/* Exported macro *********************************************************/

/* flash sector satrt address */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000)   /* 1 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08005000)   /* 1 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x08006000)   /* 1 Kbytes */

/* apm32 flash start address */
#define APM32_FLASH_START_ADDR                    ((uint32_t)(0x08000000))
/* apm32 flash size */
#define APM32_FLASH_SIZE                          (128 * 1024)
/* apm32 flash end address */
#define APM32_FLASH_END_ADDR                      ((uint32_t)(APM32_FLASH_START_ADDR + APM32_FLASH_SIZE))
/* apm32 flash sector/page size */
#define APM32_FLASH_PAGE_SIZE                     ((uint32_t)(1024))

/* flash read write total size. This value must be a multiple of the page size. */
#define FLASH_READ_WRITE_TOTAL_SIZE               ((uint32_t)(APM32_FLASH_PAGE_SIZE * 2))

/* flash read write page start address, it's must be page aligned */
#define FLASH_READ_WRITE_START_ADDR               ADDR_FLASH_SECTOR_1

/* flash read write page start address */
#define FLASH_READ_WRITE_END_ADDR                 (ADDR_FLASH_SECTOR_1 + FLASH_READ_WRITE_TOTAL_SIZE)


/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/

/* Read the specified length of data from the specified address */
int Flash_Read(uint32_t readAddr, uint8_t *pData, uint32_t len);
/* Write the specified length of data from the specified address */
int Flash_Write(uint32_t writeAddr, uint8_t *pData, uint32_t len);
