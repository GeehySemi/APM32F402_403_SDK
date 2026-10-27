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

The program to show how to configure the TMR2 peripheral to generate 4 waveform
with 4 different frequencies (2.5KHz, 5KHz, 25KHz and 50KHz).

  - TMR2 frequency = 120 MHz. Prescaler = 119, TMR counter clock = 1 MHz

  - The TMR2 CC1 register = 200
  - Channel 1 waveform frequency = 2.5KHz

  - The TMR2 CC2 register = 100
  - Channel 2 waveform frequency = 5KHz

  - The TMR2 CC3 register = 20
  - Channel 3 waveform frequency = 25KHz

  - The TMR2 CC4 register = 10
  - Channel 4 waveform frequency = 50KHz

Display TMR2 waveform by oscilloscope.

  - using TMR2 Channel 1 (PA0) to output waveform
  - using TMR2 Channel 2 (PA1) to output waveform
  - using TMR2 Channel 3 (PA2) to output waveform
  - using TMR2 Channel 4 (PA3) to output waveform

&par Directory contents

  - TMR/TMR_OCToggle/Source/main.c                 Main program
  - TMR/TMR_OCToggle/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
