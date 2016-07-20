#include "chip.h"
#include "board.h"
#include "canvas_common.h"
#include "commands.h"

// void __print_int(uint32_t number, uint32_t divider)
// {
//     /* temporary buffer */
//     #define MAX_STR 12
//     char buffer[ MAX_STR ] = { 0 };
//     /* check whether divider is sufficient
//      * assert((number/divider) < 10);
//      * loop over divider and buffer position */
//     for (uint32_t i = 0;
//         (divider != 0) && (i < MAX_STR);
//         i++, divider = divider / 10
//     )
//     {
//         buffer[ i ] = '0' + number / divider;
//         number = number % divider;
//         /* current number is zero - reset position */
//         if (buffer[ i ] == '0') i--;
//     }
//     Board_UARTPutSTR(buffer);
// }

// void print_int(uint32_t number)
// {
//     __print_int(number, 1000000000);
// }


void initialize_spi(
    LPC_SSP_T *spi
)
{
    Chip_SSP_Init(spi);
    Chip_SSP_SetMaster(spi, false);
    Chip_SSP_SetFormat(spi, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
    Chip_SSP_Int_Enable(spi);
    Chip_SSP_Enable(spi);
}

/* 'do nothing' actually means:
 * - keep rxFIFO empty for incomming data
 * - keep txFIFO filled with dummy pattern
 * So when master starts SPI transfer, txFIFO send
 * dummy to master and rxFIFO receive dummy from master.
 */
void cmd_do_nothing(
    LPC_SSP_T *spi
)
{
    while (
        (spi->SR & SSP_STAT_RNE) || (spi->SR & SSP_STAT_TNF)
    )
    {
        /* rxFIFO not empty - pop item*/
        if (spi->SR & SSP_STAT_RNE)
            spi->DR;
        /* txFIFO not full - push dummy*/
        if (spi->SR & SSP_STAT_TNF)
            spi->DR = CANVAS_CMD_DUMMY;
    }
}

int32_t cmd_draw_circle(
    LPC_SSP_T *spi,
    struct cmd_circle *circle
)
{
    uint8_t cmd = CANVAS_CMD_CIRCLE;
    /* disable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Disable(spi);
    /* send command */
    Chip_SSP_WriteFrames_Blocking(spi, &cmd, sizeof(cmd));
    /* send circle struct */
    Chip_SSP_WriteFrames_Blocking(spi, (uint8_t*)circle, sizeof(*circle));
    /* enable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_draw_rectangle(
    LPC_SSP_T *spi,
    struct cmd_rectangle *rectangle
)
{
    uint8_t cmd = CANVAS_CMD_RECTANGLE;
    /* disable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Disable(spi);
    /* send command */
    Chip_SSP_WriteFrames_Blocking(spi, &cmd, sizeof(cmd));
    /* send rectangle struct */
    Chip_SSP_WriteFrames_Blocking(spi, (uint8_t*)rectangle, sizeof(*rectangle));
    /* enable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_get_dimension(
    LPC_SSP_T *spi,
    struct ack_dimension *dimension
)
{
    uint8_t cmd = CANVAS_CMD_GETDIMENSION;
    uint8_t ack;
    /* disable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Disable(spi);
    /* send command */
    Chip_SSP_WriteFrames_Blocking(spi, &cmd, sizeof(cmd));
    /* wait for acknowledge */
    do {
        Chip_SSP_ReadFrames_Blocking(spi, (uint8_t*)&ack, sizeof(ack));
    } while (ack != CANVAS_ACK_DIMENSION);
    /* now we can read data */
    Chip_SSP_ReadFrames_Blocking(spi, (uint8_t*)dimension, sizeof(*dimension));
    /* enable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_clear_screen(
    LPC_SSP_T *spi,
    struct cmd_clearscreen *screen
)
{
    uint8_t cmd = CANVAS_CMD_CLEAR;
    /* disable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Disable(spi);
    /* send command */
    Chip_SSP_WriteFrames_Blocking(spi, &cmd, sizeof(cmd));
    /* send data */
    Chip_SSP_WriteFrames_Blocking(spi, (uint8_t*)screen, sizeof(*screen));
    /* enable sending DUMMY pattern to FIFO */
    Chip_SSP_Int_Enable(spi);
    return 0;
}

int32_t cmd_flush_drawing(
    LPC_SSP_T *spi
)
{
    uint8_t cmd = CANVAS_CMD_FLUSH_DRAWING;
    Chip_SSP_WriteFrames_Blocking(spi, &cmd, sizeof(cmd));
    return 0;
}
