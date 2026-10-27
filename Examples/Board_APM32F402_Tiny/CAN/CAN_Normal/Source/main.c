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
#include <stdio.h>

/* Private macro **********************************************************/
#define DEBUG_USART  USART1

/* Private typedef ********************************************************/
enum
{
    FAILED = 0,  /*!< Failed value */
    PASSED = 1   /*!< Passed value */
};

enum
{
    CAN_BAUD_125K = 0,  /*!< CAN baud rate config to 125K */
    CAN_BAUD_250K = 1,  /*!< CAN baud rate config to 250K */
    CAN_BAUD_500K = 2,  /*!< CAN baud rate config to 500K */
    CAN_BAUD_1M   = 3   /*!< CAN baud rate config to 1M */
};

/* Private variables ******************************************************/

CAN_Config_T       CAN_ConfigStructure;
CAN_FilterConfig_T CAN_FilterStruct;
CAN_TxMessage_T TxMessage;
CAN_RxMessage_T RxMessage;
/* Private function prototypes ********************************************/
void CAN_Init(uint8_t baud);
void NVIC_Configuration(void);
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
    USART_Config_T USART_ConfigStruct;

    SystemClockConfig();

    /* USART config */
    USART_ConfigStructInit(&USART_ConfigStruct);
    USART_ConfigStruct.baudRate = 115200;
    USART_ConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    USART_ConfigStruct.mode = USART_MODE_TX;
    USART_ConfigStruct.parity = USART_PARITY_NONE;
    USART_ConfigStruct.stopBits = USART_STOP_BIT_1;
    USART_ConfigStruct.wordLength = USART_WORD_LEN_8B;
    BOARD_COM_Config(COM1, &USART_ConfigStruct);

    /* Configures LED 2,3 */
    BOARD_LED_Config(LED2);
    BOARD_LED_Config(LED3);

    /* Turns selected LED Off */
    BOARD_LED_Off(LED2);
    BOARD_LED_Off(LED3);

    BOARD_BUTTON_Config(BUTTON_KEY1, BUTTON_MODE_GPIO);
    BOARD_BUTTON_Config(BUTTON_KEY2, BUTTON_MODE_GPIO);

    CAN_Init(CAN_BAUD_125K);

    NVIC_Configuration();

    CAN_EnableInterrupt(CAN1, CAN_INT_F0MP);

    CAN_EnableInterrupt(CAN2, CAN_INT_F0MP);

    while (1)
    {
        if (BOARD_BUTTON_GetState(BUTTON_KEY1) == RESET)
        {
            /* Turn on led LD2 */
            BOARD_LED_On(LED2);
            TxMessage.data[0]  = 0x55;
            printf("CAN1 Transmit 0x55 !\r\n");
            CAN_TxMessage(CAN1, &TxMessage);

            while (BOARD_BUTTON_GetState(BUTTON_KEY1) == RESET);
        }

        if (BOARD_BUTTON_GetState(BUTTON_KEY2) == RESET)
        {
            /* Turn on led LD2 */
            BOARD_LED_On(LED2);
            TxMessage.data[0]  = 0xAA;
            printf("CAN2 Transmit 0xAA !\r\n");
            CAN_TxMessage(CAN2, &TxMessage);
            while (BOARD_BUTTON_GetState(BUTTON_KEY2) == RESET);
        }
    }
}

/*!
 * @brief     CAN GPIO initialization
 *
 * @param     None
 *
 * @retval    None
 */
