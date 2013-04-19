Latest stable version : **1.2**

Windows binaries
================

- [1.2] (http://sourceforge.net/projects/jtv-downloader/files/release/jtv-downloader_1.2.7z/download)
  Includes official rtmpdump builds. Hosted on SourceForge.

Previous packages available [here] (https://github.com/toine512/jtv-downloader/wiki/Previous-releases "Wiki:Previous-releases").

Linux binaries
==============

PPA for Ubuntu-based distributions
----------------------------------

ppa:toine512/jtv-downloader

	sudo add-apt-repository ppa:toine512/jtv-downloader

Install **jtv-downloader** package :

	sudo apt-get install jtv-downloader

Debian binary packages
----------------------

1.2 :
- [i386] (http://sourceforge.net/projects/jtv-downloader/files/release/jtv-downloader_1.2-0build1_i386.deb/download "jtv-downloader_1.2-0build1_i386.deb")
- [AMD64] (http://sourceforge.net/projects/jtv-downloader/files/release/jtv-downloader_1.2-0build1_amd64.deb/download "jtv-downloader_1.2-0build1_amd64.deb")

Previous packages available [here] (https://github.com/toine512/jtv-downloader/wiki/Previous-releases "Wiki:Previous-releases").

Changelog
=========

[On the wiki] (https://github.com/toine512/jtv-downloader/wiki/Changelog)

Building on Debian-based distributions
======================================

You need **g++**, **qt4-qmake** and **libqt4-dev** (>= 4.7.4) packages to achieve that.

	sudo apt-get install git g++ qt4-qmake libqt4-dev
	git clone git://github.com/toine512/jtv-downloader.git
	cd jtv-downloader/
	qmake -makefile
	make

At runtime you'll need **rtmpdump**, **libqtcore4**, **libqt4-xml**, **libqtgui4**, **libqt4-network** :

	sudo apt-get install rtmpdump libqtcore4 libqt4-xml libqtgui4 libqt4-network

Configuration file samples
==========================

[On the wiki] (https://github.com/toine512/jtv-downloader/wiki)

Reporting bugs/suggestions
==========================

[GitHub issues tracker] (https://github.com/toine512/jtv-downloader/issues)