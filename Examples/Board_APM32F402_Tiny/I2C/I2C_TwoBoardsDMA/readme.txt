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

This example shows how to control I2C devices and communicate between two different boards.

To use this example, you need to load the software into two APM32F402_TINY boards (let's call them
Board master and Board Slave) then connect these two boards through I2C lines and GND.

 - At startup, Boards master and slave are both in slave receiver mode and wait for messages
   to be received.
 - When KEY1 is pressed on Board master, the master transmitter sent data to Board slave.
   If the data received from the device is consistent with that sent by the main device,
   the LED2 of the device will be lit up.

&par Hardware Description

   I2C     BOARD1    BOARD2
   SCL      PB6            PB6     (External Pull up resistance)
   SDA      PB7            PB7     (External Pull up resistance)

using USART1(TX:PA9、RX:PA10).
  - USART1 configured as follow:
  - BaudRate = 115200
  - Word Length = USART_WordLength_8b
  - Stop Bit = USART_StopBits_1
  - Parity = USART_Parity_No
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

&par Directory contents

  - I2C/I2C_TwoBoardsDMA/Master/Source/apm32f402_403_int.c     Interrupt handlers
  - I2C/I2C_TwoBoardsDMA/Master/Source/main.c              Main program
  - I2C/I2C_TwoBoardsDMA/Slave/Source/apm32f402_403_int.c     Interrupt handlers
  - I2C/I2C_TwoBoardsDMA/Slave/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
