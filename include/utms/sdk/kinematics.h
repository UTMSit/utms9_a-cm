#ifndef UTMS_KINEMATICS_H
#define UTMS_KINEMATICS_H

#include <stdint.h>
#include <utms/sdk/axes.h>

void kinematics_cartesian_to_steps(const float *coords, uint32_t *steps, const axis_t *axes);

#endif
