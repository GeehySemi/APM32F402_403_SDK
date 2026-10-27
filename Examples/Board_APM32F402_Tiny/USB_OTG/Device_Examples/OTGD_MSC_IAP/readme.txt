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

This example describes how to use a simulated USB drive device for IAP upgrade.
After plugging into the computer, the USB drive will display a 78KB BACKUP.bin file
and ready.txt. The former indicates the data readback of the APP area, and the latter
indicates that the USB drive is properly enumerated for the IAP upgrade.

By adding the .bin file extension to the USB flash drive, you can write its contents
into the corresponding APP area and execute it. When you return to the IAP upgrade code later,
the BACKUP.bin file will be refreshed.

The use of FLASH is as follows:
------------------------
|           |           |
|   50KB    |   78KB    |
|  MSC IAP  |    APP    |
|           |           |
------------------------

&par Directory contents

  - Device_Examples/OTGD_MSC_IAP/Source/apm32f402_403_int.c          Interrupt handlers
  - Device_Examples/OTGD_MSC_IAP/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.40
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
  - This feature is only applicable for WIN7/10/11 systems.
