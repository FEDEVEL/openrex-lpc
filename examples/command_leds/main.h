#ifndef __MAIN_H__
#define __MAIN_H__

/* state of command */
typedef enum {
    command_rejected = 0,
    command_processing,
} commad_status_t;

/* forward declaration */
struct command;
typedef struct command command_t;

/* callback function */
typedef void (*callback_t)(struct command *led_command);

/* led command struct */
typedef struct command {
    char            *name;
    callback_t      callback;
    void            *param;
    commad_status_t status;
} command_t;

/* led param for on/off functions */
typedef struct led_param {
    char        *name;
    pinmux_t    led;
} led_param_t;

/* led param for on/off functions */
typedef struct help_param {
    command_t   *commands;
    uint32_t    count;
} help_param_t;

void led_on(command_t *led_command);
void led_off(struct command *led_command);
void leds_on(struct command *led_command);
void leds_off(struct command *led_command);
void print_help(command_t *commands, uint32_t commands_count);
void leds_help(struct command *led_command);
void print_help(command_t *commands, uint32_t commands_count);
void process_commands(command_t *commands, uint32_t commands_count);

#endif
