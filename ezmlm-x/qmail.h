/*
 * $Id: qmail.h,v 1.1 2025-01-22 11:21:31+05:30 Cprogrammer Exp mbhangui $
 */
#ifndef QMAIL_H
#define QMAIL_H

#include <sys/types.h>
#include "substdio.h"

#define CUSTOM_ERR_FD 2
struct qmail {
	int             flagerr;
	unsigned long   pid;
	int             fdm;
	int             fde;
	int             fdc;
	substdio        ss;
	char            buf[1024];
};

extern int      qmail_open(struct qmail *);
extern void     qmail_put(struct qmail *, const char *, size_t);
extern void     qmail_puts(struct qmail *, const char *);
extern void     qmail_from(struct qmail *, const char *);
extern void     qmail_to(struct qmail *, const char *);
extern void     qmail_fail(struct qmail *);
extern int      qmail_close(struct qmail *);
extern unsigned long qmail_qp(struct qmail *);

#define QMAIL_WAITPID -2
#define QMAIL_CRASHED -3
#define QMAIL_USAGE -4
#define QMAIL_BUG -5
#define QMAIL_SYS -6
#define QMAIL_READ -7
#define QMAIL_WRITE -8
#define QMAIL_NOMEM -9
#define QMAIL_EXECSOFT -11
#define QMAIL_TIMEOUT -13
#define QMAIL_TOOLONG -14

#endif

/*
 * $Log: qmail.h,v $
 * Revision 1.1  2025-01-22 11:21:31+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
