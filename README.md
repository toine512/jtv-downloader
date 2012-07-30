Windows binaries
================
- [temporary 1.1 (30/07/2012)] (https://github.com/downloads/toine512/jtv-downloader/jtv-downloader_1.1x.7z)
  Includes rtmpdump builds by svnpenn.
- [1.0] (https://github.com/downloads/toine512/jtv-downloader/Jtv_downloader-b1.7z "In fact it's b1 but who cares ?")

Linux binaries
==============
Getting an up to date version of rtmpdump:
------------------------------------------
### Ubuntu Precise or under & Debian Squeeze or under
- i386

		wget http://ftp.fr.debian.org/debian/pool/main/r/rtmpdump/rtmpdump_2.4+20111222.git4e06e21-1_i386.deb
		sudo dpkg -i rtmpdump_2.4+20111222.git4e06e21-1_i386.deb
		rm rtmpdump_2.4+20111222.git4e06e21-1_i386.deb

- AMD64

		wget http://ftp.fr.debian.org/debian/pool/main/r/rtmpdump/rtmpdump_2.4+20111222.git4e06e21-1_amd64.deb
		sudo dpkg -i rtmpdump_2.4+20111222.git4e06e21-1_amd64.deb
		rm rtmpdump_2.4+20111222.git4e06e21-1_amd64.deb

### Ubuntu Quantal or above & Debian Wheezy and above
	sudo apt-get rtmpdump

PPA for Ubuntu-based distributions
----------------------------------
ppa:toine512/jtv-downloader

	sudo add-apt-repository ppa:toine512/jtv-downloader

Install **jtv-downloader** package :

	sudo apt-get install jtv-downloader

Debian binary packages compiled in Ubuntu Oneiric
-------------------------------------------------

1.0 :
- [i386] (https://launchpad.net/~toine512/+archive/jtv-downloader/+files/jtv-downloader_1.0-0ubuntu7_i386.deb "jtv-downloader_1.0-0ubuntu7_i386.deb")
- [AMD64] (https://launchpad.net/~toine512/+archive/jtv-downloader/+files/jtv-downloader_1.0-0ubuntu7_amd64.deb "jtv-downloader_1.0-0ubuntu7_amd64.deb")

Building on Debian-based distributions
======================================

You need **qt4-qmake** and **libqt4-dev** (>= 4.7.4) packages to achieve that.

	sudo apt-get install git g++ qt4-qmake libqt4-dev
	git clone git://github.com/toine512/jtv-downloader.git
	cd jtv-downloader/
	qmake -makefile
	make

At runtime you'll need **libqtcore4**, **libqt4-xml**, **libqtgui4**, **libqt4-network** :

	sudo apt-get install libqtcore4 libqt4-xml libqtgui4 libqt4-network
See rtmpdump binaries above.

Configuration file samples
==========================

[On the wiki] (https://github.com/toine512/jtv-downloader/wiki)

Reporting bugs/suggestions
==========================

[GitHub issues tracker] (https://github.com/toine512/jtv-downloader/issues)