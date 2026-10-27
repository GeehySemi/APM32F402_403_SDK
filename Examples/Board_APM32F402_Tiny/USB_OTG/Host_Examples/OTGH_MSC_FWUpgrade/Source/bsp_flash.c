/*!
 * @file        bsp_flash.c
 *
 * @brief       Include flash operation
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
#include "bsp_flash.h"
#include "usb_host_user.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief       Unlocks Flash for write access
 *
 * @param       None
 *
 * @retval      None
 *
 */
void FLASH_FlashInit(void)
{
    /* Unlock the flash memory */
    FMC_Unlock();

    /* Clear all FMC flags */
    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE);
}

/*!
 * @brief       Get readout protection status
 *
 * @param       None
 *
 * @retval      Readout protection status
 *
 */
uint8_t FLASH_ReadOutProtectionStatus(void)
{
    uint8_t errSta = RESET;

    if (FMC_GetReadProtectionStatus() == SET)
    {
        errSta = SET;
    }

    return errSta;
}


/*!
 * @brief       Erase of all user flash area
 *
 * @param       address: application address
 *
 * @retval      flash operation status
 *
 */
uint8_t FLASH_EraseSectors(uint32_t address)
{
    FMC_OP_T errSta = FMC_OP_OK;

    FMC_STATUS_T eraseStatus = FMC_STATUS_COMPLETE;
    uint32_t eraseaddress = address & 0xFFFFFE00;

    while ((eraseStatus == FMC_STATUS_COMPLETE) && (eraseaddress <= USER_FLASH_END_ADDRESS) && \
            (gUsbHostFS.devInfo.connectedStatus == ENABLE))
    {
        eraseStatus = FMC_ErasePage(eraseaddress);
        eraseaddress += 0X200;
    }

    if (eraseStatus != FMC_STATUS_COMPLETE)
    {
        errSta = FMC_OP_ERROR;
    }

    return errSta;
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
FMC_STATUS_T FLASH_ProgramWord(uint32_t address, uint32_t data)
{
    FMC_STATUS_T errSta = FMC_STATUS_COMPLETE;

    errSta = FMC_ProgramWord(address, data);

    return errSta;
}
