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


/* NOTE: Keep in mind that 'openrex reset button' resets MPU (Linux) 
 * but *does not reset* LPC MCU. To reset MCU, you need to toogle GPIO in /sysfs/ */

/* NOTE: custom buttons need to be attached:
 *  PLAYER1_BTN to MCU_GPIO0 signal
 *  PLAYER2_BTN to MCU_GPIO1 signal
 *  START_STOP_BTN to MCU_GPIO2 signal
 * pins are configured to use internal pulldown resistor with hysteresis and expect to read level '1' when pressed.
 * depending on particular conditions, you might want to use external pull-down resistor + parallel capacitor.
 * NOTE: MCU_GPIO2 signal has external pulldown resistor R205 (4k9) on board */
#define PLAYER1_BTN         TO_PINMUX(0, 21)
#define CFG_PLAYER1_BTN     (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_HYS_EN | IOCON_RESERVED_BIT_7)
#define PLAYER2_BTN         TO_PINMUX(1, 15)
#define CFG_PLAYER2_BTN     (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_HYS_EN | IOCON_RESERVED_BIT_7)
#define START_STOP_BTN      TO_PINMUX(0, 3)
#define CFG_START_STOP_BTN  (IOCON_FUNC0 | IOCON_MODE_PULLDOWN | IOCON_HYS_EN | IOCON_RESERVED_BIT_7)


/* merge attributes related with control 
 * and drawing ball on screen */
struct ball {
    /* reset speed value */
    int32_t x_speed;
    int32_t y_speed;
    /* add to position in each loop */
    int32_t x_move;
    int32_t y_move;
    /* rectangle under ball in previous drawing */
    struct cmd_rectangle ghost;
    /* ball attributes used in current drawing */
    struct cmd_circle ball;
};

/* merge attributes related with control 
 * and drawing plate on screen */
struct plate {
    /* reset speed value */
    int32_t y_speed;
    /* add to position in each loop */
    int32_t y_move;
    /* button responsible for movement */
    uint32_t ctrl_btn;
    /* rectangle under plate in previous drawing */
    struct cmd_rectangle ghost;
    /* plate attributes used in current drawing */
    struct cmd_rectangle plate;
    /* draw ghost on screen */
    bool ghost_enabled;
    /* internal button state */
    bool btn_active;
};

/* application state machine */
enum states {
    /* game is running. 
     *  - 'hitting the wall' app goes to 'game_over_state'.
     *  - press 'start/stop' app goes to 'reset_state' */
    running_state,
    /* game loads default settings.
     *  - press 'start/stop' btn to run the game (running_state) */
    reset_state,
    /* game is over - ball hits the wall.
     *  - press 'start/stop' btn to reload game (reset_state) */
    game_over_state
};

/* state of ball */
enum ball_status {
    /* ball is in move */
    ball_moves,
    /* ball hits the left wall (player1 looses) */
    ball_hit_left_wall,
    /* ball hits the left wall (player2 looses) */
    ball_hit_right_wall
};

/* merge app mode with 'start/stop' button */
struct app_mode {
    /* 'start/stop' button */
    uint32_t ctrl_btn;
    /* application state machine */
    enum states mode;
    /* internal btn state */
    bool btn_active;
};

/* calculate position of ball depending on 'dimension' and positions of 'players'
 * return the state of ball in current step - runing | hit_left_wall | hit_right_wall */
