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

This example demonstrates I2S (Inter-IC Sound) communication between a master
device and a slave device using SPI peripherals. The master device transmits audio
data, while the slave device receives the data and compares it with the expected
values.

After pressing KEY1, the main device begins to send data. The subordinate device receives
the data and verifies its correctness. If the data is consistent, LED2 of the subordinate
device is illuminated.

Connect I2S2 and I2S3 pins as follows:
I2S2 WS    (PB12)   to  I2S2 WS  (PB12)
I2S2 CK    (PB13)   to  I2S2 CK   (PB13)
I2S2 SD    (PB15)   to  I2S2 SD   (PB15)

&par Directory contents

  - I2S/I2S_TwoBoardsInterrupt/Master/Source/apm32f402_403_int.c     Interrupt handlers
  - I2S/I2S_TwoBoardsInterrupt/Master/Source/main.c              Main program
  - I2S/I2S_TwoBoardsInterrupt/Slave/Source/apm32f402_403_int.c     Interrupt handlers
  - I2S/I2S_TwoBoardsInterrupt/Slave/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
