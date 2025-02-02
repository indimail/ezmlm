/*
 * $Id: ezmlm-warn.c,v 1.1 2025-01-22 11:21:28+05:30 Cprogrammer Exp mbhangui $
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "direntry.h"
#include "open.h"
#include "lock.h"
#include "scan.h"
#include "issub.h"
#include "quote.h"
#include "getln.h"
#include "substdio.h"
#include "stralloc.h"
#include "slurp.h"
#include "getconf.h"
#include "byte.h"
#include "error.h"
#include "str.h"
#include "strerr.h"
#include "sig.h"
#include "now.h"
#include "datetime.h"
#include "date822fmt.h"
#include "fmt.h"
#include "cookie.h"
#include "qmail.h"

#define FATAL "ezmlm-warn: fatal: "

ssize_t         qqwrite(int, const char *, size_t);

stralloc        key, outhost, outlocal, mailinglist, fn, addr, fnhash,
				quoted, line;
char            inbuf[1024], textbuf[1024], qqbuf[1], strnum[FMT_ULONG],
				hash[COOKIE], date[DATE822FMT];
struct stat     st;
unsigned long   when;
char           *dir;
substdio        ssin, sstext;
substdio        ssqq = SUBSTDIO_FDBUF((ssize_t(*)(int, char *, size_t)) qqwrite, -1, qqbuf, sizeof (qqbuf));
struct qmail    qq;
struct datetime dt;

void
die_usage()
{
	strerr_die1x(100, "ezmlm-warn: usage: ezmlm-warn dir");
}

void
die_nomem()
{
	strerr_die2x(111, FATAL, "out of memory");
}

void
die_read()
{
	strerr_die6sys(111, FATAL, "unable to read ", dir, "/", fn.s, ": ");
}

ssize_t
qqwrite(int fd, const char *buf, size_t len)
{
	qmail_put(&qq, buf, len);
	return len;
}

void
copy(char *filename)
{
	int             fd;
	int             match;

	fd = open_read(filename);
	if (fd == -1)
		strerr_die4sys(111, FATAL, "unable to open ", filename, ": ");

	substdio_fdbuf(&sstext, (ssize_t(*)(int, char *, size_t)) read, fd, textbuf, sizeof (textbuf));
	for (;;) {
		if (getln(&sstext, &line, &match, '\n') == -1)
			strerr_die4sys(111, FATAL, "unable to read ", filename, ": ");
		if (!match)
			break;
		qmail_put(&qq, line.s, line.len);
	}

	close(fd);
}

void
doit(int flagw)
{
	int             i;
	int             fd;
	int             match;
	int             fdhash;
	datetime_sec    msgwhen;

	fd = open_read(fn.s);
	if (fd == -1)
		die_read();
	substdio_fdbuf(&ssin, (ssize_t(*)(int, char *, size_t)) read, fd, inbuf, sizeof (inbuf));

	if (getln(&ssin, &addr, &match, '\0') == -1)
		die_read();
	if (!match) {
		close(fd);
		return;
	}

	if (!issub(addr.s)) {
		close(fd);
		/*XXX*/ unlink(fn.s);
		return;
	}

	cookie(hash, "", 0, "", addr.s, "");
	if (!stralloc_copys(&fnhash, "bounce/h"))
		die_nomem();
	if (!stralloc_catb(&fnhash, hash, COOKIE))
		die_nomem();
	if (!stralloc_0(&fnhash))
		die_nomem();

	if (qmail_open(&qq) == -1)
		strerr_die2sys(111, FATAL, "unable to run qmail-queue: ");

	msgwhen = now();
	qmail_puts(&qq, "Mailing-List: ");
	qmail_put(&qq, mailinglist.s, mailinglist.len);
	qmail_puts(&qq, "\nDate: ");
	datetime_tai(&dt, msgwhen);
	qmail_put(&qq, date, date822fmt(date, &dt));
	qmail_puts(&qq, "Message-ID: <");
	qmail_put(&qq, strnum, fmt_ulong(strnum, (unsigned long) msgwhen));
	qmail_puts(&qq, ".");
	qmail_put(&qq, strnum, fmt_ulong(strnum, (unsigned long) getpid()));
	qmail_puts(&qq, ".ezmlm-warn@");
	qmail_put(&qq, outhost.s, outhost.len);
	qmail_puts(&qq, ">\nFrom: ");
	if (!quote(&quoted, &outlocal))
		die_nomem();
	qmail_put(&qq, quoted.s, quoted.len);
	qmail_puts(&qq, "-help@");
	qmail_put(&qq, outhost.s, outhost.len);
	qmail_puts(&qq, "\nTo: ");
	if (!quote2(&quoted, addr.s))
		die_nomem();
	qmail_put(&qq, quoted.s, quoted.len);
	qmail_puts(&qq, flagw ? "\nSubject: ezmlm probe\n\n" : "\nSubject: ezmlm warning\n\n");

	copy("text/top");
	copy(flagw ? "text/bounce-probe" : "text/bounce-warn");

	if (!flagw) {
		fdhash = open_read(fnhash.s);
		if (fdhash == -1) {
			if (errno != error_noent)
				strerr_die6sys(111, FATAL, "unable to open ", dir, "/", fnhash.s, ": ");
		} else {
			copy("text/bounce-num");
			substdio_fdbuf(&sstext, (ssize_t(*)(int, char *, size_t)) read, fdhash, textbuf, sizeof (textbuf));
			if (substdio_copy(&ssqq, &sstext) < 0)
				strerr_die6sys(111, FATAL, "unable to read ", dir, "/", fnhash.s, ": ");
			close(fdhash);
		}
	}

	copy("text/bounce-bottom");
	if (substdio_copy(&ssqq, &ssin) < 0)
		die_read();
	close(fd);

	strnum[fmt_ulong(strnum, when)] = 0;
	cookie(hash, key.s, key.len, strnum, addr.s, flagw ? "P" : "W");
	if (!stralloc_copy(&line, &outlocal))
		die_nomem();
	if (!stralloc_cats(&line, flagw ? "-return-probe-" : "-return-warn-"))
		die_nomem();
	if (!stralloc_cats(&line, strnum))
		die_nomem();
	if (!stralloc_cats(&line, "."))
		die_nomem();
	if (!stralloc_catb(&line, hash, COOKIE))
		die_nomem();
	if (!stralloc_cats(&line, "-"))
		die_nomem();
	i = str_chr(addr.s, '@');
	if (!stralloc_catb(&line, addr.s, i))
		die_nomem();
	if (addr.s[i]) {
		if (!stralloc_cats(&line, "="))
			die_nomem();
		if (!stralloc_cats(&line, addr.s + i + 1))
			die_nomem();
	}
	if (!stralloc_cats(&line, "@"))
		die_nomem();
	if (!stralloc_cat(&line, &outhost))
		die_nomem();
	if (!stralloc_0(&line))
		die_nomem();
	qmail_from(&qq, line.s);

	qmail_to(&qq, addr.s);
	if (qmail_close(&qq) != 0)
		strerr_die2x(111, FATAL, "temporary qmail-queue error");

	strnum[fmt_ulong(strnum, qmail_qp(&qq))] = 0;
	strerr_warn2("ezmlm-warn: info: qp ", strnum, 0);

	if (!flagw) {
		if (unlink(fnhash.s) == -1)
			if (errno != error_noent)
				strerr_die6sys(111, FATAL, "unable to remove ", dir, "/", fnhash.s, ": ");
	}
	if (unlink(fn.s) == -1)
		strerr_die6sys(111, FATAL, "unable to remove ", dir, "/", fn.s, ": ");
}

