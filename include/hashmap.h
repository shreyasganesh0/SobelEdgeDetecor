#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct {
    u8 fill; 
    int val;
} map_t;

typedef struct {
    map_t *map;
    int capcity;
    int size;
} hashmap_t;


