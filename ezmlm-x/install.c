#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "substdio.h"
#include "scan.h"
#include "stralloc.h"
#include "getln.h"
#include "open.h"
#include "error.h"
#include "strerr.h"
#include "byte.h"

stralloc        target = { 0 };

char           *to;

#define FATAL "install: fatal: "

void
nomem()
{
	strerr_die2x(111, FATAL, "out of memory");
}

char            inbuf[SUBSTDIO_INSIZE];
char            outbuf[SUBSTDIO_OUTSIZE];
substdio        ssin;
substdio        ssout;

void
doit(line)
	stralloc       *line;
{
	unsigned int    xlen, i;
	char           *x, *type, *uidstr, *gidstr, *modestr, *mid, *name;
	unsigned long   uid, gid, mode;
	int             fdin, fdout;

	x = line->s;
	xlen = line->len;

	type = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	uidstr = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	gidstr = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	modestr = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	mid = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	name = x;
	if ((i = byte_chr(x, xlen, ':')) == xlen)
		return;
	x[i++] = 0;
	x += i;
	xlen -= i;

	if (!stralloc_copys(&target, to) || !stralloc_cats(&target, mid) || !stralloc_cats(&target, name) || !stralloc_0(&target))
		nomem();

	uid = -1;
	if (*uidstr)
		scan_ulong(uidstr, &uid);
	gid = -1;
	if (*gidstr)
		scan_ulong(gidstr, &gid);
	scan_8long(modestr, &mode);

	switch (*type)
	{
	case 'd':
		if (mkdir(target.s, 0700) == -1)
			if (errno != error_exist)
				strerr_die4sys(111, FATAL, "unable to mkdir ", target.s, ": ");
		break;

	case 'p':
		if (mkfifo(target.s, 0700) == -1)
			if (errno != error_exist)
				strerr_die4sys(111, FATAL, "unable to create fifo ", target.s, ": ");

	case 'c':
		fdin = open_read(name);
		if (fdin == -1)
			strerr_die4sys(111, FATAL, "unable to read ", name, ": ");
		substdio_fdbuf(&ssin, read, fdin, inbuf, sizeof (inbuf));

		if ((fdout = open_trunc(target.s)) == -1)
			strerr_die4sys(111, FATAL, "unable to write ", target.s, ": ");
		substdio_fdbuf(&ssout, write, fdout, outbuf, sizeof (outbuf));

		switch (substdio_copy(&ssout, &ssin))
		{
		case -2:
			strerr_die4sys(111, FATAL, "unable to read ", name, ": ");
		case -3:
			strerr_die4sys(111, FATAL, "unable to write ", target.s, ": ");
		}

		close(fdin);
		if (substdio_flush(&ssout) == -1)
			strerr_die4sys(111, FATAL, "unable to write ", target.s, ": ");
		if (fsync(fdout) == -1)
			strerr_die4sys(111, FATAL, "unable to write ", target.s, ": ");
		close(fdout);
		break;

	default:
		return;
	}

	if (chown(target.s, uid, gid) == -1)
		strerr_die4sys(111, FATAL, "unable to chown ", target.s, ": ");
	if (chmod(target.s, mode) == -1)
		strerr_die4sys(111, FATAL, "unable to chmod ", target.s, ": ");
}

char            buf[256];
substdio        in = SUBSTDIO_FDBUF(read, 0, buf, sizeof (buf));
stralloc        line = { 0 };

int
main(int argc, char **argv)
{
	int             match;

	umask(077);

	if (!(to = argv[1]))
		strerr_die2x(100, FATAL, "install: usage: install dir");

	for (;;) {
		if (getln(&in, &line, &match, '\n') == -1)
			strerr_die2sys(111, FATAL, "unable to read input: ");
		doit(&line);
		if (!match)
			_exit(0);
	}
	return 0;
}
