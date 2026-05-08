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
// Copyright 2003 to 2026 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// LaserBoy.cpp is part of LaserBoy.
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
#include "LaserBoy_FLTK_GUI.hpp"

//############################################################################
int main(int argc, char *argv[])
{
    int xres = 640,
        yres = 480;
    if(argc >= 3)
    {
        xres = atoi(argv[1]);
        yres = atoi(argv[2]);
    }
    if(xres < 0) return -1;
    if(yres < 0) return -1;

    LaserBoy_GUI *window = new LaserBoy_GUI(xres, yres, "LaserBoy by James Lehman : ©GPL3 2003~2026 Extra Stimulus Inc. (in FLTK)");
    window->end();
    Fl::visual(FL_RGB);
    window->show();
    return window->start_menu_loop();
}


//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
