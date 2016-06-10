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

#define BUTTONS_BUTTON1_GPIO_PORT_NUM           0
#define BUTTONS_BUTTON1_GPIO_BIT_NUM            16

#define JOYSTICK_UP_GPIO_PORT_NUM               2
#define JOYSTICK_UP_GPIO_BIT_NUM                3
#define JOYSTICK_UP_IOCON_PIO                   IOCON_PIO2_3
#define JOYSTICK_DOWN_GPIO_PORT_NUM             2
#define JOYSTICK_DOWN_GPIO_BIT_NUM              1
#define JOYSTICK_DOWN_IOCON_PIO                 IOCON_PIO2_1
#define JOYSTICK_LEFT_GPIO_PORT_NUM             2
#define JOYSTICK_LEFT_GPIO_BIT_NUM              4
#define JOYSTICK_LEFT_IOCON_PIO                 IOCON_PIO2_4
#define JOYSTICK_RIGHT_GPIO_PORT_NUM            2
#define JOYSTICK_RIGHT_GPIO_BIT_NUM             2
#define JOYSTICK_RIGHT_IOCON_PIO                IOCON_PIO2_2
#define JOYSTICK_PRESS_GPIO_PORT_NUM            2
#define JOYSTICK_PRESS_GPIO_BIT_NUM             0
#define JOYSTICK_PRESS_IOCON_PIO                IOCON_PIO2_0

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
	/* Set the PIO_7 as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, 0, 7, true);
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber == 0) {
		Chip_GPIO_WritePortBit(LPC_GPIO_PORT, 0, 7, On);
	}
}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	bool state = false;

	if (LEDNumber == 0) {
		state = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, 0, 7);
	}

	return state;
}

void Board_LED_Toggle(uint8_t LEDNumber)
{
	Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Initialize LEDs */
	Board_LED_Init();
}

/* Initialize pin muxing for SSP interface */
void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	Chip_IOCON_PinLocSel(LPC_IOCON, IOCON_SCKLOC_PIO2_11);
}

/* Configure pin for ADC channel 0 */
void Board_ADC_Init(void)
{
	/* Muxing already setup as part of SystemInit for AD0 */
}

/* Initialize buttons on the board */
void Board_Buttons_Init(void)
{
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM, false);
}

/* Baseboard joystick buttons */
#define NUM_BUTTONS 5
static const uint8_t portButton[NUM_BUTTONS] = {
	JOYSTICK_PRESS_GPIO_PORT_NUM, 
	JOYSTICK_RIGHT_GPIO_PORT_NUM,
	JOYSTICK_UP_GPIO_PORT_NUM,
	JOYSTICK_DOWN_GPIO_PORT_NUM,
	JOYSTICK_LEFT_GPIO_PORT_NUM
};

static const uint8_t pinButton[NUM_BUTTONS] = {
	JOYSTICK_PRESS_GPIO_BIT_NUM, 
	JOYSTICK_RIGHT_GPIO_BIT_NUM,
	JOYSTICK_UP_GPIO_BIT_NUM,
	JOYSTICK_DOWN_GPIO_BIT_NUM,
	JOYSTICK_LEFT_GPIO_BIT_NUM
};

static const uint8_t stateButton[NUM_BUTTONS] = {
	JOY_PRESS, 
	JOY_RIGHT, 
	JOY_UP,
	JOY_DOWN, 
	JOY_LEFT
};

/* Initialize Joystick */
void Board_Joystick_Init(void)
{
	int ix;

	for (ix = 0; ix < NUM_BUTTONS; ix++) {
		Chip_IOCON_PinMuxSet(LPC_IOCON, (CHIP_IOCON_PIO_T) portButton[ix], (IOCON_FUNC0 | IOCON_MODE_PULLUP));
		Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, portButton[ix], pinButton[ix], false);
	}
}

/* Get Joystick status */
uint8_t Joystick_GetStatus(void)
{
	uint8_t ix, ret = 0;

	for (ix = 0; ix < NUM_BUTTONS; ix++) {
		if ((Chip_GPIO_GetPinState(LPC_GPIO_PORT, portButton[ix], pinButton[ix])) == false) {
			ret |= stateButton[ix];
		}
	}

	return ret;
}

