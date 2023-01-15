#ifndef __XTRANSPORT_FEC_XOR_H__
#define __XTRANSPORT_FEC_XOR_H__

#include <stdint.h>
#include <sys/types.h>

#include "fec.h"

typedef struct {
    xt_fec_encoder_t base;
} fec_xor_encoder_t;


fec_xor_encoder_t* fec_xor_encoder_new();
void fec_xor_encoder_delete(fec_xor_encoder_t* self);
void fec_xor_encoder_update_param(fec_xor_encoder_t* self, xt_fec_param_t* param);
int fec_xor_encoder_input(fec_xor_encoder_t* self, char* data, size_t length);



#endif /* __XTRANSPORT_FEC_XOR_H__ */

