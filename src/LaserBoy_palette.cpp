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
// LaserBoy_palette.cpp is part of LaserBoy.
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
#include "LaserBoy_SDL_GUI.hpp"

//############################################################################
LaserBoy_palette& LaserBoy_palette::reorder()
{
    if(number_of_colors())
    {
        u_int            i,
                         j,
                         index;
        short            brightest;
        LaserBoy_color   black,
                         white(255, 255, 255);
        LaserBoy_palette reordered;
        //----------------------------------------------------------------
        reordered.reserve(size());
        sort(begin(), end()); // sort this palette
        //----------------------------------------------------------------
        for(i = 0; i < number_of_colors(); i++)
            if(at(i) != black)
            {
                reordered.push_back(at(i));
                break;
            }
        //----------------------------------------------------------------
        for(/* i from above */ ; i < number_of_colors(); i++)
            if(at(i) != reordered.back()) // remove duplicates!
                reordered.push_back(at(i));
        //----------------------------------------------------------------
        if(reordered.size() == 0)
        {
            reordered.push_back(white); // at least one real color!
            reordered.push_back(black);
            *this = reordered;
            return *this;
        }
        //----------------------------------------------------------------
        clear();
        reserve(reordered.size());
        for(j = 0; j < reordered.size(); j++)
        {
            index     = 0;
            brightest = 0;
            for(i = 0; i < reordered.size(); i++)
                if(reordered.at(i).sum() > brightest)
                {
                    brightest = reordered.at(i).sum();
                    index     = i;
                }
            push_back(reordered.at(index));
            reordered.at(index) = black;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_palette& LaserBoy_palette::reverse_order()
{
    if(number_of_colors())
    {
        int              i;
        LaserBoy_palette reversed;
        //----------------------------------------------------------------
        reversed.reserve(size());
        for(i = 0; i <= (int)last_color_index(); i++)
            reversed.push_back(at(last_color_index() - i));
        *this = reversed;
    }
    return *this;
}

//############################################################################
LaserBoy_palette& LaserBoy_palette::negative()
{
    if(number_of_colors())
    {
        u_int            i;
        LaserBoy_color   color,
                         black,
                         white(255,255,255);
        LaserBoy_palette negative;
        //----------------------------------------------------------------
        negative.reserve(size());
        for(i = 0; i < number_of_colors(); i++)
        {
            if(at(i) == black)
                color = black;
            else if(at(i) == white)
                color = white;
            else
            {
                color = LaserBoy_color(255 - at(i).r,
                                       255 - at(i).g,
                                       255 - at(i).b
                                      );
            }
            negative.push_back(color);
        }
        *this = negative;
    }
    return *this;
}

//############################################################################
LaserBoy_palette& LaserBoy_palette::expand()
{
    if(number_of_colors())
    {
        LaserBoy_palette   expanded;
        int                i;
        expanded.reserve(256);
        for(i = 0; i < 256; i++)
            expanded.push_back(at((int)((double)(number_of_colors() - 1) * i / 255.0)));
        expanded.find_factors();
        *this = expanded;
    }
    return *this;
}

//############################################################################
bool LaserBoy_palette::from_ifstream_ild(std::ifstream&             in,
                                         const LaserBoy_ild_header& header
                                        )
{
    u_int           i;
    LaserBoy_color  color;
    //------------------------------------------------------------------------
    clear();
    reserve(header.quantity);
    //------------------------------------------------------------------------
    for(i = 0; i < header.quantity; i++)
        if(color.from_ifstream_ild(in))
            push_back(color);
        else
            return false;
    find_factors();
    name = header.name;
    for(i = 0; i < 8; i++)
        if(!isprint(name[i]))
        {
            name = GUID8char();
            break;
        }
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void LaserBoy_palette::to_ofstream_ild(std::ofstream& out) const
{
    for(size_t i = 0; i < number_of_colors(); i++)
        at(i).to_ofstream_ild(out);
    if(number_of_colors() < 256)
        LaserBoy_color(0,0,0).to_ofstream_ild(out);
    return;
}

//############################################################################
void LaserBoy_palette::to_ofstream_ild_fmt3(std::ofstream& out) const
{
    for(size_t i = 0; i < size(); i++)
        at(i).to_ofstream_ild(out);
    return;
}

//############################################################################
bool LaserBoy_palette::from_ifstream_txt(std::ifstream&  in,
                                         const int&      group_type,
                                         int&            line_number
                                        )
{
    u_int           i = 255;
    LaserBoy_color  color;
    string          report;
    clear();
    while(    color.from_ifstream_txt(in, group_type, line_number, report)
           && i--
         )
        push_back(color);
    if(size())
    {
        find_factors();
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_palette::save_as_txt(const string& file) const
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        to_ofstream_txt(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
void LaserBoy_palette::to_ofstream_txt(std::ofstream& out) const
{
    u_int i;
    //------------------------------------------------------------------------
    out << "# ----------------------------------------------------------------"
        << ENDL
        << (   (number_of_colors() > 256)
             ? ("table")
             : ("palette")
           )
        << (   (p_space->save_txt_color_hex)
             ? (" hex ")
             : (" rgb ")
           )
        << name
        << ENDL;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_colors(); i++)
        at(i).to_ofstream_txt(out, p_space->save_txt_color_hex);
    //------------------------------------------------------------------------
    out << ENDL;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_palette::to_bmp_palette(struct LaserBoy_bmp* bmp) const
{
    u_int i;
    if(bmp->bpp == 1)
    {
        if(number_of_colors() == 1)
        {
            bmp_set_palette_index(bmp, 0, at(0).r, at(0).g, at(0).b);
            bmp_set_palette_index(bmp, 1, 0, 0, 0);
            bmp->black = black;
            bmp->white = white;
            return LASERBOY_OK;
        }
        return LASERBOY_QUANTITY_DATA_MISSMATCH;
    }
    else if(bmp->bpp == 4)
    {
        if(number_of_colors() <= 15)
        {
            for(i = 0; i < 16; i++)
                if(i < number_of_colors())
                    bmp_set_palette_index(bmp, i, at(i).r, at(i).g, at(i).b);
                else
                    bmp_set_palette_index(bmp, i, 0, 0, 0);
            bmp->black = black;
            bmp->white = white;
            return LASERBOY_OK;
        }
        return LASERBOY_QUANTITY_DATA_MISSMATCH;
    }
    else if(bmp->bpp == 8)
    {
        for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
            if(i < number_of_colors())
                bmp_set_palette_index(bmp, i, at(i).r, at(i).g, at(i).b);
            else
                bmp_set_palette_index(bmp, i, 0, 0, 0);
        bmp->black = black;
        bmp->white = white;
        return LASERBOY_OK;
    }
    return LASERBOY_QUANTITY_DATA_MISSMATCH;
}

//############################################################################
void LaserBoy_palette::find_factors()
{
    int i;
    //------------------------------------------------------------------------
    white = black = first = last = 0;
    if(size())
    {
        for(i = 0; i < (int)size(); i++)
        {
            if(at(black) > at(i))
              black = i;
            //----------------------------------------------------------------
            if(at(white).weighted_gray() < at(i).weighted_gray())
              white = i;
        }
        //--------------------------------------------------------------------
        for(i = 0; i < (int)size(); i++)
            if(at(i).is_color(p_space->black_level))
            {
                first = i;
                break;
            }
        //--------------------------------------------------------------------
        for(i = (int)size() - 1; i >= 0 ; i--)
            if(at(i).is_color(p_space->black_level))
            {
                last = i;
                break;
            }
    }
    return;
}

//############################################################################
int LaserBoy_palette::best_match(LaserBoy_color rgb)
{
    u_int  i,
           match;
    float  j,
           k = 255.0,
           r = rgb.r,
           g = rgb.g,
           b = rgb.b;
    //------------------------------------------------------------------------
    find_factors();
    match = black;
    //------------------------------------------------------------------------
    if(
        p_space->no_black_match
        && rgb.average() < p_space->black_level
        && rgb.average() < 0
      )
        return white;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_colors(); i++)
    {
        j =   abs(at(i).r - r)
            + abs(at(i).g - g)
            + abs(at(i).b - b);
        if(j <= k)
        {
            match = i;
            k     = j;
        }
    }
    //------------------------------------------------------------------------
    if(          rgb.is_color()
        && at(match).is_black()
      )
    {
        k = 255.0;
        for(i = 0; i < number_of_colors(); i++)
        {
            j = std::abs(at(i).weighted_gray() - rgb.weighted_gray());
            if(j <= k)
            {
                match = i;
                k     = j;
            }
        }
    }
    //------------------------------------------------------------------------
    return match;
}

//############################################################################
int LaserBoy_palette::best_reduction()
{
    if(size() > 256)
    {
        u_int            i,
                         j,
                         k = 255;
        int              l;
        LaserBoy_palette sorted;
        //----------------------------------------------------------------
        reorder();
        //----------------------------------------------------------------
        for(k = 1; k < 765; k++) // minimum difference
        {
            for(i = 0; i < size() - 1; i++)
            {
                j =   abs(at(i).r - at(i + 1).r)
                    + abs(at(i).g - at(i + 1).g)
                    + abs(at(i).b - at(i + 1).b);

                if(j <= k)
                    erase(begin() + i + 1);
            }
            if(size() <= LASERBOY_PALETTE_MAX) // if we have LASERBOY_PALETTE_MAX or less colors, we're done.
                break;
        }
        for(l = p_space->number_of_palettes() - 1; l >= 0; l--)
        {
            sorted = p_space->palette_picker(l);
            sorted.reorder();
            if(sorted == *this)
                return l;
        }
        //----------------------------------------------------------------
        find_factors();
    }
    return -1;
}

//############################################################################
void LaserBoy_palette::straight_blend()
{
    if(size())
    {
        u_int          i,
                       colors;
        LaserBoy_color c1, c2, c3, c4, c5, c6, c7;

        if(at(size() - 1).is_black())
            colors = size() - 1;
        else
            colors = size();

        switch(colors)
        {
            case  0:
                c1 = (u_char)255;
                c2 = at(0);
                clear();
                for(i = 0; i < 256; i++)
                    push_back(c1.blend(c2, i / 255.0));
                break;
            case  1:
            case  2:
                c1 = at(0);
                c2 = at(1);
                clear();
                for(i = 0; i < 255; i++)
                    push_back(c1.blend(c2, i / 254.0));
                break;
            case  3:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 127; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  4:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                clear();
                for(i = 0; i < 85; i++)
                    push_back(c1.blend(c2, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c2.blend(c3, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c3.blend(c4, i / 84.0));
                break;
            case  5:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                clear();
                for(i = 0; i < 64; i++)
                    push_back(c1.blend(c2, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c2.blend(c3, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c3.blend(c4, i / 63.0));
                for(i = 0; i < 63; i++)
                    push_back(c4.blend(c5, i / 62.0));
                break;
            case  6:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                clear();
                for(i = 0; i < 51; i++)
                    push_back(c1.blend(c2, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c2.blend(c3, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c3.blend(c4, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c4.blend(c5, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c5.blend(c6, i / 50.0));
                break;
            default:
            case  7:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(6);
                clear();
                for(i = 0; i < 43; i++)
                    push_back(c1.blend(c2, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c2.blend(c3, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c3.blend(c4, i / 42.0));
                for(i = 0; i < 42; i++)
                    push_back(c4.blend(c5, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c5.blend(c6, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c6.blend(c7, i / 41.0));
                break;
        }
    }
    push_back(LaserBoy_color());
    find_factors();
    return;
}

//############################################################################
void LaserBoy_palette::circular_blend()
{
    if(size())
    {
        u_int          i,
                       colors;
        LaserBoy_color c1, c2, c3, c4, c5, c6, c7, c8;

        if(at(size() - 1).is_black())
            colors = size() - 1;
        else
            colors = size();

        switch(colors)
        {
            case  0:
                c1 = (u_char)255;
                c2 = at(0);
                c3 = (u_char)255;
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 126; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  1:
            case  2:
                c1 = at(0);
                c2 = at(1);
                c3 = at(0);
                clear();
                for(i = 0; i < 128; i++)
                    push_back(c1.blend(c2, i / 127.0));
                for(i = 0; i < 127; i++)
                    push_back(c2.blend(c3, i / 126.0));
                break;
            case  3:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(0);
                clear();
                for(i = 0; i < 85; i++)
                    push_back(c1.blend(c2, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c2.blend(c3, i / 84.0));
                for(i = 0; i < 85; i++)
                    push_back(c3.blend(c4, i / 84.0));
                break;
            case  4:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(0);
                clear();
                for(i = 0; i < 64; i++)
                    push_back(c1.blend(c2, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c2.blend(c3, i / 63.0));
                for(i = 0; i < 64; i++)
                    push_back(c3.blend(c4, i / 63.0));
                for(i = 0; i < 63; i++)
                    push_back(c4.blend(c5, i / 62.0));
                break;
            case  5:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(0);
                clear();
                for(i = 0; i < 51; i++)
                    push_back(c1.blend(c2, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c2.blend(c3, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c3.blend(c4, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c4.blend(c5, i / 50.0));
                for(i = 0; i < 51; i++)
                    push_back(c5.blend(c6, i / 50.0));
                break;
            case  6:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(0);
                clear();
                for(i = 0; i < 43; i++)
                    push_back(c1.blend(c2, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c2.blend(c3, i / 42.0));
                for(i = 0; i < 43; i++)
                    push_back(c3.blend(c4, i / 42.0));
                for(i = 0; i < 42; i++)
                    push_back(c4.blend(c5, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c5.blend(c6, i / 41.0));
                for(i = 0; i < 42; i++)
                    push_back(c6.blend(c7, i / 41.0));
                break;
            default:
            case  7:
                c1 = at(0);
                c2 = at(1);
                c3 = at(2);
                c4 = at(3);
                c5 = at(4);
                c6 = at(5);
                c7 = at(6);
                c8 = at(0);
                clear();
                for(i = 0; i < 37; i++)
                    push_back(c1.blend(c2, i / 36.0));
                for(i = 0; i < 37; i++)
                    push_back(c2.blend(c3, i / 36.0));
                for(i = 0; i < 37; i++)
                    push_back(c3.blend(c4, i / 36.0));
                for(i = 0; i < 36; i++)
                    push_back(c4.blend(c5, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c5.blend(c6, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c6.blend(c7, i / 35.0));
                for(i = 0; i < 36; i++)
                    push_back(c7.blend(c8, i / 35.0));
                break;
        }
    }
    push_back(LaserBoy_color());
    find_factors();
    return;
}

//############################################################################
void LaserBoy_palette::shade(u_char shade) // 0 shade is no change 255 is black
{
    if(shade)
    {
        for(size_t i = 0; i < number_of_colors(); i++)
        {
            (at(i).r - shade > 0) ? (at(i).r -= shade) : (at(i).r = 0);
            (at(i).g - shade > 0) ? (at(i).g -= shade) : (at(i).g = 0);
            (at(i).b - shade > 0) ? (at(i).b -= shade) : (at(i).b = 0);
        }
    }
    return;
}

//############################################################################
void LaserBoy_palette::tint(u_char tint) // 0 tint is no change 255 is white
{
    if(tint)
    {
        for(size_t i = 0; i < number_of_colors(); i++)
        {
            (at(i).r + tint < 255) ? (at(i).r += tint) : (at(i).r = 255);
            (at(i).g + tint < 255) ? (at(i).g += tint) : (at(i).g = 255);
            (at(i).b + tint < 255) ? (at(i).b += tint) : (at(i).b = 255);
        }
    }
    return;
}

//############################################################################
bool LaserBoy_palette::unite(const LaserBoy_palette& palette)
{
    if(    palette.number_of_colors() >  0
        && palette.number_of_colors() <= LASERBOY_PALETTE_MAX
      )
    {
        u_int             i,
                          j;
        LaserBoy_palette  super_palette;
        //----------------------------------------------------------------
        super_palette.reserve(LASERBOY_PALETTE_MAX);
        //----------------------------------------------------------------
        for(i = 0; i < palette.number_of_colors(); i++)
        {
            for(j = 0; j < super_palette.number_of_colors(); j++)
                if(super_palette[j] == palette.at(i))
                    break;
            if(j == super_palette.number_of_colors())
                super_palette.push_back(palette.at(i));
        }
        //----------------------------------------------------------------
        if(super_palette.number_of_colors() <= LASERBOY_PALETTE_MAX)
        {
            super_palette.reorder();
            super_palette.find_factors();
            *this = super_palette;
            return true;
        }
        //----------------------------------------------------------------
        else
            return false;
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
