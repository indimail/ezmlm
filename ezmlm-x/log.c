/*
 * $Id: log.c,v 1.1 2025-01-22 11:21:30+05:30 Cprogrammer Exp mbhangui $
 */
#include <unistd.h>
#include "substdio.h"
#include "stralloc.h"
#include "log.h"
#include "now.h"
#include "fmt.h"
#include "open.h"

static substdio ss;
static char     buf[1];
static char     num[FMT_ULONG];
static stralloc line = { 0 };

void
ezlog(char *event, char *addr)
{
	char            ch;
	int             fd;

	if (!stralloc_copyb(&line, num, fmt_ulong(num, (unsigned long) now())))
		return;
	if (!stralloc_cats(&line, " "))
		return;
	if (!stralloc_cats(&line, event))
		return;
	if (!stralloc_cats(&line, " "))
		return;
	while ((ch = *addr++)) {
		if ((ch < 33) || (ch > 126))
			ch = '?';
		if (!stralloc_append(&line, &ch))
			return;
	}
	if (!stralloc_cats(&line, "\n"))
		return;

	if ((fd = open_append("Log")) == -1)
		return;
	substdio_fdbuf(&ss, (ssize_t(*)(int, char *, size_t)) write, fd, buf, sizeof (buf));
	substdio_putflush(&ss, line.s, line.len);
	close(fd);
	return;
}

/*
 * $Log: log.c,v $
 * Revision 1.1  2025-01-22 11:21:30+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
