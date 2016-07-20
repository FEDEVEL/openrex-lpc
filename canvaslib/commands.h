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
