/**
 * Copyright (c) 2016 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file llist.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2016/06/20
 * @license
 * @description
 */

#ifndef __llist_H_wx6xkQRJ_lqol_Hq8w_sveL_u0Oi6Z8lMaDL__
#define __llist_H_wx6xkQRJ_lqol_Hq8w_sveL_u0Oi6Z8lMaDL__

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
//=============================================================================
//                  Constant Definition
//=============================================================================
/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)
//=============================================================================
//                  Macro Definition
//=============================================================================
/**
 * Get offset of a member
 */
#define OFFSET_OF(TYPE, MEMBER)      ((size_t) &((TYPE*)0)->MEMBER)

/**
 * Casts a member of a structure out to the containing structure
 * @param ptr        the pointer to the member.
 * @param TYPE       the type of the container struct this is embedded in.
 * @param MEMBER     the name of the member within the struct.
 *
 */
#define CONTAINER_OF(TYPE, MEMBER, ptr)     \
    (TYPE*)((size_t)(ptr) - OFFSET_OF(TYPE, MEMBER))


/**
 *  declare the first node
 */
#define DECLARE_LIST_HEAD(var) \
	static struct llist var = {.next = &var, .prev = &var}


/**
 *  init node
 */
#define LLIST_NODE_INIT(pllst)   \
    do{ (pllst)->next = (pllst)->prev = (pllst); }while(0)


/**
 *  new node after act_list
 */
#define LLIST_NODE_ADD_AFTER(pllst, pNewNode)      \
    do{ (pllst)->next->prev = (pNewNode);          \
        (pNewNode)->next = (pllst)->next;          \
        (pllst)->next  = pNewNode;                 \
        (pNewNode)->prev = (pllst);                \
    }while(0)


/**
 *  new node before act_list
 */
#define LLIST_NODE_ADD_BEFORE(pllst, pNewNode)     \
    do{ (pllst)->prev->next = (pNewNode);          \
        (pNewNode)->prev = (pllst)->prev;          \
        (pllst)->prev  = (pNewNode);               \
        (pNewNode)->next = (pllst);                \
    }while(0)

/**
 *  check list is empty or not
 */
#define LLIST_IS_EMPTY(pllst)                       ((pllst)->next == pllst)

/**
 *  deletes node from list.
 *  @pllst: the element to delete from the list.
 *  Note: list_empty on entry does not return true after this, the entry is
 *        in an undefined state.
 */
#define LLIST_NODE_DEL(pllst)                      \
    do{ (pllst)->next->prev = (pllst)->prev;       \
        (pllst)->prev->next = (pllst)->next;       \
        (pllst)->prev = LIST_POISON2;              \
        (pllst)->next = LIST_POISON1;              \
    }while(0)

/**
 * iterate over a list
 * @pllstCur:	the &struct list_head to use as a loop counter.
 * @pllstLst:	the head for your list.
 */
#define LLIST_FOR_EACH(pllstCur, pllstTmp, pllstLst)            \
  for( pllstCur = (pllstLst)->next, pllstTmp = pllstCur->next;  \
       pllstCur != (pllstLst);	                                \
       pllstCur = pllstTmp, pllstTmp = pllstCur->next )         \
//=============================================================================
//                  Structure Definition
//=============================================================================
/**
 * Simple doubly linked list implementation.
 */
typedef struct llist {
	struct llist    *next, *prev;
} llist_t;
//=============================================================================
//                  Global Data Definition
//=============================================================================

//=============================================================================
//                  Private Function Definition
//=============================================================================

//=============================================================================
//                  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
