/*!
 * @file        fat32.h
 *
 * @brief       Header for fat32.c module
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
#ifndef FAT32_H
#define FAT32_H

/* Includes ***************************************************************/
#include "main.h"

/* Exported macro *********************************************************/

#define FAT32_MBR_HARDCODE          0
#define FAT32_SECTOR_SIZE           512

#define FAT32_TOTAL_SEC             0x0003C0C1
#define FAT32_RSV_SEC_CNT           0x117C
#define FAT32_FAT_SZ                0x00000742
#define FAT32_SEC_PER_CLUS          1
#define FAT32_FAT_NUM               2
#define FAT32_ROOT_CLUS_NUM         0x00000002
#define FAT32_FST_CLUS              0x00000005
#define FAT32_FAT1_START_ADDR       (uint32_t)(FAT32_RSV_SEC_CNT * FAT32_SECTOR_SIZE)
#define FAT32_FAT1_END_ADDR         (uint32_t)(FAT32_FAT1_START_ADDR + 16 * FAT32_SECTOR_SIZE)
#define FAT32_FAT2_START_ADDR       (uint32_t)((FAT32_RSV_SEC_CNT + FAT32_FAT_SZ) * FAT32_SECTOR_SIZE)
#define FAT32_FAT2_END_ADDR         (uint32_t)(FAT32_FAT2_START_ADDR + 16 * FAT32_SECTOR_SIZE)

#define FAT32_DIR_ENTRY_ADDR        (uint32_t)(((FAT32_RSV_SEC_CNT + FAT32_FAT_SZ * 2) + (FAT32_ROOT_CLUS_NUM - 2) * FAT32_SEC_PER_CLUS) * FAT32_SECTOR_SIZE)
#define FAT32_FIRST_FILE_END_ADDR   (uint32_t)(((FAT32_RSV_SEC_CNT + FAT32_FAT_SZ * 2) + (FAT32_FST_CLUS - 2) * FAT32_SEC_PER_CLUS) * FAT32_SECTOR_SIZE)

#define FAT32_APP_START_CLUS               ((FAT32_RSV_SEC_CNT + FAT32_FAT_SZ * 2) + (FAT32_FST_CLUS - 2) * FAT32_SEC_PER_CLUS)
#define FAT32_APP_END_CLUS                 (FAT32_APP_START_CLUS + 78*2 - 1)

#define FAT32_APP_START_ADDR               (FAT32_APP_START_CLUS * 512)
#define FAT32_APP_END_ADDR                 (FAT32_APP_END_CLUS * 512)

#define FAT32_SYSTEM_ID             "FAT32   "
#define FAT32_VOLUME_LABEL          "BOOTLOADER "
#define FAT32_BACK_UP               "BACKUP  BIN"
#define BACKUP_FILE_SIZE            0x13800
#define FAT32_OEM_CODE              "MSDOS5.0"
#define FAT32_END_FLAG              0x55AA

#define FAT32_FSI_LEAD_SIGN         0x41615252
#define FAT32_FSI_FILE_INFO_SIGN    0x61417272

#define FAT32_FILE_NAME_SIZE        0x0B
#define FAT32_ATTR_READ_ONLY        0x01
#define FAT32_ATTR_HIDDEN           0x02
#define FAT32_ATTR_SYSTEM           0x04
#define FAT32_ATTR_VOLUME_ID        0x08
#define FAT32_ATTR_DIRECTORY        0x10
#define FAT32_ATTR_ARCHIVE          0x20
#define FAT32_ATTR_LONG_NAME        (FAT32_ATTR_READ_ONLY | FAT32_ATTR_HIDDEN | FAT32_ATTR_SYSTEM | FAT32_ATTR_VOLUME_ID)

