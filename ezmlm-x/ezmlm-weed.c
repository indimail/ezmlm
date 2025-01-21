/*
 * $ID: $
 */
#include <unistd.h>
#include "stralloc.h"
#include "str.h"
#include "byte.h"
#include "substdio.h"
#include "getln.h"
#include "strerr.h"

#define FATAL "ezmlm-weed: fatal: "

char            buf0[256];
substdio        ss0 = SUBSTDIO_FDBUF((ssize_t(*)(int, char *, size_t)) read, 0, buf0, sizeof (buf0));
stralloc        line, line1, line2, line3, line4, line5, line6, line7, line8;
int             flagmds = 0, flagsw = 0, flagsr = 0, flagas = 0, flagbw = 0;
char            warn1[] = "    **********************************************";
char            warn2[] = "    **      THIS IS A WARNING MESSAGE ONLY      **";
char            warn3[] = "    **  YOU DO NOT NEED TO RESEND YOUR MESSAGE  **";
char            warn4[] = "    **********************************************";

void
get(stralloc *sa)
{
	int             match;
	if (getln(&ss0, sa, &match, '\n') == -1)
		strerr_die2sys(111, FATAL, "unable to read input: ");
	if (!match)
		_exit(0);
}

int
main(int argc, char **argv)
{
	for (;;) {
		get(&line);
		if (line.len == 1)
			break;

		if (stralloc_starts(&line, "Subject: success notice"))
			_exit(99);
		if (stralloc_starts(&line, "Subject: deferral notice"))
			_exit(99);

		if (stralloc_starts(&line, "From: Mail Delivery Subsystem <MAILER-DAEMON@"))
			flagmds = 1;
		if (stralloc_starts(&line, "Subject: Warning: could not send message"))
			flagsw = 1;
		if (stralloc_starts(&line, "Subject: Returned mail: warning: cannot send message"))
			flagsr = 1;
		if (stralloc_starts(&line, "Auto-Submitted: auto-generated (warning"))
			flagas = 1;
	}

	get(&line1);
	get(&line2);
	get(&line3);
	get(&line4);
	get(&line5);
	get(&line6);
	get(&line7);
	get(&line8);

	if (stralloc_starts(&line1, "This is a MIME-encapsulated message"))
		if (stralloc_starts(&line3, "--"))
			if (stralloc_starts(&line5, warn1))
				if (stralloc_starts(&line6, warn2))
					if (stralloc_starts(&line7, warn3))
						if (stralloc_starts(&line8, warn4))
							flagbw = 1;

	if (stralloc_starts(&line1, warn1))
		if (stralloc_starts(&line2, warn2))
			if (stralloc_starts(&line3, warn3))
				if (stralloc_starts(&line4, warn4))
					flagbw = 1;

	if (flagmds && flagsw && flagas && flagbw)
		_exit(99);
	if (flagmds && flagsr && flagbw)
		_exit(99);

	_exit(0);
}

/*
 * $Log: $
 */
