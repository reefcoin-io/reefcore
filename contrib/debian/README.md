
Debian
====================
This directory contains files used to package reefd/reef-qt
for Debian-based Linux systems. If you compile reefd/reef-qt yourself, there are some useful files here.

## reef: URI support ##


reef-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install reef-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your reef-qt binary to `/usr/bin`
and the `../../share/pixmaps/reef128.png` to `/usr/share/pixmaps`

reef-qt.protocol (KDE)

