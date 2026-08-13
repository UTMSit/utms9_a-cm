#include <utms/sdk/gcode.h>

static float parse_float(const char **p) {
    float val = 0;
    float frac = 0;
    float div = 1;
    int sign = 1;

    if (**p == '-') {
        sign = -1;
        (*p)++;
    }

    while (**p >= '0' && **p <= '9') {
        val = val * 10 + (**p - '0');
        (*p)++;
    }

    if (**p == '.') {
        (*p)++;
        while (**p >= '0' && **p <= '9') {
            frac = frac * 10 + (**p - '0');
            div *= 10;
            (*p)++;
        }
    }

    return sign * (val + frac / div);
}

int gcode_parse(const char *line, gcode_cmd_t *cmd) {
    cmd->cmd = 0;
    cmd->x = 0;
    cmd->y = 0;
    cmd->z = 0;
    cmd->e = 0;
    cmd->f = 0;
    cmd->has_x = 0;
    cmd->has_y = 0;
    cmd->has_z = 0;
    cmd->has_e = 0;
    cmd->has_f = 0;

    const char *p = line;

    while (*p) {
        if (*p == 'G' || *p == 'g') {
            p++;
            cmd->cmd = 0;
            while (*p >= '0' && *p <= '9') {
                cmd->cmd = cmd->cmd * 10 + (*p - '0');
                p++;
            }
        } else if (*p == 'X' || *p == 'x') {
            p++;
            cmd->x = parse_float(&p);
            cmd->has_x = 1;
        } else if (*p == 'Y' || *p == 'y') {
            p++;
            cmd->y = parse_float(&p);
            cmd->has_y = 1;
        } else if (*p == 'Z' || *p == 'z') {
            p++;
            cmd->z = parse_float(&p);
            cmd->has_z = 1;
        } else if (*p == 'E' || *p == 'e') {
            p++;
            cmd->e = parse_float(&p);
            cmd->has_e = 1;
        } else if (*p == 'F' || *p == 'f') {
            p++;
            cmd->f = parse_float(&p);
            cmd->has_f = 1;
        } else {
            p++;
        }
    }

    return 0;
}