enum ball_status calculate_ball_position(
    const struct ack_dimension *dimension,
    struct ball *ball,
    const struct plate *player1,
    const struct plate *player2
)
{
    enum ball_status status = ball_moves;
    int32_t player1_tlimit, player1_blimit;
    int32_t player2_tlimit, player2_blimit;

    assert(NULL != dimension);
    assert(NULL != ball);
    assert(NULL != player1);
    assert(NULL != player2);

    /* NOTE: calculation works for *reasonable* size of screen, plates and ball. 
     * Limit conditions (radius bigger than screen, ...) are not checked ! */
    /* NOTE: bouncing from corners does not work according real Physic. */

    ball->ghost.xpos = ball->ball.xpos;
    ball->ghost.ypos = ball->ball.ypos;

    /* bounce from bottom */
    if ((int32_t)(ball->ball.ypos + ball->ball.radius + ball->y_move) > dimension->height)
    {
        ball->y_move = -ball->y_speed;
    }
    /* bounce from top */
    if ((int32_t)(ball->ball.ypos - ball->ball.radius + ball->y_move) < 0)
    {
        ball->y_move = ball->y_speed;
    }

    /* calculate top y-position of plate */
    player1_tlimit = player1->plate.ypos - (player1->plate.height >> 1);
    /* if not at top, add extra space to bounce from corner */
    if (player1_tlimit > ball->ball.radius)
        player1_tlimit -= ball->ball.radius;
    /* calculate bottom y-position of plate */
    player1_blimit = player1->plate.ypos + (player1->plate.height >> 1);
    /* if not at bottom, add extra space to bounce from corner */
    if (player1_blimit + ball->ball.radius < dimension->height)
        player1_blimit += ball->ball.radius;
    /* bounce from player1's plate - left player */
    if (
        (ball->ball.ypos < player1_blimit) && 
        (ball->ball.ypos > player1_tlimit) && 
        ((ball->ball.xpos + ball->x_move - ball->ball.radius) < (player1->plate.xpos + (player1->plate.width >> 1)))
    )
    {
        ball->x_move = ball->x_speed;
        ball->ball.color = player1->plate.color;
    }

    /* calculate top y-position of plate */
    player2_tlimit = player2->plate.ypos - (player2->plate.height >> 1);
    /* if not at top, add extra space to bounce from corner */
    if (player2_tlimit > ball->ball.radius)
        player2_tlimit -= ball->ball.radius;
    /* calculate bottom y-position of plate */
    player2_blimit = player2->plate.ypos + (player2->plate.height >> 1);
    /* if not at bottom, add extra space to bounce from corner */
    if (player2_blimit + ball->ball.radius < dimension->height)
        player2_blimit += ball->ball.radius;
    /* bounce from player2's plate - right player */
    if (
        (ball->ball.ypos < player2_blimit) && 
        (ball->ball.ypos > player2_tlimit) && 
        ((ball->ball.xpos + ball->x_move + ball->ball.radius) > (player2->plate.xpos - (player2->plate.width >> 1)))
    )
    {
        ball->x_move = -ball->x_speed;
        ball->ball.color = player2->plate.color;
    }

    /* hit right limit - game is over */
    if ((int32_t)(ball->ball.xpos + ball->ball.radius + ball->x_move) > dimension->width)
    {
        ball->x_move = 0;
        ball->y_move = 0;
        status = ball_hit_right_wall;
    }

    /* hit left limit - game is over */
    if ((int32_t)(ball->ball.xpos - ball->ball.radius + ball->x_move) < 0)
    {
        ball->x_move = 0;
        ball->y_move = 0;
        status = ball_hit_left_wall;
    }

    /* update ball position. this is the "magic" that moves the ball */
    ball->ball.xpos += ball->x_move;
    ball->ball.ypos += ball->y_move;

    return status;
}

/* calculate position of player's plate to avoid moving outside screeen.
 * draw 'ghost' only in case it's necessary */
void calculate_player_position(
    struct ack_dimension *dimension,
    struct plate *plate
)
{
    assert(NULL != dimension);
    assert(NULL != plate);

    if (plate->y_move == 0)
    {
        plate->ghost_enabled = 0;
    }
    else if (
        ((plate->plate.ypos + (plate->plate.height >> 1) + plate->y_move) < dimension->height) && 
        ((plate->plate.ypos - (plate->plate.height >> 1) + plate->y_move) > 0)
    )
    {
        /* store old plate to ghost */
        plate->ghost.xpos = plate->plate.xpos;
        plate->ghost.ypos = plate->plate.ypos;
        plate->ghost_enabled = 1;
        /* new position */
        plate->plate.ypos += plate->y_move;
    }
}

/* perform actions based on states of player's button */
void evaluate_player_buttons(
    struct plate *player1,
    struct plate *player2
)
{
    assert(NULL != player1);
    assert(NULL != player2);

    /* button pressed - activate movement */
    if (Board_BTN_GetLevel(player1->ctrl_btn))
    {
        player1->y_move = player1->y_speed;
        player1->btn_active = 1;
    }
    else
    {
        /* released after press - switch direction */
        if (player1->btn_active)
        {
            player1->y_speed = -player1->y_speed;
        }
        player1->y_move = 0;
        player1->btn_active = 0;
    }

    /* button pressed - activate movement */
    if (Board_BTN_GetLevel(player2->ctrl_btn))
    {
        player2->y_move = player2->y_speed;
        player2->btn_active = 1;
    }
    else
    {
        /* released after press - switch direction */
        if (player2->btn_active)
        {
            player2->y_speed = -player2->y_speed;
        }
        player2->y_move = 0;
        player2->btn_active = 0;
    }
}

