
//
// LaserBoy !!!
//
// by James Lehman
// Extra Stimulus Inc./
// james@akrobiz.com
//
// began: October 2003
//
// Copyright 2003 to 2022 James Lehman.
// This source is distributed under the terms of the GNU General Public License.
//
// LaserBoy_frame_set.cpp is part of LaserBoy.
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
LaserBoy_frame_set& LaserBoy_frame_set::operator += (const LaserBoy_frame& frame)
{
    if(p_space->show_effects_generation)
    {
        bmp_copy(p_space->p_bmp(), &(p_space->background));
        frame.render(0, p_space->p_bmp());
        p_space->p_GUI->display_space();
    }
    push_back(frame);
    shrink_to_fit();
    return *this;
}

//############################################################################
bool LaserBoy_frame_set::font_index_to_frames(const string& font_name,
                                              string&       report,
                                              bool          cleanup
                                             )
{
    if(size())
    {
        size_t     i;
        u32string  unicode;
        if(!get_utf8_index(unicode, LASERBOY_FONT_SHARE + font_name + ".utf8"))
        {
            if(!get_utf8_index(unicode, LASERBOY_UTF8_SHARE + font_name + ".utf8"))
            {
                report = font_name + ".utf8 file not found.";
                frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
                return false;
            }
        }
        if(cleanup)
        {
            if(unicode.size() < size())
            {
                for(i = 0; i < unicode.size(); i++)
                    at(i).name = as_hex_string(unicode[i]);
                for(i = unicode.size(); i < size(); i++)
                    at(i).name = "ffffffff";
                return true;
            }
            else if(unicode.size() > size())
            {
                LaserBoy_frame frame;
                for(i = 0; i < size(); i++)
                    at(i).name = as_hex_string(unicode[i]);
                for(i = size(); i < unicode.size(); i++)
                {
                    frame = LaserBoy_real_segment(unicode[i]);
                    frame.name = as_hex_string(unicode[i]);
                    push_back(frame);
                }
                return true;
            }
            else
            {
                for(i = 0; i < unicode.size(); i++)
                    at(i).name = as_hex_string(unicode[i]);
                return true;
            }
        }
        else
        {
            if(unicode.size() > size())
            {
                report = font_name + ".utf8 chars is greater than frames.";
                frame_set_error |= LASERBOY_QUANTITY_DATA_MISSMATCH;
                return false;
            }
            else if(unicode.size() < size())
            {
                report = font_name + ".utf8 chars is less than frames.";
                frame_set_error |= LASERBOY_QUANTITY_DATA_MISSMATCH;
                return false;
            }
            for(i = 0; i < unicode.size(); i++)
                at(i).name = as_hex_string(unicode[i]);
            return true;
        }
    }
    report = "empty frame_set.";
    frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
    return false;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::get_unicode_index(u32string& characters)
{
    if(size())
    {
        size_t               i;
        double               number;
        char                 buffer[5];
        u32string            sorted;
        LaserBoy_Error_Code  ret = LASERBOY_OK;

        buffer[4] = '\0';
        characters.clear();
        for(i = 0; i < size(); i++)
        {
            if(hex_string(at(i).name, number))
                characters.push_back((char32_t)number);
            else
                return LASERBOY_INVALID_UNICODE;
        }
        sorted = characters;
        sort(sorted.begin(), sorted.end());
        for(i = 1; i < sorted.size(); i++)
        {
            if(sorted[i - 1] == sorted[i])
            {
                ret = LASERBOY_REDUNDANT_UNICODE;
                break;
            }
        }
        return ret;
    }
    else
        return LASERBOY_NO_FRAMES_FOUND;
}

//############################################################################
bool LaserBoy_frame_set::save_utf8_frame_index(const string& utf8_path,
                                               string&       report,
                                               bool          cleanup
                                              )
{
    if(size())
    {
        bool           has_diacritics = false;
        size_t         i;
        double         number;
        char           buffer[5];
        u32string      characters;
        std::ofstream  out(utf8_path.c_str(), ios::out);
        if(!out.good())
            return false;
        buffer[4] = '\0';
        for(i = 0; i < size(); i++)
        {
            if(hex_string(at(i).name, number))
                characters.push_back((char32_t)number);
            else if(!cleanup)
            {
                report = at(i).name + "is not a unicode name.";
                frame_set_error |= LASERBOY_INVALID_UNICODE;
                return false;
            }
            else
            {
                report = at(i).name + "is not a unicode name.";
                frame_set_error |= LASERBOY_INVALID_UNICODE;
                characters.push_back((char32_t)0xffffffff);
            }
        }
        if(characters.size())
        {
            for(i = 0; i < characters.size(); i++)
            {
                char_utf32_to_utf8(characters[i], buffer);
                if(is_diacritic(characters[i]))
                {
                    has_diacritics = true;
                    out << ' ';
                }
                out << buffer;
                if(is_diacritic(characters[i]))
                    out << ' ';
            } // end for(i = 0; i < characters.size(); i++)
            out << endl << endl;
            out << "........................" << endl;
            for(i = 0; i < characters.size(); i++)
            {
                char_utf32_to_utf8(characters[i], buffer);
                out << buffer;
                out << "  ";
            }
            out << endl << endl;
            out << "........................" << endl;
            if(has_diacritics)
            {
                out << endl;
                out << utf8_path
                    << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
            }
            out << endl;
            out << "........................" << endl;
            for(i = 0; i < characters.size(); i++)
            {
                char_utf32_to_utf8(characters[i], buffer);
                out << "  "
                    << dec
                    << setw(8)
                    << setfill(' ')
                    << right
                    << i
                    << "    ";
                out << buffer
                    << "    0x"
                    << hex
                    << setw(8)
                    << setfill('0')
                    << right
                    << characters[i];
                if(is_diacritic(characters[i]))
                    out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                out << endl;
            } // end for(i = 0; i < characters.size(); i++)
            out << "........................" << endl;
            out.close();
            return true;
        } // end if(characters.size())
        report = "no unicode frame names found.";
        frame_set_error |= LASERBOY_EMPTY_STRING_ERROR;
        return false;
    }
    report = "empty frame_set.";
    frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
    return false;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::order_frames_by_name(string& report, bool cleanup)
{
    if(size())
    {
        LaserBoy_Error_Code ret = LASERBOY_OK;
        size_t              i,
                            j;
        double              number;
        u32string           unicode,
                            unique;
        LaserBoy_frame_set  sorted;

        for(i = 0; i < size(); i++)
        {
            at(i).is_selected = false;
            if(hex_string(at(i).name, number))
                unicode.push_back((char32_t)number);
            else
            {
                report = "frame name " + at(i).name + " is not a unicode name.";
                return LASERBOY_INVALID_UNICODE;
            }
        }
        sort(unicode.begin(), unicode.end());
        if(cleanup)
        {
            unique.push_back(unicode.at(0));
            for(i = 1; i < unicode.size(); i++)
            {
                if(unique.back() != unicode.at(i))
                    unique.push_back(unicode.at(i));
            }
        }
        else
        {
            for(i = 1; i < unicode.size(); i++)
            {
                if(unicode.at(i - 1) == unicode.at(i))
                {
                    report = "2 frames have the same name (" + as_hex_string(unicode[i]) + ").";
                    return LASERBOY_REDUNDANT_UNICODE;
                }
            }
            unique = unicode;
        }
        for(i = 0; i < unique.size(); i++)
        {
            for(j = 0; j < size(); j++)
            {
                if(    !at(j).is_selected
                    && at(j).name == as_hex_string(unique[i])
                  )
                {
                    at(j).is_selected = true;
                    sorted += at(j);
                    break;
                }
            }
        }
        *this = sorted;
        return ret;
    }
    report = "empty frame_set.";
    return LASERBOY_NO_FRAMES_FOUND;
}

//############################################################################
void LaserBoy_frame_set::render_frame(const int& index, struct LaserBoy_bmp *bmp) const
{
    bool     rotate_view   = (p_space->view_angle  != 0.0),
             offset_view   = (p_space->view_offset != 0  ),
             scale_view    = (p_space->view_scale  != 1.0);
    int      bmp_center    = (   (p_space->show_quad_view)
                               ? (bmp->yres / 4)
                               : (bmp->yres / 2)
                             ),
             bmp_3X_center = bmp_center * 3,
             bigger        =   (   ((p_space->show_bridge) ? (at(index).bridge.size()) : (0))
                                 > ((p_space->show_coda)   ? (at(index).coda.size())   : (0))
                               )
                             ? ((p_space->show_bridge) ? (at(index).bridge.size()) : (0))
                             : ((p_space->show_coda)   ? (at(index).coda.size())   : (0));
    u_int    bmp_bounds  ,
             bmp_clipping,
             bmp_blank   ,
             egg_color   ,
             spider_color,
             cursor_line  = (bmp->yres >= 1024) ? (2) : (1),
             bmp_bg_color = bmp->make_rgb(p_space->bg_color.r,
                                          p_space->bg_color.g,
                                          p_space->bg_color.b
                                         );
    double   deep = LASERBOY_SHORT_SPAN,
             half_deep = deep / 2.0,

             scale = (   (p_space->show_quad_view)
                       ? (bmp->yres / (deep * 2))
                       : (bmp->yres / deep)
                     ),
             z_depth_scale =   deep
                             / (   at(index).size()
                                 + ((p_space->show_intro) ? (at(index).intro.size()) : (0))
                                 + bigger
                               ),
             z_depth_body_offset =   half_deep
                                   - (   z_depth_scale
                                       * ((p_space->show_intro) ? (at(index).intro.size()) : (0))
                                     );
    LaserBoy_3D_double _0,
                       _1;
    LaserBoy_color     bit_masked;
    //------------------------------------------------------------------------
    bmp_bounds   = bmp->make_rgb(p_space->rendered_bounds.r,
                                 p_space->rendered_bounds.g,
                                 p_space->rendered_bounds.b
                                );
    bmp_clipping = bmp->make_rgb(p_space->rendered_clipping.r,
                                 p_space->rendered_clipping.g,
                                 p_space->rendered_clipping.b
                                );
    bmp_blank    = bmp->make_rgb(p_space->rendered_blank.r,
                                 p_space->rendered_blank.g,
                                 p_space->rendered_blank.b
                                );
    //------------------------------------------------------------------------
//    p_space->clean_screen();
    //------------------------------------------------------------------------
    if(p_space->show_fixed_bounds && !p_space->destructive_clipping)
    {
        if(p_space->show_quad_view)
        {
            bmp_put_rectangle(bmp,
                              0,
                              0,
                              bmp->yres / 2 - 1,
                              bmp->yres / 2 - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(bmp,
                              bmp->yres / 2,
                              0,
                              bmp->yres - 1,
                              bmp->yres / 2 - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(bmp,
                              0,
                              bmp->yres / 2,
                              bmp->yres / 2 - 1,
                              bmp->yres - 1,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(bmp,
                              bmp->yres / 2,
                              bmp->yres / 2,
                              bmp->yres - 1,
                              bmp->yres - 1,
                              bmp_bounds,
                              0
                             );
        } // end if(p_space->show_quad_view)
        else
            bmp_put_rectangle(bmp,
                              0,
                              0,
                              bmp->yres - 1,
                              bmp->yres - 1,
                              bmp_bounds,
                              0
                             );
    } // end if(p_space->show_fixed_bounds)
    //------------------------------------------------------------------------
    if(p_space->destructive_clipping)
    {
        if(p_space->show_quad_view)
        {
            bmp_put_thick_rectangle(bmp,
                                    0,
                                    0,
                                    bmp->yres / 2 - 1,
                                    bmp->yres / 2 - 1,
                                    bmp_clipping,
                                    2
                                   );
            bmp_put_thick_rectangle(bmp,
                                    bmp->yres / 2,
                                    0,
                                    bmp->yres - 1,
                                    bmp->yres / 2 - 1,
                                    bmp_clipping,
                                    2
                                   );
            bmp_put_thick_rectangle(bmp,
                                    0,
                                    bmp->yres / 2,
                                    bmp->yres / 2 - 1,
                                    bmp->yres - 1,
                                    bmp_clipping,
                                    2
                                   );
            bmp_put_thick_rectangle(bmp,
                                    bmp->yres / 2,
                                    bmp->yres / 2,
                                    bmp->yres - 1,
                                    bmp->yres - 1,
                                    bmp_clipping,
                                    2
                                   );
        } // end if(p_space->show_quad_view)
        else
            bmp_put_thick_rectangle(bmp,
                                    0,
                                    0,
                                    bmp->yres - 1,
                                    bmp->yres - 1,
                                    bmp_clipping,
                                    2
                                   );
    } // end if(p_space->destructive_clipping)
    //------------------------------------------------------------------------
    if(p_space->show_fixed_origin)
    {
        if(p_space->show_quad_view)
        {
            bmp_put_line_fast(bmp,
                              0,
                              bmp->yres / 4,
                              bmp->yres - 1,
                              bmp->yres / 4,
                              bmp_bounds
                             );
            bmp_put_line_fast(bmp,
                              0,
                              bmp->yres / 4 * 3,
                              bmp->yres - 1,
                              bmp->yres / 4 * 3,
                              bmp_bounds
                             );
            bmp_put_line_fast(bmp,
                              bmp->yres / 4,
                              0,
                              bmp->yres / 4,
                              bmp->yres - 1,
                              bmp_bounds
                             );
            bmp_put_line_fast(bmp,
                              bmp->yres / 4 * 3,
                              0,
                              bmp->yres / 4 * 3,
                              bmp->yres - 1,
                              bmp_bounds
                             );
        }
        else
        {
            bmp_put_line_fast(bmp,
                              0,
                              bmp->yres / 2,
                              bmp->yres - 1,
                              bmp->yres / 2,
                              bmp_bounds
                             );
            bmp_put_line_fast(bmp,
                              bmp->yres / 2,
                              0,
                              bmp->yres / 2,
                              bmp->yres - 1,
                              bmp_bounds
                             );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_floating_axis)
    {
        LaserBoy_3D_double  x1, x2, y1, y2, z1, z2;

        x1.x = LASERBOY_MIN_SHORT;
        x2.x = LASERBOY_MAX_SHORT;
        y1.y = LASERBOY_MIN_SHORT;
        y2.y = LASERBOY_MAX_SHORT;
        z1.z = LASERBOY_MIN_SHORT;
        z2.z = LASERBOY_MAX_SHORT;

        if(rotate_view)
        {
            x1 = rotate_vertex(x1, p_space->view_angle);
            x2 = rotate_vertex(x2, p_space->view_angle);
            y1 = rotate_vertex(y1, p_space->view_angle);
            y2 = rotate_vertex(y2, p_space->view_angle);
            z1 = rotate_vertex(z1, p_space->view_angle);
            z2 = rotate_vertex(z2, p_space->view_angle);
        }
        if(offset_view)
        {
            x1 += p_space->view_offset;
            x2 += p_space->view_offset;
            y1 += p_space->view_offset;
            y2 += p_space->view_offset;
            z1 += p_space->view_offset;
            z2 += p_space->view_offset;
        }
        if(scale_view)
        {
            x1 *= p_space->view_scale;
            x2 *= p_space->view_scale;
            y1 *= p_space->view_scale;
            y2 *= p_space->view_scale;
            z1 *= p_space->view_scale;
            z2 *= p_space->view_scale;
        }
        // X axis
        bmp_put_thick_line_fast(bmp,              (int)(x1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(x1.y * scale) + bmp_center)
                                   ,              (int)(x2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(x2.y * scale) + bmp_center)
                                   , bmp_bounds
                                   , p_space->rendered_line_width
                               );
        // X axis icon
        bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((x2.y - 500) * scale) + bmp_center)
                                   ,              (int)((x2.x + 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((x2.y + 500) * scale) + bmp_center)
                                   , bmp->make_rgb(255,0,0)
                                   , p_space->rendered_line_width
                               );
        bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((x2.y + 500) * scale) + bmp_center)
                                   ,              (int)((x2.x + 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((x2.y - 500) * scale) + bmp_center)
                                   , bmp->make_rgb(255,0,0)
                                   , p_space->rendered_line_width
                               );
        // Y axis
        bmp_put_thick_line_fast(bmp,              (int)(y1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(y1.y * scale) + bmp_center)
                                   ,              (int)(y2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(y2.y * scale) + bmp_center)
                                   , bmp_bounds
                                   , p_space->rendered_line_width
                               );
        // Y axis icon
        bmp_put_thick_line_fast(bmp,              (int)((y2.x - 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                   ,              (int)((y2.x      ) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                   , bmp->make_rgb(0,255,0)
                                   , p_space->rendered_line_width
                               );
        bmp_put_thick_line_fast(bmp,              (int)((y2.x + 500) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                   ,              (int)((y2.x      ) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                   , bmp->make_rgb(0,255,0)
                                   , p_space->rendered_line_width
                               );
        bmp_put_thick_line_fast(bmp,              (int)((y2.x      ) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                   ,              (int)((y2.x      ) * scale) + bmp_center
                                   , bmp->yres - ((int)((y2.y - 500) * scale) + bmp_center)
                                   , bmp->make_rgb(0,255,0)
                                   , p_space->rendered_line_width
                               );
        // Z axis
        bmp_put_thick_line_fast(bmp,              (int)(z1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(z1.y * scale) + bmp_center)
                                   ,              (int)(z2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(z2.y * scale) + bmp_center)
                                   , bmp_bounds
                                   , p_space->rendered_line_width
                               );
        if(p_space->show_Z_as_order)
        {
        // T axis icon
            bmp_put_thick_line_fast(bmp,              (int)((z2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       , bmp->make_rgb(255,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.x      ) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.x      ) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(255,255,0)
                                       , p_space->rendered_line_width
                                   );
        }
        else
        {
        // Z axis icon
            bmp_put_thick_line_fast(bmp,              (int)((z2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       ,              (int)((z2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
        }
        if(p_space->show_quad_view)
        {
            x1 = x2 = y1 = y2 = z1 = z2 = 0;
            x1.x = LASERBOY_MIN_SHORT;
            x2.x = LASERBOY_MAX_SHORT;
            y1.y = LASERBOY_MIN_SHORT;
            y2.y = LASERBOY_MAX_SHORT;
            z1.z = LASERBOY_MIN_SHORT;
            z2.z = LASERBOY_MAX_SHORT;
            // X axis front
            bmp_put_thick_line_fast(bmp,              (int)(x1.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(x1.y * scale) + bmp_3X_center)
                                       ,              (int)(x2.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(x2.y * scale) + bmp_3X_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // X axis icon front
            bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((x2.y - 500) * scale) + bmp_3X_center)
                                       ,              (int)((x2.x + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((x2.y + 500) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((x2.y + 500) * scale) + bmp_3X_center)
                                       ,              (int)((x2.x + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((x2.y - 500) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            // Y axis front
            bmp_put_thick_line_fast(bmp,               (int)(y1.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y1.y * scale) + bmp_3X_center)
                                       ,              (int)(y2.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y2.y * scale) + bmp_3X_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // Y axis icon front
            bmp_put_thick_line_fast(bmp,              (int)((y2.x - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_3X_center)
                                       ,              (int)((y2.x      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((y2.x + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_3X_center)
                                       ,              (int)((y2.x      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((y2.x      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_3X_center)
                                       ,              (int)((y2.x      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y - 500) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            // X axis top
            bmp_put_thick_line_fast(bmp,              (int)(x1.x * scale) + bmp_center
                                       , bmp->yres - ((int)(x1.z * scale) + bmp_3X_center)
                                       ,              (int)(x2.x * scale) + bmp_center
                                       , bmp->yres - ((int)(x2.z * scale) + bmp_3X_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // X axis icon top
            bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((x2.z - 500) * scale) + bmp_3X_center)
                                       ,              (int)((x2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((x2.z + 500) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((x2.x - 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((x2.z + 500) * scale) + bmp_3X_center)
                                       ,              (int)((x2.x + 500) * scale) + bmp_center
                                       , bmp->yres - ((int)((x2.z - 500) * scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            // Z axis top
            bmp_put_thick_line_fast(bmp,              (int)(z1.x * -scale) + bmp_center
                                       , bmp->yres - ((int)(z1.z * -scale) + bmp_3X_center)
                                       ,              (int)(z2.x * -scale) + bmp_center
                                       , bmp->yres - ((int)(z2.z * -scale) + bmp_3X_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // Z axis icon top
            bmp_put_thick_line_fast(bmp,              (int)((z2.x - 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                       ,              (int)((z2.x + 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.x + 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z + 500) * -scale) + bmp_3X_center)
                                       ,              (int)((z2.x - 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.x - 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                       ,              (int)((z2.x + 500) * -scale) + bmp_center
                                       , bmp->yres - ((int)((z2.z - 500) * -scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            // Y axis side
            bmp_put_thick_line_fast(bmp,              (int)(y1.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y1.y * scale) + bmp_center)
                                       ,              (int)(y2.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y2.y * scale) + bmp_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // Y axis icon side
            bmp_put_thick_line_fast(bmp,              (int)((y2.z - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                       ,              (int)((y2.z      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((y2.z + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y + 500) * scale) + bmp_center)
                                       ,              (int)((y2.z      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((y2.z      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y      ) * scale) + bmp_center)
                                       ,              (int)((y2.z      ) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((y2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            // Z axis side
            bmp_put_thick_line_fast(bmp,              (int)(z1.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(z1.y * scale) + bmp_center)
                                       ,              (int)(z2.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(z2.y * scale) + bmp_center)
                                       , bmp_bounds
                                       , p_space->rendered_line_width
                                   );
            // Z axis icon side
            bmp_put_thick_line_fast(bmp,              (int)((z2.z - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.z + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.z + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y + 500) * scale) + bmp_center)
                                       ,              (int)((z2.z - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)((z2.z - 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       ,              (int)((z2.z + 500) * scale) + bmp_3X_center
                                       , bmp->yres - ((int)((z2.y - 500) * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_floating_bounds && !p_space->destructive_clipping)
    {
        LaserBoy_3D_double  a(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            b(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            c(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            d(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            e(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            f(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            g(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT),
                            h(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT);

        if(rotate_view)
        {
            a = rotate_vertex(a, p_space->view_angle);
            b = rotate_vertex(b, p_space->view_angle);
            c = rotate_vertex(c, p_space->view_angle);
            d = rotate_vertex(d, p_space->view_angle);
            e = rotate_vertex(e, p_space->view_angle);
            f = rotate_vertex(f, p_space->view_angle);
            g = rotate_vertex(g, p_space->view_angle);
            h = rotate_vertex(h, p_space->view_angle);
        }
        if(offset_view)
        {
            a += p_space->view_offset;
            b += p_space->view_offset;
            c += p_space->view_offset;
            d += p_space->view_offset;
            e += p_space->view_offset;
            f += p_space->view_offset;
            g += p_space->view_offset;
            h += p_space->view_offset;
        }
        if(scale_view)
        {
            a *= p_space->view_scale;
            b *= p_space->view_scale;
            c *= p_space->view_scale;
            d *= p_space->view_scale;
            e *= p_space->view_scale;
            f *= p_space->view_scale;
            g *= p_space->view_scale;
            h *= p_space->view_scale;
        }

        a *= scale;
        b *= scale;
        c *= scale;
        d *= scale;
        e *= scale;
        f *= scale;
        g *= scale;
        h *= scale;

        bmp_put_line_fast(bmp,              (int)a.x + bmp_center
                             , bmp->yres - ((int)a.y + bmp_center)
                             ,              (int)b.x + bmp_center
                             , bmp->yres - ((int)b.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)b.x + bmp_center
                             , bmp->yres - ((int)b.y + bmp_center)
                             ,              (int)c.x + bmp_center
                             , bmp->yres - ((int)c.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)c.x + bmp_center
                             , bmp->yres - ((int)c.y + bmp_center)
                             ,              (int)d.x + bmp_center
                             , bmp->yres - ((int)d.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)d.x + bmp_center
                             , bmp->yres - ((int)d.y + bmp_center)
                             ,              (int)a.x + bmp_center
                             , bmp->yres - ((int)a.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)a.x + bmp_center
                             , bmp->yres - ((int)a.y + bmp_center)
                             ,              (int)e.x + bmp_center
                             , bmp->yres - ((int)e.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)b.x + bmp_center
                             , bmp->yres - ((int)b.y + bmp_center)
                             ,              (int)f.x + bmp_center
                             , bmp->yres - ((int)f.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)c.x + bmp_center
                             , bmp->yres - ((int)c.y + bmp_center)
                             ,              (int)g.x + bmp_center
                             , bmp->yres - ((int)g.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)d.x + bmp_center
                             , bmp->yres - ((int)d.y + bmp_center)
                             ,              (int)h.x + bmp_center
                             , bmp->yres - ((int)h.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)e.x + bmp_center
                             , bmp->yres - ((int)e.y + bmp_center)
                             ,              (int)f.x + bmp_center
                             , bmp->yres - ((int)f.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)f.x + bmp_center
                             , bmp->yres - ((int)f.y + bmp_center)
                             ,              (int)g.x + bmp_center
                             , bmp->yres - ((int)g.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)g.x + bmp_center
                             , bmp->yres - ((int)g.y + bmp_center)
                             ,              (int)h.x + bmp_center
                             , bmp->yres - ((int)h.y + bmp_center)
                             , bmp_bounds
                         );
        bmp_put_line_fast(bmp,              (int)h.x + bmp_center
                             , bmp->yres - ((int)h.y + bmp_center)
                             ,              (int)e.x + bmp_center
                             , bmp->yres - ((int)e.y + bmp_center)
                             , bmp_bounds
                         );
        if(p_space->show_quad_view)
        {
            a.x = LASERBOY_MIN_SHORT;
            a.y = LASERBOY_MIN_SHORT;
            a.z = 0;
            b.x = LASERBOY_MAX_SHORT;
            b.y = LASERBOY_MAX_SHORT;
            b.z = 0;
            a *= scale;
            b *= scale;

            bmp_put_rectangle(bmp,
                              (int)a.x + bmp_center,
                              (int)a.y + bmp_center,
                              (int)b.x + bmp_center,
                              (int)b.y + bmp_center,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(bmp,
                              (int)a.x + bmp_3X_center,
                              (int)a.y + bmp_center,
                              (int)b.x + bmp_3X_center,
                              (int)b.y + bmp_center,
                              bmp_bounds,
                              0
                             );
            bmp_put_rectangle(bmp,
                              (int)a.x + bmp_3X_center,
                              (int)a.y + bmp_3X_center,
                              (int)b.x + bmp_3X_center,
                              (int)b.y + bmp_3X_center,
                              bmp_bounds,
                              0
                             );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->destructive_clipping)
    {
        u_int thickness = (bmp->yres >= 1024) ? (2) : (1);
        LaserBoy_3D_double  a(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            b(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT),
                            c(LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            d(LASERBOY_MIN_SHORT, LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT),
                            e(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            f(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MIN_SHORT),
                            g(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT),
                            h(LASERBOY_MIN_SHORT, LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT);

        if(rotate_view)
        {
            a = rotate_vertex(a, p_space->view_angle);
            b = rotate_vertex(b, p_space->view_angle);
            c = rotate_vertex(c, p_space->view_angle);
            d = rotate_vertex(d, p_space->view_angle);
            e = rotate_vertex(e, p_space->view_angle);
            f = rotate_vertex(f, p_space->view_angle);
            g = rotate_vertex(g, p_space->view_angle);
            h = rotate_vertex(h, p_space->view_angle);
        }
        if(offset_view)
        {
            a += p_space->view_offset;
            b += p_space->view_offset;
            c += p_space->view_offset;
            d += p_space->view_offset;
            e += p_space->view_offset;
            f += p_space->view_offset;
            g += p_space->view_offset;
            h += p_space->view_offset;
        }
        if(scale_view)
        {
            a *= p_space->view_scale;
            b *= p_space->view_scale;
            c *= p_space->view_scale;
            d *= p_space->view_scale;
            e *= p_space->view_scale;
            f *= p_space->view_scale;
            g *= p_space->view_scale;
            h *= p_space->view_scale;
        }

        a *= scale;
        b *= scale;
        c *= scale;
        d *= scale;
        e *= scale;
        f *= scale;
        g *= scale;
        h *= scale;

        bmp_put_thick_line_fast(bmp,              (int)a.x + bmp_center
                                   , bmp->yres - ((int)a.y + bmp_center)
                                   ,              (int)b.x + bmp_center
                                   , bmp->yres - ((int)b.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)b.x + bmp_center
                                   , bmp->yres - ((int)b.y + bmp_center)
                                   ,              (int)c.x + bmp_center
                                   , bmp->yres - ((int)c.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)c.x + bmp_center
                                   , bmp->yres - ((int)c.y + bmp_center)
                                   ,              (int)d.x + bmp_center
                                   , bmp->yres - ((int)d.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)d.x + bmp_center
                                   , bmp->yres - ((int)d.y + bmp_center)
                                   ,              (int)a.x + bmp_center
                                   , bmp->yres - ((int)a.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)a.x + bmp_center
                                   , bmp->yres - ((int)a.y + bmp_center)
                                   ,              (int)e.x + bmp_center
                                   , bmp->yres - ((int)e.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)b.x + bmp_center
                                   , bmp->yres - ((int)b.y + bmp_center)
                                   ,              (int)f.x + bmp_center
                                   , bmp->yres - ((int)f.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)c.x + bmp_center
                                   , bmp->yres - ((int)c.y + bmp_center)
                                   ,              (int)g.x + bmp_center
                                   , bmp->yres - ((int)g.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)d.x + bmp_center
                                   , bmp->yres - ((int)d.y + bmp_center)
                                   ,              (int)h.x + bmp_center
                                   , bmp->yres - ((int)h.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)e.x + bmp_center
                                   , bmp->yres - ((int)e.y + bmp_center)
                                   ,              (int)f.x + bmp_center
                                   , bmp->yres - ((int)f.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)f.x + bmp_center
                                   , bmp->yres - ((int)f.y + bmp_center)
                                   ,              (int)g.x + bmp_center
                                   , bmp->yres - ((int)g.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)g.x + bmp_center
                                   , bmp->yres - ((int)g.y + bmp_center)
                                   ,              (int)h.x + bmp_center
                                   , bmp->yres - ((int)h.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        bmp_put_thick_line_fast(bmp,              (int)h.x + bmp_center
                                   , bmp->yres - ((int)h.y + bmp_center)
                                   ,              (int)e.x + bmp_center
                                   , bmp->yres - ((int)e.y + bmp_center)
                                   , bmp_clipping
                                   , thickness
                               );
        if(p_space->show_quad_view)
        {
            a.x = LASERBOY_MIN_SHORT;
            a.y = LASERBOY_MIN_SHORT;
            a.z = 0;
            b.x = LASERBOY_MAX_SHORT;
            b.y = LASERBOY_MAX_SHORT;
            b.z = 0;
            a *= scale;
            b *= scale;

            bmp_put_thick_rectangle(bmp,
                                    (int)a.x + bmp_center,
                                    (int)a.y + bmp_center,
                                    (int)b.x + bmp_center,
                                    (int)b.y + bmp_center,
                                    bmp_clipping,
                                    thickness
                                   );
            bmp_put_thick_rectangle(bmp,
                                    (int)a.x + bmp_3X_center,
                                    (int)a.y + bmp_center,
                                    (int)b.x + bmp_3X_center,
                                    (int)b.y + bmp_center,
                                    bmp_clipping,
                                    thickness
                                   );
            bmp_put_thick_rectangle(bmp,
                                    (int)a.x + bmp_3X_center,
                                    (int)a.y + bmp_3X_center,
                                    (int)b.x + bmp_3X_center,
                                    (int)b.y + bmp_3X_center,
                                    bmp_clipping,
                                    thickness
                                   );
        }
    }
    //------------------------------------------------------------------------
    if(p_space->show_fulcrum)
    {
        LaserBoy_3D_double  x1, x2, y1, y2, z1, z2;

        x1.x = -5000;
        x2.x =  5000;
        y1.y = -5000;
        y2.y =  5000;
        z1.z = -5000;
        z2.z =  5000;

        x1 = x1 += p_space->fulcrum;
        x2 = x2 += p_space->fulcrum;
        y1 = y1 += p_space->fulcrum;
        y2 = y2 += p_space->fulcrum;
        z1 = z1 += p_space->fulcrum;
        z2 = z2 += p_space->fulcrum;

        if(rotate_view)
        {
            x1 = rotate_vertex(x1, p_space->view_angle);
            x2 = rotate_vertex(x2, p_space->view_angle);
            y1 = rotate_vertex(y1, p_space->view_angle);
            y2 = rotate_vertex(y2, p_space->view_angle);
            z1 = rotate_vertex(z1, p_space->view_angle);
            z2 = rotate_vertex(z2, p_space->view_angle);
        }
        if(offset_view)
        {
            x1 += p_space->view_offset;
            x2 += p_space->view_offset;
            y1 += p_space->view_offset;
            y2 += p_space->view_offset;
            z1 += p_space->view_offset;
            z2 += p_space->view_offset;
        }
        if(scale_view)
        {
            x1 *= p_space->view_scale;
            x2 *= p_space->view_scale;
            y1 *= p_space->view_scale;
            y2 *= p_space->view_scale;
            z1 *= p_space->view_scale;
            z2 *= p_space->view_scale;
        }
        bmp_put_thick_line_fast(bmp,              (int)(x1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(x1.y * scale) + bmp_center)
                                   ,              (int)(x2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(x2.y * scale) + bmp_center)
                                   , bmp->make_rgb(255,0,0)
                                   , p_space->rendered_line_width
                               );
        bmp_put_thick_line_fast(bmp,              (int)(y1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(y1.y * scale) + bmp_center)
                                   ,              (int)(y2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(y2.y * scale) + bmp_center)
                                   , bmp->make_rgb(0,255,0)
                                   , p_space->rendered_line_width
                               );
        bmp_put_thick_line_fast(bmp,              (int)(z1.x * scale) + bmp_center
                                   , bmp->yres - ((int)(z1.y * scale) + bmp_center)
                                   ,              (int)(z2.x * scale) + bmp_center
                                   , bmp->yres - ((int)(z2.y * scale) + bmp_center)
                                   , bmp->make_rgb(0,0,255)
                                   , p_space->rendered_line_width
                               );
        if(p_space->show_quad_view)
        {
            x1 = x2 = y1 = y2 = z1 = z2 = 0;

            x1.x = -5000;
            x2.x =  5000;
            y1.y = -5000;
            y2.y =  5000;
            z1.z = -5000;
            z2.z =  5000;

            x1 = x1 += p_space->fulcrum;
            x2 = x2 += p_space->fulcrum;
            y1 = y1 += p_space->fulcrum;
            y2 = y2 += p_space->fulcrum;
            z1 = z1 += p_space->fulcrum;
            z2 = z2 += p_space->fulcrum;

            if(offset_view)
            {
                x1 += p_space->view_offset;
                x2 += p_space->view_offset;
                y1 += p_space->view_offset;
                y2 += p_space->view_offset;
                z1 += p_space->view_offset;
                z2 += p_space->view_offset;
            }
            if(scale_view)
            {
                x1 *= p_space->view_scale;
                x2 *= p_space->view_scale;
                y1 *= p_space->view_scale;
                y2 *= p_space->view_scale;
                z1 *= p_space->view_scale;
                z2 *= p_space->view_scale;
            }
            // front XY
            bmp_put_thick_line_fast(bmp,              (int)(x1.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(x1.y * scale) + bmp_3X_center)
                                       ,              (int)(x2.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(x2.y * scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)(y1.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y1.y * scale) + bmp_3X_center)
                                       ,              (int)(y2.x * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y2.y * scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            // top XZ
            bmp_put_thick_line_fast(bmp,              (int)(x1.x *  scale) + bmp_center
                                       , bmp->yres - ((int)(x1.z * -scale) + bmp_3X_center)
                                       ,              (int)(x2.x *  scale) + bmp_center
                                       , bmp->yres - ((int)(x2.z * -scale) + bmp_3X_center)
                                       , bmp->make_rgb(255,0,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)(z1.x *  scale) + bmp_center
                                       , bmp->yres - ((int)(z1.z * -scale) + bmp_3X_center)
                                       ,              (int)(z2.x *  scale) + bmp_center
                                       , bmp->yres - ((int)(z2.z * -scale) + bmp_3X_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
            // side YZ
            bmp_put_thick_line_fast(bmp,              (int)(y1.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y1.y * scale) + bmp_center)
                                       ,              (int)(y2.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(y2.y * scale) + bmp_center)
                                       , bmp->make_rgb(0,255,0)
                                       , p_space->rendered_line_width
                                   );
            bmp_put_thick_line_fast(bmp,              (int)(z1.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(z1.y * scale) + bmp_center)
                                       ,              (int)(z2.z * scale) + bmp_3X_center
                                       , bmp->yres - ((int)(z2.y * scale) + bmp_center)
                                       , bmp->make_rgb(0,0,255)
                                       , p_space->rendered_line_width
                                   );
        }
    }
    //------------------------------------------------------------------------
    // ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------
    if(at(index).size() > 1)
    {
        if(p_space->show_onion_skin)
        {
            int skins =   p_space->number_of_skins <= size()
                        ? p_space->number_of_skins
                        : size();
            for(int skin = skins; skin >= 0; skin--)
            {
                if((index - skin) >= 0)
                    at(index - skin).render(skin, bmp);
                else if(p_space->loop_animation)
                    at(size() + index - skin).render(skin, bmp);
            }
        }
        else
            at(index).render(0, bmp);
        //----------------------------------------------------------------
        // END ACTUAL VECTOR ART !!!
        //----------------------------------------------------------------
        if(p_space->show_cursors)
        {
            bit_masked = at(index).at(at(index).egg).bit_masked_color(p_space->signal_bit_mask);
            egg_color  = at(index).at(at(index).egg).is_blank()
                                 ? bmp_blank
                                 :   at(index).at(at(index).egg).is_black(p_space->black_level)
                                   ? bmp->make_rgb(0xff, 0xff, 0xff)
                                   : bmp->make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
            //----------------------------------------------------------------
            bit_masked = at(index).at(at(index).spider).bit_masked_color(p_space->signal_bit_mask);
            spider_color   =   at(index).at(at(index).spider).is_blank()
                                 ? bmp_blank
                                 :   at(index).at(at(index).spider).is_black(p_space->black_level)
                                   ? bmp->make_rgb(0xff, 0xff, 0xff)
                                   : bmp->make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
            //----------------------------------------------------------------
            if(p_space->infinite_vector && (at(index).spider) > 0)
            {
                _0 = at(index).at(at(index).spider - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(index).at(at(index).spider).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (at(index).spider - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z = at(index).spider * z_depth_scale - z_depth_body_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                bmp_put_thick_infinite_line_fast(bmp,
                                                              (int)round((_0.x) * scale) + bmp_center,
                                                 bmp->yres - ((int)round((_0.y) * scale) + bmp_center),
                                                              (int)round((_1.x) * scale) + bmp_center,
                                                 bmp->yres - ((int)round((_1.y) * scale) + bmp_center),
                                                 at(index).at(at(index).spider).is_blank()
                                                   ? bmp->make_rgb(0x40, 0x40, 0x40)
                                                   :   at(index).at(at(index).spider).is_black(p_space->black_level)
                                                     ? bmp->make_rgb(0xa0, 0xa0, 0xa0)
                                                     : bmp->make_rgb(bit_masked.r / 2, bit_masked.g / 2, bit_masked.b / 2),
                                                 p_space->rendered_line_width
                                                );
                bmp_put_thick_line_fast(bmp,
                                                     (int)round((_0.x) * scale) + bmp_center,
                                        bmp->yres - ((int)round((_0.y) * scale) + bmp_center),
                                                     (int)round((_1.x) * scale) + bmp_center,
                                        bmp->yres - ((int)round((_1.y) * scale) + bmp_center),
                                        spider_color,
                                        p_space->rendered_line_width
                                       );
            }
            //----------------------------------------------------------------
            _0 = at(index).at(at(index).egg   ).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(index).at(at(index).spider).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = at(index).egg    * z_depth_scale - z_depth_body_offset;
                _1.z = at(index).spider * z_depth_scale - z_depth_body_offset;
            }
            if(rotate_view)
            {
                _0 = rotate_vertex(_0, p_space->view_angle);
                _1 = rotate_vertex(_1, p_space->view_angle);
            }
            if(offset_view)
            {
                _0 += p_space->view_offset;
                _1 += p_space->view_offset;
            }
            if(scale_view)
            {
                _0 *= p_space->view_scale;
                _1 *= p_space->view_scale;
            }
            //----------------------------------------------------------------
            // egg cursor
            bmp_put_thick_rectangle(bmp,
                                                 (int)round((_0.x - 1000) * scale) + bmp_center,
                                    bmp->yres - ((int)round((_0.y - 1000) * scale) + bmp_center),
                                                 (int)round((_0.x + 1000) * scale) + bmp_center,
                                    bmp->yres - ((int)round((_0.y + 1000) * scale) + bmp_center),
                                    egg_color,
                                    cursor_line
                                   );
            //----------------------------------------------------------------
            if(p_space->show_indices)
                bmp_printf(bmp,
                           (int)( _0.x * scale) + bmp_center,
                           (int)(-_0.y * scale) + bmp_3X_center,
                           bmp_bg_color,
                           egg_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           at(index).egg
                          );
            //----------------------------------------------------------------
            // spider cursor
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x       ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                  (int)round((_1.x       ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x       ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                  (int)round((_1.x       ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x + 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                                  (int)round((_1.x + 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x - 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                                  (int)round((_1.x - 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x - 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                  (int)round((_1.x - 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x + 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                  (int)round((_1.x + 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x + 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                  (int)round((_1.x + 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            bmp_put_thick_line_fast (bmp,
                                                  (int)round((_1.x - 400 ) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                  (int)round((_1.x - 1000) * scale) + bmp_center,
                                     bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                     spider_color,
                                     cursor_line
                                    );
            //----------------------------------------------------------------
            if(p_space->show_indices)
                bmp_printf(bmp,
                           (int)( _1.x * scale) + bmp_center,
                           (int)(-_1.y * scale) + bmp_3X_center,
                           bmp_bg_color,
                           spider_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           at(index).spider

                          );
            //----------------------------------------------------------------
            if(p_space->show_quad_view)
            {
                _0 = at(index).at(at(index).egg   ).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(index).at(at(index).spider).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = at(index).egg    * z_depth_scale - z_depth_body_offset;
                    _1.z = at(index).spider * z_depth_scale - z_depth_body_offset;
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //------------------------------------------------------------
                // egg cursor front
                bmp_put_thick_rectangle(bmp,
                                                     (int)round((_0.x - 1000) * scale) + bmp_3X_center,
                                        bmp->yres - ((int)round((_0.y - 1000) * scale) + bmp_3X_center),
                                                     (int)round((_0.x + 1000) * scale) + bmp_3X_center,
                                        bmp->yres - ((int)round((_0.y + 1000) * scale) + bmp_3X_center),
                                        egg_color,
                                        cursor_line
                                       );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)( _0.x * scale) + bmp_3X_center,
                               (int)(-_0.y * scale) + bmp_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor front
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)( _1.x * scale) + bmp_3X_center,
                               (int)(-_1.y * scale) + bmp_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
                //------------------------------------------------------------
                // egg cursor top
                bmp_put_thick_rectangle(bmp,
                                                     (int)round((_0.x - 1000) *  scale) + bmp_center,
                                        bmp->yres - ((int)round((_0.z - 1000) * -scale) + bmp_3X_center),
                                                     (int)round((_0.x + 1000) *  scale) + bmp_center,
                                        bmp->yres - ((int)round((_0.z + 1000) * -scale) + bmp_3X_center),
                                        egg_color,
                                        cursor_line
                                       );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)( _0.x * scale) + bmp_center,
                               (int)( _0.z * scale) + bmp_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor top
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x       ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x       ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x       ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x       ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z       ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z       ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x - 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z       ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x - 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z       ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x - 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x - 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.x + 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 1000) * scale) + bmp_3X_center),
                                                      (int)round((_1.x + 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z - 400 ) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z - 400 ) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 400 ) * scale) + bmp_3X_center),
                                                      (int)round((_1.z - 1000) * scale) + bmp_center,
                                         bmp->yres - ((int)round((_1.z + 1000) * scale) + bmp_3X_center),
                                         spider_color,
                                         cursor_line
                                        );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)(_1.x * scale) + bmp_center,
                               (int)(_1.z * scale) + bmp_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
                //------------------------------------------------------------
                // egg cursor side
                bmp_put_thick_rectangle(bmp,
                                                     (int)((_0.z - 1000) * scale) + bmp_3X_center,
                                        bmp->yres - ((int)((_0.y - 1000) * scale) + bmp_center),
                                                     (int)((_0.z + 1000) * scale) + bmp_3X_center,
                                        bmp->yres - ((int)((_0.y + 1000) * scale) + bmp_center),
                                        egg_color,
                                        cursor_line
                                       );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)( _0.z * scale) + bmp_3X_center,
                               (int)(-_0.y * scale) + bmp_3X_center,
                               bmp_bg_color,
                               egg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).egg
                              );
                //------------------------------------------------------------
                // spider cursor side
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                      (int)round((_1.z       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                      (int)round((_1.z       ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                                      (int)round((_1.z + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                                      (int)round((_1.z - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y       ) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                      (int)round((_1.z - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                      (int)round((_1.z + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z + 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 1000) * scale) + bmp_center),
                                                      (int)round((_1.z + 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y - 400 ) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                bmp_put_thick_line_fast (bmp,
                                                      (int)round((_1.z - 400 ) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 400 ) * scale) + bmp_center),
                                                      (int)round((_1.z - 1000) * scale) + bmp_3X_center,
                                         bmp->yres - ((int)round((_1.y + 1000) * scale) + bmp_center),
                                         spider_color,
                                         cursor_line
                                        );
                //------------------------------------------------------------
                if(p_space->show_indices)
                    bmp_printf(bmp,
                               (int)( _1.z * scale) + bmp_3X_center,
                               (int)(-_1.y * scale) + bmp_3X_center,
                               bmp_bg_color,
                               spider_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               at(index).spider
                              );
            }
            //----------------------------------------------------------------
        } // end if(p_space->show_cursors)
        //----------------------------------------------------------------
    } // end if(at(index).size() > 1)
    return;
}



//############################################################################
bool LaserBoy_frame_set::from_nothing()
{
    LaserBoy_frame frame;
    push_back(frame);
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    num_2D_frames = 0;
    num_3D_frames = 0;
    p_space->p_GUI->display_state("NO DATA");
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ild_file(const string& file)
{
    long int                   bytes_skipped;
    LaserBoy_ild_header_count  counter;
    return from_ild_file(file, bytes_skipped, counter);
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ild_file(const string&              file,
                                                      long int&                  bytes_skipped,
                                                      LaserBoy_ild_header_count& counter
                                                     )
{
    std::ifstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        from_ifstream_ild(in, bytes_skipped, counter);
        in.close();
        if(size() == 0) // create an empty frame_set
        {
            from_nothing();
            frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
        }
    }
    else // file failed to open
    {
        from_nothing();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    return frame_set_error;
}

//############################################################################
void LaserBoy_frame_set::superimpose_frame_set(LaserBoy_frame_set& overlay)
{
    size_t              i;
    LaserBoy_frame_set  sum;
    if(overlay.size() > 1 && size() > 1)
    {
        if(size() > overlay.size())
        {
            for(i = 0; i < overlay.size(); i++)
                sum += (frame(i) + overlay.frame(i));
            for(i = overlay.size(); i < size(); i++)
                sum += frame(i);
        }
        else if(size() < overlay.size())
        {
            for(i = 0; i < size(); i++)
                sum += (frame(i) + overlay.frame(i));
            for(i = size(); i < overlay.size(); i++)
                sum += overlay.frame(i);
        }
        else
            for(i = 0; i < size(); i++)
                sum += (frame(i) + overlay.frame(i));
    }
    else if(overlay.size() == 1)
        for(i = 0; i < size(); i++)
            sum += (frame(i) + overlay.frame(0));
    else if(size() == 1)
        for(i = 0; i < overlay.size(); i++)
            sum += (frame(0) + overlay.frame(i));
    *this = sum;
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::superimpose_from_txt_file(const string& file,
                                                                  int &new_frame_count,
                                                                  int &new_palette_count
                                                                 )
{
    LaserBoy_frame_set  overlay,
                        sum;
    size_t              i = p_space->number_of_palettes();

    overlay.from_txt_file(file);
    new_frame_count   = overlay.size();
    new_palette_count = p_space->number_of_palettes() - i;

    if(new_frame_count)
    {
        if(overlay.size() > 1 && size() > 1)
        {
            if(size() > overlay.size())
            {
                for(i = 0; i < overlay.size(); i++)
                    sum += (frame(i) + overlay.frame(i));

                for(i = overlay.size(); i < size(); i++)
                    sum += frame(i);
            }
            else if(size() < overlay.size())
            {
                for(i = 0; i < size(); i++)
                    sum += (frame(i) + overlay.frame(i));

                for(i = size(); i < overlay.size(); i++)
                    sum += overlay.frame(i);
            }
            else
            {
                for(i = 0; i < size(); i++)
                    sum += (frame(i) + overlay.frame(i));
            }
        }
        else if(overlay.size() == 1)
        {
            for(i = 0; i < size(); i++)
                sum += (frame(i) + overlay.frame(0));
        }
        else if(size() == 1)
        {
            for(i = 0; i < overlay.size(); i++)
                sum += (frame(0) + overlay.frame(i));
        }
        *this = sum;
    }
    frame_set_error = overlay.frame_set_error;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_dxf_file(const string& file, bool append)
{
    std::ifstream in(file.c_str(), ios::in);
    //------------------------------------------------------------------------
    if(in.is_open())
    {
        LaserBoy_frame frame;
        frame_set_error = LASERBOY_OK;
        if(!append)
        {
            clear();
            num_2D_frames = 0;
            num_3D_frames = 0;
            frame_index   = 0;
        }
        frame_set_error |= frame.from_ifstream_dxf(in);
        if(frame_set_error == LASERBOY_OK)
        {
            p_space->palette_index = frame.palette_index;
            push_back(frame);
            if(back().find_rgb_in_palette(p_space->palette_picker(LASERBOY_DXF)))
                back().palette_index = LASERBOY_DXF;
        }
        else
        {
            from_nothing();
            frame_index = 0;
        }
        in.close();
    }
    else
    {
        if(!append)
        {
            from_nothing();
            frame_index = 0;
        }
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
bool LaserBoy_frame_set::from_dxf_directory(const string& dir, bool append)
{
    int                     opened_frames = 0;
    DIR                    *pDIR;
    struct dirent          *pDirEnt;
    vector<string>          file_list;
    pDIR = opendir(dir.c_str());
    if(pDIR == NULL)
    {
        if(!append)
        {
            from_nothing();
            frame_index = 0;
            frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        }
        else
            frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    } // end if(pDIR == NULL)
    else
    {
        pDirEnt = readdir(pDIR);
        while(pDirEnt != NULL)
        {
            if(    strcmp(pDirEnt->d_name, "." ) // not the dot
                && strcmp(pDirEnt->d_name, "..") // not the dot dot
              )
                file_list.push_back(pDirEnt->d_name);
            pDirEnt = readdir(pDIR);
        }
        closedir(pDIR);
        if(file_list.size())
        {
            bool  valid_data = false;
            int   i,
                  j;
            LaserBoy_real_segment      real_vertices;
            LaserBoy_real_segment_set  real_segments;
            LaserBoy_vertex            vertex;
            LaserBoy_frame             frame;
            //----------------------------------------------------------------
            frame.format        = LASERBOY_3D_FRAME ;
            frame.palette_index = LASERBOY_TRUE_COLOR;
            //----------------------------------------------------------------
            if(!append)
            {
                clear();
                num_2D_frames = 0;
                num_3D_frames = 0;
            }
            //----------------------------------------------------------------
            // for systems that don't return file lists in alpha-numeric order!
            sort(file_list.begin(), file_list.end());
            //----------------------------------------------------------------
            p_space->p_GUI->display_state("opening DXF files");
            for(i = 0; i < (int)file_list.size(); i++)
            {
                if(    file_list[i].size() >= 5
                    && iends_with(file_list[i], ".dxf")
                  ) // ?.dxf
                {
                    std::ifstream in((dir + '/' + file_list[i]).c_str(), ios::in);
                    if(in.is_open())
                    {
                        real_vertices.from_ifstream_dxf(in);
                        if(real_vertices.size() >= 2) // there is a picture in there
                            valid_data = true;
                        real_segments.push_back(real_vertices);
                        real_vertices.clear();
                        in.close();
                        p_space->p_GUI->display_progress(file_list.size() - i);
                    }
                }
            }
            //----------------------------------------------------------------
            if(valid_data)
            {
                if(p_space->auto_scale_dxf)
                    real_segments.normalize();
                reserve(real_segments.size());
                //------------------------------------------------------------
                p_space->p_GUI->display_state("normalizing DXF frames");
                for(j = 0; j < (int)real_segments.size(); j++)
                {
                    if(real_segments[j].size() >= 4) // more than the origin vector
                    {
                        for(i = 2; i < (int)real_segments[j].size(); i++) // don't include the origin vector
                        {
                            vertex = (LaserBoy_vertex)real_segments[j][i];
                            if(real_segments[j][i].is_blank())
                                vertex.blank();
                            else
                                vertex.unblank();
                            frame.push_back(vertex);
                            opened_frames++;
                            vertex.clear();
                        }
                    } // end if(real_segments[j].size() >= 2)
                    else
                    {
                        frame.push_back(vertex);
                        frame.push_back(vertex);
                    }
                    if(frame.find_rgb_in_palette(p_space->palette_picker(LASERBOY_DXF)))
                        frame.palette_index = LASERBOY_DXF;
                    frame.is_2D();
                    push_back(frame);
                    frame.clear();
                    p_space->p_GUI->display_progress(real_segments.size() - j);
                }
                //------------------------------------------------------------
            } // end if(valid_data)
            else
            {
                frame.push_back(vertex);
                frame.push_back(vertex);
                frame.palette_index = LASERBOY_DXF;
                push_back(frame);
                opened_frames++;
            }
        //----------------------------------------------------------------
        } // end if(file_list.size())
        if(!opened_frames)
        {
            if(!append)
            {
                from_nothing();
                frame_index = 0;
                frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
            }
            else
                frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        }
    } // end else of if(pDIR == NULL)
    if(!opened_frames)
    {
        from_nothing();
        frame_index = 0;
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    file_list.clear();
    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_dxf_font(const string& file, bool append)
{
    std::ifstream in(file.c_str(), ios::in);
    //------------------------------------------------------------------------
    if(in.is_open())
    {
        u_int                      i,
                                   last_i;
        double                     the_move       = 0.0,
                                   height         = 0.0,
                                   span           = 2.0, // squares
                                   move_per_frame = 0.8; // squares
        LaserBoy_3D_double         center,
                                   max,
                                   min;
        LaserBoy_real_segment      rs1,
                                   rs2,
                                   rs3;
        LaserBoy_real_segment_set  rss;
        LaserBoy_frame_set         frames;
        LaserBoy_Error_Code        error_code = rs1.from_ifstream_dxf(in);

        if(error_code)
            return error_code;
        in.close();

        rs1.reduce_blank_vectors();
        rs1.reduce_lit_vectors();
        rs1.strip_color();
        height   =  rs1.height();
        min.x    = -(span * height) / 2.0;
        max.x    =  (span * height) / 2.0;
        min.y    = -height / 2.0;
        max.y    =  height / 2.0;
        min.z    = -32767.0;
        max.z    =  32767.0;
        center   = rs1.rectangular_center_of();
        center.x = rs1.segment_left().x;
        last_i = (u_int)(   (   (   rs1.width()
                                  / height
                                )
                                + span
                            ) // in squares
                            / move_per_frame
                        );
        the_move = height * move_per_frame;
        p_space->p_GUI->display_state("finding glyphs in dxf.");
        for(i = 0; i <= last_i; i++)
        {
            p_space->p_GUI->display_progress(last_i - i);
            rs2 = rs1;
            rs2.clip_around_coordinate(center, max, min, height * 2.0);
            if(rs2.number_of_lit_vectors() > rs3.number_of_lit_vectors())
                rs3 = rs2;
            else if(rs2.number_of_lit_vectors() < rs3.number_of_lit_vectors())
            {
                rss.push_back(rs3);
                rs3.clear();
            }
            center.x += the_move;
        }
        rss.center_x(true);
        rss.normalize(false);
        frames = rss;
        frames.normalize(0.495);
        frames.move(LaserBoy_3D_double(16384, 16384, 0));
        frames.conglomerate_lit_segments();
        frames.minimize();
        frames.convert_blank_to_black();
        if(append)
            *this += frames;
        else
            *this = frames;
    } // end if(in.is_open())
    else
    {
        if(!append)
        {
            from_nothing();
            frame_index = 0;
        }
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
bool LaserBoy_frame_set::from_LaserBoy_wave_file(const string& file, bool append)
{
    std::fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index = 0;
        //----------------------------------------------------------------
        if(header.version == "!LaserBoy!")
        {
            frame_set_error |= LASERBOY_LB_WAVE_FORMAT_ERROR;
            return false;
        }
        //----------------------------------------------------------------
        else if(header.LaserBoy_wave_mode & LASERBOY_WAVE_END_OF_FRAME)
            frame_set_error |= from_fstream_LaserBoy_wave(header, in, append);
        //----------------------------------------------------------------
        else
            frame_set_error |= from_fstream_unframed_wave(header, in, append);
        //----------------------------------------------------------------
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::from_unformatted_wave_file(const string& file, bool global_polarity, bool append)
{
    std::fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index      = 0;
        frame_set_error |= from_fstream_unformatted_wave(header, in, global_polarity, append);
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::from_qm_wave_file(const string& file, bool global_polarity, bool append)
{
    std::fstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        LaserBoy_wave_header header(in);
        frame_index      = 0;
        frame_set_error |= from_fstream_qm_wave(header, in, global_polarity, append);
        in.close();
    }
    else
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;

    if(frame_set_error)
        return false;
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ifstream_ild(std::ifstream&             in,
                                                          long int&                  bytes_skipped,
                                                          LaserBoy_ild_header_count& counter
                                                         )
{
    LaserBoy_ild_header  header           ;
    LaserBoy_frame       frame;
    LaserBoy_palette     palette;
    int                  file_sections = 0;
    //------------------------------------------------------------------------
    clear();
    frame_set_error = LASERBOY_OK;
    num_2D_frames   = 0;
    num_3D_frames   = 0;
    //------------------------------------------------------------------------
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening ild file sections");
    while(header.from_ifstream_ild(in, bytes_skipped))
    {
        if(header.quantity)
        {
            switch(header.format)
            {
                //------------------------------------------------------------
                case LASERBOY_3D_FRAME: // format 0
                case LASERBOY_2D_FRAME: // format 1
                    if(!(frame.from_ifstream_ild(in, header) & LASERBOY_EOF))
                    {
                        frame.format ? counter._1++ : counter._0++;
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        frame.palette_index = p_space->palette_index;
                        frame.set_rgb_from_palette();
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //------------------------------------------------------------
                case LASERBOY_PALETTE: // format 2
                    if(palette.from_ifstream_ild(in, header))
                    {
                        bool built_in_palette = false;
                        bool palette_defined  = false;
                        for(int i = 0; i < LASERBOY_BUILT_IN_PALETTES; i++)
                            if(header.name == p_space->palette_picker(i).name)
                            {
                                p_space->palette_index = i;
                                built_in_palette = true;
                                break;
                            }
                        for(int i = p_space->number_of_palettes() - 1; i >= 0; i--)
                        {
                            if(p_space->palette_picker(i) == palette)
                            {
                                p_space->palette_index = i;
                                palette_defined = true;
                            }
                        }
                        if(    !built_in_palette
                            && !palette_defined
                          )
                        {
                            p_space->push_back_palette(palette);
                            p_space->palette_index = p_space->number_of_palettes() - 1;
                        }
                        p_space->p_GUI->display_progress(++file_sections);
                        counter._2++;
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //------------------------------------------------------------
                case LASERBOY_COLOR_TABLE: // format 3
                    if(frame.from_ifstream_format_3(in, header, bytes_skipped))
                    {
                        frame.format ? counter._1++ : counter._0++;
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        p_space->palette_index = LASERBOY_TRUE_COLOR;
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                        frame_set_error |= frame.segment_error;
                        counter._3++;
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //------------------------------------------------------------
                case LASERBOY_3D_FRAME_RGB: // format 4
                case LASERBOY_2D_FRAME_RGB: // format 5
                    if(!(frame.from_ifstream_ild(in, header) & LASERBOY_EOF))
                    {
                        frame.format == LASERBOY_3D_FRAME_RGB ? counter._4++ : counter._5++;
                        frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                        frame.palette_index = LASERBOY_TRUE_COLOR;
                        push_back(frame);
                        p_space->p_GUI->display_progress(++file_sections);
                    }
                    else
                        frame_set_error |= LASERBOY_EOF;
                    break;
                //------------------------------------------------------------
            } // end switch(header.format)
        } // end if(header.quantity)
        else
        {
            counter._e++;
            ild_file_GUID = header.owner;
        }
    } // end while(header.from_ifstream_ild(in))
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ifstream_ctn(std::ifstream& in,
                                                          long int&      bytes_skipped
                                                         )
{
    LaserBoy_ild_header  header;
    LaserBoy_frame       frame;
    LaserBoy_palette     palette;
    int                  file_sections = 0;
    //------------------------------------------------------------------------
    clear();
    num_2D_frames   = 0;
    num_3D_frames   = 0;
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening ctn file sections");
    while(header.from_ifstream_ctn(in, bytes_skipped))
    {
        if(header.quantity)
        {
            if(!(frame.from_ifstream_ild(in, header) & LASERBOY_EOF))
            {
                frame.palette_index = p_space->palette_index;
                frame.set_rgb_from_palette();
                frame.is_2D() ? num_2D_frames++ : num_3D_frames++;
                push_back(frame);
                p_space->p_GUI->display_progress(++file_sections);
                frame_set_error |= frame.segment_error;
            }
        } // end if(header.quantity)
    } // end while(header.from_ifstream_ild(in))
    //------------------------------------------------------------------------
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_txt_file(const string& file_name, bool append)
{
                                  bool  bond_word                = false,
                                        bond_line                = false,
                                        intra_interval           = false,
                                        intra_interval_false     = false,
                                        moving_fulcrum           = false,
                                        moving_fulcrum_false     = false,
                                        norm_individually        = false,
                                        norm_with_origin         = false,
                                        true_color_dxf           = false,
                                        overwrite_txt_files      = false,
                                        include_unit_reference   = false,
                                        bmp_render_vertices      = false,
                                        bmp_render_vectors       = true,
                                        disable_bmp              = false,
                                        disable_dxf              = false,
                                        disable_txt              = false,
                                        x_of_pi                  = false,
                                        y_of_pi                  = false,
                                        z_of_pi                  = false,
                                        radii_of_pi              = false,
                                        math_success             = false,
                                        divide_by_zero           = false,
                                        lost_data                = false,
                                        save_txt_with_color_was  = p_space->save_txt_with_color,
                                        save_txt_color_hex_was   = p_space->save_txt_color_hex;
                                   int  line_number              = 1,
                                        frames                   = 3,
                                        hues_shift               = 0,
                                        hues_index_multiple      = 1;
                                 u_int  i,
                                        frame_count              = 0,
                                        still_frames             = 1,
                                        first_frames             = 1,
                                        last_frames              = 1,
                                        section_count            = 0,
                                        text_frames_rendered     = 0,
                                        math_frames_rendered     = 0,
                                        txt_frames_saved         = 0,
                                        dxf_frames_saved         = 0,
                                        bmp_frames_saved         = 0,
                                        rendered_line_width      = 1;
                                double  number,
                                        hues_span_factor         = 1.0,
                                        to_space                 = 0.90, // to frame
                                        mono_font_space          = 0.70, // per square
                                        char_width               = 1.00, // per square
                                        vari_font_gap            = 0.20, // per square
                                        vari_font_space          = 1.00, // per square
                                        super_diacritic_gap      = 0.07, // per square
                                        line_space               = 1.10, // per square
                                        view_span                = 8.00, // 8 suqares
                                        move_per_frame           = 0.15, // per square
                                        phase_cycle              = 360.0,
                                        rotation_cycle           = 1.00,
                                        interval_cycle           = 1.00,
                                        start                    = 0.00,
                                       _start                    = 0.00,
                                        duration                 = two_pi,
                                       _duration                 = two_pi,
                                        iterations               = 100.00,
                                       _iterations               = 100.00,
                                        move_acceleration        = 0.0,
                                       _move_acceleration        = 0.0,
                                        scale_acceleration       = 0.0,
                                       _scale_acceleration       = 0.0,
                                        fulcrum_acceleration     = 0.0,
                                       _fulcrum_acceleration     = 0.0,
                                        rotate_acceleration      = 0.0,
                                       _rotate_acceleration      = 0.0,
                                        rhodonea_numerator       = 1.0,
                                       _rhodonea_numerator       = 1.0,
                                        rhodonea_denominator     = 1.0,
                                       _rhodonea_denominator     = 1.0,
                                        fixed_radius             = 1.0,
                                       _fixed_radius             = 1.0,
                                        roller_radius            = 1.0,
                                       _roller_radius            = 1.0,
                                        roller_offset            = 1.0,
                                       _roller_offset            = 1.0,
                                        polar_grid_radius_max    = 1.0,
                                        polar_grid_radii_units   = 1.0,
                                        polar_grid_angle_units   = 90.0,
                                        rectangular_grid_x_units = 1.0,
                                        rectangular_grid_y_units = 1.0,
                                        rectangular_grid_z_units = 1.0;
                    LaserBoy_3D_double  displacement   (0.0, 0.0, 0.0),
                                       _displacement   (0.0, 0.0, 0.0),
                                        factor         (1.0, 1.0, 1.0),
                                        factor_        (1.0, 1.0, 1.0),
                                       _factor         (1.0, 1.0, 1.0),
                                       _factor_        (1.0, 1.0, 1.0),
                                        fulcrum        (0.0, 0.0, 0.0),
                                        fulcrum_       (0.0, 0.0, 0.0),
                                       _fulcrum        (0.0, 0.0, 0.0),
                                       _fulcrum_       (0.0, 0.0, 0.0),
                                        rotation       (0.0, 0.0, 0.0),
                                       _rotation       (0.0, 0.0, 0.0),
                                        P0          (-1.0, -1.0, -1.0),
                                       _P0          (-1.0, -1.0, -1.0),
                                        P1          ( 1.0,  1.0,  1.0),
                                       _P1          ( 1.0,  1.0,  1.0);
                                string  word,
                                        report,
                                        font_name                = "laserellipse";
                             u32string  unicode_char_list;
                           vector<int>  coordinate_real_frames_index;
                              long int  bytes_skipped            = 0;
             LaserBoy_ild_header_count  counter;
                        LaserBoy_color  color          (255, 255, 255);
                   LaserBoy_oscillator  LBO1 ,  LBO2 ,  LBO3 ,  LBO4 ,  LBO5 ,  LBO6 ,
                                        LBO1_,  LBO2_,  LBO3_,  LBO4_,  LBO5_,  LBO6_,
                                       _LBO1 , _LBO2 , _LBO3 , _LBO4 , _LBO5 , _LBO6 ,
                                       _LBO1_, _LBO2_, _LBO3_, _LBO4_, _LBO5_, _LBO6_;
    LaserBoy_oscillator* LBO_set[4][6] = {
                                             {  &LBO1 ,  &LBO2 ,  &LBO3 ,  &LBO4 ,  &LBO5 ,  &LBO6  },
                                             {  &LBO1_,  &LBO2_,  &LBO3_,  &LBO4_,  &LBO5_,  &LBO6_ },
                                             { &_LBO1 , &_LBO2 , &_LBO3 , &_LBO4 , &_LBO5 , &_LBO6  },
                                             { &_LBO1_, &_LBO2_, &_LBO3_, &_LBO4_, &_LBO5_, &_LBO6_ }
                                         };
                        LaserBoy_frame  frame;
                      LaserBoy_palette  palette;
                 LaserBoy_real_segment  real_segment;
             LaserBoy_real_segment_set  coordinate_real_segments;
                    LaserBoy_frame_set  font_frames,
                                        frame_set;
            LaserBoy_real_segment_set  *A = NULL,
                                       *B = NULL,
                                       *C = NULL;
    vector<LaserBoy_real_segment_set*>  math_list;
                         std::ifstream  in        (file_name.c_str(),   ios::in );
                         std::ofstream  error_log ("txt_in_errors.txt", ios::out);
    //------------------------------------------------------------------------
    wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    //------------------------------------------------------------------------
    if(LASERBOY_OK == font_frames.from_ild_file(LASERBOY_FONT_SHARE + font_name + ".ild", bytes_skipped, counter))
    {
        LaserBoy_Error_Code ret = font_frames.get_unicode_index(unicode_char_list);
        if(ret & LASERBOY_INVALID_UNICODE)
        {
            error_log << font_name
                      << " error: invalid unicode names."
                      << ENDL;
            error_log.close();
            return frame_set_error;
        }
        if(ret & LASERBOY_REDUNDANT_UNICODE)
        {
            error_log << font_name
                      << " error: redundant unicode names."
                      << ENDL;
            error_log.close();
            return frame_set_error;
        }
    }
    else
    {
        error_log << font_name
                  << " error: file not found."
                  << ENDL;
        error_log.close();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        return frame_set_error;
    }
    //------------------------------------------------------------------------
    if(in.is_open())
    {
        frame_index     = 0;
        frame_set_error = LASERBOY_OK;
    }
    else
    {
        error_log << file_name
                  << " error: file does not exist"
                  << ENDL;
        error_log.close();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
        return frame_set_error;
    }
    //------------------------------------------------------------------------
    if(!append)
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening text file sections");
    while(in.good())
    {
        p_space->p_GUI->display_txt_line_number(line_number);
        clear_to_alpha(in, line_number);
        if(get_next_token(in, word, line_number))
        {
            //----------------------------------------------------------------
            // comment_block_start
            // .....
            // comment_block_end
            //----------------------------------------------------------------
            if(word == "comment_block_start")
            {
                do
                {
                    if(!get_next_token(in, word, line_number))
                        break;
                    if(word == "comment_block_start")
                    {
                        error_log << file_name
                                  << " line "
                                  << line_number
                                  << ", comment_block_start found again without a comment_block_end in between"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                while(word != "comment_block_end");
                if(word != "comment_block_end")
                {
                    error_log << file_name
                              << " end of file reached before comment_block_end"
                              << ENDL;
                    frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                }
            }
            //----------------------------------------------------------------
            else if(word == "comment_block_end")
            {
                error_log << file_name
                          << " comment_block_end encountered with no comment_block_start"
                          << ENDL;
                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
            }
            //----------------------------------------------------------------
            // frame
            //----------------------------------------------------------------
            else if(word == "frame")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "xy") // 2D
                    {
                        // frame xy
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xy rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy rgb short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy rgb unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy rgb ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy rgb short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "hex")
                            {
                                // frame xy hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy hex short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy hex unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token.(frame xy hex ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy hex short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "palette")
                            {
                                // frame xy palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy palette short
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy palette unit
                                        frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. ( frame xy palette ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy palette short
                                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xy table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xy table short
                                        if(size())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xy table short ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xy table unit
                                        if(size())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_UNIT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xy table unit ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xy table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, line_number);
                                        for(size_t i = 0; i < back().size(); i++)
                                        {
                                            real_segment[i + 2].r = back()[i].r;
                                            real_segment[i + 2].g = back()[i].g;
                                            real_segment[i + 2].b = back()[i].b;
                                            real_segment[i + 2].c = back()[i].c;
                                            real_segment[i + 2].k = back()[i].k;
                                        }
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xy table real ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xy table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy table ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xy table [short]
                                    back().from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                    if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                    {
                                        error_log << file_name
                                                  << " error before line: "
                                                  << line_number
                                                  << " color table to vertex count mismatch. (frame xy table short ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                    }
                                    frame_count++;
                                }
                            } // end else if(word == "table")
                            else
                            {
                                // frame xy bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xy ERROR)"
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                        else
                        {
                            // frame xy rgb short
                            frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                            push_back(frame);
                            frame_count++;
                        }
                    }
                    else if(word == "xyz") // 3D
                    {
                        // frame xyz
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xyz rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz rgb short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz rgb unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz rgb ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz rgb short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }

                            }
                            else if(word == "hex")
                            {
                                // frame xyz hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz hex short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz hex unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz hex ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz hex short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }

                            }
                            else if(word == "palette")
                            {
                                // frame xyz palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz palette short
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz palette unit
                                        frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_UNIT, line_number);
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, line_number);
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame.clear();
                                        push_back(frame);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz palette ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {
                                    // frame xyz palette short
                                    frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, LASERBOY_SHORT, line_number);
                                    push_back(frame);
                                    frame_count++;
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xyz table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "short")
                                    {
                                        // frame xyz table short
                                        if(size())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xyz table short ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "unit")
                                    {
                                        // frame xyz table unit
                                        if(size())
                                        {
                                            back().from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_UNIT, line_number);
                                            if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                            {
                                                error_log << file_name
                                                          << " error before line: "
                                                          << line_number
                                                          << " color table to vertex count mismatch. (frame xyz table unit ERROR)"
                                                          << ENDL;
                                                frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                            }
                                            frame_count++;
                                        }
                                    }
                                    else if(word == "real")
                                    {
                                        // frame xyz table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, line_number);
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table real ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        for(size_t i = 0; i < back().size(); i++)
                                        {
                                            real_segment[i + 2].r = back()[i].r;
                                            real_segment[i + 2].g = back()[i].g;
                                            real_segment[i + 2].b = back()[i].b;
                                            real_segment[i + 2].c = back()[i].c;
                                            real_segment[i + 2].k = back()[i].k;
                                        }
                                        coordinate_real_segments.push_back(real_segment);
                                        coordinate_real_frames_index.push_back(frame_count);
                                        frame_count++;
                                    }
                                    else
                                    {
                                        // frame xyz table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz table ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                                else
                                {   // frame xyz table [short]
                                    if(size())
                                    {
                                        back().from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, LASERBOY_SHORT, line_number);
                                        if(back().segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table [short] ERROR)"
                                                      << ENDL;
                                            frame_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        frame_count++;
                                    }
                                }
                            }
                            else
                            {
                                // frame xyz bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xyz ERROR)"
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                        else
                        {
                            // frame xyz rgb short
                            frame.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, LASERBOY_SHORT, line_number);
                            push_back(frame);
                            frame_count++;
                        }
                    }
                    else
                    {
                        // frame bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (frame ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                else
                {
                    // frame xy rgb short
                    frame.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, LASERBOY_SHORT, line_number);
                    push_back(frame);
                    frame_count++;
                }
            }
            //----------------------------------------------------------------
            // palette
            //----------------------------------------------------------------
            else if(word == "palette")
            {
                // palette
                if(get_next_word(in, word, line_number))
                {
                    if(word == "named")
                    {
                        if(get_next_word(in, word, line_number))
                        {   // palette named name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_OK:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a valid LaserBoy palette name."
                                                  << ENDL;
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " new named palette \'"
                                                  << word
                                                  << "\' invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else
                        {
                            // palette named [no-name]
                            error_log << file_name
                                      << " error on line: "
                                      << line_number
                                      << " LaserBoy palette name is missing. (palette named [no-name])"
                                      << ENDL;
                            frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            word.clear();
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // this is impossible
                                        break;
                                case LASERBOY_OK:
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette added to set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "rgb")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette rgb name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette rgb [no-name]
                        {
                            word.clear();
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // impossible
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "hex")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette hex name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_HEX, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' palette name is more than 8 characters."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette hex [no-name]
                        {
                            word.clear();
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_HEX, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                        // impossible
                                        break;
                                case LASERBOY_OK:
                                        // this is what we expect.
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else
                    {
                        // palette bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (palette ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number))
                else // there was no word after palette
                {
                    // palette rgb [no-name]
                    word.clear();
                    switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                    {
                        case LASERBOY_PALETTE_NAME_IN_USE:
                        case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                // impossible
                                break;
                        case LASERBOY_OK:
                                // this is what we expect.
                                break;
                        case LASERBOY_TXT_PALETTE_FAILED:
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " invalid palette data."
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                break;
                    }
                }
            }
            //----------------------------------------------------------------
            // table
            //----------------------------------------------------------------
            else if(word == "table")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "rgb")
                    {
                        // table rgb
                        frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_RGB, line_number);
                        push_back(frame);
                    }
                    else if(word == "hex")
                    {
                        // table hex
                        frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_HEX, line_number);
                        push_back(frame);
                    }
                    else
                    {
                        // table bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (table ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                else
                {
                    // table rgb
                    frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_RGB, line_number);
                    push_back(frame);
                }
            }
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            // math
            //----------------------------------------------------------------
            else if(word == "math")
            {
                math_success = false;
                lost_data    = false;
                LBO1.phase_cycle  = phase_cycle;
                LBO1_.phase_cycle = phase_cycle;
               _LBO1.phase_cycle  = phase_cycle;
               _LBO1_.phase_cycle = phase_cycle;
                LBO2.phase_cycle  = phase_cycle;
                LBO2_.phase_cycle = phase_cycle;
               _LBO2.phase_cycle  = phase_cycle;
               _LBO2_.phase_cycle = phase_cycle;
                LBO3.phase_cycle  = phase_cycle;
                LBO3_.phase_cycle = phase_cycle;
               _LBO3.phase_cycle  = phase_cycle;
               _LBO3_.phase_cycle = phase_cycle;
                LBO4.phase_cycle  = phase_cycle;
                LBO4_.phase_cycle = phase_cycle;
               _LBO4.phase_cycle  = phase_cycle;
               _LBO4_.phase_cycle = phase_cycle;
                LBO5.phase_cycle  = phase_cycle;
                LBO5_.phase_cycle = phase_cycle;
               _LBO5.phase_cycle  = phase_cycle;
               _LBO5_.phase_cycle = phase_cycle;
                LBO6.phase_cycle  = phase_cycle;
                LBO6_.phase_cycle = phase_cycle;
               _LBO6.phase_cycle  = phase_cycle;
               _LBO6_.phase_cycle = phase_cycle;
                if(!intra_interval || intra_interval_false)
                {
                    LBO1_   =  LBO1;
                    LBO2_   =  LBO2;
                    LBO3_   =  LBO3;
                    LBO4_   =  LBO4;
                    LBO5_   =  LBO5;
                    LBO6_   =  LBO6;
                   _LBO1_   = _LBO1;
                   _LBO2_   = _LBO2;
                   _LBO3_   = _LBO3;
                   _LBO4_   = _LBO4;
                   _LBO5_   = _LBO5;
                   _LBO6_   = _LBO6;
                }
                if(!moving_fulcrum || moving_fulcrum_false)
                {
                   fulcrum_ = fulcrum;
                  _fulcrum_ =_fulcrum;
                }
                //------------------------------------------------------------
                if(get_next_word(in, word, line_number))
                {
                    if(word == "phase_cycle")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number > 0.0)
                            {
                                phase_cycle = number;
                                math_success = true;
                            }
                            else
                                divide_by_zero = true;
                        }
                        else
                            report = "missing value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rotation_cycle")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number > 0.0)
                            {
                                rotation_cycle = number;
                                math_success = true;
                            }
                            else
                                divide_by_zero = true;
                        }
                        else
                            report = "missing value";
                    }
                    //--------------------------------------------------------
                    else if(word == "intra_interval_interpolation")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            intra_interval = (bool)number;
                            if(!intra_interval)
                                intra_interval_false = true;
                            else
                                intra_interval_false = false;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "moving_fulcrum")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            moving_fulcrum = (bool)number;
                            if(!moving_fulcrum)
                                moving_fulcrum_false = true;
                            else
                                moving_fulcrum_false = false;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "to_space")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(to_space > 0.0 && to_space <= 1.0)
                            {
                                to_space     = number;
                                math_success = true;
                            }
                            else
                                report = "to_space value must be > 0.0 and <= 1.0";
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "normalize_frames_with_origin")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            norm_with_origin = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "normalize_frames_individually")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            norm_individually = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_true_color_dxf")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            true_color_dxf = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_txt_with_color")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            p_space->save_txt_with_color = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_txt_color_hex")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            p_space->save_txt_color_hex = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "overwrite_txt_files")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            overwrite_txt_files = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "include_unit_reference")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            include_unit_reference = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "bmp_render_vertices")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            bmp_render_vertices = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "bmp_render_vectors")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            bmp_render_vectors = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "bmp_line_width")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number >= 0)
                            {
                                rendered_line_width = number;
                                math_success = true;
                            }
                            else
                                report = "value must be a positive integer";
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "disable_bmp")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            disable_bmp = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "disable_dxf")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            disable_dxf = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "disable_txt")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            disable_txt = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "x_of_pi")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            x_of_pi = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "y_of_pi")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            y_of_pi = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "z_of_pi")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            z_of_pi = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "radii_of_pi")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            radii_of_pi = (bool)number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value (zero or nonzero)";
                    }
                    //--------------------------------------------------------
                    else if(word == "interval_cycle")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number > 0.0)
                            {
                                interval_cycle = number;
                                math_success = true;
                            }
                            else
                                report = "value must be positive";
                        }
                        else
                            report = "missing value";
                    }
                    //--------------------------------------------------------
                    else if(word == "hues_shift")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            hues_shift = (int)number;
                            while(hues_shift < 0)
                                hues_shift += 1530;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "hues_index_multiple")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            hues_index_multiple = (u_int)abs(number);
                            if(hues_index_multiple == 0)
                            {
                                hues_index_multiple = 1;
                                report = "hues_index_multiple cannot be 0";
                            }
                            else
                                math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "hues_span_factor")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            hues_span_factor = abs(number);
                            if(hues_span_factor == 0)
                            {
                                hues_index_multiple = 1;
                                report = "hues_span_factor cannot be 0";
                            }
                            else
                                math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "still_frames")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            still_frames = (u_int)abs(number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "first_frames")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            first_frames = (u_int)abs(number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "last_frames")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            last_frames = (u_int)abs(number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "frames")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            frames = (int)abs(number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "start")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            start = number * (two_pi / interval_cycle);
                            math_success  = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_start")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _start = number * (two_pi / interval_cycle);
                            math_success   = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "duration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            duration = abs(number) * (two_pi / interval_cycle);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_duration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _duration = abs(number) * (two_pi / interval_cycle);
                            math_success      = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "iterations")
                    {
                        if(get_next_number(in, number, line_number))
                        {

                            iterations = abs(number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_iterations")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _iterations = abs(number);
                            math_success        = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rhodonea_numerator")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rhodonea_numerator = number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_rhodonea_numerator")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _rhodonea_numerator = number;
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rhodonea_denominator")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number != 0)
                            {
                                rhodonea_denominator = number;
                                math_success = true;
                            }
                            else
                                report = "divide by zero error";
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_rhodonea_denominator")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number != 0)
                            {
                                _rhodonea_denominator = number;
                                math_success = true;
                            }
                            else
                                report = "divide by zero error";
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "fixed_radius")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            fixed_radius = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_fixed_radius")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _fixed_radius = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "roller_radius")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            roller_radius = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_roller_radius")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _roller_radius = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "roller_offset")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            roller_offset = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_roller_offset")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _roller_offset = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_radius_max")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            polar_grid_radius_max = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_radii_units")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            polar_grid_radii_units = (radii_of_pi) ? (number  * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_angle_units")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            polar_grid_angle_units = number * (two_pi / phase_cycle);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_x_units")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rectangular_grid_x_units = (x_of_pi) ? (number * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_y_units")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rectangular_grid_y_units = (y_of_pi) ? (number * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_z_units")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rectangular_grid_z_units = (z_of_pi) ? (number * pi) : (number);
                            math_success = true;
                        }
                        else
                            report = "missing numeric value";
                    }
                    //--------------------------------------------------------
                    else if(word == "displacement")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            displacement.x = (x_of_pi) ? (number * pi) : (number);
                            if(get_next_number(in, number, line_number))
                            {
                                displacement.y = (y_of_pi) ? (number * pi) : (number);
                                if(get_next_number(in, number, line_number))
                                {
                                    displacement.z = (z_of_pi) ? (number * pi) : (number);
                                    math_success = true;
                                }
                                else
                                    report = "missing displacement Z value";
                            }
                            else
                                report = "missing displacement Y value";
                        }
                        else
                            report = "missing displacement X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_displacement")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _displacement.x = (x_of_pi) ? (number * pi) : (number);
                            if(get_next_number(in, number, line_number))
                            {
                                _displacement.y = (y_of_pi) ? (number * pi) : (number);
                                if(get_next_number(in, number, line_number))
                                {
                                    _displacement.z = (z_of_pi) ? (number * pi) : (number);
                                    math_success = true;
                                }
                                else
                                    report = "missing _displacement Z value";
                            }
                            else
                                report = "missing _displacement Y value";
                        }
                        else
                            report = "missing _displacement X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "factor")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            factor.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                factor.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    factor.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing factor Z value";
                            }
                            else
                                report = "missing factor Y value";
                        }
                        else
                            report = "missing factor X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "factor_")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            factor_.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                factor_.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    factor_.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing factor_ Z value";
                            }
                            else
                                report = "missing factor_ Y value";
                        }
                        else
                            report = "missing factor_ X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_factor")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _factor.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                _factor.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    _factor.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing _factor Z value";
                            }
                            else
                                report = "missing _factor Y value";
                        }
                        else
                            report = "missing _factor X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_factor_")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _factor_.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                _factor_.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    _factor_.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing _factor_ Z value";
                            }
                            else
                                report = "missing _factor_ Y value";
                        }
                        else
                            report = "missing _factor_ X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "fulcrum")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            fulcrum.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                fulcrum.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    fulcrum.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing fulcrum Z value";
                            }
                            else
                                report = "missing fulcrum Y value";
                        }
                        else
                            report = "missing fulcrum X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "fulcrum_")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            fulcrum_.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                fulcrum_.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    fulcrum_.z     = number;
                                    math_success   = true;
                                    moving_fulcrum = true;
                                }
                                else
                                    report = "missing fulcrum_ Z value";
                            }
                            else
                                report = "missing fulcrum_ Y value";
                        }
                        else
                            report = "missing fulcrum_ X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_fulcrum")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _fulcrum.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                _fulcrum.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    _fulcrum.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing _fulcrum Z value";
                            }
                            else
                                report = "missing _fulcrum Y value";
                        }
                        else
                            report = "missing _fulcrum X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_fulcrum_")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _fulcrum_.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                _fulcrum_.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    _fulcrum_.z    = number;
                                    math_success   = true;
                                    moving_fulcrum = true;
                                }
                                else
                                    report = "missing _fulcrum_ Z value";
                            }
                            else
                                report = "missing _fulcrum_ Y value";
                        }
                        else
                            report = "missing _fulcrum_ X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rotation")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rotation.x = number * (two_pi / rotation_cycle);
                            if(get_next_number(in, number, line_number))
                            {
                                rotation.y = number * (two_pi / rotation_cycle);
                                if(get_next_number(in, number, line_number))
                                {
                                    rotation.z = number * (two_pi / rotation_cycle);
                                    math_success = true;
                                }
                                else
                                    report = "missing rotation Z value";
                            }
                            else
                                report = "missing rotation Y value";
                        }
                        else
                            report = "missing rotation X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_rotation")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _rotation.x = number * (two_pi / rotation_cycle);
                            if(get_next_number(in, number, line_number))
                            {
                                _rotation.y = number * (two_pi / rotation_cycle);
                                if(get_next_number(in, number, line_number))
                                {
                                    _rotation.z = number * (two_pi / rotation_cycle);
                                    math_success = true;
                                }
                                else
                                    report = "missing _rotation Z value";
                            }
                            else
                                report = "missing _rotation Y value";
                        }
                        else
                            report = "missing _rotation X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "P0")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(x_of_pi)
                                P0.x = number * pi;
                            else
                                P0.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                if(y_of_pi)
                                    P0.y = number * pi;
                                else
                                    P0.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    if(z_of_pi)
                                        P0.z = number * pi;
                                    else
                                        P0.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing P0 Z value";
                            }
                            else
                                report = "missing P0 Y value";
                        }
                        else
                            report = "missing P0 X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_P0")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(x_of_pi)
                                _P0.x = number * pi;
                            else
                                _P0.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                if(y_of_pi)
                                    _P0.y = number * pi;
                                else
                                    _P0.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    if(z_of_pi)
                                        _P0.z = number * pi;
                                    else
                                        _P0.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing _P0 Z value";
                            }
                            else
                                report = "missing _P0 Y value";
                        }
                        else
                            report = "missing _P0 X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "P1")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(x_of_pi)
                                P1.x = number * pi;
                            else
                                P1.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                if(y_of_pi)
                                    P1.y = number * pi;
                                else
                                    P1.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    if(z_of_pi)
                                        P1.z = number * pi;
                                    else
                                        P1.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing P1 Z value";
                            }
                            else
                                report = "missing P1 Y value";
                        }
                        else
                            report = "missing P1 X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_P1")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(x_of_pi)
                                _P1.x = number * pi;
                            else
                                _P1.x = number;
                            if(get_next_number(in, number, line_number))
                            {
                                if(y_of_pi)
                                    _P1.y = number * pi;
                                else
                                    _P1.y = number;
                                if(get_next_number(in, number, line_number))
                                {
                                    if(z_of_pi)
                                        _P1.z = number * pi;
                                    else
                                        _P1.z = number;
                                    math_success = true;
                                }
                                else
                                    report = "missing _P1 Z value";
                            }
                            else
                                report = "missing _P1 Y value";
                        }
                        else
                            report = "missing _P1 X value";
                    }
                    //--------------------------------------------------------
                    else if(word == "move_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            move_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing move_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_move_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _move_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing _move_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "scale_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            scale_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing scale_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_scale_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _scale_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing _scale_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "rotate_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            rotate_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing rotate_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_rotate_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _rotate_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing _rotate_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "fulcrum_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            fulcrum_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing fulcrum_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(word == "_fulcrum_acceleration")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            _fulcrum_acceleration = number;
                            math_success = true;
                        }
                        else
                            report = "missing _fulcrum_acceleration value";
                    }
                    //--------------------------------------------------------
                    else if(    word ==  "LBO1"
                             || word ==  "LBO1_"
                             || word == "_LBO1"
                             || word == "_LBO1_"
                             || word ==  "LBO2"
                             || word ==  "LBO2_"
                             || word == "_LBO2"
                             || word == "_LBO2_"
                             || word ==  "LBO3"
                             || word ==  "LBO3_"
                             || word == "_LBO3"
                             || word == "_LBO3_"
                             || word ==  "LBO4"
                             || word ==  "LBO4_"
                             || word == "_LBO4"
                             || word == "_LBO4_"
                             || word ==  "LBO5"
                             || word ==  "LBO5_"
                             || word == "_LBO5"
                             || word == "_LBO5_"
                             || word ==  "LBO6"
                             || word ==  "LBO6_"
                             || word == "_LBO6"
                             || word == "_LBO6_"
                           )
                    {
                        int    rank,
                               generation;
                        string lbo_name = word,
                               parameter;
                        if(lbo_name[0] == 'L')
                        {
                            generation = 0;
                            rank = atoi(word.substr(3, 1).c_str()) - 1;
                            if(word.size() == 5)
                            {
                                generation     = 1;
                                intra_interval = true;
                            }
                        }
                        else
                        {
                            generation = 2;
                            rank = atoi(word.substr(4, 1).c_str()) - 1;
                            if(word.size() == 6)
                            {
                                generation     = 3;
                                intra_interval = true;
                            }
                        }
                        if(get_next_word(in, word, line_number))
                        {
                            parameter = word;
                            if(parameter == "function")
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(LBO_set[generation][rank]->function(word))
                                        math_success = true;
                                    else
                                        report = word + " is not a valid LBO function";
                                }
                            }
                            else if(parameter == "amplitude")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->amplitude = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing amplitude value";
                            }
                            else if(parameter == "frequency")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->frequency = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing frequency value";
                            }
                            else if(parameter == "phase")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->phase = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing phase value";
                            }
                            else if(parameter == "duty_cycle")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->duty_cycle = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing duty_cycle value";
                            }
                            else if(parameter == "damping")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->damping = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing damping value";
                            }
                            else if(parameter == "offset")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    LBO_set[generation][rank]->offset = number;
                                    math_success = true;
                                }
                                else
                                    report = lbo_name + " missing offset value";
                            }
                            else if(parameter == "sin")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->sin_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "triangle")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->triangle_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "ramp")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->ramp_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "square")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->square_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "pulse")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->pulse_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "trapezoid")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->trapezoid_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "circle")
                            {
                                if(get_next_number(in, number, line_number))
                                {
                                    if(number >= 0 && number <= 1)
                                    {
                                        LBO_set[generation][rank]->circle_level = number;
                                        math_success = true;
                                    }
                                    else
                                        report = lbo_name + " mix must be from 0.0 ~ 1.0";
                                }
                                else
                                    report = lbo_name + " missing " + parameter + " mix value";
                            }
                            else if(parameter == "reset")
                            {
                                LBO_set[generation][rank]->reset();
                                math_success = true;
                            }
                            else
                                report = parameter + " is not a valie LBO parameter";
                        }
                        else
                            report = lbo_name + " missing parameter";
                    }
                    //--------------------------------------------------------
                    else if(word == "LBO_reset_all")
                    {
                         LBO1.reset();
                         LBO2.reset();
                         LBO3.reset();
                         LBO4.reset();
                         LBO5.reset();
                         LBO6.reset();
                         LBO1_.reset();
                         LBO2_.reset();
                         LBO3_.reset();
                         LBO4_.reset();
                         LBO5_.reset();
                         LBO6_.reset();
                        _LBO1.reset();
                        _LBO2.reset();
                        _LBO3.reset();
                        _LBO4.reset();
                        _LBO5.reset();
                        _LBO6.reset();
                        _LBO1_.reset();
                        _LBO2_.reset();
                        _LBO3_.reset();
                        _LBO4_.reset();
                        _LBO5_.reset();
                        _LBO6_.reset();
                        math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "reverse_frames")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->reverse_frame_order();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "collapse_frames")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->collapse_frames();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "reverse_vectors")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->reverse_vector_order();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "to_polar")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->to_polar();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "differences")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->differences();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "sums")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->sums();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "to_rectangular")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->to_rectangular();
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "size")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(A != NULL)
                            {
                                if(B != NULL)
                                {
                                    if(!A->been_used)
                                        lost_data = true;
                                    delete A;
                                    A = new LaserBoy_real_segment_set(*B);
                                }
                                else
                                    B = new LaserBoy_real_segment_set(*A);
                                if(norm_individually)
                                    for(size_t i = 0; i < B->size(); i++)
                                        if(norm_with_origin)
                                            B->at(i).to_unit_with_origin(false);
                                        else
                                            B->at(i).to_unit(false);
                                else
                                    if(norm_with_origin)
                                        B->to_unit_with_origin(false);
                                    else
                                        B->to_unit(false);
                                A->been_used = true;
                                B->scale_around_origin(number);
                                if(lost_data)
                                    report = "unused frame_set deleted at " + word;
                                else
                                    math_success = true;
                            }
                            else
                                report = "no math frames defined before " + word;
                        }
                        else
                            report = "missing value for size";
                    }
                    //--------------------------------------------------------
                    else if(word == "move")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->move(displacement);
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_move")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_move( displacement,
                                     _displacement,
                                      move_acceleration
                                    );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "spread_move")
                             || (word == "move_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->move_(displacement,
                                     move_acceleration
                                    );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "_spread_move")
                             || (word == "_move_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_move_( displacement,
                                      _displacement,
                                       move_acceleration,
                                      _move_acceleration
                                     );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "scale")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->scale_around_origin(factor);
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_scale")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_scale_around_origin( factor,
                                                    _factor,
                                                     scale_acceleration
                                                   );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "clip")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->clip(P1, P0, 1.0 / LASERBOY_EPSILON);
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_clip")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_clip( P1,
                                     _P1,
                                      P0,
                                     _P0,
                                      1.0 / LASERBOY_EPSILON
                                    );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "spread_scale")
                             || (word == "scale_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->scale_( factor,
                                       factor_,
                                       scale_acceleration
                                      );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "_spread_scale")
                             || (word == "_scale_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_scale_( factor,
                                       _factor,
                                        factor_,
                                       _factor_,
                                        scale_acceleration,
                                       _scale_acceleration
                                      );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "rotate")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->rotate_on_coordinates(fulcrum,
                                                     fulcrum_,
                                                     fulcrum_acceleration,
                                                     rotation
                                                    );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_rotate")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_rotate(fulcrum,  //  from
                                      _fulcrum,  // _from
                                       fulcrum_, //  to
                                      _fulcrum_, // _to
                                       fulcrum_acceleration,
                                      _fulcrum_acceleration,
                                       rotation,
                                      _rotation
                                      );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "spread_rotate")
                             || (word == "rotate_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->rotate_( fulcrum,
                                        fulcrum_,
                                        fulcrum_acceleration,
                                        rotation,
                                        rotate_acceleration
                                      );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(    (word == "_spread_rotate")
                             || (word == "_rotate_")
                           )
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = new LaserBoy_real_segment_set(*B);
                            }
                            else
                                B = new LaserBoy_real_segment_set(*A);
                            A->been_used = true;
                            B->_rotate_( fulcrum,  //  from
                                        _fulcrum,  // _from
                                         fulcrum_, //  to
                                        _fulcrum_, // _to
                                         fulcrum_acceleration,
                                        _fulcrum_acceleration,
                                         rotation,
                                        _rotation,
                                         rotate_acceleration,
                                        _rotate_acceleration
                                       );
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no math frames defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "swap")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = A;
                            A = B;
                            B = C;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "splice")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            *A += *B;
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "composite")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->composite(*B);
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "add")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->add(B->at(0));
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_add")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->_add(*B);
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "multiply")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->multiply(B->at(0));
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_multiply")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->_multiply(*B);
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "warp")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->warp(fulcrum,
                                    fulcrum_,
                                    fulcrum_acceleration,
                                    B->at(0)
                                   );
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_warp")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->_warp(fulcrum,  //  from
                                    _fulcrum,  // _from
                                     fulcrum_, //  to
                                    _fulcrum_, // _to
                                     fulcrum_acceleration,
                                    _fulcrum_acceleration,
                                     *B
                                    );
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "xyz_rgb")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->xyz_rgb(B->at(0));
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "_xyz_rgb")
                    {
                        if(A != NULL && B != NULL)
                        {
                            C = new LaserBoy_real_segment_set(*B);
                            A->_xyz_rgb(*B);
                            delete B;
                            B = A;
                            A = C;
                            A->been_used = true;
                            B->been_used = true;
                            math_success = true;
                        }
                        else
                            report = "two math frame_sets must be defined before " + word;
                    }
                    //--------------------------------------------------------
                    else if(word == "import_space")
                    {
                        double size = 1.0;
                        if(this->size()) // this is the current frame_set
                        {
                            if(get_next_number(in, number, line_number))
                                size = number;
                            if(A != NULL)
                            {
                                if(B != NULL)
                                {
                                    if(!A->been_used)
                                        lost_data = true;
                                    delete A;
                                    A = B;
                                }
                                B = new LaserBoy_real_segment_set(to_real_segment_set(false));
                                C = B;
                            }
                            else
                            {
                                A = new LaserBoy_real_segment_set(to_real_segment_set(false));
                                C = A;
                            }
                            if(norm_individually)
                            {
                                for(size_t i = 0; i < C->size(); i++)
                                {
                                    if(norm_with_origin)
                                        C->at(i).to_unit_with_origin(false);
                                    else
                                        C->at(i).to_unit(false);
                                }
                            }
                            else
                            {
                                if(norm_with_origin)
                                    C->to_unit_with_origin(false);
                                else
                                    C->to_unit(false);
                            }
                            C->scale_around_origin(number);
                            if(lost_data)
                                report = "unused frame_set deleted at " + word;
                            else
                                math_success = true;
                        }
                        else
                            report = "no frames currently loaded in LaserBoy app";
                    }
                    //--------------------------------------------------------
                    else if(word == "import_txt")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            LaserBoy_Error_Code error_code = LASERBOY_OK;
                            if(word.size() < 5 || !iends_with(word, ".txt"))
                                word += ".txt";
                            if(file_exists(LASERBOY_TXT_SHARE + word))
                            {
                                if(A != NULL)
                                {
                                    if(B != NULL)
                                    {
                                        if(!A->been_used)
                                            lost_data = true;
                                        delete A;
                                        A = B;
                                    }
                                    B = new LaserBoy_real_segment_set;
                                    C = B;
                                }
                                else
                                {
                                    A = new LaserBoy_real_segment_set;
                                    C = A;
                                }
                                if(!(error_code |= C->from_txt_file(LASERBOY_TXT_SHARE + word)))
                                {
                                    if(lost_data)
                                        report = "unused frame_set deleted at " + word;
                                    else
                                        math_success = true;
                                }
                                else if(error_code & LASERBOY_FILE_OPEN_FAILED)
                                    report = string("./txt/") + word + " failed to open";
                                else if(C->size() != 0)
                                    report = string("./txt/") + word + " errors reported in ./LaserBoy/txt_in_real_segment_errors.txt.";
                            }
                            else
                                report = string("./txt/") + word + " does not exist";
                        }
                        else
                            report = "missing txt file name";
                    }
                    //--------------------------------------------------------
                    else if(word == "import_dxf")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            LaserBoy_Error_Code error_code = LASERBOY_OK;
                            if(directory_exists(LASERBOY_DXF_SHARE + word, p_space->absolute_path_of_LaserBoy))
                            {
                                if(A != NULL)
                                {
                                    if(B != NULL)
                                    {
                                        if(!A->been_used)
                                            lost_data = true;
                                        delete A;
                                        A = B;
                                    }
                                    B = new LaserBoy_real_segment_set;
                                    C = B;
                                }
                                else
                                {
                                    A = new LaserBoy_real_segment_set;
                                    C = A;
                                }
                                if(!(error_code |= C->from_dxf_directory(LASERBOY_DXF_SHARE + word)))
                                {
                                    if(lost_data)
                                        report = "unused frame_set deleted at " + word;
                                    else
                                        math_success = true;
                                }
                                else if(error_code & LASERBOY_DIR_OPEN_FAILED)
                                    report = string("./dxf/") + word + " failed to open";
                                else if(error_code & LASERBOY_FILE_OPEN_FAILED)
                                    report = string("one or more files in ./dxf/") + word + " failed to open";
                            }
                            else
                                report = string("./dxf/") + word + " does not exist";
                        }
                        else
                            report = "missing dxf directory name";
                    }
                    //--------------------------------------------------------
                    else if(word == "color_rgb")
                    {
                        if(color.from_ifstream_txt(in, LASERBOY_RGB, line_number, report))
                        {
                            if(A != NULL)
                            {
                                if(B != NULL)
                                    C = B;
                                else
                                    C = A;
                                C->strip_color_rgb(color);
                                math_success = true;
                            }
                            else
                                report = "no frames defined to color";
                        }
                    }
                    //--------------------------------------------------------
                    else if(word == "color_hex")
                    {
                        if(color.from_ifstream_txt(in, LASERBOY_HEX, line_number, report))
                        {
                            if(A != NULL)
                            {
                                if(B != NULL)
                                    C = B;
                                else
                                    C = A;
                                C->strip_color_rgb(color);
                                math_success = true;
                            }
                            else
                                report = "no frames defined to color";
                        }
                    }
                    //--------------------------------------------------------
                    else if(word == "color_index_hues")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                                C = B;
                            else
                                C = A;
                            C->index_hues(hues_index_multiple, hues_shift);
                            math_success = true;
                        }
                        else
                            report = "no frames defined to color";
                    }
                    //--------------------------------------------------------
                    else if(word == "color_span_hues")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                                C = B;
                            else
                                C = A;
                            C->span_hues(hues_span_factor, hues_shift);
                            math_success = true;
                        }
                        else
                            report = "no frames defined to color";
                    }
                    //--------------------------------------------------------
                    else if(word == "color_set_index_hues")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                                C = B;
                            else
                                C = A;
                            C->index_hues_set(hues_index_multiple);
                            math_success = true;
                        }
                        else
                            report = "no frames defined to color";
                    }
                    //--------------------------------------------------------
                    else if(word == "color_set_span_hues")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                                C = B;
                            else
                                C = A;
                            C->span_hues_set(hues_span_factor);
                            math_success = true;
                        }
                        else
                            report = "no frames defined to color";
                    }
                    //--------------------------------------------------------
                    else if(word == "splice_list_from_name")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(math_list.size())
                            {
                                bool name_found = false;
                                for(size_t i = 0; i < math_list.size(); i++)
                                {
                                    if(math_list[i]->name == word)
                                    {
                                        C = new LaserBoy_real_segment_set;
                                        name_found = true;
                                        for(size_t j = i; j < math_list.size(); j++)
                                        {
                                            *C += *(math_list[j]);
                                            math_list[j]->been_used = true;
                                        }
                                        if(A != NULL)
                                        {
                                            if(B != NULL)
                                            {
                                                if(!A->been_used)
                                                    lost_data = true;
                                                delete A;
                                                A = B;
                                            }
                                            B = C;
                                        }
                                        else
                                            A = C;
                                        if(lost_data)
                                            report = "unused frame_set deleted at " + word;
                                        else
                                            math_success = true;
                                        break;
                                    }
                                } // end for(size_t i = 0; i < math_list.size(); i++)
                                if(!name_found)
                                    report = "no math frames named " + word;
                            }
                            else
                                report = "no math frames defined before splice_frame_sets_from_name";
                        }
                        else
                            report = "missing frame_set name";
                    }
                    //--------------------------------------------------------
                    else if(word == "composite_list_from_name")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(math_list.size())
                            {
                                bool name_found = false;
                                for(size_t i = 0; i < math_list.size(); i++)
                                {
                                    if(math_list[i]->name == word)
                                    {
                                        C = new LaserBoy_real_segment_set;
                                        name_found = true;
                                        *C = *(math_list[i]);
                                        for(size_t j = i + 1; j < math_list.size(); j++)
                                        {
                                            C->composite(*(math_list[j]));
                                            math_list[j]->been_used = true;
                                        }
                                        if(A != NULL)
                                        {
                                            if(B != NULL)
                                            {
                                                if(!A->been_used)
                                                    lost_data = true;
                                                delete A;
                                                A = B;
                                            }
                                            B = C;
                                        }
                                        else
                                            A = C;
                                        if(lost_data)
                                            report = "unused frame_set deleted at " + word;
                                        else
                                            math_success = true;
                                        break;
                                    }
                                } // end for(size_t i = 0; i < math_list.size(); i++)
                                if(!name_found)
                                    report = "no math frames named " + word;
                            }
                            else
                                report = "no math frames defined before splice_frame_sets_from_name";
                        }
                        else
                            report = "missing frame_set name";
                    }
                    //--------------------------------------------------------
                    else if(word == "glean_list")
                    {
                        int unused = 0;
                        for(size_t i = 0; i < math_list.size(); i++)
                        {
                            if(math_list[i]->name.size() == 0)
                            {
                                if(!math_list[i]->been_used)
                                    unused++;
                                delete math_list[i];
                                math_list.erase(math_list.begin() + i);
                            }
                        }
                        if(unused)
                            report = to_string(unused) + " unused frame_sets delted in glean_list";
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "delist")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            bool name_found = false,
                                 unused     = false;
                            for(size_t i = 0; i < math_list.size(); i++)
                            {
                                if(math_list[i]->name == word)
                                {
                                    name_found = true;
                                    if(!math_list[i]->been_used)
                                        unused = true;
                                    delete math_list[i];
                                    math_list.erase(math_list.begin() + i);
                                    break;
                                }
                            }
                            if(!name_found)
                                report = word + " not found in list";
                            else
                            {
                                if(unused)
                                    report = "unused frame_set " + word + " delted in delist";
                                math_success = true;
                            }
                        }
                        else
                            report = "missing name to delist";
                    }
                    //--------------------------------------------------------
                    else if(word == "clear_list")
                    {
                        for(size_t i = 0; i < math_list.size(); i++)
                            delete math_list[i];
                        math_list.clear();
                        math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "render")
                    {
                        if(A != NULL)
                        {
                            LaserBoy_real_segment_set rss;
                            if(include_unit_reference)
                                rss += LaserBoy_real_segment::unit_reference();
                            if(B != NULL)
                            {
                                B->been_used = true;
                                rss += *B;
                            }
                            else
                            {
                                A->been_used = true;
                                rss += *A;
                            }
                            if(norm_individually)
                            {
                                for(size_t i = 0; i < rss.size(); i++)
                                {
                                    if(norm_with_origin)
                                        rss.at(i).normalize_vectors_with_origin(false);
                                    else
                                        rss.at(i).normalize_vectors(false);
                                }
                            }
                            else
                            {
                                if(norm_with_origin)
                                    rss.normalize_vectors_with_origin(false);
                                else
                                    rss.normalize_vectors(false);
                            }
                            if(get_next_number(in, number, line_number))
                            {
                                if(number > 0.0 && number <= 1.0)
                                {
                                    rss.scale(number);
                                    *this += rss;
                                    math_frames_rendered += rss.size();
                                    math_success = true;
                                }
                                else
                                {
                                    *this += rss;
                                    math_frames_rendered += rss.size();
                                    report = "render_result to value must be > 0.0 and <= 1.0";
                                }
                            }
                            else
                            {
                                rss.scale(to_space);
                                *this += rss;
                                math_frames_rendered += rss.size();
                                math_success = true;
                            }
                        }
                        else
                            report = "no frames defined to render";
                    }
                    //--------------------------------------------------------
                    else if(word == "render_list_from_name")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(math_list.size())
                            {
                                bool   name_found = false;
                                size_t i, j;
                                for(i = 0; i < math_list.size(); i++)
                                {
                                    if(math_list[i]->name == word)
                                    {
                                        name_found = true;
                                        break;
                                    }
                                } // end for(...)
                                if(!name_found)
                                    report = "no math frames named " + word;
                                else
                                {
                                    LaserBoy_real_segment_set rss;
                                    if(include_unit_reference)
                                        rss += LaserBoy_real_segment::unit_reference();
                                    for(j = i; j < math_list.size(); j++)
                                    {
                                        rss += *(math_list[j]);
                                        math_list[j]->been_used = true;
                                    }
                                    if(norm_individually)
                                    {
                                        if(norm_with_origin)
                                            for(i = 0; i < rss.size(); i++)
                                                rss.at(i).normalize_vectors_with_origin(false);
                                        else
                                            for(i = 0; i < rss.size(); i++)
                                                rss.at(i).normalize_vectors(false);
                                    }
                                    else
                                    {
                                        if(norm_with_origin)
                                            rss.normalize_vectors_with_origin(false);
                                        else
                                            rss.normalize_vectors(false);
                                    }
                                    if(get_next_number(in, number, line_number))
                                    {
                                        if(number > 0.0 && number <= 1.0)
                                        {
                                            rss.scale(number);
                                            *this += rss;
                                            math_frames_rendered += rss.size();
                                            math_success = true;
                                        }
                                        else
                                        {
                                            *this += rss;
                                            math_frames_rendered += rss.size();
                                            report = "render_frames to value must be > 0.0 and <= 1.0";
                                        }
                                    }
                                    else
                                    {
                                        rss.scale(to_space);
                                        *this += rss;
                                        math_frames_rendered += rss.size();
                                        math_success = true;
                                    }
                                }
                            }
                            else
                                report = "no frames defined to render";
                        }
                        else
                            report = "missing frames name";
                    }
                    //--------------------------------------------------------
                    else if(word == "store")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            bool name_found = false;
                            for(size_t i = 0; i < math_list.size(); i++)
                            {
                                if(math_list[i]->name == word)
                                {
                                    name_found = true;
                                    break;
                                }
                            }
                            if(!name_found)
                            {
                                if(A != NULL)
                                {
                                    if(B != NULL)
                                    {
                                        C = new LaserBoy_real_segment_set(*B);
                                        B->been_used = true;
                                    }
                                    else
                                    {
                                        C = new LaserBoy_real_segment_set(*A);
                                        A->been_used = true;
                                    }
                                    math_list.push_back(C);
                                    math_list.back()->name = word;
                                    math_success = true;
                                }
                                else
                                    report = "no math frames defined before store_frames";
                            }
                            else
                                report = word + " is already defined";
                        }
                        else
                        {
                            if(math_list.size())
                            {
                                if(A != NULL)
                                {
                                    if(B != NULL)
                                    {
                                        C = new LaserBoy_real_segment_set(*B);
                                        B->been_used = true;
                                    }
                                    else
                                    {
                                        C = new LaserBoy_real_segment_set(*A);
                                        A->been_used = true;
                                    }
                                    math_list.push_back(C);
                                    math_success = true;
                                }
                                else
                                    report = "no math frames defined before store_frames";
                            }
                            else
                                report = "first frame_set stored in the list must be named";
                        }
                    }
                    //--------------------------------------------------------
                    else if(word == "recall")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            for(size_t i = 0; i < math_list.size(); i++)
                                if(math_list[i]->name == word)
                                {
                                    *B = *(math_list[i]);
                                    math_list[i]->been_used = true;
                                    if(lost_data)
                                        report = "unused frame_set deleted at " + word;
                                    else
                                        math_success = true;
                                    break;
                                }
                            if(!math_success)
                                report = "no math frames named " + word;
                        }
                        else
                            report = "missing frames name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_as_txt")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(word.size() < 5 || !iends_with(word, ".txt"))
                                word += ".txt";
                            if(    !file_exists(LASERBOY_TXT_SHARE + word)
                                || overwrite_txt_files
                              )
                            {
                                if(A != NULL)
                                {
                                    if(B != NULL)
                                        C = B;
                                    else
                                        C = A;
                                    if(!disable_txt)
                                    {
                                        C->save_as_txt(LASERBOY_TXT_SHARE + word);
                                        C->been_used = true;
                                        txt_frames_saved += C->size();
                                    }
                                    math_success = true;
                                }
                                else
                                    report = "no math frames defined before save_as_txt";
                            }
                            else
                                report = "txt file name already exists";
                        }
                        else
                            report = "missing txt file name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_list_from_name_as_txt")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            string real_segments_name = word;
                            if(get_next_word(in, word, line_number))
                            {
                                string txt_name = word;
                                if(math_list.size())
                                {
                                    bool name_found = false;
                                    LaserBoy_real_segment_set rss;
                                    for(size_t i = 0; i < math_list.size(); i++)
                                    {
                                        if(math_list[i]->name == real_segments_name)
                                        {
                                            if(!disable_txt)
                                            {
                                                for(size_t j = i; j < math_list.size(); j++)
                                                {
                                                    rss += *(math_list[j]);
                                                    math_list[j]->been_used = true;
                                                }
                                                rss.save_as_txt(LASERBOY_TXT_SHARE + txt_name);
                                                txt_frames_saved += rss.size();
                                            }
                                            math_success = true;
                                            name_found   = true;
                                            break;
                                        }
                                    } // end for(size_t i = 0; i < math_list.size(); i++)
                                    if(!name_found)
                                        report = "no math frames named " + real_segments_name;
                                }
                                else
                                    report = "no math frames defined before save_list_from_name_as_txt";
                            }
                            else
                                report = "missing bmp directory name";
                        }
                        else
                            report = "missing frame_set name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_as_dxf")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(A != NULL)
                            {
                                if(B != NULL)
                                    C = B;
                                else
                                    C = A;
                                if(!disable_dxf)
                                {
                                    C->save_as_dxf_directory(LASERBOY_DXF_SHARE, word, true_color_dxf);
                                    C->been_used = true;
                                    dxf_frames_saved += C->size();
                                }
                                math_success = true;
                            }
                            else
                                report = "no math frames defined before save_as_dxf";
                        }
                        else
                            report = "missing dxf directory name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_list_from_name_as_dxf")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            string real_segments_name = word;
                            if(get_next_word(in, word, line_number))
                            {
                                string dxf_dir_name = word;
                                if(math_list.size())
                                {
                                    bool name_found = false;
                                    LaserBoy_real_segment_set rss;
                                    for(size_t i = 0; i < math_list.size(); i++)
                                    {
                                        if(math_list[i]->name == real_segments_name)
                                        {
                                            if(!disable_dxf)
                                            {
                                                for(size_t j = i; j < math_list.size(); j++)
                                                {
                                                    rss += *(math_list[j]);
                                                    math_list[j]->been_used = true;
                                                }
                                                rss.save_as_dxf_directory(LASERBOY_DXF_SHARE, dxf_dir_name, true_color_dxf);
                                                dxf_frames_saved += rss.size();
                                            }
                                            math_success = true;
                                            name_found   = true;
                                            break;
                                        }
                                    } // end for(size_t i = 0; i < math_list.size(); i++)
                                    if(!name_found)
                                        report = "no math frames named " + real_segments_name;
                                }
                                else
                                    report = "no math frames defined before save_list_from_name_as_dxf";
                            }
                            else
                                report = "missing bmp directory name";
                        }
                        else
                            report = "missing frame_set name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_as_bmp")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            u_int x_res,
                                  y_res;
                            if(get_next_number(in, number, line_number))
                            {
                                x_res = (u_int)number;
                                if(get_next_number(in, number, line_number))
                                {
                                    y_res = (u_int)number;
                                    if(A != NULL)
                                    {
                                        if(B != NULL)
                                            C = B;
                                        else
                                            C = A;
                                        if(bmp_render_vectors || bmp_render_vertices)
                                        {
                                            if(!disable_bmp)
                                            {
                                                C->save_as_bmp_directory(LASERBOY_BMP_SHARE,
                                                                         word,
                                                                         x_res,
                                                                         y_res,
                                                                         norm_individually,
                                                                         rendered_line_width,
                                                                         bmp_render_vectors,
                                                                         bmp_render_vertices
                                                                        );
                                                C->been_used = true;
                                                bmp_frames_saved += C->size();
                                            }
                                            math_success = true;
                                        }
                                        else
                                            report = "either bmp_render_vectors or bmp_render_vertices must be true (1)";
                                    }
                                    else
                                        report = "no math frames defined before save_as_bmp";
                                }
                                else
                                    report = "missing Y resolution";
                            }
                            else
                                report = "missing X resolution";
                        }
                        else
                            report = "missing bmp directory name";
                    }
                    //--------------------------------------------------------
                    else if(word == "save_list_from_name_as_bmp")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            string real_segments_name = word;
                            if(get_next_word(in, word, line_number))
                            {
                                u_int  x_res,
                                       y_res;
                                string bmp_dir_name = word;
                                if(get_next_number(in, number, line_number))
                                {
                                    x_res = (u_int)number;
                                    if(get_next_number(in, number, line_number))
                                    {
                                        y_res = (u_int)number;
                                        if(!disable_bmp)
                                        {
                                            if(bmp_render_vectors || bmp_render_vertices)
                                            {
                                                if(math_list.size())
                                                {
                                                    bool name_found = false;
                                                    for(size_t i = 0; i < math_list.size(); i++)
                                                    {
                                                        if(math_list[i]->name == real_segments_name)
                                                        {
                                                            if(bmp_render_vectors || bmp_render_vertices)
                                                            {
                                                                LaserBoy_real_segment_set rss;
                                                                for(size_t j = i; j < math_list.size(); j++)
                                                                {
                                                                    rss += *(math_list[j]);
                                                                    math_list[j]->been_used = true;
                                                                }
                                                                rss.save_as_bmp_directory(LASERBOY_BMP_SHARE,
                                                                                          bmp_dir_name,
                                                                                          x_res,
                                                                                          y_res,
                                                                                          norm_individually,
                                                                                          rendered_line_width,
                                                                                          bmp_render_vectors,
                                                                                          bmp_render_vertices
                                                                                         );
                                                                bmp_frames_saved += rss.size();
                                                                math_success = true;
                                                                name_found   = true;
                                                                break;
                                                            }
                                                        }
                                                    } // end for(size_t i = 0; i < math_list.size(); i++)
                                                    if(!name_found)
                                                        report = "no math frames named " + real_segments_name;
                                                }
                                                else
                                                    report = "no math frames stored before save_list_from_name_as_bmp";
                                            }
                                            else
                                                report = "either bmp_render_vectors or bmp_render_vertices must be true";
                                        }
                                        else
                                           math_success = true;
                                    }
                                    else
                                        report = "missing Y resolution";
                                }
                                else
                                    report = "missing X resolution";
                            }
                            else
                                report = "missing bmp directory name";
                        }
                        else
                            report = "missing frame_set name";
                    }
                    //--------------------------------------------------------
                    //     math frames
                    //--------------------------------------------------------
                    else if(word == "line")
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::line(P0, P1);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_xy") // P0 to P1
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::rectangular_grid_xy(P0,
                                                                         P1,
                                                                         rectangular_grid_x_units,
                                                                         rectangular_grid_y_units
                                                                        );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_xz") // P0 to P1
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::rectangular_grid_xz(P0,
                                                                         P1,
                                                                         rectangular_grid_x_units,
                                                                         rectangular_grid_z_units
                                                                        );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_yz") // P0 to P1
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::rectangular_grid_yz(P0,
                                                                         P1,
                                                                         rectangular_grid_y_units,
                                                                         rectangular_grid_z_units
                                                                        );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "rectangular_grid_xyz") // P0 to P1
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += (   LaserBoy_real_segment::rectangular_grid_xy(P0,
                                                                             P1,
                                                                             rectangular_grid_x_units,
                                                                             rectangular_grid_y_units
                                                                            )
                                + LaserBoy_real_segment::rectangular_grid_xz(P0,
                                                                             P1,
                                                                             rectangular_grid_x_units,
                                                                             rectangular_grid_z_units
                                                                            )
                                + LaserBoy_real_segment::rectangular_grid_yz(P0,
                                                                             P1,
                                                                             rectangular_grid_y_units,
                                                                             rectangular_grid_z_units
                                                                            )
                              );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_xy") // P0
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::polar_grid_xy(P0,
                                                                   polar_grid_radius_max,
                                                                   polar_grid_radii_units,
                                                                   polar_grid_angle_units
                                                                  );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_xz") // P0
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::polar_grid_xz(P0,
                                                                   polar_grid_radius_max,
                                                                   polar_grid_radii_units,
                                                                   polar_grid_angle_units
                                                                  );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_yz") // P0
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += LaserBoy_real_segment::polar_grid_yz(P0,
                                                                   polar_grid_radius_max,
                                                                   polar_grid_radii_units,
                                                                   polar_grid_angle_units
                                                                  );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_grid_xyz") // P0
                    {
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = new LaserBoy_real_segment_set;
                            C = B;
                        }
                        else
                        {
                            A = new LaserBoy_real_segment_set;
                            C = A;
                        }
                        *C += (
                                  LaserBoy_real_segment::polar_grid_xy(P0,
                                                                       polar_grid_radius_max,
                                                                       polar_grid_radii_units,
                                                                       polar_grid_angle_units
                                                                      )
                                + LaserBoy_real_segment::polar_grid_xz(P0,
                                                                       polar_grid_radius_max,
                                                                       polar_grid_radii_units,
                                                                       polar_grid_angle_units
                                                                      )
                                + LaserBoy_real_segment::polar_grid_yz(P0,
                                                                       polar_grid_radius_max,
                                                                       polar_grid_radii_units,
                                                                       polar_grid_angle_units
                                                                      )
                              );
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "rhodonea")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::rhodonea(fixed_radius,
                                                              rhodonea_numerator,
                                                              rhodonea_denominator,
                                                              start,
                                                              duration,
                                                              iterations
                                                             );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_rhodonea")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_rhodonea(LBO1,
                                                                         LBO2,
                                                                         LBO3,
                                                                         LBO4,
                                                                         LBO1_,
                                                                         LBO2_,
                                                                         LBO3_,
                                                                         LBO4_,
                                                                         fixed_radius,
                                                                         rhodonea_numerator,
                                                                         rhodonea_denominator,
                                                                         start,
                                                                         duration,
                                                                         iterations
                                                                        );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_rhodonea")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_rhodonea(fixed_radius,
                                                                  rhodonea_numerator,
                                                                  rhodonea_denominator,
                                                                  start,
                                                                  duration,
                                                                  iterations,
                                                                 _fixed_radius,
                                                                 _rhodonea_numerator,
                                                                 _rhodonea_denominator,
                                                                 _start,
                                                                 _duration,
                                                                 _iterations,
                                                                  frames
                                                                 );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_rhodonea")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_rhodonea(LBO1,
                                                                             LBO2,
                                                                             LBO3,
                                                                             LBO4,
                                                                             LBO1_,
                                                                             LBO2_,
                                                                             LBO3_,
                                                                             LBO4_,
                                                                             fixed_radius,
                                                                             rhodonea_numerator,
                                                                             rhodonea_denominator,
                                                                             start,
                                                                             duration,
                                                                             iterations,
                                                                            _LBO1,
                                                                            _LBO2,
                                                                            _LBO3,
                                                                            _LBO4,
                                                                            _LBO1_,
                                                                            _LBO2_,
                                                                            _LBO3_,
                                                                            _LBO4_,
                                                                            _fixed_radius,
                                                                            _rhodonea_numerator,
                                                                            _rhodonea_denominator,
                                                                            _start,
                                                                            _duration,
                                                                            _iterations,
                                                                             frames
                                                                            );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "epicycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::epicycloid(fixed_radius,
                                                                roller_radius,
                                                                roller_radius,
                                                                start,
                                                                duration,
                                                                iterations
                                                               );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_epicycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_epicycloid(LBO1,
                                                                           LBO2,
                                                                           LBO3,
                                                                           LBO4,
                                                                           LBO1_,
                                                                           LBO2_,
                                                                           LBO3_,
                                                                           LBO4_,
                                                                           fixed_radius,
                                                                           roller_radius,
                                                                           roller_radius,
                                                                           start,
                                                                           duration,
                                                                           iterations
                                                                           );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_epicycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_epicycloid(fixed_radius,
                                                                    roller_radius,
                                                                    roller_radius,
                                                                    start,
                                                                    duration,
                                                                    iterations,
                                                                   _fixed_radius,
                                                                   _roller_radius,
                                                                   _roller_radius,
                                                                   _start,
                                                                   _duration,
                                                                   _iterations,
                                                                    frames
                                                                   );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_epicycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_epicycloid(LBO1,
                                                                               LBO2,
                                                                               LBO3,
                                                                               LBO4,
                                                                               LBO1_,
                                                                               LBO2_,
                                                                               LBO3_,
                                                                               LBO4_,
                                                                               fixed_radius,
                                                                               roller_radius,
                                                                               roller_radius,
                                                                               start,
                                                                               duration,
                                                                               iterations,
                                                                              _LBO1,
                                                                              _LBO2,
                                                                              _LBO3,
                                                                              _LBO4,
                                                                              _LBO1_,
                                                                              _LBO2_,
                                                                              _LBO3_,
                                                                              _LBO4_,
                                                                              _fixed_radius,
                                                                              _roller_radius,
                                                                              _roller_radius,
                                                                              _start,
                                                                              _duration,
                                                                              _iterations,
                                                                               frames
                                                                              );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "epitrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::epicycloid(fixed_radius,
                                                                roller_radius,
                                                                roller_offset,
                                                                start,
                                                                duration,
                                                                iterations
                                                               );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_epitrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_epicycloid(LBO1,
                                                                           LBO2,
                                                                           LBO3,
                                                                           LBO4,
                                                                           LBO1_,
                                                                           LBO2_,
                                                                           LBO3_,
                                                                           LBO4_,
                                                                           fixed_radius,
                                                                           roller_radius,
                                                                           roller_offset,
                                                                           start,
                                                                           duration,
                                                                           iterations
                                                                          );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_epitrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_epicycloid(fixed_radius,
                                                                    roller_radius,
                                                                    roller_offset,
                                                                    start,
                                                                    duration,
                                                                    iterations,
                                                                   _fixed_radius,
                                                                   _roller_radius,
                                                                   _roller_offset,
                                                                   _start,
                                                                   _duration,
                                                                   _iterations,
                                                                    frames
                                                                   );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_epitrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_epicycloid(LBO1,
                                                                               LBO2,
                                                                               LBO3,
                                                                               LBO4,
                                                                               LBO1_,
                                                                               LBO2_,
                                                                               LBO3_,
                                                                               LBO4_,
                                                                               fixed_radius,
                                                                               roller_radius,
                                                                               roller_offset,
                                                                               start,
                                                                               duration,
                                                                               iterations,
                                                                              _LBO1,
                                                                              _LBO2,
                                                                              _LBO3,
                                                                              _LBO4,
                                                                              _LBO1_,
                                                                              _LBO2_,
                                                                              _LBO3_,
                                                                              _LBO4_,
                                                                              _fixed_radius,
                                                                              _roller_radius,
                                                                              _roller_offset,
                                                                              _start,
                                                                              _duration,
                                                                              _iterations,
                                                                               frames
                                                                              );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "hypocycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::hypocycloid(fixed_radius,
                                                                 roller_radius,
                                                                 roller_radius,
                                                                 start,
                                                                 duration,
                                                                 iterations
                                                                );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_hypocycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_hypocycloid(LBO1,
                                                                            LBO2,
                                                                            LBO3,
                                                                            LBO4,
                                                                            LBO1_,
                                                                            LBO2_,
                                                                            LBO3_,
                                                                            LBO4_,
                                                                            fixed_radius,
                                                                            roller_radius,
                                                                            roller_radius,
                                                                            start,
                                                                            duration,
                                                                            iterations
                                                                           );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_hypocycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_hypocycloid(fixed_radius,
                                                                     roller_radius,
                                                                     roller_radius,
                                                                     start,
                                                                     duration,
                                                                     iterations,
                                                                    _fixed_radius,
                                                                    _roller_radius,
                                                                    _roller_radius,
                                                                    _start,
                                                                    _duration,
                                                                    _iterations,
                                                                     frames
                                                                    );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_hypocycloid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_hypocycloid(LBO1,
                                                                                LBO2,
                                                                                LBO3,
                                                                                LBO4,
                                                                                LBO1_,
                                                                                LBO2_,
                                                                                LBO3_,
                                                                                LBO4_,
                                                                                fixed_radius,
                                                                                roller_radius,
                                                                                roller_radius,
                                                                                start,
                                                                                duration,
                                                                                iterations,
                                                                               _LBO1,
                                                                               _LBO2,
                                                                               _LBO3,
                                                                               _LBO4,
                                                                               _LBO1_,
                                                                               _LBO2_,
                                                                               _LBO3_,
                                                                               _LBO4_,
                                                                               _fixed_radius,
                                                                               _roller_radius,
                                                                               _roller_radius,
                                                                               _start,
                                                                               _duration,
                                                                               _iterations,
                                                                                frames
                                                                               );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "hypotrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::hypocycloid(fixed_radius,
                                                                 roller_radius,
                                                                 roller_offset,
                                                                 start,
                                                                 duration,
                                                                 iterations
                                                                );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_hypotrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_hypocycloid(LBO1,
                                                                            LBO2,
                                                                            LBO3,
                                                                            LBO4,
                                                                            LBO1_,
                                                                            LBO2_,
                                                                            LBO3_,
                                                                            LBO4_,
                                                                            fixed_radius,
                                                                            roller_radius,
                                                                            roller_offset,
                                                                            start,
                                                                            duration,
                                                                            iterations
                                                                           );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_hypotrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_hypocycloid(fixed_radius,
                                                                     roller_radius,
                                                                     roller_offset,
                                                                     start,
                                                                     duration,
                                                                     iterations,
                                                                    _fixed_radius,
                                                                    _roller_radius,
                                                                    _roller_offset,
                                                                    _start,
                                                                    _duration,
                                                                    _iterations,
                                                                     frames
                                                                    );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_hypotrochoid")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_hypocycloid(LBO1,
                                                                                LBO2,
                                                                                LBO3,
                                                                                LBO4,
                                                                                LBO1_,
                                                                                LBO2_,
                                                                                LBO3_,
                                                                                LBO4_,
                                                                                fixed_radius,
                                                                                roller_radius,
                                                                                roller_offset,
                                                                                start,
                                                                                duration,
                                                                                iterations,
                                                                               _LBO1,
                                                                               _LBO2,
                                                                               _LBO3,
                                                                               _LBO4,
                                                                               _LBO1_,
                                                                               _LBO2_,
                                                                               _LBO3_,
                                                                               _LBO4_,
                                                                               _fixed_radius,
                                                                               _roller_radius,
                                                                               _roller_offset,
                                                                               _start,
                                                                               _duration,
                                                                               _iterations,
                                                                                frames
                                                                               );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator(LBO1,
                                                                LBO1_,
                                                                start,
                                                                duration,
                                                                iterations
                                                               );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator(LBO1,
                                                                    LBO1_,
                                                                    start,
                                                                    duration,
                                                                    iterations,
                                                                   _LBO1,
                                                                   _LBO1_,
                                                                   _start,
                                                                   _duration,
                                                                   _iterations,
                                                                    frames
                                                                   );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "oscillator_sum")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_sum(LBO1,
                                                                    LBO2,
                                                                    LBO1_,
                                                                    LBO2_,
                                                                    start,
                                                                    duration,
                                                                    iterations
                                                                   );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_oscillator_sum")
                   {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_sum(LBO1,
                                                                        LBO2,
                                                                        LBO1_,
                                                                        LBO2_,
                                                                        start,
                                                                        duration,
                                                                        iterations,
                                                                       _LBO1,
                                                                       _LBO2,
                                                                       _LBO1_,
                                                                       _LBO2_,
                                                                       _start,
                                                                       _duration,
                                                                       _iterations,
                                                                        frames
                                                                       );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(    word == "lissajou"
                             || word == "oscillator_xy"
                           )
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_xy(LBO1,
                                                                   LBO2,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   start,
                                                                   duration,
                                                                   iterations
                                                                  );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(    word == "_lissajou"
                             || word == "_oscillator_xy"
                           )
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_xy(LBO1,
                                                                       LBO2,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       start,
                                                                       duration,
                                                                       iterations,
                                                                      _LBO1,
                                                                      _LBO2,
                                                                      _LBO1_,
                                                                      _LBO2_,
                                                                      _start,
                                                                      _duration,
                                                                      _iterations,
                                                                       frames
                                                                      );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(    word == "lissajou_xyz"
                             || word == "oscillator_xyz"
                           )
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::oscillator_xyz(LBO1,
                                                                    LBO2,
                                                                    LBO3,
                                                                    LBO1_,
                                                                    LBO2_,
                                                                    LBO3_,
                                                                    start,
                                                                    duration,
                                                                    iterations
                                                                   );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(    word == "_lissajou_xyz"
                             || word == "_oscillator_xyz"
                           )
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_oscillator_xyz(LBO1,
                                                                        LBO2,
                                                                        LBO3,
                                                                        LBO1_,
                                                                        LBO2_,
                                                                        LBO3_,
                                                                        start,
                                                                        duration,
                                                                        iterations,
                                                                       _LBO1,
                                                                       _LBO2,
                                                                       _LBO3,
                                                                       _LBO1_,
                                                                       _LBO2_,
                                                                       _LBO3_,
                                                                       _start,
                                                                       _duration,
                                                                       _iterations,
                                                                        frames
                                                                       );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "harmonograph")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::harmonograph(LBO1,
                                                                  LBO2,
                                                                  LBO3,
                                                                  LBO4,
                                                                  LBO1_,
                                                                  LBO2_,
                                                                  LBO3_,
                                                                  LBO4_,
                                                                  start,
                                                                  duration,
                                                                  iterations
                                                                 );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    } // end if(word == "harmonograph")
                    //--------------------------------------------------------
                    else if(word == "_harmonograph")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_harmonograph(LBO1,
                                                                      LBO2,
                                                                      LBO3,
                                                                      LBO4,
                                                                      LBO1_,
                                                                      LBO2_,
                                                                      LBO3_,
                                                                      LBO4_,
                                                                      start,
                                                                      duration,
                                                                      iterations,
                                                                     _LBO1,
                                                                     _LBO2,
                                                                     _LBO3,
                                                                     _LBO4,
                                                                     _LBO1_,
                                                                     _LBO2_,
                                                                     _LBO3_,
                                                                     _LBO4_,
                                                                     _start,
                                                                     _duration,
                                                                     _iterations,
                                                                      frames
                                                                     );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "harmonograph_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::harmonograph_xyz(LBO1,
                                                                      LBO2,
                                                                      LBO3,
                                                                      LBO4,
                                                                      LBO5,
                                                                      LBO6,
                                                                      LBO1_,
                                                                      LBO2_,
                                                                      LBO3_,
                                                                      LBO4_,
                                                                      LBO5_,
                                                                      LBO6_,
                                                                      start,
                                                                      duration,
                                                                      iterations
                                                                     );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    } // end if(word == "harmonograph_xyz")
                    //--------------------------------------------------------
                    else if(word == "_harmonograph_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_harmonograph_xyz(LBO1,
                                                                          LBO2,
                                                                          LBO3,
                                                                          LBO4,
                                                                          LBO5,
                                                                          LBO6,
                                                                          LBO1_,
                                                                          LBO2_,
                                                                          LBO3_,
                                                                          LBO4_,
                                                                          LBO5_,
                                                                          LBO6_,
                                                                          start,
                                                                          duration,
                                                                          iterations,
                                                                         _LBO1,
                                                                         _LBO2,
                                                                         _LBO3,
                                                                         _LBO4,
                                                                         _LBO5,
                                                                         _LBO6,
                                                                         _LBO1_,
                                                                         _LBO2_,
                                                                         _LBO3_,
                                                                         _LBO4_,
                                                                         _LBO5_,
                                                                         _LBO6_,
                                                                         _start,
                                                                         _duration,
                                                                         _iterations,
                                                                          frames
                                                                         );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "amplitude_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::amplitude_mod(LBO1,
                                                                   LBO2,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   start,
                                                                   duration,
                                                                   iterations
                                                                  );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_amplitude_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_amplitude_mod(LBO1,
                                                                       LBO2,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       start,
                                                                       duration,
                                                                       iterations,
                                                                      _LBO1,
                                                                      _LBO2,
                                                                      _LBO1_,
                                                                      _LBO2_,
                                                                      _start,
                                                                      _duration,
                                                                      _iterations,
                                                                       frames
                                                                      );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "amplitude_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::amplitude_mod_xy(LBO1,
                                                                      LBO2,
                                                                      LBO3,
                                                                      LBO4,
                                                                      LBO1_,
                                                                      LBO2_,
                                                                      LBO3_,
                                                                      LBO4_,
                                                                      start,
                                                                      duration,
                                                                      iterations
                                                                     );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_amplitude_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_amplitude_mod_xy(LBO1,
                                                                          LBO2,
                                                                          LBO3,
                                                                          LBO4,
                                                                          LBO1_,
                                                                          LBO2_,
                                                                          LBO3_,
                                                                          LBO4_,
                                                                          start,
                                                                          duration,
                                                                          iterations,
                                                                         _LBO1,
                                                                         _LBO2,
                                                                         _LBO3,
                                                                         _LBO4,
                                                                         _LBO1_,
                                                                         _LBO2_,
                                                                         _LBO3_,
                                                                         _LBO4_,
                                                                         _start,
                                                                         _duration,
                                                                         _iterations,
                                                                          frames
                                                                         );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "amplitude_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::amplitude_mod_xyz(LBO1,
                                                                       LBO2,
                                                                       LBO3,
                                                                       LBO4,
                                                                       LBO5,
                                                                       LBO6,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       LBO3_,
                                                                       LBO4_,
                                                                       LBO5_,
                                                                       LBO6_,
                                                                       start,
                                                                       duration,
                                                                       iterations
                                                                      );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_amplitude_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_amplitude_mod_xyz(LBO1,
                                                                           LBO2,
                                                                           LBO3,
                                                                           LBO4,
                                                                           LBO5,
                                                                           LBO6,
                                                                           LBO1_,
                                                                           LBO2_,
                                                                           LBO3_,
                                                                           LBO4_,
                                                                           LBO5_,
                                                                           LBO6_,
                                                                           start,
                                                                           duration,
                                                                           iterations,
                                                                          _LBO1,
                                                                          _LBO2,
                                                                          _LBO3,
                                                                          _LBO4,
                                                                          _LBO5,
                                                                          _LBO6,
                                                                          _LBO1_,
                                                                          _LBO2_,
                                                                          _LBO3_,
                                                                          _LBO4_,
                                                                          _LBO5_,
                                                                          _LBO6_,
                                                                          _start,
                                                                          _duration,
                                                                          _iterations,
                                                                           frames
                                                                          );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "frequency_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::frequency_mod(LBO1,
                                                                   LBO2,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   start,
                                                                   duration,
                                                                   iterations
                                                                  );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_frequency_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_frequency_mod(LBO1,
                                                                       LBO2,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       start,
                                                                       duration,
                                                                       iterations,
                                                                      _LBO1,
                                                                      _LBO2,
                                                                      _LBO1_,
                                                                      _LBO2_,
                                                                      _start,
                                                                      _duration,
                                                                      _iterations,
                                                                       frames
                                                                      );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "frequency_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::frequency_mod_xy(LBO1,
                                                                      LBO2,
                                                                      LBO3,
                                                                      LBO4,
                                                                      LBO1_,
                                                                      LBO2_,
                                                                      LBO3_,
                                                                      LBO4_,
                                                                      start,
                                                                      duration,
                                                                      iterations
                                                                     );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_frequency_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_frequency_mod_xy(LBO1,
                                                                          LBO2,
                                                                          LBO3,
                                                                          LBO4,
                                                                          LBO1_,
                                                                          LBO2_,
                                                                          LBO3_,
                                                                          LBO4_,
                                                                          start,
                                                                          duration,
                                                                          iterations,
                                                                         _LBO1,
                                                                         _LBO2,
                                                                         _LBO3,
                                                                         _LBO4,
                                                                         _LBO1_,
                                                                         _LBO2_,
                                                                         _LBO3_,
                                                                         _LBO4_,
                                                                         _start,
                                                                         _duration,
                                                                         _iterations,
                                                                          frames
                                                                         );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "frequency_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::frequency_mod_xyz(LBO1,
                                                                       LBO2,
                                                                       LBO3,
                                                                       LBO4,
                                                                       LBO5,
                                                                       LBO6,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       LBO3_,
                                                                       LBO4_,
                                                                       LBO5_,
                                                                       LBO6_,
                                                                       start,
                                                                       duration,
                                                                       iterations
                                                                      );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_frequency_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_frequency_mod_xyz(LBO1,
                                                                           LBO2,
                                                                           LBO3,
                                                                           LBO4,
                                                                           LBO5,
                                                                           LBO6,
                                                                           LBO1_,
                                                                           LBO2_,
                                                                           LBO3_,
                                                                           LBO4_,
                                                                           LBO5_,
                                                                           LBO6_,
                                                                           start,
                                                                           duration,
                                                                           iterations,
                                                                          _LBO1,
                                                                          _LBO2,
                                                                          _LBO3,
                                                                          _LBO4,
                                                                          _LBO5,
                                                                          _LBO6,
                                                                          _LBO1_,
                                                                          _LBO2_,
                                                                          _LBO3_,
                                                                          _LBO4_,
                                                                          _LBO5_,
                                                                          _LBO6_,
                                                                          _start,
                                                                          _duration,
                                                                          _iterations,
                                                                           frames
                                                                          );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "phase_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::phase_mod(LBO1,
                                                               LBO2,
                                                               LBO1_,
                                                               LBO2_,
                                                               start,
                                                               duration,
                                                               iterations
                                                              );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_phase_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_phase_mod(LBO1,
                                                                   LBO2,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   start,
                                                                   duration,
                                                                   iterations,
                                                                  _LBO1,
                                                                  _LBO2,
                                                                  _LBO1_,
                                                                  _LBO2_,
                                                                  _start,
                                                                  _duration,
                                                                  _iterations,
                                                                   frames
                                                                  );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "phase_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::phase_mod_xy(LBO1,
                                                                  LBO2,
                                                                  LBO3,
                                                                  LBO4,
                                                                  LBO1_,
                                                                  LBO2_,
                                                                  LBO3_,
                                                                  LBO4_,
                                                                  start,
                                                                  duration,
                                                                  iterations
                                                                 );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_phase_mod_xy")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_phase_mod_xy(LBO1,
                                                                      LBO2,
                                                                      LBO3,
                                                                      LBO4,
                                                                      LBO1_,
                                                                      LBO2_,
                                                                      LBO3_,
                                                                      LBO4_,
                                                                      start,
                                                                      duration,
                                                                      iterations,
                                                                     _LBO1,
                                                                     _LBO2,
                                                                     _LBO3,
                                                                     _LBO4,
                                                                     _LBO1_,
                                                                     _LBO2_,
                                                                     _LBO3_,
                                                                     _LBO4_,
                                                                     _start,
                                                                     _duration,
                                                                     _iterations,
                                                                      frames
                                                                     );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "phase_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::phase_mod_xyz(LBO1,
                                                                   LBO2,
                                                                   LBO3,
                                                                   LBO4,
                                                                   LBO5,
                                                                   LBO6,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   LBO3_,
                                                                   LBO4_,
                                                                   LBO5_,
                                                                   LBO6_,
                                                                   start,
                                                                   duration,
                                                                   iterations
                                                                  );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_phase_mod_xyz")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_phase_mod_xyz(LBO1,
                                                                       LBO2,
                                                                       LBO3,
                                                                       LBO4,
                                                                       LBO5,
                                                                       LBO6,
                                                                       LBO1_,
                                                                       LBO2_,
                                                                       LBO3_,
                                                                       LBO4_,
                                                                       LBO5_,
                                                                       LBO6_,
                                                                       start,
                                                                       duration,
                                                                       iterations,
                                                                      _LBO1,
                                                                      _LBO2,
                                                                      _LBO3,
                                                                      _LBO4,
                                                                      _LBO5,
                                                                      _LBO6,
                                                                      _LBO1_,
                                                                      _LBO2_,
                                                                      _LBO3_,
                                                                      _LBO4_,
                                                                      _LBO5_,
                                                                      _LBO6_,
                                                                      _start,
                                                                      _duration,
                                                                      _iterations,
                                                                       frames
                                                                      );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::polar(LBO1,
                                                           LBO1_,
                                                           start,
                                                           duration,
                                                           iterations
                                                          );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_polar")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_polar(LBO1,
                                                               LBO1_,
                                                               start,
                                                               duration,
                                                               iterations,
                                                              _LBO1,
                                                              _LBO1_,
                                                              _start,
                                                              _duration,
                                                              _iterations,
                                                               frames
                                                              );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_sum")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::polar_sum(LBO1,
                                                               LBO2,
                                                               LBO1_,
                                                               LBO2_,
                                                               start,
                                                               duration,
                                                               iterations
                                                              );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_polar_sum")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_polar_sum(LBO1,
                                                                   LBO2,
                                                                   LBO1_,
                                                                   LBO2_,
                                                                   start,
                                                                   duration,
                                                                   iterations,
                                                                  _LBO1,
                                                                  _LBO2,
                                                                  _LBO1_,
                                                                  _LBO2_,
                                                                  _start,
                                                                  _duration,
                                                                  _iterations,
                                                                   frames
                                                                  );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_amplitude_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::polar_amplitude_mod(LBO1,
                                                                         LBO2,
                                                                         LBO1_,
                                                                         LBO2_,
                                                                         start,
                                                                         duration,
                                                                         iterations
                                                                        );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_polar_amplitude_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_polar_amplitude_mod(LBO1,
                                                                             LBO2,
                                                                             LBO1_,
                                                                             LBO2_,
                                                                             start,
                                                                             duration,
                                                                             iterations,
                                                                            _LBO1,
                                                                            _LBO2,
                                                                            _LBO1_,
                                                                            _LBO2_,
                                                                            _start,
                                                                            _duration,
                                                                            _iterations,
                                                                             frames
                                                                            );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_frequency_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::polar_frequency_mod(LBO1,
                                                                         LBO2,
                                                                         LBO1_,
                                                                         LBO2_,
                                                                         start,
                                                                         duration,
                                                                         iterations
                                                                        );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_polar_frequency_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_polar_frequency_mod(LBO1,
                                                                             LBO2,
                                                                             LBO1_,
                                                                             LBO2_,
                                                                             start,
                                                                             duration,
                                                                             iterations,
                                                                            _LBO1,
                                                                            _LBO2,
                                                                            _LBO1_,
                                                                            _LBO2_,
                                                                            _start,
                                                                            _duration,
                                                                            _iterations,
                                                                             frames
                                                                            );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "polar_phase_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        *C += LaserBoy_real_segment::polar_phase_mod(LBO1,
                                                                     LBO2,
                                                                     LBO1_,
                                                                     LBO2_,
                                                                     start,
                                                                     duration,
                                                                     iterations
                                                                    );
                        if(still_frames > 1)
                            C->repeat_back(still_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "_polar_phase_mod")
                    {
                        C = new LaserBoy_real_segment_set;
                        *C = LaserBoy_real_segment_set::_polar_phase_mod(LBO1,
                                                                         LBO2,
                                                                         LBO1_,
                                                                         LBO2_,
                                                                         start,
                                                                         duration,
                                                                         iterations,
                                                                        _LBO1,
                                                                        _LBO2,
                                                                        _LBO1_,
                                                                        _LBO2_,
                                                                        _start,
                                                                        _duration,
                                                                        _iterations,
                                                                         frames
                                                                        );
                        if(A != NULL)
                        {
                            if(B != NULL)
                            {
                                if(!A->been_used)
                                    lost_data = true;
                                delete A;
                                A = B;
                            }
                            B = C;
                        }
                        else
                            A = C;
                        if(first_frames > 1)
                            C->repeat_front(first_frames);
                        if(last_frames > 1)
                            C->repeat_back(last_frames);
                        if(lost_data)
                            report = "unused frame_set deleted at " + word;
                        else
                            math_success = true;
                    }
                    //--------------------------------------------------------
                    else
                    {
                        // math [nonsense]
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : math "
                                  << word
                                  << " is not a recognized form. (ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                    //--------------------------------------------------------
                    //--------------------------------------------------------
                    //--------------------------------------------------------
                    if(divide_by_zero)
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " divide by zero. (ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                    //--------------------------------------------------------
                    else if(!math_success)
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " syntax. (ERROR) "
                                  << report
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number)) (after finding math)
                //------------------------------------------------------------
                else
                {
                    // math [nothing]
                    error_log << file_name
                              << " error on line: "
                              << line_number
                              << " \'math\' not followd by any form. (ERROR)"
                              << ENDL;
                    frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                }
                //------------------------------------------------------------
            } // end else if(word == "math")
            //----------------------------------------------------------------

            //----------------------------------------------------------------
            // utf8
            //----------------------------------------------------------------
            else if(word == "utf8")
            {
                bool    utf8_success = false;
                string  some_text,
                        report;
                if(get_next_word(in, word, line_number)) // utf8 (word)
                {
                    if(word == "codes")
                    {
                        if(get_next_word(in, word, line_number)) // file name
                        {
                            bool       fail           = false;
                            char32_t   uni_char       = 0,
                                       uni_char_from  = 0,
                                       uni_char_to    = 0;
                            string     utf8_name      = word;
                            u32string  unicode;

                            word.clear();
                            report.clear();
                            //------------------------------------------------
                            get_next_token(in, word, line_number);
                            while(in.good() && word.substr(0, 16) != "<<__TEXT_END__>>")
                            {
                                if(hex_string(word, number))
                                {
                                    uni_char = (char32_t)number;
                                    unicode.push_back(uni_char);
                                }
                                else if(word == "range")
                                {
                                    get_next_token(in, word, line_number);
                                    if(hex_string(word, number))
                                    {
                                        uni_char_from = (char32_t)number;
                                        get_next_token(in, word, line_number);
                                        if(hex_string(word, number))
                                        {
                                            uni_char_to = (char32_t)number;
                                            if(uni_char_from > uni_char_to)
                                            {
                                                report += "text codes range is out of order.\n";
                                                fail = true;
                                            }
                                            else
                                                for(uni_char = uni_char_from; uni_char <= uni_char_to; uni_char++)
                                                    unicode.push_back(uni_char);
                                        } // end if(hex_string(word, number))
                                        else
                                        {
                                            report += " : " + word + " is not valid after text codes range.\n";
                                            fail = true;
                                        }
                                    } // end if(hex_string(word, number))
                                    else
                                    {
                                        report += " : " + word + " is not valid after text codes range.\n";
                                        fail = true;
                                    }
                                } // end if(word == "range")
                                else if(word.substr(0, 16) != "<<__TEXT_END__>>")
                                {
                                    report += " : " + word + " is not valid after text codes range.\n";
                                    fail = true;
                                }
                                if(fail)
                                {
                                    string text_end_token = "<<__TEXT_END__>>";
                                    clear_through_string(in, text_end_token, line_number);
                                    break;
                                }
                                else
                                    get_next_token(in, word, line_number);
                            } // end while(in.good() && word.substr(0, 16) != "<<__TEXT_END__>>")
                            //------------------------------------------------
                            if(    unicode.size()
                                && report.size() == 0
                              )
                            {
                                size_t         i = 0;
                                u32string      characters;
                                sort(unicode.begin(), unicode.end());
                                for(i = 0; i < unicode.size(); i++)
                                {
                                    if(unicode.at(i) > 0x00000020)
                                    {
                                        characters.push_back(unicode.at(i)); // first visible character
                                        break;
                                    }
                                }
                                if(characters.size())
                                {
                                    bool           has_diacritics = false;
                                    char           buffer[5];
                                    std::ofstream  out((LASERBOY_UTF8_SHARE + utf8_name + ".utf8").c_str(), ios::out);
                                    buffer[4] = '\0';
                                    while(i < unicode.size())
                                    {
                                        if(characters.back() != unicode.at(i))
                                            characters.push_back(unicode.at(i));
                                        i++;
                                    }
                                    for(i = 0; i < characters.size(); i++)
                                    {
                                        char_utf32_to_utf8(characters[i], buffer);
                                        if(is_diacritic(characters[i]))
                                        {
                                            has_diacritics = true;
                                            out << ' ';
                                        }
                                        out << buffer;
                                        if(is_diacritic(characters[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < characters.size(); i++)
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    for(i = 0; i < characters.size(); i++)
                                    {
                                        char_utf32_to_utf8(characters[i], buffer);
                                        out << buffer;
                                        out << "  ";
                                    }
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    if(has_diacritics)
                                    {
                                        out << endl;
                                        out << utf8_name
                                            << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        out << endl << endl;
                                    }
                                    out << "........................" << endl;
                                    out << "     frame  glyph  hex" << endl;
                                    for(i = 0; i < characters.size(); i++)
                                    {
                                        char_utf32_to_utf8(characters[i], buffer);
                                        out << "  "
                                            << dec
                                            << setw(8)
                                            << setfill(' ')
                                            << right
                                            << i
                                            << "    ";
                                        out << buffer
                                            << "    0x"
                                            << hex
                                            << setw(8)
                                            << setfill('0')
                                            << right
                                            << characters[i];
                                        if(is_diacritic(characters[i]))
                                            out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                        out << endl;
                                    } // end for(i = 0; i < characters.size(); i++)
                                    out << endl;
                                    out << "........................" << endl;
                                    out.close();
                                    utf8_success = true;
                                } // end if(characters.size())
                                else
                                    report += " : text codes no visible characters found.";
                            } // end else if(report.size() == 0 && unicode.size())
                            else
                                report += " : text codes no unicode values found.";
                        } // if(get_next_word(in, word, line_number)) // file name
                        else
                            report = " : text codes missing file name.";
                    } // end if(word == "codes")
                    //--------------------------------------------------------
                    else if(word == "chars")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            string     all_text,
                                       utf8_name = word;
                            word.clear();
                            report.clear();
                            //------------------------------------------------
                            get_next_line(in, some_text, line_number);
                            all_text += some_text;
                            some_text.clear();
                            while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                            {
                                if(some_text.size() > 0)
                                    all_text += some_text;
                                some_text.clear();
                                get_next_line(in, some_text, line_number);
                            }
                            if(all_text.size())
                            {
                                u32string unicode = conv_utf8_utf32.from_bytes(all_text);
                                if(unicode.size())
                                {
                                    size_t     i = 0;
                                    u32string  characters;
                                    sort(unicode.begin(), unicode.end());
                                    for(i = 0; i < unicode.size(); i++)
                                    {
                                        if(unicode.at(i) > 0x00000020)
                                        {
                                            characters.push_back(unicode.at(i));
                                            break;
                                        }
                                    }
                                    if(characters.size())
                                    {
                                        bool           has_diacritics = false;
                                        char           buffer[5];
                                        std::ofstream  out((LASERBOY_UTF8_SHARE + utf8_name + ".utf8").c_str(), ios::out);
                                        buffer[4] = '\0';
                                        while(i < unicode.size())
                                        {
                                            if(characters.back() != unicode.at(i))
                                                characters.push_back(unicode.at(i));
                                            i++;
                                        }
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            char_utf32_to_utf8(characters[i], buffer);
                                            if(is_diacritic(characters[i]))
                                            {
                                                has_diacritics = true;
                                                out << ' ';
                                            }
                                            out << buffer;
                                            if(is_diacritic(characters[i]))
                                                out << ' ';
                                        } // end for(i = 0; i < characters.size(); i++)
                                        out << endl << endl;
                                        out << "........................" << endl;
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            char_utf32_to_utf8(characters[i], buffer);
                                            out << buffer;
                                            out << "  ";
                                        }
                                        out << endl << endl;
                                        out << "........................" << endl;

                                        if(has_diacritics)
                                        {
                                            out << endl;
                                            out << utf8_name
                                                << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        }
                                        out << endl;
                                        out << "........................" << endl;
                                        out << "     frame  glyph  hex" << endl;
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            char_utf32_to_utf8(characters[i], buffer);
                                            out << "  "
                                                << dec
                                                << setw(8)
                                                << setfill(' ')
                                                << right
                                                << i
                                                << "    ";
                                            out << buffer
                                                << "    0x"
                                                << hex
                                                << setw(8)
                                                << setfill('0')
                                                << right
                                                << characters[i];
                                            if(is_diacritic(characters[i]))
                                                out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                            out << endl;
                                        } // end for(i = 0; i < characters.size(); i++)
                                        out << "........................" << endl;
                                        out << all_text << endl;
                                        out << "........................" << endl;
                                        out.close();
                                        utf8_success = true;
                                    } // end if(characters.size())
                                    else
                                        report = " : no text found in text chars.";
                                } // end if(unicode.size())
                                else
                                    report = " : no text found in text chars.";
                            } // end if(all_text.size())
                            else
                                report = " : no text found in text chars.";
                        } // if(get_next_word(in, word, line_number)) // file name
                        else
                            report = " : text chars missing file name.";
                    } // end else if(word == "chars")
                    //--------------------------------------------------------
                    else if(word == "chars_to_codes")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            string     all_text,
                                       utf8_name = word;
                            word.clear();
                            report.clear();
                            //------------------------------------------------
                            get_next_line(in, some_text, line_number);
                            all_text += some_text;
                            some_text.clear();
                            while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                            {
                                if(some_text.size() > 0)
                                    all_text += some_text;
                                some_text.clear();
                                get_next_line(in, some_text, line_number);
                            }
                            if(all_text.size())
                            {
                                u32string unicode = conv_utf8_utf32.from_bytes(all_text);
                                if(unicode.size())
                                {
                                    size_t     i = 0;
                                    u32string  characters;
                                    for(i = 0; i < unicode.size(); i++)
                                    {
                                        if(unicode.at(i) > 0x00000020)
                                        {
                                            characters.push_back(unicode.at(i));
                                            break;
                                        }
                                    }
                                    if(characters.size())
                                    {
                                        bool           has_diacritics = false;
                                        char           buffer[5];
                                        std::ofstream  out((LASERBOY_UTF8_SHARE + utf8_name + ".utf8").c_str(), ios::out);
                                        buffer[4] = '\0';
                                        while(i < unicode.size())
                                        {
                                            if(characters.back() != unicode.at(i))
                                                characters.push_back(unicode.at(i));
                                            i++;
                                        }
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            char_utf32_to_utf8(characters[i], buffer);
                                            if(is_diacritic(characters[i]))
                                            {
                                                has_diacritics = true;
                                                out << ' ';
                                            }
                                            out << buffer;
                                            if(is_diacritic(characters[i]))
                                                out << ' ';
                                        } // end for(i = 0; i < characters.size(); i++)
                                        out << endl << endl;
                                        out << "........................" << endl;
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            out << "    0x"
                                                << hex
                                                << setw(8)
                                                << setfill('0')
                                                << right
                                                << characters[i]
                                                << endl;
                                        } // end for(i = 0; i < characters.size(); i++)
                                        if(has_diacritics)
                                        {
                                            out << "........................" << endl;
                                            out << endl;
                                            out << utf8_name
                                                << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        }
                                        out << "........................" << endl;
                                        out << "     frame  glyph  hex" << endl;
                                        for(i = 0; i < characters.size(); i++)
                                        {
                                            char_utf32_to_utf8(characters[i], buffer);
                                            out << "  "
                                                << dec
                                                << setw(8)
                                                << setfill(' ')
                                                << right
                                                << i
                                                << "    ";
                                            if(is_diacritic(characters[i]))
                                                out << ' ';
                                            out << buffer
                                                << "    0x"
                                                << hex
                                                << setw(8)
                                                << setfill('0')
                                                << right
                                                << characters[i];
                                            if(is_diacritic(characters[i]))
                                                out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                            out << endl;
                                        } // end for(i = 0; i < characters.size(); i++)
                                        out << "........................" << endl;
                                        out << endl;
                                        out.close();
                                        utf8_success = true;
                                    } // end if(characters.size())
                                    else
                                        report = " : no text found in text chars.";
                                } // end if(unicode.size())
                                else
                                    report = " : no text found in text chars.";
                            } // end if(all_text.size())
                            else
                                report = " : no text found in text chars.";
                        } // if(get_next_word(in, word, line_number)) // file name
                        else
                            report = " : text chars missing file name.";
                    } // end else if(word == "chars_to_codes")
                    //--------------------------------------------------------
                    else if(word == "compare")
                    {
                        do
                        {
                            if(get_next_word(in, word, line_number)) // file_name A
                            {
                                string     utf8str,
                                           A_name = word;
                                u32string  A;
                                if(!get_utf8_index(A, LASERBOY_FONT_SHARE + word + ".utf8"))
                                    if(!get_utf8_index(A, LASERBOY_UTF8_SHARE + word + ".utf8"))
                                    {
                                        report = " : " + word + ".utf8 not found";
                                        break;
                                    }
                                if(get_next_word(in, word, line_number)) // file_name B
                                {
                                    bool           has_diacritics = false;
                                    size_t         i, j;
                                    string         utf8_name,
                                                   B_name = word;
                                    u32string      B, A_B, B_A, AB;
                                    char           buffer[5];
                                    buffer[4] = '\0';
                                    if(!get_utf8_index(B, LASERBOY_FONT_SHARE + word + ".utf8"))
                                        if(!get_utf8_index(B, LASERBOY_UTF8_SHARE + word + ".utf8"))
                                        {
                                            report = " : " + word + ".utf8 not found";
                                            break;
                                        }
                                    utf8_name = A_name + '_';
                                    utf8_name += B_name;
                                    std::ofstream  out((LASERBOY_UTF8_SHARE + utf8_name + ".utf8").c_str(), ios::out);
                                    A_B = A + B;
                                    sort(  A.begin(),   A.end());
                                    sort(  B.begin(),   B.end());
                                    sort(A_B.begin(), A_B.end());
                                    for(i = 1; i < A_B.size(); i++)
                                        if(A_B[i] != A_B[i - 1])
                                            AB.push_back(A_B[i]);
                                    A_B.clear();
                                    for(i = 0; i < A.size(); i++)
                                    {
                                        for(j = 0; j < B.size(); j++)
                                        {
                                           if(A[i] == B[j])
                                              break;
                                        }
                                        if(j == B.size())
                                            A_B.push_back(A[i]);
                                    }
                                    for(i = 0; i < B.size(); i++)
                                    {
                                        for(j = 0; j < A.size(); j++)
                                        {
                                           if(B[i] == A[j])
                                              break;
                                        }
                                        if(j == A.size())
                                            B_A.push_back(B[i]);
                                    }
                                    //----------------------------------------------
                                    // print to file
                                    for(i = 0; i < AB.size(); i++)
                                    {
                                        char_utf32_to_utf8(AB[i], buffer);
                                        if(is_diacritic(AB[i]))
                                            out << ' ';
                                        out << buffer;
                                        if(is_diacritic(AB[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < AB.size(); i++)
                                    out << endl << endl;
                                    //----------------------------------------------
                                    out << "........................" << endl;
                                    out << A_name << " (in unicode order)" << endl;
                                    for(i = 0; i < A.size(); i++)
                                    {
                                        char_utf32_to_utf8(A[i], buffer);
                                        if(is_diacritic(A[i]))
                                            out << ' ';
                                        out << buffer;
                                        if(is_diacritic(A[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < A.size(); i++)
                                    out << endl  << endl << A_name << " not in " << B_name << endl;
                                    for(i = 0; i < A_B.size(); i++)
                                    {
                                        char_utf32_to_utf8(A_B[i], buffer);
                                        if(is_diacritic(A_B[i]))
                                            out << ' ';
                                        out << buffer;
                                        if(is_diacritic(A_B[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < A_B.size(); i++)
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    out << B_name << " (in unicode order)" << endl;
                                    for(i = 0; i < B.size(); i++)
                                    {
                                        char_utf32_to_utf8(B[i], buffer);
                                        if(is_diacritic(B[i]))
                                            out << ' ';
                                        out << buffer;
                                        if(is_diacritic(B[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < B.size(); i++)
                                    out << endl  << endl << B_name << " not in " << A_name << endl;
                                    for(i = 0; i < B_A.size(); i++)
                                    {
                                        char_utf32_to_utf8(B_A[i], buffer);
                                        if(is_diacritic(B_A[i]))
                                            out << ' ';
                                        out << buffer;
                                        if(is_diacritic(B_A[i]))
                                            out << ' ';
                                    } // end for(i = 0; i < B_A.size(); i++)
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    out << A_name << endl;
                                    out << "     frame  glyph  hex" << endl;
                                    for(i = 0; i < A.size(); i++)
                                    {
                                        char_utf32_to_utf8(A[i], buffer);
                                        out << "  "
                                            << dec
                                            << setw(8)
                                            << setfill(' ')
                                            << right
                                            << i
                                            << "    ";
                                        if(is_diacritic(A[i]))
                                        {
                                            has_diacritics = true;
                                            out << ' ';
                                        }
                                        out << buffer
                                            << "    0x"
                                            << hex
                                            << setw(8)
                                            << setfill('0')
                                            << right
                                            << A[i];
                                        if(is_diacritic(A[i]))
                                            out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                        out << endl;
                                    } // end for(i = 0; i < A.size(); i++)
                                    out << "........................" << endl;
                                    if(has_diacritics)
                                    {
                                        out << endl;
                                        out << A_name
                                            << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        has_diacritics = false;
                                    }
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    out << B_name << endl;
                                    out << "     frame  glyph  hex" << endl;
                                    for(i = 0; i < B.size(); i++)
                                    {
                                        char_utf32_to_utf8(B[i], buffer);
                                        out << "  "
                                            << dec
                                            << setw(8)
                                            << setfill(' ')
                                            << right
                                            << i
                                            << "    ";
                                        if(is_diacritic(B[i]))
                                        {
                                            has_diacritics = true;
                                            out << ' ';
                                        }
                                        out << buffer
                                            << "    0x"
                                            << hex
                                            << setw(8)
                                            << setfill('0')
                                            << right
                                            << B[i];
                                        if(is_diacritic(B[i]))
                                            out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                        out << endl;
                                    } // end for(i = 0; i < B.size(); i++)
                                    out << "........................" << endl;
                                    if(has_diacritics)
                                    {
                                        out << endl;
                                        out << B_name
                                            << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        has_diacritics = false;
                                    }
                                    out << endl << endl;
                                    out << "........................" << endl;
                                    out << A_name << " union " << B_name << endl;
                                    out << "     frame  glyph  hex" << endl;
                                    for(i = 0; i < AB.size(); i++)
                                    {
                                        char_utf32_to_utf8(AB[i], buffer);
                                        out << "  "
                                            << dec
                                            << setw(8)
                                            << setfill(' ')
                                            << right
                                            << i
                                            << "    ";
                                        if(is_diacritic(AB[i]))
                                        {
                                            has_diacritics = true;
                                            out << ' ';
                                        }
                                        out << buffer
                                            << "    0x"
                                            << hex
                                            << setw(8)
                                            << setfill('0')
                                            << right
                                            << AB[i];
                                        if(is_diacritic(AB[i]))
                                            out << "    O" << buffer << "    o" << buffer << "     " << buffer;
                                        out << endl;
                                    } // end for(i = 0; i < AB.size(); i++)
                                    out << "........................" << endl;
                                    if(has_diacritics)
                                    {
                                        out << endl;
                                        out << A_name
                                            << " union "
                                            << B_name
                                            << " contains diacritic marks that add to the previous character and do not advance the text cursor." << endl;
                                        has_diacritics = false;
                                    }
                                    out << endl;
                                    out.close();
                                    utf8_success = true;
                                } // end if(get_next_word(in, word, line_number)) // file_name B
                            } // end if(get_next_word(in, word, line_number)) // file_name A
                        } while(false);
                    } // end else if(word == "compare")
                    //--------------------------------------------------------
                    else // "utf8 (word)"
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " is not a recognized utf8 form. (ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                    //--------------------------------------------------------
                    if(!utf8_success)
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " syntax. (ERROR) "
                                  << report
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number)) (after finding utf8)
                //------------------------------------------------------------
                else
                {
                    // utf8 [nothing]
                    error_log << file_name
                              << " error on line: "
                              << line_number
                              << " \'text\' not followd by any form. (ERROR)"
                              << ENDL;
                    frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                }
                //------------------------------------------------------------
            } // end else if(word == "utf8")
            //----------------------------------------------------------------

            //----------------------------------------------------------------
            // text
            //----------------------------------------------------------------

            //----------------------------------------------------------------
            else if(word == "text")
            {
                bool                       text_success        = false,
                                           allow_lit_black_was = p_space->allow_lit_black,
                                           no_black_match_was  = p_space->no_black_match;
                string                     some_text,
                                           report;
                LaserBoy_3D_double         line_offset;
                //------------------------------------------------------------
                p_space->allow_lit_black = true;
                p_space->no_black_match  = false;
                //------------------------------------------------------------
                if(get_next_word(in, word, line_number))
                {
                    if(word == "fixed_space")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            mono_font_space = number;
                            text_success    = true;
                        }
                        else
                            report = " : text fixed_space missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "char_width")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            char_width   = number;
                            text_success = true;
                        }
                        else
                            report = " : text char_width missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "vari_space")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            vari_font_space = number;
                            text_success    = true;
                        }
                        else
                            report = " : text fixed_space missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "vari_gap")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            vari_font_gap = number;
                            text_success  = true;
                        }
                        else
                            report = " : text vari_space missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "super_diacritic_gap")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            super_diacritic_gap = number;
                            text_success  = true;
                        }
                        else
                            report = " : text super_diacritic_gap missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "line_space")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            line_space   = number;
                            text_success = true;
                        }
                        else
                            report = " : text line_space missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "to_space")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            to_space     = number;
                            text_success = true;
                        }
                        else
                            report = " : text to_space missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "color_rgb")
                    {
                        if(color.from_ifstream_txt(in, LASERBOY_RGB, line_number, report))
                            text_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "color_hex")
                    {
                        if(color.from_ifstream_txt(in, LASERBOY_HEX, line_number, report))
                            text_success = true;
                    }
                    //--------------------------------------------------------
                    else if(word == "bond_word")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            bond_word    = (bool)number;
                            text_success = true;
                        }
                        else
                            report = " : text bond_word missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "bond_line")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            bond_line    = (bool)number;
                            text_success = true;
                        }
                        else
                            report = " : text bond_line missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "move_per_frame")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number >= 0)
                            {
                                move_per_frame = number;
                                text_success   = true;
                            }
                        }
                        else
                            report = " : text move_per_frame missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "view_span")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number >= 0)
                            {
                                view_span    = number;
                                text_success = true;
                            }
                        }
                        else
                            report = " : text view_span missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "font_name")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            LaserBoy_Error_Code ret = LASERBOY_OK;
                            font_name = word;
                            if(LASERBOY_OK == font_frames.from_ild_file(LASERBOY_FONT_SHARE + font_name + ".ild", bytes_skipped, counter))
                            {
                                ret = font_frames.get_unicode_index(unicode_char_list);
                                if(ret & LASERBOY_INVALID_UNICODE)
                                {
                                    unicode_char_list = conv_utf8_utf32.from_bytes(string("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " : "
                                              << font_name
                                              << ".ild contains invalid unicode frame names."
                                              << ENDL;
                                }
                                else if(ret & LASERBOY_REDUNDANT_UNICODE)
                                {
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " : "
                                              << font_name
                                              << ".ild contains redundant unicode frame names."
                                              << ENDL;
                                }
                                else
                                    text_success = true;
                            } // end if(LASERBOY_OK == font_frames.from_ild_file(LASERBOY_FONT_SHARE + font_name + ".ild", bytes_skipped, counter))
                            else
                            {
                                font_frames.from_ild_file(LASERBOY_FONT_SHARE + "laserellipse.ild", bytes_skipped, counter);
                                font_frames.get_unicode_index(unicode_char_list);
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " : "
                                          << font_name
                                          << " is not a recognized font name. (ERROR)"
                                          << ENDL;
                                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        } // end if(get_next_word(in, word, line_number))
                        else
                            report = " : text font_name missing value.";
                    }
                    //--------------------------------------------------------
                    else if(word == "still_frames")
                    {
                        if(get_next_number(in, number, line_number))
                        {
                            if(number >= 0)
                            {
                                still_frames = (u_int)number;
                                text_success = true;
                            }
                        }
                        else
                            report = " : text still_frames missing value.";
                    }
                    //--------------------------------------------------------
                    // text generators
                    //--------------------------------------------------------

                    //--------------------------------------------------------
                    // font frames
                    //--------------------------------------------------------
                    else if(word == "characters") // text characters
                    {
                        string     all_text;
                        report.clear();
                        //------------------------------------------------
                        get_next_line(in, some_text, line_number);
                        all_text += some_text;
                        some_text.clear();
                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                        {
                            if(some_text.size() > 0)
                                all_text += some_text;
                            some_text.clear();
                            get_next_line(in, some_text, line_number);
                        }
                        if(all_text.size())
                        {
                            u32string unicode = conv_utf8_utf32.from_bytes(all_text);
                            if(unicode.size())
                            {
                                size_t     i = 0;
                                u32string  characters;
                                for(i = 0; i < unicode.size(); i++)
                                    if(unicode.at(i) > 0x00000020)
                                        characters.push_back(unicode.at(i));
                                if(characters.size())
                                {
                                    size_t                     char_index = 0;
                                    LaserBoy_real_segment      character,
                                                               diacritc;
                                    LaserBoy_real_segment_set  rss;
                                    LaserBoy_frame_set         frames;
                                    while(char_index < characters.size())
                                    {
                                        if(unicode_char_list.find(characters[char_index]) == string::npos) // character not found
                                            character = LaserBoy_real_segment(characters[char_index]);
                                        else
                                            character = font_frames[unicode_char_list.find(characters[char_index])].to_real_segment();
                                        //--------------------------------------------------------------------
                                        while(    (char_index + 1) < characters.size()
                                               && is_super_diacritic(characters[char_index + 1])
                                             )
                                        {
                                            if(unicode_char_list.find(characters[char_index + 1]) == string::npos) // character not found
                                                diacritc = LaserBoy_real_segment(characters[char_index + 1]);
                                            else
                                            {
                                                diacritc = font_frames[unicode_char_list.find(characters[char_index + 1])].to_real_segment();
                                                diacritc.move(LaserBoy_3D_double(   0.0,
                                                                                   -diacritc.min_y()
                                                                                 +  character.max_y()
                                                                                 +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                                                    0.0
                                                                                )
                                                            );
                                            }
                                            character += diacritc;
                                            character.convert_blank_to_black();
                                            char_index++;
                                        }
                                        //--------------------------------------------------------------------
                                        while(    (char_index + 1) < characters.size()
                                               && is_sub_diacritic(characters[char_index + 1])
                                             )
                                        {
                                            if(unicode_char_list.find(characters[char_index + 1]) == string::npos) // character not found
                                                diacritc = LaserBoy_real_segment(characters[char_index + 1]);
                                            else
                                                diacritc = font_frames[unicode_char_list.find(characters[char_index + 1])].to_real_segment();
                                            character += diacritc;
                                            character.convert_blank_to_black();
                                            char_index++;
                                        }
                                        rss += character;
                                        char_index++;
                                    }
                                    rss.normalize(false);
                                    rss.scale(LaserBoy_3D_double(0.5, 0.5, 0.0));
                                    rss.move(LaserBoy_3D_double(LASERBOY_HALF_SHORT, LASERBOY_HALF_SHORT, 0.0));
                                    frames = rss;
                                    *this += frames;
                                    text_frames_rendered += frames.size();
                                    text_success = true;
                                } // end if(characters.size())
                                else
                                    report = " : no text found in text chars.";
                            } // end if(unicode.size())
                            else
                                report = " : no text found in text chars.";
                        } // end if(all_text.size())
                        else
                            report = " : no text found in text chars.";
                    }
                    //--------------------------------------------------------
                    else if(word == "compose") // text compose
                    {
                                           size_t  i,
                                                   j,
                                                   k,
                                                   pos;
                                        u32string  unicode,
                                                   diacritics,
                                                   characters,
                                                   new_uni,
                                                   uni_name_list;

                               LaserBoy_3D_double  point;
                            LaserBoy_real_segment  character,
                                                   diacritc;
                        LaserBoy_real_segment_set  rss;
                        LaserBoy_frame_set         frames;

                        report.clear();
                        some_text.clear();
                        get_next_visible_line(in, some_text, line_number);

                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                        {
                            pos = some_text.find("diacritic");
                            if(pos != string::npos)
                            {
                                if(hex_string(some_text.substr(pos + 10), number))
                                {
                                    if(((char32_t)number))
                                        diacritics.push_back((char32_t)number);
                                }
                                else
                                {
                                    report = " : diacritic followed by non hex value in text compose.";
                                    break;
                                }
                            } // end if((pos = some_text.find("diacritic")) != string::npos)
                            else
                            {
                                pos = some_text.find("reverse");
                                if(pos != string::npos)
                                    diacritics.push_back((char32_t)0x0001);
                                else
                                {
                                    pos = some_text.find("invert");
                                    if(pos != string::npos)
                                        diacritics.push_back((char32_t)0x0002);
                                    else
                                    {
                                        pos = some_text.find("rotate");
                                        if(pos != string::npos)
                                            diacritics.push_back((char32_t)0x0003);
                                        else // next 2 lines are characters and new_uni
                                        {
                                            unicode = conv_utf8_utf32.from_bytes(some_text);
                                            for(i = 0; i < unicode.size(); i++)
                                                if(unicode.at(i) > 0x00000020)
                                                    characters.push_back(unicode.at(i));
                                            if(!characters.size())
                                            {
                                                report = " : no character list found in text compose.";
                                                break;
                                            }
                                            get_next_visible_line(in, some_text, line_number);
                                            unicode = conv_utf8_utf32.from_bytes(some_text);
                                            for(i = 0; i < unicode.size(); i++)
                                                if(unicode.at(i) > 0x00000020)
                                                    new_uni.push_back(unicode.at(i));
                                            if(characters.size() != new_uni.size())
                                            {
                                                report = " : number of characters and new unicode names is not the same in text compose.";
                                                break;
                                            }
                                            for(i = 0; i < characters.size(); i++)
                                            {
                                                if(unicode_char_list.find(characters[i]) == string::npos) // character not found
                                                    character = LaserBoy_real_segment(characters[i]);
                                                else
                                                    character = font_frames[unicode_char_list.find(characters[i])].to_real_segment();
                                                for(j = 0; j < diacritics.size(); j++)
                                                {
                                                    switch(diacritics[j])
                                                    {
                                                        case 0x0000:
                                                            break;
                                                        case 0x0001:
                                                            point = character.rectangular_center_of();
                                                            for(k = 0; k < character.size(); k++)
                                                                character.at(k) = rotate_vertex_on_coordinates_y(character.at(k), point, pi);
                                                            break;
                                                        case 0x0002:
                                                            point = character.rectangular_center_of();
                                                            for(k = 0; k < character.size(); k++)
                                                                character.at(k) = rotate_vertex_on_coordinates_x(character.at(k), point, pi);
                                                            break;
                                                        case 0x0003:
                                                            point = character.rectangular_center_of();
                                                            for(k = 0; k < character.size(); k++)
                                                                character.at(k) = rotate_vertex_on_coordinates_z(character.at(k), point, pi);
                                                            break;
                                                        default:
                                                            if(unicode_char_list.find(diacritics[j]) == string::npos) // character not found
                                                                diacritc = LaserBoy_real_segment(diacritics[j]);
                                                            else
                                                                diacritc = font_frames[unicode_char_list.find(diacritics[j])].to_real_segment();
                                                            if(is_super_diacritic(diacritics[j]))
                                                                diacritc.move(LaserBoy_3D_double(   0.0,
                                                                                                   -diacritc.min_y()
                                                                                                 +  character.max_y()
                                                                                                 +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                                                                    0.0
                                                                                                )
                                                                            );
                                                            character += diacritc;
                                                            character.convert_blank_to_black();
                                                            break;
                                                    }
                                                } // end for(i = 0; i < diacritcs.size(); i++)
                                                rss += character;
                                            } // end for(i = 0; i < characters.size(); i++)
                                            diacritics.clear();
                                            characters.clear();
                                            uni_name_list += new_uni;
                                            new_uni.clear();
                                        } // end else // next 2 lines are characters and new_uni
                                    }
                                }
                            } // end  if((pos = some_text.find("diacritic")) != string::npos)
                            get_next_visible_line(in, some_text, line_number);
                        } // while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                        rss.normalize(false);
                        rss.scale(LaserBoy_3D_double(0.499, 0.499, 0.0));
                        rss.move(LaserBoy_3D_double(LASERBOY_HALF_SHORT, LASERBOY_HALF_SHORT, 0.0));
                        frames = rss;
                        if(frames.size() == uni_name_list.size())
                            for(i = 0; i < frames.size(); i++)
                                frames.at(i).name = as_hex_string(uni_name_list[i]);
                        *this += frames;
                        text_frames_rendered += frames.size();
                        text_success = true;
                    }
                    //--------------------------------------------------------

                    //--------------------------------------------------------
                    // fixed (word)
                    //--------------------------------------------------------
                    else if(word == "fixed")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "line") // text fixed line
                            {
                                get_next_visible_line(in, some_text, line_number);
                                LaserBoy_real_segment rs(font_frames,
                                                         unicode_char_list,
                                                         some_text,
                                                         char_width,
                                                         mono_font_space,
                                                         super_diacritic_gap,
                                                         bond_word,
                                                         bond_line
                                                        );
                                rs.strip_color_rgb(color);
                                rs.normalize(false);
                                frame = rs;
                                frame.normalize(to_space);
                                for(u_int i = 0; i < still_frames; i++)
                                    push_back(frame);
                                text_frames_rendered += still_frames;
                                frame.clear();
                                text_success = true;
                            } // end else if(word == "line")
                            //------------------------------------------------
                            else if(word == "left") // text fixed left
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text fixed left lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           mono_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                    //----------------------------------------
                                    else if(word == "pages") // text fixed left pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                mono_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end else if(word == "pages") // text fixed left pages
                                } // end if(get_next_word(in, word, line_number))
                                //--------------------------------------------
                                else
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        mono_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            } // end else if(word == "left")
                            //------------------------------------------------
                            else if(word == "right") // text fixed right
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text fixed right lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           mono_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                line_offset.x = -rs.segment_right().x;
                                                rs.move(line_offset);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                    else if(word == "pages") // text fixed right pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                mono_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                line_offset.x = -rs2.segment_right().x;
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end else if(word == "pages") // text fixed right pages
                                } // end if(get_next_word(in, word, line_number))
                                //--------------------------------------------
                                else // text fixed right
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        mono_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        line_offset.x = -rs2.segment_right().x;
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            } // end else if(word == "right")
                            //------------------------------------------------
                            else if(word == "center") // text fixed center
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text fixed right lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           mono_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                line_offset.x = -rs.rectangular_center_of().x;
                                                rs.move(line_offset);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                    //----------------------------------------
                                    else if(word == "pages") // text fixed center pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                mono_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                line_offset.x = -rs2.rectangular_center_of().x;
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end else if(word == "pages") // text fixed center pages
                                }
                                //--------------------------------------------
                                else // text fixed center
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        mono_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        line_offset.x = -rs2.rectangular_center_of().x;
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            } // end else if(word == "center")
                            //------------------------------------------------
                            else if(word == "just") // text fixed just
                            {
                                int                                number_of_lines,
                                                                   line_index,
                                                                   line_number_was,
                                                                   line_diff;
                                LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                   each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                   page     (LASERBOY_ILDA_DEFAULT, false);
                                LaserBoy_real_segment_set          rss;
                                vector <LaserBoy_real_segment_set> rss_S;
                                word.clear();
                                line_number_was = line_number;
                                while(in.good())
                                {
                                    get_next_token(in, word, line_number);
                                    if(word == "<<__TEXT_END__>>")
                                        break;
                                    if(line_number - line_number_was)
                                    {
                                        rss_S.push_back(rss);
                                        for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                            rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        rss.clear();
                                        line_number_was = line_number;
                                    }
                                    rss += LaserBoy_real_segment(font_frames,
                                                                 unicode_char_list,
                                                                 word,
                                                                 char_width,
                                                                 mono_font_space,
                                                                 super_diacritic_gap,
                                                                 bond_word,
                                                                 bond_line
                                                                );
                                } // if(word == "<<__TEXT_END__>>") || EOF
                                if(line_number - line_number_was)
                                {
                                    rss_S.push_back(rss);
                                    for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                        rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                }
                                rss.clear();
                                number_of_lines = (int)rss_S.size();
                                if(number_of_lines > 0)
                                {
                                    int                word_index,
                                                       number_of_words   = 0;
                                    double             words_width       = 0.0,
                                                       line_width        = 0.0,
                                                       widest_line       = 0.0,
                                                       word_gap          = 0.0;
                                    LaserBoy_3D_double word_offset;
                                    for(line_index = 0; line_index < number_of_lines; line_index++)
                                    {
                                        number_of_words = (int)rss_S.at(line_index).size();
                                        words_width     = 0.0;
                                        line_width      = 0.0;
                                        for(word_index = 0; word_index < number_of_words; word_index++)
                                            words_width += rss_S.at(line_index).at(word_index).width();
                                        line_width =   words_width
                                                     + (   (number_of_words - 1)
                                                         * LASERBOY_MAX_SHORT
                                                         * mono_font_space
                                                       );
                                        if(widest_line < line_width)
                                            widest_line = line_width;
                                    } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                    word_offset.x = 0.0;
                                    word_offset.y = 0.0;
                                    word_offset.z = 0.0;
                                    for(line_index = 0; line_index < number_of_lines; line_index++)
                                    {
                                        word_offset.x = 0.0;
                                        number_of_words = (int)rss_S.at(line_index).size();
                                        words_width = 0.0;
                                        for(word_index = 0; word_index < number_of_words; word_index++)
                                            words_width += rss_S.at(line_index).at(word_index).width();
                                        if(number_of_words > 1) // more than one word.
                                            word_gap = (widest_line - words_width) / (number_of_words - 1);
                                        else
                                            word_gap = 0.0;
                                        if(number_of_words > 1)
                                        {
                                            each_word = rss_S.at(line_index).at(0);
                                            each_word.move(word_offset); // moves in Y
                                            each_line += each_word;
                                            word_offset.x += (each_word.width() + word_gap);
                                            for(word_index = 1; word_index < number_of_words; word_index++)
                                            {
                                                each_word = rss_S.at(line_index).at(word_index);
                                                each_word.move(word_offset);
                                                if(bond_line)
                                                    each_word.front().black();
                                                each_line += each_word;
                                                word_offset.x += (each_word.width() + word_gap);
                                            }
                                        }
                                        else if(number_of_words == 1)
                                        {
                                            each_word = rss_S.at(line_index).at(0);
                                            each_word.move(word_offset); // moves in Y
                                            each_line += each_word;
                                        }
                                        if(each_line.size() && bond_line)
                                        {
                                            each_line.convert_blank_to_black();
                                            each_line.front().blank();
                                        }
                                        page += each_line;
                                        each_line.clear();
                                        word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                    } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                    page.strip_color_rgb(color);
                                    page.normalize(false);
                                    frame = page;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                } // end if(rss_S.size())
                                rss_S.clear();
                            } // end else if(word == "just")
                            //------------------------------------------------
                            else if(word == "at") // text fixed at
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "pages") // text fixed at pages
                                    {
                                        if(get_next_number(in, number, line_number))
                                        {
                                            double chars_per_space = number;
                                            if(get_next_number(in, number, line_number))
                                            {
                                                double x_pos = number;
                                                if(get_next_number(in, number, line_number))
                                                {
                                                    int                        line_number_was = line_number;
                                                    double                     y_pos = number + 1;
                                                    LaserBoy_real_segment      rs1,
                                                                               rs2;
                                                    LaserBoy_real_segment_set  rss;
                                                    LaserBoy_frame_set         frames;
                                                    get_next_line(in, some_text, line_number);
                                                    line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                                    {
                                                        while(    in.good()
                                                               && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                               && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                             )
                                                        {
                                                            if(some_text.size() > 0)
                                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                                            unicode_char_list,
                                                                                            some_text,
                                                                                            char_width,
                                                                                            mono_font_space,
                                                                                            super_diacritic_gap,
                                                                                            bond_word,
                                                                                            bond_line
                                                                                           );
                                                            rs2.move(line_offset);
                                                            rs1 += rs2;
                                                            rs2.clear();
                                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                            get_next_line(in, some_text, line_number);
                                                        }
                                                        if(rs1.size())
                                                        {
                                                            rs1.strip_color_rgb(color);
                                                            rss.push_back(rs1);
                                                            rs1.clear();
                                                        }
                                                        if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                            break;
                                                        get_next_line(in, some_text, line_number);
                                                        line_offset.y = 0.0;
                                                    }
                                                    rss.move(LaserBoy_3D_double(x_pos * LASERBOY_MAX_SHORT,
                                                                                y_pos * LASERBOY_MAX_SHORT,
                                                                                0.0
                                                                               )
                                                            );
                                                    rss.scale_around_origin(1.0 / chars_per_space);
                                                    rss.clip();
                                                    frames = rss;
                                                    *this += frames;
                                                    text_frames_rendered += frames.size();
                                                    text_success = true;
                                                } // if(get_next_number(in, number, line_number))  y_pos
                                            } // if(get_next_number(in, number, line_number))  x_pos
                                        } // if(get_next_number(in, number, line_number))  chars_per_space
                                    } // end if(word == "pages")
                                } // end if(get_next_word(in, word, line_number))
                                //--------------------------------------------
                                else // text fixed at
                                {
                                    if(get_next_number(in, number, line_number))
                                    {
                                        double chars_per_space = number;
                                        if(get_next_number(in, number, line_number))
                                        {
                                            double x_pos = number;
                                            if(get_next_number(in, number, line_number))
                                            {
                                                double                 y_pos = number;
                                                LaserBoy_real_segment  rs1,
                                                                       rs2;
                                                while(in.good() && some_text.size() == 0)
                                                    get_next_line(in, some_text, line_number);
                                                while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                                {
                                                    if(some_text.size() > 0)
                                                        rs2 = LaserBoy_real_segment(font_frames,
                                                                                    unicode_char_list,
                                                                                    some_text,
                                                                                    char_width,
                                                                                    mono_font_space,
                                                                                    super_diacritic_gap,
                                                                                    bond_word,
                                                                                    bond_line
                                                                                   );
                                                    rs2.move(line_offset);
                                                    rs1 += rs2;
                                                    rs2.clear();
                                                    line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                    get_next_line(in, some_text, line_number);
                                                }
                                                rs1.strip_color_rgb(color);
                                                rs1.move(LaserBoy_3D_double(x_pos * LASERBOY_MAX_SHORT,
                                                                            y_pos * LASERBOY_MAX_SHORT,
                                                                            0.0
                                                                           )
                                                        );
                                                rs1.scale_around_origin(1.0 / chars_per_space);
                                                rs1.clip();
                                                frame = rs1;
                                                for(u_int i = 0; i < still_frames; i++)
                                                    push_back(frame);
                                                text_frames_rendered += still_frames;
                                                frame.clear();
                                                text_success = true;
                                            }
                                        }
                                    }
                                }
                            } // end else if(word == "at")
                            //------------------------------------------------
                            // animated!
                            //------------------------------------------------
                            else if(word == "marquee") // text fixed marquee
                            {
                                u_int                      i,
                                                           last_i;
                                double                     the_move = 0.0;
                                LaserBoy_3D_double         center,
                                                           max,
                                                           min;
                                LaserBoy_real_segment      rs1,
                                                           rs2;
                                LaserBoy_real_segment_set  rss;
                                LaserBoy_frame_set         frames;
                                get_next_line(in, some_text, line_number);
                                while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                {
                                    if(some_text.size() > 0)
                                        rs2 = LaserBoy_real_segment(font_frames,
                                                                    unicode_char_list,
                                                                    some_text,
                                                                    char_width,
                                                                    mono_font_space,
                                                                    super_diacritic_gap,
                                                                    bond_word,
                                                                    bond_line
                                                                   );
                                    rs2.move(line_offset);
                                    rs1 += rs2;
                                    rs2.clear();
                                    line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                    get_next_line(in, some_text, line_number);
                                }
                                rs1.strip_color_rgb(color);
                                min.x    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                max.x    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                min.y    = -rs1.height() / 2.0;
                                max.y    =  rs1.height() / 2.0;
                                min.z    = -32767.0;
                                max.z    =  32767.0;
                                center   = rs1.rectangular_center_of();
                                center.x =    rs1.segment_left().x
                                            - (   LASERBOY_MAX_SHORT
                                                * (   view_span
                                                    / 2.0
                                                  )
                                              );
                                last_i = (u_int)(   (   (   rs1.width()
                                                          / LASERBOY_MAX_SHORT
                                                        )
                                                        + view_span
                                                    ) // in squares
                                                    / move_per_frame
                                                );
                                the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                p_space->p_GUI->display_state("rendering text fixed marquee.");
                                for(i = 0; i <= last_i; i++)
                                {
                                    p_space->p_GUI->display_progress(last_i - i);
                                    rs2 = rs1;
                                    rs2.clip_around_coordinate(center, max, min, 65536.0);
                                    rss.push_back(rs2);
                                    center.x += the_move;
                                }
                                rss.normalize(false);
                                frames = rss;
                                frames.normalize(to_space);
                                *this += frames;
                                text_frames_rendered += frames.size();
                                frames.clear();
                                text_success = true;
                            } // end else if(word == "marquee")
                            //------------------------------------------------
                            else if(word == "scroll") // text fixed scroll
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "left") // text fixed scroll left
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            mono_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text fixed scroll left.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "left")
                                    //----------------------------------------
                                    else if(word == "right") // text fixed scroll right
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            mono_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            line_offset.x = -rs2.segment_right().x;
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text fixed scroll right.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "right")
                                    //----------------------------------------
                                    else if(word == "center") // text fixed scroll center
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            mono_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            line_offset.x = -rs2.rectangular_center_of().x;
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text fixed scroll center.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "center")
                                    //----------------------------------------
                                    else if(word == "just") // text fixed scroll just
                                    {
                                        u_int                              i,
                                                                           last_i;
                                        int                                number_of_lines,
                                                                           line_index,
                                                                           line_number_was,
                                                                           line_diff;
                                        double                             the_move = 0.0;
                                        LaserBoy_3D_double                 center,
                                                                           max,
                                                                           min;
                                        LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                           each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                           page     (LASERBOY_ILDA_DEFAULT, false);
                                        LaserBoy_real_segment_set          rss;
                                        LaserBoy_frame_set                 frames;
                                        vector <LaserBoy_real_segment_set> rss_S;
                                        word.clear();
                                        line_number_was = line_number;
                                        while(in.good())
                                        {
                                            get_next_token(in, word, line_number);
                                            if(word == "<<__TEXT_END__>>")
                                                break;
                                            if(line_number - line_number_was)
                                            {
                                                rss_S.push_back(rss);
                                                for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                                    rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                                rss.clear();
                                                line_number_was = line_number;
                                            }
                                            rss += LaserBoy_real_segment(font_frames,
                                                                         unicode_char_list,
                                                                         word,
                                                                         char_width,
                                                                         mono_font_space,
                                                                         super_diacritic_gap,
                                                                         bond_word,
                                                                         bond_line
                                                                        );
                                        } // if(word == "<<__TEXT_END__>>") || EOF
                                        if(line_number - line_number_was)
                                        {
                                            rss_S.push_back(rss);
                                            for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                                rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        }
                                        rss.clear();
                                        number_of_lines = (int)rss_S.size();
                                        if(number_of_lines > 0)
                                        {
                                            int                word_index,
                                                               number_of_words   = 0;
                                            double             words_width       = 0.0,
                                                               line_width        = 0.0,
                                                               widest_line       = 0.0,
                                                               word_gap          = 0.0;
                                            LaserBoy_3D_double word_offset;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width     = 0.0;
                                                line_width      = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                line_width =   words_width
                                                             + (   (number_of_words - 1)
                                                                 * LASERBOY_MAX_SHORT
                                                                 * mono_font_space
                                                               );
                                                if(widest_line < line_width)
                                                    widest_line = line_width;
                                            } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                            word_offset.x = 0.0;
                                            word_offset.y = 0.0;
                                            word_offset.z = 0.0;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                word_offset.x = 0.0;
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                if(number_of_words > 1) // more than one word.
                                                    word_gap = (widest_line - words_width) / (number_of_words - 1);
                                                else
                                                    word_gap = 0.0;
                                                if(number_of_words > 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                    word_offset.x += (each_word.width() + word_gap);
                                                    for(word_index = 1; word_index < number_of_words; word_index++)
                                                    {
                                                        each_word = rss_S.at(line_index).at(word_index);
                                                        each_word.move(word_offset);
                                                        if(bond_line)
                                                            each_word.front().black();
                                                        each_line += each_word;
                                                        word_offset.x += (each_word.width() + word_gap);
                                                    }
                                                }
                                                else if(number_of_words == 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                }
                                                if(each_line.size() && bond_line)
                                                {
                                                    each_line.convert_blank_to_black();
                                                    each_line.front().blank();
                                                }
                                                page += each_line;
                                                each_line.clear();
                                                word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                            page.strip_color_rgb(color);
                                            min.x    = -page.width() / 2.0;
                                            max.x    =  page.width() / 2.0;
                                            min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                            max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                            min.z    = -32767.0;
                                            max.z    =  32767.0;
                                            center   = page.rectangular_center_of();
                                            center.y =    page.segment_top().y
                                                        + (   LASERBOY_MAX_SHORT
                                                            * (   view_span
                                                                / 2.0
                                                              )
                                                          );
                                            last_i = (u_int)(   (   (   page.height()
                                                                      / LASERBOY_MAX_SHORT
                                                                    )
                                                                    + view_span
                                                                ) // in squares
                                                                / move_per_frame
                                                            );
                                            the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                            rss.clear();
                                            p_space->p_GUI->display_state("rendering text fixed scroll just.");
                                            for(i = 0; i <= last_i; i++)
                                            {
                                                p_space->p_GUI->display_progress(last_i - i);
                                                each_line = page;
                                                each_line.clip_around_coordinate(center, max, min, 65536.0);
                                                rss.push_back(each_line);
                                                center.y -= the_move;
                                            }
                                            rss.normalize(false);
                                            frames = rss;
                                            frames.normalize(to_space);
                                            *this += frames;
                                            text_frames_rendered += frames.size();
                                            frames.clear();
                                            text_success = true;
                                        } // end if(rss_S.size())
                                        rss_S.clear();
                                    } // end else if(word == "just")
                                    //----------------------------------------
                                    else if(word == "starwars") // text fixed scroll starwars
                                    {
                                        u_int                              i,
                                                                           last_i;
                                        int                                number_of_lines,
                                                                           line_index,
                                                                           line_number_was,
                                                                           line_diff;
                                        double                             the_move = 0.0;
                                        LaserBoy_3D_double                 center,
                                                                           max,
                                                                           min;
                                        LaserBoy_palette                   palette;
                                        LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                           each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                           page     (LASERBOY_ILDA_DEFAULT, false);
                                        LaserBoy_real_segment_set          rss;
                                        LaserBoy_frame_set                 frames;
                                        vector <LaserBoy_real_segment_set> rss_S;
                                        word.clear();
                                        line_number_was = line_number;
                                        while(in.good())
                                        {
                                            get_next_token(in, word, line_number);
                                            if(word == "<<__TEXT_END__>>")
                                                break;
                                            if(line_number - line_number_was)
                                            {
                                                rss_S.push_back(rss);
                                                for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                                    rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                                rss.clear();
                                                line_number_was = line_number;
                                            }
                                            rss += LaserBoy_real_segment(font_frames,
                                                                         unicode_char_list,
                                                                         word,
                                                                         char_width,
                                                                         mono_font_space,
                                                                         super_diacritic_gap,
                                                                         bond_word,
                                                                         bond_line
                                                                        );
                                        } // if(word == "<<__TEXT_END__>>") || EOF
                                        if(line_number - line_number_was)
                                        {
                                            rss_S.push_back(rss);
                                            for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                                rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        }
                                        rss.clear();
                                        number_of_lines = (int)rss_S.size();
                                        if(number_of_lines > 0)
                                        {
                                            int                word_index,
                                                               number_of_words   = 0;
                                            double             words_width       = 0.0,
                                                               line_width        = 0.0,
                                                               widest_line       = 0.0,
                                                               word_gap          = 0.0,
                                                               window_height     = 0.0;
                                            LaserBoy_3D_double word_offset;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width     = 0.0;
                                                line_width      = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                line_width =   words_width
                                                             + (   (number_of_words - 1)
                                                                 * LASERBOY_MAX_SHORT
                                                                 * mono_font_space
                                                               );
                                                if(widest_line < line_width)
                                                    widest_line = line_width;
                                            } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                            word_offset.x = 0.0;
                                            word_offset.y = 0.0;
                                            word_offset.z = 0.0;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                word_offset.x = 0.0;
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                if(number_of_words > 1) // more than one word.
                                                    word_gap = (widest_line - words_width) / (number_of_words - 1);
                                                else
                                                    word_gap = 0.0;
                                                if(number_of_words > 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                    word_offset.x += (each_word.width() + word_gap);
                                                    for(word_index = 1; word_index < number_of_words; word_index++)
                                                    {
                                                        each_word = rss_S.at(line_index).at(word_index);
                                                        each_word.move(word_offset);
                                                        if(bond_line)
                                                            each_word.front().black();
                                                        each_line += each_word;
                                                        word_offset.x += (each_word.width() + word_gap);
                                                    }
                                                }
                                                else if(number_of_words == 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                }
                                                if(each_line.size() && bond_line)
                                                {
                                                    each_line.convert_blank_to_black();
                                                    each_line.front().blank();
                                                }
                                                page += each_line;
                                                each_line.clear();
                                                word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                            line_width    =  page.width();
                                            min.x         = -line_width / 2.0;
                                            max.x         =  line_width / 2.0;
                                            min.y         = -line_width;
                                            max.y         =  line_width;
                                            min.z         = -32767.0;
                                            max.z         =  32767.0;
                                            center        =  page.rectangular_center_of();
                                            center.y      =  page.segment_top().y + line_width;
                                            window_height = line_width * 2.0;
                                            last_i = (u_int)( (   (page.height() / LASERBOY_MAX_SHORT) // whole page
                                                                + (window_height / LASERBOY_MAX_SHORT) // the window
                                                              ) // in squares
                                                              / move_per_frame
                                                            );
                                            the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                            rss.clear();
                                            page.strip_color_rgb(color);
                                            page.palette_index = LASERBOY_TRUE_COLOR;
                                            page.add_lit_span_vertices(512);
                                            for(i = 0; i < 256; i++)
                                                palette.push_back(color.blend(LaserBoy_color(0, 0, 0), i / 255.0));
                                            palette.first = 0;
                                            palette.last  = 254;
                                            palette.white = 0;
                                            palette.black = 255;
                                            palette.name = GUID8char();
                                            *p_space += palette;
                                            p_space->p_GUI->display_state("rendering text fixed scroll starwars.");
                                            for(i = 0; i <= last_i; i++)
                                            {
                                                p_space->p_GUI->display_progress(last_i - i);
                                                each_line = page;
                                                each_line.clip_around_coordinate(center, max, min, 65536.0);
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - (each_line.at(_i).y + max.y)) / window_height;
                                                    if(each_line.at(_i).is_color())
                                                    {
                                                        each_line.at(_i).r = color.r * words_width;
                                                        each_line.at(_i).g = color.g * words_width;
                                                        each_line.at(_i).b = color.b * words_width;
                                                    }
                                                }
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - each_line.at(_i).y) / window_height;
                                                    words_width *= 0.5;
                                                    each_line.at(_i) = LaserBoy_3D_double(each_line.at(_i).x,
                                                                                          each_line.at(_i).y * words_width,
                                                                                          -each_line.at(_i).y
                                                                                         );
                                                }
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - (each_line.at(_i).y + max.y)) / window_height;
                                                    words_width *= 2.0;
                                                    each_line.at(_i) = LaserBoy_3D_double(each_line.at(_i).x * words_width,
                                                                                          each_line.at(_i).y,
                                                                                          -each_line.at(_i).y
                                                                                         );
                                                }
                                                each_line.reduce_lit_vectors();
                                                each_line.best_match_palette(p_space->number_of_palettes() - 1);
                                                rss.push_back(each_line);
                                                center.y -= the_move;
                                            }
                                            rss.normalize(false);
                                            frames = rss;
                                            frames.normalize(to_space);
                                            *this += frames;
                                            text_frames_rendered += frames.size();
                                            frames.clear();
                                            text_success = true;
                                        } // end if(rss_S.size())
                                        rss_S.clear();
                                    } // end else if(word == "starwars")
                                    else // end if(word == "left")
                                    {
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " :  text fixed scroll "
                                                  << word
                                                  << " is not a recognized form. (ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            } // end else if(word == "scroll")
                        }
                    } // end if(word == "fixed")
                    //--------------------------------------------------------
                    // text generators
                    //--------------------------------------------------------
                    // variable
                    //--------------------------------------------------------
                    else if(word == "variable")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "line") // text variable line
                            {
                                get_next_visible_line(in, some_text, line_number);
                                LaserBoy_real_segment rs(font_frames,
                                                         unicode_char_list,
                                                         some_text,
                                                         char_width,
                                                         vari_font_gap,
                                                         vari_font_space,
                                                         super_diacritic_gap,
                                                         bond_word,
                                                         bond_line
                                                        );
                                rs.strip_color_rgb(color);
                                rs.normalize(false);
                                frame = rs;
                                frame.normalize(to_space);
                                for(u_int i = 0; i < still_frames; i++)
                                    push_back(frame);
                                text_frames_rendered += still_frames;
                                frame.clear();
                                text_success = true;
                            } // end else if(word == "line")
                            //------------------------------------------------
                            else if(word == "left") // text variable left
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text variable left lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           vari_font_gap,
                                                                           vari_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end if(word == "lines")
                                    //----------------------------------------
                                    else if(word == "pages") // text variable left pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                vari_font_gap,
                                                                                vari_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                //--------------------------------------------
                                } // end if(get_next_word(in, word, line_number))
                                else // text variable left
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        vari_font_gap,
                                                                        vari_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            }
                            //------------------------------------------------
                            else if(word == "right") // text variable right
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text fixed left lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           vari_font_gap,
                                                                           vari_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                line_offset.x = -rs.segment_right().x;
                                                rs.move(line_offset);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end if(word == "lines")
                                    //----------------------------------------
                                    else if(word == "pages") // text variable left pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                vari_font_gap,
                                                                                vari_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                line_offset.x = -rs2.segment_right().x;
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                }
                                //--------------------------------------------
                                else
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        vari_font_gap,
                                                                        vari_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        line_offset.x = -rs2.segment_right().x;
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            }
                            //------------------------------------------------
                            else if(word == "center") // text variable center
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "lines") // text fixed left lines
                                    {
                                        LaserBoy_real_segment      rs;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs = LaserBoy_real_segment(font_frames,
                                                                           unicode_char_list,
                                                                           some_text,
                                                                           char_width,
                                                                           vari_font_gap,
                                                                           vari_font_space,
                                                                           super_diacritic_gap,
                                                                           bond_word,
                                                                           bond_line
                                                                          );
                                            if(rs.size())
                                            {
                                                rs.strip_color_rgb(color);
                                                line_offset.x = -rs.rectangular_center_of().x;
                                                rs.move(line_offset);
                                                rss.push_back(rs);
                                                rs.clear();
                                            }
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    } // end if(word == "lines")
                                    //----------------------------------------
                                    else if(word == "pages") // text variable left pages
                                    {
                                        int                        line_number_was = line_number;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            while(    in.good()
                                                   && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                   && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                 )
                                            {
                                                if(some_text.size() > 0)
                                                    rs2 = LaserBoy_real_segment(font_frames,
                                                                                unicode_char_list,
                                                                                some_text,
                                                                                char_width,
                                                                                vari_font_gap,
                                                                                vari_font_space,
                                                                                super_diacritic_gap,
                                                                                bond_word,
                                                                                bond_line
                                                                               );
                                                line_offset.x = -rs2.rectangular_center_of().x;
                                                rs2.move(line_offset);
                                                rs1 += rs2;
                                                rs2.clear();
                                                line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                get_next_line(in, some_text, line_number);
                                            }
                                            if(rs1.size())
                                            {
                                                rs1.strip_color_rgb(color);
                                                rss.push_back(rs1);
                                                rs1.clear();
                                            }
                                            if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                break;
                                            get_next_line(in, some_text, line_number);
                                            line_offset.y = 0.0;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        text_success = true;
                                    }
                                }
                                //--------------------------------------------
                                else // text variable center
                                {
                                    LaserBoy_real_segment rs1,
                                                          rs2;
                                    get_next_line(in, some_text, line_number);
                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                    {
                                        if(some_text.size() > 0)
                                            rs2 = LaserBoy_real_segment(font_frames,
                                                                        unicode_char_list,
                                                                        some_text,
                                                                        char_width,
                                                                        vari_font_gap,
                                                                        vari_font_space,
                                                                        super_diacritic_gap,
                                                                        bond_word,
                                                                        bond_line
                                                                       );
                                        line_offset.x = -rs2.rectangular_center_of().x;
                                        rs2.move(line_offset);
                                        rs1 += rs2;
                                        rs2.clear();
                                        line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                        get_next_line(in, some_text, line_number);
                                    }
                                    rs1.strip_color_rgb(color);
                                    rs1.normalize(false);
                                    frame = rs1;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                }
                            }
                            //------------------------------------------------
                            else if(word == "just") // text variable just
                            {
                                int                                number_of_lines,
                                                                   line_index,
                                                                   line_number_was,
                                                                   line_diff;
                                LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                   each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                   page     (LASERBOY_ILDA_DEFAULT, false);
                                LaserBoy_real_segment_set          rss;
                                vector <LaserBoy_real_segment_set> rss_S;
                                word.clear();
                                line_number_was = line_number;
                                while(in.good())
                                {
                                    get_next_token(in, word, line_number);
                                    if(word == "<<__TEXT_END__>>")
                                        break;
                                    if(line_number - line_number_was)
                                    {
                                        rss_S.push_back(rss);
                                        for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                            rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        rss.clear();
                                        line_number_was = line_number;
                                    }
                                    rss += LaserBoy_real_segment(font_frames,
                                                                 unicode_char_list,
                                                                 word,
                                                                 char_width,
                                                                 vari_font_gap,
                                                                 vari_font_space,
                                                                 super_diacritic_gap,
                                                                 bond_word,
                                                                 bond_line
                                                                );
                                } // if(word == "<<__TEXT_END__>>") || EOF
                                if(line_number - line_number_was)
                                {
                                    rss_S.push_back(rss);
                                    for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                        rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                }
                                rss.clear();
                                number_of_lines = (int)rss_S.size();
                                if(number_of_lines > 0)
                                {
                                    int                word_index,
                                                       number_of_words   = 0;
                                    double             words_width       = 0.0,
                                                       line_width        = 0.0,
                                                       widest_line       = 0.0,
                                                       word_gap          = 0.0;
                                    LaserBoy_3D_double word_offset;
                                    for(line_index = 0; line_index < number_of_lines; line_index++)
                                    {
                                        number_of_words = (int)rss_S.at(line_index).size();
                                        words_width     = 0.0;
                                        line_width      = 0.0;
                                        for(word_index = 0; word_index < number_of_words; word_index++)
                                            words_width += rss_S.at(line_index).at(word_index).width();
                                        if(number_of_words > 1)
                                            line_width =   words_width
                                                         + (   (number_of_words - 1)
                                                             * LASERBOY_MAX_SHORT
                                                             * vari_font_space
                                                           );
                                        else
                                            line_width = words_width;
                                        if(widest_line < line_width)
                                            widest_line = line_width;
                                    } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                    word_offset.x = 0.0;
                                    word_offset.y = 0.0;
                                    word_offset.z = 0.0;
                                    for(line_index = 0; line_index < number_of_lines; line_index++)
                                    {
                                        word_offset.x = 0.0;
                                        number_of_words = (int)rss_S.at(line_index).size();
                                        words_width = 0.0;
                                        for(word_index = 0; word_index < number_of_words; word_index++)
                                            words_width += rss_S.at(line_index).at(word_index).width();
                                        if(number_of_words > 1) // more than one word.
                                            word_gap = (widest_line - words_width) / (number_of_words - 1);
                                        else
                                            word_gap = 0.0;
                                        if(number_of_words > 1)
                                        {
                                            each_word = rss_S.at(line_index).at(0);
                                            each_word.move(word_offset); // moves in Y
                                            each_line += each_word;
                                            word_offset.x += (each_word.width() + word_gap);
                                            for(word_index = 1; word_index < number_of_words; word_index++)
                                            {
                                                each_word = rss_S.at(line_index).at(word_index);
                                                each_word.move(word_offset);
                                                if(bond_line)
                                                    each_word.front().black();
                                                each_line += each_word;
                                                word_offset.x += (each_word.width() + word_gap);
                                            }
                                        }
                                        else if(number_of_words == 1)
                                        {
                                            each_word = rss_S.at(line_index).at(0);
                                            each_word.move(word_offset); // moves in Y
                                            each_line += each_word;
                                        }
                                        if(each_line.size() && bond_line)
                                        {
                                            each_line.convert_blank_to_black();
                                            each_line.front().blank();
                                        }
                                        page += each_line;
                                        each_line.clear();
                                        word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                    } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                    page.strip_color_rgb(color);
                                    page.normalize(false);
                                    frame = page;
                                    frame.normalize(to_space);
                                    for(u_int i = 0; i < still_frames; i++)
                                        push_back(frame);
                                    text_frames_rendered += still_frames;
                                    frame.clear();
                                    text_success = true;
                                } // end if(rss_S.size())
                                rss_S.clear();
                            } // end else if(word == "just")
                            //------------------------------------------------
                            else if(word == "at") // text variable at
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "pages") // text variable at pages
                                    {
                                        if(get_next_number(in, number, line_number))
                                        {
                                            double chars_per_space = number;
                                            if(get_next_number(in, number, line_number))
                                            {
                                                double x_pos = number;
                                                if(get_next_number(in, number, line_number))
                                                {
                                                    double                     y_pos = number + 1;
                                                    int                        line_number_was = line_number;
                                                    LaserBoy_real_segment      rs1,
                                                                               rs2;
                                                    LaserBoy_real_segment_set  rss;
                                                    LaserBoy_frame_set         frames;
                                                    get_next_line(in, some_text, line_number);
                                                    line_offset.y += ((line_number - line_number_was) * (LASERBOY_MAX_SHORT * line_space));
                                                    while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                                    {
                                                        while(    in.good()
                                                               && some_text.substr(0, 16) != "<<__TEXT_END__>>"
                                                               && some_text.substr(0, 16) != "<<__PAGE_END__>>"
                                                             )
                                                        {
                                                            if(some_text.size() > 0)
                                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                                            unicode_char_list,
                                                                                            some_text,
                                                                                            char_width,
                                                                                            vari_font_gap,
                                                                                            vari_font_space,
                                                                                            super_diacritic_gap,
                                                                                            bond_word,
                                                                                            bond_line
                                                                                           );
                                                            rs2.move(line_offset);
                                                            rs1 += rs2;
                                                            rs2.clear();
                                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                            get_next_line(in, some_text, line_number);
                                                        }
                                                        if(rs1.size())
                                                        {
                                                            rs1.strip_color_rgb(color);
                                                            rss.push_back(rs1);
                                                            rs1.clear();
                                                        }
                                                        if(some_text.substr(0, 16) == "<<__TEXT_END__>>")
                                                            break;
                                                        get_next_line(in, some_text, line_number);
                                                        line_offset.y = 0.0;
                                                    }
                                                    rss.move(LaserBoy_3D_double(x_pos * LASERBOY_MAX_SHORT,
                                                                                y_pos * LASERBOY_MAX_SHORT,
                                                                                0.0
                                                                               )
                                                            );
                                                    rss.scale_around_origin(1.0 / chars_per_space);
                                                    rss.clip();
                                                    frames = rss;
                                                    *this += frames;
                                                    text_frames_rendered += frames.size();
                                                    text_success = true;
                                                } // if(get_next_number(in, number, line_number))  y_pos
                                            } // if(get_next_number(in, number, line_number))  x_pos
                                        } // if(get_next_number(in, number, line_number))  chars_per_space
                                    } // end if(word == "pages")
                                } // end if(get_next_word(in, word, line_number))
                                //--------------------------------------------
                                else // text variable at
                                {
                                    if(get_next_number(in, number, line_number))
                                    {
                                        double chars_per_space = number;
                                        if(get_next_number(in, number, line_number))
                                        {
                                            double x_pos = number;
                                            if(get_next_number(in, number, line_number))
                                            {
                                                double                 y_pos = number;
                                                LaserBoy_real_segment  rs1,
                                                                       rs2;
                                                while(in.good() && some_text.size() == 0)
                                                    get_next_line(in, some_text, line_number);
                                                while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                                {
                                                    if(some_text.size() > 0)
                                                        rs2 = LaserBoy_real_segment(font_frames,
                                                                                    unicode_char_list,
                                                                                    some_text,
                                                                                    char_width,
                                                                                    vari_font_gap,
                                                                                    vari_font_space,
                                                                                    super_diacritic_gap,
                                                                                    bond_word,
                                                                                    bond_line
                                                                                   );
                                                    rs2.move(line_offset);
                                                    rs1 += rs2;
                                                    rs2.clear();
                                                    line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                                    get_next_line(in, some_text, line_number);
                                                }
                                                rs1.strip_color_rgb(color);
                                                rs1.move(LaserBoy_3D_double(x_pos * LASERBOY_MAX_SHORT,
                                                                            y_pos * LASERBOY_MAX_SHORT,
                                                                            0.0
                                                                           )
                                                        );
                                                rs1.scale_around_origin(1.0 / chars_per_space);
                                                rs1.clip();
                                                frame = rs1;
                                                for(u_int i = 0; i < still_frames; i++)
                                                    push_back(frame);
                                                text_frames_rendered += still_frames;
                                                frame.clear();
                                                text_success = true;
                                            } // end if(get_next_number(in, number, line_number)) y_pos
                                        } // end if(get_next_number(in, number, line_number)) x_pos
                                    } // end if(get_next_number(in, number, line_number)) chars_per_space
                                } // end else (word != "pages")
                            } // end else if(word == "at")
                            //------------------------------------------------
                            else if(word == "marquee") // text variable marquee (animated)
                            {
                                u_int                      i,
                                                           last_i;
                                double                     the_move = 0.0;
                                LaserBoy_3D_double         center,
                                                           max,
                                                           min;
                                LaserBoy_real_segment      rs1,
                                                           rs2;
                                LaserBoy_real_segment_set  rss;
                                LaserBoy_frame_set         frames;
                                get_next_line(in, some_text, line_number);
                                while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                {
                                    if(some_text.size() > 0)
                                        rs2 = LaserBoy_real_segment(font_frames,
                                                                    unicode_char_list,
                                                                    some_text,
                                                                    char_width,
                                                                    vari_font_gap,
                                                                    vari_font_space,
                                                                    super_diacritic_gap,
                                                                    bond_word,
                                                                    bond_line
                                                                   );
                                    rs2.move(line_offset);
                                    rs1 += rs2;
                                    rs2.clear();
                                    line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                    get_next_line(in, some_text, line_number);
                                }
                                rs1.strip_color_rgb(color);
                                min.x    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                max.x    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                min.y    = -rs1.height() / 2.0;
                                max.y    =  rs1.height() / 2.0;
                                min.z    = -32767.0;
                                max.z    =  32767.0;
                                center   = rs1.rectangular_center_of();
                                center.x =    rs1.segment_left().x
                                            - (   LASERBOY_MAX_SHORT
                                                * (   view_span
                                                    / 2.0
                                                  )
                                              );
                                last_i = (u_int)(   (   (   rs1.width()
                                                          / LASERBOY_MAX_SHORT
                                                        )
                                                        + view_span
                                                    ) // in squares
                                                    / move_per_frame
                                                );
                                the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                p_space->p_GUI->display_state("rendering text variable marquee.");
                                for(i = 0; i <= last_i; i++)
                                {
                                    p_space->p_GUI->display_progress(last_i - i);
                                    rs2 = rs1;
                                    rs2.clip_around_coordinate(center, max, min, 65536.0);
                                    rss.push_back(rs2);
                                    center.x += the_move;
                                }
                                rss.normalize(false);
                                frames = rss;
                                frames.normalize(to_space);
                                *this += frames;
                                text_frames_rendered += frames.size();
                                frames.clear();
                                text_success = true;
                            } // end else if(word == "marquee")
                            //------------------------------------------------
                            else if(word == "scroll") // text variable scroll
                            {
                                if(get_next_word(in, word, line_number))
                                {
                                    if(word == "left") // text variable scroll left
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            vari_font_gap,
                                                                            vari_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text variable scroll left.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "left")
                                    //----------------------------------------
                                    else if(word == "right") // text variable scroll right
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            vari_font_gap,
                                                                            vari_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            line_offset.x = -rs2.segment_right().x;
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text variable scroll right.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "right")
                                    //----------------------------------------
                                    else if(word == "center") // text variable scroll center
                                    {
                                        u_int                      i,
                                                                   last_i;
                                        double                     the_move = 0.0;
                                        LaserBoy_3D_double         center,
                                                                   max,
                                                                   min;
                                        LaserBoy_real_segment      rs1,
                                                                   rs2;
                                        LaserBoy_real_segment_set  rss;
                                        LaserBoy_frame_set         frames;
                                        get_next_line(in, some_text, line_number);
                                        while(in.good() && some_text.substr(0, 16) != "<<__TEXT_END__>>")
                                        {
                                            if(some_text.size() > 0)
                                                rs2 = LaserBoy_real_segment(font_frames,
                                                                            unicode_char_list,
                                                                            some_text,
                                                                            char_width,
                                                                            vari_font_gap,
                                                                            vari_font_space,
                                                                            super_diacritic_gap,
                                                                            bond_word,
                                                                            bond_line
                                                                           );
                                            line_offset.x = -rs2.rectangular_center_of().x;
                                            rs2.move(line_offset);
                                            rs1 += rs2;
                                            rs2.clear();
                                            line_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            get_next_line(in, some_text, line_number);
                                        }
                                        rs1.strip_color_rgb(color);
                                        min.x    = -rs1.width() / 2.0;
                                        max.x    =  rs1.width() / 2.0;
                                        min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                        min.z    = -32767.0;
                                        max.z    =  32767.0;
                                        center   = rs1.rectangular_center_of();
                                        center.y =    rs1.segment_top().y
                                                    + (   LASERBOY_MAX_SHORT
                                                        * (   view_span
                                                            / 2.0
                                                          )
                                                      );
                                        last_i = (u_int)(   (   (   rs1.height()
                                                                  / LASERBOY_MAX_SHORT
                                                                )
                                                                + view_span
                                                            ) // in squares
                                                            / move_per_frame
                                                        );
                                        the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                        p_space->p_GUI->display_state("rendering text variable scroll center.");
                                        for(i = 0; i <= last_i; i++)
                                        {
                                            p_space->p_GUI->display_progress(last_i - i);
                                            rs2 = rs1;
                                            rs2.clip_around_coordinate(center, max, min, 65536.0);
                                            rss.push_back(rs2);
                                            center.y -= the_move;
                                        }
                                        rss.normalize(false);
                                        frames = rss;
                                        frames.normalize(to_space);
                                        *this += frames;
                                        text_frames_rendered += frames.size();
                                        frames.clear();
                                        text_success = true;
                                    } // end if(word == "center")
                                    //----------------------------------------
                                    else if(word == "just") // text variable scroll just
                                    {
                                        u_int                              i,
                                                                           last_i;
                                        int                                number_of_lines,
                                                                           line_index,
                                                                           line_number_was,
                                                                           line_diff;
                                        double                             the_move = 0.0;
                                        LaserBoy_3D_double                 center,
                                                                           max,
                                                                           min;
                                        LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                           each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                           page     (LASERBOY_ILDA_DEFAULT, false);
                                        LaserBoy_real_segment_set          rss;
                                        LaserBoy_frame_set                 frames;
                                        vector <LaserBoy_real_segment_set> rss_S;
                                        word.clear();
                                        line_number_was = line_number;
                                        while(in.good())
                                        {
                                            get_next_token(in, word, line_number);
                                            if(word == "<<__TEXT_END__>>")
                                                break;
                                            if(line_number - line_number_was)
                                            {
                                                rss_S.push_back(rss);
                                                for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                                    rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                                rss.clear();
                                                line_number_was = line_number;
                                            }
                                            rss += LaserBoy_real_segment(font_frames,
                                                                         unicode_char_list,
                                                                         word,
                                                                         char_width,
                                                                         vari_font_gap,
                                                                         vari_font_space,
                                                                         super_diacritic_gap,
                                                                         bond_word,
                                                                         bond_line
                                                                        );
                                        } // if(word == "<<__TEXT_END__>>") || EOF
                                        if(line_number - line_number_was)
                                        {
                                            rss_S.push_back(rss);
                                            for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                                rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        }
                                        rss.clear();
                                        number_of_lines = (int)rss_S.size();
                                        if(number_of_lines > 0)
                                        {
                                            int                word_index,
                                                               number_of_words   = 0;
                                            double             words_width       = 0.0,
                                                               line_width        = 0.0,
                                                               widest_line       = 0.0,
                                                               word_gap          = 0.0;
                                            LaserBoy_3D_double word_offset;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width     = 0.0;
                                                line_width      = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                line_width =   words_width
                                                             + (   (number_of_words - 1)
                                                                 * LASERBOY_MAX_SHORT
                                                                 * vari_font_space
                                                               );
                                                if(widest_line < line_width)
                                                    widest_line = line_width;
                                            } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                            word_offset.x = 0.0;
                                            word_offset.y = 0.0;
                                            word_offset.z = 0.0;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                word_offset.x = 0.0;
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                if(number_of_words > 1) // more than one word.
                                                    word_gap = (widest_line - words_width) / (number_of_words - 1);
                                                else
                                                    word_gap = 0.0;
                                                if(number_of_words > 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                    word_offset.x += (each_word.width() + word_gap);
                                                    for(word_index = 1; word_index < number_of_words; word_index++)
                                                    {
                                                        each_word = rss_S.at(line_index).at(word_index);
                                                        each_word.move(word_offset);
                                                        if(bond_line)
                                                            each_word.front().black();
                                                        each_line += each_word;
                                                        word_offset.x += (each_word.width() + word_gap);
                                                    }
                                                }
                                                else if(number_of_words == 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                }
                                                if(each_line.size() && bond_line)
                                                {
                                                    each_line.convert_blank_to_black();
                                                    each_line.front().blank();
                                                }
                                                page += each_line;
                                                each_line.clear();
                                                word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                            page.strip_color_rgb(color);
                                            min.x    = -page.width() / 2.0;
                                            max.x    =  page.width() / 2.0;
                                            min.y    = -(view_span * LASERBOY_MAX_SHORT) / 2.0;
                                            max.y    =  (view_span * LASERBOY_MAX_SHORT) / 2.0;
                                            min.z    = -32767.0;
                                            max.z    =  32767.0;
                                            center   = page.rectangular_center_of();
                                            center.y =    page.segment_top().y
                                                        + (   LASERBOY_MAX_SHORT
                                                            * (   view_span
                                                                / 2.0
                                                              )
                                                          );
                                            last_i = (u_int)(   (   (   page.height()
                                                                      / LASERBOY_MAX_SHORT
                                                                    )
                                                                    + view_span
                                                                ) // in squares
                                                                / move_per_frame
                                                            );
                                            the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                            rss.clear();
                                            p_space->p_GUI->display_state("rendering text variable scroll just.");
                                            for(i = 0; i <= last_i; i++)
                                            {
                                                p_space->p_GUI->display_progress(last_i - i);
                                                each_line = page;
                                                each_line.clip_around_coordinate(center, max, min, 65536.0);
                                                rss.push_back(each_line);
                                                center.y -= the_move;
                                            }
                                            rss.normalize(false);
                                            frames = rss;
                                            frames.normalize(to_space);
                                            *this += frames;
                                            text_frames_rendered += frames.size();
                                            frames.clear();
                                            text_success = true;
                                        } // end if(rss_S.size())
                                        rss_S.clear();
                                    } // end else if(word == "just")
                                    //----------------------------------------
                                    else if(word == "starwars") // text variable scroll starwars
                                    {
                                        u_int                              i,
                                                                           last_i;
                                        int                                number_of_lines,
                                                                           line_index,
                                                                           line_number_was,
                                                                           line_diff;
                                        double                             the_move = 0.0;
                                        LaserBoy_3D_double                 center,
                                                                           max,
                                                                           min;
                                        LaserBoy_palette                   palette;
                                        LaserBoy_real_segment              each_word(LASERBOY_ILDA_DEFAULT, false),
                                                                           each_line(LASERBOY_ILDA_DEFAULT, false),
                                                                           page     (LASERBOY_ILDA_DEFAULT, false);
                                        LaserBoy_real_segment_set          rss;
                                        LaserBoy_frame_set                 frames;
                                        vector <LaserBoy_real_segment_set> rss_S;
                                        word.clear();
                                        line_number_was = line_number;
                                        while(in.good())
                                        {
                                            get_next_token(in, word, line_number);
                                            if(word == "<<__TEXT_END__>>")
                                                break;
                                            if(line_number - line_number_was)
                                            {
                                                rss_S.push_back(rss);
                                                for(line_diff = 0; line_diff < (line_number - line_number_was - 1); line_diff++)
                                                    rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                                rss.clear();
                                                line_number_was = line_number;
                                            }
                                            rss += LaserBoy_real_segment(font_frames,
                                                                         unicode_char_list,
                                                                         word,
                                                                         char_width,
                                                                         vari_font_gap,
                                                                         vari_font_space,
                                                                         super_diacritic_gap,
                                                                         bond_word,
                                                                         bond_line
                                                                        );
                                        } // if(word == "<<__TEXT_END__>>") || EOF
                                        if(line_number - line_number_was)
                                        {
                                            rss_S.push_back(rss);
                                            for(line_diff = 0; line_diff < (line_number - line_number_was); line_diff++)
                                                rss_S.push_back(LaserBoy_real_segment_set()); // empty lines
                                        }
                                        rss.clear();
                                        number_of_lines = (int)rss_S.size();
                                        if(number_of_lines > 0)
                                        {
                                            int                word_index,
                                                               number_of_words   = 0;
                                            double             words_width       = 0.0,
                                                               line_width        = 0.0,
                                                               widest_line       = 0.0,
                                                               word_gap          = 0.0,
                                                               window_height     = 0.0;
                                            LaserBoy_3D_double word_offset;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width     = 0.0;
                                                line_width      = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                line_width =   words_width
                                                             + (   (number_of_words - 1)
                                                                 * LASERBOY_MAX_SHORT
                                                                 * vari_font_space
                                                               );
                                                if(widest_line < line_width)
                                                    widest_line = line_width;
                                            } // end for(line_index = 0; line_index < rss_S.size(); line_index++)
                                            word_offset.x = 0.0;
                                            word_offset.y = 0.0;
                                            word_offset.z = 0.0;
                                            for(line_index = 0; line_index < number_of_lines; line_index++)
                                            {
                                                word_offset.x = 0.0;
                                                number_of_words = (int)rss_S.at(line_index).size();
                                                words_width = 0.0;
                                                for(word_index = 0; word_index < number_of_words; word_index++)
                                                    words_width += rss_S.at(line_index).at(word_index).width();
                                                if(number_of_words > 1) // more than one word.
                                                    word_gap = (widest_line - words_width) / (number_of_words - 1);
                                                else
                                                    word_gap = 0.0;
                                                if(number_of_words > 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                    word_offset.x += (each_word.width() + word_gap);
                                                    for(word_index = 1; word_index < number_of_words; word_index++)
                                                    {
                                                        each_word = rss_S.at(line_index).at(word_index);
                                                        each_word.move(word_offset);
                                                        if(bond_line)
                                                            each_word.front().black();
                                                        each_line += each_word;
                                                        word_offset.x += (each_word.width() + word_gap);
                                                    }
                                                }
                                                else if(number_of_words == 1)
                                                {
                                                    each_word = rss_S.at(line_index).at(0);
                                                    each_word.move(word_offset); // moves in Y
                                                    each_line += each_word;
                                                }
                                                if(each_line.size() && bond_line)
                                                {
                                                    each_line.convert_blank_to_black();
                                                    each_line.front().blank();
                                                }
                                                page += each_line;
                                                each_line.clear();
                                                word_offset.y -= (LASERBOY_MAX_SHORT * line_space);
                                            } // end for(size_t line_index = 0; line_index < rss_S.size(); line_index++)
                                            line_width    =  page.width();
                                            min.x         = -line_width / 2.0;
                                            max.x         =  line_width / 2.0;
                                            min.y         = -line_width;
                                            max.y         =  line_width;
                                            min.z         = -32767.0;
                                            max.z         =  32767.0;
                                            center        =  page.rectangular_center_of();
                                            center.y      =  page.segment_top().y + line_width;
                                            window_height = line_width * 2.0;
                                            last_i = (u_int)( (   (page.height() / LASERBOY_MAX_SHORT) // whole page
                                                                + (window_height / LASERBOY_MAX_SHORT) // the window
                                                              ) // in squares
                                                              / move_per_frame
                                                            );

                                            the_move = LASERBOY_MAX_SHORT * move_per_frame;
                                            rss.clear();
                                            page.strip_color_rgb(color);
                                            page.palette_index = LASERBOY_TRUE_COLOR;
                                            page.add_lit_span_vertices(512);
                                            for(i = 0; i < 256; i++)
                                                palette.push_back(color.blend(LaserBoy_color(0, 0, 0), i / 255.0));
                                            palette.first = 0;
                                            palette.last  = 254;
                                            palette.white = 0;
                                            palette.black = 255;
                                            palette.name = GUID8char();
                                            *p_space += palette;
                                            p_space->p_GUI->display_state("rendering text variable scroll starwars.");
                                            for(i = 0; i <= last_i; i++)
                                            {
                                                p_space->p_GUI->display_progress(last_i - i);
                                                each_line = page;
                                                each_line.clip_around_coordinate(center, max, min, 65536.0);
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - (each_line.at(_i).y + max.y)) / window_height;
                                                    if(each_line.at(_i).is_color())
                                                    {
                                                        each_line.at(_i).r = color.r * words_width;
                                                        each_line.at(_i).g = color.g * words_width;
                                                        each_line.at(_i).b = color.b * words_width;
                                                    }
                                                }
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - each_line.at(_i).y) / window_height;
                                                    words_width *= 0.5;
                                                    each_line.at(_i) = LaserBoy_3D_double( each_line.at(_i).x,
                                                                                           each_line.at(_i).y * words_width,
                                                                                          -each_line.at(_i).y
                                                                                         );
                                                }
                                                for(size_t _i = 0; _i < each_line.size(); _i++)
                                                {
                                                    words_width = (window_height - (each_line.at(_i).y + max.y)) / window_height;
                                                    words_width *= 2.0;
                                                    each_line.at(_i) = LaserBoy_3D_double( each_line.at(_i).x * words_width,
                                                                                           each_line.at(_i).y,
                                                                                          -each_line.at(_i).y
                                                                                         );
                                                }
                                                each_line.reduce_lit_vectors();
                                                each_line.best_match_palette(p_space->number_of_palettes() - 1);
                                                rss.push_back(each_line);
                                                center.y -= the_move;
                                            }
                                            rss.normalize(false);
                                            frames = rss;
                                            frames.normalize(to_space);
                                            *this += frames;
                                            text_frames_rendered += frames.size();
                                            frames.clear();
                                            text_success = true;
                                        } // end if(rss_S.size())
                                        rss_S.clear();
                                    } // end else if(word == "starwars")
                                    else // end if(word == "left")
                                    {
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " :  text variable scroll "
                                                  << word
                                                  << " is not a recognized form. (ERROR)"
                                                  << ENDL;
                                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            } // end else if(word == "scroll")
                        }
                    } // end else if(word == "variable")
                    //--------------------------------------------------------
                    else // "text (word)"
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " is not a recognized text form. (ERROR)"
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                    //--------------------------------------------------------
                    //--------------------------------------------------------
                    //--------------------------------------------------------
                    if(!text_success)
                    {
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " : "
                                  << word
                                  << " syntax. (ERROR) "
                                  << report
                                  << ENDL;
                        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number)) (after finding text)
                //------------------------------------------------------------
                else
                {
                    // text [nothing]
                    error_log << file_name
                              << " error on line: "
                              << line_number
                              << " \'text\' not followd by any form. (ERROR)"
                              << ENDL;
                    frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                }
                //------------------------------------------------------------
                p_space->allow_lit_black = allow_lit_black_was;
                p_space->no_black_match  = no_black_match_was;
                //------------------------------------------------------------
            } // end else if(word == "text")
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            else
            {
                // bad_word!
                error_log << file_name
                          << " error on line: "
                          << line_number
                          << " \'"
                          << word
                          << "\' is not a LaserBoy text format token. (ERROR)"
                          << ENDL;
                frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
            }
        } // end if(get_next_word(in, word, line_number))
        p_space->p_GUI->display_progress(++section_count);
        p_space->p_GUI->display_txt_line_number(line_number);
    } // end while(in.good())
    if(A != NULL && !A->been_used)
    {
        error_log << file_name
                  << " unused frame_set in first register at end-of-file."
                  << ENDL;
        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
        p_space->display_error("unused frame_set in first register at end of file");
        p_space->p_GUI->wait_4_Esc();
    }
    if(B != NULL && !B->been_used)
    {
        error_log << file_name
                  << " unused frame_set in second register at end-of-file."
                  << ENDL;
        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
        p_space->display_error("unused frame_set in second register at end of file");
        p_space->p_GUI->wait_4_Esc();
    }
    if(math_list.size())
    {
        int unused = 0;
        for(size_t i = 0; i < math_list.size(); i++)
            if(math_list[i]->been_used == false)
                unused++;
        if(unused)
        {
            error_log << file_name
                      << " "
                      << unused
                      << " unused frame_sets in list at end-of-file."
                      << ENDL;
            frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
            p_space->display_error(to_string(unused) + " unused frame_sets in list at end of file");
            p_space->p_GUI->wait_4_Esc();
        }
    }
    if(txt_frames_saved)
    {
        p_space->display_message(to_string(txt_frames_saved) + " frames saved as txt");
        p_space->p_GUI->wait_4_any_key();
    }
    if(dxf_frames_saved)
    {
        p_space->display_message(to_string(dxf_frames_saved) + " frames saved as dxf");
        p_space->p_GUI->wait_4_any_key();
    }
    if(bmp_frames_saved)
    {
        p_space->display_message(to_string(bmp_frames_saved) + " frames saved as bmp");
        p_space->p_GUI->wait_4_any_key();
    }
    if(math_frames_rendered)
    {
        p_space->display_message(to_string(math_frames_rendered) + " math frames rendered");
        p_space->p_GUI->wait_4_any_key();
    }
    if(text_frames_rendered)
    {
        p_space->display_message(to_string(text_frames_rendered) + " text frames rendered");
        p_space->p_GUI->wait_4_any_key();
    }
    if(    section_count        == 0
        && math_frames_rendered == 0
        && text_frames_rendered == 0
      )
    {
        error_log << file_name
                  << " contains nothing to render!"
                  << ENDL;
        frame_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
    }
    //------------------------------------------------------------------------
    if(coordinate_real_frames_index.size())
    {
        coordinate_real_segments.normalize();
        for(i = 0; i < coordinate_real_frames_index.size(); i++)
            at(coordinate_real_frames_index[i]) = coordinate_real_segments[i];
    }
    //------------------------------------------------------------------------
    p_space->save_txt_with_color = save_txt_with_color_was;
    p_space->save_txt_color_hex  = save_txt_color_hex_was;
    in.close();
    error_log.close();
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_ctn_file(const string& file,
                                                      long int&     bytes_skipped
                                                     )
{
    std::ifstream in(file.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        from_ifstream_ctn(in, bytes_skipped);
        in.close();
        if(size() == 0) // create an empty frame_set
        {
            from_nothing();
            frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
        }
    }
    else // file failed to open
    {
        from_nothing();
        frame_set_error |= LASERBOY_FILE_OPEN_FAILED;
    }
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_LaserBoy_wave(LaserBoy_wave_header header,
                                                                   std::fstream&        in,
                                                                   bool                 append
                                                                  )
{
    u_int                         i,
                                  j,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame;
    LaserBoy_vertex               vertex;
    LaserBoy_wave_sample_pointer  roll_over,
                                 *samples_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening formatted wave");
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    span  = (offset_max - offset_min) + 2; // size of samples_window
    last  = span - 1                     ; // last index of samples_window
    red   = -header.offset[2] + 1        ; // index of red
    green = -header.offset[3] + 1        ; // index of green
    blue  = -header.offset[4] + 1        ; // index of blue
    //------------------------------------------------------------------------
    samples_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        samples_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame_set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    for(j = 0; j < last - 1; j++)
    {
        roll_over = samples_window[0];
        for(i = 0; i < last; i++)
            samples_window[i] = samples_window[i + 1];
        samples_window[last] = roll_over;
        if(!samples_window[last]->from_fstream_wave(in)) // if EOF
        {
            from_nothing();
            frame_set_error |= LASERBOY_NO_FRAMES_FOUND;
            return frame_set_error;
        }
        if((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) == 0)
            samples_window[last]->negate();
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = samples_window[0];
        for(i = 0; i < last; i++)
            samples_window[i] = samples_window[i + 1];
        samples_window[last] = roll_over;

        if(!samples_window[last]->from_fstream_wave(in)) // if EOF
            break;

        if((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) == 0)
            samples_window[last]->negate();
        //----------------------------------------------------------------
        vertex.x = samples_window[1]->channel[0];
        vertex.y = samples_window[1]->channel[1];
        if(header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
            vertex.z = samples_window[1]->channel[5];
        else
            vertex.z = 0;
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.r = rescale_to_index(header.color_rescale_r, samples_window[red]->channel[2]);
        else
        {
            if(samples_window[red]->channel[2]) // != 0
                vertex.r = (u_char)abs(samples_window[red]->channel[2] >> 7);
            else
                vertex.r = 0;
        }
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
            vertex.g = rescale_to_index(header.color_rescale_g, samples_window[green]->channel[3]);
        else
        {
            if(samples_window[green]->channel[3])
                vertex.g = (u_char)abs(samples_window[green]->channel[3] >> 7);
            else
                vertex.g = 0;
        }
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
            vertex.b = rescale_to_index(header.color_rescale_b, samples_window[blue]->channel[4]);
        else
        {
            if(samples_window[blue]->channel[4])
                vertex.b = (u_char)abs(samples_window[blue]->channel[4] >> 7);
            else
                vertex.b = 0;
        }
        //----------------------------------------------------------------
        vertex.k = 0;
        vertex.c = 0;
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        frame.push_back(vertex);
        //----------------------------------------------------------------
        if(samples_window[red]->channel[2] & 0x0001) // end of frame
        {
            frame.is_unique = true;
            if(    (header.LaserBoy_wave_mode & LASERBOY_WAVE_UNIQUE_FRAME)  // if we are looking for it
                && (!(samples_window[green]->channel[3] & 0x0001)) // and we do not find it
              )
                frame.is_unique = false;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
        //----------------------------------------------------------------
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 2)
    {
        frame.is_unique = true;
        if(total_frames)
          frame.is_unique = (!frame.same_as(back()));
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    for(i = 0; i < span; i++)
        delete samples_window[i];
    delete samples_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_unframed_wave(LaserBoy_wave_header header,
                                                                   std::fstream&        in,
                                                                   bool                 append
                                                                  )
{
    u_int                         i,
                                  vertices = 0,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame;
    LaserBoy_vertex               vertex        ;
    LaserBoy_wave_sample_pointer  roll_over    ;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening unframed wave");
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    span  = (offset_max - offset_min) + 2; // size of sample_window
    last  = span - 1                     ; // last index of sample_window
    red   = -header.offset[2] + 1        ; // index of red
    green = -header.offset[3] + 1        ; // index of green
    blue  = -header.offset[4] + 1        ; // index of blue
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame_set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;

        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;

        if((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) == 0)
            sample_window[last]->negate();
        //----------------------------------------------------------------
        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        if(header.signal_id[5] == LASERBOY_SIGNAL_Z_POSITION)
            vertex.z = sample_window[1]->channel[5];
        else
            vertex.z = 0;
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.r = rescale_to_index(header.color_rescale_r, sample_window[red]->channel[2]);
        else
        {
            if(sample_window[red  ]->channel[2] > 0)
                vertex.r = (u_char)abs(sample_window[red  ]->channel[2] >> 7);
            else
                vertex.r = 0;
        }
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.g = rescale_to_index(header.color_rescale_g, sample_window[green]->channel[3]);
        else
        {
            if(sample_window[green]->channel[3] > 0)
                vertex.g = (u_char)abs(sample_window[green]->channel[3] >> 7);
            else
                vertex.g = 0;
        }
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            vertex.b = rescale_to_index(header.color_rescale_b, sample_window[blue]->channel[4]);
        else
        {
            if(sample_window[blue ]->channel[4] > 0)
                vertex.b = (u_char)abs(sample_window[blue ]->channel[4] >> 7);
            else
                vertex.b = 0;
        }
        //----------------------------------------------------------------
        vertex.k = 0;
        vertex.c = 0;
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //----------------------------------------------------------------
        frame.push_back(vertex);
        vertices++;
        if(!(vertices % p_space->unframed_wave_stride))
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    delete sample_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_qm_wave(LaserBoy_wave_header header,
                                                             std::fstream&        in,
                                                             bool                 global_polarity,
                                                             bool                 append
                                                            )
{
    u_int            i,
                     color_sum,
                     static_sample_count = 0,
                     verticies = 0,
                     span,
                     last;
    int              red,
                     green,
                     blue,
                     offset_max = p_space->wave_offsets[0],
                     offset_min,
                     total_frames = 0;
    double           red_gain   = 1.0 / 128.0,
                     green_gain = 1.0 / 128.0,
                     blue_gain  = 1.0 / 128.0;
    LaserBoy_frame   frame;
    LaserBoy_vertex  vertex,
                     vertex_1,
                     vertex_2,
                     vertex_3,
                     vertex_4;

    LaserBoy_wave_sample_pointer  roll_over    ;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening blanked wave");
    //------------------------------------------------------------------------
    for(i = 1; i < 6; i++)
        if(p_space->wave_offsets[i] > offset_max)
            offset_max = p_space->wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        p_space->wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = p_space->wave_offsets[i]; // use current system offsets
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    //------------------------------------------------------------------------
    span  = (offset_max - offset_min) + 2; // at least 2 samples!
    last  = span - 1;
    red   = -header.offset[2] + 1;
    green = -header.offset[3] + 1;
    blue  = -header.offset[4] + 1;
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame_set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;
        verticies++;
        if(global_polarity)
            sample_window[last]->negate();
        //----------------------------------------------------------------
        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        vertex.z = 0;
        color_sum =   sample_window[red  ]->channel[2]
                    + sample_window[green]->channel[3]
                    + sample_window[blue ]->channel[4];
        if(color_sum < 2000) // very close to black!
        {
            vertex.r = 0;
            vertex.g = 0;
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
            vertex.c = 0; // 24 bit

            if(    (vertex | vertex_2) <= p_space->insignificant_distance
                && (vertex | vertex_4) <= p_space->insignificant_distance
              )
                static_sample_count++;

            if(static_sample_count > 80) // we found a frame boundary
            {
                frame.is_unique = true;
                if((int)frame.size() > 1)
                {
                    frame.at(1).blank();
                    push_back(frame);
                    frame.clear();
                }
                verticies = 0;
                static_sample_count = 0;
                while(true)
                {
                    roll_over = sample_window[0];
                    for(i = 0; i < last; i++)
                        sample_window[i] = sample_window[i + 1];
                    sample_window[last] = roll_over;
                    if(!sample_window[last]->from_fstream_wave(in)) // if EOF
                        break;
                    verticies++;
                    if(global_polarity)
                        sample_window[last]->negate();
                    color_sum =   sample_window[red  ]->channel[2]
                                + sample_window[green]->channel[3]
                                + sample_window[blue ]->channel[4];
                    vertex.x = sample_window[1]->channel[0];
                    vertex.y = sample_window[1]->channel[1];
                    vertex.z = 0;
                    if(color_sum > 2000) // we have color!
                        break;
                    frame.push_back(vertex);
                }
            }
            vertex_4 = vertex_3;
            vertex_3 = vertex_2;
            vertex_2 = vertex_1;
            vertex_1 = vertex;
        }
        else
        {
            vertex.r = (u_char)((sample_window[red  ]->channel[2] > 0) ? (sample_window[red  ]->channel[2] *   red_gain) : (0));
            vertex.g = (u_char)((sample_window[green]->channel[3] > 0) ? (sample_window[green]->channel[3] * green_gain) : (0));
            vertex.b = (u_char)((sample_window[blue ]->channel[4] > 0) ? (sample_window[blue ]->channel[4] *  blue_gain) : (0));
            vertex.k = 0;
            vertex.c = 0;
            static_sample_count = 0;
        }
        //----------------------------------------------------------------
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //----------------------------------------------------------------
        frame.push_back(vertex);
        if(verticies == p_space->unframed_wave_stride)
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            verticies = 0;
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    //------------------------------------------------------------------------
    delete sample_window;
    return frame_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::from_fstream_unformatted_wave(LaserBoy_wave_header header,
                                                                      std::fstream&        in,
                                                                      bool                 global_polarity,
                                                                      bool                 append
                                                                     )
{
    u_int                         i,
                                  vertices = 0,
                                  span,
                                  last;
    int                           red,
                                  green,
                                  blue,
                                  offset_max,
                                  offset_min,
                                  total_frames = 0;
    LaserBoy_frame                frame;
    LaserBoy_vertex               vertex;
    LaserBoy_wave_sample_pointer  roll_over;
    LaserBoy_wave_sample_pointer *sample_window;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening unformatted wave");
    //------------------------------------------------------------------------
    offset_max = p_space->wave_offsets[0];
    for(i = 1; i < 6; i++)
        if(p_space->wave_offsets[i] > offset_max)
            offset_max = p_space->wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        p_space->wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = p_space->wave_offsets[i]; // use current system offsets
    //------------------------------------------------------------------------
    offset_min = header.offset[0];
    offset_max = header.offset[0];
    for(i = 1; i < 6; i++)
    {
        if(header.offset[i] > offset_max)
            offset_max = header.offset[i];
        if(header.offset[i] < offset_min)
            offset_min = header.offset[i];
    }
    //------------------------------------------------------------------------
    span  = (offset_max - offset_min) + 2; // at least 2 samples!
    last  = span - 1;
    red   = -header.offset[2] + 1;
    green = -header.offset[3] + 1;
    blue  = -header.offset[4] + 1;
    //------------------------------------------------------------------------
    sample_window = new LaserBoy_wave_sample_pointer[span];
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    frame.format        = LASERBOY_2D_FRAME;
    frame.palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(!append) // to the current frame_set
    {
        clear();
        num_2D_frames = 0;
        num_3D_frames = 0;
    }
    //------------------------------------------------------------------------
    while(true)
    {
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;

        if(!sample_window[last]->from_fstream_wave(in)) // if EOF
            break;

        vertices++;

        if(global_polarity)
            sample_window[last]->negate();

        vertex.x = sample_window[1]->channel[0];
        vertex.y = sample_window[1]->channel[1];
        vertex.z = 0;
        switch(header.num_channels)
        {
            case 2:
                vertex.r =
                vertex.g =
                vertex.b = 255;
                break;
            //----------------------------------------------------------------
            case 3:
            case 4:
                vertex.r =
                vertex.g =
                vertex.b = (u_char) abs(sample_window[red  ]->channel[2] / 128);
                break;
            //----------------------------------------------------------------
            case 5:
            case 6:
            case 8:
                if(p_space->z_from_unformatted)
                {
                    vertex.z = sample_window[1]->channel[5];
                    header.signal_id[5] = LASERBOY_SIGNAL_Z_POSITION;
                }
                vertex.r = (u_char) abs(sample_window[red  ]->channel[2] / 128);
                vertex.g = (u_char) abs(sample_window[green]->channel[3] / 128);
                vertex.b = (u_char) abs(sample_window[blue ]->channel[4] / 128);
                break;
            //----------------------------------------------------------------
        }
        vertex.k = 0;
        vertex.c = 0;
        //----------------------------------------------------------------
        if((vertex.r + vertex.g + vertex.b) <= (int)p_space->black_level)
        {
            vertex.r =
            vertex.g =
            vertex.b = 0;
            vertex.k = LASERBOY_BLANKING_BIT;
        }
        //----------------------------------------------------------------
        frame.push_back(vertex);
        if(!(vertices % p_space->unframed_wave_stride))
        {
            frame.is_unique = true;
            push_back(frame);
            frame.clear();
            total_frames++;
            p_space->p_GUI->display_progress(total_frames);
        }
    } // end while(true)
    //------------------------------------------------------------------------
    if(frame.size() > 1)
    {
        frame.is_unique = true;
        push_back(frame);
        total_frames++;
        p_space->p_GUI->display_progress(total_frames);
    }
    //------------------------------------------------------------------------
    frame_set_error = LASERBOY_OK;
    return frame_set_error;
}

//############################################################################
bool LaserBoy_frame_set::is_ild_busted()
{
    p_space->p_GUI->display_state("checking vertex counts");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(at(i).size() > LASERBOY_MAX_USHORT)
            return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_frame_set::is_2D()
{
    p_space->p_GUI->display_state("checking for 2D");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(!at(i).is_2D())
            return false;
    }
    return true;
}

//############################################################################
void LaserBoy_frame_set::flatten_z()
{
    p_space->p_GUI->display_state("flattening Z");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).flatten_z();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::flat_to_2D()
{
    p_space->p_GUI->display_state("checking for flat frames");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).flat_to_2D();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::z_order_vertices(unsigned short span)
{
    p_space->p_GUI->display_state("Z ordering");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).z_order_vertices(span);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::unblank_all_vertices()
{
    p_space->p_GUI->display_state("unblanking all");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).unblank_all_vertices();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::convert_black_to_blank()
{
    p_space->p_GUI->display_state("converting black to blank");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).convert_black_to_blank();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::convert_black_to_color()
{
    p_space->p_GUI->display_state("converting black to color");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).convert_black_to_color();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::convert_blank_to_black()
{
    p_space->p_GUI->display_state("converting blank to black");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).convert_blank_to_black();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::impose_black_level()
{
    p_space->p_GUI->display_state("imposing black level");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).impose_black_level();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::reduce_blank_vectors()
{
    p_space->p_GUI->display_state("reducing blank vectors");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).reduce_blank_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::remove_dots()
{
    p_space->p_GUI->display_state("removing dots");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).remove_dots();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::enhance_dots()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("enhancing dots");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).enhance_dots();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::remove_dwell_vertices()
{
    p_space->p_GUI->display_state("removing dwell");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).remove_dwell_vertices();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::remove_short_vectors()
{
    p_space->p_GUI->display_state("removing short vectors");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).remove_short_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::reduce_lit_vectors()
{
    p_space->p_GUI->display_state("reducing lit vectors");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).reduce_lit_vectors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::minimize()
{
    p_space->p_GUI->display_state("minimizing frames");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).minimize(i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_dots()
{
    p_space->p_GUI->display_state("converting to dots");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).to_dots();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::impose_bit_resolution()
{
    p_space->p_GUI->display_state("bit reducing");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).impose_bit_resolution();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::explode_current_frame()
{
    u_int               i;
    LaserBoy_frame_set  results,
                        exploded = current_frame().explode_segments();

    for(i = 0; i < frame_index; i++)
        results += at(i);

    results += exploded;

    for(i = frame_index + 1; i < size(); i++)
        results += at(i);
    *this = results;
    return;
}

//############################################################################
void LaserBoy_frame_set::explode_all_frames()
{
    u_int               i;
    LaserBoy_frame_set  results,
                        exploded;
    p_space->p_GUI->display_state("exploding frames");
    for(i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        results +=  at(i).explode_segments();
    }
    *this = results;
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::collapse_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame  collapsed;
        int             first_selected_frame_index = -1;
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_selected)
            {
                if(first_selected_frame_index == -1)
                    first_selected_frame_index = i;
                collapsed += at(i);
            }
        }
        if(first_selected_frame_index != -1)
        {
            frame(first_selected_frame_index) = collapsed;
            frame(first_selected_frame_index).is_selected = false;
            delete_selected_frames();
            frame_index = first_selected_frame_index;
        }
    }
    return LASERBOY_OK;
}

//############################################################################
void LaserBoy_frame_set::delete_every_nth_frame(unsigned short n)
{
    LaserBoy_frame_set results;
    p_space->p_GUI->display_state("omitting frames");
    if(n > 1 && n < size())
    {
        for(size_t i = 0; i < size(); i++)
        {
            p_space->p_GUI->display_progress(size() - i);
            if(i % n)
                results += at(i);
        }
        *this = results;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::keep_every_nth_frame(unsigned short n)
{
    LaserBoy_frame_set results;
    p_space->p_GUI->display_state("omitting frames");
    if(n > 1 && n < size())
    {
        for(size_t i = 0; i < size(); i++)
        {
            p_space->p_GUI->display_progress(size() - i);
            if(!(i % n))
                results += at(i);
        }
        *this = results;
        normalize_cursors();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::fracture_segments()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("fracturing segments");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).fracture_segments();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::omit_equivalent_vectors()
{
    p_space->p_GUI->display_state("blanking equal vectors");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).omit_equivalent_vectors();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::bond_segments()
{
    p_space->p_GUI->display_state("bonding segments in frame");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).bond_segments();
    }
    return LASERBOY_OK;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::reorder_segments()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    LaserBoy_frame origin;
    origin.push_back(LaserBoy_vertex());
    origin.push_back(LaserBoy_vertex());
    stat |= front().reorder_segments(origin);
    p_space->p_GUI->display_state("reordering segments in frame");
    for(size_t i = 1; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).reorder_segments(at(i - 1));
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::randomize_segments()
{
    p_space->p_GUI->display_state("randomizing segments in frame");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).randomize_segments();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::conglomerate_lit_segments()
{
    p_space->p_GUI->display_state("conglomerating segments in frame");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).conglomerate_lit_segments();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_dwell()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding dwell");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).add_dwell();
    }
    return stat;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_lit_span_vertices()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding lit span");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).add_lit_span_vertices();
    }
    return stat;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::add_blank_span_vertices()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    p_space->p_GUI->display_state("adding blank span");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        stat |= at(i).add_blank_span_vertices();
    }
    return stat;
}

//############################################################################
void LaserBoy_frame_set::delete_redundant_frames()
{
    LaserBoy_frame_set reduced;
    p_space->p_GUI->display_state("omitting frames");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(at(i).is_unique)
            reduced.push_back(at(i));
    }
    *this = reduced;
    normalize_cursors();
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::optimize()
{
    LaserBoy_Error_Code stat = LASERBOY_OK;
    if(size())
    {
        size_t           i;
        LaserBoy_vertex  origin;
        LaserBoy_frame   frame;
        //----------------------------------------------------------------
        p_space->p_GUI->display_state("optimizing frame_set");
        //----------------------------------------------------------------
        front().minimize(0); // at(0).
        stat |= front().optimize(origin);
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
                    at(i    ).minimize(i);
                    at(i - 1).add_coda(at(i    ).front().as_3D_short());
            stat |= at(i    ).optimize(at(i - 1).back().as_3D_short());
            p_space->p_GUI->display_progress(size() - i);
        }
        back().add_coda(origin);
        back().coda += LaserBoy_segment(back().coda.back(), origin);
    } // end if(size())
    return stat;
}

//############################################################################
void LaserBoy_frame_set::sync_rgb_and_palette()
{
    for(size_t i = 0; i < size(); i++)
        at(i).sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_frame_set::index_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette indexing");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).index_palette(p_space->recolor_index_multiple, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::index_segments_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette indexing");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).index_segments_palette(p_space->recolor_index_multiple, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning vectors");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_x_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning X");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_x_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_y_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning Y");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_y_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_z_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning Z");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_z_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_radial_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning radial");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_radial_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_axial_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning radial");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_axial_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_segments_palette()
{
    static int color_offset = 0;
    //-----------------------------------------------------------------------
    u_int span  =   p_space->palette_picker(current_frame().palette_index).last
                  - p_space->palette_picker(current_frame().palette_index).first
                  + 1;
    //-----------------------------------------------------------------------
    p_space->p_GUI->display_state("palette spanning radial");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).span_segments_palette(p_space->recolor_span_factor, color_offset);
        color_offset += p_space->color_frame_shift;
        color_offset %= span;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::random_color_vectors_palette()
{
    p_space->p_GUI->display_state("random vector palette");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).random_color_vectors_palette(p_space->recolor_index_multiple);
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::random_color_segments_palette()
{
    p_space->p_GUI->display_state("random vector palette");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).palette_index = p_space->target_palette_index;
        at(i).random_color_segments_palette();
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::index_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue indexing");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).index_hues(p_space->recolor_index_multiple, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::index_segments_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue indexing segments");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).index_segments_hues(p_space->recolor_index_multiple, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_x_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning X");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_x_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_y_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning Y");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_y_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_z_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning Z");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_z_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_radial_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning radial");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_radial_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_axial_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning axial");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_axial_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::span_segments_hues()
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning segments");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_segments_hues(p_space->recolor_span_factor, hues_offset);
        hues_offset += p_space->color_frame_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::random_color_vectors_hues()
{
    p_space->p_GUI->display_state("random vector hues");
    for(size_t i = 0; i < size(); i++)
    {
        if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
            at(i).palette_index = current_frame().palette_index;
        else
            at(i).palette_index = p_space->target_palette_index;
        at(i).random_color_vectors_hues(p_space->recolor_index_multiple);
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::random_color_segments_hues()
{
    p_space->p_GUI->display_state("random vector hues");
    for(size_t i = 0; i < size(); i++)
    {
        if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
            at(i).palette_index = current_frame().palette_index;
        else
            at(i).palette_index = p_space->target_palette_index;
        at(i).random_color_segments_hues();
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::quarter_turn(u_int plane, u_int turns)
{
    p_space->p_GUI->display_state("turning");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).quarter_turn(plane, turns);
    }
    return;
}

//############################################################################
double LaserBoy_frame_set::max_distance_from_origin() const
{
    double max_d = 0,
           temp;
    for(size_t i = 0; i < size(); i++)
    {
       temp = at(i).max_distance_from_origin();
       if(temp >= max_d)
           max_d = temp;
    }
    return max_d;
}

//############################################################################
double LaserBoy_frame_set::max_distance_from_origin_xy() const
{
    double max_d = 0,
           temp;
    for(size_t i = 0; i < size(); i++)
    {
       temp = at(i).max_distance_from_origin_xy();
       if(temp >= max_d)
           max_d = temp;
    }
    return max_d;
}

//############################################################################
double LaserBoy_frame_set::min_distance_from_origin() const
{
    double min_d = 80000,
           temp;
    for(size_t i = 0; i < size(); i++)
    {
       temp = at(i).min_distance_from_origin();
       if(temp == 0.0)
           return 0.0;
       if(temp < min_d)
           min_d = temp;
    }
    return min_d;
}

//############################################################################
double LaserBoy_frame_set::min_distance_from_origin_xy() const
{
    double min_d = 80000,
           temp;
    for(size_t i = 0; i < size(); i++)
    {
       temp = at(i).min_distance_from_origin_xy();
       if(temp == 0.0)
           return 0.0;
       if(temp < min_d)
           min_d = temp;
    }
    return min_d;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::apply_view()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("setting view");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).apply_view();
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::scale_around_origin(LaserBoy_3D_double factor)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("scaling");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).scale_around_origin(factor);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::scale_around_origin()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("scaling");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).scale_around_origin();
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::rotate_around_origin(LaserBoy_3D_double angle)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("rotating");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).rotate_around_origin(angle);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::rotate_around_origin()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("rotating");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).rotate_around_origin();
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::move(LaserBoy_3D_double offset)
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("moving");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).move(offset);
    }
    return out_of_bounds;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame_set::move()
{
    LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
    p_space->p_GUI->display_state("moving");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        out_of_bounds |= at(i).move();
    }
    return out_of_bounds;
}

