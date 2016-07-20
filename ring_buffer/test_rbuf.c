
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "pthread.h"

#include "ring_buffer.h"
/////////////////////////////////////////////////////

#define err(str, args...)           do{ pthread_mutex_lock(&g_mutex_log);\
                                        fprintf(stderr, "%s[#%u] " str, __func__, __LINE__, ## args);\
                                        pthread_mutex_unlock(&g_mutex_log);\
                                    }while(0)

#define msg(str, args...)           do{ pthread_mutex_lock(&g_mutex_log);\
                                        fprintf(stderr, str, ## args);\
                                        pthread_mutex_unlock(&g_mutex_log);\
                                    }while(0)

/////////////////////////////////////////////////////
static uint8_t              *g_pRingBuf = 0;
static RBUF_OPT_s           g_rb_opt = {0};
static pthread_mutex_t      g_mutex_log = PTHREAD_MUTEX_INITIALIZER;
/////////////////////////////////////////////////////

static void*
_thread_write(void *arg)
{
    int             i;
    bool            *pBCancel_thread = (bool*)arg;
    uint8_t         data_w[1024] = {0};
    uint8_t         value_w = 0;
    int             length_w = 0;
    int             total_length = 0;

    while( *pBCancel_thread == false )
    {
        length_w = (rand() >> 2) & 0x1f;
        length_w = (length_w) ? length_w : 1;

        for(i = 0; i < length_w; ++i)
        {
            data_w[i] = (value_w + i) & 0xFF;
        }

        if( rbuf_update_w(&g_rb_opt, data_w, length_w) )
        {
            value_w = data_w[length_w - 1] + 1;
            total_length += length_w;
        }

        Sleep(5);
    }

    msg("++++++ %d\n", data_w[length_w - 1]);
    if( 0 )
    {
        uint8_t     val_start = g_pRingBuf[0];
        msg(" total size = %d, val= %d\n", 1024 - rbuf_get_remain_size(&g_rb_opt), val_start);
        for(i = 0; i < total_length; ++i)
        {
            if( g_pRingBuf[i] != ((val_start + i) & 0xff) )
                err("err, %d/%d\n", data_w[i], i);
        }
    }

    err("leave  %s\n", __func__);
    pthread_exit(NULL);
    return 0;
}

int main()
{
    bool            bCancel_thread = false;
    uint8_t         data_r[1024] = {0}, data_value = 0;
    int             read_length = 0, get_length = 0;
    time_t          start_sec = 0;
    pthread_t       tid;

    if( !(g_pRingBuf = malloc(1024)) )
    {
        err("malloc fail !\n");
        return 1;
    }

    rbuf_init(&g_rb_opt, g_pRingBuf, 1024);

    start_sec = time(NULL);
    srand((unsigned int)start_sec);

    pthread_create(&tid, NULL, _thread_write, &bCancel_thread);

    while( (time(NULL) - start_sec) < 100 /* sec*/)
    {
        read_length = (rand() >> 3) & 0x1F;
        read_length = (read_length) ? read_length : 1;
        memset(data_r, 0x0, 1024);

        #if 0
        uint8_t     *pData_r = 0;

        read_length = 0;
        if( (get_length = rbuf_update_r(&g_rb_opt, &pData_r, read_length)) )
        {
            if( *pData_r != ((data_value + 1) & 0xFF) )
            {
                msg("    r_err: val= %d, %d, len= (%d/%d)\n", data_value, data_r[0], get_length, read_length);
            }

            data_value = pData_r[get_length - 1];
        }

        #else
        if( (get_length = rbuf_update_dup_r(&g_rb_opt, data_r, read_length)) )
        {
            if( data_r[0] != ((data_value + 1) & 0xFF) )
            {
                msg("    r_err: val= %d, %d, len= (%d/%d)\n", data_value, data_r[0], get_length, read_length);
            }

            data_value = data_r[get_length - 1];
        }
        #endif
        Sleep(1);
    }

    bCancel_thread = true;
    pthread_join(tid, NULL);

    while( (get_length = rbuf_update_dup_r(&g_rb_opt, data_r, 20)) )
        data_value = data_r[get_length - 1];

    msg("--- %d\n", data_value);

    if( g_pRingBuf )      free(g_pRingBuf);

    return 0;
}

