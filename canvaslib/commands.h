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

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void initialize_spi(
    LPC_SSP_T *spi
);

void cmd_do_nothing(
    LPC_SSP_T *spi
);

int32_t cmd_draw_circle(
    LPC_SSP_T *spi,
    struct cmd_circle *circle
);

int32_t cmd_draw_rectangle(
    LPC_SSP_T *spi,
    struct cmd_rectangle *rectangle
);

int32_t cmd_get_dimension(
    LPC_SSP_T *spi,
    struct ack_dimension *dimension
);

int32_t cmd_clear_screen(
    LPC_SSP_T *spi,
    struct cmd_clearscreen *screen
);

int32_t cmd_flush_drawing(
    LPC_SSP_T *spi
);

#endif
