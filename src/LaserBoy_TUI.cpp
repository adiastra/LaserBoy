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
// LaserBoy_TUI.cpp is part of LaserBoy.
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
void LaserBoy_TUI::overlay_palette()
{
    u_int  i,
           j,
           y_line       = 8 * font_size_factor,
           y_off        =   LASERBOY_PALETTE_MAX * font_size_factor
                          + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    if(palette_index != current_frame().palette_index)
        palette_index = current_frame().palette_index;
    //------------------------------------------------------------------------
    if(palette_index != LASERBOY_TRUE_COLOR) // 8 bit frame palette
    {
        if(selected_color_index > palette_picker(palette_index).number_of_colors())
            selected_color_index = 0;
        for(i = 0; i < palette_picker(palette_index).number_of_colors(); i++)
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off,
                                  i * font_size_factor + j,
                                  x_off + 50,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(palette_index)[i].r,
                                               palette_picker(palette_index)[i].g,
                                               palette_picker(palette_index)[i].b
                                              )
                                 );
    }
    //------------------------------------------------------------------------
    else // (palette_index == LASERBOY_TRUE_COLOR)
        for(i = 0; i < palette_picker(target_palette_index).number_of_colors(); i++) // target palette
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off + 70,
                                  i * font_size_factor + j,
                                  x_off + 120,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(target_palette_index)[i].r,
                                               palette_picker(target_palette_index)[i].g,
                                               palette_picker(target_palette_index)[i].b
                                              )
                                 );
    //------------------------------------------------------------------------
    if(show_color_cursor) // selected_color_index example line
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
            bmp_put_line_fast(&bmp,
                              x_off + 56,
                              0,
                              x_off + 56,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(palette_index)[selected_color_index].r,
                                           palette_picker(palette_index)[selected_color_index].g,
                                           palette_picker(palette_index)[selected_color_index].b
                                          )
                             );
        //----------------------------------------------------------------
        else //(palette_index == LASERBOY_TRUE_COLOR)
            bmp_put_line_fast(&bmp,
                              x_off + 63,
                              0,
                              x_off + 63,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(target_palette_index)[selected_color_index].r,
                                           palette_picker(target_palette_index)[selected_color_index].g,
                                           palette_picker(target_palette_index)[selected_color_index].b
                                          )
                             );
        //----------------------------------------------------------------
        for(j = 0; j < font_size_factor; j++) // selected color marker
            bmp_put_line_fast(&bmp,
                              x_off + 40,
                              selected_color_index * font_size_factor + j,
                              x_off + 80,
                              selected_color_index * font_size_factor + j,
                              white_text.as_uint()
                             );
    }
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR)
    {
        bmp_printf(   &bmp // current frame palette name
                    , x_off
                    , y_off
                    , bmp_bg_color
                    , white_text.as_uint()
                    , 0
                    , font_size_factor
                    , "-24-bit-"
                  );
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , 0
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "%s %-6d"
                    , palette_picker(target_palette_index).name.c_str()
                    , target_palette_index
                  );
    }
    else
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "%s %-6d"
                    , palette_picker(palette_index).name.c_str()
                    , palette_index
                  );
    //------------------------------------------------------------------------
    if(show_color_cursor)
    {
        if(palette_index == LASERBOY_TRUE_COLOR)
        {
            bmp_printf(   &bmp
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(0,0,255)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "            %3d"
                        , palette_picker(target_palette_index)[selected_color_index].b
                      );
            bmp_printf(   &bmp
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(0,255,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "        %3d"
                        , palette_picker(target_palette_index)[selected_color_index].g
                      );
            bmp_printf(   &bmp
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(255,0,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "    %3d"
                        , palette_picker(target_palette_index)[selected_color_index].r
                      );
            bmp_printf(   &bmp
                        , x_off + 130
                        , y_line * 17
                        , white_text.as_uint()
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "%3d"
                        , selected_color_index
                      );
        }
        y_off += y_line;
        bmp_printf(   &bmp // selected color blue value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "            %3d"
                    , palette_picker(palette_index)[selected_color_index].b
                  );
        bmp_printf(   &bmp // selected color green value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "        %3d"
                    , palette_picker(palette_index)[selected_color_index].g
                  );
        bmp_printf(   &bmp // selected color red value
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "    %3d"
                    , palette_picker(palette_index)[selected_color_index].r
                  );
        bmp_printf(   &bmp // selected color index
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "%3d"
                    , selected_color_index
                  );
    }
}

//############################################################################
void LaserBoy_TUI::overlay_target_palette()
{
    u_int  i,
           j,
           y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    for(i = 0; i < palette_picker(target_palette_index).number_of_colors(); i++) // target palette
        for(j = 0; j < font_size_factor; j++)
            bmp_put_line_fast(&bmp,
                              x_off + 70,
                              i * font_size_factor + j,
                              x_off + 120,
                              i * font_size_factor + j,
                              bmp.make_rgb(palette_picker(target_palette_index)[i].r,
                                           palette_picker(target_palette_index)[i].g,
                                           palette_picker(target_palette_index)[i].b
                                          )
                             );
    //------------------------------------------------------------------------
    if(show_color_cursor) // selected_color_index example line
    {
        bmp_put_line_fast(&bmp,
                          x_off + 63,
                          0,
                          x_off + 63,
                          LASERBOY_PALETTE_MAX * font_size_factor,
                          bmp.make_rgb(palette_picker(target_palette_index)[selected_color_index].r,
                                       palette_picker(target_palette_index)[selected_color_index].g,
                                       palette_picker(target_palette_index)[selected_color_index].b
                                      )
                         );
        //----------------------------------------------------------------
        for(j = 0; j < font_size_factor; j++) // selected color marker
            bmp_put_line_fast(&bmp,
                              x_off + 40,
                              selected_color_index * font_size_factor + j,
                              x_off + 80,
                              selected_color_index * font_size_factor + j,
                              white_text.as_uint()
                             );
    }
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // target palette name
                , x_off + 130
                , 0
                , white_text.as_uint()
                , bmp_bg_color
                , 1
                , font_size_factor
                , "%s %-6d"
                , palette_picker(target_palette_index).name.c_str()
                , target_palette_index
              );
    //------------------------------------------------------------------------
    if(show_color_cursor)
    {
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , y_line * 17
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "            %3d"
                    , palette_picker(target_palette_index)[selected_color_index].b
                  );
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , y_line * 17
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "        %3d"
                    , palette_picker(target_palette_index)[selected_color_index].g
                  );
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , y_line * 17
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "    %3d"
                    , palette_picker(target_palette_index)[selected_color_index].r
                  );
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , y_line * 17
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "%3d"
                    , selected_color_index
                  );
    }
}

//############################################################################
void LaserBoy_TUI::overlay_palettes()
{
    u_int  i,
           j,
           y_line       = 8 * font_size_factor,
           y_off        =   LASERBOY_PALETTE_MAX * font_size_factor
                          + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    if(palette_index != current_frame().palette_index)
        palette_index = current_frame().palette_index;
    //------------------------------------------------------------------------
    if(palette_index != LASERBOY_TRUE_COLOR) // 8 bit frame palette
    {
        if(selected_color_index > palette_picker(palette_index).number_of_colors())
            selected_color_index = 0;
        for(i = 0; i < palette_picker(palette_index).number_of_colors(); i++)
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off,
                                  i * font_size_factor + j,
                                  x_off + 50,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(palette_index)[i].r,
                                               palette_picker(palette_index)[i].g,
                                               palette_picker(palette_index)[i].b
                                              )
                                 );
    }
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        for(i = 0; i < palette_picker(target_palette_index).number_of_colors(); i++) // target palette
            for(j = 0; j < font_size_factor; j++)
                bmp_put_line_fast(&bmp,
                                  x_off + 70,
                                  i * font_size_factor + j,
                                  x_off + 120,
                                  i * font_size_factor + j,
                                  bmp.make_rgb(palette_picker(target_palette_index)[i].r,
                                               palette_picker(target_palette_index)[i].g,
                                               palette_picker(target_palette_index)[i].b
                                              )
                                 );
    //------------------------------------------------------------------------
    if(show_color_cursor) // selected_color_index example line
    {
        if(palette_index != LASERBOY_TRUE_COLOR)
            bmp_put_line_fast(&bmp,
                              x_off + 56,
                              0,
                              x_off + 56,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(palette_index)[selected_color_index].r,
                                           palette_picker(palette_index)[selected_color_index].g,
                                           palette_picker(palette_index)[selected_color_index].b
                                          )
                             );
        //----------------------------------------------------------------
        if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
            bmp_put_line_fast(&bmp,
                              x_off + 63,
                              0,
                              x_off + 63,
                              LASERBOY_PALETTE_MAX * font_size_factor,
                              bmp.make_rgb(palette_picker(target_palette_index)[selected_color_index].r,
                                           palette_picker(target_palette_index)[selected_color_index].g,
                                           palette_picker(target_palette_index)[selected_color_index].b
                                          )
                             );
        //----------------------------------------------------------------
        for(j = 0; j < font_size_factor; j++) // selected color marker
            bmp_put_line_fast(&bmp,
                              x_off + 40,
                              selected_color_index * font_size_factor + j,
                              x_off + 80,
                              selected_color_index * font_size_factor + j,
                              white_text.as_uint()
                             );
    }
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR)
        bmp_printf(   &bmp // current frame palette name
                    , x_off
                    , y_off
                    , bmp_bg_color
                    , white_text.as_uint()
                    , 0
                    , font_size_factor
                    , "-24-bit-"
                  );
    else
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "%s %-6d"
                    , palette_picker(palette_index).name.c_str()
                    , palette_index
                  );
    //------------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        bmp_printf(   &bmp // target palette name
                    , x_off + 130
                    , 0
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 1
                    , font_size_factor
                    , "%s %-6d"
                    , palette_picker(target_palette_index).name.c_str()
                    , target_palette_index
                  );
    //------------------------------------------------------------------------
    if(show_color_cursor)
    {
        if(palette_index == LASERBOY_TRUE_COLOR || show_target_palette)
        {
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(0,0,255)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "            %3d"
                        , palette_picker(target_palette_index)[selected_color_index].b
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(0,255,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "        %3d"
                        , palette_picker(target_palette_index)[selected_color_index].g
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 17
                        , bmp.make_rgb(255,0,0)
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "    %3d"
                        , palette_picker(target_palette_index)[selected_color_index].r
                      );
            bmp_printf(   &bmp // target palette name
                        , x_off + 130
                        , y_line * 17
                        , white_text.as_uint()
                        , bmp_bg_color
                        , 1
                        , font_size_factor
                        , "%3d"
                        , selected_color_index
                      );
        }
        y_off += y_line;
        bmp_printf(   &bmp // selected color blue value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,0,255)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "            %3d"
                    , palette_picker(palette_index)[selected_color_index].b
                  );
        bmp_printf(   &bmp // selected color green value
                    , x_off
                    , y_off
                    , bmp.make_rgb(0,255,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "        %3d"
                    , palette_picker(palette_index)[selected_color_index].g
                  );
        bmp_printf(   &bmp // selected color red value
                    , x_off
                    , y_off
                    , bmp.make_rgb(255,0,0)
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "    %3d"
                    , palette_picker(palette_index)[selected_color_index].r
                  );
        bmp_printf(   &bmp // selected color index
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , 0
                    , font_size_factor
                    , "%3d"
                    , selected_color_index
                  );
    }
}

