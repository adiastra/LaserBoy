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
// Copyright 2003 to 2022 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// LaserBoy_macros.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_MACRO_DEFINITIONS__
#define __LASERBOY_MACRO_DEFINITIONS__

//############################################################################
#include "LaserBoy_includes.hpp"

//############################################################################
#define    LASERBOY_VERSION                "LaserBoy-05-05-2026-editor"
//----------------------------------------------------------------------------
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-06-03-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-15-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-17X2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-24-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-08-28-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-09-03-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-10-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-11-11-2008"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-01-06-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-01-09-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-01-16-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-02-10-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-03-04-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-01-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-05-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-18-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-09-15-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-03-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-27-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-31-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-12-06-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-12-08-2009"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-06-28-2010"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-08-21-2011"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-02-17-2013"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-12-02-2014"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-02-16-2015"  X
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-03-10-2016"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-03-03-2019"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-06-2020"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-07-28-2021"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-08-20-2021"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-08-27-2022"
//#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-12-19-2023"
#define    LASERBOY_CFG_VERSION            "LaserBoy.wtf-10-12-2022"

//----------------------------------------------------------------------------
//#define    LASERBOY_WAVE_VERSION           "LaserBoy06072008"
//#define    LASERBOY_WAVE_VERSION           "LaserBoy07132008"
//#define    LASERBOY_WAVE_VERSION           "LaserBoy10102008"
//#define    LASERBOY_WAVE_VERSION           "LaserBoy11112008"
//#define    LASERBOY_WAVE_VERSION           "LaserBoy12082009"
//#define    LASERBOY_WAVE_VERSION           "LaserBoy06282010"
#define    LASERBOY_WAVE_VERSION           "LaserBoy20231223"
//----------------------------------------------------------------------------
#define    LASERBOY_TXT_VERSION            "LaserBoy-txt-09-08-2022"
//----------------------------------------------------------------------------
#define    LASERBOY_BMP_SHARE              (string("./bmp/"))
#define    LASERBOY_CTN_SHARE              (string("./ctn/"))
#define    LASERBOY_DXF_SHARE              (string("./dxf/"))
#define    LASERBOY_ILD_SHARE              (string("./ild/"))
#define    LASERBOY_FONT_SHARE             (string("./ild/fonts/"))
#define    LASERBOY_TXT_SHARE              (string("./txt/"))
#define    LASERBOY_RESCALE_SHARE          (string("./txt/rescales/"))
#define    LASERBOY_WAV_SHARE              (string("./wav/"))
#define    LASERBOY_AUDIO_SHARE            (string("./wav/audio/"))
#define    LASERBOY_CLIPS_SHARE            (string("./wav/clips/"))
#define    LASERBOY_UNFORMATTED_SHARE      (string("./wav/unformatted/"))
#define    LASERBOY_WTF_SHARE              (string("./wtf/"))
#define    LASERBOY_UTF8_SHARE             (string("./utf8/"))
#define    LASERBOY_NO_FILE_SAVED          (string("[NO_FILE_SAVED]"))
//----------------------------------------------------------------------------
#define    LASERBOY_DIRECTORY                            0
#define    LASERBOY_FILE                                 1
//----------------------------------------------------------------------------
#define    LASERBOY_OK                                   0b00000000000000000000000000000000
#define    LASERBOY_EOF                                  0b00000000000000000000000000000001
#define    LASERBOY_FILE_OPEN_FAILED                     0b00000000000000000000000000000010
#define    LASERBOY_DIR_OPEN_FAILED                      0b00000000000000000000000000000100
#define    LASERBOY_NO_FRAMES_FOUND                      0b00000000000000000000000000001000
#define    LASERBOY_NO_PALETTES_FOUND                    0b00000000000000000000000000010000
#define    LASERBOY_QUANTITY_DATA_MISSMATCH              0b00000000000000000000000000100000
#define    LASERBOY_COLOR_VERTEX_MISSMATCH               0b00000000000000000000000001000000
#define    LASERBOY_LB_WAVE_FORMAT_ERROR                 0b00000000000000000000000010000000
#define    LASERBOY_TXT_SYNTAX_ERROR                     0b00000000000000000000000100000000
#define    LASERBOY_TXT_PALETTE_FAILED                   0b00000000000000000000001000000000
#define    LASERBOY_PALETTE_NAME_IN_USE                  0b00000000000000000000010000000000
#define    LASERBOY_PALETTE_ALREADY_DEFINED              0b00000000000000000000100000000000
#define    LASERBOY_TXT_PALETTE_NAME_NOT_FOUND           0b00000000000000000001000000000000
#define    LASERBOY_TXT_UNEXPECED_PALETTE                0b00000000000000000010000000000000
#define    LASERBOY_TXT_PALETTE_NAME_TOO_LONG            0b00000000000000000100000000000000
#define    LASERBOY_BAD_FONT_FILE                        0b00000000000000001000000000000000
#define    LASERBOY_EFFECT_NAME_NOT_FOUND                0b00000000000000010000000000000000
#define    LASERBOY_EFFECT_PROMPT_ESC                    0b00000000000000100000000000000000
#define    LASERBOY_NOTHING_SELECTED                     0b00000000000001000000000000000000
#define    LASERBOY_DIVIDE_BY_ZERO_ERROR                 0b00000000000010000000000000000000
#define    LASERBOY_EMPTY_STRING_ERROR                   0b00000000000100000000000000000000
#define    LASERBOY_INVALID_UNICODE                      0b00000000001000000000000000000000
#define    LASERBOY_REDUNDANT_UNICODE                    0b00000000010000000000000000000000
//----------------------------------------------------------------------------
#define    LASERBOY_PARENT_DIR_INVALID                  -2
#define    LASERBOY_PARENT_DIR_EMPTY                    -1
#define    LASERBOY_NAME_NO_MATCH                        0
#define    LASERBOY_NAME_MATCH                           1
#define    LASERBOY_NAME_COMPLETE                        2
//----------------------------------------------------------------------------
#define    LASERBOY_FORMAT_0_1                           1
#define    LASERBOY_FORMAT_2                             2
#define    LASERBOY_FORMAT_3                             4
#define    LASERBOY_FORMAT_4_5                           8
//----------------------------------------------------------------------------
#define    LASERBOY_WAVE_NO_MODE                         0
#define    LASERBOY_WAVE_NEGATIVE                        0
#define    LASERBOY_WAVE_POSITIVE                        1 // 0
#define    LASERBOY_WAVE_END_OF_FRAME                    2 // 1
#define    LASERBOY_WAVE_UNIQUE_FRAME                    4 // 2
#define    LASERBOY_WAVE_UNIQUE_VERTEX                   8 // 3
#define    LASERBOY_WAVE_OFFSETS                        16 // 4
#define    LASERBOY_WAVE_OPTIMIZED                      32 // 5
#define    LASERBOY_WAVE_SIGNAL_MATRIX                  64 // 6
#define    LASERBOY_SIGNAL_BIT_RESOLUTION              128 // 7
#define    LASERBOY_WAVE_NUM_FRAMES                    256 // 8
#define    LASERBOY_COLOR_RESCALE_R                    512 // 9
#define    LASERBOY_COLOR_RESCALE_G                   1024 // 10
#define    LASERBOY_COLOR_RESCALE_B                   2048 // 11
#define    LASERBOY_COLOR_RESCALE_I                   4096 // 12
//----------------------------------------------------------------------------
#define    LASERBOY_NO_SIGNAL                         0x00 // zero
#define    LASERBOY_SIGNAL_UNDEFINED                  0x01 // not zero
//----------------------------------------------------------------------------
#define    LASERBOY_SIGNAL_X_POSITION                 0x10
#define    LASERBOY_SIGNAL_Y_POSITION                 0x11
#define    LASERBOY_SIGNAL_Z_POSITION                 0x12
#define    LASERBOY_SIGNAL_DX_POSITION                0x13
#define    LASERBOY_SIGNAL_DY_POSITION                0x14
#define    LASERBOY_SIGNAL_X2_POSITION                0x15
#define    LASERBOY_SIGNAL_Y2_POSITION                0x16
//----------------------------------------------------------------------------
#define    LASERBOY_SIGNAL_BEAM_WIDTH                 0x20
//----------------------------------------------------------------------------
#define    LASERBOY_SIGNAL_RED_TTL                    0x30
#define    LASERBOY_SIGNAL_RED_ANALOG                 0x31
#define    LASERBOY_SIGNAL_YELLOW_TTL                 0x32
#define    LASERBOY_SIGNAL_YELLOW_ANALOG              0x33
#define    LASERBOY_SIGNAL_GREEN_TTL                  0x34
#define    LASERBOY_SIGNAL_GREEN_ANALOG               0x35
#define    LASERBOY_SIGNAL_BLUE_TTL                   0x36
#define    LASERBOY_SIGNAL_BLUE_ANALOG                0x37
#define    LASERBOY_SIGNAL_VIOLET_TTL                 0x38
#define    LASERBOY_SIGNAL_VIOLET_ANALOG              0x39
#define    LASERBOY_SIGNAL_MONO_TTL                   0x3a
#define    LASERBOY_SIGNAL_MONO_OR_ANALOG             0x3b
#define    LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG       0x3c
#define    LASERBOY_SIGNAL_MONO_AVG_ANALOG            0x3d
#define    LASERBOY_SIGNAL_MONO_O_SCOPE               0x3e
//----------------------------------------------------------------------------
#define    LASERBOY_SIGNAL_AUDIO_SMTPE                0x40
#define    LASERBOY_SIGNAL_AUDIO_MONO                 0x41
#define    LASERBOY_SIGNAL_AUDIO_LEFT                 0x42
#define    LASERBOY_SIGNAL_AUDIO_RIGHT                0x43
#define    LASERBOY_SIGNAL_AUDIO_CENTER               0x44
#define    LASERBOY_SIGNAL_AUDIO_SUB                  0x45
#define    LASERBOY_SIGNAL_AUDIO_S_LEFT               0x46
#define    LASERBOY_SIGNAL_AUDIO_S_RIGHT              0x47
#define    LASERBOY_SIGNAL_AUDIO_R_LEFT               0x48
#define    LASERBOY_SIGNAL_AUDIO_R_RIGHT              0x49
//----------------------------------------------------------------------------
#define    LASERBOY_SIGNAL_REVERSE_RED_ANALOG         0x50
#define    LASERBOY_SIGNAL_REVERSE_YELLOW_ANALOG      0x51
#define    LASERBOY_SIGNAL_REVERSE_GREEN_ANALOG       0x52
#define    LASERBOY_SIGNAL_REVERSE_BLUE_ANALOG        0x53
#define    LASERBOY_SIGNAL_REVERSE_VIOLET_ANALOG      0x54
//----------------------------------------------------------------------------
#define    LASERBOY_LSB_NOT_USED                         0
#define    LASERBOY_LSB_BLANKING                         1
#define    LASERBOY_LSB_END_OF_FRAME                     2
#define    LASERBOY_LSB_UNIQUE_FRAME                     3
#define    LASERBOY_LSB_UNIQUE_VERTEX                    4
//----------------------------------------------------------------------------
#define    LASERBOY_DOTS_IGNORE                          0
#define    LASERBOY_DOTS_REMOVE                          1
#define    LASERBOY_DOTS_ENHANCE                         2
//----------------------------------------------------------------------------
#define    LASERBOY_RESCALE_BAD_CHANNEL_INDEX            1
#define    LASERBOY_RESCALE_INCOMPLETE_DATA              2
#define    LASERBOY_RESCALE_IMPROPER_DATA                3
#define    LASERBOY_RESCALE_NONZERO_ZERO                 4
#define    LASERBOY_RESCALE_ODD_DATA                     5
#define    LASERBOY_RESCALE_REDUNDANT_DATA               6
#define    LASERBOY_RESCALE_FILE_FAILED                  7
//----------------------------------------------------------------------------
#define    LASERBOY_IN_BOUNDS                            0
#define    LASERBOY_OUT_POS_X                            1
#define    LASERBOY_OUT_NEG_X                            2
#define    LASERBOY_OUT_POS_Y                            4
#define    LASERBOY_OUT_NEG_Y                            8
#define    LASERBOY_OUT_POS_Z                           16
#define    LASERBOY_OUT_NEG_Z                           32
#define    LASERBOY_BOUNDS_MASK                         63
//----------------------------------------------------------------------------
#define    LASERBOY_3D_FRAME                             0 // actual ILDA section numbers
#define    LASERBOY_2D_FRAME                             1
#define    LASERBOY_PALETTE                              2
#define    LASERBOY_COLOR_TABLE                          3
#define    LASERBOY_3D_FRAME_RGB                         4
#define    LASERBOY_2D_FRAME_RGB                         5
//----------------------------------------------------------------------------
#define    LASERBOY_3D_FRAME_PALETTE                     6 // added for txt output
#define    LASERBOY_2D_FRAME_PALETTE                     7
#define    LASERBOY_3D_FRAME_TABLE                       8
#define    LASERBOY_2D_FRAME_TABLE                       9
#define    LASERBOY_3D_FRAME_HEX                        10
#define    LASERBOY_2D_FRAME_HEX                        11
//----------------------------------------------------------------------------
#define    LASERBOY_SHORT                                0 // added for txt output
#define    LASERBOY_UNIT                                 2
//#define    LASERBOY_REAL                                 3
#define    LASERBOY_RGB                                  4
#define    LASERBOY_HEX                                  5
//----------------------------------------------------------------------------
#define    LASERBOY_PALETTE_NOT_FOUND                   -2
#define    LASERBOY_TRUE_COLOR                          -1
#define    LASERBOY_ILDA_DEFAULT                         0
#define    LASERBOY_ILDA_TTL                             1
#define    LASERBOY_X29                                  2
#define    LASERBOY_ILDA_STANDARD                        3
#define    LASERBOY_DXF                                  4
#define    LASERBOY_LASERBOY_HUES                        5
#define    LASERBOY_REDGREEN                             6
#define    LASERBOY_REDUCED_332                          7
#define    LASERBOY_REDS                                 8
#define    LASERBOY_GREENS                               9
#define    LASERBOY_BLUES                               10
#define    LASERBOY_GRAYS                               11
#define    LASERBOY_BUILT_IN_PALETTES                   12
//----------------------------------------------------------------------------
#define    LASERBOY_BLANKING_BIT                        64  // 0100 0000
#define    LASERBOY_LAST_VERTEX                        128  // 1000 0000
//----------------------------------------------------------------------------
#define    LASERBOY_PALETTE_MAX                        256
//----------------------------------------------------------------------------
#define    LASERBOY_DEBUG_MESSAGES                       0
//----------------------------------------------------------------------------
#define    LASERBOY_STATS_DISPLAY_OFFSET                 4
//----------------------------------------------------------------------------
#define    LASERBOY_SHORT_SPAN                       65535
#define    LASERBOY_MAX_USHORT                       65535
#define    LASERBOY_MAX_SHORT                        32767
#define    LASERBOY_MAX_COLOR_SHORT                  32640 // 0111 1111 1000 0000
#define    LASERBOY_MIN_SHORT                       -32767
#define    LASERBOY_HALF_SHORT                       16383
//----------------------------------------------------------------------------
#define    LASERBOY_BMP_FILE_HEADER_SIZE                54
#define    LASERBOY_BITS_IN_SHORT                       16
#define    LASERBOY_BITS_IN_CHAR                         8
//----------------------------------------------------------------------------
#define    LASERBOY_CUBE                                 1
#define    LASERBOY_SPHERE                               2
#define    LASERBOY_7_8THS_CUBE                          3
#define    LASERBOY_7_8THS_SPHERE                        4
//----------------------------------------------------------------------------
#define    LASERBOY_EPSILON                       0.000001
//----------------------------------------------------------------------------
#ifndef O_BINARY
#define    O_BINARY 0
#endif
//----------------------------------------------------------------------------
//#if defined __MINGW_LASERBOY__ // Windows
#define    STARTS_WITH     starts_with // case sensitive filenames
//#define    ENDL            '\n'

//#else // UNIX
//#define    STARTS_WITH     starts_with // case sensitive filenames
#define    ENDL            endl
//#endif
//----------------------------------------------------------------------------
// defalut RGB order of standard bitmap files
#define    BMP_ALPHA_BIT_SHIFT                     24
#define    BMP_RED_BIT_SHIFT                       16
#define    BMP_GREEN_BIT_SHIFT                      8
#define    BMP_BLUE_BIT_SHIFT                       0

// defalut RGBA for most versions of Linux and Windows
#define    ALPHA_BIT_SHIFT                         24
#define    RED_BIT_SHIFT                           16
#define    GREEN_BIT_SHIFT                          8
#define    BLUE_BIT_SHIFT                           0

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################



