prefix=/usr
version=$(shell cat ../conf-version)
release=$(shell cat ../conf-release)
sysconfdir=/etc/indimail
#

all: rules debian.tar.gz dummy.tar.gz ezmlm.dsc

clean:
	/bin/rm -f changelog *.dsc debian.tar.gz dummy.tar.gz

edit = sed \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@version\@,$(version),g' \
	-e 's,@release\@,$(release),g' \
	-e 's,@prefix\@,$(prefix),g'

ezmlm.dsc: ezmlm.dsc.in Makefile
	$(edit) $@.in > $@
changelog: changelog.in ../conf-release ../conf-version
	(cat $@.in; echo "";\
	echo " -- Manvendra Bhangui <manvendra@indimail.org>  `date +'%a, %d %b %Y %H:%S:%M %z'`" )| $(edit) > $@
dummy.tar.gz: README.source
	tar cf - README.source |gzip > $@
debian.tar.gz: copyright rules control changelog compat \
ezmlm.install Makefile
	tar cf - copyright rules control changelog compat \
		ezmlm.install |gzip > $@
