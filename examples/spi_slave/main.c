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

#include <string.h>
#include "chip.h"
#include "board.h"

/* Note: BUFFER_SIZE must have the same size as 'spi_mcu_slave' app on IMX */
#define BUFFER_SIZE 4

/*
 * Note: SSP in master mode toogle CS after each transmited word.
 *       In slave mode, it receives only one word per CS active.
 */

int main(void)
{
    Chip_SSP_DATA_SETUP_T transfer;
    char tx_buffer[ BUFFER_SIZE ];
    char rx_buffer[ BUFFER_SIZE ];

    /* fill tx buffer with dummy data that are ignored on master */
    memset(tx_buffer, 0xFF, BUFFER_SIZE);

    SystemCoreClockUpdate();
    Board_Init();

    Chip_SSP_Init(LPC_SSP0);
    Chip_SSP_SetMaster(LPC_SSP0, false);
    // Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
    Chip_SSP_Enable(LPC_SSP0);

    transfer.tx_data = tx_buffer;
    transfer.rx_data = rx_buffer;
    transfer.length = BUFFER_SIZE;

    while (true)
    {
        /* reset indexes */
        transfer.tx_cnt = 0;
        transfer.rx_cnt = 0;
        /* send and receive buffers at the same time */
        Chip_SSP_RWFrames_Blocking(LPC_SSP0, &transfer);
        /* copy rx_buffer to tx_buffer - so in next loop 
         * it transmits received data
         */
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            /* uppercase to lowercase */
            if (rx_buffer[i] >= 0x41 && rx_buffer[i] <= 0x5A)
                tx_buffer[i] = rx_buffer[i] + 0x20;
            /* lowercase to uppercase */
            else if (rx_buffer[i] >= 0x61 && rx_buffer[i] <= 0x7A)
                tx_buffer[i] = rx_buffer[i] - 0x20;
            /* return back everything else */
            else
                tx_buffer[i] = rx_buffer[i];
        }
    }

    while(1);
    return 0;
}
