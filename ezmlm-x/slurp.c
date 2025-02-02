/*
 * $Id: slurp.c,v 1.1 2025-01-22 11:21:29+05:30 Cprogrammer Exp mbhangui $
 */
#include <sys/types.h>
#include "slurpclose.h"
#include "stralloc.h"
#include "slurp.h"
#include "error.h"
#include "open.h"

int
slurp(char *fn, stralloc *sa, size_t bufsize)
{
	int             fd;
	fd = open_read(fn);
	if (fd == -1) {
		if (errno == error_noent)
			return 0;
		return -1;
	}
	if (slurpclose(fd, sa, bufsize) == -1)
		return -1;
	return 1;
}

/*
 * $Log: slurp.c,v $
 * Revision 1.1  2025-01-22 11:21:29+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