//############################################################################
void LaserBoy_TUI::render_space()
{
    LaserBoy_color  bit_masked;
    u_int           select_start_color,
                    select_end_color  ,
                    cursor_line  = (bmp.yres >= 1024) ? (2) : (1),
                    bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    int             x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                    y_off        =   LASERBOY_PALETTE_MAX * font_size_factor
                                   + (LASERBOY_STATS_DISPLAY_OFFSET + 24) * font_size_factor,
                    y_line       = 8 * font_size_factor;
    double          scale        = bmp.yres / 65536.0;
    //------------------------------------------------------------------------
    render_frame(frame_index, p_bmp());
    //------------------------------------------------------------------------
    if(show_frame_stats)
        overlay_frame_stats();
    if(show_frame_set_stats)
        overlay_frame_set_stats();
    if(show_palette && show_target_palette)
        overlay_palettes();
    else if(show_palette)
        overlay_palette();
    else if(show_target_palette)
        overlay_target_palette();
    //------------------------------------------------------------------------
    if(show_stats)
    {
        string display_name;
        if(current_frame().name.substr(0, 4) == "0000")
            display_name = string("    ") + current_frame().name.substr(4, 4);
        else
            display_name = current_frame().name;
        if(current_frame().name.size() && current_frame().name != "ESI.....")
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , (rainbow_menus) ? (bmp.make_rgb(0,  255, 0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                        , bmp_bg_color
                        , transparent_menu_font
                        , font_size_factor
                        , "name   %s"
                        , display_name.c_str()
                      );
        //----------------------------------------------------------------
        y_off += y_line;
        if(current_frame().size() <= LASERBOY_MAX_USHORT)
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , (rainbow_menus) ? (bmp.make_rgb(  0, 255, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                        , (current_frame().is_unique) ? (bmp_bg_color) : (bmp.make_rgb(0,127,0))
                        , transparent_menu_font
                        , font_size_factor
                        , "vertices %6d"
                        , (int)current_frame().size()
                      );
        else
        {
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , bmp.make_rgb(255,0,0)
                        , (current_frame().is_unique) ? (bmp_bg_color) : (bmp.make_rgb(0,127,0))
                        , transparent_menu_font
                        , font_size_factor
                        , "vertices %6d"
                        , (int)current_frame().size()
                      );
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , (rainbow_menus) ? (bmp.make_rgb(  0, 255, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                        , (current_frame().is_unique) ? (bmp_bg_color) : (bmp.make_rgb(0,127,0))
                        , transparent_menu_font
                        , font_size_factor
                        , "vertices"
                      );
        }
        //----------------------------------------------------------------
        y_off += y_line;
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(255,  0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , (show_Z_as_order) ? (bmp.make_rgb(0,  0, 128)) : (bmp_bg_color)
                    , transparent_menu_font
                    , font_size_factor
                    , "segments %6d"
                    , current_frame().number_of_segments()
                  );
        //----------------------------------------------------------------
        y_off += y_line;
        if(current_frame().is_2D())
            current_frame().format = LASERBOY_2D_FRAME;
        else
           current_frame().format = LASERBOY_3D_FRAME;
        if(current_frame().format == LASERBOY_2D_FRAME)
        {
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , (rainbow_menus) ? (bmp.make_rgb(255, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                        , (current_frame().is_selected) ? (bmp.make_rgb(127,  0,   0)) : (bmp_bg_color)
                        , transparent_menu_font
                        , font_size_factor
                        , "2D frame %6d"
                        , frame_index
                      );
        }
        else
        {
            //----------------------------------------------------------------
            bmp_printf(   &bmp
                        , x_off
                        , y_off
                        , (rainbow_menus) ? (bmp.make_rgb(255, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                        , (current_frame().is_selected) ? (bmp.make_rgb(127,  0,   0)) : (bmp_bg_color)
                        , transparent_menu_font
                        , font_size_factor
                        , "3D frame %6d"
                        , frame_index
                      );
        }
        //----------------------------------------------------------------
    } // end if(show_stats)
    if(current_frame().size() && show_cursors)
    {
        bit_masked = current_frame().at(current_frame().egg).bit_masked_color(signal_bit_mask);
        select_start_color =   current_frame().at(current_frame().egg).is_blank()
                             ? bmp.gray
                             :   current_frame().at(current_frame().egg).is_black(black_level)
                               ? white_text.as_uint()
                               : bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
        //----------------------------------------------------------------
        bit_masked = current_frame().at(current_frame().spider).bit_masked_color(signal_bit_mask);
        select_end_color   =   current_frame().at(current_frame().spider).is_blank()
                             ? bmp.gray
                             :   current_frame().at(current_frame().spider).is_black(black_level)
                               ? white_text.as_uint()
                               : bmp.make_rgb(bit_masked.r, bit_masked.g, bit_masked.b);
        //----------------------------------------------------------------
        // icons
        y_off += (2 * y_line); // vertex segment color for start cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "           %3d"
                    , current_frame().at(current_frame().egg).c
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(255,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "      %4d"
                    , current_frame().segment_index_of_vertex(current_frame().egg)
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0, 255, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "  %4d"
                    , current_frame().egg
                  );
        //----------------------------------------------------------------
        y_off += y_line; // RGB values for start cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "            %3d"
                    , current_frame().at(current_frame().egg).b
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "        %3d"
                    , current_frame().at(current_frame().egg).g
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(255,  0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "    %3d"
                    , current_frame().at(current_frame().egg).r
                  );
        //----------------------------------------------------------------
        y_off += y_line; // color vertex segment for end cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , white_text.as_uint()
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "           %3d"
                    , current_frame().at(current_frame().spider).c
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(255,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "      %4d"
                    , current_frame().segment_index_of_vertex(current_frame().spider)
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0, 255, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "  %4d"
                    , current_frame().spider
                  );
        //----------------------------------------------------------------
        y_off += y_line; // RGB values for end cursor
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "            %3d"
                    , current_frame().at(current_frame().spider).b
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "        %3d"
                    , current_frame().at(current_frame().spider).g
                  );
        bmp_printf(   &bmp
                    , x_off
                    , y_off
                    , (rainbow_menus) ? (bmp.make_rgb(255,   0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                    , bmp_bg_color
                    , transparent_menu_font
                    , font_size_factor
                    , "    %3d"
                    , current_frame().at(current_frame().spider).r
                  );
        //----------------------------------------------------------------
        y_off -= (3 * y_line);
        bmp_put_thick_rectangle(&bmp, x_off
                                    , y_off
                                    , x_off + (int)round(2000 * scale)
                                    , y_off + (int)round(2000 * scale)
                                    , select_start_color
                                    , cursor_line
                               ); // start cursor icon
        //----------------------------------------------------------------
        y_off += (2 * y_line);
        bmp_put_thick_line_fast(&bmp, x_off + (int)round(1000 * scale)
                                    , y_off
                                    , x_off + (int)round(1000 * scale)
                                    , y_off + (int)round(2000 * scale)
                                    , select_end_color
                                    , cursor_line
                               );
        bmp_put_thick_line_fast(&bmp, x_off + (int)round(2000 * scale)
                                    , y_off + (int)round(1000 * scale)
                                    , x_off
                                    , y_off + (int)round(1000 * scale)
                                    , select_end_color
                                    , cursor_line
                               );
        bmp_put_thick_line_fast(&bmp, x_off
                                    , y_off
                                    , x_off + (int)round(2000 * scale)
                                    , y_off + (int)round(2000 * scale)
                                    , select_end_color
                                    , cursor_line
                                );
        bmp_put_thick_line_fast(&bmp, x_off + (int)round(2000 * scale)
                                    , y_off
                                    , x_off
                                    , y_off + (int)round(2000 * scale)
                                    , select_end_color
                                    , cursor_line
                               ); // end cursor icon
        bmp_put_thick_rectangle(&bmp, x_off + (int)round(1000 * scale)
                                    , y_off + (int)round(1000 * scale)
                                    , x_off + (int)round(1000 * scale)
                                    , y_off + (int)round(1000 * scale)
                                    , bmp_bg_color
                                    , cursor_line * 4
                               ); // start cursor icon
    //------------------------------------------------------------------------
    } // end if(show_cursors)
    //------------------------------------------------------------------------
    if(show_TUI_clue)
    {
        bmp_printf(&bmp, // what menu?
                   bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   bmp.yres - 16 * font_size_factor,
                   (rainbow_menus) ? (current_menu_name_bc) : (bmp_bg_color),
                   (rainbow_menus) ? (white_text.as_uint()) : (mono_text.as_uint()),
                   0, // must be opaque for bc
                   font_size_factor,
                   "%s                                                     ",
                   current_menu_name.c_str()
                  );
        bmp_printf(&bmp,
                   bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   bmp.yres - 8 * font_size_factor,
                   (rainbow_menus) ? (white_text.as_uint()) : (mono_text.as_uint()),
                   (rainbow_menus) ? (current_menu_name_bc) : (bmp_bg_color),
                   0,
                   font_size_factor,
                   "%s                                                     ",
                   TUI_clue.c_str()
                  );
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_stats()
{
    u_int i,
          vertices     = (int)current_frame().size(),
          from_vertex  = 0,
          lit          = current_frame().number_of_color_vectors(),
          blank        = current_frame().number_of_dark_vectors(),
          y_line       = 8 * font_size_factor,
          x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
          y_off        = bmp.yres - 13 * y_line, // 13 lines from the bottom
          bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    for(i = 0; i < frame_index; i++)
        from_vertex += (int)frame_picker(i).size();
    //------------------------------------------------------------------------
    TUI_palette_index = 0;

    if(current_frame().size())
    {
        double             sphere    = current_frame().max_distance_from_origin();
        LaserBoy_vertex    at_egg    = current_frame().at(current_frame().egg   ),
                           at_spider = current_frame().at(current_frame().spider);
//                           rectangle = current_frame().max_off_axis_xyz();

        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "name=%s : ownr=%s : id=%d : scn=%d"
                 , current_frame().name.c_str()
                 , current_frame().owner.c_str()
                 , current_frame().identity
                 , current_frame().scanner
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "i %d-%d : %-0.2f%% to origin : %-0.2f%% itself"
                 , from_vertex
                 , from_vertex + vertices - 1
                 , current_frame().relative_volume_to_origin() * 100.0
                 , current_frame().relative_volume() * 100.0
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "%d lit %-0.2f%% : %d drk %-0.2f%% : %d clrs [%s]"
                 , lit
                 , (lit/(double)(vertices - 1)) * 100.0 // a vector takes two vertices!
                 , blank
                 , (blank/(double)(vertices - 1)) * 100.0
                 , current_frame().number_of_unique_colors()
                 , (current_frame().palette_index == LASERBOY_TRUE_COLOR)
                       ?("-24-bit-")
                       :(palette_picker(current_frame().palette_index).name.c_str())
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "lt %-0.2f : dk %-0.2f : av %-0.2f : tot %-0.2f"
                 , current_frame().max_color_magnitude()
                 , current_frame().max_dark_magnitude()
                 , current_frame().total_magnitude() / (vertices - 1)
                 , current_frame().total_magnitude()
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
//                 , "h %d w %d d %d : m %d %d %d : d %-0.2f"
                 , "h=%d w=%d d=%d : ma %-0.2f%c : md %-0.2f"
                 , current_frame().height()
                 , current_frame().width()
                 , current_frame().depth()
                 , (current_frame().max_angle() * one_radian)
                 , 0xf8
//                 , rectangle.x
//                 , rectangle.y
//                 , rectangle.z
                 , sphere
                  );
        if(show_cursors)
        {
/*
            if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
            {
                at_egg.r    = palette_picker(current_frame().palette_index).at(at_egg.c   ).r;
                at_egg.g    = palette_picker(current_frame().palette_index).at(at_egg.c   ).g;
                at_egg.b    = palette_picker(current_frame().palette_index).at(at_egg.c   ).b;
                at_spider.r = palette_picker(current_frame().palette_index).at(at_spider.c).r;
                at_spider.g = palette_picker(current_frame().palette_index).at(at_spider.c).g;
                at_spider.b = palette_picker(current_frame().palette_index).at(at_spider.c).b;
            }
*/
            at_egg    = at_egg.bit_masked(signal_bit_mask);
            at_spider = at_spider.bit_masked(signal_bit_mask);
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                     , "%c %s i=%-6d X=%-7d Y=%-7d Z=%-7d"
                     , 0x01
                     , (at_egg.is_lit()) ? ("lit  ") : ("blank")
                     , current_frame().egg
                     , at_egg.x
                     , at_egg.y
                     , at_egg.z
                      );
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                     , "%c %s i=%-6d X=%-7d Y=%-7d Z=%-7d"
                     , 0x0f
                     , (at_spider.is_lit()) ? ("lit  ") : ("blank")
                     , current_frame().spider
                     , at_spider.x
                     , at_spider.y
                     , at_spider.z
                      );
            y_off += y_line;
            bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                     , "%c---%c %d cntr X=%-0.2f Y=%-0.2f Z=%-0.2f diff=%-0.2f"
                     , 0x01
                     , 0x0f
                     , (int)current_frame().selected_segment().size()
                     , current_frame().selected_segment().rectangular_center_of().x
                     , current_frame().selected_segment().rectangular_center_of().y
                     , current_frame().selected_segment().rectangular_center_of().z
                     , current_frame().at(current_frame().egg) | current_frame().at(current_frame().spider)
                      );
        }
    }
    else
    {
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "name: %s : ownr: %s : id: %d : scn: %d"
                 , current_frame().name.c_str()
                 , current_frame().owner.c_str()
                 , current_frame().identity
                 , current_frame().scanner
                  );
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "This frame has no vertices!"
                  );
    }
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_set_stats()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = bmp.yres - 13 * y_line; // starting at 13 lines from the bottom
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    count_frames();
    TUI_palette_index = 64;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "frames    %d"
             , number_of_frames()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "2D frames %d"
             , num_2D_frames
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "3D frames %d"
             , num_3D_frames
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "verticies %d"
             , total_vertices()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "%-0.2f%% on origin"
             , relative_volume_to_origin() * 100.0
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "%-0.2f%% itself"
             , relative_volume() * 100.0
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "max X %d"
             , closest_x_to_edge()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "max Y %d"
             , closest_y_to_edge()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "max Z %d"
             , closest_z_to_edge()
              );
//    y_off += y_line;
//    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
//             , "GUID %s"
//             , ild_file_GUID.c_str()
//              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_palette_set_stats()
{
    int    i;
    u_int  y_line       = 8 * font_size_factor,
           x_off        = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    TUI_palette_index = 128;
    bmp_printf(&bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "                  "
              );
    bmp_printf(&bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "palette set stats "
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "total palettes %d "
             , number_of_palettes()
              );
    y_off += y_line;
    bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
             , "added palettes %d"
             , number_of_palettes() - LASERBOY_BUILT_IN_PALETTES
              );
    for(i = 0; i < number_of_palettes(); i++)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                 , "%s          "
                 , LaserBoy_palette_set::at(i).name.c_str()
                  );
    }
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_main_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    TUI_palette_index = 192;
    bmp_printf(   &bmp, x_off, y_off
                , (rainbow_menus) ? (bmp.make_rgb(0, 255, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "                                       %d : %d"
                , bmp.xres
                , bmp.yres
              );
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " Welcome to " LASERBOY_VERSION " !!!"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     i o  file           input | output"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     ` ~  show         forward | reverse"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     - _  orbit        forward | reverse"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     = +  show & orbit forward | reverse"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     %c %c  index frame previous | next"
                , 0x1b
                , 0x1a
              );
    y_off += y_line;
    if(number_of_frames() >= 100)
    {
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "     z Z  index frame     +100 | -100"
                  );
        y_off += y_line;
    }
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     9 0  go to          first | last frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     y Y  go to frame at index | unicode"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Space]  current frame select | unselect"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     ( )  set | unset frames-( | )-frames select"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     ? /  show | hide    frame | frame_set stats"
              );
    if(bg_file_name != ".")
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "     ; :  show | hide background bmp    negative"
                  );
    }
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       g  settings  check"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       k  draw, color, blank  vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       l  move, scale, rotate segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       m  render segments by  coordinates"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       n  apply effect functions by name"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       h  transform frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       j  transform frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       c  transform frame colors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       p  transform colors and palette set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       b  wave file utilities"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "     v V  apply view to  frame | frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Bkspc]  delete current frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Tab]  application settings menus"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Enter]  this menu       show | hide"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Esc]  exit LaserBoy"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_file_type_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 83;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "file input type"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wav"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 txt"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 ctn"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 bmp"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 wtf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 utf8"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_ild_in_cd_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 48;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "ild change directory in action"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 stay in current directory %s"
                , (LASERBOY_ILD_SHARE + working_subdirectory).c_str()
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 go to root directory %s"
                , (LASERBOY_ILD_SHARE).c_str()
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 change directory"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_ild_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 48;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "ild file input action"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 prepend to frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 insert after current frame"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append to frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 superimpose frame(s) into frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_dxf_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 96;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "dxf file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame_set with dxf file"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 append  frame_set with dxf file"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 replace frame_set with dxf directory"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append  frame_set with dxf directory"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "font frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 replace frame_set with dxf font (frame_set)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 append  frame_set with dxf font (frame_set)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_wav_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 144;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted wave (optimized)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace   current frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 append to current frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 open as unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted wave clip (not playable)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 replace   current frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 append to current frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 open as unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "unformatted wave (not made in LaserBoy)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 replace   current frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 append to current frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 reframe based on blanking (experimental)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_txt_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 240;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "txt file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 prepend to frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 insert after current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append to frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 superimpose frame(s) into frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 import palettes only"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_ctn_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 48;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "ctn file input action"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 replace frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 prepend to frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 insert after current frame"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 append to frame_set"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 superimpose frame(s) into frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_in_bmp_action_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 32;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "bitmap file input action"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 open as frame color map"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 open as frame_set color map"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 open bmp directory as color map"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 open as frame mask"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 open as frame_set mask"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 open bmp directory as frame_set mask"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 open as palette"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 open as display background"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 open bmp directory as background"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    +|- bmp frame in menus K, L with Q, W."
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 clear display background bitmap"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_if_not_z_options()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 16;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "signal option if not Z"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, bmp.make_rgb(255,0,0), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[%s]"
                , LaserBoy_wave_signal_id_to_name(signal_if_not_z).c_str()
              );
    y_off += y_line;
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_NO_SIGNAL)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 1 nothing"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    1 nothing"
                  );
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_SIGNAL_MONO_TTL)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 2 monochrome TTL (on or off)"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    2 monochrome TTL (on or off)"
                  );
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_SIGNAL_MONO_AVG_ANALOG)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 3 monochrome simple average of RGB"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    3 monochrome simple average of RGB"
                  );
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_SIGNAL_MONO_OR_ANALOG)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 4 monochrome bitwise OR of RGB"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    4 monochrome bitwise OR of RGB"
                  );
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 5 monochrome weighted RGB gray scale"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    5 monochrome weighted RGB gray scale"
                  );
    y_off += y_line;
    if(signal_if_not_z == LASERBOY_SIGNAL_MONO_O_SCOPE)
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "--> 6 monochrome weighted gray for o-scope"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    6 monochrome weighted gray for o-scope"
                  );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_dots_options()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 220;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "what to do with dots"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ignore  dots"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 remove  dots"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 enhance dots by dwell of %d"
                , dwell_on_dot
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_file_type_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 50;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "output file type"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wav"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 txt"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 ctn"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 bmp"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 wtf"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 utf8"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_ild_out_cd_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    TUI_palette_index = 48;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "ild change directory out action"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 stay in current directory %s"
                , (LASERBOY_ILD_SHARE + working_subdirectory).c_str()
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 go to root directory %s"
                , (LASERBOY_ILD_SHARE).c_str()
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 change directory"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 make new directory"
              );

    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 split frame_set into new directory"
              );

    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_ild_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 150;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "laser art output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ild save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 ild save [%d] selected frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 ild save all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 ild effect current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 ild effect [%d] selected frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 ild effect frame_set"
              );
    if(working_subdirectory == "fonts/")
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    7 ild save font and utf8 index file."
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_dxf_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 86;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "DXF output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 dxf save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 dxf save directory of [%d] selected frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 dxf save directory all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_wav_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 72;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted wave output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "optimized (playable)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 wave of current frame once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 wave of current frame for %.2f seconds"
                , still_frame_duration
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 wave animation of each frame shown once"
                , frames_per_second
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 wave animation at %.3f fps of all frames"
                , frames_per_second
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "optimized clip (not playable)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 wave clip of current frame"
                , frames_per_second
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "unoptimized wave (as-is vector data)"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 wave of current frame once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 wave of current frame for %.2f seconds"
                , still_frame_duration
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 wave animation of each frame shown once"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 wave of all frames at %.3f fps"
                , frames_per_second
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_txt_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 133;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "plain ASCII text output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 save [%d] selected frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 save all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 save current frame palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 save target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 save LaserBoy wave color rescales"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 save system settings (wtf)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_ctn_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 150;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "laser art output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 ctn save current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 ctn save [%d] selected frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 ctn save all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_out_bmp_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 166;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "bitmap output options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  2D lit vectors only"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 bmp save %d%c pxl of current frame"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 bmp save directory %d%c pxl [%d] selected frames"
                , output_bmp_size
                , 0xfd
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 bmp save directory %d%c pxl all frames"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  current view with visuals"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 bmp save %d%c pxl of current frame"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 bmp save directory %d%c pxl [%d] selected frames"
                , output_bmp_size
                , 0xfd
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 bmp save directory %d%c pxl all frames"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel action"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 user interface visuals"
                , lit_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 change and apply view"
                , max_dwell_millisec
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 raster display parameters"
                , blank_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 vector editor  parameters"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 optimization   parameters"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 timing and wave settings"
                , lit_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 file type specific options"
                , blank_delta_max
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 signal bit resolution"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_optimization_factors_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "optimization parameters"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 insignificant angle      degrees ... %3.2lf%c"
                , insignificant_angle * one_radian
                , 0xf8
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 insignificant distance   points .... %3.2lf"
                , insignificant_distance
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 max lit   distance       points .... %3.2lf"
                , lit_delta_max
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 max blank distance       points .... %3.2lf"
                , blank_delta_max
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 black threshhold ................... %d"
                , black_level
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 max wagging time         seconds ... %1.4f"
                , (max_wag_microsec / 1000000.0)
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 max angular dwell        millisec .. %3.2lf"
                , max_dwell_millisec
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 lit dwell overhang       vertices .. %d"
                , lit_dwell_overhang
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 dwell on dot             vertices .. %d"
                , dwell_on_dot
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 what to do with dots ............... %s"
                , LaserBoy_dots_setting_id_to_name(dots_setting).c_str()
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (no_equivalent_vectors)
                    ? ("--> a remove equal vectors     (slow)")
                    : ("    a remove equal vectors     (slow)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (conglomerate_in_minimize)
                    ? ("--> b conglomerate in minimize (slower)")
                    : ("    b conglomerate in minimize (slower)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (fracture_b4_conglomerate)
                    ? ("--> c fracture conglomerate    (slowest)")
                    : ("    c fracture conglomerate    (slowest)")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (black_dwell_vertices)
                    ? ("--> d black dwell")
                    : ("    d black dwell")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (no_black_match)
                    ? ("--> e no black palette match")
                    : ("    e no black palette match")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (allow_lit_black)
                    ? ("--> f allow lit black")
                    : ("    f allow lit black")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_1_frame_bridge)
                    ? ("--> g save single frame with bridge")
                    : ("    g save single frame with bridge")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_minimize)
                    ? ("--> h auto minimize ild, dxf, txt file save")
                    : ("    h auto minimize ild, dxf, txt file save")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_opti_ctn)
                    ? ("--> i auto optimize ctn output")
                    : ("    i auto optimize ctn output")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit optimization parameters"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_vector_editor_values_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "vector editor parameters"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 move points per key hit .......... %d"
                , displacement_step
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 move as 1 / [n] of space ......... 1/%5.6f"
                , 65536.0 / displacement_step
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 rotate degrees per key hit ....... %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 scale %% per key hit .............. %3.2f%%"
                , magnitude_step * 100.0
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 rendered arc step in degrees ..... %3.2f%c"
                , rendered_arc_angle
                , 0xf8
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 rendered mono font field space ... %3.2f%%"
                , rendered_mono_font_space * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 rendered variable font gap ....... %3.2f%%"
                , rendered_vari_font_gap * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 rendered variable font space ..... %3.2f%%"
                , rendered_vari_font_space * 100.0
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (destructive_clipping)
                    ? ("--> 9 destructive clipping in move scale rotate")
                    : ("    9 destructive clipping in move scale rotate")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (maintain_real_origin)
                    ? ("--> 0 normalize vectors to origin (editor & dxf import)")
                    : ("    0 normalize vectors to origin (editor & dxf import)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (bond_word)
                    ? ("--> a bond words in editor font rendering")
                    : ("    a bond words in editor font rendering")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit vector editor parameters"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_timing_and_wave_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "timing and wave settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 wave sample shift offsets"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [1] X            %d"
                , wave_offsets[0]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [2] Y            %d"
                , wave_offsets[1]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [3] red          %d"
                , wave_offsets[2]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [4] green        %d"
                , wave_offsets[3]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [5] blue         %d"
                , wave_offsets[4]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [6] Z or other   %d"
                , wave_offsets[5]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [7] L audio      %d"
                , wave_offsets[6]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       [8] R audio      %d"
                , wave_offsets[7]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 frames per second ................. %3.2lf"
                , frames_per_second
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 still frame duration in seconds ... %3.2lf"
                , still_frame_duration
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 output wave sample rate ........... %d"
                , sample_rate
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 unframed wave saples per frame .... %d"
                , unframed_wave_stride
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 wave chasnnel 6 if not z .......... %s"
                , LaserBoy_wave_signal_id_to_name(signal_if_not_z).c_str()
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (z_from_unformatted)
                    ? ("--> 7 import Z from unformatted wave")
                    : ("    7 import Z from unformatted wave")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_flatten_z)
                    ? ("--> 8 omit   Z in wave output (6 is intensity)")
                    : ("    8 omit   Z in wave output (6 is intensity)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (invert_wave_output)
                    ? ("--> 9 invert wave output (all channels)")
                    : ("    9 invert wave output (all channels)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_apply_offsets)
                    ? ("--> 0 apply  system offsets to wave")
                    : ("    0 apply  system offsets to wave")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_apply_rescales)
                    ? ("--> a apply  system color rescales to wave")
                    : ("    a apply  system color rescales to wave")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (channels_of_wav_out == 8)
                    ? ("--> b save   8 channel waves (zero 7 & 8)")
                    : ("    b save   8 channel waves (zero 7 & 8)")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit timing and wave settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_display_options_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 158;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "raster display parameters"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 rendered vector line width ............ %d"
                , rendered_line_width
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 menu font size factor ................. %d"
                , font_size_factor
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 background bitmap size factor ......... %3.2lf%%"
                , background_bmp_scale * 100.0
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                        %3d"
                , bg_color.b
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "                                                   %3d"
                , bg_color.g
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255,   0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                              %3d"
                , bg_color.r
              );
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 display background color .............."
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                        %3d"
                , mono_text.b
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                   %3d"
                , mono_text.g
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255,   0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                              %3d"
                , mono_text.r
              );
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 mono text color ......................."
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                        %3d"
                , rendered_blank.b
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color, transparent_menu_font, font_size_factor
                , "                                                   %3d"
                , rendered_blank.g
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255,   0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                              %3d"
                , rendered_blank.r
              );
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 rendered blank line color ............."
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0,   0, 255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                        %3d"
                , rendered_black.b
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 255,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                                   %3d"
                , rendered_black.g
              );
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255,   0,   0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , transparent_menu_font, font_size_factor
                , "                                              %3d"
                , rendered_black.r
              );
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 rendered black line color ............."
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 onion skin fade ....................... %3.2lf%%"
                , onion_skin_fade * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 onion skin depth ...................... %d"
                , number_of_skins
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 output bitmap size in square pixels ... %d%c"
                , output_bmp_size
                , 0xfd
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit raster display parameters"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_view_menu()
{
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off,
           y_line       =  8 * font_size_factor,
           x_tab        = 26 * y_line,
           x_tab2       =  9 * y_line;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    bmp_printf(   &bmp
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "press & hold view keys, [Shift] reverse"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "1 ! orbit X   %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "5 %% front | back"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "2 @ orbit  Y  %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "6 ^  left | right"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "3 # orbit   Z %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "7 &   top | bottom"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "4 $ orbit XYZ %3.2f%c"
                , rotation_step * one_radian
                , 0xf8
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "8 * orbit & frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "q Q pan X  %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "a A zoom X  +|- %3.2f%%"
                , magnitude_step * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "w W pan  Y %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "s S zoom  Y +|- %3.2f%%"
                , magnitude_step * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "e E pan XY %5d points"
                , displacement_step
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "d D zoom XY +|- %3.2f%%"
                , magnitude_step * 100.0
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "r R center"
              );
    bmp_printf(   &bmp
                , x_tab
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "f F 1:1"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "current view"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,0,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "X"
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "Y"
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp.make_rgb(0,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "Z"
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , " angle                              degrees"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.x * one_radian
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.y * one_radian
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp.make_rgb(255,255,0)
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_angle.z * one_radian
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp_bg_color
                , 0
                , font_size_factor
                , "offset                              points"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-6d"
                , (int)view_offset.x
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(255,0,255)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-6d"
                , (int)view_offset.y
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp_bg_color
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "      "
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp_bg_color
                , 0
                , font_size_factor
                , " scale"
              );
    bmp_printf(   &bmp
                , x_tab2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp.make_rgb(127,0,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_scale.x
              );
    bmp_printf(   &bmp
                , x_tab2 * 2
                , y_off
                , bmp.make_rgb(0,255,0)
                , bmp.make_rgb(0,127,0)
                , 0
                , font_size_factor
                , "%-3.4f"
                , view_scale.y
              );
    bmp_printf(   &bmp
                , x_tab2 * 3
                , y_off
                , bmp_bg_color
                , bmp.make_rgb(0,0,127)
                , 0
                , font_size_factor
                , "      "
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  v V  to apply view to frame | frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  [Esc] exit view & apply"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_visuals_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 80;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "user interface visuals settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_vectors)
                    ? (" --> 1 vectors")
                    : ("     1 vectors")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_vertices)
                    ? (" --> 2 vertices")
                    : ("     2 vertices")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_direction)
                    ? (" --> 3 direction")
                    : ("     3 direction")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_blanking)
                    ? (" --> 4 blanking")
                    : ("     4 blanking")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_intro)
                    ? (" --> 5 intro")
                    : ("     5 intro")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_bridge)
                    ? (" --> 6 bridge")
                    : ("     6 bridge")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_coda)
                    ? (" --> 7 coda")
                    : ("     7 coda")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_indices)
                    ? (" --> 8 indices")
                    : ("     8 indices")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_palette)
                    ? (" --> 9 palette")
                    : ("     9 palette")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_target_palette)
                    ? (" --> 0 target palette")
                    : ("     0 target palette")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_stats)
                    ? (" --> a stats")
                    : ("     a stats")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (transparent_menu_font)
                    ? (" --> b transparent menu font")
                    : ("     b transparent menu font")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (!rainbow_menus)
                    ? (" --> c monochrome  menu font")
                    : ("     c monochrome  menu font")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_fixed_bounds)
                    ? (" --> d fixed bounds")
                    : ("     d fixed bounds")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_fixed_origin)
                    ? (" --> e fixed origin")
                    : ("     e fixed origin")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_floating_axis)
                    ? (" --> f floating axis")
                    : ("     f floating axis")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_floating_bounds)
                    ? (" --> g floating bounds")
                    : ("     g floating bounds")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_onion_skin)
                    ? (" --> h onion skin")
                    : ("     h onion skin")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_onion_visuals)
                    ? (" --> i onion visuals")
                    : ("     i onion visuals")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_Z_as_order)
                    ? (" --> j display vector order in Z")
                    : ("     j display vector order in Z")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_quad_view)
                    ? (" --> k quad orthographic view")
                    : ("     k quad orthographic view")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (loop_animation)
                    ? (" --> l loop animation")
                    : ("     l loop animation")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (approximate_frame_rate)
                    ? (" --> m animate at approximated frame rate")
                    : ("     m animate at approximated frame rate")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_effects_generation)
                    ? (" --> n show effect rendering (slow)")
                    : ("     n show effect rendering (slow)")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_cursor_limits_msg)
                    ? (" --> o vector cursor limits messages")
                    : ("     o vector cursor limits messages")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (infinite_vector)
                    ? (" --> p infinite spider vector")
                    : ("     p infinite spider vector")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (show_TUI_clue)
                    ? (" --> q TUI clue")
                    : ("     q TUI clue")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (track_bmp_view)
                    ? (" --> r track bmp background with view")
                    : ("     r track bmp background with view")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit user interface visuals settings"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_file_type_specific_options_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 190;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "file type specific options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_ild_fmt_4_5)
                    ? ("--> 1 ild save format 4 or 5")
                    : ("    1 ild save format 4 or 5")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_2D_as_3D)
                    ? ("--> 2 ild save 2D frames as 3D")
                    : ("    2 ild save 2D frames as 3D")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (auto_scale_dxf)
                    ? ("--> 3 dxf scale coordinate import")
                    : ("    3 dxf scale coordinate import")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_true_color_dxf)
                    ? ("--> 4 dxf save in true color (420)")
                    : ("    4 dxf save in true color (420)")
                  )
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_unit)
                    ? ("--> 5 txt save in unit coordinates (-1.0 to +1.0)")
                    : ("    5 txt save in unit coordinates (-1.0 to +1.0)")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_with_color)
                    ? ("--> 6 txt save frames with integrated color")
                    : ("    6 txt save frames with integrated color")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_color_hex)
                    ? ("--> 7 txt save colors in hex")
                    : ("    7 txt save colors in hex")
                  )
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , (   (save_txt_named_palettes)
                    ? ("--> 8 txt save named LaserBoy palette values")
                    : ("    8 txt save named LaserBoy palette values")
                  )
              );
    //------------------------------------------------------------------------
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit switch settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_color_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 45;
    if(alt_menu_c)
    {
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "transform frame colors to -24-bit- hues"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  = set hue     span factor        %3.2lf"
                    , recolor_span_factor
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  + set hue     index multiple     %d"
                    , recolor_index_multiple
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  - set hue     color frame shift  %d"
                    , color_frame_shift
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  < expose black to color (current frame)"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  > expose black to color (all frames)"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  t toggle to palette menu"
                  );
    }
    else
    {
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "transform frame colors to palette"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  = set palette span factor        %3.2lf"
                    , recolor_span_factor
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  + set palette index multiple     %d"
                    , recolor_index_multiple
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  - set palette color frame shift  %d"
                    , color_frame_shift
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  < expose black to color (current frame)"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  > expose black to color (all frames)"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  t toggle to -24-bit- hues menu"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Shift] key with below applies to all frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  a span space on X"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  b span space on Y"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  c span space on Z"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  d span radial from origin"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  e span axial  from origin"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  f span  vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  g index vectors"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  h span  segments"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  i index segments"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  j random color vectors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  k random color segments"
              );
    if(!alt_menu_c)
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  l best match colors to target"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  m change to target by index"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  n to all white"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  o to mono RGB logical OR"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  p to mono gray scale"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  q promote to 24-bit"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  r 24-bit to palette lossless"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  s 24-bit to palette bit reduce RGB to 332"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  v reduce to minimal palette best match"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  u best reduce all frames to 1 super palette"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , " %c%c +|- target palette index"
                    , 0x18
                    , 0x19
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  . set target to current frame's palette"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  , set frame  to first use of target palette"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit frame to palettes transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_palette_set_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 45;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "transform colors and palette set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  %c %c +|- target palette"
                , 0x18
                , 0x19
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  p P +|- selected color index"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a import palette(s) from txt"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b import palette from bmp"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c export target palette to txt"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d export all added palettes to txt"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    e delete unused added palettes"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    f delete target palette"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    g add new target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    h copy target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    i copy target palette in reverse order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j copy target palette in RGB sum order"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    k copy target palette in negative colors"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    l copy target palette expanded to 255 colors"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m add new color to target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    n set selected color RGB in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    o insert color RGB before selected in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    q delete selected color in target palette"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    r delete selected color to end of target palette"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    s rename added target palette"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "define first 2 to 7 colors in target to apply below"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    t linear   blend 2 to 7 colors in target"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    u circular blend 2 to 7 colors in target"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit palette set transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_set_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 170;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "frame_set transforms"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        %d frames are currently selected."
                , number_of_selected_frames()
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Space] select | unselect frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ( )    set | unset frames-( | )-frames select"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 0  go to            first | last frame"
              );
    y_off += y_line;
    if(number_of_frames() >= 100)
    {
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    z Z index frame        +100 | -100"
                  );
        y_off += y_line;
    }
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  a invert  selections"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  b delete  selected  frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  c trim to selected  frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  d delete  redundant frames"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  e copy selected frames before first"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  f copy selected frames after  last"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  g move selected frames before first"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  h move selected frames after  last"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  i add empty frame before first"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  j add empty frame after  current"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  k add empty frame after  last"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  l reverse order of frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  m reverse order of selected frames"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  n rotate  order of frame_set current first"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  o random  order of frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  p unicode order of frame_set (names)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  q delete  every n-th frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  r trim to every n-th frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  > explode frame     into segment frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  = explode frame_set into segment frame_set"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  < amass   selected frames into one"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit frame_set transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_frame_tranz_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 57;
    if(alt_menu_h)
    {
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "modify frame space"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    t toggle to minimize optimize menu"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    T normalize vectors %% to space on itself | origin"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "[Shift] key applies to all frames"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    a flatten Z"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    b Z order"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    c mirror"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    d flip"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    e quarter turn CCW"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    f reduce flat 3D to 2D"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    g normalize vectors %% to space %s",
                    (maintain_real_origin) ? ("with origin") : ("of itself")
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    h normalize vectors to dimension and center"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    i center    vectors in X|Y|Z"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    j reverse vector order"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    k unblank all vectors"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    l black to color"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    m black to blank"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    n blank to black"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    o impose black level"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    p convert to dots"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    q rename frame"
                  );
    }
    else
    {
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "minimize, optimize frame"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    t toggle to modify frame space menu"
                  );
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "[Shift] key applies to all frames"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    a remove blank span"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    b remove angle dwell"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    c remove dots (not lines)"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    d fit to insignificant distace %-6.2f"
                    , insignificant_distance
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    e reduce lit vectors"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    f remove equivalent vectors"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    g conglomerate segments"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    h bond segments within         %-6.2f"
                    , insignificant_distance
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    i minimize"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    j fracture  segments"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    k randomize segments"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    l reorder   segments"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    m add angle dwell"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    n add lit distance span"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    o add blank distance span"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    p enhance dots"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    q optimize"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit frame transforms"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_draw_color_blank_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 22;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "draw color blank vectors %c---%c or vertex at %c"
                , 0x01
                , 0x0f
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    set %c on %c to select single vector!"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    u  U change select %c---%c by +|- 1 lit segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   iI oO change select %c | %c by +|- 1 lit segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [{ ]} change select %c | %c by +|- 1 vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    \\  |    set select %c | %c on first | last vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    '  \"    set select %c | %c to vertex index"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    K  L    set select  %c to %c | %c to %c"
                , 0x0f
                , 0x01
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j  J    move vertex %c to %c | %c to %c"
                , 0x0f
                , 0x01
                , 0x01
                , 0x0f
              );
    if(bmp_file_index != -1)
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    Q  W +|- background bmp frame"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    if(current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    %c  %c +|- target   palette index"
                    , 0x18
                    , 0x19
                  );
        y_off += y_line;
    }
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    p  P +|- selected color   index"
              );
    if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    c  C %c---%c   set | get color"
                    , 0x01
                    , 0x0f
                  );
    }
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b  B %c---%c blank | unblank"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [dot] . add vertex after %c split mid or at end"
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       > add blank vertex at %c"
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       l add line from %c to %c to end of frame"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Bkspc] delete vertex at %c"
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    ,  < reorder all from %c | %c as first vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       ` copy frame"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "xX yY zZ move   vertex at %c in     X|Y|Z +|- %d"
                , 0x0f
                , displacement_step
              );
    if(alt_menu_k)
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       t toggle to vector  edit menu"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       d %c---%c convert to dots"
                    , 0x01
                    , 0x0f
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    e  E %c---%c bond | unbond with lit black"
                    , 0x01
                    , 0x0f
                  );
        y_off += y_line;
        if(current_frame().palette_index != LASERBOY_TRUE_COLOR)
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "    f  F %c---%c set to palette by index | span"
                        , 0x01
                        , 0x0f
                      );
        else
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "    f  F %c---%c set to target palette by index | span"
                        , 0x01
                        , 0x0f
                      );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       g %c---%c set black to selected color"
                    , 0x01
                    , 0x0f
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    m  M %c---%c set black to palette by index | span"
                    , 0x01
                    , 0x0f
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    n  N %c---%c +|- cycle colors"
                    , 0x01
                    , 0x0f
                  );
    }
    else
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       t toggle to segment edit menu"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , " q  w  e set    vertex at %c        X|Y|Z"
                    , 0x0f
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "dD fF gG rotate vector at %c around X|Y|Z +|- %3.2f%c"
                    , 0x0f
                    , rotation_step * one_radian
                    , 0xf8
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    m  M scale  vector at %c magnitude    +|- %3.2f%%"
                    , 0x0f
                    , magnitude_step * 100.0
                  );
        y_off += y_line;
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Esc] exit draw, color, blank"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_move_scale_rotate_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 47;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "move scale rotate selected vectors %c---%c"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    u  U change select %c---%c by +|- 1 lit segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   iI oO change select %c | %c by +|- 1 lit segment"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [{ ]} change select %c | %c by +|- 1 vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    \\  |    set select %c | %c on first | last vertex"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    '  \"    set select %c | %c to vertex index"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    K  L    set select  %c to %c | %c to %c"
                , 0x0f
                , 0x01
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       < reverse %c---%c vector order"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       c copy    %c---%c (in place) to end"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "       ` copy frame"
              );
    if(bmp_file_index != -1)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    Q  W +|- background bmp frame"
                  );
    }
    //------------------------------------------------------------------------
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " q  w  e set     %c---%c X|Y|Z center of itself"
                , 0x01
                , 0x0f
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "xX yY zZ move    %c---%c X|Y|Z            +|- %d"
                , 0x01
                , 0x0f
                , displacement_step
              );
    if(alt_menu_l)
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       t toggle to relative to center of itself"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "dD fF gG rotate  %c---%c on fulcrum X|Y|Z +|- %3.2f%c"
                    , 0x01
                    , 0x0f
                    , rotation_step * one_radian
                    , 0xf8
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "vV bB nN scale   %c---%c on fulcrum X|Y|Z +|- %3.2f%%"
                    , 0x01
                    , 0x0f
                    , magnitude_step * 100.0
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "      mM scale   %c---%c on fulcrum  all  +|- %3.2f%%"
                    , 0x01
                    , 0x0f
                    , magnitude_step * 100.0
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       fulcrum X=%-0.2f Y=%-0.2f Z=%-0.2f"
                    , fulcrum.x
                    , fulcrum.y
                    , fulcrum.z
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , " j  k  l set  fulcrum X|Y|Z"
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "aA sS rR move fulcrum X|Y|Z +|- %d"
                    , displacement_step
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    ,  . set  fulcrum on   %c | %c"
                    , 0x01
                    , 0x0f
                  );
    }
    else
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "       t toggle to relative to fulcrum"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "dD fF gG rotate  %c---%c on itself  X|Y|Z +|- %3.2f%c"
                    , 0x01
                    , 0x0f
                    , rotation_step * one_radian
                    , 0xf8
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "vV bB nN scale   %c---%c on itself  X|Y|Z +|- %3.2f%%"
                    , 0x01
                    , 0x0f
                    , magnitude_step * 100.0
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "      mM scale   %c---%c on itself   all  +|- %3.2f%%"
                    , 0x01
                    , 0x0f
                    , magnitude_step * 100.0
                  );
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
        y_off += y_line;
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " [Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   [Esc] exit move, scale, rotate vectors"
              );
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_direct_draw_segments_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 100;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "direct draw segments by coordinates"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    `   copy frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      ~ new frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    p P +|- selected color"
              );
    if(palette_index == LASERBOY_TRUE_COLOR)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "    %c %c +|- target palette"
                    , 0x18
                    , 0x19
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  3D"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a point"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b line"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  2D"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c rectangle"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d polygon"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    e polyline"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    f polystar"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    g circular arc"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    h elliptical arc"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  wiki the following curves!"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    i rhodonea"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    j epicycloid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    k epitrochoid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    l hypocycloid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    m hypotrochoid"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    n Lissajous curve"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    o harmonograph"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    q mono spaced font"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    r variable spaced font"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] show | hide menu"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] exit direct draw segments"
              );
    //------------------------------------------------------------------------
    return;
}
//############################################################################
void LaserBoy_TUI::overlay_oscillator_stats(LaserBoy_oscillator lbo, string name, u_int number)
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 100;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "oscillator stats"
              );
    y_off += y_line;
    y_off += number * y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "%s : amp %3.6lf frq %3.6lf phs %3.6lf dmp %3.6lf"
                , name.c_str()
                , lbo.amplitude
                , lbo.frequency
                , lbo.phase
                , lbo.damping
              );
    y_off += y_line;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_effect_functions_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 120;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "apply effect by name"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  1 apply effect on current frame"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  2 apply effect on selected [%d] frames"
                , number_of_selected_frames()
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  3 apply effect on frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit apply effect by name"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_utilities_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 222;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave file utilities"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 wave show screen settings"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 show wave on screen"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 set system wave sample offsets"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 apply offsets to wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 overwrite offsets in LaserBoy header"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 set color rescale files"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 apply color rescales to wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 remove color rescale tables in LaserBoy header"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 save color rescale tables to files"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    0 impose 8-bit color and black level of %d"
                , black_level
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    a impose bit resolution on wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    b split or join wave channels"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    c invert signal polarities of LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    d clear signal polarity list in LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "e | E invert polarity of LaserBoy | unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "f | F show LaserBoy | unformatted header information"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "g | G find max vector in LaserBoy | unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit wave file utilities"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_show_split_or_join_waves_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 233;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "split or join waves options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 split wave into XY, r, g, b, i, LR"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 split wave into XY, rg, b (i or Z), LR"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 add stereo audio to wave (6 to 8)"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "LaserBoy formatted waves"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    4 join XY, r, g, b, i or Z, waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    5 join XY, rg, b[Z], waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    6 join XY, rg, b[Z], LR waves to 8 channel wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "unformatted waves"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    7 join XY, r, g, b, i or Z waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    8 join XY, rg, b[Z], waves to 6 channel wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    9 join XY, rg, b[Z], LR waves to 8 channel wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit split or join waves options"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_show_wave_options_menu()
{
    int y_line = 8 * font_size_factor,
        x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
        y_off  = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 111;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "show wave options"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    1 show LaserBoy wave"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    2 show LaserBoy wave as unformatted"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "    3 show unformatted wave"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit show wave options"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_show_settings_menu()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 37;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave show settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_vertices)?(" --> 1 vertices"):("     1 vertices"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_blanks)?(" --> 2 blanking"):("     2 blanking"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_real_time)?(" --> 3 real time"):("     3 real time"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_show_inverted)?(" --> 4 inverted"):("     4 inverted"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_clean_screen)?(" --> 5 clean screen"):("     5 clean screen"))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , ((wave_loop)?(" --> 6 loop"):("     6 loop"))
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] exit wave show settings"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_TUI::overlay_wave_analysis(string file_name)
{
    std::fstream in;
    in.open(file_name.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        u_int     //i,
                  y_line        = 8 * font_size_factor,
                  x_off         = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                  y_off         = x_off,
                  sample_count  = 0,
                  max_diff_x    = 0,
                  max_diff_y    = 0,
                  max_x_index   = 0,
                  max_y_index   = 0,
                  bmp_bg_color  = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
//        vector<int>             red_map(1024),
//                                green_map(1024),
//                                blue_map(1024);
//        for(u_int i = 0; i < 1024; i++)
//            red_map[i] = green_map[i] = blue_map[i] = 0;
        LaserBoy_wave_header header;
        header.from_fstream_wave(in);
        LaserBoy_wave_sample sample_0(header.num_channels),
                             sample_1(header.num_channels);
        //----------------------------------------------------------------
        p_GUI->display_state("analyzing wave");
        while(sample_1.from_fstream_wave(in))
        {
            if((u_int)(abs(sample_1.channel[0] - sample_0.channel[0])) > max_diff_x)
            {
                max_diff_x = abs(sample_1.channel[0] - sample_0.channel[0]);
                max_x_index = sample_count;
            }
            if((u_int)(abs(sample_1.channel[1] - sample_0.channel[1])) > max_diff_y)
            {
                max_diff_y = abs(sample_1.channel[1] - sample_0.channel[1]);
                max_y_index = sample_count;
            }
//            ++(red_map[(sample_1.channel[2] + LASERBOY_MAX_SHORT) / 64]);
//            ++(green_map[(sample_1.channel[3] + LASERBOY_MAX_SHORT) / 64]);
//            ++(blue_map[(sample_1.channel[4] + LASERBOY_MAX_SHORT) / 64]);
            sample_0 = sample_1;
            sample_count++;
            if(!(sample_count % 4800))
                p_GUI->display_progress(header.num_samples - sample_count);
        }
        in.close();
        render_space();
        //----------------------------------------------------------------
        TUI_palette_index = 14;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "wave analysis"
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , file_name.c_str()
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "num_samples .............: %u"
                    , header.num_samples
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "sample_rate .............: %u"
                    , header.sample_rate
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "number of channels ......: %d"
                    , header.num_channels
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "bits per sample .........: %d"
                    , header.bits_per_sample
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "playing time ............: %s hh:mm:ss.ss"
                    , time_as_string((double)header.num_samples / header.sample_rate).c_str()
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "maximum single step in X : %d"
                    , max_diff_x
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "at sample ...............: %d"
                    , max_x_index
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "maximum single step in Y : %d"
                    , max_diff_y
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "at sample ...............: %d"
                    , max_y_index
                  );
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "[Esc] to continue"
                  );
        //----------------------------------------------------------------
/*
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(green_map[i] > max_diff_x)
                max_diff_x = green_map[i];

        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((green_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(0,255,0)
                             );
        //----------------------------------------------------------------
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(blue_map[i] > max_diff_x)
                max_diff_x = blue_map[i];

        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((blue_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(0,0,255)
                             );
        //----------------------------------------------------------------
        max_diff_x = 0;
        for(u_int i = 0; i < 1024; i++)
            if(red_map[i] > max_diff_x)
                max_diff_x = red_map[i];
        for(u_int i = 0; i < 1024; i++)
            bmp_put_line_fast(&bmp,
                              i,
                              bmp.yres - (int)((red_map[i] / (double)max_diff_x) * 1024),
                              i,
                              bmp.yres,
                              bmp.make_rgb(255,0,0)
                             );
        //----------------------------------------------------------------
*/
        return true;
    }
    else
        return false;
}

//############################################################################
bool LaserBoy_TUI::overlay_wave_header_stats(string file_name)
{
    int       i,
              y_line       = 8 * font_size_factor,
              x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
              y_off        = x_off;
    u_int     bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    std::fstream         in;
    LaserBoy_wave_header header;
    //------------------------------------------------------------------------
    in.open(file_name.c_str(), ios::in | ios::binary);
    if(in.is_open())
    {
        header.from_fstream_wave(in);
        in.close();
    }
    else
        return false;
    //------------------------------------------------------------------------
    TUI_palette_index = 19;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave stats"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , file_name.c_str()
              );
    y_off += y_line;
    y_off += y_line;
    if(header.version == "!LaserBoy!")
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "wave has no LaserBoy format information"
                  );
    else
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "LaserBoy wave header version : %s"
                    , header.version.c_str()
                  );
    if(!(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED))
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "no optimization information"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "num_samples ..........: %u"
                , header.num_samples
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "sample_rate ..........: %u"
                , header.sample_rate
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "number of channels ...: %d"
                , header.num_channels
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "bits per sample ......: %d"
                , header.bits_per_sample
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "playing time .........: %s hh:mm:ss.ss"
                , time_as_string((double)header.num_samples / header.sample_rate).c_str()
              );
    if(header.version != "!LaserBoy!")
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , ((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) ? ("wave polarity ........: positive signal") : ("wave polarity ........: negative signal"))
                  );
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "lit dwell overhang ...: %d"
                        , header.parms.lit_dwell_overhang
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "lit distance delta max: %6.2f"
                        , header.parms.lit_delta_max
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "blank dist delta max .: %6.2f"
                        , header.parms.blank_delta_max
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "max dwell u_sec ......: %6.2f"
                        , header.parms.max_dwell_millisec
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "max scan wag u_sec ...: %1.4f"
                        , header.parms.max_wag_microsec
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "insignificant distance: %6.2f"
                        , header.parms.insignificant_distance
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "insignificant angle ..: %6.2f"
                        , header.parms.insignificant_angle
                      );
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "frames per second ....: %6.2f"
                        , header.parms.frames_per_second
                      );
        }
        if(header.LaserBoy_wave_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "number of frames .....: %d"
                        , header.num_frames
                      );
        }
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "ch: +/- :  contents     : LSB subcode  : offset : bit"
                  );
        for(i = 0; i < header.num_channels; i++)
        {
            y_off += y_line;
            bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                        , "%2d: %s : %c%s : %s :   %4d : %3d"
                        , i
                        , ((header.signal_id[i] >= 0) ? ("   ") : ("neg"))
                        , (  (    (i == 2 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                               || (i == 3 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                               || (i == 4 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                               || (i == 5 && header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
                             )
                           ? ('*')
                           : (' ')
                          )
                        , LaserBoy_wave_signal_id_to_name(header.signal_id[i]).c_str()
                        , LaserBoy_wave_LSB_tag_to_name(header.LSB_tag[i]).c_str()
                        , header.offset[i]
                        , header.resolution[i]
                      );
        }
    }
    if(    (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
        || (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
      )
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "* color signals are rescaled"
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] to continue"
              );
    return true;
}

//############################################################################
short LaserBoy_TUI::overlay_wave_signal_polarities(string file_name, int polarity[8])
{
    int            i,
                   y_line       = 8 * font_size_factor,
                   x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                   y_off        = x_off;
    u_int          bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    std::fstream   in;
    //------------------------------------------------------------------------
    in.open(file_name.c_str(), ios::in | ios::binary);
    LaserBoy_wave_header header(in);
    in.close();
    //------------------------------------------------------------------------
    TUI_palette_index = 61;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave stats                                          "
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , file_name.c_str()
              );
    if(header.version != "!LaserBoy!")
    {
        y_off += y_line;
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , ((header.LaserBoy_wave_mode & LASERBOY_WAVE_POSITIVE) ? ("wave polarity ........: positive signal") : ("wave polarity ........: negative signal"))
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "ch: sig : contents"
              );
    for(i = 0; i < header.num_channels; i++)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "%2d: %s : %s"
                    , i
                    , ((header.signal_id[i] >= 0) ? ("   ") : ("neg"))
                    , LaserBoy_wave_signal_id_to_name(header.signal_id[i]).c_str()
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave signal polarity matrix"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [0] X          %s"
                , ((polarity[0] == -1) ? ("") : ((polarity[0]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [1] Y          %s"
                , ((polarity[1] == -1) ? ("") : ((polarity[1]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [2] red        %s"
                , ((polarity[2] == -1) ? ("") : ((polarity[2]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [3] green      %s"
                , ((polarity[3] == -1) ? ("") : ((polarity[3]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [4] blue       %s"
                , ((polarity[4] == -1) ? ("") : ((polarity[4]) ? ("invert") : ("unchanged")))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [5] Z or other %s"
                , ((polarity[5] == -1) ? ("") : ((polarity[5]) ? ("invert") : ("unchanged")))
              );
    if(header.num_channels == 8)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  [6] L audio    %s"
                    , ((polarity[6] == -1) ? ("") : ((polarity[6]) ? ("invert") : ("unchanged")))
                  );
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  [7] R audio    %s"
                    , ((polarity[7] == -1) ? ("") : ((polarity[7]) ? ("invert") : ("unchanged")))
                  );
    }
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    return header.num_channels;
}

//############################################################################
void LaserBoy_TUI::overlay_wave_offsets(u_int i)
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 119;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "wave sample shift offsets"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 0) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [1] X          %d"
                , wave_offsets[0]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 1) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [2] Y          %d"
                , wave_offsets[1]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 2) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [3] red        %d"
                , wave_offsets[2]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 3) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [4] green      %d"
                , wave_offsets[3]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 4) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [5] blue       %d"
                , wave_offsets[4]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 5) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [6] Z or other %d"
                , wave_offsets[5]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 6) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [7] L audio    %d"
                , wave_offsets[6]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off
                , ((i > 7) ? (bmp.make_rgb(255, 0, 0)) : (next_hue()))
                , bmp_bg_color
                , transparent_menu_font
                , font_size_factor
                , "  [8] R audio    %d"
                , wave_offsets[7]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_color_rescale_stats()
{
    int    y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off;
    u_int  bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 43;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "color rescales"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        red      %s"
                , ((color_rescale_file[0] == ".") ? ("(no file)") : (color_rescale_file[0].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        green    %s"
                , ((color_rescale_file[1] == ".") ? ("(no file)") : (color_rescale_file[1].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        blue     %s"
                , ((color_rescale_file[2] == ".") ? ("(no file)") : (color_rescale_file[2].c_str()))
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "        auxilary %s"
                , ((color_rescale_file[3] == ".") ? ("(no file)") : (color_rescale_file[3].c_str()))
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Enter] to skip"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "      . to clear"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  [Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::overlay_bit_resolution_settings()
{
    u_int  y_line       = 8 * font_size_factor,
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off        = x_off,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    //------------------------------------------------------------------------
    TUI_palette_index = 244;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "signal bit mask settings"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "  set    chanel    bits"
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   a  [1]       X  %d"
                , 16 - signal_bit_mask[0]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   b  [2]       Y  %d"
                , 16 - signal_bit_mask[1]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   c  [3]       Z  %d"
                , 16 - signal_bit_mask[5]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   d  [4]     red  %d"
                , 16 - signal_bit_mask[2]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   e  [5]   green  %d"
                , 16 - signal_bit_mask[3]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   f  [6]    blue  %d"
                , 16 - signal_bit_mask[4]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   g  [7] L-audio  %d"
                , 16 - signal_bit_mask[6]
              );
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "   h  [8] R-audio  %d"
                , 16 - signal_bit_mask[7]
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                , " i I   impose bit masks to frame | frame_set"
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "[Esc] cancel"
              );
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::get_file_list(string dir, string ext)
{
    working_directory = dir;
    current_extension = ext;
    //------------------------------------------------------------------------
    if(!exists(dir))
        return;
    working_directory_list.clear();
    directory_iterator end_itr;
    for(directory_iterator itr(dir); itr != end_itr; itr++)
    {
        if(    is_regular_file(working_directory + itr->path().filename().string())
            && iends_with(itr->path().filename().string(), current_extension)
          )
            working_directory_list.push_back(*itr);
    }
    if(working_directory_list.size()) // ext4 does not return the list in alphanumeric order!
        sort(working_directory_list.begin(), working_directory_list.end());
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_TUI::get_subdirectory_list(string dir)
{
    working_directory = dir;
    //------------------------------------------------------------------------
    if(!exists(dir))
        return;
    working_directory_list.clear();
    directory_iterator end_itr;
    for(directory_iterator itr(dir); itr != end_itr; itr++)
    {
        if(is_directory(working_directory + itr->path().filename().string()))
            working_directory_list.push_back(*itr);
    }
    if(working_directory_list.size()) // ext4 does not return the list in alphanumeric order!
        sort(working_directory_list.begin(), working_directory_list.end());
    //------------------------------------------------------------------------
    return;
}

//############################################################################
int LaserBoy_TUI::overlay_file_list()
{
    bool   character_matches;
    char   c;
    u_int  i,
           j,
           y_line         = 8 * font_size_factor,
           x_off          = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off          = y_line,
           files_shown    = 0,
           shortest_match = 100,
           bmp_bg_color   = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    vector <string> partial_matches;
    //------------------------------------------------------------------------
    TUI_palette_index = 7;
    bmp_put_rectangle(&bmp, x_off, 0, bmp.xres, bmp.xres, bmp_bg_color, true);
    bmp_printf(&bmp, x_off, 0, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "%s                  ",
               working_directory.c_str()
              );
    //------------------------------------------------------------------------
    for(i = 0; i < working_directory_list.size(); i++)
    {
        if(    is_regular_file(working_directory + working_directory_list[i].path().filename().string())
            && STARTS_WITH(working_directory_list[i].path().filename().string(), file_name)
            && iends_with (working_directory_list[i].path().filename().string(), current_extension)
          )
            partial_matches.push_back(working_directory_list[i].path().filename().string());
    }
    //------------------------------------------------------------------------
    if(partial_matches.size() > 1)
    {
        for(i = 0; i < partial_matches.size(); i++)
        {
            if(shortest_match > partial_matches[i].size())
                shortest_match = partial_matches[i].size();
        }
        for(i = 0; i < shortest_match; i++) // i is the char pos
        {
            character_matches = true;
            c = partial_matches[0][i]; // always the first name in list
            for(j = 1; j < partial_matches.size(); j++) // j is the name in the list
                character_matches = character_matches && (c == partial_matches[j][i]);
            if(!character_matches)
                break;
        }
        file_name = partial_matches[0].substr(0, i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < partial_matches.size(); i++)
    {
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "%s                  "
                    , partial_matches[i].c_str()
                  );
        y_off += y_line;
        files_shown++;
    }
    //------------------------------------------------------------------------
    if(files_shown == 0)
    {
        bmp_printf(  &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                   , "no *%s files found             "
                   , current_extension.c_str()
                  );
        return LASERBOY_PARENT_DIR_EMPTY;
    }
    //------------------------------------------------------------------------
    return files_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_subdirectory_list()
{
    bool   character_matches;
    char   c;
    u_int  i,
           j,
           y_line         = 8 * font_size_factor,
           x_off          = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off          = y_line,
           dirs_shown     = 0,
           shortest_match = 100,
           bmp_bg_color   = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    vector <string> partial_matches;
    //------------------------------------------------------------------------
    TUI_palette_index = 7;
    bmp_put_rectangle(&bmp, x_off, 0, bmp.xres, bmp.xres, bmp_bg_color, true);
    bmp_printf(&bmp, x_off, 0, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "%s                  ",
               working_directory.c_str()
              );
    //------------------------------------------------------------------------
    for(i = 0; i < working_directory_list.size(); i++)
    {
        if(    is_directory(working_directory + working_directory_list[i].path().filename().string())
            && STARTS_WITH(working_directory_list[i].path().filename().string(), working_subdirectory)
          )
            partial_matches.push_back(working_directory_list[i].path().filename().string());
    }
    //------------------------------------------------------------------------
    if(partial_matches.size() > 1)
    {
        for(i = 0; i < partial_matches.size(); i++)
        {
            if(shortest_match > partial_matches[i].size())
                shortest_match = partial_matches[i].size();
        }
        for(i = 0; i < shortest_match; i++) // i is the char pos
        {
            character_matches = true;
            c = partial_matches[0][i]; // always the first name in list
            for(j = 1; j < partial_matches.size(); j++) // j is the name in the list
                character_matches = character_matches && (c == partial_matches[j][i]);
            if(!character_matches)
                break;
        }
        working_subdirectory = partial_matches[0].substr(0, i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < partial_matches.size(); i++)
    {
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%s                  ",
                   partial_matches[i].c_str()
                  );
        y_off += y_line;
        dirs_shown++;
    }
    //------------------------------------------------------------------------
    if(dirs_shown == 0)
    {
        bmp_printf(&bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no directories found                 "
                  );
        return LASERBOY_PARENT_DIR_EMPTY;
    }
    //------------------------------------------------------------------------
    return dirs_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_frame_effects()
{
    bool   character_matches;
    char   c;
    u_int  i,
           j,
           y_line         = 8 * font_size_factor,
           x_off          = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off          = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           effects_shown  = 0,
           shortest_match = 100,
           bmp_bg_color   = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    vector <string> partial_matches;
    //------------------------------------------------------------------------
    TUI_palette_index = 113;
    bmp_printf(&bmp, x_off, 0, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "frame effects             "
              );
    //------------------------------------------------------------------------
    for(i = 0; i < frame_effect_names.size(); i++)
    {
        if(starts_with(frame_effect_names[i], effect_name))
            partial_matches.push_back(frame_effect_names[i]);
    }
    //------------------------------------------------------------------------
    if(partial_matches.size() > 1)
    {
        for(i = 0; i < partial_matches.size(); i++)
        {
            if(shortest_match > partial_matches[i].size())
                shortest_match = partial_matches[i].size();
        }
        for(i = 0; i < shortest_match; i++) // i is the char pos
        {
            character_matches = true;
            c = partial_matches[0][i]; // always the first name in list
            for(j = 1; j < partial_matches.size(); j++) // j is the name in the list
                character_matches = character_matches && (c == partial_matches[j][i]);
            if(!character_matches)
                break;
        }
        effect_name = partial_matches[0].substr(0, i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < partial_matches.size(); i++)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%s                  ",
                   partial_matches[i].c_str()
                  );
        effects_shown++;
    }
    //------------------------------------------------------------------------
    if(effects_shown == 0)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no frame effects loaded"
                  );
    }
    //------------------------------------------------------------------------
    return effects_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_frame_set_effects()
{
    bool   character_matches;
    char   c;
    u_int  i,
           j,
           y_line         = 8 * font_size_factor,
           x_off          = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off          = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           effects_shown  = 0,
           shortest_match = 100,
           bmp_bg_color   = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    vector <string> partial_matches;
    //------------------------------------------------------------------------
    TUI_palette_index = 113;
    bmp_printf(&bmp, x_off, 0, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "frame_set effects             "
              );
    //------------------------------------------------------------------------
    for(i = 0; i < frame_set_effect_names.size(); i++)
    {
        if(starts_with(frame_set_effect_names[i], effect_name))
            partial_matches.push_back(frame_set_effect_names[i]);
    }
    //------------------------------------------------------------------------
    if(partial_matches.size() > 1)
    {
        for(i = 0; i < partial_matches.size(); i++)
        {
            if(shortest_match > partial_matches[i].size())
                shortest_match = partial_matches[i].size();
        }
        for(i = 0; i < shortest_match; i++) // i is the char pos
        {
            character_matches = true;
            c = partial_matches[0][i]; // always the first name in list
            for(j = 1; j < partial_matches.size(); j++) // j is the name in the list
                character_matches = character_matches && (c == partial_matches[j][i]);
            if(!character_matches)
                break;
        }
        effect_name = partial_matches[0].substr(0, i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < partial_matches.size(); i++)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%s                  ",
                   partial_matches[i].c_str()
                  );
        effects_shown++;
    }
    //------------------------------------------------------------------------
    if(effects_shown == 0)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no frame_set effects loaded"
                  );
    }
    //------------------------------------------------------------------------
    return effects_shown;
}

//############################################################################
int LaserBoy_TUI::overlay_selected_frames_effects()
{
    bool   character_matches;
    char   c;
    u_int  i,
           j,
           y_line         = 8 * font_size_factor,
           x_off          = bmp.yres + LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off          = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           effects_shown  = 0,
           shortest_match = 100,
           bmp_bg_color   = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    vector <string> partial_matches;
    //------------------------------------------------------------------------
    TUI_palette_index = 113;
    bmp_printf(&bmp, x_off, 0, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor,
               "selected frames effects             "
              );
    //------------------------------------------------------------------------
    for(i = 0; i < selected_frames_effect_names.size(); i++)
    {
        if(starts_with(selected_frames_effect_names[i], effect_name))
            partial_matches.push_back(selected_frames_effect_names[i]);
    }
    //------------------------------------------------------------------------
    if(partial_matches.size() > 1)
    {
        for(i = 0; i < partial_matches.size(); i++)
        {
            if(shortest_match > partial_matches[i].size())
                shortest_match = partial_matches[i].size();
        }
        for(i = 0; i < shortest_match; i++) // i is the char pos
        {
            character_matches = true;
            c = partial_matches[0][i]; // always the first name in list
            for(j = 1; j < partial_matches.size(); j++) // j is the name in the list
                character_matches = character_matches && (c == partial_matches[j][i]);
            if(!character_matches)
                break;
        }
        effect_name = partial_matches[0].substr(0, i);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < partial_matches.size(); i++)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "%s                  ",
                   partial_matches[i].c_str()
                  );
        effects_shown++;
    }
    //------------------------------------------------------------------------
    if(effects_shown == 0)
    {
        y_off += y_line;
        bmp_printf(&bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor,
                   "no selected frames effects loaded"
                  );
    }
    //------------------------------------------------------------------------
    return effects_shown;
}

//############################################################################
bool LaserBoy_TUI::report_ild_file_open(LaserBoy_frame_set& frame_set, string& file_name, LaserBoy_ild_header_count& counter)
{
    long int       bytes_skipped = 0;
    ostringstream  output_message;
    frame_set.from_ild_file(LASERBOY_ILD_SHARE + working_subdirectory + file_name, bytes_skipped, counter);
    if(frame_set.frame_set_error & LASERBOY_FILE_OPEN_FAILED)
    {
        display_error(LASERBOY_ILD_SHARE + working_subdirectory + file_name + " failed to open");
        p_GUI->wait_4_Esc();
        return false;
    }
    else if(frame_set.frame_set_error & LASERBOY_NO_FRAMES_FOUND)
    {
        if(bytes_skipped == -2)
        {
            clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << working_subdirectory
                           << file_name
                           << " is empty, zero bytes";
            display_error(output_message.str());
        }
        else if(bytes_skipped > 0)
        {
            clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << working_subdirectory
                           << file_name
                           << " : "
                           << bytes_skipped
                           << " bytes no frames found";
            display_error(output_message.str());
        }
        else
            display_error(LASERBOY_ILD_SHARE + working_subdirectory + file_name + " no frames found");
        p_GUI->wait_4_Esc();
        return false;
    }
    else if(frame_set.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
    {
        if(bytes_skipped > 0)
        {
            clean_screen();
            output_message << LASERBOY_ILD_SHARE
                           << working_subdirectory
                           << file_name
                           << " : "
                           << bytes_skipped
                           << " bytes unrecognised data";
            display_error(output_message.str());
        }
        display_error(LASERBOY_ILD_SHARE + working_subdirectory + file_name + " table to frame quantity mismatch");
        p_GUI->wait_4_Esc();
    }
    else if(bytes_skipped > 0)
    {
        clean_screen();
        output_message << LASERBOY_ILD_SHARE
                       << working_subdirectory
                       << file_name
                       << " : "
                       << bytes_skipped
                       << " bytes of unrecognised data";
        display_error(output_message.str());
        p_GUI->wait_4_Esc();
    }
    return true;
}

//############################################################################
bool LaserBoy_TUI::report_ctn_file_open(LaserBoy_frame_set& frame_set, string& file_name)
{
    long int       bytes_skipped = 0;
    ostringstream  output_message;
    frame_set.from_ctn_file(LASERBOY_CTN_SHARE + file_name, bytes_skipped);
    if(frame_set.frame_set_error & LASERBOY_FILE_OPEN_FAILED)
    {
        display_error(LASERBOY_CTN_SHARE + file_name + " failed to open");
        p_GUI->wait_4_Esc();
        return false;
    }
    else if(frame_set.frame_set_error & LASERBOY_NO_FRAMES_FOUND)
    {
        if(bytes_skipped == -2)
        {
            clean_screen();
            output_message << LASERBOY_CTN_SHARE
                           << file_name
                           << " is empty, zero bytes";
            display_error(output_message.str());
        }
        else if(bytes_skipped > 0)
        {
            clean_screen();
            output_message << LASERBOY_CTN_SHARE
                           << file_name
                           << " : "
                           << bytes_skipped
                           << " bytes no frames found";
            display_error(output_message.str());
        }
        else
            display_error(LASERBOY_CTN_SHARE + file_name + " no frames found");
        p_GUI->wait_4_Esc();
        return false;
    }
    else if(bytes_skipped > 0)
    {
        clean_screen();
        output_message << LASERBOY_CTN_SHARE
                       << file_name
                       << " : "
                       << bytes_skipped
                       << " bytes of unrecognised data";
        display_error(output_message.str());
        p_GUI->wait_4_Esc();
    }
    else if(frame_set.frame_set_error & LASERBOY_QUANTITY_DATA_MISSMATCH)
    {
        clean_screen();
        output_message << LASERBOY_CTN_SHARE
                       << file_name
                       << " : quatity data mistmatch";
        display_error(output_message.str());
        p_GUI->wait_4_Esc();
    }
    return true;
}

//############################################################################
bool LaserBoy_TUI::report_wtf_changes()
{
                  bool changed      = false;
                 u_int y_line       = 8 * font_size_factor,
                       x_off        = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
                       y_off        = x_off,
                       bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);
    LaserBoy_settings  wtf;
    wtf.load_wtf_file(LASERBOY_WTF_SHARE + "LaserBoy.wtf");
    //------------------------------------------------------------------------
    bmp_printf(   &bmp, x_off, y_off, white_text.as_uint(), bmp_bg_color, transparent_menu_font, font_size_factor
                , "settings compared to LaserBoy.wtf"
              );
    y_off += y_line;
    //------------------------------------------------------------------------
    if(bg_file_name != wtf.bg_file_name)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  bg_file_name             was %s : is %s"
                    , wtf.bg_file_name.c_str()
                    ,     bg_file_name.c_str()
                  );
    }
    //------------------------------------------------------------------------
    if(bg_color != wtf.bg_color)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  bg_color                 was %3d %3d %3d : is %3d %3d %3d"
                    , wtf.bg_color.r
                    , wtf.bg_color.g
                    , wtf.bg_color.b
                    ,     bg_color.r
                    ,     bg_color.g
                    ,     bg_color.b
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(mono_text != wtf.mono_text)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  mono_text                was %3d %3d %3d : is %3d %3d %3d"
                    , wtf.mono_text.r
                    , wtf.mono_text.g
                    , wtf.mono_text.b
                    ,     mono_text.r
                    ,     mono_text.g
                    ,     mono_text.b
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(rendered_blank != wtf.rendered_blank)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_blank           was %3d %3d %3d : is %3d %3d %3d"
                    , wtf.rendered_blank.r
                    , wtf.rendered_blank.g
                    , wtf.rendered_blank.b
                    ,     rendered_blank.r
                    ,     rendered_blank.g
                    ,     rendered_blank.b
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(rendered_black != wtf.rendered_black)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_black           was %3d %3d %3d : is %3d %3d %3d"
                    , wtf.rendered_black.r
                    , wtf.rendered_black.g
                    , wtf.rendered_black.b
                    ,     rendered_black.r
                    ,     rendered_black.g
                    ,     rendered_black.b
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(sample_rate != wtf.sample_rate)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  sample_rate              was %d : is %d"
                    , wtf.sample_rate
                    ,     sample_rate
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(lit_dwell_overhang != wtf.lit_dwell_overhang)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  lit_dwell_overhang       was %d : is %d"
                    , wtf.lit_dwell_overhang
                    ,     lit_dwell_overhang
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(dwell_on_dot != wtf.dwell_on_dot)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  dwell_on_dot             was %d : is %d"
                    , wtf.dwell_on_dot
                    ,     dwell_on_dot
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(font_size_factor != wtf.font_size_factor)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  font_size_factor         was %d : is %d"
                    , wtf.font_size_factor
                    ,     font_size_factor
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(rendered_line_width != wtf.rendered_line_width)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_line_width      was %d : is %d"
                    , wtf.rendered_line_width
                    ,     rendered_line_width
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(background_bmp_scale - wtf.background_bmp_scale) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  background_bmp_scale     was %3.2f : is %3.2f"
                    , wtf.background_bmp_scale
                    ,     background_bmp_scale
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(displacement_step != wtf.displacement_step)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  displacement_step        was %d : is %d"
                    , wtf.displacement_step
                    ,     displacement_step
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(color_frame_shift != wtf.color_frame_shift)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  color_frame_shift        was %d : is %d"
                    , wtf.color_frame_shift
                    ,     color_frame_shift
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(channels_of_wav_out != wtf.channels_of_wav_out)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  channels_of_wav_out      was %d : is %d"
                    , wtf.channels_of_wav_out
                    ,     channels_of_wav_out
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(output_bmp_size != wtf.output_bmp_size)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  output_bmp_size          was %d : is %d"
                    , wtf.output_bmp_size
                    ,     output_bmp_size
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(transparent_menu_font != wtf.transparent_menu_font)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  transparent_menu_font    was %s : is %s"
                    , ((wtf.transparent_menu_font) ? ("on ") : ("off"))
                    , ((    transparent_menu_font) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_if_not_z != wtf.signal_if_not_z)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_if_not_z          was %d : is %d"
                    , wtf.signal_if_not_z
                    ,     signal_if_not_z
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(unframed_wave_stride != wtf.unframed_wave_stride)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  unframed_wave_stride     was %d : is %d"
                    , wtf.unframed_wave_stride
                    ,     unframed_wave_stride
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if((fulcrum - wtf.fulcrum).magnitude() > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  fulcrum                  was %f %f %f : is %f %f %f"
                    , wtf.fulcrum.x
                    , wtf.fulcrum.y
                    , wtf.fulcrum.z
                    ,     fulcrum.x
                    ,     fulcrum.y
                    ,     fulcrum.z
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if((view_angle - wtf.view_angle).magnitude() > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  view_angle               was %f %f %f : is %f %f %f"
                    , wtf.view_angle.x
                    , wtf.view_angle.y
                    , wtf.view_angle.z
                    ,     view_angle.x
                    ,     view_angle.y
                    ,     view_angle.z
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if((view_scale - wtf.view_scale).magnitude() > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  view_scale               was %f %f : is %f %f"
                    , wtf.view_scale.x
                    , wtf.view_scale.y
                    ,     view_scale.x
                    ,     view_scale.y
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if((view_offset - wtf.view_offset).magnitude() > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  view_offset              was %f %f : is %f %f"
                    , wtf.view_offset.x
                    , wtf.view_offset.y
                    ,     view_offset.x
                    ,     view_offset.y
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(lit_delta_max - wtf.lit_delta_max) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  lit_delta_max            was %f : is %f"
                    , wtf.lit_delta_max
                    ,     lit_delta_max
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(blank_delta_max - wtf.blank_delta_max) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  blank_delta_max          was %f : is %f"
                    , wtf.blank_delta_max
                    ,     blank_delta_max
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(max_dwell_millisec - wtf.max_dwell_millisec) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  max_dwell_millisec       was %f : is %f"
                    , wtf.max_dwell_millisec
                    ,    max_dwell_millisec
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(max_wag_microsec - wtf.max_wag_microsec) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  max_wag_microsec         was %f : is %f"
                    , wtf.max_wag_microsec
                    ,     max_wag_microsec
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(insignificant_distance - wtf.insignificant_distance) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  insignificant_distance   was %f : is %f"
                    , wtf.insignificant_distance
                    ,     insignificant_distance
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(insignificant_angle - wtf.insignificant_angle) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  insignificant_angle      was %f : is %f"
                    , wtf.insignificant_angle
                    ,     insignificant_angle
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(recolor_span_factor - wtf.recolor_span_factor) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  recolor_span_factor      was %f : is %f"
                    , wtf.recolor_span_factor
                    ,     recolor_span_factor
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(recolor_index_multiple != wtf.recolor_index_multiple)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  recolor_index_multiple was %u : is %u"
                    , wtf.recolor_index_multiple
                    ,     recolor_index_multiple
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(onion_skin_fade - wtf.onion_skin_fade) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  onion_skin_fade          was %f : is %f"
                    , wtf.onion_skin_fade
                    ,     onion_skin_fade
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(rendered_arc_angle - wtf.rendered_arc_angle) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_arc_angle       was %f : is %f"
                    , wtf.rendered_arc_angle
                    ,     rendered_arc_angle
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(rendered_mono_font_space - wtf.rendered_mono_font_space) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_mono_font_space was %f : is %f"
                    , wtf.rendered_mono_font_space
                    ,     rendered_mono_font_space
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(rendered_vari_font_gap - wtf.rendered_vari_font_gap) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_vari_font_gap   was %f : is %f"
                    , wtf.rendered_vari_font_gap
                    ,     rendered_vari_font_gap
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(rendered_vari_font_space - wtf.rendered_vari_font_space) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rendered_vari_font_space was %f : is %f"
                    , wtf.rendered_vari_font_space
                    ,     rendered_vari_font_space
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(rotation_step - wtf.rotation_step) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rotation_step            was %f : is %f"
                    , wtf.rotation_step
                    ,     rotation_step
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(magnitude_step - wtf.magnitude_step) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  magnitude_step           was %f : is %f"
                    , wtf.magnitude_step
                    ,     magnitude_step
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(frames_per_second - wtf.frames_per_second) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  frames_per_second        was %f : is %f"
                    , wtf.frames_per_second
                    ,     frames_per_second
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(abs(still_frame_duration - wtf.still_frame_duration) > LASERBOY_EPSILON)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  still_frame_duration     was %f : is %f"
                    , wtf.still_frame_duration
                    ,     still_frame_duration
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(rainbow_menus != wtf.rainbow_menus)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  rainbow_menus            was %s : is %s"
                    , ((wtf.rainbow_menus) ? ("on ") : ("off"))
                    , ((    rainbow_menus) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(alt_menu_c != wtf.alt_menu_c)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  alt_menu_c               was %s : is %s"
                    , ((wtf.alt_menu_c) ? ("on ") : ("off"))
                    , ((    alt_menu_c) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(alt_menu_h != wtf.alt_menu_h)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  alt_menu_h               was %s : is %s"
                    , ((wtf.alt_menu_h) ? ("on ") : ("off"))
                    , ((    alt_menu_h) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(alt_menu_k != wtf.alt_menu_k)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  alt_menu_k               was %s : is %s"
                    , ((wtf.alt_menu_k) ? ("on ") : ("off"))
                    , ((    alt_menu_k) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(alt_menu_l != wtf.alt_menu_l)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  alt_menu_l               was %s : is %s"
                    , ((wtf.alt_menu_l) ? ("on ") : ("off"))
                    , ((    alt_menu_l) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_vectors != wtf.show_vectors)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_vectors             was %s : is %s"
                    , ((wtf.show_vectors) ? ("on ") : ("off"))
                    , ((    show_vectors) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_vertices != wtf.show_vertices)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_vertices            was %s : is %s"
                    , ((wtf.show_vertices) ? ("on ") : ("off"))
                    , ((    show_vertices) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_blanking != wtf.show_blanking)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_blanking            was %s : is %s"
                    , ((wtf.show_blanking) ? ("on ") : ("off"))
                    , ((    show_blanking) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_intro != wtf.show_intro)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_intro               was %s : is %s"
                    , ((wtf.show_intro) ? ("on ") : ("off"))
                    , ((    show_intro) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_bridge != wtf.show_bridge)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_bridge              was %s : is %s"
                    , ((wtf.show_bridge) ? ("on ") : ("off"))
                    , ((    show_bridge) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_coda != wtf.show_coda)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_coda                was %s : is %s"
                    , ((wtf.show_coda) ? ("on ") : ("off"))
                    , ((    show_coda) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_direction != wtf.show_direction)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_direction           was %s : is %s"
                    , ((wtf.show_direction) ? ("on ") : ("off"))
                    , ((    show_direction) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_direction != wtf.show_direction)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_direction           was %s : is %s"
                    , ((wtf.show_direction) ? ("on ") : ("off"))
                    , ((    show_direction) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_stats != wtf.show_stats)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_stats               was %s : is %s"
                    , ((wtf.show_stats) ? ("on ") : ("off"))
                    , ((    show_stats) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(track_bmp_view != wtf.track_bmp_view)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  track_bmp_view           was %s : is %s"
                    , ((wtf.track_bmp_view) ? ("on ") : ("off"))
                    , ((    track_bmp_view) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_fixed_bounds != wtf.show_fixed_bounds)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_fixed_bounds        was %s : is %s"
                    , ((wtf.show_fixed_bounds) ? ("on ") : ("off"))
                    , ((    show_fixed_bounds) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_fixed_origin != wtf.show_fixed_origin)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_fixed_origin        was %s : is %s"
                    , ((wtf.show_fixed_origin) ? ("on ") : ("off"))
                    , ((    show_fixed_origin) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_floating_axis != wtf.show_floating_axis)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_floating_axis       was %s : is %s"
                    , ((wtf.show_floating_axis) ? ("on ") : ("off"))
                    , ((    show_floating_axis) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_floating_bounds != wtf.show_floating_bounds)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_floating_bounds     was %s : is %s"
                    , ((wtf.show_floating_bounds) ? ("on ") : ("off"))
                    , ((    show_floating_bounds) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(infinite_vector != wtf.infinite_vector)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  infinite_vector          was %s : is %s"
                    , ((wtf.infinite_vector) ? ("on ") : ("off"))
                    , ((    infinite_vector) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(destructive_clipping != wtf.destructive_clipping)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  destructive_clipping     was %s : is %s"
                    , ((wtf.destructive_clipping) ? ("on ") : ("off"))
                    , ((    destructive_clipping) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_onion_skin != wtf.show_onion_skin)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_onion_skin          was %s : is %s"
                    , ((wtf.show_onion_skin) ? ("on ") : ("off"))
                    , ((    show_onion_skin) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_onion_visuals != wtf.show_onion_visuals)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_onion_visuals       was %s : is %s"
                    , ((wtf.show_onion_visuals) ? ("on ") : ("off"))
                    , ((    show_onion_visuals) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_indices != wtf.show_indices)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_indices             was %s : is %s"
                    , ((wtf.show_indices) ? ("on ") : ("off"))
                    , ((    show_indices) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_effects_generation != wtf.show_effects_generation)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_effects_generation  was %s : is %s"
                    , ((wtf.show_effects_generation) ? ("on ") : ("off"))
                    , ((    show_effects_generation) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_Z_as_order != wtf.show_Z_as_order)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_Z_as_order          was %s : is %s"
                    , ((wtf.show_Z_as_order) ? ("on ") : ("off"))
                    , ((    show_Z_as_order) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_quad_view != wtf.show_quad_view)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_quad_view           was %s : is %s"
                    , ((wtf.show_quad_view) ? ("on ") : ("off"))
                    , ((    show_quad_view) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(show_cursor_limits_msg != wtf.show_cursor_limits_msg)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  show_cursor_limits_msg   was %s : is %s"
                    , ((wtf.show_cursor_limits_msg) ? ("on ") : ("off"))
                    , ((    show_cursor_limits_msg) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_scale_dxf != wtf.auto_scale_dxf)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_scale_dxf           was %s : is %s"
                    , ((wtf.auto_scale_dxf) ? ("on ") : ("off"))
                    , ((    auto_scale_dxf) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(maintain_real_origin != wtf.maintain_real_origin)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  maintain_real_origin     was %s : is %s"
                    , ((wtf.maintain_real_origin) ? ("on ") : ("off"))
                    , ((    maintain_real_origin) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_true_color_dxf != wtf.save_true_color_dxf)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_true_color_dxf      was %s : is %s"
                    , ((wtf.save_true_color_dxf) ? ("on ") : ("off"))
                    , ((    save_true_color_dxf) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(loop_animation != wtf.loop_animation)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  loop_animation           was %s : is %s"
                    , ((wtf.loop_animation) ? ("on ") : ("off"))
                    , ((    loop_animation) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(approximate_frame_rate != wtf.approximate_frame_rate)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  approximate_frame_rate   was %s : is %s"
                    , ((wtf.approximate_frame_rate) ? ("on ") : ("off"))
                    , ((    approximate_frame_rate) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(black_dwell_vertices != wtf.black_dwell_vertices)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  black_dwell_vertices     was %s : is %s"
                    , ((wtf.black_dwell_vertices) ? ("on ") : ("off"))
                    , ((    black_dwell_vertices) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_flatten_z != wtf.auto_flatten_z)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_flatten_z           was %s : is %s"
                    , ((wtf.auto_flatten_z) ? ("on ") : ("off"))
                    , ((    auto_flatten_z) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(z_from_unformatted != wtf.z_from_unformatted)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  z_from_unformatted       was %s : is %s"
                    , ((wtf.z_from_unformatted) ? ("on ") : ("off"))
                    , ((    z_from_unformatted) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(no_equivalent_vectors != wtf.no_equivalent_vectors)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  no_equivalent_vectors    was %s : is %s"
                    , ((wtf.no_equivalent_vectors) ? ("on ") : ("off"))
                    , ((    no_equivalent_vectors) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(conglomerate_in_minimize != wtf.conglomerate_in_minimize)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  conglomerate_in_minimize was %s : is %s"
                    , ((wtf.conglomerate_in_minimize) ? ("on ") : ("off"))
                    , ((    conglomerate_in_minimize) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(fracture_b4_conglomerate != wtf.fracture_b4_conglomerate)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  fracture_b4_conglomerate was %s : is %s"
                    , ((wtf.fracture_b4_conglomerate) ? ("on ") : ("off"))
                    , ((    fracture_b4_conglomerate) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(dots_setting != wtf.dots_setting)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  dots_setting             was %u : is %u"
                    , wtf.dots_setting
                    ,     dots_setting
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(invert_wave_output != wtf.invert_wave_output)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  invert_wave_output       was %s : is %s"
                    , ((wtf.invert_wave_output) ? ("on ") : ("off"))
                    , ((    invert_wave_output) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_minimize != wtf.auto_minimize)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_minimize            was %s : is %s"
                    , ((wtf.auto_minimize) ? ("on ") : ("off"))
                    , ((    auto_minimize) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_opti_ctn != wtf.auto_opti_ctn)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_opti_ctn            was %s : is %s"
                    , ((wtf.auto_opti_ctn) ? ("on ") : ("off"))
                    , ((    auto_opti_ctn) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_apply_offsets != wtf.auto_apply_offsets)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_apply_offsets       was %s : is %s"
                    , ((wtf.auto_apply_offsets) ? ("on ") : ("off"))
                    , ((    auto_apply_offsets) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(auto_apply_rescales != wtf.auto_apply_rescales)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  auto_apply_rescales      was %s : is %s"
                    , ((wtf.auto_apply_rescales) ? ("on ") : ("off"))
                    , ((    auto_apply_rescales) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(no_black_match != wtf.no_black_match)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  no_black_match           was %s : is %s"
                    , ((wtf.no_black_match) ? ("on ") : ("off"))
                    , ((    no_black_match) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(allow_lit_black != wtf.allow_lit_black)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  allow_lit_black          was %s : is %s"
                    , ((wtf.allow_lit_black) ? ("on ") : ("off"))
                    , ((    allow_lit_black) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_2D_as_3D != wtf.save_2D_as_3D)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_2D_as_3D            was %s : is %s"
                    , ((wtf.save_2D_as_3D) ? ("on ") : ("off"))
                    , ((    save_2D_as_3D) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_1_frame_bridge != wtf.save_1_frame_bridge)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_1_frame_bridge      was %s : is %s"
                    , ((wtf.save_1_frame_bridge) ? ("on ") : ("off"))
                    , ((    save_1_frame_bridge) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_ild_fmt_4_5 != wtf.save_ild_fmt_4_5)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_ild_fmt_4_5         was %s : is %s"
                    , ((wtf.save_ild_fmt_4_5) ? ("on ") : ("off"))
                    , ((    save_ild_fmt_4_5) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_txt_with_color != wtf.save_txt_with_color)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_txt_with_color      was %s : is %s"
                    , ((wtf.save_txt_with_color) ? ("on ") : ("off"))
                    , ((    save_txt_with_color) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_txt_unit != wtf.save_txt_unit)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_txt_unit            was %s : is %s"
                    , ((wtf.save_txt_unit) ? ("on ") : ("off"))
                    , ((    save_txt_unit) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_txt_color_hex != wtf.save_txt_color_hex)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_txt_color_hex       was %s : is %s"
                    , ((wtf.save_txt_color_hex) ? ("on ") : ("off"))
                    , ((    save_txt_color_hex) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_txt_named_palettes != wtf.save_txt_named_palettes)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_txt_named_palettes  was %s : is %s"
                    , ((wtf.save_txt_named_palettes) ? ("on ") : ("off"))
                    , ((    save_txt_named_palettes) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(save_fmtd_wav_splits != wtf.save_fmtd_wav_splits)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  save_fmtd_wav_splits     was %s : is %s"
                    , ((wtf.save_fmtd_wav_splits) ? ("on ") : ("off"))
                    , ((    save_fmtd_wav_splits) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(bond_word != wtf.bond_word)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  bond_word                was %s : is %s"
                    , ((wtf.bond_word) ? ("on ") : ("off"))
                    , ((    bond_word) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_show_vertices != wtf.wave_show_vertices)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_show_vertices       was %s : is %s"
                    , ((wtf.wave_show_vertices) ? ("on ") : ("off"))
                    , ((    wave_show_vertices) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_show_blanks != wtf.wave_show_blanks)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_show_blanks         was %s : is %s"
                    , ((wtf.wave_show_blanks) ? ("on ") : ("off"))
                    , ((    wave_show_blanks) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_real_time != wtf.wave_real_time)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_real_time           was %s : is %s"
                    , ((wtf.wave_real_time) ? ("on ") : ("off"))
                    , ((    wave_real_time) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_show_inverted != wtf.wave_show_inverted)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_show_inverted       was %s : is %s"
                    , ((wtf.wave_show_inverted) ? ("on ") : ("off"))
                    , ((    wave_show_inverted) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_clean_screen != wtf.wave_clean_screen)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_clean_screen        was %s : is %s"
                    , ((wtf.wave_clean_screen) ? ("on ") : ("off"))
                    , ((    wave_clean_screen) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_loop != wtf.wave_loop)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_loop                was %s : is %s"
                    , ((wtf.wave_loop) ? ("on ") : ("off"))
                    , ((    wave_loop) ? ("on ") : ("off"))
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[0] != wtf.wave_offsets[0])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[0]          was %d : is %d"
                    , wtf.wave_offsets[0]
                    ,     wave_offsets[0]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[1] != wtf.wave_offsets[1])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[1]          was %d : is %d"
                    , wtf.wave_offsets[1]
                    ,     wave_offsets[1]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[2] != wtf.wave_offsets[2])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[2]          was %d : is %d"
                    , wtf.wave_offsets[2]
                    ,     wave_offsets[2]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[3] != wtf.wave_offsets[3])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[3]          was %d : is %d"
                    , wtf.wave_offsets[3]
                    ,     wave_offsets[3]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[4] != wtf.wave_offsets[4])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[4]          was %d : is %d"
                    , wtf.wave_offsets[4]
                    ,     wave_offsets[4]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[5] != wtf.wave_offsets[5])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[5]          was %d : is %d"
                    , wtf.wave_offsets[5]
                    ,     wave_offsets[5]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[6] != wtf.wave_offsets[6])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[6]          was %d : is %d"
                    , wtf.wave_offsets[6]
                    ,     wave_offsets[6]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(wave_offsets[7] != wtf.wave_offsets[7])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  wave_offsets[7]          was %d : is %d"
                    , wtf.wave_offsets[7]
                    ,     wave_offsets[7]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(black_level != wtf.black_level)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  black_level              was %d : is %d"
                    , wtf.black_level
                    ,     black_level
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(number_of_skins != wtf.number_of_skins)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  number_of_skins          was %d : is %d"
                    , wtf.number_of_skins
                    ,     number_of_skins
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[0] != wtf.signal_bit_mask[0])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[1]       was %d : is %d"
                    , wtf.signal_bit_mask[0]
                    ,     signal_bit_mask[0]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[1] != wtf.signal_bit_mask[1])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[2]       was %d : is %d"
                    , wtf.signal_bit_mask[1]
                    ,     signal_bit_mask[1]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[2] != wtf.signal_bit_mask[2])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[3]       was %d : is %d"
                    , wtf.signal_bit_mask[2]
                    ,     signal_bit_mask[2]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[3] != wtf.signal_bit_mask[3])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[4]       was %d : is %d"
                    , wtf.signal_bit_mask[3]
                    ,     signal_bit_mask[3]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[4] != wtf.signal_bit_mask[4])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[5]       was %d : is %d"
                    , wtf.signal_bit_mask[4]
                    ,     signal_bit_mask[4]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[5] != wtf.signal_bit_mask[5])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[6]       was %d : is %d"
                    , wtf.signal_bit_mask[5]
                    ,     signal_bit_mask[5]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[6] != wtf.signal_bit_mask[6])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[7]       was %d : is %d"
                    , wtf.signal_bit_mask[6]
                    ,     signal_bit_mask[6]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(signal_bit_mask[7] != wtf.signal_bit_mask[7])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  signal_bit_mask[8]       was %d : is %d"
                    , wtf.signal_bit_mask[7]
                    ,     signal_bit_mask[7]
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(color_rescale_file[0] != wtf.color_rescale_file[0])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  color_rescale_file[0]    was %s : is %s"
                    , wtf.color_rescale_file[0].c_str()
                    ,     color_rescale_file[0].c_str()
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(color_rescale_file[1] != wtf.color_rescale_file[1])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  color_rescale_file[1]    was %s : is %s"
                    , wtf.color_rescale_file[1].c_str()
                    ,     color_rescale_file[1].c_str()
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(color_rescale_file[2] != wtf.color_rescale_file[2])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  color_rescale_file[2]    was %s : is %s"
                    , wtf.color_rescale_file[2].c_str()
                    ,     color_rescale_file[2].c_str()
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(color_rescale_file[3] != wtf.color_rescale_file[3])
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  color_rescale_file[3]    was %s : is %s"
                    , wtf.color_rescale_file[3].c_str()
                    ,     color_rescale_file[3].c_str()
                  );
        changed = true;
    }
    //------------------------------------------------------------------------
    if(!changed)
    {
        y_off += y_line;
        bmp_printf(   &bmp, x_off, y_off, next_hue(), bmp_bg_color, transparent_menu_font, font_size_factor
                    , "  current settings are identicel to LaserBoy.wtf"
                  );
    }
    //------------------------------------------------------------------------
    return changed;
}

//############################################################################
void LaserBoy_TUI::display_error(const string& error)
{
    clean_screen();
    TUI_clue = "oops!";
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // top of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor + 8 * font_size_factor
                , white_text.as_uint()
                , bmp.make_rgb(192, 0, 0)
                , 0
                , font_size_factor
                , "%s                                                                                                                                                   "
                , error.c_str()
              );
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // bottom of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , bmp.yres - 24 * font_size_factor
                , white_text.as_uint()
                , bmp.make_rgb(192, 0, 0)
                , 0
                , font_size_factor
                , "[Esc] key to continue                                                                                                                                                          "
              );
    //------------------------------------------------------------------------
    p_GUI->display_space();
    return;
}

//############################################################################
void LaserBoy_TUI::display_message(const string& message)
{
    clean_screen();
    TUI_clue = "Notice!";
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // top of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor + 16 * font_size_factor
                , bmp.make_rgb(255, 255, 0)
                , bmp.make_rgb(0, 0, 192)
                , 0
                , font_size_factor
                , "%s                                                                                                                                                   "
                , message.c_str()
              );
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // bottom of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , bmp.yres - 24 * font_size_factor
                , bmp.make_rgb(255, 255, 0)
                , bmp.make_rgb(0, 0, 192)
                , 0
                , font_size_factor
                , "any key to continue                                                                                                                                                          "
              );
    //------------------------------------------------------------------------
    p_GUI->display_space();
    return;
}

//############################################################################
void LaserBoy_TUI::display_please_wait()
{
    clean_screen();
    TUI_clue = "Hang on!";
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // top of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor + 16 * font_size_factor
                , bmp.make_rgb(255, 255, 0)
                , bmp.make_rgb(0, 0, 192)
                , 0
                , font_size_factor
                , "     please wait...                                                                                                                                                   "
              );
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // bottom of screen
                , LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor
                , bmp.yres - 24 * font_size_factor
                , bmp.make_rgb(255, 255, 0)
                , bmp.make_rgb(0, 0, 192)
                , 0
                , font_size_factor
                , "     please wait...                                                                                                                                                   "
              );
    //------------------------------------------------------------------------
    p_GUI->display_space();
    return;
}

//############################################################################
void LaserBoy_TUI::display_ild_file_stats(LaserBoy_ild_header_count counter)
{
    u_int  x_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor,
           y_off  = LASERBOY_STATS_DISPLAY_OFFSET * font_size_factor + 16 * font_size_factor,
           y_line = 8 * font_size_factor,
           bmp_bg_color = bmp.make_rgb(bg_color.r, bg_color.g, bg_color.b);

    clean_screen();
    TUI_clue = "ild file stats";
    //------------------------------------------------------------------------
    bmp_printf(   &bmp // top of screen
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "ild file stats:                                                                                                                                                   "
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255,  0,  0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  3D frames    format 0 : %u                                                                                                                                                  "
                , counter._0
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255, 127,  0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  2D frames    format 1 : %u                                                                                                                                                  "
                , counter._1
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(255, 255,  0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  palettes     format 2 : %u                                                                                                                                                  "
                , counter._2
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 255,  0)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  color tables format 3 : %u                                                                                                                                                  "
                , counter._3
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0, 127,  255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  3D frames    format 4 : %u                                                                                                                                                  "
                , counter._4
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb(  0,   0,  255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "  2D frames    format 5 : %u                                                                                                                                                  "
                , counter._5
              );
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , (rainbow_menus) ? (bmp.make_rgb( 127,   0,  255)) : (bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b))
                , bmp_bg_color
                , 0
                , font_size_factor
                , "               ILDA_end : %u                                                                                                                                                  "
                , counter._e
              );
    y_off += y_line;
    y_off += y_line;
    bmp_printf(   &bmp
                , x_off
                , y_off
                , white_text.as_uint()
                , bmp_bg_color
                , 0
                , font_size_factor
                , "[Esc] key to continue                                                                                                                                                  "
              );
    //------------------------------------------------------------------------
    p_GUI->display_space();
    return;
}

//############################################################################
void LaserBoy_TUI::display_rescale_file_error(LaserBoy_Rescale_Error_Code error)
{
    switch(error)
    {
        case LASERBOY_RESCALE_INCOMPLETE_DATA:
            display_error("Not enough data");
            break;
        case LASERBOY_RESCALE_IMPROPER_DATA:
            display_error("Data out of range");
            break;
        case LASERBOY_RESCALE_ODD_DATA:
            display_error("Data contains odd numbers");
            break;
        case LASERBOY_RESCALE_NONZERO_ZERO:
            display_error("First value is not zero");
            break;
        case LASERBOY_RESCALE_REDUNDANT_DATA:
            display_error("Redundant data elements");
            break;
        case LASERBOY_RESCALE_FILE_FAILED:
            display_error("File open failed");
            break;
    }
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
