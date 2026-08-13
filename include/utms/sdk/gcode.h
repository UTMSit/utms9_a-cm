#ifndef UTMS_GCODE_H
#define UTMS_GCODE_H

#include <stdint.h>

typedef struct {
    uint8_t cmd;
    float x;
    float y;
    float z;
    float e;
    float f;
    uint8_t has_x;
    uint8_t has_y;
    uint8_t has_z;
    uint8_t has_e;
    uint8_t has_f;
} gcode_cmd_t;

int gcode_parse(const char *line, gcode_cmd_t *cmd);

#endif
