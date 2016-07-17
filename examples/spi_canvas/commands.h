#ifndef __COMMANDS_H__
#define __COMMANDS_H__

int32_t cmd_draw_circle(
    LPC_SSP_T *spi,
    struct canvas_circle *circle
);

int32_t cmd_draw_rectangle(
    LPC_SSP_T *spi,
    struct canvas_rectangle *rectangle
);

int32_t cmd_get_dimension(
    LPC_SSP_T *spi,
    struct canvas_dimension *dimension
);

int32_t cmd_clear_screen(
    LPC_SSP_T *spi,
    uint32_t color
);

#endif
