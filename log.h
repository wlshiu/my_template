/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file log.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/09/08
 * @license
 * @description
 */

#ifndef __log_H_wqB2y7CS_liSw_H35Z_sUFI_uXFA3vB7NhIQ__
#define __log_H_wqB2y7CS_liSw_H35Z_sUFI_uXFA3vB7NhIQ__


#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
//=============================================================================
//                  Constant Definition
//=============================================================================
#define MAX_LOG_FLAG_NUM            64

/**
 *  log type and it MUST be less than MAX_LOG_FLAG_NUM
 */
typedef enum LOG_TYPE
{
    LOG_BASE            = 0,
    LOG_TEST,

} LOG_TYPE_e;
//=============================================================================
//                  Macro Definition
//=============================================================================
#define LOG_FLAG_SET(pLogFlag, bit_order)       ((pLogFlag)->BitFlag[(bit_order)>>5] |=  (1<<((bit_order)&0x1F)))
#define LOG_FLAG_CLR(pLogFlag, bit_order)       ((pLogFlag)->BitFlag[(bit_order)>>5] &= ~(1<<((bit_order)&0x1F)))
#define LOG_FLAG_IS_SET(pLogFlag, bit_order)    ((pLogFlag)->BitFlag[(bit_order)>>5] &   (1<<((bit_order)&0x1F)))
#define LOG_FLAG_ZERO(pLogFlag)                 memset((void*)(pLogFlag),0x0,sizeof(LOG_FLAG_s))

//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct LOG_FLAG
{
    unsigned int        BitFlag[((MAX_LOG_FLAG_NUM)+0x1F)>>5];
} LOG_FLAG_s;
//=============================================================================
//                  Global Data Definition
//=============================================================================
extern LOG_FLAG_s    gLogFlags;
//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
#define log_err(str, args...)               fprintf(stderr, "%s[#%u] " str, __func__, __LINE__, ## args)

#define log(type, str, args...)             ((void)((LOG_FLAG_IS_SET(&gLogFlags, type)) ? fprintf(stderr, str, ## args): 0))
#define log_on(type)                        LOG_FLAG_SET(&gLogFlags, type)
#define log_off(type)                       LOG_FLAG_CLR(&gLogFlags, type)


#ifdef __cplusplus
}
#endif

#endif

