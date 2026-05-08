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
// Copyright 2003 ~ 2021 James Lehman.
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

If your computer is Internet connected, running LaserBoy
will make a standard HTTP request to laserboy.org to check
for the latest version found here:

http://laserboy.org/code/LaserBoy_Current.zip

//############################################################################

As of this version: LaserBoy-08-04-2021, The Windows executable that comes
precompiled in the distribution zip is for 64-bit Windows!

It is still possible to build the source code to make a 32-bit Windows version.

//############################################################################

To run LaserBoy,

In Windows,

edit the file LaserBoy.bat so that it calls the executable
(LaserBoy.exe) with the two arguments following that are the
width and the height in pixels of the window that LaserBoy
will open and populate. Width can be the width of your display.
Height should be at least 30 pixels less than your display height,
to allow for the window title bar.

Start LaserBoy by double clicking on LaserBoy.bat inside of the
LaserBoy folder.


//############################################################################

In Linux,

unzip the LaserBoy distribution in the location of your choice.
navigate to inside of LaserBoy/src/ and issue the make command
to build LaserBoy from source files. This is the SDL2 version.

If you want the SDL version make -f Makefile_sdl1

cd ../ to the root of the LaserBoy directory to find the
executable file, LaserBoy (or LaserBoy_sdl1).

it is possible to run LaserBoy_sdl1 from a terminal, if the kernel
is compiled for Linux Frame Buffer support.

From inside the LaserBoy directory, issue the command:

[root@home ~]# fbset -depth 32 -s

To put the frame buffer in 32 bits per pixel color mode and
display the geometry; [width] [height] in pixels of the display.

Start LaserBoy:
[root@home ~]# ./LaserBoy_sdl1 [width] [height]

where [width] [height] are the actual values displayed by
calling fbset.

It is also possible to run LaserBoy_sdl1 or LaserBoy in
Xfree86 in much the same way; by calling it from a terminal
from the LaserBoy directory:

[root@home ~]# ./LaserBoy [width] [height]

where [width] [height] are the dimensions of the window
that it will open.

//############################################################################

In Mac OSX,

See Compiling_for_MacOS.txt.

[root@home ~]# make -f Makefile.osx

or

[root@home ~]# make -f Makefile.mm1

cd ../ to the root of the LaserBoy directory to find the
executable file, LaserBoy. Issue the command:

[root@home ~]# ./LaserBoy [width] [height]

where [width] [height] are the dimensions of the window
that it will open in decimal.

//############################################################################

LaserBoy.exe (Win64) is compiled for Windows with:

Msys2 + Mingw-w64 g++
Rev1, Built by MSYS2 project 10.2.0

boost C++ version 1.73.0
installed by msys2 via pacman

libSDL2 version 2.0.12-4
installed by msys2 via pacman

//############################################################################

The Simple DirectMedia Layer (SDL & SDL2 for short) are cross-platform
libraries designed to make it easy to write multi-media software,
such as games and emulators.

The Simple DirectMedia Layer library source code is available from:
http://www.libsdl.org/

This library is distributed under the terms of the GNU LGPL license:
http://www.gnu.org/copyleft/lesser.html

//############################################################################

Enjoy!

