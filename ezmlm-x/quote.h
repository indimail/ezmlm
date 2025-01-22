/*
 * $Id: quote.h,v 1.1 2025-01-22 11:21:31+05:30 Cprogrammer Exp mbhangui $
 */
#ifndef QUOTE_H
#define QUOTE_H
#include <stralloc.h>

extern int      quote_need(char *, unsigned int);
extern int      quote(stralloc *, stralloc *);
extern int      quote2(stralloc *, char *);

#endif

/*
 * $Log: quote.h,v $
 * Revision 1.1  2025-01-22 11:21:31+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
