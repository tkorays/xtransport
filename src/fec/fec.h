#ifndef __XTRANSPORT_FEC_H__
#define __XTRANSPORT_FEC_H__

#include <stdint.h>
#include <sys/types.h>

typedef enum {
    XT_FEC_TYPE_UNKNOWN = 0,
    XT_FEC_TYPE_XOR,
    XT_FEC_TYPE_RS,
    XT_FEC_TYPE_RAPTOR,
    XT_FEC_TYPE_NUM,
} XT_FEC_TYPE;

typedef struct {
    int num_data_blocks;
    int num_fec_blocks;
    float protect_rate;
} xt_fec_param_t;


typedef struct xt_fec_encoder {
    XT_FEC_TYPE type;
} xt_fec_encoder_t;
typedef struct xt_fec_decoder xt_fec_decoder_t;


#ifdef __cplusplus
extern "C" {
#endif

xt_fec_encoder_t* xt_fec_new(XT_FEC_TYPE type);
void xt_fec_delete(xt_fec_encoder_t* self);
void xt_fec_update_param(xt_fec_encoder_t* self, xt_fec_param_t* param);
int xt_fec_encoder_input(xt_fec_encoder_t* self, char* data, size_t length);
int xt_fec_encoder_output(xt_fec_encoder_t* self, char* buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* __XTRANSPORT_FEC_H__ */

