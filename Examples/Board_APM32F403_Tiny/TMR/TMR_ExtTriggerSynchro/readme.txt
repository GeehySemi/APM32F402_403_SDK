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

This example shows how to synchronize TMR2 and TMR peripherals in cascade mode
with an external trigger

  - TMR2 is master for TMR3,
    - Configure toggle mode and enable event is used as trigger output
    - The TI2FP2(PA1) is used as input trigger
  - TMR3 is slave for TMR2, Configure Toggle Mode
    - Configure toggle mode and enable event is used as trigger output
    - The ITR1(TMR2) is used as input trigger
  - TMR4 is slave for TMR2, Configure Toggle Mode
    - The ITR1(TMR2) is used as input trigger

  - The TMR2 peripherals clock is 60MHz.

  - The Master Timer TMR2 frequency :
  - TMR2 frequency = (120M) / (59 + 1) / 2 = 1MHz

  - The TMR3 is running at:
  - TMR3 frequency = (1M) / (1 + 1) = 500KHz

  - The TMR4 is running at:
  - TMR4 frequency = (1M) / (3 + 1) = 250KHz

The starts and stops of the TMR2 counters are controlled by the external trigger.
The TMR3 starts and stops are controlled by the TMR2, and the TMR4 starts and
stops are controlled by the TMR3.

  - using TMR2 CHANNEL2(PA1) to get input trigger
  - using TMR2 CHANNEL1(PA0) to output PWM
  - using TMR3 CHANNEL1(PA6) to output PWM
  - using TMR4 CHANNEL1(PB6) to output PWM

&par Directory contents

  - TMR/TMR_ExtTriggerSynchro/Source/main.c                 Main program
  - TMR/TMR_ExtTriggerSynchro/Source/apm32f4xx_int.c        Interrupt handlers

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F403 TINY Devices.
