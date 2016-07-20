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
#include "assert.h"
#include "chip.h"
#include "board.h"
#include "canvas_common.h"
#include "commands.h"

void draw_movement(LPC_SSP_T *spi)
{
    struct ack_dimension dimension = {
        .width = 0,
        .height = 0,
    };
    struct cmd_circle circle = {
        .xpos       = 0,
        .ypos       = 0,
        .color      = CANVAS_COLOR_BLACK,
        .radius     = 50,
        .in_centre  = 1,
    };
    int32_t x_speed = 3, y_speed = 1, color_rotation = 0;
    int32_t x_move = x_speed, y_move = y_speed, color_move = 1;

    assert(NULL != spi);

    /* get screen dimension */
    cmd_get_dimension(spi, &dimension);

    /* 'circle.radius' must be smaller than
     * 'dimension.width' and 'dimension.height' */
    while (1)
    {
        /* bottom limit */
        if ((int32_t)(circle.ypos + circle.radius + y_move) > dimension.height)
        {
            y_move = -y_speed;
        }
        /* upper limit */
        if ((int32_t)(circle.ypos - circle.radius + y_move) < 0)
        {
            y_move = y_speed;
        }
        /* right limit */
        if ((int32_t)(circle.xpos + circle.radius + x_move) > dimension.width)
        {
            x_move = -x_speed;
        }
        /* left limit */
        if ((int32_t)(circle.xpos - circle.radius + x_move) < 0)
        {
            x_move = x_speed;
        }

        /* update color. Each 'base color' starts and ends at 0 value */
        if (((circle.color >> color_rotation) & 0xFF) + color_move == 0xFF)
        {
            color_move = -1;
        }
        /* hit the lower limit - move to next 'base color' */
        if (((circle.color >> color_rotation) & 0xFF) + color_move == 0)
        {
            color_move = 1;
            color_rotation += 8;
        }
        /* color rotation is invalid - reset back to 0 */
        if (color_rotation == 24)
        {
            color_rotation = 0;
        }

        /* update circle attributes */
        circle.xpos += x_move;
        circle.ypos += y_move;
        circle.color += color_move << color_rotation;

        /* send command to draw a circle */
        cmd_draw_circle(spi, &circle);
        /* put image on screen */
        cmd_flush_drawing(spi);
    }
}

/* SPI interrupt occours in case txFIFO is half empty
 * which means that our application doesn't have any data
 * to send so we have to keep bus idle by sending DUMMY */
void SSP0_IRQHandler(void)
{
    cmd_do_nothing(LPC_SSP0);
}

int main(void)
{

    SystemCoreClockUpdate();
    /* Initialize IOCON, GPIO, ... */
    Board_Init();
    /* initialize SPI */
    initialize_spi(LPC_SSP0);
    /* enable SPI interrupts */
    NVIC_EnableIRQ(SSP0_IRQn);
    /* run main loop */
    draw_movement(LPC_SSP0);

    while(1);
    return 0;
}
