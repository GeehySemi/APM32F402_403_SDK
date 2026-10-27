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
#define FLASH_PAGE_SIZE         (0x400U)

#define FLASH_WRITE_START_ADDR  ((uint32_t)0x08008000)
#define FLASH_WRITE_END_ADDR    ((uint32_t)0x0800C000)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
uint32_t PageNumber     = 0;
uint32_t Address        = 0;
uint32_t EraseCounter   = 0;
uint32_t FlashStatus    = FMC_STATUS_COMPLETE;
uint32_t ProgramStatus  = 1;
uint32_t Data           = 0x89ABCDEF;

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

    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_FMC);

    PageNumber = (FLASH_WRITE_END_ADDR - FLASH_WRITE_START_ADDR) / FLASH_PAGE_SIZE;

    FMC_Unlock();

    /* Erase flash area */
    EraseCounter = 0;
    while((EraseCounter < PageNumber) && (FlashStatus == FMC_STATUS_COMPLETE))
    {
        FlashStatus = FMC_ErasePage(FLASH_WRITE_START_ADDR + (EraseCounter * FLASH_PAGE_SIZE));
        EraseCounter++;
    }

    /* Program flash */
    Address = FLASH_WRITE_START_ADDR;
    while((Address < FLASH_WRITE_END_ADDR) && (FlashStatus == FMC_STATUS_COMPLETE))
    {
        FlashStatus = FMC_ProgramWord(Address, Data);
        Address += 4;
    }

    FMC_Lock();

    /* Read back */
    Address = FLASH_WRITE_START_ADDR;

    while((Address < FLASH_WRITE_END_ADDR))
    {
        if(*((__IO uint32_t*) Address) != Data)
        {
            ProgramStatus = 0;
            break;
        }
        Address += 4;
    }

    if (ProgramStatus == 0)
    {
        BOARD_LED_On(LED3);
    }
    else
    {
        BOARD_LED_On(LED2);
    }

    /* Infinite loop */
    while (1)
    {
    }
}