/* evaluate 'start/stop' button and update application state machine */
void evaluate_control_button(struct app_mode *app_mode)
{
    assert(NULL != app_mode);

    /* button pressed - change mode */
    if (Board_BTN_GetLevel(app_mode->ctrl_btn))
    {
        if (app_mode->btn_active == 0)
        {
            if (app_mode->mode == reset_state)
                app_mode->mode = running_state;
            else
                app_mode->mode = reset_state;
            app_mode->btn_active = 1;
        }
    }
    else
    {
        app_mode->btn_active = 0;
    }
}

/* reload default attributes of all graphic objects. usefull for 'reset_mode' */
void reset_objects(
    struct ack_dimension *dimension,
    struct ball *ball,
    struct plate *player1,
    struct plate *player2
)
{
    assert(NULL != dimension);
    assert(NULL != ball);
    assert(NULL != player1);
    assert(NULL != player2);

    /* ball attributes */
    ball->y_speed = 4;
    ball->x_speed = 9;
    ball->x_move = ball->x_speed;
    ball->y_move = ball->y_speed;
    ball->ball.color = CANVAS_COLOR_WHITE;
    ball->ball.radius = 20;
    ball->ball.in_centre = 1;
    ball->ghost.color = CANVAS_COLOR_BLACK;
    ball->ghost.width = ball->ball.radius << 1;
    ball->ghost.height = ball->ghost.width;
    ball->ghost.in_centre = ball->ball.in_centre;
    /* player 1 attributes */
    player1->ctrl_btn = PLAYER1_BTN;
    player1->y_speed = 4;
    player1->plate.width = 10;
    player1->plate.height = 200;
    player1->plate.in_centre = 1;
    player1->plate.color = CANVAS_COLOR_BLUE;
    player1->ghost.width = player1->plate.width;
    player1->ghost.height = player1->plate.height;
    player1->ghost.in_centre = player1->plate.in_centre;
    player1->ghost.color = CANVAS_COLOR_BLACK;
    /* player 2 attributes */
    player2->ctrl_btn = PLAYER2_BTN;
    player2->y_speed = 4;
    player2->plate.width = 10;
    player2->plate.height = 200;
    player2->plate.in_centre = 1;
    player2->plate.color = CANVAS_COLOR_GREEN;
    player2->ghost.width = player2->plate.width;
    player2->ghost.height = player2->plate.height;
    player2->ghost.in_centre = player2->plate.in_centre;
    player2->ghost.color = CANVAS_COLOR_BLACK;
    /* init position of ball - depends on screen dimension */
    ball->ball.xpos = dimension->width >> 1;
    ball->ball.ypos = dimension->height >> 1;
    ball->ghost.xpos = ball->ball.xpos;
    ball->ghost.ypos = ball->ball.ypos;
    /* init position of players - depends on screen dimension */
    player1->plate.xpos = player1->plate.width >> 1;
    player1->plate.ypos = dimension->height >> 1;
    player2->plate.xpos = dimension->width - (player2->plate.width >> 1);
    player2->plate.ypos = dimension->height >> 1;
}

/* call drawing functions on objects */
void draw_objects(
    LPC_SSP_T *spi,
    struct ack_dimension *dimension,
    struct ball *ball,
    struct plate *player1,
    struct plate *player2
)
{
    assert(NULL != spi);
    assert(NULL != dimension);
    assert(NULL != ball);
    assert(NULL != player1);
    assert(NULL != player2);

    /* clear area where ball has been previously placed */
    cmd_draw_rectangle(spi, &ball->ghost);
    /* draw new ball on new position */
    cmd_draw_circle(spi, &ball->ball);

    /* draw player 1 */
    if (player1->ghost_enabled)
        cmd_draw_rectangle(spi, &player1->ghost);
    cmd_draw_rectangle(spi, &player1->plate);
    /* draw player 2 */
    if (player2->ghost_enabled)
        cmd_draw_rectangle(spi, &player2->ghost);
    cmd_draw_rectangle(spi, &player2->plate);
}

