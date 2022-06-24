prefix=/usr
version=$(shell cat ../conf-version)
release=$(shell cat ../conf-release)
email=$(shell cat ../conf-email)
sysconfdir=$(shell head -1 ../conf-sysconfdir)
#

all: rules debian.tar.gz dummy.tar.gz ezmlm.dsc

clean:
	/bin/rm -f changelog *.dsc debian.tar.gz dummy.tar.gz \
	changelog copyright control

edit = sed \
	-e 's,@sysconfdir\@,$(sysconfdir),g' \
	-e 's,@version\@,$(version),g' \
	-e 's,@release\@,$(release),g' \
	-e 's,@email\@,$(email),g' \
	-e 's,@prefix\@,$(prefix),g'

ezmlm.dsc: ezmlm.dsc.in Makefile
	$(edit) $@.in > $@
changelog: ../doc/ChangeLog ../conf-version ../conf-release \
../conf-email
	../catChangeLog --debian --name=ezmlm --state=stable \
	--urgency=low ../doc/ChangeLog > $@
copyright: copyright.in ../conf-email
	$(edit) $@.in > $@
control: control.in ../conf-release ../conf-version ../conf-email
	$(edit) $@.in > $@
dummy.tar.gz: README.source
	tar cf - README.source |gzip > $@
debian.tar.gz: copyright rules control changelog compat \
ezmlm.install Makefile source/format
	cd .. && tar cf - debian/copyright debian/rules debian/control \
	debian/changelog debian/compat debian/ezmlm.install \
	debian/source | gzip > debian/$@
