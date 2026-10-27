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
#include "fat32.h"
#include <stdint.h>
#include <string.h>

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

FAT32_INFO_T fat32Info;

#if (FAT32_MBR_HARDCODE > 0U)
static const uint8_t FAT32_BPB[] = {
    0xEB,                               /*00 - BS_jmpBoot */
    0xFE,                               /*01 - BS_jmpBoot */
    0x90,                               /*02 - BS_jmpBoot */
    'M','S','D','O','S','5','.','0',    /* 03-10 - BS_OEMName */
    0x00, 0x02,                         /*11 - BPB_BytesPerSec = 512 */
    0x01,                               /*13 - BPB_Sec_PerClus = 2K*1 = 2K*/
    0x7C, 0x11,                         /*14 - BPB_RsvdSecCnt = 0x117C * 512 / 1024 == 2238KB */
    0x02,                               /*16 - BPB_NumFATs = 2 */
    0x00, 0x00,                         /*17 - BPB_RootEntCnt = 512 */
    0x00, 0x00,                         /*19 - BPB_TotSec16 = 0 */
    0xF8,                               /*21 - BPB_Media = 0xF8 */
    0x00, 0x00,                         /*22 - BPBFATSz16 = 0x0000 */
    0x3F, 0x00,                         /*24 - BPB_SecPerTrk = 0x003F */
    0xFF, 0x00,                         /*26 - BPB_NumHeads = 0x00FF */
    0x3F, 0x00, 0x00, 0x00,             /*28 - BPB_HiddSec = 0x0000003F */
    0xC1, 0xC0, 0x03, 0x00,             /*32 - BPB_TotSec32 = 0x0003C0C1 120MB */
    0x42, 0x07, 0x00, 0x00,             /*36 - BPB_FATSz32  = 0x00000742 */
    0x00, 0x00,                         /*38 - BPB_ExtFlags  = 0x0000 */
    0x00, 0x00,                         /*40 - BPB_FSVer  = 0x0000 */
    0x02, 0x00, 0x00, 0x00,             /*42 - BPB_RootClus  = 0x00000002 */
    0x01, 0x00,                         /*46 - BPB_FSInfo  = 0x0001 */
    0x06, 0x00,                         /*48 - BS_Reserved */
    0x00, 0x00, 0x00, 0x00,             /*50 - BS_Reserved */
    0x00, 0x00, 0x00, 0x00,             /*54 - BS_Reserved */
    0x00, 0x00, 0x00, 0x00,             /*58 - BS_Reserved */
    0x80,                               /*62 - BS_DrvNum = 0x80 */
    0x00,                               /*63 - BS_Reserved1 = 0 , dirty bit = 0*/
    0x29,                               /*64 - BS_BootSig = 0x29 */
    0xB0, 0x49, 0x90, 0x02,             /*65 - BS_VolID = 0x029049B0 */
    'N','O',' ','N','A','M','E',' ',' ',' ',' ',  /*69 - BS_VolLab */
    'F','A','T','3','2',' ',' ',' '     /*80 - BS_FilSysType */
};
#endif

/* External functions *****************************************************/

