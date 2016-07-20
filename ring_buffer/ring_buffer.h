/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file ring_buffer.h
 *
 * @author Wei-Lun Hsu
 * @version 1.0
 * @date 2016/07/20
 * @license
 * @description
 */

#ifndef __ring_buffer_H_w01mWfFV_l7PP_HDq5_sHYs_uQpKPSLBbOYJ__
#define __ring_buffer_H_w01mWfFV_l7PP_HDq5_sHYs_uQpKPSLBbOYJ__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================
#define rbuf_err(str, args...)             fprintf(stderr, str, ## args)

#if 0
    #define rbuf_dbg(str, args...)             fprintf(stderr, str, ## args)
#else
    #define rbuf_dbg(str, args...)
#endif
//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct _RBUF_OPT
{
    unsigned char       *pStartAddr;
    unsigned char       *pEndAddr;
    unsigned char       *pWriteAddr;
    unsigned char       *pReadAddr;
    unsigned long       isEmpty;
} RBUF_OPT;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
/**
 *  init handle
 */
static inline int
rbuf_init(RBUF_OPT *pOpt, unsigned char *pBufAddr, int BufSize)
{
    pOpt->pStartAddr    = pBufAddr;
    pOpt->pEndAddr      = pOpt->pStartAddr + BufSize;
    pOpt->pWriteAddr    = pOpt->pStartAddr;
    pOpt->pReadAddr     = pOpt->pStartAddr;
    pOpt->isEmpty       = 1ul;
    return 0;
}

/**
 *  update write index
 *  @return     The total number of data successfully written.
 */
static inline int
rbuf_update_w(RBUF_OPT *pOpt, unsigned char *pData, int Length)
{
    unsigned long   start = 0ul, end = 0ul;
    unsigned long   cur_w = 0ul, cur_r = 0ul;
    unsigned long   isEmpty = 0ul;

    if( !pOpt || !pData || !Length )
    {
        rbuf_err("rbuf get invalid parameters (%p, %p, %d)!\n",
                 pOpt, pData, Length);
        return 0;
    }

    isEmpty = pOpt->isEmpty;
    cur_w   = (unsigned long)pOpt->pWriteAddr;
    cur_r   = (unsigned long)pOpt->pReadAddr;
    start   = (unsigned long)pOpt->pStartAddr;
    end     = (unsigned long)pOpt->pEndAddr;

    if( cur_w > cur_r ||
        (cur_w == cur_r && isEmpty) )
    {
        if( (end - cur_w) > Length )
        {
            memcpy((void*)cur_w, (void*)pData, Length);
            cur_w += Length;
        }
        else
        {
            int     vect_0 = end - cur_w;
            int     vect_1 = 0;

            vect_1 = Length - vect_0;

            if( (cur_r - start) < vect_1 )
            {
                rbuf_dbg("%s", "rbuf space not enough !\n");
                return 0;
            }

            memcpy((void*)cur_w, (void*)pData, vect_0);
            if( vect_1 )
                memcpy((void*)start, (void*)(pData + vect_0), vect_1);

            cur_w = start + vect_1;
        }
    }
    else if( cur_w < cur_r )
    {
        if( (cur_r - cur_w) < Length )
        {
            rbuf_dbg("%s", "rbuf space not enough !\n");
            return 0;
        }

        memcpy((void*)cur_w, (void*)pData, Length);
        cur_w += Length;
    }
    else
    {
        rbuf_dbg("%s", "rbuf full !\n");
        return 0;
    }

    //-----------------------
    pOpt->pWriteAddr = (unsigned char*)cur_w;
    pOpt->isEmpty    = 0ul;
    return Length;
}

/**
 *  update read index and duplicate data (cover wrap case)
 *  @return     The total number of data successfully read.
 */
