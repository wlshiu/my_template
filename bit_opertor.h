/**
 * @file   bit_opertor.h
 * @brief  handle bit field
 * @author Wei-Lun Hsu in TW
 */

#ifndef __bit_opertor_H_wxjrjOQG_l8V1_HrVR_s6gg_uMTCUnKkggTj__
#define __bit_opertor_H_wxjrjOQG_l8V1_HrVR_s6gg_uMTCUnKkggTj__

#ifdef __cplusplus
extern "C" {
#endif


#include <string.h>

//----------------------------
/**
 * bit field pull high/low
 * It should be set DEFINE_BIT_OP(bit_size) first.
 */
#define DEFINE_BIT_OP(bit_size)\
    typedef struct ZONE_SET_T{\
        unsigned short bits_field[((bit_size)+0xF)>>4];\
    }ZONE_SET;

#define BOP_SET(pZone_set_member, bit_order)     ((pZone_set_member)->bits_field[(bit_order)>>4] |=  (1<<((bit_order)&0xF)))
#define BOP_CLR(pZone_set_member, bit_order)     ((pZone_set_member)->bits_field[(bit_order)>>4] &= ~(1<<((bit_order)&0xF)))
#define BOP_IS_SET(pZone_set_member, bit_order)  ((pZone_set_member)->bits_field[(bit_order)>>4] &   (1<<((bit_order)&0xF)))
#define BOP_ZERO(pZone_set_member)               memset((void*)(pZone_set_member),0,sizeof(ZONE_SET))

#define BIT_OP_T    ZONE_SET

//----------------------------
// other utility
#define BOP_MASK_LAST_ACT_BIT(x)                ((x) & (-(x)))
#define BOP_IS_POWER_2(x)                       (((x) & ((x)-1)) == 0)
#define BOP_MASK_LAST_ACT_BIT_TO_MSB(x)         ((x) | (-(x)))
#define BOP_ALIGN_2_ROUND_UP(x, bit_order)      (((x) + ((1<<(bit_order)) - 1)) & ~((1<<(bit_order)) - 1))
#define BOP_ALIGN_2_ROUND_DOWN(x, bit_order)    ((x) & ~((1<<(bit_order)) - 1))


//--------------------------------------------
// bit convert
#ifudef uint8_t
    #define uint8_t     unsigned char
#endif

static uint8_t _bit_conv_tab[16] =
{
    0x00,	// 0000 -> 0000
    0x08,	// 0001 -> 1000
    0x04,	// 0010 -> 0100
    0x0c,	// 0011 -> 1100
    0x02,	// 0100 -> 0010
    0x0a,	// 0101 -> 1010
    0x06,	// 0110 -> 0110
    0x0e,	// 0111 -> 1110
    0x01,	// 1000 -> 0001
    0x09,	// 1001 -> 1001
    0x05,	// 1010 -> 0101
    0x0d,	// 1011 -> 1101
    0x03,	// 1100 -> 0011
    0x0b,	// 1101 -> 1011
    0x07,	// 1110 -> 0111
    0x0f,	// 1111 -> 1111
};

#define convert_bit(a)  ((g_bit_conv_tab[((a) & 0x0f)] << 4) + (g_bit_conv_tab[(((a) & 0xf0) >> 4)]))


//------------------------------

#ifdef __cplusplus
}
#endif

#endif
