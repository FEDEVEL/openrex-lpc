/*
 * @brief LPCXpresso 1343 Sysinit file
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

#include "board.h"
#include "string.h"

/* The System initialization code is called prior to the application and
   initializes the board for run-time operation. Board initialization
   includes clock setup and default pin muxing configuration. */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* Pin muxing table, only items that need changing from their default pin
   state are in this table. */
STATIC const PINMUX_GRP_T pinmuxing[] = {
	{(uint32_t)IOCON_PIO0_1,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO0_1 used for CLKOUT */
	{(uint32_t)IOCON_PIO0_2,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_PULLUP)},/* PIO0_2 used for SSEL */
	{(uint32_t)IOCON_PIO0_3,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO0_3 used for USB_VBUS */
	{(uint32_t)IOCON_PIO0_4,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_FASTI2C_EN)}, /* PIO0_4 used for SCL */
	{(uint32_t)IOCON_PIO0_5,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_FASTI2C_EN)}, /* PIO0_5 used for SDA */
	{(uint32_t)IOCON_PIO0_6,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO0_6 used for USB_CONNECT */
	{(uint32_t)IOCON_PIO0_8,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO0_8 used for MISO */
	{(uint32_t)IOCON_PIO0_9,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO0_9 used for MOSI */
	{(uint32_t)IOCON_PIO0_11, (IOCON_FUNC2 | IOCON_RESERVED_BIT_6 | IOCON_ADMODE_EN      | IOCON_FILT_DIS)},   /* PIO0_11 used for AD0 */
	{(uint32_t)IOCON_PIO1_6,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO1_6 used for RXD */
	{(uint32_t)IOCON_PIO1_7,  (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO1_7 used for TXD */
	{(uint32_t)IOCON_PIO2_11, (IOCON_FUNC1 | IOCON_RESERVED_BIT_6 | IOCON_RESERVED_BIT_7 | IOCON_MODE_INACT)}, /* PIO2_11 used for SCK */
};

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Sets up system pin muxing */
void Board_SetupMuxing(void)
{
	/* Enable IOCON clock */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);

	Chip_IOCON_SetPinMuxing(LPC_IOCON, pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));
}

/* Set up and initialize clocking prior to call to main */
void Board_SetupClocking(void)
{
	Chip_SetupXtalClocking();
}

/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
	/* Booting from FLASH, so remap vector table to FLASH */
	Chip_SYSCTL_Map(REMAP_USER_FLASH_MODE);

	/* Setup system clocking and muxing */
	Board_SetupMuxing();
	Board_SetupClocking();
}
