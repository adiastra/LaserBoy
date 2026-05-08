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
// LaserBoy_color.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_COLOR_DEFINITIONS__
#define __LASERBOY_COLOR_DEFINITIONS__

#include "LaserBoy_common.hpp"

//############################################################################
class LaserBoy_color
{
public:
    LaserBoy_color( const u_char &_r = 0,
                    const u_char &_g = 0,
                    const u_char &_b = 0
                  )
            : r(_r), g(_g), b(_b)                      {}
    //------------------------------------------------------------------------
    LaserBoy_color(const LaserBoy_color &c)
            : r(c.r), g(c.g), b(c.b)                   {}
    //------------------------------------------------------------------------
    LaserBoy_color(const u_int &c)
                {
                    r = ((((u_char)0xff) << RED_BIT_SHIFT  ) & c) >> RED_BIT_SHIFT;
                    g = ((((u_char)0xff) << GREEN_BIT_SHIFT) & c) >> GREEN_BIT_SHIFT;
                    b = ((((u_char)0xff) << BLUE_BIT_SHIFT ) & c) >> BLUE_BIT_SHIFT;
                }
    //------------------------------------------------------------------------
    LaserBoy_color(u_short hue_index)
                {
                    u_int shift;
                    hue_index = hue_index % 1530;
                    shift     = hue_index % 255;
                    if(hue_index < 255)
                    {
                        r = 255;
                        g = shift;
                        b = 0;
                    }
                    else if(hue_index < 510)
                    {
                        r = 255 - shift;
                        g = 255;
                        b = 0;
                    }
                    else if(hue_index < 765)
                    {
                        r = 0;
                        g = 255;
                        b = shift;
                    }
                    else if(hue_index < 1020)
                    {
                        r = 0;
                        g = 255 - shift;
                        b = 255;
                    }
                    else if(hue_index < 1275)
                    {
                        r = shift;
                        g = 0;
                        b = 255;
                    }
                    else
                    {
                        r = 255;
                        g = 0;
                        b = 255 - shift;
                    }
                }
    //------------------------------------------------------------------------
   ~LaserBoy_color()
    {}
    //------------------------------------------------------------------------
    LaserBoy_color& operator = (const LaserBoy_color &c)
                {
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    return *this;
                }
    //------------------------------------------------------------------------
    LaserBoy_color& operator = (const u_char &c)
                {
                    r = c;
                    g = c;
                    b = c;
                    return *this;
                }
    //------------------------------------------------------------------------
    LaserBoy_color& operator = (const u_int &i)
                {
                    *this = LaserBoy_color(i);
                    return *this;
                }
    //------------------------------------------------------------------------
    LaserBoy_color  operator - (const LaserBoy_color &c) const
            {
                return LaserBoy_color(r - c.r, g - c.g, b - c.b);
            }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_color &c) const
            {
                return(as_uint() == c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator != (const LaserBoy_color &c) const
            {
                return(as_uint() != c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator >  (const LaserBoy_color &c) const
            {
                return(as_uint() > c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator >= (const LaserBoy_color &c) const
            {
                return(as_uint() >= c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator <  (const LaserBoy_color &c) const
            {
                return(as_uint() < c.as_uint());
            }
    //------------------------------------------------------------------------
    bool operator <=  (const LaserBoy_color &c) const
            {
                return(as_uint() <= c.as_uint());
            }
    //------------------------------------------------------------------------
    bool is_black()
            {
                return (r + g + b) == 0;
            }
    //------------------------------------------------------------------------
    bool is_color(int black_level = 0) const
            {
                return ((r + g + b) > black_level);
            }
    //------------------------------------------------------------------------
    bool from_ifstream_ild (std::ifstream &in)
            {
                r = (u_char)in.get();
                g = (u_char)in.get();
                b = (u_char)in.get();
                return in.good();
            }
    //------------------------------------------------------------------------
    void  to_ofstream_ild (std::ofstream &out) const
            {
                out.put((char)r);
                out.put((char)g);
                out.put((char)b);
                return;
            }
    //------------------------------------------------------------------------
    bool from_ifstream_txt (std::ifstream &in, const u_int &group_type, int &line_number, string &report)
            {
                bool     got_data = false;
                int      next_char;
                double   number;
                clear_to_token(in, next_char, line_number);
                if(isdigit(next_char))
                {
                    if(get_next_number(in, number, line_number))
                    {
                        if(group_type == LASERBOY_HEX)
                        {
                            *this = LaserBoy_color((u_int)number);
                            got_data = true;
                        }
                        else
                        {
                            if(number >= 0)
                            {
                                r = (u_char)number;
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0)
                                    {
                                        g = (u_char)number;
                                        if(get_next_number(in, number, line_number))
                                        {
                                            if(number >= 0)
                                            {
                                                got_data = true;
                                                b = (u_char)number;
                                            }
                                            else
                                                report = "color negative b value not allowed";
                                        }
                                        else
                                            report = "color missing b value";
                                    }
                                    else
                                        report = "color negative g value not allowed";
                                }
                                else
                                    report = "color missing g value";
                            }
                            else
                                report = "color negative r value not allowed";
                        }
                    }
                    else
                        report = "color missing value(s)";
                }
                return (in.good() && got_data);
            }
    //------------------------------------------------------------------------
    void  to_ofstream_txt (std::ofstream &out, const bool &save_as_hex) const
            {
                if(save_as_hex)
                    out << "    0x"
                        << hex
                        << setw(6)
                        << right
                        << setfill('0')
                        << as_uint()
                        << dec
                        << left
                        << setfill(' ')
                        << ENDL;
                else
                    out << setw(7) << (int)r
                        << setw(7) << (int)g
                        << setw(7) << (int)b
                        << ENDL;
                return;
            }
    //------------------------------------------------------------------------
    u_int as_uint() const
            {
                u_int value = 0x00000000;
                value |= (r              << RED_BIT_SHIFT  );
                value |= (g              << GREEN_BIT_SHIFT);
                value |= (b              << BLUE_BIT_SHIFT );
                value |= (((u_char)0x00) << ALPHA_BIT_SHIFT);
                return value;
            }
    //------------------------------------------------------------------------
    double weighted_gray()
            {
                 return (   (   ( 76.0 * r)
                              + (150.0 * g)
                              + ( 28.0 * b)
                            )
                          / 254.0
                        );
            }
    //------------------------------------------------------------------------
    short  sum    ()  const {    return(r + g + b);          }
    double average()  const {    return(r + g + b) / 3.0;    }
    u_char bits_or()  const {    return(r | g | b);          }
    //------------------------------------------------------------------------
    LaserBoy_color  blend(const LaserBoy_color& c, double ratio)
            {
                LaserBoy_color blended;
                blended.r = (u_char)(((1.0 - ratio) * r) + (ratio * c.r) + 0.5);
                blended.g = (u_char)(((1.0 - ratio) * g) + (ratio * c.g) + 0.5);
                blended.b = (u_char)(((1.0 - ratio) * b) + (ratio * c.b) + 0.5);
                return blended;
            }
    //------------------------------------------------------------------------
    LaserBoy_color& shade(const u_char& s)
            {
                (r - s > 0) ? (r -= s) : (r = 0);
                (g - s > 0) ? (g -= s) : (g = 0);
                (b - s > 0) ? (b -= s) : (b = 0);
                return *this;
            }
    //------------------------------------------------------------------------
    LaserBoy_color& tint(const u_char& t)
            {
                (r + t <= 255) ? (r += t) : (r = 255);
                (g + t <= 255) ? (g += t) : (g = 255);
                (b + t <= 255) ? (b += t) : (b = 255);
                return *this;
            }
    //------------------------------------------------------------------------
    u_char r, g, b;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