#define FAT32_MAKE_DATE(dd, mm, yyyy)   (uint16_t)( ( (((yyyy)-1980) & 0x7F)  << 9) | (((mm) & 0x0F) << 5) | ((dd) & 0x1F) )
#define FAT32_MAKE_TIME(hh,mm)          (uint16_t)(( ((hh) & 0x1F)<< 11) | (((mm) & 0x3F) << 5))

#define FAT32_MIN(a,b)              (((a) < (b)) ? (a) :(b))

/* Exported typedef *******************************************************/
//#pragma pack(1)
/*!
 * @brief    BIOS paramter block
 */
typedef struct
{
    uint8_t     BS_JmpBoot[3];
    uint8_t     BS_OEMName[8];
    uint16_t    BPB_BytsPerSec;
    uint8_t     BPB_SecPerClus;
    uint16_t    BPB_RsvdSecCnt;
    uint8_t     BPB_NumFATs;
    uint16_t    BPB_RootEntCnt;
    uint16_t    BPB_TotSec16;
    uint8_t     BPB_Media;
    uint16_t    BPB_FATSz16;
    uint16_t    BPB_SecPerTrk;
    uint16_t    BPB_NumHeads;
    uint32_t    BPB_HiddSec;
    uint32_t    BPB_TotSec32;

    /* FAT32 Structure */
    uint32_t    BPB_FATSz32;
    uint16_t    BPB_ExtFlags;
    uint16_t    BPB_FSVer;
    uint32_t    BPB_RootClus;
    uint16_t    BPB_FSInfo;
    uint16_t    BPB_BkBootSec;
    uint8_t     BS_Reserved1[12];
    uint8_t     BS_DrvNum;
    uint8_t     BS_Reserved2;
    uint8_t     BS_BootSig;
    uint32_t    BS_VolID;
    uint8_t     BS_VolLab[11];
    uint8_t     BS_FilSysType[8];
}__attribute__((packed)) FAT32_PBP_T;

/*!
 * @brief    FS info
 */
typedef struct
{
    uint32_t  FSI_LeadSig;
    uint8_t   FSI_Reserved1[480];
    uint32_t  FSI_StrucSig;
    uint32_t  FSI_Free_Count;
    uint32_t  FSI_Nxt_Free;
    uint8_t   FSI_Reserved2[12];
    uint32_t  FSI_TrailSig;
}__attribute__((packed)) FAT32_FSINFO_T;

/*!
 * @brief    Director entry information
 */
typedef struct
{
    uint8_t DIR_Name[11];
    uint8_t DIR_Attr;
    uint8_t DIR_NTRes;
    uint8_t DIR_CrtTimeTenth;

    uint16_t DIR_CrtTime;
    uint16_t DIR_CrtDate;
    uint16_t DIR_LstAccDate;
    uint16_t DIR_FstClusHI;
    uint16_t DIR_WrtTime;
    uint16_t DIR_WrtDate;
    uint16_t DIR_FstClusLO;
    uint32_t DIR_FileSize;
}__attribute__((packed)) FAT32_DIR_ENTRY_T;

/*!
 * @brief    FAT32 information
 */
typedef struct
{
    FAT32_PBP_T         *bpb;
    FAT32_DIR_ENTRY_T   *dir;
    FAT32_DIR_ENTRY_T   *entry;
}__attribute__((packed)) FAT32_INFO_T;

/* Exported function prototypes *******************************************/

USER_STATUS_T FAT32_CheckAddrAlign(uint32_t addr);
USER_STATUS_T FAT32_ReadBootSector(uint8_t *buffer);
USER_STATUS_T FAT32_ReadFSInfo1(uint8_t *buffer);
USER_STATUS_T FAT32_ReadFSInfo2(uint8_t *buffer);
USER_STATUS_T FAT32_ReadFatTable(uint8_t *buffer, uint32_t addr);
USER_STATUS_T FAT32_ReadFat2Table(uint8_t *buffer, uint32_t addr);
USER_STATUS_T FAT32_ReadDirEntry(uint8_t *buffer, uint8_t *fileName);

#endif /* FAT32_H */
