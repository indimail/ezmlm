/*
 * $Id: getconf.h,v 1.1 2025-01-22 11:21:31+05:30 Cprogrammer Exp mbhangui $
 */
#ifndef GETCONF_H
#define GETCONF_H

extern int      getconf(stralloc *, char *, int, char *, char *);
extern int      getconf_line(stralloc *, char *, int, char *, char *);

#endif

/*
 * $Log: getconf.h,v $
 * Revision 1.1  2025-01-22 11:21:31+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
