/* Modified for VM/370 CMS and GCC by Robert O'Hara, July 2010. */
/*
 * $Id: stack.h,v 1.4 2008/07/15 07:40:07 bnv Exp $
 * $Log: stack.h,v $
 * Revision 1.4  2008/07/15 07:40:07  bnv
 * MVS, CMS support
 *
 * Revision 1.3  2002/06/11 12:37:56  bnv
 * Added: CDECL
 *
 * Revision 1.2  2001/06/25 18:52:04  bnv
 * Header -> Id
 *
 * Revision 1.1  1998/07/02 17:35:50  bnv
 * Initial revision
 *
 */

#ifndef __STACK_H__
#define __STACK_H__

#include "lstring.h"
#include "dqueue.h"

/* ---- function prototypes ---- */
#ifndef __CMS__

void __CDECL CreateStack(void);

void __CDECL DeleteStack(void);

#endif

void __CDECL Queue2Stack(PLstr str);

void __CDECL Push2Stack(PLstr str);

PLstr __CDECL PullFromStack(void);

long __CDECL StackQueued(void);

#endif
