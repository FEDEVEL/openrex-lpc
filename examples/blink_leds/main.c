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

#include "chip.h"
#include "board.h"

void delay(uint32_t count)
{
    for (;count; count--)
        __asm("nop");
}

int main(void)
{
    pinmux_t leds[] = { ONBOARD_LEDS };
    uint32_t leds_count = sizeof(leds)/(sizeof(pinmux_t));

    SystemCoreClockUpdate();
    Board_Init();

    while (1)
    {
        for (int i = 0; i < leds_count; i++)
        {
            Board_LED_Set(leds[i], true);
            delay(250000);
        }
        for (int i = 0; i < leds_count; i++)
        {
            Board_LED_Set(leds[i], false);
            delay(250000);
        }
    }

    while(1);
    return 0;
}
