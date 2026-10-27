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

This example shows how to config TMR peripherals to generate a Single Pulse
with an external trigger

  - TMR1 Configure Single Pulse Mode and Enable event is used as Trigger Output
  - The TMR1 peripherals clock is 120MHz.
  - Configure Timer TMR1 counter clock at 1 MHz:
  - TMR1 counter clock = (120M) / (119 + 1) = 1MHz

  - TMR1->AUTORLD is 999, so the trigger input max frequency value
  - is 1MHz / (999 + 1) = 1KHz .
  - TMR1 pulse = 500
  - Single Pulse delay value = 500 / (1MHz) = 500us
  - And the pulse value is fixed to single pulse value
  - Pulse value = (period - pulse) / counter clock
  - = (1000 - 500)/ 1MHz = 500us

  - using TMR1 CHANNEL1(PA8) to output Pulse by TMR1 CHANNEL2(PA9)
  - using TMR1 CHANNEL2(PA9) connect the external signal

&par Directory contents

  - TMR/TMR_SinglePulse/Source/main.c                 Main program
  - TMR/TMR_SinglePulse/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F403 TINY Devices.