/*!
 * @brief  fat32 check address align status
 *
 * @param  addr: operation address
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_CheckAddrAlign(uint32_t addr)
{
    USER_STATUS_T status = USER_OK;
    
    if(addr & (FAT32_SECTOR_SIZE - 1))
    {
        status = USER_ERROR;
    }
    
    return status;
}

/*!
 * @brief  fat32 read boot sector(DBR)
 *
 * @param  buffer: read buffer
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadBootSector(uint8_t *buffer)
{
    USER_STATUS_T status = USER_OK;
    
#if (FAT32_MBR_HARDCODE > 0U)
    memcpy(buffer, FAT32_BPB, sizeof(FAT32_BPB));
    memset(buffer + sizeof(FAT32_BPB), 0x00, FAT32_SECTOR_SIZE - sizeof(FAT32_BPB) - 2);
    buffer[510]                     = (uint8_t)FAT32_END_FLAG >> 8;
    buffer[511]                     = (uint8_t)FAT32_END_FLAG;
#else
    fat32Info.bpb = (FAT32_PBP_T*)buffer;
    
    memset(buffer, 0, FAT32_SECTOR_SIZE);
    
    fat32Info.bpb->BS_JmpBoot[0]         = 0xEB;
    fat32Info.bpb->BS_JmpBoot[1]         = 0xFE;
    fat32Info.bpb->BS_JmpBoot[2]         = 0x90;
    
    memcpy(fat32Info.bpb->BS_OEMName, FAT32_OEM_CODE, 8);
    fat32Info.bpb->BPB_BytsPerSec        = FAT32_SECTOR_SIZE;
    fat32Info.bpb->BPB_SecPerClus        = FAT32_SEC_PER_CLUS;
    fat32Info.bpb->BPB_RsvdSecCnt        = FAT32_RSV_SEC_CNT;
    fat32Info.bpb->BPB_NumFATs           = FAT32_FAT_NUM;
    fat32Info.bpb->BPB_Media             = 0xF8;
    fat32Info.bpb->BPB_SecPerTrk         = 0x003F;
    fat32Info.bpb->BPB_NumHeads          = 0x00FF;
    fat32Info.bpb->BPB_HiddSec           = 0x0000003F;
    fat32Info.bpb->BPB_TotSec32          = FAT32_TOTAL_SEC;
    
    fat32Info.bpb->BPB_FATSz32           = FAT32_FAT_SZ;
    fat32Info.bpb->BPB_ExtFlags          = 0x0000;
    fat32Info.bpb->BPB_FSVer             = 0x0000;
    fat32Info.bpb->BPB_RootClus          = FAT32_ROOT_CLUS_NUM;
    fat32Info.bpb->BPB_FSInfo            = 0x0001;
    fat32Info.bpb->BPB_BkBootSec         = 0x0006;
    fat32Info.bpb->BS_DrvNum             = 0x80;
    fat32Info.bpb->BS_BootSig            = 0x29;
    fat32Info.bpb->BS_VolID              = 0x029049B0;
    memcpy(fat32Info.bpb->BS_VolLab, "NO NAME    ", 11);
    memcpy(fat32Info.bpb->BS_FilSysType, FAT32_SYSTEM_ID, 8);
    
    buffer[510]                         = (uint8_t)(FAT32_END_FLAG >> 8);
    buffer[511]                         = (uint8_t)FAT32_END_FLAG;
#endif

    return status;
}

/*!
 * @brief  fat32 read FS Info 1 sector
 *
 * @param  buffer: read buffer
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadFSInfo1(uint8_t *buffer)
{
    USER_STATUS_T status = USER_OK;
    FAT32_FSINFO_T *fat32Fsinfo = (FAT32_FSINFO_T*)buffer;
    
    memset(buffer, 0, FAT32_SECTOR_SIZE);
    
    fat32Fsinfo->FSI_LeadSig    = FAT32_FSI_LEAD_SIGN;
    fat32Fsinfo->FSI_StrucSig   = FAT32_FSI_FILE_INFO_SIGN;
    fat32Fsinfo->FSI_Free_Count = 0x000398BE;
    fat32Fsinfo->FSI_Nxt_Free   = 0x00000805;
    buffer[510]                 = (uint8_t)(FAT32_END_FLAG >> 8);
    buffer[511]                 = (uint8_t)FAT32_END_FLAG;
    
    return status;
}

/*!
 * @brief  fat32 read FS Info 2 sector
 *
 * @param  buffer: read buffer
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadFSInfo2(uint8_t *buffer)
{
    USER_STATUS_T status = USER_OK;
    
    memset(buffer, 0, FAT32_SECTOR_SIZE);
    
    buffer[510] = (uint8_t)(FAT32_END_FLAG >> 8);
    buffer[511] = (uint8_t)FAT32_END_FLAG;
    
    return status;
}

/*!
 * @brief  fat32 read FAT table
 *
 * @param  buffer: read buffer
 *
 * @param  addr: read addr
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadFatTable(uint8_t *buffer, uint32_t addr)
{
    USER_STATUS_T status = USER_OK;
    uint32_t offset = (addr - FAT32_FAT1_START_ADDR) >> 2;
    uint32_t *buffer32 = (uint32_t*)buffer;
    uint32_t i;
    
    if(addr == FAT32_FAT1_START_ADDR)
    {
        buffer32[0] = 0x0FFFFFF8;
        buffer32[1] = 0x0FFFFFFF;
        buffer32[2] = 0x03;
        buffer32[3] = 0x04;
        buffer32[4] = 0x0FFFFFFF;

        for(i = 5; i < 0x80; i++)
        {
            buffer32[i] = offset + i + 1;
        }
        
    }
    else if(addr == FAT32_FAT1_END_ADDR)
    {
        buffer32[0] = 0x0FFFFFFF;
        
        for(i = 1; i < 0x80; i++)
        {
            buffer32[i] = 0x00000000;
        }
    }
    else
    {
        for(i = 0; i < 0x80; i++)
        {
            buffer32[i] = offset + i + 1;
            if(buffer32[i] == 161) //78*2+5
            {
                buffer32[i] = 0x0FFFFFFF;
            }
        }
    }
    
    return status;
}

/*!
 * @brief  fat32 read FAT table
 *
 * @param  buffer: read buffer
 *
 * @param  addr: read addr
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadFat2Table(uint8_t *buffer, uint32_t addr)
{
    USER_STATUS_T status = USER_OK;
    uint32_t offset = (addr - FAT32_FAT2_START_ADDR) >> 2;
    uint32_t *buffer32 = (uint32_t*)buffer;
    uint32_t i;
    
    if(addr == FAT32_FAT2_START_ADDR)
    {
        buffer32[0] = 0x0FFFFFF8;
        buffer32[1] = 0x0FFFFFFF;
        buffer32[2] = 0x0FFFFFFF;
        buffer32[3] = 0x0FFFFFFF;
        buffer32[4] = 0x0FFFFFFF;

        for(i = 5; i < 0x80; i++)
        {
            buffer32[i] = offset + i + 1;
        }
    }
    else if(addr == FAT32_FAT2_END_ADDR)
    {
        buffer32[0] = 0x0FFFFFFF;
        
        for(i = 1; i < 0x80; i++)
        {
            buffer32[i] = 0x00000000;
        }
    }
    else
    {
        for(i = 0; i < 0x80; i++)
        {
            buffer32[i] = offset + i + 1;
            if(buffer32[i] == 0x400)
            {
                buffer32[i] = 0x0FFFFFFF;
            }
        }
    }
    
    return status;
}

/*!
 * @brief  fat32 read root director
 *
 * @param  buffer: read buffer
 *
 * @param  fileName: file name
 *
 * @retval FAT32 status
 */