/* main application loop */
void draw_movement(LPC_SSP_T *spi)
{
    assert(NULL != spi);

    struct ack_dimension dimension = {0};
    struct ball ball = {0};
    struct plate player1 = {0};
    struct plate player2 = {0};
    struct app_mode app_mode = {
        .ctrl_btn = START_STOP_BTN,
        .mode = reset_state,
        .btn_active = false,
    };
    struct cmd_clearscreen background = {
        .color = CANVAS_COLOR_BLACK
    };
    enum ball_status ball_status;

    /* get screen dimension */
    cmd_get_dimension(spi, &dimension);
    /* set default value of objects */
    reset_objects(&dimension, &ball, &player1, &player2);

    /* clear screen for first time */
    cmd_clear_screen(spi, &background);

    /* 'circle.radius' must be smaller than
     * 'dimension.width' and 'dimension.height' */
    while (1)
    {
        evaluate_control_button(&app_mode);
        if (app_mode.mode == reset_state)
        {
            reset_objects(&dimension, &ball, &player1, &player2);
            cmd_clear_screen(spi, &background);
            cmd_flush_drawing(spi);
        }
        else if (app_mode.mode == game_over_state)
        {
            struct cmd_clearscreen looser_background = { 0 };
            looser_background.color = (
                ball_hit_right_wall == ball_status ?
                player2.plate.color : player1.plate.color
            );
            /* SPI transfer is slow enough to make it
             * blink without additional delay */
            cmd_clear_screen(spi, &looser_background);
            cmd_flush_drawing(spi);
            /* original background */
            cmd_clear_screen(spi, &background);
            cmd_flush_drawing(spi);
        }
        else if (app_mode.mode == running_state)
        {
            evaluate_player_buttons(&player1, &player2);
            /* calculate player 1 position */
            calculate_player_position(&dimension, &player1);
            /* calculate player 2 position */
            calculate_player_position(&dimension, &player2);
            /* calculate ball position */
            ball_status = calculate_ball_position(&dimension, &ball, &player1, &player2);
            /* ball is not in movement - game is over */
            if (ball_status != ball_moves)
            {
                app_mode.mode = game_over_state;       
            }
            /* draw objects */
            draw_objects(spi, &dimension, &ball, &player1, &player2);
            /* put image on screen */
            cmd_flush_drawing(spi);
        }
    }
}

/* SPI interrupt occours in case txFIFO is half empty
 * which means that our application doesn't have any data
 * to send. So we have to keep bus idle by sending DUMMY */
void SSP0_IRQHandler(void)
{
    cmd_do_nothing(LPC_SSP0);
}

int main(void)
{
    SystemCoreClockUpdate();

    /* Initialize IOCON, GPIO, ... */
    Board_Init();

    /* - set pin to be controlled by GPIO
     * - set GPIO for input */
    /* player 1 btn */
    Chip_IOCON_PinMuxSet(LPC_IOCON, TO_PORT(PLAYER1_BTN), TO_PIN(PLAYER1_BTN), CFG_PLAYER1_BTN);
    Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TO_PORT(PLAYER1_BTN), TO_PIN(PLAYER1_BTN));

    /* player 2 btn */
    Chip_IOCON_PinMuxSet(LPC_IOCON, TO_PORT(PLAYER2_BTN), TO_PIN(PLAYER2_BTN), CFG_PLAYER2_BTN);
    Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TO_PORT(PLAYER2_BTN), TO_PIN(PLAYER2_BTN));

    /* start/stop btn */
    Chip_IOCON_PinMuxSet(LPC_IOCON, TO_PORT(START_STOP_BTN), TO_PIN(START_STOP_BTN), CFG_START_STOP_BTN);
    Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TO_PORT(START_STOP_BTN), TO_PIN(START_STOP_BTN));

    /* initialize SPI */
    initialize_spi(LPC_SSP0);
    /* enable SPI interrupts */
    NVIC_EnableIRQ(SSP0_IRQn);

    /* start main loop */
    draw_movement(LPC_SSP0);

    while(1);
    return 0;
}