//############################################################################
void LaserBoy_frame_set::toggle_frame_select_start()
{
    int i;
    if(frame_select_start == -1) // was unset
    {
        frame_select_start = frame_index; // set
        if(frame_select_end != -1)
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = true;
            at(frame_select_end).is_selected = true;
        }
    }
    else // was set
    {
        if(frame_select_end != -1) // was set
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = false;
            at(frame_select_end).is_selected = false;
        }
        frame_select_start = -1; // unset
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::toggle_frame_select_end()
{
    int i;
    if(frame_select_end == -1) // was unset
    {
        frame_select_end = frame_index; // set
        if(frame_select_start != -1)
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = true;
            at(frame_select_end).is_selected = true;
        }
    }
    else // was set
    {
        if(frame_select_start != -1) // was set
        {
            for(i = frame_select_start;
                i != frame_select_end;
                (frame_select_start <= frame_select_end) ? (i++) : (i--)
               )
                at(i).is_selected = false;
            at(frame_select_end).is_selected = false;
        }
        frame_select_end = -1; // unset
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::invert_frame_selections()
{
    for(size_t i = 0; i < size(); i++)
        at(i).is_selected = !at(i).is_selected;
    return;
}

//############################################################################
void LaserBoy_frame_set::delete_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame_set reduced;
        for(size_t i = 0; i < size(); i++)
        {
            if(!at(i).is_selected)
                reduced.push_back(at(i));
        }
        if(reduced.size() == 0)
            reduced.push_back(LaserBoy_frame());
        if(    frame_select_start != -1
            && frame_select_end   != -1
          )
        {
            frame_index = (frame_select_start - 1);
            frame_select_start = -1;
            frame_select_end   = -1;
        }
        if(frame_index > reduced.size())
            frame_index = reduced.size() - 1;
        *this = reduced;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::trim_to_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_frame_set reduced;
        reduced.reserve(size());
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_selected)
                reduced.push_back(at(i));
        }
        if(    frame_select_start != -1
            && frame_select_end   != -1
          )
        {
            frame_select_start = -1;
            frame_select_end   = -1;
        }
        frame_index = 0;
        *this = reduced;
        normalize_cursors();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::bit_reduce_to_palette()
{
    p_space->p_GUI->display_state("bit palette reducing");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).bit_reduce_to_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_reduce_to_palette()
{
    p_space->p_GUI->display_state("best palette reducing");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).best_reduce_to_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_reduce_all_to_palette()
{
    size_t            i,
                      j;
    vector<int>       used_palette_indices;
    LaserBoy_palette  super_palette;
    //------------------------------------------------------------------------
    if(frame_set_palette_index() == LASERBOY_TRUE_COLOR)
    {
        p_space->p_GUI->display_state("reducing to best palette");
        for(i = 0; i < size(); i++)
        {
            if(frame_picker(i).palette_index != LASERBOY_TRUE_COLOR)
            {
                if(used_palette_indices.size()) // counted any palette indexes?
                {
                    for(j = 0; j < used_palette_indices.size(); j++)
                        if(frame_picker(i).palette_index == used_palette_indices[j]) // alredy counted it
                            break;
                    if(j == used_palette_indices.size())
                    {
                        used_palette_indices.push_back(frame_picker(i).palette_index); // count it
                        super_palette += p_space->palette_picker(frame_picker(i).palette_index);
                    }
                }
                else
                {
                    used_palette_indices.push_back(frame_picker(i).palette_index); // count it
                    super_palette += p_space->palette_picker(frame_picker(i).palette_index);
                }
            }
            else
                super_palette += (frame_picker(i).as_color_table().reorder());
            p_space->p_GUI->display_progress(size() - i);
        }
        //--------------------------------------------------------------------
        super_palette.best_reduction(); // squeeze all that in <=256 colors
        super_palette.name = GUID8char();
        p_space->push_back_palette(super_palette);
        //--------------------------------------------------------------------
        best_match_palette(p_space->number_of_palettes() - 1);
        //--------------------------------------------------------------------
    } // end if(frame_set_palette_index() == LASERBOY_TRUE_COLOR)
    used_palette_indices.clear();
    return;
}

