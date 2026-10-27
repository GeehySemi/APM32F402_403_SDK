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

This example describes how to use ADC1 to monitor the voltage of ADC1_Channel14 continuously.
If the voltage on ADC1_Channel14(PC4) is not in the thresholds which is set before, analog watchdog
interrupt will generate and light LED2.

&par Directory contents

  - ADC/ADC_AnalogWindowWatchdog/Source/main.c                  Main program
  - ADC/ADC_AnalogWindowWatchdog/Source/apm32f4xx_int.c         Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
