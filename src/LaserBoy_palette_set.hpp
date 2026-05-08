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
// LaserBoy_palette_set.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_PALETTE_SET_DEFINITIONS__
#define __LASERBOY_PALETTE_SET_DEFINITIONS__

//############################################################################
#include "LaserBoy_palette.hpp"

//############################################################################
class LaserBoy_palette_set : public vector<LaserBoy_palette>
{
public:
    LaserBoy_palette_set()
                  : vector<LaserBoy_palette>(),
                    palette_set_error       (LASERBOY_OK),
                    palette_index           (LASERBOY_ILDA_DEFAULT),
                    target_palette_index    (0),
                    selected_color_index    (0)
    {}
    //------------------------------------------------------------------------
   ~LaserBoy_palette_set()
    {}
    //------------------------------------------------------------------------
    LaserBoy_palette_set& replace_palette_set(const LaserBoy_palette_set& palette_set)
    {
        clear();
        insert(begin(), palette_set.begin(), palette_set.end());
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette_set& operator = (const LaserBoy_palette_set& palette_set)
    {
        clear();
        reserve(palette_set.size());
        insert(begin(), palette_set.begin(), palette_set.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette_set& operator += (const LaserBoy_palette_set& palette_set)
    {
        reserve(size() + palette_set.size());
        insert(end(), palette_set.begin(), palette_set.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette_set& operator += (const LaserBoy_palette& palette)
    {
        push_back(palette);
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette_set& push_back_palette(const LaserBoy_palette& palette)
    {
        push_back(palette);
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    void  set_palette_index(int index) //sets palette_index
    {
        if(number_of_palettes())
        {
            if(index >= number_of_palettes())
                index = 0;
            if(index < 0)
                index = number_of_palettes() - 1;
            palette_index = index;
        }
        else
            palette_index = 0;
    }
    //------------------------------------------------------------------------
    void  set_target_palette_index(int index) //sets target_palette_index
    {
        if(number_of_palettes())
        {
            if(index >= number_of_palettes())
                index = 0;
            if(index < 0)
                index = number_of_palettes() - 1;
            target_palette_index = index;
        }
        else
            target_palette_index = 0;
    }
    //------------------------------------------------------------------------
    LaserBoy_palette&  palette(int index) //sets palette_index
    {
        if(number_of_palettes())
        {
            if(index >= number_of_palettes())
                index = 0;
            if(index < 0)
                index = number_of_palettes() - 1;
            palette_index = index;
            return at(palette_index);
        }
        return front();
    }
    //------------------------------------------------------------------------
    LaserBoy_palette&  set_target_palette_to(int index) //sets target_palette_index
    {
        if(number_of_palettes())
        {
            if(index >= number_of_palettes())
                index = 0;
            if(index < 0)
                index = number_of_palettes() - 1;
            target_palette_index = index;
            return at(target_palette_index);
        }
        return front();
    }
    //------------------------------------------------------------------------
    LaserBoy_palette& current_palette()
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
            return at(palette_index);
        return at(target_palette_index);
    }
    //------------------------------------------------------------------------
    LaserBoy_palette& target_palette()
    {
        return at(target_palette_index);
    }
    //------------------------------------------------------------------------
    int  name_to_palette_index(string name)
    {
        for(int i = 0; i < number_of_palettes(); i++)
            if(at(i).name == name)
                return i;
        return LASERBOY_PALETTE_NOT_FOUND; // -2
    }
    //------------------------------------------------------------------------
    bool is_palette_name(string name)
    {
        for(int i = 0; i < number_of_palettes(); i++)
            if(at(i).name == name)
                return true;
        return false;
    }
    //------------------------------------------------------------------------
    int add_unique_palette(const LaserBoy_palette& palette) // returns palette index of match or -1
    {
        for(int i = 0; i < (int)size(); i++)
            if(at(i) == palette)
                return i;
        push_back(palette);
        return -1;
    }
    //------------------------------------------------------------------------
    u_int greater_number_of_colors() const
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
              (at(palette_index).size() >= at(target_palette_index).size())
            ? (at(palette_index).size())
            : (at(target_palette_index).size());
        }
        return at(target_palette_index).size();
    }
    //------------------------------------------------------------------------
    int number_of_palettes() const
    {
        return (int)size();
    }
    //------------------------------------------------------------------------
    LaserBoy_palette&   palette_picker              (int index);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code palette_from_ifstream_txt   (std::ifstream&  in,
                                                     const u_int&    group_type,
                                                     int&            line_number,
                                                     const string&   name = ""
                                                    );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code palettes_from_txt_file      (const string& file);
    LaserBoy_Error_Code save_new_palettes_as_txt    (const string& file);
    LaserBoy_Error_Code copy_palette_from_bmp       (const string& file,
                                                     const string& name);
    //------------------------------------------------------------------------
    void                populate_palette_set        ();
    void                add_palette                 ();
    void                copy_palette                (int index);
    void                copy_palette_reverse        (int index);
    void                copy_palette_reorder        (int index);
    void                copy_palette_negative       (int index);
    void                copy_palette_expanded       (int index);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  palette_set_error   ;
    int                  palette_index       ;  // may be LASERBOY_TRUE_COLOR (-1)
    u_int                target_palette_index,
                         selected_color_index;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
