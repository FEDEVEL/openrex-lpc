#ifndef __CANVAS_H__
#define __CANVAS_H__


#define CANVAS_RGBCOLOR(r, g, b)           (uint32_t)( (((uint8_t)r) << 16) | (((uint8_t)g) << 8) | (((uint8_t)b)))
#define CANVAS_RGBCOLOR_TO_RED(color)      ((color >> 16) & 0xFF)
#define CANVAS_RGBCOLOR_TO_GREEN(color)    ((color >> 8) & 0xFF)
#define CANVAS_RGBCOLOR_TO_BLUE(color)     (color & 0xFF)

#define CANVAS_WHITE_COLOR     CANVAS_RGBCOLOR(0xFF, 0xFF, 0xFF)
#define CANVAS_BLACK_COLOR     CANVAS_RGBCOLOR(0x00, 0x00, 0x00)
#define CANVAS_RED_COLOR       CANVAS_RGBCOLOR(0xFF, 0x00, 0x00)
#define CANVAS_GREEN_COLOR     CANVAS_RGBCOLOR(0x00, 0xFF, 0x00)
#define CANVAS_BLUE_COLOR      CANVAS_RGBCOLOR(0x00, 0x00, 0xFF)

#define CANVAS_CLEAR_CMD        0x01
#define CANVAS_DIMENSION_CMD    0x02
#define CANVAS_RECTANGLE_CMD    0x03
#define CANVAS_CIRCLE_CMD       0x04
#define CANVAS_NOCOMMAND_CMD    0xFF

#define CANVAS_DIMENSION_ACK    0x02

struct canvas_circle {
    int32_t xpos;
    int32_t ypos;
    uint32_t color;
    uint32_t in_centre;
    uint32_t radius;
};

struct canvas_rectangle {
    int32_t xpos;
    int32_t ypos;
    uint32_t color;
    uint32_t in_centre;
    uint32_t width;
    uint32_t height;
};

struct canvas_dimension {
    int32_t width;
    int32_t height;
};

#endif
