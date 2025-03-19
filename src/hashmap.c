#include <stdint.h>

u64 hash_function(const char *data) {

    u32 fnv_offset_basis = 0x811C9DC5;
    u32 fnv_prime = 0x01000193;

    u64 hash = fnv_offset_basis;

    hash *= fnv_offset_basis;
    hash ^= *data;

    return hash;
}

hashmap_t *hashmap_create(int capacity) {

    hashmap_t *hashmap = malloc(sizeof(hashmap_t));
    hashmap->map = malloc(capacity * sizeof(map_t));

    hashmap->size = 0;
    hashmap->capacity = capacity;
    for (int i = 0; i < capacity; i++) {

        hashmap->map[idx].val = 0;
        hashmap->map[idx].fill = 0;
    }

    return hashmap;
}

void hashmap_insert(const char *byte, hashmap_t *hashmap, u8 value) {

    u64 hash = hash_function(byte);

    int idx = hash % hashmap->capacity;

    while (hashmap->map[idx].fill == 1) {
        idx++;
        idx = idx - (idx >= hashmap->capacity) * hashmap->capacity;
    }
    hashmap->map[idx].val = value;
    hashmap->map[idx].fill = 1;

    return;
}

int hashmap_index_get(const char *key, hashmap_t *hashmap) {

    u64 hash = hash_function(key);

    int idx = hash % hashmap->capacity;

    while (hashmap->map[idx].fill == 1) {
        idx++;
        idx = idx - (idx >= hashmap->capacity) * hashmap->capacity;
    }

    return idx;
}
