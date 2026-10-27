/*!
 * @file        usbd_fat32_iap.c
 *
 * @brief       USB device FAT32 IAP program body
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
#include "usbd_fat32_iap.h"
#include "fat32.h"
#include "apm32f402_403_fmc.h"
#include <stdint.h>
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

IAP_INFO_T iapInfo;

uint8_t FAT32_StatusFileName[FAT32_FILE_NAME_SIZE] = {
    'S','T','A','T','U','S',' ',' ','T','X','T'
};

uint32_t JumpAddress;
uint8_t *promptFileName;

pFunction Jump_To_Application;

/* External functions *****************************************************/

/*!
 * @brief       Unlocks Flash for write access
 *
 * @param       None
 *
 * @retval      None
 *
 */
void IAP_FlashInit(void)
{
    /* Unlock the flash memory */
    FMC_Unlock();

    /* Clear all FMC flags */
    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE);
}

/*!
 * @brief       Erase of all user flash area
 *
 * @param       address: application address
 *
 * @retval      flash operation status
 *
 */
IAP_OP_T IAP_FlashEraseSectors(uint32_t address)
{
    IAP_OP_T iapStatus = IAP_OP_OK;
    uint32_t eraseaddress = address;

    for(; eraseaddress < FLASH_SIZE; eraseaddress += 0X400)
    {
        FMC_ErasePage(address);
    }

    return iapStatus;
}

/*!
 * @brief       Programs a word at a specified address
 *
 * @param       address: specifies the address to be programmed
 *
 * @param       data: specifies the data to be programmed
 *
 * @retval      flash status
 *
 */
void IAP_FlashProgramWord(uint32_t address, uint32_t data)
{
    FMC_ProgramWord(address, data);
}

/*!
 * @brief       IAP Init
 *
 * @param       None
 *
 * @retval      operation status
 */
USER_STATUS_T IAP_Init(void)
{
    USER_STATUS_T status = USER_OK;

    iapInfo.fwBegin = FAT32_FIRST_FILE_END_ADDR;
    iapInfo.fwEnd = (FAT32_FIRST_FILE_END_ADDR + USER_APP_SIZE);

    iapInfo.state = IAP_READY;

    return status;
}

/*!
 * @brief       FAT32 read firmware
 *
 * @param       buffer: read buffer
 *
 * @param       addr: read address
 *
 * @retval      operation status
 */
static USER_STATUS_T IAP_ReadFirmware(uint8_t *buffer, uint32_t addr)
{
    USER_STATUS_T status = USER_OK;
    uint32_t addrbase = 0;
    uint32_t offset = 0;

    if(addr <= FAT32_APP_END_ADDR)
    {
        addrbase = USER_APP_ADDR;
        offset = addr - FAT32_APP_START_ADDR;
    }

    memcpy(buffer, (void*)(addrbase + offset), 512);

    return status;
}

/*!
 * @brief       FAT32 write firmware
 *
 * @param       buffer: read buffer
 *
 * @param       addr: read address
 *
 * @param       fileSize: file size
 *
 * @retval      operation status
 */
uint32_t writeFirmwareSize = 0;
static USER_STATUS_T IAP_WriteFirmware(uint8_t *buffer, uint32_t addr, uint32_t fileSize)
{
    USER_STATUS_T status = USER_OK;

    uint32_t offset = addr - iapInfo.fwBegin;
    uint32_t progAddr = USER_APP_ADDR + offset;
    uint32_t progSize = FAT32_MIN(FAT32_SECTOR_SIZE, iapInfo.fwEnd - iapInfo.fwBegin);
    const uint8_t *wbuf;
    uint32_t i;

    if(progSize & 0x03)
    {
        progSize += 4;
    }

    IAP_FlashInit();

    if(addr == iapInfo.fwBegin)
    {
        if(IAP_FlashEraseSectors(USER_APP_ADDR) != IAP_OP_OK)
        {
            status = USER_ERROR;
            /* Lock the Program memory */
            FMC_Lock();
            return status;
        }
    }

    if((progAddr >= USER_APP_ADDR) && (progAddr < (USER_APP_ADDR + USER_APP_SIZE)))
    {
        for(i = 0; i < progSize; i += 4)
        {
            wbuf = buffer + i;

            IAP_FlashProgramWord(progAddr + i, *((uint32_t*)wbuf));

            /* Verify Data */
            if((*(uint32_t*)(progAddr + i) != *(uint32_t*)(wbuf)))
            {
                status = USER_ERROR;
                return status;
            }
        }

        writeFirmwareSize += progSize;

        if(writeFirmwareSize >= fileSize)
        {
            iapInfo.state = IAP_OK;
        }
    }

    FMC_Lock();

    return status;
}

/*!
 * @brief       FAT32 read operation
 *
 * @param       buffer: read buffer
 *
 * @param       address: read address
 *
 * @retval      operation status
 */
