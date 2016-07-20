/*
 * @brief LPCXpresso 1343 board file
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
#include "retarget.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* System oscillator rate and clock rate on the CLKIN pin */
const uint32_t OscRateIn = 12000000;
const uint32_t ExtRateIn = 0;

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize pin muxing for UART interface */
void Board_UART_Init(void)
{
    /* Pin Muxing has already been done during SystemInit */
}

/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_ENABLE)
    /* Wait for space in FIFO */
    while ((Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE) == 0) {}
    Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_ENABLE)
    if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
        return (int) Chip_UART_ReadByte(DEBUG_UART);
    }
#endif
 return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(char *str)
{
#if defined(DEBUG_ENABLE)
    while (*str != '\0') {
        Board_UARTPutChar(*str++);
    }
#endif
}

/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_ENABLE)
    Board_UART_Init();
    /* Setup UART for 115.2K8N1 */
    Chip_UART_Init(LPC_USART);
    Chip_UART_SetBaud(LPC_USART, 115200);
    Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
    Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
    Chip_UART_TXEnable(LPC_USART);
#endif
}

/* Initializes board LED(s) */
static void Board_LED_Init(void)
{
    pinmux_t leds[] = { ONBOARD_LEDS };
    Chip_GPIO_Init(LPC_GPIO_PORT);
    for (int i = 0; i < (sizeof(leds)/sizeof(leds[0])); i++)
    {
        Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, TO_PORT(leds[i]), TO_PIN(leds[i]));
    }
}

/* Initializes board Button(s) */
static void Board_BTN_Init(void)
{
    pinmux_t buttons[] = { ONBOARD_BTNS };
    Chip_GPIO_Init(LPC_GPIO_PORT);
    for (int i = 0; i < (sizeof(buttons)/sizeof(buttons[0])); i++)
    {
        Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TO_PORT(buttons[i]), TO_PIN(buttons[i]));
    }
}

/* Set the GPIO level of board LED */
void Board_LED_SetLevel(pinmux_t pinmux, bool high)
{
    Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, TO_PORT(pinmux), TO_PIN(pinmux), high);
}

/* Get the GPIO level of board LED */
bool Board_LED_GetLevel(pinmux_t pinmux)
{
    return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TO_PORT(pinmux), TO_PIN(pinmux));
}

/* Get the GPIO level of board Button */
bool Board_BTN_GetLevel(pinmux_t pinmux)
{
    return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TO_PORT(pinmux), TO_PIN(pinmux));
}

void BOARD_assert(const char *file, const int line)
{
    /* NOTE: If LED 24 (the one next to power supply connector) blinks, 
     * it means that your application has some serious issues */

    /* IOCON and GPIO doesn't have to be enabled at this point */
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);
    Chip_GPIO_Init(LPC_GPIO_PORT);
    /* set mux and output direction */
    Chip_IOCON_PinMuxSet(LPC_IOCON, TO_PORT(ONBOARD_ASSERT_LED), TO_PIN(ONBOARD_ASSERT_LED), ONBOARDCFG_ASSERT_LED);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, TO_PORT(ONBOARD_ASSERT_LED), TO_PIN(ONBOARD_ASSERT_LED));
    /* blink forever. the speed depends on core clock settings */
    for (volatile int i = 1; i; )
    {
        Board_LED_SetLevel(ONBOARD_ASSERT_LED, true);
        for (volatile int k = 500000; k; k--) __asm("nop");
        Board_LED_SetLevel(ONBOARD_ASSERT_LED, false);
        for (volatile int k = 500000; k; k--) __asm("nop");
    }
}

/* Set up common peripherals */
void Board_Init(void)
{
    /* Sets up DEBUG UART */
    DEBUGINIT();

    /* Initialize GPIO */
    Chip_GPIO_Init(LPC_GPIO_PORT);

    /* Initialize LEDs */
    Board_LED_Init();

    /* Initialize BTNs */
    Board_BTN_Init();
}
