[![Matrix](https://img.shields.io/matrix/indimail:matrix.org.svg)](https://matrix.to/#/#indimail:matrix.org)

**Current Compilation Status**

[![ezmlm Ubuntu, Mac OSX CI](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-c-cpp.yml/badge.svg)](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-c-cpp.yml)
[![ezmlm FreeBSD CI](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-freebsd.yml/badge.svg)](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-freebsd.yml)

# ezmlm

[ezmlm fork](https://cr.yp.to/ezmlm.html) with support for [indimail-mta's](https://github.com/mbhangui/indimail-mta) multi-queue architecture.

This has been forked to adapt it to work with indimail-mta's multi-queue architecture. The three major changes that this version has made after the fork are

1. Ability to set environment variables in /etc/indimail/ezmlm/global\_vars. You can create any file in this directory. The file name becomes the environment variable and the file content becomes the value of environment variable.
2. New queue program ezmlm-queue which can use qmail-qmqpc to use [QMQP](http://cr.yp.to/proto/qmqp.html) protocol, [qmail-multi](https://github.com/mbhangui/indimail-virtualdomains/wiki/IndiMail#qmail-multi) to queue mails to indimail's multi-queue instance or [qmail-queue](https://github.com/mbhangui/indimail-virtualdomains/wiki/IndiMail#qmail-queue) to queue mails to any [qmta](https://github.com/mbhangui/indimail-virtualdomains/wiki/IndiMail#qmta---using-a-minimal-standalone-qmta-send-mta) / [qmail](http://cr.yp.to/qmail.html) / [netqmail](http://netqmail.org/) / [notqmail](https://github.com/notqmail/notqmail) instance.
3. Use all common functions from [libqmail](https://github.com/mbhangui/libqmail) library.

ezmlm is an easy-to-use, high-speed mailing list manager for qmail.

ezmlm lets users set up their own mailing lists within qmail's address hierarchy. A user, Joe, types ``ezmlm-make ~/SOS ~/.qmail-sos joe-sos isp.net'' and instantly has a functioning mailing list, joe-sos@isp.net, with all relevant information stored in a new ~/SOS directory.

ezmlm sets up joe-sos-subscribe and joe-sos-unsubscribe for automatic processing of subscription and unsubscription requests. Any message to joe-sos-subscribe will work; Joe doesn't have to explain any tricky command formats. ezmlm will send back instructions if a subscriber sends a message to joe-sos-request or joe-sos-help.

ezmlm automatically archives new messages. Messages are labelled with sequence numbers; a subscriber can fetch message 123 by sending mail to joe-sos-get.123. The archive format supports fast message retrieval even when there are thousands of messages.

ezmlm takes advantage of qmail's VERPs to reliably determine the recipient address and message number for every incoming bounce message. It waits ten days and then sends the subscriber a list of message numbers that bounced. If that warning bounces, ezmlm sends a probe; if the probe bounces, ezmlm automatically removes the subscriber from the mailing list.

ezmlm is easy for users to control. Joe can edit ~/SOS/text/* to change any of the administrative messages sent to subscribers. He can remove ~/SOS/public and ~/SOS/archived to disable automatic subscription and archiving. He can put his own address into ~/SOS/editor to set up a moderated mailing list. He can edit ~/SOS/{headeradd,headerremove} to control outgoing headers. ezmlm has several utilities to manually inspect and manage mailing lists.

ezmlm uses `Delivered-To` header to stop forwarding loops, Mailing-List to protect other mailing lists against false subscription requests, and real cryptographic cookies to protect normal users against false subscription requests. ezmlm can also be used for a sublist, redistributing messages from another list.

ezmlm is reliable, even in the face of system crashes. It writes each new subscription and each new message safely to disk before it reports success to qmail.

ezmlm doesn't mind huge mailing lists. Lists don't even have to fit into memory. ezmlm hashes the subscription list into a set of independent files so that it can handle subscription requests quickly. ezmlm uses qmail for blazingly fast parallel SMTP deliveries.

# Source Compiling/Linking

## Download / clone / compile libqmail

ezmlm uses functions from libqmail library. libqmail uses GNU autotools. You need to have autoconf, automake, libtool and pkg config package. Follow the instructions below to have them installed in case you don't have them.

```
$ cd /usr/local/src
$ git clone https://github.com/mbhangui/libqmail.git
$ cd /usr/local/src/libqmail
$ ./default.configure
$ make
$ sudo make install-strip
```

(check version in libqmail/conf-version)

**NOTES**

For Darwin (Mac OSX), install [MacPorts](https://www.macports.org/) or brew. You can look at this [document](https://paolozaino.wordpress.com/2015/05/05/how-to-install-and-use-autotools-on-mac-os-x/) for installing MacPorts.

```
FreeBSD
# pkg install automake autoconf libtool pkgconf

Darwin
# port install autoconf libtool automake pkgconfig openssl
# port update outdated

Arch Linux
# pacman -S --refresh --sysupgrade
# pacman -S --needed archlinux-keyring
# pacman -S base-devel diffutils coreutils openssl

Gentoo Linux
# emaint -a sync
# emerge-webrsync
# emerge -a app-portage/eix
# eix-sync
# etc-update

Alpine Linux
# apk add gcc g++ make git autoconf automake libtool m4 sed
# apk add openssl-dev
```

## Download / clone / compile ezmlm

The build below depends on several Makefiles. For the build to operate without errors, you need to run default.configure the first time and everytime after you do a `make distclean`. If you don't run default.configure, you can replace `make` with `./qmake`

```
$ cd /usr/local/src
$ git clone https://github.com/mbhangui/ezmlm.git
$ cd /usr/local/src/ezmlm/ezmlm-x
$ ./default.configure
$ make or ./qmake
$ sudo make install-strip
```

# Binary Builds on openSUSE Build Service

**[Build Status on](https://build.opensuse.org/project/monitor/home:mbhangui) [Open Build Service](https://build.opensuse.org/project/show/home:mbhangui)**

[![ezmlm obs trigger](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-obs.yml/badge.svg)](https://github.com/mbhangui/ezmlm/actions/workflows/ezmlm-obs.yml)

[![ezmlm](https://build.opensuse.org/projects/home:mbhangui/packages/ezmlm/badge.svg?type=percent)](https://build.opensuse.org/package/show/home:mbhangui/ezmlm)

You can get binary RPM / Debian packages at

* [Stable Releases](http://download.opensuse.org/repositories/home:/indimail/)
* [Experimental Releases](http://download.opensuse.org/repositories/home:/mbhangui/)

If you want to use DNF / YUM / apt-get, the corresponding install instructions for the two repositories, depending on whether you want to install a stable or an experimental release, are

* [Stable](https://software.opensuse.org/download.html?project=home%3Aindimail&package=ezmlm)
* [Experimental](https://software.opensuse.org/download.html?project=home%3Ambhangui&package=ezmlm)

```
Currently, the list of supported binary distributions for ezmlm is

    * Arch Linux

    * SUSE
          o openSUSE_Leap_15.3
          o openSUSE_Leap_15.4
          o openSUSE_Tumbleweed
          o SUSE Linux Enterprise 12
          o SUSE Linux Enterprise 12 SP1
          o SUSE Linux Enterprise 12 SP2
          o SUSE Linux Enterprise 12 SP3
          o SUSE Linux Enterprise 12 SP4
          o SUSE Linux Enterprise 12 SP5
          o SUSE Linux Enterprise 15
          o SUSE Linux Enterprise 15 SP1
          o SUSE Linux Enterprise 15 SP2
          o SUSE Linux Enterprise 15 SP3
          o SUSE Linux Enterprise 15 SP4

    * Red Hat
          o Fedora 36
          o Fedora 37
          o Red Hat Enterprise Linux 7
          o Scientific Linux 7
          o CentOS 7
          o CentOS 8
          o CentOS 8 Stream
          o CentOS 9 Stream
          o AlmaLinux 8
          o AlmaLinux 9

    * Debian
          o Debian 10.0
          o Debian 11.0

    * Ubuntu
          o Ubuntu 18.04
          o Ubuntu 20.04
          o Ubuntu 22.04
```

NOTE: You can also build local binary packages. To generate RPM packages locally for all components refer to [Create Local Binary Packages](.github/CREATE-Packages.md)

# SUPPORT INFORMATION

For this forked version of ezmlm, you can contact on IRC or mailing list

## IRC / Matrix

[![Matrix](https://img.shields.io/matrix/indimail:matrix.org.svg)](https://matrix.to/#/#indimail:matrix.org)

* [Matrix Invite Link #indimail:matrix.org](https://matrix.to/#/#indimail:matrix.org)
* IndiMail has an [IRC channel on libera](https://libera.chat/) #indimail-mta

The matrix room and libera.chat channel have been bridged so joining either one should be sufficient.

## Mailing list

There are four Mailing Lists for IndiMail

1. indimail-support  - You can subscribe for Support [here](https://lists.sourceforge.net/lists/listinfo/indimail-support). You can mail [indimail-support](mailto:indimail-support@lists.sourceforge.net) for support Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-support)
2. indimail-devel - You can subscribe [here](https://lists.sourceforge.net/lists/listinfo/indimail-devel). You can mail [indimail-devel](mailto:indimail-devel@lists.sourceforge.net) for development activities. Old discussions can be seen [here](https://sourceforge.net/mailarchive/forum.php?forum_name=indimail-devel)
3. indimail-announce - This is only meant for announcement of New Releases or patches. You can subscribe [here](http://groups.google.com/group/indimail)
4. Archive at [Google Groups](http://groups.google.com/group/indimail). This groups acts as a remote archive for indimail-support and indimail-devel.

There is also a [Project Tracker](http://sourceforge.net/tracker/?group_id=230686) for IndiMail (Bugs, Feature Requests, Patches, Support Requests)

## CREDITS

The IDX patches add: Indexing, (Remote) Moderation, digest, make patches, multi-language, MIME, global interface, SQL database support.

Fred Lindberg <lindberg@id.wustl.edu>, Fred B. Ringel <fredr@rivertown.net>, Bruce Guenter <bruce@untroubled.org>, and many others