USER_STATUS_T IAP_FAT32_Read(uint8_t *buffer, uint32_t address)
{
    USER_STATUS_T status = USER_OK;

    /* Not Align */
    if(FAT32_CheckAddrAlign(address) != USER_OK)
    {
        return USER_ERROR;
    }

    /* 0x0000 - 0x0C00 */
    if(address == 0x0000 || address == 0x0C00)
    {
        FAT32_ReadBootSector(buffer);
    }
    /* 0x0200 - 0x0E00 */
    else if(address == 0x0200 || address == 0x0E00)
    {
        FAT32_ReadFSInfo1(buffer);
    }
    /* 0x0400 - 0x1000 */
    else if(address == 0x0400 || address == 0x1000)
    {
        FAT32_ReadFSInfo2(buffer);
    }
    else if((address >= FAT32_FAT1_START_ADDR && address <= FAT32_FAT1_END_ADDR))
    {
        FAT32_ReadFatTable(buffer, address);
    }
    else if((address >= FAT32_FAT2_START_ADDR && address <= FAT32_FAT2_END_ADDR))
    {
        FAT32_ReadFat2Table(buffer, address);
    }
    else if(address == FAT32_DIR_ENTRY_ADDR)
    {
        promptFileName = FAT32_StatusFileName;

        FAT32_ReadDirEntry(buffer,promptFileName);
    }
    else if(address >= FAT32_APP_START_ADDR && address <= FAT32_APP_END_ADDR)
    {
        IAP_ReadFirmware(buffer, address);
    }
    else
    {
        memset(buffer, 0x00, FAT32_SECTOR_SIZE);
    }

    return status;
}

/*!
 * @brief       FAT32 write operation
 *
 * @param       buffer: read buffer
 *
 * @param       address: read address
 *
 * @retval      operation status
 */
USER_STATUS_T IAP_FAT32_Write(uint8_t *buffer, uint32_t addr)
{
    USER_STATUS_T status = USER_OK;
    uint32_t alignAddrEnd;
    uint32_t i;
    uint32_t clus;
    const uint8_t *offset;
    FAT32_DIR_ENTRY_T *entry;
    static uint32_t fileSize = 0;

    /* Not align */
    if(FAT32_CheckAddrAlign(addr) != USER_OK)
    {
        return USER_ERROR;
    }

    alignAddrEnd = (iapInfo.fwEnd & ~(FAT32_SECTOR_SIZE - 1)) + \
                   ((iapInfo.fwEnd & (FAT32_SECTOR_SIZE - 1)) ? FAT32_SECTOR_SIZE : 0);

    if(addr < FAT32_DIR_ENTRY_ADDR)
    {

    }
    else if(addr == FAT32_DIR_ENTRY_ADDR)
    {
        for(i = 0; i < FAT32_SECTOR_SIZE; i += sizeof(FAT32_DIR_ENTRY_T))
        {
            offset = (const uint8_t *)(buffer + i);

            entry = (FAT32_DIR_ENTRY_T*)offset;

            if(memcmp((void*) &entry->DIR_Name[8], "BIN", 3) == 0)
            {
                clus = (((uint32_t)(entry->DIR_FstClusHI)) << 16) | entry->DIR_FstClusLO;

                iapInfo.fwBegin = ((clus - 2) + FAT32_RSV_SEC_CNT + FAT32_FAT_SZ * 2 ) * FAT32_SECTOR_SIZE;
                iapInfo.fwEnd = iapInfo.fwBegin + FAT32_MIN(entry->DIR_FileSize, USER_APP_SIZE);

                fileSize = entry->DIR_FileSize;
            }
        }
    }
    else if(addr >= iapInfo.fwBegin && addr < alignAddrEnd)
    {
        status = IAP_WriteFirmware(buffer, addr, fileSize);
    }
    else
    {

    }

    return status;
}

/*!
  * @brief  IAP set status file name
  *
  * @param  name: file name
  *
  * @retval IAP status
  */
USER_STATUS_T IAP_FAT32_SetStatusFileName(const char * name)
{
    USER_STATUS_T status = USER_OK;
    uint8_t i;
    uint8_t len = (uint8_t)strlen(name);

    for(i = 0; i < 8 && i < len; i++)
    {
        FAT32_StatusFileName[i] = name[i];
    }

    /* Fill in the blanks */
    for(; i < 8; i++)
    {
        FAT32_StatusFileName[i] = ' ';
    }

    return status;
}

/*!
 * @brief       Jump to user application
 *
 * @param       addr: application address
 *
 * @retval      None
 *
 * @note
 */
void IAP_Jump2App(uint32_t addr)
{
    uint32_t address;

    /* Lock the Program memory */
    FMC_Lock();

    RCM->AHBRST = 0xFFFFFFFFU;
    RCM->APB1RST = 0xFFFFFFFFU;
    RCM->APB2RST = 0xFFFFFFFFU;

    RCM->AHBRST = 0x00000000U;
    RCM->APB1RST = 0x00000000U;
    RCM->APB2RST = 0x00000000U;

    RCM->AHBCLKEN = 0x00000000U;
    RCM->APB1CLKEN = 0x00000000U;
    RCM->APB2CLKEN = 0x00000000U;

    address = addr;

    /* Jump to user application */
    JumpAddress = *(__IO uint32_t *) (address + 4);
    Jump_To_Application = (pFunction) JumpAddress;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *) address);

    /* Jump to application */
    Jump_To_Application();
}

