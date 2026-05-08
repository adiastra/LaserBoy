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
// LaserBoy_common.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_COMMON_DEFINITIONS__
#define __LASERBOY_COMMON_DEFINITIONS__

//############################################################################
#include "LaserBoy_macros.hpp"

//############################################################################
typedef unsigned int   u_int  ;
typedef unsigned short u_short;
typedef unsigned char  u_char ;

typedef unsigned int   LaserBoy_Bounds    ;
typedef unsigned int   LaserBoy_Error_Code;
typedef unsigned int   LaserBoy_Rescale_Error_Code;

//############################################################################
class LaserBoy_real_segment_set;
class LaserBoy_frame_set;
class LaserBoy_space;
class LaserBoy_TUI;

//############################################################################
extern LaserBoy_TUI* p_space; // the one and only pointer to LaserBoy_TUI!

//############################################################################
extern const double    quarter_pi;
extern const double    half_pi;
extern const double    three_quarters_pi;
extern const double    pi;
extern const double    pi_and_a_half;
extern const double    two_pi;
extern const double    one_degree; // in radians
extern const double    one_radian; // in degrees
extern const double    _e_;

extern const u_short   short_bit_mask[16];

//############################################################################
string GUID8char             ();

//############################################################################
//                 delta_02, delta_01, delta_12,
double delta_angle           (double a, double b, double c); // sss triangle

//############################################################################
double simple_2D_angle       (double x1, double y1, double x2, double y2);

//############################################################################
u_char rescale_to_index      (u_short* rescale, u_short value);

//############################################################################
int    greatest_common_devisor   (int x, int y);
int    lowest_common_denominator (int x, int y);

//############################################################################
bool   file_exists           (const string& file);
bool   directory_exists      (const string& dir, const string& home);

//############################################################################
string time_as_string        (double seconds);

//############################################################################
double random_01             ();
double random_neg_to_pos_1   ();

//############################################################################
bool   get_dxf_pair          (std::ifstream& in,
	                          int& group_code,
	                          string& entity_string
                             );

//############################################################################
void   txt_tag               (std::ofstream& out);

bool   clear_to_alpha        (std::ifstream& in, int& line_number);
bool   clear_to_digit        (std::ifstream& in, int& line_number);
bool   clear_to_token        (std::ifstream& in, int& line_number);
bool   clear_to_token        (std::ifstream& in, int& next_char, int& line_number);

bool   clear_through_string  (std::ifstream& in, const string &token, int& line_number);

bool   get_next_word         (std::ifstream& in, string& word,   int& line_number);
bool   get_next_token        (std::ifstream& in, string& word,   int& line_number);
bool   get_next_number       (std::ifstream& in, double& number, int& line_number);
bool   get_next_hex          (std::ifstream& in, double& number, int& line_number);

bool   hex_string            (const string& token, double& number);
string as_hex_string         (const char32_t& utf32);

bool   get_next_line         (std::ifstream& in, string& line,   int& line_number);
bool   get_next_visible_line (std::ifstream& in, string& line,   int& line_number);

//############################################################################
char*  char_utf32_to_utf8    (const char32_t& utf32,
	                          const char* buffer
                             );
bool   is_orientation        (const char32_t& utf32);
bool   is_super_diacritic    (const char32_t& utf32);
bool   is_sub_diacritic      (const char32_t& utf32);
bool   is_diacritic          (const char32_t& utf32);
bool   get_utf8_index        (u32string& characters,
	                          const string &font_name
                             );
//############################################################################
int    LaserBoy_version_check(string &version_string,
                              string install_GUID,
                              u_int app_runs_count
                             );

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
