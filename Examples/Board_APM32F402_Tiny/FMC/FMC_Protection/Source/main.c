/**
 * @file        main.c
 *
 * @brief       Main program body
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

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define DEBUG_USART             COM1_PORT

#define FLASH_PAGE_SIZE         (0x400U)

#define FLASH_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define FLASH_PROTECTION_PAGE   (FLASH_WRP_PAGE_32_35)

#define FLASH_WRITE_DATE
//#define FLASH_ENABLE_WRITE_PROTECTION
#define FLASH_DISABLE_WRITE_PROTECTION
/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint32_t FlashStatus    = FMC_STATUS_COMPLETE;
uint32_t Data           = 0x89ABCDEF;
volatile uint32_t WriteProtect   = 0;
/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    SystemClockConfig();

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    FMC_Unlock();
    /* Read write protection page */
    WriteProtect = FMC_ReadOptionByteWriteProtection();

#ifdef FLASH_ENABLE_WRITE_PROTECTION
    /* Check whether pages need enable wirte protection */
    if((~WriteProtect) != FLASH_PROTECTION_PAGE)
    {
        FMC_EraseOptionBytes();
        /* Set write protection */
        FlashStatus = FMC_EnableWriteProtection(FLASH_PROTECTION_PAGE | (~WriteProtect));
        /* Reload the option byte */
        NVIC_SystemReset();
    }
#elif defined FLASH_DISABLE_WRITE_PROTECTION
    /* Check whether pages need disable wirte protection */
    if(((~WriteProtect) & FLASH_PROTECTION_PAGE) == FLASH_PROTECTION_PAGE)
    {
        FMC_EraseOptionBytes();
        /* Reset write protection */
        FlashStatus = FMC_EnableWriteProtection((~FLASH_PROTECTION_PAGE) & (~WriteProtect));
        /* Reload the option byte */
        NVIC_SystemReset();
    }
#endif

#ifdef FLASH_WRITE_DATE
    /* Erase flash page */
    FlashStatus = FMC_ErasePage(FLASH_WRITE_START_ADDR);
    /* Check for write protection errors */
    if(FlashStatus == FMC_STATUS_ERROR_WRP)
    {
        BOARD_LED_On(LED3);
    }
    else
    {
        /* Write page */
        FlashStatus = FMC_ProgramWord(FLASH_WRITE_START_ADDR, Data);
        /* Check whether writing successful */
        if((FlashStatus == FMC_STATUS_COMPLETE) && (*(uint32_t*)(FLASH_WRITE_START_ADDR) == Data))
        {
            BOARD_LED_On(LED2);
        }
    }
#endif

    FMC_Lock();
    
    /* Infinite loop */
    while (1)
    {

    }
}
