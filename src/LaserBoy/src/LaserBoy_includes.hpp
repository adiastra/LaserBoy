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
// LaserBoy_includes.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_INCLUDES_DEFINITIONS__
#define __LASERBOY_INCLUDES_DEFINITIONS__

//############################################################################
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif

#define _FILE_OFFSET_BITS 64

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sys/stat.h>
#include <stdint.h>
#include <math.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdarg.h>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cfloat>
#include <ctime>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <functional>
#include <locale>
#include <codecvt>

#if defined __MINGW_LASERBOY__ // Windows
#include <io.h>
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#endif

using namespace std;

//boost
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/locale.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

using namespace boost::system;
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace boost::chrono;

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
