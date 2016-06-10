/*
 * @brief NXP LPCXpresso 1343 board file
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

#ifndef __BOARD_H_
#define __BOARD_H_

#include "chip.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BOARD_NXP_LPCXPRESSO_1343 NXP LPC1343 LPCXpresso board support functions
 * @ingroup LPCOPEN_13XX_BOARD_LPCXPRESSO_1343
 * @{
 */

/** @defgroup BOARD_NXP_LPCXPRESSO_1343_OPTIONS BOARD: NXP LPC1343 LPCXpresso board build options
 * This board has options that configure its operation at build-time.<br>
 * @{
 */
 
 /** Define DEBUG_ENABLE to enable IO via the DEBUGSTR, DEBUGOUT, and
    DEBUGIN macros. If not defined, DEBUG* functions will be optimized
    out of the code at build time.
 */
//#define DEBUG_ENABLE

/** Define DEBUG_SEMIHOSTING along with DEBUG_ENABLE to enable IO support
    via semihosting. You may need to use a C library that supports
    semihosting with this option.
 */
//#define DEBUG_SEMIHOSTING

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
    Board_UARTPutSTR functions.
 */
#define DEBUG_UART 	LPC_USART
#define EOF 		(-1)

/**
 * @}
 */
 
 /* Board name */
#define BOARD_FEDEVEL_OPENREX


/**
 * LED defines
 */
#define TO_PINMUX(port, pin)    (((port & 0x1) << 0x8) | (pin & 0x1F))
#define TO_PIN(pinmux)          (pinmux & 0x1F)
#define TO_PORT(pinmux)         ((pinmux >> 0x8) & 0x1)

#define ONBOARD_LED_D1          (TO_PINMUX(0, 17))
#define ONBOARDCF_LED_D1        (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_RESERVED_BIT_7)
#define ONBOARD_LED_D2          (TO_PINMUX(0, 6))
#define ONBOARDCF_LED_D2        (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_RESERVED_BIT_7)
#define ONBOARD_LED_D3          (TO_PINMUX(0, 20))
#define ONBOARDCF_LED_D3        (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_RESERVED_BIT_7)
/* LED 4 is not connected by default */
// #define ONBOARD_LED_D4          (TO_PINMUX(1, 19))
// #define ONBOARDCF_LED_D4        (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_RESERVED_BIT_7)
#define ONBOARD_LED_D24         (TO_PINMUX(0, 1))
#define ONBOARDCF_LED_D24       (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_RESERVED_BIT_7)

// #define ONBOARD_LEDS            ONBOARD_LED_D1, ONBOARD_LED_D2, ONBOARD_LED_D3, ONBOARD_LED_D4, ONBOARD_LED_D24
#define ONBOARD_LEDS            ONBOARD_LED_D1, ONBOARD_LED_D2, ONBOARD_LED_D3, ONBOARD_LED_D24

#define ONBOARD_SPI0_CS         (TO_PINMUX(0, 2))
#define ONBOARDCF_SPI0_CS       (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_RESERVED_BIT_7)
#define ONBOARD_SPI0_MISO       (TO_PINMUX(0, 8))
#define ONBOARDCF_SPI0_MISO     (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_RESERVED_BIT_7)
#define ONBOARD_SPI0_MOSI       (TO_PINMUX(0, 9))
#define ONBOARDCF_SPI0_MOSI     (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_RESERVED_BIT_7)
#define ONBOARD_SPI0_SCK        (TO_PINMUX(0, 10))
#define ONBOARDCF_SPI0_SCK      (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_RESERVED_BIT_7)

#define ONBOARD_UART_RX         (TO_PINMUX(0, 18))
#define ONBOARDCF_UART_RX       (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_RESERVED_BIT_7)
#define ONBOARD_UART_TX         (TO_PINMUX(0, 19))
#define ONBOARDCF_UART_TX       (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_RESERVED_BIT_7)


/**
 * @brief   Sets up board specific ADC interface
 * @return  Nothing
 */
// void Board_ADC_Init(void);

/**
 * @brief   Initialize pin muxing for SSP interface
 * @param   pSSP    : Pointer to SSP interface to initialize
 * @return  Nothing
 */
// void Board_SSP_Init(LPC_SSP_T *pSSP);



/**
 * @brief   Sets up board specific ADC interface
 * @return  Nothing
 */
// void Board_ADC_Init(void);

/**
 * @brief   Initialize pin muxing for SSP interface
 * @param   pSSP    : Pointer to SSP interface to initialize
 * @return  Nothing
 */
// void Board_SSP_Init(LPC_SSP_T *pSSP);


/**
 * @}
 */

#include "board_api.h"

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
