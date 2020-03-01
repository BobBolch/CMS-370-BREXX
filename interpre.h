/*
 * $Id: interpre.h,v 1.5 2008/07/15 14:54:05 bnv Exp $
 * $Log: interpre.h,v $
 * Revision 1.5  2008/07/15 14:54:05  bnv
 * #include correction
 *
 * Revision 1.4  2002/06/11 12:37:56  bnv
 * Added: CDECL
 *
 * Revision 1.3  2001/06/25 18:52:04  bnv
 * Header -> Id
 *
 * Revision 1.2  1999/11/29 14:58:00  bnv
 * Changed: Some defines
 *
 * Revision 1.1  1998/07/02 17:35:50  bnv
 * Initial revision
 *
 */

#include "lstring.h"
#include "bintree.h"

#ifndef __INTERPRET_H__
#define __INTERPRET_H__

#ifndef ALIGN
# define CIPTYPE byte
#else
# define CIPTYPE dword
#endif

/* -------- variables definitions ------ */
#include "context.h"

/* -------- function prototypes ------- */
void __CDECL RxInitInterpret( void );
void __CDECL RxDoneInterpret( void );
int __CDECL RxInterpret( void );

#endif
