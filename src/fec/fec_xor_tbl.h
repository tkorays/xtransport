#ifndef __XTRANSPORT_FEC_XOR_TBL_H__
#define __XTRANSPORT_FEC_XOR_TBL_H__

#include <stdint.h>
#include <sys/types.h>

typedef enum {
    XOR_FEC_MASK_TYPE_RANDOM,
    XOR_FEC_MASK_TYPE_BURSTY,
} XOR_FEC_MASK_TYPE;


typedef struct {
    size_t      mask_length;    // 16 or 48
    size_t      tbl_size;
    uint8_t*    tbl;
} fec_xor_tbl_t;

#ifdef __cplusplus
extern "C" {
#endif

fec_xor_tbl_t* fec_xor_tbl_new(XOR_FEC_MASK_TYPE type);
void fec_xor_tbl_delete(fec_xor_tbl_t* self);
const uint8_t* fec_xor_tbl_lookup(fec_xor_tbl_t* self, int num_media_packets, int num_fec_packets);

#ifdef __cplusplus
}
#endif

#endif /* __XTRANSPORT_FEC_XOR_TBL_H__ */

