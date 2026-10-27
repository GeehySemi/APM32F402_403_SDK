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

This example describes how to use OTG to implement a composite device with CDC and WINUSB functional interfaces.

&par Hardware Description

The USART2 is configured as follows:
    - TX:PA2, RX:PA3
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

    - VCC5V            ----------> VBUS
    - OTG_FS_DM(PA11)  ----------> USB_DN
    - OTG_FS_DP(PA12)  ----------> USB_DP
    - GND              ----------> GND

&par Directory contents

  - Device_Example/OTGD_Composite_CDC_WINUSB/Source/main.c                 Main program
  - Device_Example/OTGD_Composite_CDC_WINUSB/Source/apm32f402_403_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
