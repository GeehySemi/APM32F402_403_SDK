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

This example shows how to enter STOP mode and wake up from this mode through
EINT interrupt. There is an infinite loop that will keep LED2 blinking in main
program which means program is running.
Press KEY1, system enters STOP mode and LED2 turn off, LED3 turn on.
Press KEY2, system wake up from stop mode and LED2 blink, LED3 turn off.

&par Directory contents

  - PMU/PMU_Stop/Source/apm32f402_403_int.c     Interrupt handlers
  - PMU/PMU_Stop/Source/main.c              Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F403 TINY Devices.
