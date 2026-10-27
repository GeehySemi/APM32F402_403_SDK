/**
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
 *
 * @version     V1.0.1
 *
 * @date        2024-02-14
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

This example shows how to configure the TMR3 peripheral to generate PWM signals
with different duty cycles.
The TMR3 waveform can be displayed using an oscilloscope.

Using TMR3 CHANNEL1(PA6) to output PWM
If using TMR3 parital remap, the TMR3 CHANNEL1(PB4) be used to output PWM

&par Directory contents

  - TMR/TMR_TMR3PWMOutput/Source/main.c                 Main program
  - TMR/TMR_TMR3PWMOutput/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F403 TINY Devices.
