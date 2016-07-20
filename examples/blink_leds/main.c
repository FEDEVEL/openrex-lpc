/**
 *  Copyright 2016 
 *  Marian Cingel - cingel.marian@gmail.com
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

#include "assert.h"
#include "chip.h"
#include "board.h"

void delay(volatile uint32_t count)
{
    for (;count; count--)
        __asm("nop");
}

int main(void)
{
    int32_t state = 0;
    bool btn_active = true;
    pinmux_t leds[] = { ONBOARD_LEDS };
    uint32_t leds_count = sizeof(leds)/(sizeof(pinmux_t));

    SystemCoreClockUpdate();
    Board_Init();

    while (1)
    {
        /* Check button state. SW2 is active/pressed as '0' level.
         * You have to press button little bit longer - due delay functions. */
        if (!Board_BTN_GetLevel(ONBOARD_BTN_SW2))
        {
            /* use button 'falling edge' as event to switch state */
            if (btn_active == 0)
            {
                state = state == 0 ? 1 : 0;
            }
            btn_active = 1;
        }
        else
        {
            btn_active = 0;
        }

        /* LED blinking */
        if (0 == state)
        {
            for (int i = 0; i < leds_count; i++)
            {
                Board_LED_SetLevel(leds[i], true);
                delay(250000);
            }
            for (int i = 0; i < leds_count; i++)
            {
                Board_LED_SetLevel(leds[i], false);
                delay(250000);
            }
        }
        else if (1 == state)
        {
            for (int i = 0; i < leds_count; i++)
            {
                /* turn off all leds */
                for (int j = 0; j < leds_count; j++)
                {
                    Board_LED_SetLevel(leds[j], false);
                }
                /* enable one */
                Board_LED_SetLevel(leds[i], true);
                delay(750000);
            }
        }
    }

    while(1);
    return 0;
}
