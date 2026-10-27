/*!
 * @file        flash_read_write.c
 *
 * @brief       This file provides a flash read/write interface
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
#include "flash_read_write.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Specifies the start address of the sector. The purpose is to occupy a space at the specified address of MCU flash. */
#if defined (__CC_ARM)
    const uint8_t __attribute__((section(".ARM.__at_0x08004000"))) Flash_Para_Area[FLASH_READ_WRITE_TOTAL_SIZE];
#elif defined (__ICCARM__)
    #pragma location = 0x08004000
    __root const uint8_t Flash_Para_Area[FLASH_READ_WRITE_TOTAL_SIZE];
#elif defined (__GNUC__)

#else
    #warning Not supported compiler type
#endif

/* The buffer that write or erase page data */
static uint8_t Flash_Buffer[APM32_FLASH_PAGE_SIZE];

/* Private function prototypes ********************************************/
static int Flash_WriteOnePage(uint32_t writeAddr, const uint8_t *pData, uint32_t len);

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief     Write the specified length of data from the specified address.
 *            Can be written across page.
 *
 * @param     writeAddr: write address.
 *
 * @param     pData: save the write data.
 *
 * @param     len: write data length.
 *
 * @retval    Return Success or error status. It can be one of value:
 *            @arg -1         : Write data error.
 *            @arg flashStatus: FMC program status. The value refer to FMC_STATE_T.
 *
 * @note      Address and length must be 4-bytes aligned.
 *            The example must be performed in the page 17~19.
 *
 */
int Flash_Write(uint32_t writeAddr, uint8_t *pData, uint32_t len)
{
    uint32_t numOfPage = 0, numOfByte = 0, offsetAddr = 0;
    uint32_t count = 0, temp = 0;
    int writeStatus = 0;

    if ((len == 0))
        return FMC_STATUS_COMPLETE;

    /* address and len is not 4-bytes aligned */
    if ((writeAddr % 4 != 0) || (len % 4 != 0))
        return -1;

    /* offerset address in the page */
    offsetAddr = writeAddr % APM32_FLASH_PAGE_SIZE;

    /* The size of the remaining space in the page from writeAddr */
    count = APM32_FLASH_PAGE_SIZE - offsetAddr;

    /* Calculate how many pages to write */
    numOfPage = len / APM32_FLASH_PAGE_SIZE;

    /* Calculate how many bytes are left less than one page */
    numOfByte = len % APM32_FLASH_PAGE_SIZE;

    /* offsetAddr = 0, writeAddr is page aligned */
    if (offsetAddr == 0)
    {
        /* len less than one page, still consider it as one page */
        if(numOfPage == 0)
        {
            numOfPage = 1;
        }

        while(numOfPage--)
        {
            if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, len)) != FMC_STATUS_COMPLETE)
            {
                return writeStatus;
            }

            writeAddr +=  APM32_FLASH_PAGE_SIZE;
            pData += APM32_FLASH_PAGE_SIZE;
        }

        /* write remaining data */
        if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, numOfByte)) != FMC_STATUS_COMPLETE)
        {
            return writeStatus;
        }
    }
    /* offsetAddr != 0, writeAddr is not page aligned */
    else
    {
        /* len < APM32_FLASH_PAGE_SIZE, the data length is less than one page */
        if (numOfPage == 0)
        {
            /* numOfByte > count,  need to write across the page */
            if (numOfByte > count)
            {
                temp = numOfByte - count;
                /* fill the current page */
                if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, count)) != FMC_STATUS_COMPLETE)
                {
                    return writeStatus;
                }

                writeAddr +=  count;
                pData += count;
                /* write remaining data */
                if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, temp)) != FMC_STATUS_COMPLETE)
                {
                    return writeStatus;
                }
            }
            else
            {
                if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, len)) != FMC_STATUS_COMPLETE)
                {
                    return writeStatus;
                }
            }
        }
        /* len > APM32_FLASH_PAGE_SIZE */
        else
        {
            len -= count;
            numOfPage = len / APM32_FLASH_PAGE_SIZE;
            numOfByte = len % APM32_FLASH_PAGE_SIZE;

            /* write count data */
            if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, count)) != FMC_STATUS_COMPLETE)
            {
                return writeStatus;
            }

            writeAddr +=  count;
            pData += count;

            /* write numOfPage page */
            while (numOfPage--)
            {
                if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, APM32_FLASH_PAGE_SIZE)) != FMC_STATUS_COMPLETE)
                {
                    return writeStatus;
                }
                writeAddr +=  APM32_FLASH_PAGE_SIZE;
                pData += APM32_FLASH_PAGE_SIZE;
            }

            if (numOfByte != 0)
            {
                if ((writeStatus = Flash_WriteOnePage(writeAddr, pData, numOfByte)) != FMC_STATUS_COMPLETE)
                {
                    return writeStatus;
                }
            }
        }
    }

    return FMC_STATUS_COMPLETE;
}

