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
#include "main.h"

#define EXAMPLE_LEDS        ONBOARD_LED_D1, ONBOARD_LED_D2, ONBOARD_LED_D3
#define EXAMPLE_TTY_LEAD    "mcu> "

/* params for led_on/led_of callbacks */
led_param_t led_params[] = {
    { .name = "led1", .led = ONBOARD_LED_D1 },
    { .name = "led2", .led = ONBOARD_LED_D2 },
    { .name = "led3", .led = ONBOARD_LED_D3 },
};

/* forward declaration */
help_param_t help_param;

command_t commands[] = {
    { .name = "led1-on",    .callback = led_on,     .param = &led_params[0], },
    { .name = "led2-on",    .callback = led_on,     .param = &led_params[1], },
    { .name = "led3-on",    .callback = led_on,     .param = &led_params[2], },
    { .name = "led1-off",   .callback = led_off,    .param = &led_params[0], },
    { .name = "led2-off",   .callback = led_off,    .param = &led_params[1], },
    { .name = "led3-off",   .callback = led_off,    .param = &led_params[2], },
    { .name = "leds-on",    .callback = leds_on,    },
    { .name = "leds-off",   .callback = leds_off,   },
    { .name = "help",       .callback = leds_help,  .param = &help_param },
};

/* param for help callback */
help_param_t help_param = {
    .commands = (command_t*)commands,
    .count = sizeof(commands)/sizeof(command_t)
};

/* callback, turn on led specify by 'param' field */
void led_on(command_t *led_command)
{
    led_param_t *led_param = (led_param_t*)led_command->param;
    Board_LED_SetLevel(led_param->led, true);
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD);
    Board_UARTPutSTR(led_param->name);
    Board_UARTPutSTR(" has turned on\n");
}

/* callback, turn off led specify by 'param' field */
void led_off(struct command *led_command)
{
    led_param_t *led_param = (led_param_t*)led_command->param;
    Board_LED_SetLevel(led_param->led, false);
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD);
    Board_UARTPutSTR(led_param->name);
    Board_UARTPutSTR(" has turned off\n");
}

/* callback, turn on all custom leds */
void leds_on(struct command *led_command)
{
    pinmux_t leds[] = { EXAMPLE_LEDS };
    uint32_t count = sizeof(leds)/sizeof(pinmux_t);
    for (int i = 0; i < count; i++)
    {
        Board_LED_SetLevel(leds[i], true);
    }
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD);
    Board_UARTPutSTR("all leds have turned on\n");
}

/* callback, turn off all custom leds */
void leds_off(struct command *led_command)
{
    pinmux_t leds[] = { EXAMPLE_LEDS };
    uint32_t count = sizeof(leds)/sizeof(pinmux_t);
    for (int i = 0; i < count; i++)
    {
        Board_LED_SetLevel(leds[i], false);
    }
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD);
    Board_UARTPutSTR("all leds have turned off\n");
}

/* forward declaration */
void print_help(command_t *commands, uint32_t commands_count);

/* callback, invoke printing help */
void leds_help(struct command *led_command)
{
    help_param_t *param = (help_param_t*)led_command->param;
    print_help(param->commands, param->count);
}

void print_help(command_t *commands, uint32_t commands_count)
{
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD "--------------------------------------------------\n");
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD " App help, available commands are: \n");
    Board_UARTPutSTR(EXAMPLE_TTY_LEAD "--------------------------------------------------\n");
    for (int i = 0; i < commands_count; i++)
    {
        Board_UARTPutSTR(EXAMPLE_TTY_LEAD);
        Board_UARTPutSTR(commands[i].name);
        Board_UARTPutSTR("\n");
    }
}

void process_commands(command_t *commands, uint32_t commands_count)
{
    bool processing;
    int32_t sign;
    while (1)
    {
        processing = true;
        /* set 'command_processing' to all commands */
        for (int command_i = 0; command_i < commands_count; command_i++)
        {
            commands[ command_i ].status = command_processing;
        }
        /* process until one command pass or all are rejected */
        for (int received_i = 0, rejected_count = 0; processing; )
        {
            sign = Board_UARTGetChar();
            /* if is valid char */
            if (sign != EOF)
            {
                /* loop over commands and try to compare names */
                for (int command_i = 0; command_i < commands_count; command_i++)
                {
                    /* already rejected */
                    if (commands[ command_i ].status == command_rejected)
                    {
                        continue;
                    }
                    /* end of the string or line - command name matched */
                    if (commands[ command_i ].name[ received_i ] == '\0' &&
                        (sign == 0xA || sign == 0xD)
                    )
                    {
                        /* call command */
                        commands[ command_i ].callback(&commands[ command_i ]);
                        processing = false;
                        break;
                    }
                    /* received char does not match name - rejected */
                    if (sign != commands[ command_i ].name[ received_i ])
                    {
                        commands[ command_i ].status = command_rejected;
                        rejected_count++;
                        continue;
                    }
                }
                received_i++;
                /* all commands are rejected */
                if (rejected_count >= commands_count)
                {
                    processing = false;
                }
                /* newline pressed - show tty lead */
                if (sign == 0xA)
                {
                    Board_UARTPutSTR("\n" EXAMPLE_TTY_LEAD);
                }
                else
                {
                    Board_UARTPutChar(sign);
                }
            }
        }
    }
}

int main(void)
{
    uint32_t commands_count = sizeof(commands)/sizeof(command_t);

    SystemCoreClockUpdate();
    Board_Init();

    /* wait for first keypress */
    while (Board_UARTGetChar() == EOF);
    Board_UARTPutSTR("Application started\n");
    /* print help */
    print_help(commands, commands_count);
    /* process commands */
    process_commands(commands, commands_count);

    while(1);
    return 0;
}
