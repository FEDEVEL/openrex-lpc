/*
 * @brief Basic CMSIS include file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __CMSIS_1343_H_
#define __CMSIS_1343_H_

#include "lpc_types.h"
#include "sys_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CMSIS_1343 CHIP: LPC1343 CMSIS include file
 * @ingroup CHIP_13XX_CMSIS_Drivers
 * @{
 */

#if defined(__ARMCC_VERSION)
// Kill warning "#pragma push with no matching #pragma pop"
  #pragma diag_suppress 2525
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
//  #pragma push // FIXME not usable for IAR
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
 
#if !defined(CHIP_LPC1343)
#error Incorrect or missing device variant (CHIP_LPC1343)
#endif

/** @defgroup CMSIS_1343_IRQ CHIP_1343: LPC1311/LPC1313/LPC1342/LPC1343 peripheral interrupt numbers
 * @{
 */

typedef enum LPC1343_IRQn {
	Reset_IRQn                    = -15,	/*!< 1  Reset Vector, invoked on Power up and warm reset */
	NonMaskableInt_IRQn           = -14,	/*!< 2 Non Maskable Interrupt                         */
	MemoryManagement_IRQn         = -12,	/*!< 4 Cortex-M3 Memory Management Interrupt          */
	BusFault_IRQn                 = -11,	/*!< 5 Cortex-M3 Bus Fault Interrupt                  */
	UsageFault_IRQn               = -10,	/*!< 6 Cortex-M3 Usage Fault Interrupt                */
	SVCall_IRQn                   = -5,		/*!< 11 Cortex-M3 SV Call Interrupt                   */
	DebugMonitor_IRQn             = -4,		/*!< 12 Cortex-M3 Debug Monitor Interrupt             */
	PendSV_IRQn                   = -2,		/*!< 14 Cortex-M3 Pend SV Interrupt                   */
	SysTick_IRQn                  = -1,		/*!< 15 Cortex-M3 System Tick Interrupt               */

	/******  LPC13xx Specific Interrupt Numbers *******************************************************/
	WAKEUP0_IRQn                  = 0,		/*!< All I/O pins can be used as wakeup source.       */
	WAKEUP1_IRQn                  = 1,		/*!< There are 40 pins in total for LPC17xx           */
	WAKEUP2_IRQn                  = 2,
	WAKEUP3_IRQn                  = 3,
	WAKEUP4_IRQn                  = 4,
	WAKEUP5_IRQn                  = 5,
	WAKEUP6_IRQn                  = 6,
	WAKEUP7_IRQn                  = 7,
	WAKEUP8_IRQn                  = 8,
	WAKEUP9_IRQn                  = 9,
	WAKEUP10_IRQn                 = 10,
	WAKEUP11_IRQn                 = 11,
	WAKEUP12_IRQn                 = 12,
	WAKEUP13_IRQn                 = 13,
	WAKEUP14_IRQn                 = 14,
	WAKEUP15_IRQn                 = 15,
	WAKEUP16_IRQn                 = 16,
	WAKEUP17_IRQn                 = 17,
	WAKEUP18_IRQn                 = 18,
	WAKEUP19_IRQn                 = 19,
	WAKEUP20_IRQn                 = 20,
	WAKEUP21_IRQn                 = 21,
	WAKEUP22_IRQn                 = 22,
	WAKEUP23_IRQn                 = 23,
	WAKEUP24_IRQn                 = 24,
	WAKEUP25_IRQn                 = 25,
	WAKEUP26_IRQn                 = 26,
	WAKEUP27_IRQn                 = 27,
	WAKEUP28_IRQn                 = 28,
	WAKEUP29_IRQn                 = 29,
	WAKEUP30_IRQn                 = 30,
	WAKEUP31_IRQn                 = 31,
	WAKEUP32_IRQn                 = 32,
	WAKEUP33_IRQn                 = 33,
	WAKEUP34_IRQn                 = 34,
	WAKEUP35_IRQn                 = 35,
	WAKEUP36_IRQn                 = 36,
	WAKEUP37_IRQn                 = 37,
	WAKEUP38_IRQn                 = 38,
	WAKEUP39_IRQn                 = 39,
	I2C0_IRQn                     = 40,		/*!< I2C Interrupt                                    */
	TIMER_16_0_IRQn               = 41,		/*!< 16-bit Timer0 Interrupt                          */
	TIMER_16_1_IRQn               = 42,		/*!< 16-bit Timer1 Interrupt                          */
	TIMER_32_0_IRQn               = 43,		/*!< 32-bit Timer0 Interrupt                          */
	TIMER_32_1_IRQn               = 44,		/*!< 32-bit Timer1 Interrupt                          */
	SSP0_IRQn                     = 45,		/*!< SSP Interrupt                                    */
	UART0_IRQn                    = 46,		/*!< UART Interrupt                                   */
	USB0_IRQn                     = 47,		/*!< USB Regular Interrupt                            */
	USB0_FIQ_IRQn                 = 48,		/*!< USB Fast Interrupt                               */
	ADC_IRQn                      = 49,		/*!< A/D Converter Interrupt                          */
	WDT_IRQn                      = 50,		/*!< Watchdog timer Interrupt                         */
	BOD_IRQn                      = 51,		/*!< Brown Out Detect(BOD) Interrupt                  */
	EINT3_IRQn                    = 53,		/*!< External Interrupt 3 Interrupt                   */
	EINT2_IRQn                    = 54,		/*!< External Interrupt 2 Interrupt                   */
	EINT1_IRQn                    = 55,		/*!< External Interrupt 1 Interrupt                   */
	EINT0_IRQn                    = 56,		/*!< External Interrupt 0 Interrupt                   */
	SSP1_IRQn                     = 57,		/*!< SSP1 Interrupt                                   */
} LPC1343_IRQn_Type;

/**
 * @}
 */

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/** @defgroup CMSIS_1343_COMMON CHIP: Common LPC1343 Cortex CMSIS definitions
 * @{
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
// FIXME - need to check rev number
#define __CM3_REV            	0x0200		/*!< Cortex-M3 Core Revision                          */
#define __MPU_PRESENT             0			/*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          3			/*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0			/*!< Set to 1 if different SysTick Config is used     */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_1343_H_ */
