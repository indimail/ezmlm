/*
 * $Id: ezmlm-sub.c,v 1.1 2025-01-22 11:21:30+05:30 Cprogrammer Exp mbhangui $
 */
#include <unistd.h>
#include "strerr.h"
#include "subscribe.h"
#include "log.h"

#define FATAL "ezmlm-sub: fatal: "
#define WARNING "ezmlm-sub: warning: "

int
main(int argc, char **argv)
{
	char           *dir;
	char           *addr;

	dir = argv[1];
	if (!dir)
		strerr_die1x(100, "ezmlm-sub: usage: ezmlm-sub dir box@domain ...");
	if (chdir(dir) == -1)
		strerr_die4sys(111, FATAL, "unable to switch to ", dir, ": ");

	argv += 2;
	while ((addr = *argv++))
		switch (subscribe(addr, 1)) {
		case -1:
			strerr_die1(111, FATAL, &subscribe_err);
		case -2:
			strerr_warn4(WARNING, "cannot subscribe ", addr, ": ", &subscribe_err);
			break;
		case 1:
			ezlog("+manual", addr);
		}
	_exit(0);
}

/*
 * $Log: ezmlm-sub.c,v $
 * Revision 1.1  2025-01-22 11:21:30+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
