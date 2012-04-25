Jtv live downloader, temporary instructions :
=============================================

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