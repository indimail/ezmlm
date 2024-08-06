#include <unistd.h>
#include "substdio.h"
#include "wait.h"
#include "env.h"
#include "scan.h"
#include "fd.h"
#include "stralloc.h"
#include "envdir.h"
#include "pathexec.h"
#include "error.h"
#include "qmail.h"
#include "auto_qmail.h"
#include "auto_sysconfdir.h"

static stralloc tmp = { 0 };

int qmail_open(struct qmail *qq)
{
  int pim[2], pie[2];
  int pic[2], i, unreadable = 0, errfd; /*- custom message */
  const char *x, *err;
  char *binqqargs[2] = { 0, 0 };
  char **e, **orig_env;
  char errbuf[256];
  substdio sserr;

  if (pipe(pim) == -1) return -1;
  if (pipe(pie) == -1) { close(pim[0]); close(pim[1]); return -1; }
  if (pipe(pic) == -1) { close(pim[0]); close(pim[1]); close(pie[0]); close(pie[1]); return -1; }
 
  switch(qq->pid = vfork()) {
    case -1:
      close(pim[0]); close(pim[1]);
      close(pie[0]); close(pie[1]);
      close(pic[0]); close(pic[1]);
      return -1;
    case 0:
      close(pim[1]);
      close(pie[1]);
      close(pic[0]); /*- we want to receive data */
      if (fd_move(0,pim[0]) == -1) _exit(120);
      if (fd_move(1,pie[0]) == -1) _exit(120);
      if (!(x = env_get("ERROR_FD"))) errfd = CUSTOM_ERR_FD;
      else scan_int(x, &errfd);
      if (fd_move(errfd, pic[1]) == -1) _exit(120);
      if (chdir(auto_qmail) == -1) _exit(120);
      if (!stralloc_copys(&tmp, auto_sysconfdir) ||
          !stralloc_catb(&tmp, "/global_vars", 12) ||
          !stralloc_0(&tmp))
        _exit(51);
      if (!access(tmp.s, X_OK)) {
		orig_env = environ;
        env_clear();
        if ((i = envdir((char *) tmp.s, &err, 1, &unreadable))) {
          substdio_fdbuf(&sserr,write,errfd,errbuf,sizeof(errbuf));
          substdio_put(&sserr, "Zenvdir: ", 9);
          substdio_puts(&sserr, envdir_str(i));
		  substdio_put(&sserr, ": ", 2);
		  substdio_puts(&sserr, err);
          substdio_put(&sserr, " (#4.3.0)", 9);
          substdio_flush(&sserr);
          _exit(88);
        }
        if ((e = pathexec(0))) environ = e;
        else environ = orig_env;
      } else
      if (errno != error_noent)
          _exit(55);
      if (!binqqargs[0]) binqqargs[0] = env_get("QMAILQUEUE");
      if (!binqqargs[0]) binqqargs[0] = "sbin/ezmlm-queue";
      execv(*binqqargs,binqqargs);
      _exit(120);
  }

  qq->fdm = pim[1]; close(pim[0]);
  qq->fde = pie[1]; close(pie[0]);
  qq->fdc = pic[0]; close(pic[1]);
  substdio_fdbuf(&qq->ss,write,qq->fdm,qq->buf,sizeof(qq->buf));
  qq->flagerr = 0;
  return 0;
}

unsigned long qmail_qp(qq) struct qmail *qq;
{
  return qq->pid;
}

void qmail_fail(qq) struct qmail *qq;
{
  qq->flagerr = 1;
}

void qmail_put(qq,s,len) struct qmail *qq; char *s; int len;
{
  if (!qq->flagerr) if (substdio_put(&qq->ss,s,len) == -1) qq->flagerr = 1;
}

void qmail_puts(qq,s) struct qmail *qq; char *s;
{
  if (!qq->flagerr) if (substdio_puts(&qq->ss,s) == -1) qq->flagerr = 1;
}

void qmail_from(qq,s) struct qmail *qq; char *s;
{
  if (substdio_flush(&qq->ss) == -1) qq->flagerr = 1;
  close(qq->fdm);
  substdio_fdbuf(&qq->ss,write,qq->fde,qq->buf,sizeof(qq->buf));
  qmail_put(qq,"F",1);
  qmail_puts(qq,s);
  qmail_put(qq,"",1);
}

void qmail_to(qq,s) struct qmail *qq; char *s;
{
  qmail_put(qq,"T",1);
  qmail_puts(qq,s);
  qmail_put(qq,"",1);
}

int qmail_close(qq)
struct qmail *qq;
{
  int wstat;

  qmail_put(qq,"",1);
  if (!qq->flagerr) if (substdio_flush(&qq->ss) == -1) qq->flagerr = 1;
  close(qq->fde);

  if (wait_pid(&wstat,qq->pid) != qq->pid) return QMAIL_WAITPID;
  if (wait_crashed(wstat)) return QMAIL_CRASHED;
  switch(wait_exitcode(wstat)) {
    case 0: if (qq->flagerr) return QMAIL_BUG; return 0;
    case 112: return QMAIL_USAGE;
    case 115: return QMAIL_TOOLONG;
    case 103: case 104: case 105: case 106: case 108: return QMAIL_SYS;
    case 121: return QMAIL_READ;
    case 122: return QMAIL_WRITE;
    case 123: return QMAIL_NOMEM;
    case 124: return QMAIL_TIMEOUT;
    case 120: return QMAIL_EXECSOFT;
    default: /* 101 or 102 */ return QMAIL_BUG;
  }
}