//############################################################################
void LaserBoy_frame_set::promote_to_true_color()
{
    p_space->p_GUI->display_state("promoting to 24-bit");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).promote_to_true_color();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_match_palette(u_int index)
{
    p_space->p_GUI->display_state("best matching palette");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).best_match_palette(index);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::best_match_target_palette()
{
    if(!p_space->allow_lit_black)
        convert_black_to_blank();
    p_space->p_GUI->display_state("best matching target");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).best_match_target_palette();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color()
{
    p_space->p_GUI->display_state("stripping color to white");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).strip_color();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color_or()
{
    p_space->p_GUI->display_state("stripping color by logical OR");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).strip_color_or();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::strip_color_avg()
{
    p_space->p_GUI->display_state("stripping color by averaging");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).strip_color_avg();
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_palette_by_index(u_int index)
{
    p_space->p_GUI->display_state("converting to palette by index");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).to_palette_by_index(index);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_target_palette_by_index()
{
    p_space->p_GUI->display_state("converting to target by index");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).to_target_palette_by_index();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::color_from_bmp(const string& file)
{
    char file_name[256];
    struct LaserBoy_bmp bmp = {0};
    strcpy(file_name, file.c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        color_from_bmp(&bmp);
        bmp_free(&bmp);
        return LASERBOY_OK;
    }
    return LASERBOY_FILE_OPEN_FAILED;
}