void CAN_GPIOInit(void)
{
    GPIO_Config_T  GPIO_InitStructure;

    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);

    GPIO_InitStructure.pin   = GPIO_PIN_9;       // CAN1 Tx remap 2
    GPIO_InitStructure.mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.pin = GPIO_PIN_8;         // CAN1 Rx remap 2
    GPIO_InitStructure.mode = GPIO_MODE_IN_PU;
    GPIO_Config(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.pin   = GPIO_PIN_13;       // CAN2 Tx remap 1
    GPIO_InitStructure.mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.speed = GPIO_SPEED_50MHz;
    GPIO_Config(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.pin  = GPIO_PIN_12;        // CAN2 Rx remap 1
    GPIO_InitStructure.mode = GPIO_MODE_IN_PU;
    GPIO_Config(GPIOB, &GPIO_InitStructure);

    GPIO_ConfigPinRemap(GPIO_REMAP1_CAN1);
}

/*!
 * @brief     CAN initialization
 *
 * @param     baud: baud rate for CAN
 *                  This parameter can be one of the following values:
 *                  @arg CAN_BAUD_125K : CAN baud rate config to 125K
 *                  @arg CAN_BAUD_250K : CAN baud rate config to 250K
 *                  @arg CAN_BAUD_500K : CAN baud rate config to 500K
 *                  @arg CAN_BAUD_1M   : CAN baud rate config to 1M
 *
 * @retval    None
 */
void CAN_Init(uint8_t baud)
{
    CAN_Config_T       CAN_ConfigStructure;
    CAN_FilterConfig_T CAN_FilterStruct;
    uint32_t apb1Clock;
    uint16_t prescaler;
    uint16_t timeSegment1;
    uint16_t timeSegment2;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1 | RCM_APB1_PERIPH_CAN2);

    RCM_ReadPCLKFreq(&apb1Clock, NULL);
    apb1Clock /= 1000000;

    /* Config GPIO for CAN */
    CAN_GPIOInit();

    /* CAN register init */
    CAN_Reset(CAN1);
    CAN_Reset(CAN2);

    CAN_ConfigStructInit(&CAN_ConfigStructure);

    /* CAN cell init */
    CAN_ConfigStructure.autoBusOffManage = DISABLE;
    CAN_ConfigStructure.autoWakeUpMode   = DISABLE;
    CAN_ConfigStructure.nonAutoRetran    = DISABLE;
    CAN_ConfigStructure.rxFIFOLockMode   = DISABLE;
    CAN_ConfigStructure.txFIFOPriority   = ENABLE;
    CAN_ConfigStructure.mode             = CAN_MODE_NORMAL;

    /* Baudrate = PCLK1 / (prescaler * (timeSegment1 + timeSegment2 + 1)) */
    CAN_ConfigStructure.syncJumpWidth = CAN_SJW_1;
    /* When APB1 Clock is not 60M, you should calculate by you clock!! */

    if (baud == CAN_BAUD_500K)
    {
        CAN_ConfigStructure.timeSegment1  = CAN_TIME_SEGMENT1_2;
        CAN_ConfigStructure.timeSegment2  = CAN_TIME_SEGMENT2_3;
        CAN_ConfigStructure.prescaler     = 20;
    }
    else if (baud == CAN_BAUD_1M)
    {
        CAN_ConfigStructure.timeSegment1  = CAN_TIME_SEGMENT1_2;
        CAN_ConfigStructure.timeSegment2  = CAN_TIME_SEGMENT2_3;
        CAN_ConfigStructure.prescaler     = 20/2;
    }
    else //!< 125K and 250K
    {
        CAN_ConfigStructure.timeSegment1  = CAN_TIME_SEGMENT1_3;
        CAN_ConfigStructure.timeSegment2  = CAN_TIME_SEGMENT2_4;
        CAN_ConfigStructure.prescaler     = apb1Clock >> baud;
    }
    CAN_Config(CAN1, &CAN_ConfigStructure);
    CAN_Config(CAN2, &CAN_ConfigStructure);

    /* CAN filter init */
    CAN_FilterStruct.filterMode       = CAN_FILTER_MODE_IDMASK;
    CAN_FilterStruct.filterScale      = CAN_FILTER_SCALE_32BIT;
    CAN_FilterStruct.filterIdHigh     = 0x0000;
    CAN_FilterStruct.filterIdLow      = 0x0000;
    CAN_FilterStruct.filterMaskIdHigh = 0x0000;
    CAN_FilterStruct.filterMaskIdLow  = 0x0000;
    CAN_FilterStruct.filterFIFO       = CAN_FILTER_FIFO_0;
    CAN_FilterStruct.filterActivation = ENABLE;

    /* CAN1 filter can be from 0 to 13 */
    CAN_FilterStruct.filterNumber = 1;
    CAN_ConfigFilter(&CAN_FilterStruct);

    /* CAN2 filter can be from 14 to 27(using CAN1 register) */
    CAN_FilterStruct.filterNumber = 15;
    CAN_FilterStruct.filterIdHigh = 0x0000;
    CAN_ConfigFilter(&CAN_FilterStruct);

    /* Transmit */
    TxMessage.stdID = 0x321;
    TxMessage.extID = 0x01;
    TxMessage.remoteTxReq = CAN_RTXR_DATA;
    TxMessage.typeID = CAN_TYPEID_STD;
    TxMessage.dataLengthCode = 1;

    prescaler = CAN_ConfigStructure.prescaler;
    timeSegment1 = CAN_ConfigStructure.timeSegment1;
    timeSegment2 = CAN_ConfigStructure.timeSegment2;

    printf("   Configuration :\r\n");
    printf("   APB1 Clock : %d MHz\r\n", (uint16_t)apb1Clock);
    printf("   CAN Baud   : %d K\r\n",
           (uint16_t)(apb1Clock * 1000) / (prescaler * (timeSegment1 + timeSegment2 + 3)));
}

/*!
 * @brief     Configures NVIC and Vector Table base location
 *
 * @param     None
 *
 * @retval    None
 */
void NVIC_Configuration(void)
{
    NVIC_EnableIRQRequest(CAN1_RX0_IRQn, 0, 0);
    NVIC_EnableIRQRequest(CAN2_RX0_IRQn, 1, 0);
}

/*!
 * @brief     This function handles CAN RX0 interrupt request
 *
 * @param     None
 *
 * @retval    None
 *
 */
void CAN1_RX0IRQRequest(void)
{
    CAN_RxMessage(CAN1, CAN_RX_FIFO_0, &RxMessage);

    if ((RxMessage.stdID == 0x321) && (RxMessage.typeID == CAN_TYPEID_STD) && (RxMessage.dataLengthCode == 1) && (RxMessage.data[0] == 0xAA))
    {
        BOARD_LED_On(LED2);
        printf("CAN1 Receive 0xAA !\r\n");
    }
    else
    {
        BOARD_LED_On(LED3);
        printf("CAN1 Receive Error !\r\n");
    }
}

/*!
 * @brief     This function handles CAN RX0 interrupt request
 *
 * @param     None
 *
 * @retval    None
 *
 */
void CAN2_RX0IRQRequest(void)
{
    CAN_RxMessage(CAN2, CAN_RX_FIFO_0, &RxMessage);

    if ((RxMessage.stdID == 0x321) && (RxMessage.typeID == CAN_TYPEID_STD) && (RxMessage.dataLengthCode == 1) && (RxMessage.data[0] == 0x55))
    {
        BOARD_LED_On(LED2);
        printf("CAN2 Receive 0x55 !\r\n");
    }
    else
    {
        BOARD_LED_On(LED3);
        printf("CAN2 Receive Error !\r\n");
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

    /* wait for the data to be send */
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

    /* wait for the data to be send */
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
