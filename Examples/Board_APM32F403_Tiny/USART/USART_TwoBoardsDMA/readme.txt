/**
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
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

&par Example Description

The program aims to show how to use KEY1 button to trigger USART communication using DMA, in this case,
you need to load program on two APM32F403_TINY boards. Then connect these two boards through USART lines and GND.
If USART RX Board receives the correct data from USART TX Board, USART RX Board LED2 will turn on.

BOARDA: TX
BOARDB: RX

&par Hardware Description

*---------------------------------------*
|  USART TX Board       USART RX Board  |
|                                       |
| USART1_TX(PA9)  ----> USART1_RX(PA10) |
| USART1_RX(PA10) ----> USART1_TX(PA9)  |
| GND                   GND             |
|                                       |
*---------------------------------------*

  - USART1 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - USART/USART_TwoBoardsInterrupt/BoardA/Source/apm32f402_403_int.c     Interrupt handlers
  - USART/USART_TwoBoardsInterrupt/BoardA//Source/main.c              Main program
  - USART/USART_TwoBoardsInterrupt/BoardB/Source/apm32f402_403_int.c     Interrupt handlers
  - USART/USART_TwoBoardsInterrupt/BoardB//Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F403 TINY Devices.