/*!
 * @brief     Read the specified length of data from the specified address.
 *
 * @param     readAddr: read address.
 *
 * @param     pData: save the read data.
 *
 * @param     len: read data length.
 *
 * @retval    Return Success or error status. It can be one of value:
 *            @arg -1 : Read data error.
 *            @arg 0  : Read data successful.
 *
 * @note      Address and length must be 4-bytes aligned.
 *            The example must be performed in the sectors 1~3.
 *
 */
int Flash_Read(uint32_t readAddr, uint8_t *pData, uint32_t len)
{
    /* illegal address direct return */
    if ((readAddr < FLASH_READ_WRITE_START_ADDR) || ((readAddr + len) > FLASH_READ_WRITE_END_ADDR))
        return -1;

    /* illegal pointer direct return */
    if (pData == NULL)
        return -1;

    /* read data */
    for (uint32_t i = 0; i < len; i++)
    {
        pData[i] = (*(__IO uint8_t *)readAddr);
        readAddr += 1;
    }

    return 0;
}

/*!
 * @brief     In a page, write the specified length of data from the specified address.
 *
 * @param     writeAddr: write address.
 *
 * @param     pData: save the write data.
 *
 * @param     len: write data length.
 *
 * @retval    Return Success or error status. It can be one of value:
 *            @arg -1         : Write data error.
 *            @arg flashStatus: FMC program status. The value refer to FMC_STATUS_T.
 *
 * @note      Address and length must be 4-bytes aligned.
 *            The example must be performed in the sectors 17~19.
 *
 */
static int Flash_WriteOnePage(uint32_t writeAddr, const uint8_t *pData, uint32_t len)
{
    uint32_t startAddr;
    uint32_t offsetAddr;
    uint32_t i = 0;
    uint8_t *pTemp = Flash_Buffer;
    FMC_STATUS_T flashStatus = FMC_STATUS_COMPLETE;

    startAddr = writeAddr / APM32_FLASH_PAGE_SIZE * APM32_FLASH_PAGE_SIZE;
    offsetAddr = writeAddr % APM32_FLASH_PAGE_SIZE;

    if ((len == 0))
        return FMC_STATUS_COMPLETE;

    /* illegal address direct return */
    if ((writeAddr < FLASH_READ_WRITE_START_ADDR) || ((writeAddr + len) > FLASH_READ_WRITE_END_ADDR))
        return -1;

    /* illegal pointer direct return */
    if (pData == NULL)
        return -1;

    /* unlock flash for erase or write*/
    FMC_Unlock();

    /* read the entire page data to the buffer before write or erase */
    Flash_Read(startAddr, Flash_Buffer, APM32_FLASH_PAGE_SIZE);

    /* copy the data to the buffer */
    for (i = 0; i < len; i++)
    {
        Flash_Buffer[offsetAddr + i] = pData[i];
    }

    /* erase the page where the address is located */
    if ((flashStatus = FMC_ErasePage(startAddr)) != FMC_STATUS_COMPLETE)
    {
        return flashStatus;
    }

    /* write the entire page data */
    for (i = 0; i < APM32_FLASH_PAGE_SIZE / 4; i++)
    {
        if ((flashStatus = FMC_ProgramWord(startAddr, *(uint32_t *)pTemp)) != FMC_STATUS_COMPLETE)
        {
            return flashStatus;
        }
        startAddr += 4;
        pTemp += 4;
    }

    /* lock flash */
    FMC_Lock();

    return FMC_STATUS_COMPLETE;
}
