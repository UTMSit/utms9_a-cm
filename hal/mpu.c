#include <utms/hal/mpu.h>
#include <utms/hal/stm32f4xx.h>

#define MPU_BASE        0xE000ED90UL
#define MPU_TYPE        (*(volatile uint32_t *)(MPU_BASE + 0x00))
#define MPU_CTRL        (*(volatile uint32_t *)(MPU_BASE + 0x04))
#define MPU_RNR         (*(volatile uint32_t *)(MPU_BASE + 0x08))
#define MPU_RBAR        (*(volatile uint32_t *)(MPU_BASE + 0x0C))
#define MPU_RASR        (*(volatile uint32_t *)(MPU_BASE + 0x10))

#define MPU_CTRL_ENABLE           (1UL << 0)
#define MPU_CTRL_HFNMIENA         (1UL << 1)
#define MPU_CTRL_PRIVDEFENA       (1UL << 2)

#define MPU_RASR_ENABLE           (1UL << 0)
#define MPU_RASR_SIZE_SHIFT       1
#define MPU_RASR_S                (1UL << 18)
#define MPU_RASR_AP_SHIFT         24
#define MPU_RASR_XN               (1UL << 28)

static uint32_t compute_region_size(uint32_t size_bytes) {
    uint32_t region_size = 0;
    uint32_t s = 32;
    while (s <= (1UL << 31)) {
        if (s >= size_bytes) {
            break;
        }
        s <<= 1;
        region_size++;
    }
    return region_size + 5;
}

void utms_mpu_init(void) {
    if (!(MPU_TYPE)) return;

    utms_mpu_configure_region(UTMS_MPU_REGION_0, 0x00000000UL, 1024,
                              UTMS_MPU_ACCESS_NONE, 0, 0);

    utms_mpu_configure_region(UTMS_MPU_REGION_1, 0x08000000UL, 1024 * 1024,
                              UTMS_MPU_ACCESS_PRIV_RO, 1, 0);

    utms_mpu_configure_region(UTMS_MPU_REGION_2, 0x20000000UL, 128 * 1024,
                              UTMS_MPU_ACCESS_FULL_RW, 0, 0);

    utms_mpu_configure_region(UTMS_MPU_REGION_3, 0x10000000UL, 64 * 1024,
                              UTMS_MPU_ACCESS_PRIV_RW, 1, 0);

    utms_mpu_configure_region(UTMS_MPU_REGION_4, 0x40000000UL, 512 * 1024 * 1024,
                              UTMS_MPU_ACCESS_PRIV_RW, 0, 1);

    utms_mpu_enable();
}

void utms_mpu_configure_region(utms_mpu_region_t region, uint32_t base_addr,
                               uint32_t size_bytes, utms_mpu_access_t access,
                               uint8_t executable, uint8_t shareable) {
    if (!(MPU_TYPE)) return;

    MPU_RNR = (uint32_t)region;
    MPU_RBAR = base_addr | (uint32_t)region | (1UL << 4);

    uint32_t size_enc = compute_region_size(size_bytes);
    uint32_t rasr = MPU_RASR_ENABLE |
                    (size_enc << MPU_RASR_SIZE_SHIFT) |
                    ((uint32_t)access << MPU_RASR_AP_SHIFT);

    if (!executable) {
        rasr |= MPU_RASR_XN;
    }
    if (shareable) {
        rasr |= MPU_RASR_S;
    }

    MPU_RASR = rasr;
}

void utms_mpu_enable(void) {
    if (!(MPU_TYPE)) return;
    __asm__ volatile ("dsb" ::: "memory");
    __asm__ volatile ("isb" ::: "memory");
    MPU_CTRL = MPU_CTRL_ENABLE | MPU_CTRL_PRIVDEFENA;
    __asm__ volatile ("dsb" ::: "memory");
    __asm__ volatile ("isb" ::: "memory");
}

void utms_mpu_disable(void) {
    if (!(MPU_TYPE)) return;
    MPU_CTRL = 0;
    __asm__ volatile ("dsb" ::: "memory");
    __asm__ volatile ("isb" ::: "memory");
}
