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
#define BAKR_DATA_COUNT              42

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
BAKPR_DATA_T BKPDataReg[BAKR_DATA_COUNT] =
{
    BAKPR_DATA1, BAKPR_DATA2, BAKPR_DATA3, BAKPR_DATA4, BAKPR_DATA5, BAKPR_DATA6, BAKPR_DATA7, BAKPR_DATA8,
    BAKPR_DATA9, BAKPR_DATA10, BAKPR_DATA11, BAKPR_DATA12, BAKPR_DATA13, BAKPR_DATA14, BAKPR_DATA15, BAKPR_DATA16,
    BAKPR_DATA17, BAKPR_DATA18, BAKPR_DATA19, BAKPR_DATA20, BAKPR_DATA21, BAKPR_DATA22, BAKPR_DATA23, BAKPR_DATA24,
    BAKPR_DATA25, BAKPR_DATA26, BAKPR_DATA27, BAKPR_DATA28, BAKPR_DATA29, BAKPR_DATA30, BAKPR_DATA31, BAKPR_DATA32,
    BAKPR_DATA33, BAKPR_DATA34, BAKPR_DATA35, BAKPR_DATA36, BAKPR_DATA37, BAKPR_DATA38, BAKPR_DATA39, BAKPR_DATA40,
    BAKPR_DATA41, BAKPR_DATA42
};

volatile uint32_t TimingDelay = 0;
/* Private function prototypes ********************************************/
void WriteDataToBackupReg(uint16_t data);
uint32_t CheckBackupRegData(uint16_t data);
void Delay(void);

/* External variables *****************************************************/

/* External functions *****************************************************/

/*!
 * @brief   Main program
 *
 * @param   None
 *
 * @retval  None
 */
int main(void)
{
    SystemClockConfig();

    /* NVIC configuration */
    NVIC_EnableIRQRequest(TAMPER_IRQn, 0, 0);

    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    RCM_EnableAPB1PeriphClock((RCM_APB1_PERIPH_T)(RCM_APB1_PERIPH_BAKR | RCM_APB1_PERIPH_PMU));

    PMU_EnableBackupAccess();

    BAKPR_DisableTamperPin();
    BAKPR_DisableInterrupt();
    BAKPR_ConfigTamperPinLevel(BAKPR_TAMPER_PIN_LEVEL_LOW);
    BAKPR_ClearStatusFlag();
    BAKPR_EnableInterrupt();
    BAKPR_EnableTamperPin();

    WriteDataToBackupReg(0x1234);

    /* Infinite loop */
    while (1)
    {
        /* Check if the written data are correct */
        if (!CheckBackupRegData(0x1234))
        {
            /* Toggle LED2 */
            BOARD_LED_Toggle(LED2);
            Delay();
        }
        else
        {
            /* Toggle LED3 */
            BOARD_LED_Toggle(LED3);
            Delay();
        }
    }
}

/*!
 * @brief       Writes data Backup DRx registers
 *
 * @param       data: data to be written to Backup data registers
 *
 * @retval      None
 *
 * @note
 */
void WriteDataToBackupReg(uint16_t data)
{
    uint32_t i = 0;

    for (i = 0; i < BAKR_DATA_COUNT; i++)
    {
        BAKPR_ConfigBackupRegister(BKPDataReg[i], data + (i * 0x02));
    }
}

/*!
 * @brief       Checks if the Backup DRx registers values are correct or not
 *
 * @param       data: data to be compared with Backup data registers
 *
 * @retval      0: All Backup DRx registers values are correct
 *
 * @note
 */
uint32_t CheckBackupRegData(uint16_t data)
{
    uint32_t i = 0;

    for (i = 0; i < BAKR_DATA_COUNT; i++)
    {
        if (BAKPR_ReadBackupRegister(BKPDataReg[i]) != (data + (i * 0x02)))
        {
            return (i + 1);
        }
    }

    return 0;
}

/*!
 * @brief       Checks if the Backup DRx registers are reset or not
 *
 * @param       FirstBackupData: data to be compared with Backup data registers
 *
 * @retval      0: All Backup DRx registers values are correct
 *
 * @note
 */
uint32_t CheckBackupRegIsReset(void)
{
    uint32_t i = 0;

    for (i = 0; i < BAKR_DATA_COUNT; i++)
    {
        if (BAKPR_ReadBackupRegister(BKPDataReg[i]))
        {
            return (i + 1);
        }
    }

    return 0;
}

/*!
 * @brief       Delay
 *
 * @param       None
 *
 * @retval      None
 *
 */
void Delay(void)
{
    volatile uint32_t delay = 0x5FFFF;

    while (delay--);
}

/*!
 * @brief   This function handles TAMPER interrupt request
 *
 * @param   None
 * 
 * @retval  None
 */
void TAMPER_IRQRequest(void)
{
    if (!CheckBackupRegIsReset())
    {
        BOARD_LED_Off(LED2);
    }
    else
    {
        BOARD_LED_Off(LED3);
    }

    BAKPR_ClearIntFlag();

    BAKPR_ClearStatusFlag();

    BAKPR_DisableTamperPin();

    BAKPR_EnableTamperPin();
}
