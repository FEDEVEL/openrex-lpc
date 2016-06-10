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

int main(void)
{
    uint8_t buffer[] = { 1, 2, 3, 4 };
    volatile int i = 1;

    SystemCoreClockUpdate();
    Board_Init();

    Chip_SSP_Init(LPC_SSP0);
    Chip_SSP_SetMaster(LPC_SSP0, false);
    // Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
    Chip_SSP_Enable(LPC_SSP0);

    for (int j = 0; i; j++)
    {
        Chip_SSP_WriteFrames_Blocking(LPC_SSP0, buffer, sizeof(buffer));
        Board_LED_Set(ONBOARD_LED_D1, j % 2);
    }

    while(1);
    return 0;
}
