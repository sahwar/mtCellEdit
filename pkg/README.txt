This document outlines my testing procedures before this release.

* You cannot be root.  You must be a normal user with sudo rights.

* Your system must satisfy all of the requirements outlined in section 3 of the handbook.


---------------------------------
Debian 10 (amd64) [x86_64] (2018)
---------------------------------

Package deps:
sudo apt-get install dh-make pbuilder clang clang-tools-6.0 valgrind bison flex cppcheck txt2tags automake libreadline-dev libqt4-dev qtbase5-dev qtcreator qt4-qtconfig time libpango1.0-dev libpng-dev libgif-dev libjpeg-dev libsqlite3-dev libsndfile1-dev

* Install
	./build_debian.sh all clean --preconf "LDFLAGS=-Wl,--as-needed" --conf "debug --libdir=/usr/lib/x86_64-linux-gnu"

* Build using clang-static to expose possible errors:
	./clang_scan.sh
	firefox /tmp/scan-build-*/index.html
	./build_install.sh flush

* Use cppcheck to expose programmer errors and unused functions:
	./cppcheck.sh > ~/cppcheck.txt 2>&1
	cat ~/cppcheck.txt | grep "^\[" | sort | wc
	cat ~/cppcheck.txt | grep "^\[" | sort | less
	cat ~/cppcheck.txt | less
	rm ~/cppcheck.txt

* Run ./test/* test suites to expose regressions.
	./configure debug
	make valg
	make clean
	make time
	./configure flush

* Use file format test suites from:
	- http://www.schaik.com/pngsuite/
	- http://entropymine.com/jason/bmpsuite/
	- https://code.google.com/archive/p/imagetestsuite/

	valgrind --leak-check=full --show-possibly-lost=no pixyls .../*.bmp
	valgrind --leak-check=full --show-possibly-lost=no pixyls .../*.gif
	valgrind --leak-check=full --show-possibly-lost=no pixyls .../*.jpg
	valgrind --leak-check=full --show-possibly-lost=no pixyls .../*.png

* Smoke test the apps.

* Skim documentation to expose cruft and mistakes.

* Remove
	./build_debian.sh remove all clean

* Install, test, uninstall locally:
	DIR="$HOME/test/usr"; ./build_local.sh --preconf "LDFLAGS=-Wl,-rpath=$DIR/lib" --conf "--disable-man --prefix=$DIR"
	./build_local.sh flush
	DIR="$HOME/test/usr"; ./build_local.sh --preconf "LDFLAGS=-Wl,-rpath=$DIR/lib" --conf "--use-qt4 --disable-man --prefix=$DIR" libmtqex4 mtcelledit-qt4 mtraft-qt4 mtpixy-qt4
	./build_local.sh flush

	DIR="$HOME/test/usr"; ./build_local.sh --conf "--use-qt4 --disable-man --prefix=$DIR" remove libmtqex4 mtcelledit-qt4 mtraft-qt4 mtpixy-qt4
	./build_local.sh flush
	DIR="$HOME/test/usr"; ./build_local.sh --conf "--disable-man --prefix=$DIR" remove
	./build_local.sh flush

-------------------------------
ArcoLinux 6.9.1 [x86_64] (2018)
-------------------------------

Package deps:
sudo pacman -S base-devel txt2tags qt5-base qt4 clang-analyzer time valgrind cppcheck libpng giflib libjpeg sqlite libsndfile

* Install
	./build_arch.sh all clean

* Smoke test the apps.

* Remove
	./build_arch.sh remove all clean


-------------------------------
CentOS 7.4 XFCE [x86_64] (2017)
Fedora  28 XFCE [x86_64] (2018)
-------------------------------

Package deps:
sudo yum install clang clang-analyzer valgrind txt2tags automake gcc gcc-c++ bison flex cppcheck readline-devel rpmdevtools qt5-qtbase-devel qtchooser qt-devel qt-config cairo-devel pango-devel libpng-devel libjpeg-turbo-devel giflib-devel libsqlite3x-devel libsndfile-devel

* Install
	./build_fedora.sh all clean --conf "--libdir=/usr/lib64"

* Smoke test the apps.

* Remove
	./build_fedora.sh remove all clean


------------------------------
Suse 42.3 XFCE [x86_64] (2017)
------------------------------

Package deps:
sudo zypper install automake gcc gcc-c++ bison flex cppcheck readline-devel rpmdevtools libqt5-qtbase-devel qt-devel cairo-devel pango-devel libpng-devel libjpeg62-devel giflib-devel sqlite3-devel libsndfile-devel

* Install
	./build_suse.sh all clean --conf "--libdir=/usr/lib64"

* Smoke test the apps.

* Remove
	./build_suse.sh remove all clean


-----------------------------------------
Debian 7 (i386 256MB RAM) [x86_32] (2013)
-----------------------------------------

Package deps:
sudo apt-get install dh-make pbuilder clang valgrind bison flex cppcheck txt2tags automake libreadline-dev libqt4-dev qtcreator qt4-qtconfig time libpango1.0-dev libpng-dev libgif-dev libjpeg-dev libsqlite3-dev libsndfile1-dev

* Install
	./build_debian.sh clean --bcfile etc/bcfile_debian7_i386.txt
	./build_debian.sh flush

* Run ./test/* test suites to expose regressions.
	./configure
	make time
	./configure flush

* Smoke test the apps.

* Remove
	./build_debian.sh --bcfile etc/bcfile_debian7_i386.txt remove
	./build_debian.sh --bcfile etc/bcfile_debian7_i386.txt flush


------------------------------------------------------
Debian 7 (armhf vexpress-a9 256MB RAM) [ARM_32] (2013)
------------------------------------------------------

Package deps:
sudo apt-get install dh-make pbuilder clang valgrind bison flex cppcheck txt2tags automake libreadline-dev libqt4-dev qtcreator qt4-qtconfig time libpango1.0-dev libpng-dev libgif-dev libjpeg-dev libsqlite3-dev libsndfile1-dev

* Install
	./build_install.sh --bcfile etc/bcfile_debian7_arm.txt

* Run ./test/* test suites to expose regressions.
	./configure
	make time
	./configure flush

* Smoke test the apps.

* Remove
	./build_install.sh --bcfile etc/bcfile_debian7_arm.txt remove
	./build_install.sh --bcfile etc/bcfile_debian7_arm.txt flush


--------------------------------------------
Debian 7 (powerpc 256MB RAM) [PPC_32] (2013)
--------------------------------------------

Package deps:
sudo apt-get install dh-make pbuilder clang valgrind bison flex cppcheck txt2tags automake libreadline-dev libqt4-dev qtcreator qt4-qtconfig time libpango1.0-dev libpng-dev libgif-dev libjpeg-dev libsqlite3-dev libsndfile1-dev

* Install
	./build_install.sh --bcfile etc/bcfile_debian7_ppc.txt

* Run ./test/* test suites to expose regressions.
	./configure
	make time
	./configure flush

* Smoke test the apps.

* Remove
	./build_install.sh --bcfile etc/bcfile_debian7_ppc.txt remove
	./build_install.sh --bcfile etc/bcfile_debian7_ppc.txt flush

-------------------------------------
Lubuntu 14.04 (amd64) [x86_64] (2014)
-------------------------------------

sudo apt-get install bison flex automake libqt4-dev libpango1.0-dev libpng-dev libgif-dev libjpeg-dev libsqlite3-dev libsndfile1-dev

Build AppImages:

./build_appimage_all.sh


-----
NOTES
---------------------
Slackware 12.0 - 14.1
---------------------
mtCedCLI doesn't build successfully due to an issue with the libreadline library.  For some reason the library has symbol references to libncurses, but with no explicit library link.  Therefore the linker generates an error when these symbols cannot be resolved.  This problem can be solved by editing the configure script.  I have put these changes into configure.slackware12.0 as a convenience.  This also means you cannot use '-Wl,--as-needed' because the linker doesn't really know what is going on.

----------------------------
Debian 7 (armhf vexpress-a9)
----------------------------
Clang seems to build bad code which doesn't run properly.  Valgrind is very unreliable.

----------
References
----------
https://wiki.debian.org/Hardening#User_Space
https://wiki.ubuntu.com/Security/Features
http://www.akkadia.org/drepper/dsohowto.pdf - "How To Write Shared Libraries" by Ulrich Drepper
http://www.akkadia.org/drepper/goodpractice.pdf
http://www.akkadia.org/drepper/defprogramming.pdf

