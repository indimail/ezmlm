/*
 * $Id: surfpcs.h,v 1.1 2025-01-22 11:21:31+05:30 Cprogrammer Exp mbhangui $
 */
#ifndef SURFPCS_H
#define SURFPCS_H

#include "surf.h"
#include "uint32.h"

typedef struct {
	uint32          seed[32];
	uint32          sum[8];
	uint32          out[8];
	uint32          in[12];
	int             todo;
} surfpcs;

#define SURFPCS_LEN 32

extern void     surfpcs_init(surfpcs *s, uint32 k[32]);
extern void     surfpcs_add(surfpcs *s, unsigned char *x, unsigned int n);
extern void     surfpcs_out(surfpcs *s, unsigned char h[32]);

#endif

/*
 * $Log: surfpcs.h,v $
 * Revision 1.1  2025-01-22 11:21:31+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
