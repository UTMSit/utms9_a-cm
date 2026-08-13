#ifndef UTMS_MEMORY_POOL_H
#define UTMS_MEMORY_POOL_H

#include <stdint.h>
#include <stddef.h>

typedef struct utms_mem_pool {
    uint32_t block_size;
    uint32_t count;
    uint8_t *base;
    void *free_list;
    uint32_t free_count;
} utms_mem_pool_t;

#define UTMS_MEM_POOL_DEFINE(name, bsize, cnt) \
    static uint8_t __attribute__((aligned(32))) name##_storage[(bsize) * (cnt)]; \
    static utms_mem_pool_t name = { \
        .block_size = (bsize), \
        .count = (cnt), \
        .base = name##_storage, \
        .free_list = 0, \
        .free_count = 0 \
    }

#define UTMS_MEM_POOL_EXTERN(name) extern utms_mem_pool_t name

void utms_mem_pool_init(utms_mem_pool_t *pool);
void *utms_mem_alloc(utms_mem_pool_t *pool);
void utms_mem_free(utms_mem_pool_t *pool, void *ptr);
uint32_t utms_mem_free_count(const utms_mem_pool_t *pool);

#endif
