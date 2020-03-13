/*
 * $Id: main.c,v 1.14 2009/06/30 13:51:40 bnv Exp bnv $
 * $Log: main.c,v $
 * Revision 1.14  2009/06/30 13:51:40  bnv
 * Added -a option to break arg into words
 *
 * Revision 1.13  2008/07/15 14:57:55  bnv
 * mvs corrections
 *
 * Revision 1.12  2008/07/15 07:40:25  bnv
 * #include changed from <> to ""
 *
 * Revision 1.11  2006/01/26 10:25:35  bnv
 * Changed: conio
 *
 * Revision 1.10  2003/10/30 13:16:28  bnv
 * Variable name change
 *
 * Revision 1.9  2002/08/22 12:27:09  bnv
 * Added: Unix initialisation commands
 *
 * Revision 1.8  2002/07/03 13:15:08  bnv
 * Changed: Version define
 *
 * Revision 1.7  2002/06/11 12:37:38  bnv
 * Added: CDECL
 *
 * Revision 1.6  2002/06/06 08:24:02  bnv
 * Corrected: READLINE support when using redirected input
 *
 * Revision 1.5  2001/06/25 18:51:48  bnv
 * Header -> Id
 *
 * Revision 1.4  1999/11/26 13:13:47  bnv
 * Added: A filter option.
 *
 * Revision 1.3  1999/05/26 16:48:06  bnv
 * Gene corrections in RXCONIO
 *
 * Revision 1.2  1999/02/10 15:43:16  bnv
 * Additions from Generoso Martello
 *
 * Revision 1.1  1998/07/02 17:34:50  bnv
 * Initial revision
 *
 */
#include <stdio.h>
#include <string.h>

#include "lstring.h"
#include "rexx.h"
#include "rxdefs.h"
#if !defined(__CMS__) && !defined(__MVS__)
# include <sys/stat.h>
#endif
#include "context.h"

/* ------- Includes for any other external library ------- */
#ifdef RXCONIO
extern void __CDECL RxConIOInitialize();
#endif

#ifdef RXMYSQLSTATIC
# include "rxmysql.c"
#endif

/* --------------------- main ---------------------- */
int __CDECL
main(int ac, char *av[])
{
 Lstr args[MAXARGS], tracestr, file;
 int ia,ir,iaa;
 bool input, loop_over_stdin, parse_args;
 int returnCode;
#if defined(HAVE_READLINE)
 Lstr line;
 LINITSTR(line);
#endif
 PushContext();

 /* Interim code for f0006 - Print arguments */
 {
   int i;
   for (i=0; i<ac; i++) {
     printf("Arg %d is (%s)\n", i, av[i]);
   }
 }
 input = loop_over_stdin = parse_args = FALSE;
 for (ia=0; ia<MAXARGS; ia++) LINITSTR(args[ia]);
 LINITSTR(tracestr);
 LINITSTR(file);

 if (ac<2) {
#ifndef __CMS__
  puts("\nsyntax: rexx [-[trace]|-F|-a] <filename> <args>...\n");
  puts("options:");
  puts("\t-\tto use stdin");
  puts("\t-a\tbreak words into multiple arguments");
  puts("\t-F\tloop over standard input");
                puts("\t\t\'linein\' contains each line from stdin.\n");
  puts(VERSIONSTR);
  puts("Author: "AUTHOR);
  puts("Please report bugs, errors or comments to the above address.\n");
#endif
  PopContext();
  return 0;
 }
#ifdef __DEBUG__
 __debug__ = FALSE;
#endif

 /* --- Initialise --- */
 RxInitialize(av[0]);

 /* --- Register functions of external libraries --- */
#ifdef RXMYSQLSTATIC
 RxMySQLInitialize();
#endif
#ifdef RXCONIO
 RxConIOInitialize();
#endif

 /* --- scan arguments --- */
 ia = 1;
 if (av[ia][0]=='-') {
  if (av[ia][1]==0)
   input = TRUE;
  else
  if (av[ia][1]=='F')
   loop_over_stdin = input = TRUE;
  else
  if (av[ia][1]=='a')
   parse_args = TRUE;
  else
   Lscpy(&tracestr,av[ia]+1);
  ia++;
 } else
 if (av[ia][0]=='?' || av[ia][0]=='!') {
  Lscpy(&tracestr,av[ia]);
  ia++;
 }

 /* --- let's read a normal file --- */
 if (!input && ia<ac) {
  /* prepare arguments for program */
  iaa = 0;
  for (ir=ia+1; ir<ac; ir++) {
   if (parse_args) {
    Lscpy(&args[iaa], av[ir]);
    if (++iaa >= MAXARGS) break;
   } else {
    Lcat(&args[0], av[ir]);
    if (ir<ac-1) Lcat(&args[0]," ");
   }
  }
  RxRun(av[ia],NULL,args,&tracestr,NULL);
 } else {
  if (ia>=ac) {
#if !defined(HAVE_READLINE)
   Lread(STDIN,&file,LREADFILE);
#else
   struct stat buf;
   fstat(0,&buf);
   if (S_ISCHR(buf.st_mode)) {
    printf("End with \";\" on a line by itself.\n");
    while (1) {
     Lread(STDIN,&line,LREADLINE);
     if (!Lcmp(&line,";"))
      break;
     Lstrcat(&file,&line);
     Lcat(&file,"\n");
    }
   } else
    Lread(STDIN,&file,LREADFILE);
#endif
  } else {
   /* Copy a small header */
   if (loop_over_stdin)
    Lcat(&file,"do forever;"
     "linein=read();"
     "if eof(0) then exit;");
   for (;ia<ac; ia++) {
    Lcat(&file,av[ia]);
    if (ia<ac-1) Lcat(&file," ");
   }
   /* and a footer */
   if (loop_over_stdin)
    Lcat(&file,";end");
  }
  RxRun(NULL,&file,args,&tracestr,NULL);
 }

 /* --- Free everything --- */
 RxFinalize();
 for (ia=0; ia<MAXARGS; ia++) LFREESTR(args[ia]);
 LFREESTR(tracestr);
 LFREESTR(file);
#if defined(HAVE_READLINE)
 LFREESTR(line);
#endif
#ifdef RXMYSQLSTATIC
 RxMySQLFinalize();
#endif

#ifdef __DEBUG__
 if (mem_allocated()!=0) {
  fprintf(STDERR,"\nMemory left allocated: %ld\n",mem_allocated());
  mem_list();
 }
#endif

 returnCode = rxReturnCode; /* The Global Context will die on the next line ... */
 PopContext();
 return returnCode;
} /* main */
