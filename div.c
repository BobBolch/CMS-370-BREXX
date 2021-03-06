/*
 * $Id: div.c,v 1.4 2008/07/15 07:40:54 bnv Exp $
 * $Log: div.c,v $
 * Revision 1.4  2008/07/15 07:40:54  bnv
 * #include changed from <> to ""
 *
 * Revision 1.3  2002/06/11 12:37:15  bnv
 * Added: CDECL
 *
 * Revision 1.2  2001/06/25 18:49:48  bnv
 * Header changed to Id
 *
 * Revision 1.1  1998/07/02 17:18:00  bnv
 * Initial Version
 *
 */

#include <cmssys.h>
#include "lerror.h"
#include "lstring.h"

/* ------------------- Ldiv ----------------- */
void __CDECL
Ldiv(const PLstr to, const PLstr A, const PLstr B) {
    double b;
    Context *context = (Context *) CMSGetPG();

    b = Lrdreal(B);
    if (b == 0) (context->lstring_Lerror)(ERR_ARITH_OVERFLOW, 0);
    LREAL(*to) = Lrdreal(A) / b;
    LTYPE(*to) = LREAL_TY;
    LLEN(*to) = sizeof(double);
} /* Ldiv */
