//############################################################################
//
// LaserBoy !!!
//
// by James Lehman
// Extra Stimulus Inc.
// james@akrobiz.com
//
// began: October 2003
//
// Copyright 2003 ~ 2026 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// README.txt (this file) is part of LaserBoy.
//
// LaserBoy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LaserBoy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LaserBoy. If not, see <http://www.gnu.org/licenses/>.
//
//############################################################################

LaserBoy is a free and open source application for creating, editing,
viewing, converting, and exporting laser display vector art.

LaserBoy works with ILDA laser frame files, wave files, palettes,
fonts, DXF, bitmap rendering, plain text tables, and other related
laser display formats.

LaserBoy is keyboard driven.

It does not use the mouse for drawing or editing.

//############################################################################

Latest public version:

http://laserboy.org/code/LaserBoy_Current.zip

If your computer is Internet connected, running LaserBoy may make
a standard HTTP request to laserboy.org to check for the latest
public version.

//############################################################################

This release uses FLTK.

The older SDL and SDL2 source trees have been retired from this
release.

You do not need SDL or SDL2 to compile this version of LaserBoy.

//############################################################################

Windows
=======

The Windows executable included in this distribution is for
64-bit Windows.

To run LaserBoy in Windows, double click:

LaserBoy.bat

LaserBoy.bat starts:

LaserBoy.exe

with two numbers after it.

Those numbers are the width and height of the LaserBoy window
in pixels.

For example:

LaserBoy.exe 1024 700

You can edit LaserBoy.bat with a plain text editor and change
those numbers to fit your display.

The height should be smaller than your screen height so there
is room for the window title bar and borders.

To compile LaserBoy in Windows, see:

doc/Compiling_for_Windows.txt

//############################################################################

Linux
=====

Unzip the LaserBoy distribution.

On a normal Linux workstation, LaserBoy may be placed wherever
you want.

On a Raspberry Pi, Orange Pi, piDAC, or other small Linux computer,
the recommended location is:

/LaserBoy

This version of LaserBoy is a graphical FLTK application.

It needs a running desktop, X server, VNC desktop, or other
graphical Linux session.

It does not run directly on the Linux framebuffer.

To compile LaserBoy in Linux, go to the source directory:

cd LaserBoy/src

or, if LaserBoy is installed at /LaserBoy:

cd /LaserBoy/src

Then build it:

time make

or use the platform-specific makefile if appropriate:

time make -f Makefile.pi

When the build is done, go back to the main LaserBoy directory
and run it:

cd ..

./LaserBoy 1200 800

or

./LaserBoy_pi 1200 800

The two numbers are the width and height of the LaserBoy window
in pixels.

To compile LaserBoy on a Linux Pi, see:

doc/Raspberry_Pi.txt

//############################################################################

MacOS
=====

LaserBoy can be compiled on MacOS using Homebrew, FLTK, Boost,
and Cairo.

To compile LaserBoy on MacOS, see:

doc/Compiling_for_MacOS.txt

After compiling, run LaserBoy from the main LaserBoy directory:

./LaserBoy_osx 1600 980

or

./LaserBoy_mm1 1600 980

The two numbers are the width and height of the LaserBoy window
in pixels.

//############################################################################

Source Code
===========

The LaserBoy source code is in:

src/

This release builds with FLTK.

The source code is C++ and is distributed under the terms of the
GNU General Public License.

For the list of makefiles, see:

doc/List_of_Makefiles.txt

//############################################################################

Dependencies
============

LaserBoy requires different development packages depending on
the operating system.

In general, the FLTK version of LaserBoy uses:

FLTK
Boost
Cairo

See the compile documents in the doc directory for exact
installation commands.

//############################################################################

Enjoy!

James Lehman
james@akrobiz.com
laserboy.org
