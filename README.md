You need rtmpdump first :
=========================

[Patches and Windows builds by KSV] (http://stream-recorder.com/forum/release-unofficial-rtmpdump-binaries-t11030.html)

Jtv live downloader, how do I get it ?
======================================

Windows binaries
----------------

[1.0] (https://github.com/downloads/toine512/jtv-downloader/Jtv_downloader-b1.7z "In fact it's b1 but who cares ?")

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
--------------------------------------

You need **qt4-qmake** and **libqt4-dev** (>= 4.7.4) packages to achieve that.

	sudo apt-get install git qt4-qmake libqt4-dev
	git clone git://github.com/toine512/jtv-downloader.git
	cd jtv-downloader/
	qmake --makefile
	make

At runtime you'll need **libqtcore4**, **libqt4-xml**, **libqtgui4**, **libqt4-network** :

	sudo apt-get install libqtcore4 libqt4-xml libqtgui4 libqt4-network

Configuration file samples
==========================

[On the wiki] (https://github.com/toine512/jtv-downloader/wiki)

Reporting bugs/suggestions
==========================

[GitHub issues tracker] (https://github.com/toine512/jtv-downloader/issues)