/**
 * @file xt_fec.h
 * @author tkorays (tkorays@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __XT_FEC_H__
#define __XT_FEC_H__

#include "cf/types.h"

/**
 * @brief fec parameters
 */
typedef struct {
    int num_data_blocks;
    int num_fec_blocks;
} xt_fec_param_t;

/**
 * @brief fec encoder.
 * 
 */
typedef struct {
    xt_fec_param_t  param;
} xt_fec_encoder_t;

/**
 * @brief fec decoder
 * 
 */
typedef struct {
    int a;
} xt_fec_decoder_t;


cf_bool_t xt_fec_encoder_init(xt_fec_encoder_t* self);
void xt_fec_encoder_deinit(xt_fec_encoder_t* self);
void xt_fec_encoder_update_param(
    xt_fec_encoder_t* self,
    xt_fec_param_t* param
);
cf_bool_t xt_fec_encoder_input(
    xt_fec_encoder_t* self,
    char* data,
    cf_size_t length
);
int xt_fec_encoder_output(
    xt_fec_encoder_t* self,
    char* buffer,
    cf_size_t buffer_size
);


cf_bool_t xt_fec_decoder_init(xt_fec_decoder_t* self);
void xt_fec_decoder_deinit(xt_fec_decoder_t* self);
cf_bool_t xt_fec_decoder_input(
    xt_fec_decoder_t* self,
    char* data,
    cf_size_t length
);
int xt_fec_decoder_output(
    xt_fec_decoder_t* self,
    char* buffer,
    cf_size_t buffer_size
);


#endif /* __XT_FEC_H__ */
