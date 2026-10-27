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

This project demonstrates how to implement bidirectional polling communication between
two development boards using the APM32F402 microcontroller via the USART interface.
The two boards are referred to as Board A and Board B, where Board A sends data to Board B,
and Board B receives and echoes back the data.

BoardA and BoardB:
  - USART1 configured as follow:
  - BaudRate    = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit    = USART_StopBits_1
  - Parity      = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Transmit enabled

&par Hardware Connection

Board A
  - The TX (transmit) pin is connected to the RX (receive) pin of Board B.
  - The RX (receive) pin is connected to the TX (transmit) pin of Board B.
  - Common Ground (GND)

&par Directory contents

  - USART/USART_TwoBoardsPolling/BoardA/Source/apm32f402_403_int.c     Interrupt handlers
  - USART/USART_TwoBoardsPolling/BoardA//Source/main.c              Main program
  - USART/USART_TwoBoardsPolling/BoardB/Source/apm32f402_403_int.c     Interrupt handlers
  - USART/USART_TwoBoardsPolling/BoardB//Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