//############################################################################
void LaserBoy_frame_set::color_from_bmp(struct LaserBoy_bmp* bmp)
{
    p_space->p_GUI->display_state("bitmap coloring");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).color_from_bmp(bmp);
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame_set::subtract_bmp(const string& file)
{
    char file_name[256];
    struct LaserBoy_bmp bmp = {0};
    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        subtract_bmp(&bmp);
        bmp_free(&bmp);
        return LASERBOY_OK;
    }
    return LASERBOY_FILE_OPEN_FAILED;
}

//############################################################################
void LaserBoy_frame_set::subtract_bmp(struct LaserBoy_bmp* bmp)
{
    p_space->p_GUI->display_state("bitmap masking");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).subtract_bmp(bmp);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::flip(int plane)
{
    p_space->p_GUI->display_state("flipping frames");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).flip(plane);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::renumber()
{
    size_t i;
    int    local_palette_index = LASERBOY_ILDA_DEFAULT,
           added_palettes      = 0,
           frame_id            = 0;
    bool   in_true_color       = false;
    char   number[12];
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
    {
        if(at(i).size() > 1)
        {
            if(!p_space->save_ild_fmt_4_5)
            {
                if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                {
                    if(!in_true_color)
                    {
                        frame_id++;
                        added_palettes++; // add LASERBOY_REDUCED_332
                        local_palette_index = LASERBOY_REDUCED_332;
                        in_true_color = true;
                    }
                    frame_id++;
                    added_palettes++; // add true_color
                }
                else if(at(i).palette_index != local_palette_index)
                {
                    frame_id++;
                    added_palettes++;
                    local_palette_index = at(i).palette_index;
                    in_true_color = false;
                }
            }
            at(i).identity = frame_id;
            if(at(i).name.size() == 0 || at(i).name == "ESI.....")
            {
                sprintf(number, "%05d", frame_id);
                at(i).name     = "ESI" + string(number);
            }
            frame_id++;
        }
        else
            delete_frame(i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
        at(i).total = (u_short)(size() + added_palettes);
    if(frame_index >= size())
        frame_index = size() - 1;
    return;
}

//############################################################################
u_short LaserBoy_frame_set::max_dimension() const
{
    if(size())
    {
        u_short max = 0;
        for(size_t i = 0; i < size(); i++)
            if(at(i).max_dimension() >= max)
                max = at(i).max_dimension();
        return max;
    }
    else
        return 0;
}

//############################################################################
double LaserBoy_frame_set::relative_volume() const
{
    return (max_dimension() / 65536.0);
}

//############################################################################
double LaserBoy_frame_set::relative_volume_to_origin() const
{
    double max = 0;
    for(size_t i = 0; i < size(); i++)
        if(at(i).relative_volume_to_origin() > max)
            max = at(i).relative_volume_to_origin();
    if(max)
        return max;
    return 0.0;
}

//############################################################################
size_t LaserBoy_frame_set::closest_x_to_edge() const
{
    size_t index = 0;
    int    max   = 0;
    for(size_t i = 0; i < size(); i++)
    {
        if(abs(at(i).min_x()) > max)
        {
            max = abs(at(i).min_x());
            index = i;
        }
        if(abs(at(i).max_x()) > max)
        {
            max = abs(at(i).max_x());
            index = i;
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_frame_set::closest_y_to_edge() const
{
    size_t index = 0;
    int    max   = 0;
    for(size_t i = 0; i < size(); i++)
    {
        if(abs(at(i).min_y()) > max)
        {
            max = abs(at(i).min_y());
            index = i;
        }
        if(abs(at(i).max_y()) > max)
        {
            max = abs(at(i).max_y());
            index = i;
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_frame_set::closest_z_to_edge() const
{
    size_t index = 0;
    int    max   = 0;
    for(size_t i = 0; i < size(); i++)
    {
        if(abs(at(i).min_z()) > max)
        {
            max = abs(at(i).min_z());
            index = i;
        }
        if(abs(at(i).max_z()) > max)
        {
            max = abs(at(i).max_z());
            index = i;
        }
    }
    return index;
}

//############################################################################
int LaserBoy_frame_set::frame_set_palette_index()
{
    if(size())
    {
        p_space->sync_rgb_and_palette();
        int local_palette_index = at(0).palette_index;
        for(int i = 1; i < (int)size(); i++)
            if(local_palette_index != at(i).palette_index)
               return LASERBOY_TRUE_COLOR;
        return local_palette_index;
    }
    return LASERBOY_ILDA_DEFAULT;
}

//############################################################################
int LaserBoy_frame_set::selected_frames_palette_index()
{
    if(number_of_selected_frames())
    {
        size_t             i;
        LaserBoy_frame_set selected = selected_frames();
        for(i = 0; i < selected.size(); i++)
                selected.at(i).sync_rgb_and_palette();
        int local_palette_index = selected.at(0).palette_index;
        for(i = 1; i < selected.size(); i++)
            if(local_palette_index != selected.at(i).palette_index)
               return LASERBOY_TRUE_COLOR;
        return local_palette_index;
    }
    return LASERBOY_ILDA_DEFAULT;
}

//############################################################################
bool LaserBoy_frame_set::true_color_frames()
{
    if(size())
    {
        p_space->sync_rgb_and_palette();
        for(int i = 0; i < (int)size(); i++)
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
               return true;
    }
    return false;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::reverse()
{
    LaserBoy_frame_set reversed;
    p_space->p_GUI->display_state("reversing frame order");
    for(size_t i = 1; i <= size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        reversed.push_back(at(size() - i));
    }
    clear();
    insert(begin(), reversed.begin(), reversed.end());
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::make_current_first()
{
    u_int              i;
    LaserBoy_frame_set new_order;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("reordering frames");
    for(i = frame_index; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        new_order += at(i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < frame_index; i++)
    {
        p_space->p_GUI->display_progress(frame_index - i);
        new_order += at(i);
    }
    //------------------------------------------------------------------------
    *this = new_order;
    frame_index = 0;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::swap_frames(int i, int j)
{
    if(    (i >= (int)size())
        || (i < 0)
        || (j >= (int)size())
        || (j < 0)
      )
        return *this;
    //------------------------------------------------------------------------
    LaserBoy_frame frame = at(i);
    at(i) = at(j);
    at(j) = frame;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::reverse_selected()
{
    size_t             i,
                       j;
    LaserBoy_frame_set selected;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("reversing selected frame order");
    for(i = 0; i < size(); i++)
        if(at(i).is_selected)
            selected += at(i);
    //------------------------------------------------------------------------
    j = selected.size();
    //------------------------------------------------------------------------
    for(i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(at(i).is_selected)
            at(i) = selected[--j];
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::random_order()
{
    LaserBoy_frame_set random;
    vector<bool>       been_here(size());
    //------------------------------------------------------------------------
    int random_index = rand() % size();
    //------------------------------------------------------------------------
    for(size_t i = 0; i < size(); i++)
        been_here[i] = false;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("randomizing frame order");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        while(been_here[random_index])
        {
            random_index = rand() % size();
        }
        random.push_back(at(random_index));
        been_here[random_index] = true;
    }
    //------------------------------------------------------------------------
    clear();
    insert(begin(), random.begin(), random.end());
    been_here.clear();
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::frame_reverse()
{
    p_space->p_GUI->display_state("reversing frame vectors");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).reverse();
    }
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::center_x(bool individually)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(false);
    rss.center_x(individually);
    *this = rss;
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::center_y(bool individually)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(false);
    rss.center_y(individually);
    *this = rss;
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::center_z(bool individually)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(false);
    rss.center_z(individually);
    *this = rss;
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::center(bool individually)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(false);
    rss.center(individually);
    *this = rss;
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::normalize(double s)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(true);
    rss.normalize();
    for(size_t i = 0; i < size(); i++)
    {
        rss.at(i).remove_vertex(0);
        rss.at(i).remove_vertex(0); // remove added origin from every frame
    }
    *this = rss;
    scale_around_origin(s);
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::normalize_vectors(double s)
{
    if(s > 0.0)
    {
        LaserBoy_real_segment_set  rss;
        rss = to_real_segment_set(false);
        rss.normalize_vectors();
        *this = rss;
        scale_around_origin(s);
    }
    else
    {
        LaserBoy_real_segment_set  rss;
        rss = to_real_segment_set(false);
        rss.center();
        *this = rss;
    }
    return *this;
}

//############################################################################
LaserBoy_frame_set& LaserBoy_frame_set::normalize_vectors_with_origin(double s)
{
    LaserBoy_real_segment_set  rss;
    rss = to_real_segment_set(true);
    rss.normalize_vectors_with_origin();
    for(size_t i = 0; i < size(); i++)
    {
        rss.at(i).remove_vertex(0);
        rss.at(i).remove_vertex(0); // remove added origin from every frame
    }
    *this = rss;
    scale_around_origin(s);
    return *this;
}

//############################################################################
LaserBoy_real_segment_set LaserBoy_frame_set::to_real_segment_set(bool add_origin) const
{
    LaserBoy_real_segment     rs;
    LaserBoy_real_segment_set rss;
    rss.reserve(size());
    for(size_t i = 0; i < size(); i++)
    {
        rs = at(i).to_real_segment(add_origin);
        rss.push_back(rs);
    }
    rss.frame_index        = frame_index;
    rss.frame_select_start = frame_select_start;
    rss.frame_select_end   = frame_select_end;
    return rss;
}

//############################################################################
bool LaserBoy_frame_set::save_as_ild(const string& file, LaserBoy_ild_header_count& counter)
{
    if(size())
    {
        std::ofstream out(file.c_str(), ios::out | ios::binary);
        if(out.is_open())
        {
            if(p_space->auto_minimize)
            {
                LaserBoy_frame_set copy(*this);
                copy.minimize();
                copy.to_ofstream_ild(out, counter);
            }
            else
                to_ofstream_ild(out, counter);
            out.close();
        }
        else
            return false;
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_frame_set::save_as_ctn(const string& file)
{
    std::ofstream out(file.c_str(), ios::out | ios::binary);
    if(out.is_open())
    {
        LaserBoy_frame_set copy(*this);
        copy.best_match_palette(LASERBOY_ILDA_DEFAULT);
        copy.renumber();
        copy.sync_rgb_and_palette();
        copy.to_ofstream_ctn(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
bool LaserBoy_frame_set::save_as_txt(const string& file)
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        if(p_space->auto_minimize)
        {
            LaserBoy_frame_set copy(*this);
            copy.minimize();
            copy.to_ofstream_txt(out);
        }
        else
            to_ofstream_txt(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
void LaserBoy_frame_set::to_ofstream_ild(std::ofstream& out, LaserBoy_ild_header_count& counter)
{
    if(size())
    {
        u_short              id_num              = 0;
        bool                 in_true_color       = false;
        int                  local_palette_index = LASERBOY_ILDA_DEFAULT;
        LaserBoy_ild_header  header;
        header.format   = LASERBOY_PALETTE;
        header.owner    = "LaserBoy";
        header.identity = id_num;
        //--------------------------------------------------------------------
//        p_space->sync_rgb_and_palette();
        //--------------------------------------------------------------------
        if(p_space->save_ild_fmt_4_5)
            header.total = size();
        else
            header.total = size() + number_of_ild_color_tables() + number_of_ild_palettes();
        //--------------------------------------------------------------------
        p_space->p_GUI->display_state("writing ild file");
        for(size_t i = 0; i < size(); i++)
        {
            p_space->p_GUI->display_progress(size() - i - 1);
            if(!p_space->save_ild_fmt_4_5)
            {
                if(    (at(i).palette_index == LASERBOY_TRUE_COLOR)
                    && (!in_true_color) // write the 332 palette as a fallback for true color.
                  )
                {
                    header.name         = p_space->palette_picker(LASERBOY_REDUCED_332).name;
                    header.quantity     = p_space->palette_picker(LASERBOY_REDUCED_332).number_of_colors();
                    header.identity     = id_num++;
                    header.to_ofstream_ild(out);
                    p_space->palette_picker(LASERBOY_REDUCED_332).to_ofstream_ild(out);
                    counter._2++;
                    local_palette_index = LASERBOY_REDUCED_332;
                    in_true_color = true;
                }
                else if(at(i).palette_index != local_palette_index)
                {
                    header.name         = p_space->palette_picker(at(i).palette_index).name;
                    header.quantity     = p_space->palette_picker(at(i).palette_index).number_of_colors();
                    header.identity     = id_num++;
                    if(header.quantity < 256)
                        header.quantity += 1;
                    header.to_ofstream_ild(out);
                    p_space->palette_picker(at(i).palette_index).to_ofstream_ild(out);
                    counter._2++;
                    local_palette_index = at(i).palette_index;
                    in_true_color = false;
                }
            } // end if(!p_space->save_ild_fmt_4_5)
            at(i).total = header.total;
            at(i).to_ofstream_ild(out, id_num, counter);
        } // end for(size_t i = 0; i < size(); i++)
        //--------------------------------------------------------------------
        if(p_space->save_ild_fmt_4_5)
            header.format = LASERBOY_3D_FRAME_RGB;
        else
            header.format = LASERBOY_3D_FRAME;
        header.name     = "ILDA_End";
        header.owner    = p_space->install_GUID;
        header.quantity = 0;
        header.identity = id_num;
        header.to_ofstream_ild(out); // last section header written to file
        counter._e++;
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_ofstream_ctn(std::ofstream& out)
{
    p_space->p_GUI->display_state("writing ctn file");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        at(i).to_ofstream_ctn(out);
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::to_ofstream_txt(std::ofstream& out)
{
    int  local_palette_index = LASERBOY_TRUE_COLOR;
    p_space->p_GUI->display_state("writing text file");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(!p_space->save_txt_with_color)
        {
            if(    at(i).palette_index != local_palette_index
                && at(i).palette_index != LASERBOY_TRUE_COLOR
              )
            {
                if(    p_space->save_txt_named_palettes
                    || at(i).palette_index >= LASERBOY_BUILT_IN_PALETTES
                  )
                    p_space->palette_picker(at(i).palette_index).to_ofstream_txt(out);
                else
                {
                    out << "palette named "
                        << p_space->palette_picker(at(i).palette_index).name
                        << ENDL
                        << ENDL;
                }
                local_palette_index = at(i).palette_index;
            }
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                at(i).to_ofstream_txt_color_table(out);
        }
        //----------------------------------------------------------------
        at(i).to_ofstream_txt(out, i);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame_set::save_as_wave(const string& file, bool optimized, bool timed)
{
    if(size() > 1)
    {
        std::fstream out(file.c_str(), ios::out | ios::binary);
        if(out.is_open())
        {
            to_fstream_wave(out, optimized, timed);
            out.close();
            return true;
        }
        else
            return false;
    }
    else // there is only one frame
        return front().save_as_wave(file, optimized, timed, false);
}

//############################################################################
void LaserBoy_frame_set::to_fstream_wave(std::fstream& out, bool optimized, bool timed)
{
    u_int                 i;
    LaserBoy_frame_set    copy(*this);
    LaserBoy_wave_header  header(p_space->sample_rate,
                                    LASERBOY_WAVE_POSITIVE
                                  | LASERBOY_WAVE_END_OF_FRAME
                                  | LASERBOY_WAVE_UNIQUE_FRAME
                                  | LASERBOY_WAVE_SIGNAL_MATRIX
                                  | LASERBOY_SIGNAL_BIT_RESOLUTION
                                  | LASERBOY_WAVE_NUM_FRAMES
                                  | p_space->color_rescales_flags(),
                                 p_space->channels_of_wav_out,
                                 LASERBOY_WAVE_VERSION
                                );
    //------------------------------------------------------------------------
    if(p_space->invert_wave_output)
        header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE;
    //------------------------------------------------------------------------
    header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION; // default values
    header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION;
    header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG;
    header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
    header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG;
    header.signal_id [5] = LASERBOY_SIGNAL_Z_POSITION;
    header.signal_id [6] = LASERBOY_SIGNAL_UNDEFINED;
    header.signal_id [7] = LASERBOY_SIGNAL_UNDEFINED;
    //------------------------------------------------------------------------
    header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED; // default values
    header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [2] = LASERBOY_LSB_END_OF_FRAME;
    header.LSB_tag   [3] = LASERBOY_LSB_UNIQUE_FRAME;
    header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
    header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;
    //------------------------------------------------------------------------
    header.resolution[0] = 16 - p_space->signal_bit_mask[0];
    header.resolution[1] = 16 - p_space->signal_bit_mask[1];
    header.resolution[2] = 16 - p_space->signal_bit_mask[2];
    header.resolution[3] = 16 - p_space->signal_bit_mask[3];
    header.resolution[4] = 16 - p_space->signal_bit_mask[4];
    header.resolution[5] = 16 - p_space->signal_bit_mask[5];
    header.resolution[6] = 16 - p_space->signal_bit_mask[6];
    header.resolution[7] = 16 - p_space->signal_bit_mask[7];
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        for(i = 0; i < 256; i++)
            header.color_rescale_r[i] = p_space->color_rescale_r[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        for(i = 0; i < 256; i++)
            header.color_rescale_g[i] = p_space->color_rescale_g[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        for(i = 0; i < 256; i++)
            header.color_rescale_b[i] = p_space->color_rescale_b[i];
    //------------------------------------------------------------------------
    if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
        for(i = 0; i < 256; i++)
            header.color_rescale_i[i] = p_space->color_rescale_i[i];
    //------------------------------------------------------------------------
    if(p_space->view_angle != 0.0)
        for(i = 0; i < copy.size(); i++)
            copy[i].rotate_around_origin();
    //------------------------------------------------------------------------
    if(is_2D() || p_space->auto_flatten_z)
    {
        header.signal_id [5] = p_space->signal_if_not_z;
        header.resolution[5] = 9;
        copy.flatten_z();
    }
    //------------------------------------------------------------------------
    if(optimized)
    {
        header.LaserBoy_wave_mode |= LASERBOY_WAVE_OPTIMIZED;
        header.parms.lit_dwell_overhang     =        p_space->lit_dwell_overhang;
        header.parms.lit_delta_max          = (float)p_space->lit_delta_max;
        header.parms.blank_delta_max        = (float)p_space->blank_delta_max;
        header.parms.max_dwell_millisec     = (float)p_space->max_dwell_millisec;
        header.parms.max_wag_microsec       = (float)p_space->max_wag_microsec;
        header.parms.insignificant_distance = (float)p_space->insignificant_distance;
        header.parms.insignificant_angle    = (float)p_space->insignificant_angle;
        header.parms.frames_per_second      =    (timed)
                                               ? ((float)p_space->frames_per_second)
                                               : (0);
        copy.optimize();
    }
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("writing wave file");
    //------------------------------------------------------------------------
    header.to_fstream_wave(out); // take up the space!
    //------------------------------------------------------------------------
    if(optimized)
    {
        bool              not_last_frame;
        int               frame_scan,
                          total_frame_scans = 1;
        u_int             chill_samples = (u_int)ceil(   p_space->sample_rate
                                                       * (   p_space->max_dwell_millisec
                                                           / 500000.0 // make it twice the max_dwell_millisec
                                                         )
                                                     );
        LaserBoy_vertex   origin;
        LaserBoy_segment  chill;
        if(chill_samples < 10)
            chill_samples = 10;
        for(i = 0; i < chill_samples; i++)
            chill.push_back(origin);
        chill.to_fstream_wave(out, header, false, false);
        //----------------------------------------------------------------
        for(i = 0; i < copy.size(); i++)
        {
            not_last_frame    = (i < (copy.size() - 1));
            total_frame_scans = 1;
            copy.at(i).intro.to_fstream_wave(out, header, false, false);
            //----------------------------------------------------------------
            if(timed)
            {
                total_frame_scans = (int)ceil(   (p_space->sample_rate / p_space->frames_per_second)
                                               / (copy.at(i).size() + copy.at(i).bridge.size())
                                             );
                if(total_frame_scans > 1)
                {
                    ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out,
                                                                   header,
                                                                   copy.at(i).is_wagged, // end_of_frame()
                                                                   copy.at(i).is_wagged // unique_frame()
                                                                  );
                    for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
                    {
                        copy.at(i).bridge.to_fstream_wave(out,
                                                          header,
                                                          true,
                                                          (!copy.at(i).is_wagged) && (frame_scan == 1) // unique_frame()
                                                         ); // frame scan 1 is unique
                        ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out,
                                                                       header,
                                                                       copy.at(i).is_wagged && (frame_scan < total_frame_scans - 1), // end_of_frame()
                                                                       false // unique_frame()
                                                                      );
                    } // end for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
                    copy.at(i).coda.to_fstream_wave(out,
                                                    header,
                                                    not_last_frame, // end_of_frame()
                                                    false           // unique_frame()
                                                   );
                } // end if(total_frame_scans > 1)
                else // total_frame_scans == 1
                {
                    ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out,
                                                                   header,
                                                                   false, // end_of_frame()
                                                                   false  // unique_frame()
                                                                  );
                    copy.at(i).coda.to_fstream_wave(out,
                                                    header,
                                                    not_last_frame,
                                                    not_last_frame
                                                   ); // only 1 frame scan is unique
                }
            } // end if(timed)
            else
            {
                ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out,
                                                               header,
                                                               false,
                                                               false
                                                              );
                copy.at(i).coda.to_fstream_wave(out,
                                                header,
                                                not_last_frame,
                                                not_last_frame
                                               ); // only 1 frame scan is unique
            }
            //----------------------------------------------------------------
            p_space->p_GUI->display_progress(copy.size() - i);
        } // end for(i = 0; i < copy.size(); i++)
        chill.to_fstream_wave(out,
                              header,
                              true,
                              (total_frame_scans == 1) && (!copy.back().is_wagged)
                             );
    } // end if(optimized)
    //------------------------------------------------------------------------
    else if(timed)
    {
        int  frame_scan,
             total_frame_scans;
        for(i = 0; i < copy.size(); i++)
        {
            total_frame_scans = (int)ceil(   (p_space->sample_rate / p_space->frames_per_second)
                                           / copy.at(i).size()
                                         );
            ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, true);
            for(frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
            {
                ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, false);
                p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
            }
            //----------------------------------------------------------------
            p_space->p_GUI->display_progress(copy.size() - i);
        } // end for(i = 0; i < copy.size(); i++)
    } // end else if(timed)
    //------------------------------------------------------------------------
    else // not optimized or timed!
        for(i = 0; i < copy.size(); i++)
        {
            ((LaserBoy_segment)copy.at(i)).to_fstream_wave(out, header, true, true);
            p_space->p_GUI->display_progress(copy.size() - i);
        }
    //------------------------------------------------------------------------
    header.to_fstream_wave(out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_frame_set::save_as_bmp_directory(const string& dir, const string& name) const
{
    char number[12];
#if defined __MINGW_LASERBOY__
    mkdir(dir.c_str());
#else
    mkdir(dir.c_str(), 0777);
#endif
    for(size_t i = 0; i < size(); i++)
    {
        sprintf(number, "%06d", (int)i);
        at(i).save_as_bmp(string(dir + '/' + name + '_' + number + ".bmp"));
    }
    return;
}

//############################################################################
void LaserBoy_frame_set::save_as_bmp_view(u_int frame_index, const string& file) const
{
        char   file_name[80];
        struct LaserBoy_bmp bmp_out = {0};
        p_space->p_GUI->display_state("rendering bmp file " + file);
        bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size, 24);
        bmp_clear(&bmp_out, bmp_out.make_rgb(p_space->bg_color.r,
                                             p_space->bg_color.g,
                                             p_space->bg_color.b
                                            )
                 );
        render_frame(frame_index, &bmp_out);
        sprintf(file_name, "%s", file.c_str());
        bmp_flip(&bmp_out);
        p_space->p_GUI->display_state("writing   bmp file " + file);
        bmp_save_as(&bmp_out, file_name);
        bmp_free(&bmp_out);
        return;
}

//############################################################################
void LaserBoy_frame_set::save_as_bmp_directory_view(const string& dir, const string& name) const
{
    char number[12];
#if defined __MINGW_LASERBOY__
    mkdir(dir.c_str());
#else
    mkdir(dir.c_str(), 0777);
#endif
    for(size_t i = 0; i < size(); i++)
    {
        sprintf(number, "%06d", (int)i);
        save_as_bmp_view(i, string(dir + '/' + name + '_' + number + ".bmp"));
    }
    return;
}

//############################################################################
bool LaserBoy_frame_set::save_as_dxf_directory(const string& dir, const string& name) const
{
    bool all_saved = true;
    char number[16];
#if defined __MINGW_LASERBOY__
    mkdir((dir + name).c_str());
#else
    mkdir((dir + name).c_str(), 0777);
#endif
    p_space->p_GUI->display_state("writing DXF file");
    for(size_t i = 0; i < size(); i++)
    {
        sprintf(number, "%06d", (int)i);
        all_saved &= at(i).save_as_dxf(string(dir + name + '/' + name + '_' + number + ".dxf"));
        p_space->p_GUI->display_progress(size() - i);
    }
    return all_saved;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