static inline int
rbuf_update_dup_r(RBUF_OPT *pOpt, unsigned char *pData, int Length)
{
    unsigned long   start = 0ul, end = 0ul;
    unsigned long   cur_w = 0ul, cur_r = 0ul;
    unsigned long   isEmpty = 0ul;

    if( !pOpt || !pData || !Length )
    {
        rbuf_err("rbuf get invalid parameters (%p, %p, %d)!\n",
                 pOpt, pData, Length);
        return 0;
    }

    isEmpty = pOpt->isEmpty;
    cur_w   = (unsigned long)pOpt->pWriteAddr;
    cur_r   = (unsigned long)pOpt->pReadAddr;
    start   = (unsigned long)pOpt->pStartAddr;
    end     = (unsigned long)pOpt->pEndAddr;

    if( cur_w < cur_r ||
        (cur_w == cur_r && !isEmpty) )
    {
        if( (end - cur_r) > Length )
        {
            memcpy((void*)pData, (void*)cur_r, Length);
            cur_r += Length;
        }
        else
        {
            int     vect_0 = end - cur_r;
            int     vect_1 = 0;

            memcpy((void*)pData, (void*)cur_r, vect_0);

            vect_1 = Length - vect_0;

            if( (cur_w - start) < vect_1 )
                vect_1  = cur_w - start;

            memcpy((void*)(pData + vect_0), (void*)start, vect_1);

            cur_r  = start + vect_1;
            Length = vect_0 + vect_1;
        }
    }
    else if( cur_r < cur_w )
    {
        if( (cur_w - cur_r) < Length )
            Length = cur_w - cur_r;

        memcpy((void*)pData, (void*)cur_r, Length);
        cur_r += Length;
    }
    else
    {
        rbuf_dbg("%s", "rbuf is empty !\n");
        return 0;
    }

    isEmpty = (cur_r == cur_w) ? 1ul : isEmpty;
    //-----------------------
    pOpt->pReadAddr = (unsigned char*)cur_r;
    pOpt->isEmpty   = isEmpty;
    return Length;
}

/**
 *  update read index and use share memory (no memory copy)
 *  ps. when wrap, split 2 part
 */
static inline int
rbuf_update_r(RBUF_OPT *pOpt, unsigned char **ppData, int Length)
{
    long            data_length = 0l;
    unsigned long   start = 0ul, end = 0ul;
    unsigned long   cur_w = 0ul, cur_r = 0ul;
    unsigned long   isEmpty = 0ul;

    if( !pOpt || !ppData )
    {
        rbuf_err("rbuf get invalid parameters (%p, %p)!\n",
                 pOpt, ppData);
        return 0;
    }

    isEmpty = pOpt->isEmpty;
    cur_w   = (unsigned long)pOpt->pWriteAddr;
    cur_r   = (unsigned long)pOpt->pReadAddr;
    start   = (unsigned long)pOpt->pStartAddr;
    end     = (unsigned long)pOpt->pEndAddr;

    data_length = (Length) ? Length : (end - start);

    if( cur_w < cur_r ||
        (cur_w == cur_r && !isEmpty) )
    {
        *ppData = (unsigned char*)cur_r;

        if( (end - cur_r) > data_length )
        {
            cur_r += data_length;
        }
        else
        {
            data_length = end - cur_r;

            cur_r = start;
        }
    }
    else if( cur_r < cur_w )
    {
        if( (cur_w - cur_r) < data_length )
            data_length = cur_w - cur_r;

        *ppData = (unsigned char*)cur_r;

        cur_r += data_length;
    }
    else
    {
        rbuf_dbg("%s", "rbuf is empty !\n");
        return 0;
    }

    isEmpty = (cur_r == cur_w) ? 1ul : isEmpty;
    //-----------------------
    pOpt->pReadAddr = (unsigned char*)cur_r;
    pOpt->isEmpty   = isEmpty;
    return data_length;
}

/**
 *  return remain size in ring buffer
 */
static inline size_t
rbuf_get_remain_size(
    RBUF_OPT  *pOpt)
{
    size_t          data_length = 0l;
    unsigned long   start = 0ul, end = 0ul;
    unsigned long   cur_w = 0ul, cur_r = 0ul;
    unsigned long   isEmpty = 0ul;

    if( !pOpt )
    {
        rbuf_err("rbuf get invalid parameters (%p)!\n", pOpt);
        return 0;
    }

    isEmpty = pOpt->isEmpty;
    cur_w   = (unsigned long)pOpt->pWriteAddr;
    cur_r   = (unsigned long)pOpt->pReadAddr;
    start   = (unsigned long)pOpt->pStartAddr;
    end     = (unsigned long)pOpt->pEndAddr;

    if( cur_w > cur_r )
    {
        data_length = end - cur_w + cur_r - start;
    }
    else if( cur_w < cur_r )
    {
        data_length = cur_r - cur_w;
    }
    else
    {
        if( isEmpty )       data_length = 0;
        else                data_length = end - start;
    }

    return (size_t)data_length;
}


#ifdef __cplusplus
}
#endif

#endif
