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

/*
 * This is the common file shared between MPU(Linux)/MPU(baremetal)
 */

#ifndef __CANVAS_COMMON_H__
#define __CANVAS_COMMON_H__

#include <stdint.h>

/* macro to assemble and dissasemble color */
#define CANVAS_RGBCOLOR(r, g, b)        ((uint32_t)( (((uint8_t)(r)) << 16) | (((uint8_t)(g)) << 8) | (((uint8_t)(b))) ))
#define CANVAS_RGBCOLOR_RED(color)      (((color) >> 16) & 0xFF)
#define CANVAS_RGBCOLOR_GREEN(color)    (((color) >> 8) & 0xFF)
#define CANVAS_RGBCOLOR_BLUE(color)     ((color) & 0xFF)

#define CANVAS_COLOR_WHITE              CANVAS_RGBCOLOR(0xFF, 0xFF, 0xFF)
#define CANVAS_COLOR_BLACK              CANVAS_RGBCOLOR(0x00, 0x00, 0x00)
#define CANVAS_COLOR_RED                CANVAS_RGBCOLOR(0xFF, 0x00, 0x00)
#define CANVAS_COLOR_GREEN              CANVAS_RGBCOLOR(0x00, 0xFF, 0x00)
#define CANVAS_COLOR_BLUE               CANVAS_RGBCOLOR(0x00, 0x00, 0xFF)

/* commands from baremetal to Linux */
#define CANVAS_CMD_CLEAR                (0x01)
#define CANVAS_CMD_GETDIMENSION         (0x02)
#define CANVAS_CMD_RECTANGLE            (0x03)
#define CANVAS_CMD_CIRCLE               (0x04)
#define CANVAS_CMD_GETCOLOR             (0x05)
#define CANVAS_CMD_FLUSH_DRAWING        (0x06)
#define CANVAS_CMD_DUMMY                (0xFF)

/* acknowledge from Linux to baremetal */
#define CANVAS_ACK_DIMENSION            (0x02)
#define CANVAS_ACK_GETCOLOR             (0x05)
#define CANVAS_ACK_DUMMY                (0xFF)

/* NOTE: If some struct member does not have 4B alignment, 
 * use __attribute__((packed)) */

/* clear screen command attributes */
/* yes, I know I could use color without wrapping struct */
struct cmd_clearscreen {
    uint32_t color;
};

/* circle command attributes */
struct cmd_circle {
    int32_t xpos;
    int32_t ypos;
    uint32_t color;
    uint32_t in_centre;
    uint32_t radius;
};

/* rectangle command attributes */
struct cmd_rectangle {
    int32_t xpos;
    int32_t ypos;
    uint32_t color;
    uint32_t in_centre;
    uint32_t width;
    uint32_t height;
};

struct cmd_getcolor {
    int32_t xpos;
    int32_t ypos;
};

/* dimension acknowledge attributes */
struct ack_dimension {
    int32_t width;
    int32_t height;
    uint32_t pixel_bits;
};

/* dimension acknowledge attributes */
struct ack_getcolor {
    uint32_t color;
};

#endif
