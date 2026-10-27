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


This example shows how to synchronize TMR peripherals in cascade mode.

  - TMR2 is master for TMR3,
    - Configure PWM1 mode and TMR2 update event is used as trigger output
  - TMR3 is slave for TMR2 and master for TMR4,
    - Configure PWM1 Mode and TMR3 Update event is used as Trigger Output
    - The ITR1(TMR2) is used as input trigger
  - TMR4 is slave for TMR3,
    - Configure PWM1 Mode
    - The ITR2(TMR3) is used as input trigger

  - The TMR2 peripherals clock is 120M.

  - The Master Timer TMR2 frequency:
  - TMR2 frequency = (120M) / (119 + 1) = 1 MHz
  - duty cycle = 60 / (119 + 1) = 50%.

  - The TMR3 is running at:
  - (1 MHz) / (99 + 1) = 10 KHz
  - duty cycle = 50 / (99 + 1) = 50%

  - The TMR4 is running at:
  - (10 KHz) / (9 + 1) = 1 KHz
  - duty cycle = 5 / (9 + 1) = 50%

  - using TMR2 CHANNEL1(PA0) to output PWM
  - using TMR3 CHANNEL1(PA6) to output PWM
  - using TMR4 CHANNEL1(PB6) to output PWM

&par Directory contents

  - TMR/TMR_CascadeSynchro/Source/main.c                 Main program
  - TMR/TMR_CascadeSynchro/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
