/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file descriptor_def.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/08/01
 * @license
 * @description
 */

#ifndef __descriptor_def_H_wd56IL2B_lcFu_H3bg_sGlm_u27ruiJNsXon__
#define __descriptor_def_H_wd56IL2B_lcFu_H3bg_sGlm_u27ruiJNsXon__

#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//                  Constant Definition
//=============================================================================

//=============================================================================
//                  Macro Definition
//=============================================================================
#define DESC_NAME_RULE(struct_name, tag)    \
            g_##struc_name##_##tag##_desc

#define REGISTER_DESC(struct_name, tag, pFirstDescriptor)       \
    do{ extern struct_name   DESC_NAME_RULE(struc_name, tag);   \
        struct_name   **p;                                      \
        p = &pFirstDescriptor;                                  \
        while (*p != 0) p = &(*p)->Next;                        \
        *p = &DESC_NAME_RULE(struc_name, tag);                  \
        DESC_NAME_RULE(struc_name, tag).Next = 0;               \
    }while(0)

#define INSTANCE_DESC(struc_name, tag, args...)         \
    struc_name      DESC_NAME_RULE(struc_name, tag) = { \
        .Next = 0, .id = tag,                           \
        ## args                                         \
    }
//=============================================================================
//                  Structure Definition
//=============================================================================

//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================
/*****************************

// example
#include <stdio.h>
#include <stdlib.h>

typedef struct my_desc
{
    struct my_desc  *Next;
    int     id;

    int     (*init)(void);
    int     (*deinit)(void);
} my_desc_t;

 static int
_init()
{
    printf("%s[%d]\n", __func__, __LINE__);
    return 0;
}

static int
_deinit()
{
    printf("%s[%d]\n", __func__, __LINE__);
    return 0;
}

static int
_init_2()
{
    printf("%s[%d]\n", __func__, __LINE__);
    return 0;
}

static int
_deinit_2()
{
    printf("%s[%d]\n", __func__, __LINE__);
    return 0;
}

INSTANCE_DESC(my_desc_t, 1, .init = _init, .deinit = _deinit);
INSTANCE_DESC(my_desc_t, 2, .init = _init_2, .deinit = _deinit_2);

int main()
{
    int             i;
    my_desc_t       *pFirst_desc = 0;
    REGISTER_DESC(my_desc_t, 1, pFirst_desc);
    REGISTER_DESC(my_desc_t, 2, pFirst_desc);

    for(i = 0; i < 2; i++)
    {
        my_desc_t   *pCur_desc = pFirst_desc + i;
        pCur_desc->init();
        pCur_desc->deinit();
    }
    return 0;
}
 *****************************/

#ifdef __cplusplus
}
#endif

#endif
