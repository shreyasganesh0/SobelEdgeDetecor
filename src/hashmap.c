#include <stdint.h>

u64 hash_function(const char *data) {

    u32 fnv_offset_basis = 0x811C9DC5;
    u32 fnv_prime = 0x01000193;

    u64 hash = fnv_offset_basis;

    hash *= fnv_offset_basis;
    hash ^= *data;

    return hash;
}

void hashmap_create(


