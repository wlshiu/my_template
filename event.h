/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file event.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/06/13
 * @license
 * @description
 */

#ifndef __event_H_wHDIbK0w_lgzZ_HWrU_sXAR_ufL9vTlxh0qh__
#define __event_H_wHDIbK0w_lgzZ_HWrU_sXAR_ufL9vTlxh0qh__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================
#if defined(__MINGW32__)
    #include <windows.h>
    #undef sleep
    #define sleep(x)        Sleep(x * 1000)

    #undef usleep
    #define usleep(x)       Sleep((unsigned long)x / 1000ul)
#endif
//=============================================================================
//                  Structure Definition
//=============================================================================
typedef struct event
{
    unsigned int    uid;
    // pid_t           pid;
    unsigned int    post_events;
    // unsigned int    wait_events;
} event_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================
static event_t*
event_create(void)
{
    event_t     *pHEvent = malloc(sizeof(event_t));

    if( pHEvent )
    {
        memset(pHEvent, 0x0, sizeof(event_t));
        pHEvent->uid = (clock() & 0xFFFFFFFF);
    }
    return pHEvent;
}

static void
event_destroy(event_t *pHEvent)
{
    if( pHEvent )   free(pHEvent);
    return;
}

static int
event_wait(
    event_t         *pHEvent,
    unsigned int    mask,
    unsigned int    timeout_ms,
    unsigned int    *pEvent_flag)
{
    unsigned int    bCheckTimeout = (timeout_ms) ? 1 : 0;
    unsigned int    period_ms = 100;

    while(1)
    {
        if( pHEvent->post_events & mask )
        {
            if( pEvent_flag )
                *pEvent_flag = pHEvent->post_events & mask;
            break;
        }

        if( bCheckTimeout )
        {
            if( !timeout_ms )
                break;

            timeout_ms -= period_ms;
            timeout_ms = (timeout_ms > 0) ? timeout_ms : 0;
        }

        usleep(period_ms * 1000);
    }

    pHEvent->post_events = 0;
    return 0;
}

static int
event_set(event_t *pHEvent, unsigned int flags)
{
    pHEvent->post_events |= flags;
    return 0;
}
//=============================================================================
//                  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
