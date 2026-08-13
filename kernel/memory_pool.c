#include <utms/kernel/memory_pool.h>

#define ENTER_CRITICAL() __asm__ volatile ("cpsid i" ::: "memory")
#define EXIT_CRITICAL()  __asm__ volatile ("cpsie i" ::: "memory")

void utms_mem_pool_init(utms_mem_pool_t *pool) {
    ENTER_CRITICAL();
    pool->free_list = 0;
    pool->free_count = pool->count;

    for (uint32_t i = 0; i < pool->count; i++) {
        uint8_t *block = pool->base + i * pool->block_size;
        void **next_ptr = (void **)block;
        *next_ptr = pool->free_list;
        pool->free_list = block;
    }
    EXIT_CRITICAL();
}

void *utms_mem_alloc(utms_mem_pool_t *pool) {
    ENTER_CRITICAL();
    if (!pool->free_list) {
        EXIT_CRITICAL();
        return 0;
    }

    void *block = pool->free_list;
    pool->free_list = *((void **)block);
    pool->free_count--;
    EXIT_CRITICAL();

    uint32_t *p = (uint32_t *)block;
    uint32_t words = pool->block_size >> 2;
    for (uint32_t i = 0; i < words; i++) {
        p[i] = 0;
    }
    return block;
}

void utms_mem_free(utms_mem_pool_t *pool, void *ptr) {
    if (!ptr) return;

    ENTER_CRITICAL();
    *((void **)ptr) = pool->free_list;
    pool->free_list = ptr;
    pool->free_count++;
    EXIT_CRITICAL();
}

uint32_t utms_mem_free_count(const utms_mem_pool_t *pool) {
    return pool->free_count;
}
