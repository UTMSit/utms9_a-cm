#ifndef UTMS_MPU_H
#define UTMS_MPU_H

#include <stdint.h>

typedef enum {
    UTMS_MPU_REGION_0 = 0,
    UTMS_MPU_REGION_1 = 1,
    UTMS_MPU_REGION_2 = 2,
    UTMS_MPU_REGION_3 = 3,
    UTMS_MPU_REGION_4 = 4,
    UTMS_MPU_REGION_5 = 5,
    UTMS_MPU_REGION_6 = 6,
    UTMS_MPU_REGION_7 = 7
} utms_mpu_region_t;

typedef enum {
    UTMS_MPU_ACCESS_NONE = 0,
    UTMS_MPU_ACCESS_PRIV_RW = 1,
    UTMS_MPU_ACCESS_FULL_RW = 3,
    UTMS_MPU_ACCESS_PRIV_RO = 5,
    UTMS_MPU_ACCESS_FULL_RO = 6
} utms_mpu_access_t;

void utms_mpu_init(void);
void utms_mpu_configure_region(utms_mpu_region_t region, uint32_t base_addr,
                               uint32_t size_bytes, utms_mpu_access_t access,
                               uint8_t executable, uint8_t shareable);
void utms_mpu_enable(void);
void utms_mpu_disable(void);

#endif