int
main(int argc, char **argv)
{
	DIR            *bouncedir;
	direntry       *d;
	unsigned long   bouncedate;
	int             fdlock;

	umask(022);
	sig_pipeignore();
	when = (unsigned long) now();

	dir = argv[1];
	if (!dir)
		die_usage();

	if (chdir(dir) == -1)
		strerr_die4sys(111, FATAL, "unable to switch to ", dir, ": ");

	switch (slurp("key", &key, 32)) {
	case -1:
		strerr_die4sys(111, FATAL, "unable to read ", dir, "/key: ");
	case 0:
		strerr_die3x(100, FATAL, dir, "/key does not exist");
	}
	getconf_line(&outhost, "outhost", 1, FATAL, dir);
	getconf_line(&outlocal, "outlocal", 1, FATAL, dir);
	getconf_line(&mailinglist, "mailinglist", 1, FATAL, dir);

	fdlock = open_append("lockbounce");
	if (fdlock == -1)
		strerr_die4sys(111, FATAL, "unable to open ", dir, "/lockbounce: ");
	if (lock_ex(fdlock) == -1)
		strerr_die4sys(111, FATAL, "unable to lock ", dir, "/lockbounce: ");

	bouncedir = opendir("bounce");
	if (!bouncedir)
		strerr_die4sys(111, FATAL, "unable to open ", dir, "/bounce: ");

	while ((d = readdir(bouncedir))) {
		if (str_equal(d->d_name, "."))
			continue;
		if (str_equal(d->d_name, ".."))
			continue;

		if (!stralloc_copys(&fn, "bounce/"))
			die_nomem();
		if (!stralloc_cats(&fn, d->d_name))
			die_nomem();
		if (!stralloc_0(&fn))
			die_nomem();

		if (stat(fn.s, &st) == -1) {
			if (errno == error_noent)
				continue;
			strerr_die6sys(111, FATAL, "unable to stat ", dir, "/", fn.s, ": ");
		}

		if (when > st.st_mtime + 3000000)
			if (unlink(fn.s) == -1)
				strerr_die6sys(111, FATAL, "unable to remove ", dir, "/", fn.s, ": ");

		if ((d->d_name[0] == 'd') || (d->d_name[0] == 'w')) {
			scan_ulong(d->d_name + 1, &bouncedate);
			if (when > bouncedate + 1000000)
				doit(d->d_name[0] == 'w');
		}
	}

	closedir(bouncedir);

	_exit(0);
}

/*
 * $Log: ezmlm-warn.c,v $
 * Revision 1.1  2025-01-22 11:21:28+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
