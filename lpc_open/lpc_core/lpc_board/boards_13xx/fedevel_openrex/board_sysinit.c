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
#if (defined(ONBOARD_LED_D1) && defined(ONBOARDCF_LED_D1))
    {TO_PORT(ONBOARD_LED_D1), TO_PIN(ONBOARD_LED_D1), ONBOARDCF_LED_D1},
#endif
#if (defined(ONBOARD_LED_D2) && defined(ONBOARDCF_LED_D2))
    {TO_PORT(ONBOARD_LED_D2), TO_PIN(ONBOARD_LED_D2), ONBOARDCF_LED_D2},
#endif
#if (defined(ONBOARD_LED_D3) && defined(ONBOARDCF_LED_D3))
    {TO_PORT(ONBOARD_LED_D3), TO_PIN(ONBOARD_LED_D3), ONBOARDCF_LED_D3},
#endif
#if (defined(ONBOARD_LED_D4) && defined(ONBOARDCF_LED_D4))
    {TO_PORT(ONBOARD_LED_D4), TO_PIN(ONBOARD_LED_D4), ONBOARDCF_LED_D4},
#endif
#if 0 && (defined(ONBOARD_LED_D24) && defined(ONBOARDCF_LED_D24))
    {TO_PORT(ONBOARD_LED_D24), TO_PIN(ONBOARD_LED_D24), ONBOARDCF_LED_D24},
#endif
#if (defined(ONBOARD_SPI0_CS) && defined(ONBOARDCF_SPI0_CS))
    {TO_PORT(ONBOARD_SPI0_CS), TO_PIN(ONBOARD_SPI0_CS), ONBOARDCF_SPI0_CS},
#endif
#if (defined(ONBOARD_SPI0_MISO) && defined(ONBOARDCF_SPI0_MISO))
    {TO_PORT(ONBOARD_SPI0_MISO), TO_PIN(ONBOARD_SPI0_MISO), ONBOARDCF_SPI0_MISO},
#endif
#if (defined(ONBOARD_SPI0_MOSI) && defined(ONBOARDCF_SPI0_MOSI))
    {TO_PORT(ONBOARD_SPI0_MOSI), TO_PIN(ONBOARD_SPI0_MOSI), ONBOARDCF_SPI0_MOSI},
#endif
#if (defined(ONBOARD_SPI0_SCK) && defined(ONBOARDCF_SPI0_SCK))
    {TO_PORT(ONBOARD_SPI0_SCK), TO_PIN(ONBOARD_SPI0_SCK), ONBOARDCF_SPI0_SCK},
#endif
#if (defined(ONBOARD_UART_RX) && defined(ONBOARDCF_UART_RX))
    {TO_PORT(ONBOARD_UART_RX), TO_PIN(ONBOARD_UART_RX), ONBOARDCF_UART_RX},
#endif
#if (defined(ONBOARD_UART_TX) && defined(ONBOARDCF_UART_TX))
    {TO_PORT(ONBOARD_UART_TX), TO_PIN(ONBOARD_UART_TX), ONBOARDCF_UART_TX},
#endif
#if (defined(ONBOARD_BTN_SW2) && defined(ONBOARDCF_BTN_SW2))
    {TO_PORT(ONBOARD_BTN_SW2), TO_PIN(ONBOARD_BTN_SW2), ONBOARDCF_BTN_SW2},
#endif
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
    //Chip_SYSCTL_Map(REMAP_USER_FLASH_MODE);

    /* Setup system clocking and muxing */
    Board_SetupMuxing();
    Board_SetupClocking();
}
