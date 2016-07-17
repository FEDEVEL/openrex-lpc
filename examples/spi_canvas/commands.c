#include "chip.h"
#include "board.h"
#include "canvas.h"
#include "commands.h"

void __print_int(uint32_t number, uint32_t divider)
{
    // temprary buffer
    #define MAX_STR 12
    char buffer[ MAX_STR ] = { 0 };
    // check whether divider is sufficient
    // assert((number/divider) < 10);
    // loop over divider and buffer position
    for (uint32_t i = 0;
        (divider != 0) && (i < MAX_STR);
        i++, divider = divider / 10
    )
    {
        buffer[ i ] = '0' + number / divider;
        number = number % divider;
        // current number is zero - reset position
        if (buffer[ i ] == '0') i--;
    }
    Board_UARTPutSTR(buffer);
}

void print_int(uint32_t number)
{
    __print_int(number, 1000000000);
}

int32_t cmd_draw_circle(
    LPC_SSP_T *spi,
    struct canvas_circle *circle
)
{
    uint8_t cmd = CANVAS_CIRCLE_CMD;
    Chip_SSP_Int_Disable(spi);
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, &cmd, sizeof(cmd));
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, (uint8_t*)circle, sizeof(*circle));
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_draw_rectangle(
    LPC_SSP_T *spi,
    struct canvas_rectangle *rectangle
)
{
    uint8_t cmd = CANVAS_RECTANGLE_CMD;
    Chip_SSP_Int_Disable(spi);
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, &cmd, sizeof(cmd));
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, (uint8_t*)rectangle, sizeof(*rectangle));
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_get_dimension(
    LPC_SSP_T *spi,
    struct canvas_dimension *dimension
)
{
    uint8_t cmd = CANVAS_DIMENSION_CMD;
    uint8_t ack;
    Chip_SSP_Int_Disable(spi);
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, &cmd, sizeof(cmd));
    // wait for ACK
    do {
        Chip_SSP_ReadFrames_Blocking(LPC_SSP0, (uint8_t*)&ack, sizeof(ack));
    } while (ack != CANVAS_DIMENSION_ACK);
    // now we get data
    Chip_SSP_ReadFrames_Blocking(LPC_SSP0, (uint8_t*)dimension, sizeof(*dimension));
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_clear_screen(
    LPC_SSP_T *spi,
    uint32_t color
)
{
    uint8_t cmd = CANVAS_CLEAR_CMD;
    Chip_SSP_Int_Disable(spi);
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, &cmd, sizeof(cmd));
    Chip_SSP_WriteFrames_Blocking(LPC_SSP0, (uint8_t*)&color, sizeof(color));
    Chip_SSP_Int_Enable(spi);
    return 0;
}
