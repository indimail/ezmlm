# Don't edit Makefile! Use conf-* for configuration.
COFLAGS=-z+05:30
SHELL=/bin/sh
include Makevars.$(SYSTEM)

edit = sed \
	-e 's}@prefix\@}$(prefix)}g' \
	-e 's}@qmaildir\@}$(qmaildir)}g' \
	-e 's}@sysconfdir\@}$(sysconfdir)}g' \
	-e 's}@mandir\@}$(mandir)}g' \
	-e 's,@email\@,$(email),g' \
	-e 's}@version\@}$(version)}g' \
	-e 's|@release\@|$(release)|g'

default: it ezmlm.spec ezmlm.changes PKGBUILD ezmlm-queue.1
all: it

auto-ccld.sh: \
conf-cc conf-ld warn-auto.sh
	( cat warn-auto.sh; \
	echo CC=\'`head -1 conf-cc`\'; \
	echo LD=\'`head -1 conf-ld`\' \
	) > auto-ccld.sh

auto-str: \
load auto-str.o
	./load auto-str -lqmail

auto-str.o: \
compile auto-str.c auto-str.c auto-str.c auto-str.c
	./compile auto-str.c

auto_bin.c: \
auto-str conf-bin
	./auto-str auto_bin `head -1 conf-bin` > auto_bin.c

auto_bin.o: \
compile auto_bin.c
	./compile auto_bin.c

auto_qmail.c: \
auto-str conf-qmail
	./auto-str auto_qmail `head -1 conf-qmail` > auto_qmail.c

auto_qmail.o: \
compile auto_qmail.c
	./compile auto_qmail.c

compile: \
make-compile warn-auto.sh systype
	( cat warn-auto.sh; ./make-compile "`cat systype`" ) > \
	compile
	chmod 755 compile

cookie.o: \
compile cookie.c cookie.h cookie.c cookie.c uint32.h cookie.c \
surfpcs.h uint32.h surfpcs.h cookie.c
	./compile cookie.c

direntry.h: \
compile trydrent.c direntry.h1 direntry.h2
	( ./compile trydrent.c >/dev/null 2>&1 \
	&& cat direntry.h2 || cat direntry.h1 ) > direntry.h
	rm -f trydrent.o

ezmlm-list: \
load ezmlm-list.o 
	./load ezmlm-list -lqmail

ezmlm-list.o: \
compile ezmlm-list.c
	./compile ezmlm-list.c

ezmlm-make: \
load ezmlm-make.o auto_bin.o
	./load ezmlm-make auto_bin.o -lqmail

ezmlm-make.o: \
compile ezmlm-make.c auto_bin.h
	./compile ezmlm-make.c

ezmlm-manage: \
load ezmlm-manage.o auto_qmail.o getconf.o subscribe.o log.o cookie.o \
slurpclose.o slurp.o qmail.o quote.o \
surf.a
	./load ezmlm-manage auto_qmail.o getconf.o subscribe.o \
	log.o cookie.o slurpclose.o \
	slurp.o qmail.o quote.o surf.a -lqmail

ezmlm-manage.o: \
compile slurp.h getconf.h qmail.h qmail.h quote.h subscribe.h \
subscribe.h log.h cookie.h 
	./compile ezmlm-manage.c

ezmlm-reject: \
load ezmlm-reject.o
	./load ezmlm-reject -lqmail

ezmlm-reject.o: \
compile ezmlm-reject.c 
	./compile ezmlm-reject.c

ezmlm-return: \
load ezmlm-return.o quote.o getconf.o issub.o subscribe.o log.o \
slurpclose.o slurp.o cookie.o surf.a 
	./load ezmlm-return quote.o getconf.o issub.o subscribe.o \
	log.o slurpclose.o slurp.o cookie.o surf.a -lqmail

ezmlm-return.o: \
compile ezmlm-return.c slurp.h getconf.h quote.h \
cookie.h subscribe.h log.h issub.h
	./compile ezmlm-return.c

ezmlm-send: \
load ezmlm-send.o auto_qmail.o getconf.o qmail.o slurp.o \
slurpclose.o 
	./load ezmlm-send auto_qmail.o getconf.o qmail.o \
	slurp.o slurpclose.o -lqmail

ezmlm-send.o: \
compile ezmlm-send.c qmail.h getconf.h
	./compile ezmlm-send.c

ezmlm-sub: \
load ezmlm-sub.o subscribe.o log.o
	./load ezmlm-sub subscribe.o log.o -lqmail

ezmlm-sub.o: \
compile ezmlm-sub.c subscribe.h subscribe.h log.h
	./compile ezmlm-sub.c

ezmlm-unsub: \
load ezmlm-unsub.o subscribe.o log.o
	./load ezmlm-unsub subscribe.o log.o -lqmail

ezmlm-unsub.o: \
compile ezmlm-unsub.c subscribe.h subscribe.h log.h
	./compile ezmlm-unsub.c

ezmlm-warn: \
load ezmlm-warn.o auto_qmail.o getconf.o cookie.o issub.o \
slurpclose.o slurp.o quote.o qmail.o surf.a
	./load ezmlm-warn auto_qmail.o getconf.o cookie.o issub.o \
	slurpclose.o slurp.o quote.o qmail.o surf.a -lqmail

ezmlm-warn.o: \
compile ezmlm-warn.c direntry.h slurp.h getconf.h cookie.h qmail.h \
issub.h quote.h 
	./compile ezmlm-warn.c

ezmlm-weed: \
load ezmlm-weed.o
	./load ezmlm-weed -lqmail

ezmlm-weed.o: \
compile ezmlm-weed.c
	./compile ezmlm-weed.c

find-systype: \
find-systype.sh auto-ccld.sh
	cat auto-ccld.sh find-systype.sh > find-systype
	chmod 755 find-systype

getconf.o: \
compile getconf.c getconf.h slurp.h
	./compile getconf.c

install: \
load install.o 
	./load install -lqmail

install.o: \
compile install.c
	./compile install.c

issub.o: \
compile issub.c issub.h uint32.h
	./compile issub.c

it: \
ezmlm-make ezmlm-manage ezmlm-send ezmlm-reject ezmlm-return \
ezmlm-warn ezmlm-weed ezmlm-list ezmlm-sub ezmlm-unsub ezmlm-queue

load: \
make-load warn-auto.sh systype
	( cat warn-auto.sh; ./make-load "`cat systype`" ) > load
	chmod 755 load

log.o: \
compile log.c log.h
	./compile log.c

make-compile: \
make-compile.sh auto-ccld.sh
	cat auto-ccld.sh make-compile.sh > make-compile
	chmod 755 make-compile

make-load: \
make-load.sh auto-ccld.sh
	cat auto-ccld.sh make-load.sh > make-load
	chmod 755 make-load

make-makelib: \
make-makelib.sh auto-ccld.sh
	cat auto-ccld.sh make-makelib.sh > make-makelib
	chmod 755 make-makelib

makelib: \
make-makelib warn-auto.sh systype
	( cat warn-auto.sh; ./make-makelib "`cat systype`" ) > \
	makelib
	chmod 755 makelib

qmail.o: \
compile qmail.c qmail.h
	./compile qmail.c

quote.o: \
compile quote.c quote.h 
	./compile quote.c

setup: \
default install conf-bin
	mkdir -p $(DESTDIR)"`head -n 1 conf-bin`"
	./install $(DESTDIR)"`head -1 conf-bin`" < BIN
	mkdir -p $(DESTDIR)"`head -n 1 conf-sbin`"
	./install $(DESTDIR)"`head -1 conf-sbin`" < SBIN
	mkdir -p $(DESTDIR)$(mandir)
	./install $(DESTDIR)$(mandir) < MAN

slurp.o: \
compile slurp.c slurp.h slurpclose.h
	./compile slurp.c

slurpclose.o: \
compile slurpclose.c slurpclose.h
	./compile slurpclose.c

subscribe.o: \
compile subscribe.c uint32.h subscribe.h
	./compile subscribe.c

surf.a: \
makelib surf.o surfpcs.o
	./makelib surf.a surf.o surfpcs.o

surf.o: \
compile surf.c surf.h surf.c uint32.h surf.c
	./compile surf.c

surfpcs.o: \
compile surfpcs.c surf.h surfpcs.c surfpcs.h uint32.h surfpcs.h \
surfpcs.c
	./compile surfpcs.c

systype: \
find-systype trycpp.c
	./find-systype > systype

uint32.h: \
tryulong32.c compile load uint32.h1 uint32.h2
	( ( ./compile tryulong32.c && ./load tryulong32 && \
	./tryulong32 ) >/dev/null 2>&1 \
	&& cat uint32.h2 || cat uint32.h1 ) > uint32.h
	rm -f tryulong32.o tryulong32

ezmlm-queue: ezmlm-queue.in conf-prefix conf-sysconfdir warn-auto.sh
	cat warn-auto.sh ezmlm-queue.in \
	| sed s}PREFIX}"`head -1 conf-prefix`"}g \
	| sed s}SYSCONFDIR}"`head -1 conf-sysconfdir`"}g \
	> ezmlm-queue
	chmod 755 ezmlm-queue

ezmlm-queue.1: ezmlm-queue.9 conf-sysconfdir
	cat ezmlm-queue.9 \
	| sed s}SYSCONFDIR}"`head -1 conf-sysconfdir`"}g \
	> ezmlm-queue.1

ezmlm.spec: ezmlm.spec.in catChangeLog doc/ChangeLog conf-prefix \
conf-sysconfdir conf-qmail conf-bin conf-email
	(cat $@.in;./catChangeLog) | $(edit) > $@
ezmlm.changes: doc/ChangeLog conf-version conf-release conf-email
	./catChangeLog --changes doc/ChangeLog > $@
PKGBUILD: PKGBUILD.in catChangeLog Makefile conf-email \
conf-version conf-release doc/ChangeLog ezmlm.changes
	cat $@.in | $(edit) > $@

distclean: clean
	/bin/rm -f Makefile. Makevars. 

clean: TARGETS
	/bin/rm -f `cat TARGETS`
