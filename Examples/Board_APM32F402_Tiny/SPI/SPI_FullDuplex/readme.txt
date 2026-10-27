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

This example describes how to use SPI peripheral.
by making a board, the master/slave full duplex communication between the SPI1 and SPI2.
If communication success, LED2 will turn on, otherwise LED3 will turn on.

Connect UART1 and SPI pins as follows:
SPI1 MOSI  (PA7)   to SPI2 MOSI (PB15)
SPI1 MISO  (PA6)   to SPI2 MISO (PB14)
SPI1 SCK   (PA5)   to SPI2 SCK  (PB13)
SPI1 SCK   (PA4)   to SPI2 SCK  (PB12)

&par Directory contents

  - SPI/SPI_FullDuplex/Source/main.c                 Main program
  - SPI/SPI_FullDuplex/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