USER_STATUS_T FAT32_ReadDirEntry(uint8_t *buffer, uint8_t *fileName)
{
    USER_STATUS_T status = USER_OK;
    
    fat32Info.dir = (FAT32_DIR_ENTRY_T*)buffer;
    memset(buffer, 0, FAT32_SECTOR_SIZE);
    
    memcpy(fat32Info.dir->DIR_Name, FAT32_VOLUME_LABEL, 11);
    fat32Info.dir->DIR_Attr = FAT32_ATTR_VOLUME_ID;
    fat32Info.dir->DIR_NTRes = 0x00;
    fat32Info.dir->DIR_CrtTimeTenth = 0x00;
    fat32Info.dir->DIR_CrtTime = 0x0000;
    fat32Info.dir->DIR_CrtDate = 0x0000;
    fat32Info.dir->DIR_LstAccDate = 0x0000;
    fat32Info.dir->DIR_FstClusHI = 0x0000;
    fat32Info.dir->DIR_WrtTime = FAT32_MAKE_TIME(0,0);
    fat32Info.dir->DIR_WrtDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_FstClusLO = 0x0000;
    fat32Info.dir->DIR_FileSize = 0x00000000;
    
    ++fat32Info.dir;
  
    memcpy(fat32Info.dir->DIR_Name, FAT32_BACK_UP, 11);
    fat32Info.dir->DIR_Attr = FAT32_ATTR_ARCHIVE;
    fat32Info.dir->DIR_NTRes = 0x10;
    fat32Info.dir->DIR_CrtTimeTenth = 0x00;
    fat32Info.dir->DIR_CrtTime = FAT32_MAKE_TIME(0,0);
    fat32Info.dir->DIR_CrtDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_LstAccDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_FstClusHI = (uint16_t)(0x5 >> 16);
    fat32Info.dir->DIR_WrtTime = FAT32_MAKE_TIME(0,0);
    fat32Info.dir->DIR_WrtDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_FstClusLO = (uint16_t)(0x5);
    fat32Info.dir->DIR_FileSize = BACKUP_FILE_SIZE;
    ++fat32Info.dir;
    
    memcpy(fat32Info.dir->DIR_Name, fileName, 11);
    fat32Info.dir->DIR_Attr = FAT32_ATTR_ARCHIVE;
    fat32Info.dir->DIR_NTRes = 0x10;
    fat32Info.dir->DIR_CrtTimeTenth = 0x00;
    fat32Info.dir->DIR_CrtTime = FAT32_MAKE_TIME(0,0);
    fat32Info.dir->DIR_CrtDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_LstAccDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_FstClusHI = (uint16_t)(0 >> 16);
    fat32Info.dir->DIR_WrtTime = FAT32_MAKE_TIME(0,0);
    fat32Info.dir->DIR_WrtDate = FAT32_MAKE_DATE(10,02,2023);
    fat32Info.dir->DIR_FstClusLO = (uint16_t)(0);
    fat32Info.dir->DIR_FileSize = 0x00000000;

    return status;
}
