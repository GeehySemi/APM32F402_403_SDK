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
#include "usbd_fat32_iap.h"
/* Private macro **********************************************************/

#define DEBUG_USART    USART2

/* Private typedef ********************************************************/

/**
 * @brief User application state
 */
typedef enum
{
    USER_APP_INIT,
    USER_APP_RUN,
} USER_APP_STATE_T;

/* Private variables ******************************************************/

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
    /* Set the Vector Table base address at 0x08000000 */
    NVIC_ConfigVectorTable(NVIC_VECT_TAB_FLASH, 0x0000);

    SystemClockConfig();

    BOARD_Delay_Config();
    
    /* Init USB device */
    USB_DeviceInit();
    
    IAP_Init();

    while(1)
    {
        switch(iapInfo.state)
        {
            case IAP_IDLE:
                break;
            
            case IAP_READY:
                IAP_FAT32_SetStatusFileName("READY");
                break;
            
            case IAP_OK:
                
                /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
                /* (*(__IO uint32_t*)USER_APP_ADDR) The saved value points to the top of the stack */
                /* Verify RAM Size:128K 0x20000 -> 0x2FFF_FFFF - (0x20000 - 1) */
                /* (*(__IO uint32_t*)USER_APP_ADDR) = 0x2FF1FFFF */
                if (((*(__IO uint32_t*)USER_APP_ADDR) & 0x2FFE0000 ) == 0x20000000)
                {
                    IAP_FAT32_SetStatusFileName("SUCCESS");
                    
                    iapInfo.state = IAP_IDLE;
                    BOARD_Delay_Ms(200);

                    USB_DeviceReset();
                    BOARD_Delay_Ms(100);
                    IAP_Jump2App(USER_APP_ADDR);
                }
                else
                {
                    IAP_FAT32_SetStatusFileName("INVALID");
                    iapInfo.state = IAP_IDLE;
                    BOARD_Delay_Ms(200);
                    USB_DeviceReset();
                    BOARD_Delay_Ms(200);
                    /* Configure USB device */
                    USB_DeviceInit();
                }
                break;
                
            default:
                IAP_FAT32_SetStatusFileName("ERROR");
                break;
        }
        
        BOARD_LED_Toggle(LED3);
        BOARD_Delay_Ms(200);
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
    UNUSED(file);
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif
