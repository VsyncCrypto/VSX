
Debian
====================
This directory contains files used to package carid/cari-qt
for Debian-based Linux systems. If you compile carid/cari-qt yourself, there are some useful files here.

## cari: URI support ##


cari-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install cari-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your cari-qt binary to `/usr/bin`
and the `../../share/pixmaps/cari128.png` to `/usr/share/pixmaps`

cari-qt.protocol (KDE)

