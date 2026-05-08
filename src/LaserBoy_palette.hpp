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
// LaserBoy_palettes.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_PALETTE_DEFINITIONS__
#define __LASERBOY_PALETTE_DEFINITIONS__

//############################################################################
#include "LaserBoy_bmp.hpp"
#include "LaserBoy_color.hpp"
#include "LaserBoy_ild_header.hpp"

//############################################################################
class LaserBoy_space; // pre-declairation

//############################################################################
class LaserBoy_palette : public vector<LaserBoy_color>
{
public:
    LaserBoy_palette()
              : vector<LaserBoy_color>(),
                in_use                (false),
                white                 ( 0),
                black                 ( 0),
                first                 ( 0),
                last                  ( 0),
                dud                   (  ),
                name                  ("")
    {}
    //------------------------------------------------------------------------
    LaserBoy_palette(const LaserBoy_palette& p)
              : vector<LaserBoy_color>(),
                in_use                (true     ),
                white                 (p.white  ),
                black                 (p.black  ),
                first                 (p.first  ),
                last                  (p.last   ),
                dud                   (         ),
                name                  (p.name   )
    {
        clear();
        reserve(p.size());
        insert(begin(), p.begin(), p.end());
    }
    //------------------------------------------------------------------------
   ~LaserBoy_palette()
    {}
    //------------------------------------------------------------------------
    LaserBoy_palette& operator = (const LaserBoy_palette& p)
    {
        clear();
        reserve(p.size());
        insert(end(), p.begin(), p.end());
        white   = p.white  ;
        black   = p.black  ;
        first   = p.first  ;
        last    = p.last   ;
        name    = p.name   ;
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette& operator += (const LaserBoy_palette& p)
    {
        reserve(size() + p.size());
        insert(end(), p.begin(), p.end());
        return *this;
    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_palette& p)
    {
        if(last_color_index() != p.last_color_index())
            return false;
        for(size_t i = 0; i <= last_color_index(); i++)
            if(at(i) != p.at(i))
                return false;
        return true;
    }
    //------------------------------------------------------------------------
    LaserBoy_color& operator [] (const int& index)
    {
        if(index >= 0 && index < (int)number_of_colors())
            return at(index);
        return dud;
    }
    //------------------------------------------------------------------------
    u_int last_color_index() const
    {
        if(size())
        {
            int              last_color;
            LaserBoy_color   black;
            //------------------------------------------------------------
            for(last_color = size() - 1; last_color >= 0 ; last_color--)
                if(at(last_color) != black)
                    break;
            return last_color;
        }
        return 0;
    }
    //------------------------------------------------------------------------
    size_t number_of_colors () const {    return size();    }
    //------------------------------------------------------------------------
    void   set_index_color  (const u_int&  color_index,
                             const u_char& r,
                             const u_char& g,
                             const u_char& b
                            )
    {
        if(color_index < number_of_colors())
        {
            at(color_index).r = r;
            at(color_index).g = g;
            at(color_index).b = b;
        }
    }
    //------------------------------------------------------------------------
    void   set_index_color  (const u_int&          color_index,
                             const LaserBoy_color& c
                            )
    {
        if(color_index < number_of_colors())
            at(color_index) = c;
    }
    //------------------------------------------------------------------------
    void   erase_index(u_int index)
    {
        if(index < number_of_colors())
            erase(begin() + index);
        if(size())
        {
            if(!at(size() - 1).is_black())
                push_back(LaserBoy_color());
        }
        else
            push_back(LaserBoy_color());
        find_factors();
        return;
    }
    //------------------------------------------------------------------------
    void   clear_from_index(u_int index) // to end
    {
        if(index < number_of_colors())
            erase(begin() + index, end());
        if(size())
        {
            if(!at(size() - 1).is_black())
                push_back(LaserBoy_color());
        }
        else
            push_back(LaserBoy_color());
        find_factors();
        return;
    }
    //------------------------------------------------------------------------
    void   wipe()
    {
        for(size_t i = 0; i < number_of_colors(); i++)
            at(i) = (u_char)0x00;
        return;
        find_factors();
    }
    //------------------------------------------------------------------------
    bool   add_color(LaserBoy_color c = (u_char)0x00)
    {
        if(size() < 256)
        {
            if(size() && at(size() - 1).is_black())
            {
                at(size() - 1) = c;
                push_back(LaserBoy_color(0, 0, 0));
            }
            else
                push_back(c);
            find_factors();
            return true;
        }
        find_factors();
        return false;
    }
    //------------------------------------------------------------------------
    bool   insert_color(const LaserBoy_color& c, u_int index)
    {
        if(size() < 256)
        {
            while(size() <= index)
                add_color();
            insert(begin() + index, c);
            find_factors();
            return true;
        }
        find_factors();
        return false;
    }
    //------------------------------------------------------------------------
    bool   has_color(const LaserBoy_color& c) const
    {
        for(size_t i = 0; i < size(); i++)
            if(at(i) == c)
                return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool   is_in(const LaserBoy_palette& p) const
    {
        bool all_there = true;
        for(size_t i = 0; i < size(); i++)
            all_there &= p.has_color(at(i));
        return all_there;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette&   reorder();
    LaserBoy_palette&   reverse_order();
    LaserBoy_palette&   negative();
    LaserBoy_palette&   expand();    
    //------------------------------------------------------------------------
    bool   from_ifstream_ild             (std::ifstream&             in, 
                                          const LaserBoy_ild_header& header
                                         );
    void   to_ofstream_ild               (std::ofstream& out       ) const;
    void   to_ofstream_ild_fmt3          (std::ofstream& out       ) const;

    //------------------------------------------------------------------------
    bool   from_ifstream_txt             (std::ifstream&  in,
                                          const int&      group_type,
                                          int&            line_number
                                         );
    //------------------------------------------------------------------------
    bool   save_as_txt                   (const string&  file      ) const ;
    void   to_ofstream_txt               (std::ofstream& out       ) const ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  to_bmp_palette  (struct LaserBoy_bmp* bmp ) const ;
    //------------------------------------------------------------------------
    void   find_factors                  (                         )       ;
    int    best_match                    (LaserBoy_color rgb       )       ;
    int    best_reduction                (                         )       ; // returns palette_index or -1
    void   straight_blend                (                         )       ;
    void   circular_blend                (                         )       ;
    void   shade                         (u_char shade             )       ;
    void   tint                          (u_char tint              )       ;
    bool   unite                         (const LaserBoy_palette& palette) ;
    //------------------------------------------------------------------------
    bool             in_use ;
    size_t           white  ,
                     black  ,
                     first  ,
                     last   ;
    LaserBoy_color   dud    ;
    string           name   ;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
