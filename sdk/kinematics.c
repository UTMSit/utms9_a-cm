#include <utms/sdk/kinematics.h>

void kinematics_cartesian_to_steps(const float *coords, uint32_t *steps, const axis_t *axes) {
    steps[0] = (uint32_t)(coords[0] * axes[0].steps_per_mm);
    steps[1] = (uint32_t)(coords[1] * axes[1].steps_per_mm);
    steps[2] = (uint32_t)(coords[2] * axes[2].steps_per_mm);
}
