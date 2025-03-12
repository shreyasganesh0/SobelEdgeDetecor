#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

typedef enum {
    BIT_PREC_8,
    BIT_PREC_16,
} dqt_prec_t;

typedef struct {
    dqt_prec_t prec;
    u8 idx;
    u8 *data_8;
    u16 *data_16;
} dqt_t;


