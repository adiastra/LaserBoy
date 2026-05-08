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
// LaserBoy_FLTK_GUI.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_FLTK_GUI_DEFINITIONS__
#define __LASERBOY_FLTK_GUI_DEFINITIONS__

//############################################################################
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
//#include <FL/Fl_JPEG_Image.H>  // Use this header for JPEG images
//#include <FL/Fl_PNG_Image.H>   // Use this header for PNG images
//#include <FL/Fl_BMP_Image.H>   // Use this header for BMP images

#include "LaserBoy_TUI.hpp"

//############################################################################
class LaserBoy_GUI : public Fl_Window
{
public:
    LaserBoy_GUI(int w, int h, const char* title);
    //------------------------------------------------------------------------
   ~LaserBoy_GUI();
    //------------------------------------------------------------------------
    bool is_special(int _key)
    {
        if(    _key == FL_Left
            || _key == FL_Up
            || _key == FL_Right
            || _key == FL_Down
            || _key == FL_BackSpace
            || _key == FL_Tab
            || _key == FL_Enter
            || _key == FL_Escape
            || _key == FL_F
            || _key == FL_Delete
            || (    _key > FL_F
                 && _key < FL_F + 13
               )
          )
            return true;
        return false;
    }
    //------------------------------------------------------------------------
    int handle(int event) override
    {
        if(event == FL_KEYDOWN)
        {
            int e_key = Fl::event_key();
            if(is_special(e_key))
            {
                key     = e_key;
                new_key = true;
                return 1;
            }
            if(Fl::event_length())
            {
                key     = *(Fl::event_text());
                new_key = true;
                return 1;
            }
        } // end if(event == FL_KEYDOWN)
        return Fl_Window::handle(event);
    }
    //------------------------------------------------------------------------
    void draw() override
    {
        fl_draw_image(screen->image, 0, 0, xres, yres);
        return;
    }
    //------------------------------------------------------------------------
    void set_bmp(LaserBoy_bmp* bmp)
    {
        screen = bmp;
        return;
    }
    //------------------------------------------------------------------------
    //  BITMAP MEMORY AND FLTK
    //------------------------------------------------------------------------
    void bmp_to_screen(struct LaserBoy_bmp* bmp)
    {
        set_bmp(bmp);
        redraw();
        Fl::flush();
        return;
    }
    //------------------------------------------------------------------------
    void display_space()
    {
        set_bmp(space->p_bmp());
        redraw();
        Fl::flush();
        return;
    }
    //------------------------------------------------------------------------
    void display_state(const string& state)
    {
        bmp_printf(&(space->bmp),
                   0,
                   0,
                   LaserBoy_color(255, 255,   0).as_uint(),
                   LaserBoy_color(  0,   0,  64).as_uint(),
                   0,
                   space->font_size_factor,
                   "             %s                                                                ", // end string
                   state.c_str()
                  );
        display_space();
        return;
    }
    //------------------------------------------------------------------------
    void display_progress(int countdown)
    {
        if(!(countdown % 10))
        {
            bmp_printf(&(space->bmp),
                       0,
                       0,
                       LaserBoy_color(255, 255,   0).as_uint(),
                       LaserBoy_color(  0,   0,  64).as_uint(),
                       0,
                       space->font_size_factor,
                       "%12d",
                       countdown
                      );
            display_space();
        }
        return;
    }
    //------------------------------------------------------------------------
    void display_txt_line_number(int line_number)
    {
        bmp_printf(&(space->bmp),
                   space->bmp.xres - 8 * space->font_size_factor * 12,
                   0,
                   LaserBoy_color(255, 255,   0).as_uint(),
                   LaserBoy_color(  0,   0,  64).as_uint(),
                   0,
                   space->font_size_factor,
                   "%12d",
                   line_number
                  );
        //----------------------------------------------------
        display_space();
        return;
    }
    //------------------------------------------------------------------------
    void       capture_screen                             ();
    //------------------------------------------------------------------------
    void       display_prompt_file_with_auto_complete     (const string& prompt = "",
                                                           const string& value  = ""
                                                          );
    //------------------------------------------------------------------------
    void       display_prompt_dir_with_auto_complete      (const string& prompt );
    void       display_prompt_f_effect_with_auto_complete (const string& prompt );
    void       display_prompt_sf_effect_with_auto_complete(const string& prompt );
    void       display_prompt_fs_effect_with_auto_complete(const string& prompt );
    string     display_prompt_and_echo_name               (const string& prompt,
                                                           const u_int   max_len = 64
                                                          );
    string     display_prompt_and_echo_string             (const string& prompt );
    string     display_prompt_and_echo_unicode            (const string& prompt );
    //------------------------------------------------------------------------
    string     display_prompt_and_echo_channel_map        (const string& prompt );
    //------------------------------------------------------------------------
    double     display_prompt_and_echo_double             (const string& prompt,
                                                           double        value = 0,
                                                           double        v_max =  DBL_MAX,
                                                           double        v_min = -DBL_MAX
                                                          );
    //------------------------------------------------------------------------
    u_char     display_prompt_and_echo_u_char             (const string& prompt,
                                                           u_char        value = 0
                                                          );
    //------------------------------------------------------------------------
    short      display_prompt_and_echo_short              (const string& prompt,
                                                           short         value = 0
                                                          );
    //------------------------------------------------------------------------
    int        display_prompt_and_echo_int                (const string& prompt,
                                                           int           value = 0
                                                          );
    //------------------------------------------------------------------------
    u_int      display_prompt_and_echo_u_int              (const string& prompt,
                                                           u_int         value = 0,
                                                           u_int         v_max = UINT_MAX
                                                          );
    //------------------------------------------------------------------------
    int        display_prompt_and_echo_nibble             (const string& prompt );
    bool       display_prompt_and_echo_bool               (const string& prompt );
    //------------------------------------------------------------------------
    void       wait_4_Esc                                 ();
    void       wait_4_any_key                             ();
    //------------------------------------------------------------------------
    void       check_for_nav_keys                   (int _key);
    void       check_for_digit_keys                 (int _key);
    void       animate_forward                      ();
    void       animate_reverse                      ();
    void       rotate_forward                       ();
    void       rotate_reverse                       ();
    void       animate_rotate_forward               ();
    void       animate_rotate_reverse               ();
    //------------------------------------------------------------------------
    void       system_settings_menu                 ();
    void       optimization_factors_menu            ();
    void       vector_editor_values_menu            ();
    void       timing_and_wav_settings_menu         ();
    void       display_options_menu                 ();
    void       edit_background_color_menu           ();
    void       edit_mono_text_color_menu            ();
    void       edit_rendered_blank_menu             ();
    void       edit_rendered_black_menu             ();
    void       wav_channel_i_menu                   ();
    void       dots_setting_menu                    ();
    void       bit_resolution_menu                  ();
    void       visuals_settings_menu                ();
    void       file_type_specific_options_menu      ();
    void       apply_view_menu                      ();
    bool       frame_color_trans_menu               ();
    bool       palette_set_trans_menu               ();
    void       add_color_to_target_palette_menu     ();
    void       insert_color_in_target_palette_menu  ();
    void       edit_color_in_target_palette_menu    ();
    void       frame_transforms_menu                ();
    void       frame_set_transforms_menu            ();
    void       draw_color_blank_menu                ();
    void       move_scale_rotate_menu               ();
    void       direct_draw_segments_menu            ();
    void       effect_functions_menu                ();
    void       draw_point_menu                      ();
    void       draw_line_menu                       ();
    void       draw_rectangle_menu                  ();
    void       draw_polygon_menu                    ();
    void       draw_polyline_menu                   ();
    void       draw_polystar_menu                   ();
    void       draw_circular_arc_menu               ();
    void       draw_elliptical_arc_menu             ();
    void       draw_rhodonea_menu                   ();
    void       draw_epicycloid_menu                 ();
    void       draw_epitrochoid_menu                ();
    void       draw_hypotrochoid_menu               ();
    void       draw_hypocycloid_menu                ();
    void       draw_lissajous_menu                  ();
    void       draw_mono_spaced_font_menu           ();
    void       draw_variable_spaced_font_menu       ();
    void       wav_utilities_menu                   ();
    void       wav_show_settings_menu               ();
    void       wav_raster_display_menu              ();
    void       set_wav_offsets_menu                 ();
    void       set_color_rescales_menu              ();
    void       apply_color_rescales_menu            ();
    void       omit_color_rescales_menu             ();
    void       set_signal_polarities_menu           ();
    void       clear_wav_polarities_menu            ();
    void       bit_resolution_to_wav_menu           ();
    void       file_open_menu                       ();
    bool       file_in_cd_menu                      (string LB_folder);
    bool       ild_file_open_menu                   ();
    bool       dxf_file_open_menu                   ();
    bool       wav_file_open_menu                   ();
    bool       wav_qm_open_menu                     (bool append);
    bool       wav_unformatted_open_menu            (bool append);
    bool       wav_LB_headerless_open_menu          (bool append);
    bool       txt_file_open_menu                   ();
    bool       ctn_file_open_menu                   ();
    bool       bmp_file_open_menu                   ();
    void       save_as_file_menu                    ();
    bool       ild_out_cd_menu                      ();
    bool       save_as_ild_menu                     ();
    bool       save_as_dxf_menu                     ();
    bool       save_as_wav_menu                     ();
    bool       save_as_txt_menu                     ();
    bool       save_as_ctn_menu                     ();
    bool       save_as_bmp_menu                     ();
    //------------------------------------------------------------------------
    void       main_menu                            ();
    void       Laserboy_terminating                 ();
    int        start_menu_loop                      ();
    //------------------------------------------------------------------------
    bool       display_bmp_directory                (string bmp_dir, bool scale_to_screen);
    //------------------------------------------------------------------------
    bool       display_wav                          (string wav_file);
    bool       display_unframed_wav                 (string wav_file);
    void       display_LaserBoy_wav(std::fstream& in, const LaserBoy_wav_header& header);
    void       display_unframed_wav(std::fstream& in, LaserBoy_wav_header& header);
    //------------------------------------------------------------------------
    bool           running,
                   prompt_escaped,
                   new_key;
    int            key;
    u_int          xres,
                   yres;
    LaserBoy_TUI  *space;
    LaserBoy_bmp  *screen;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################

/*
FL_BackSpace
FL_Tab
FL_Enter
FL_Pause
FL_Scroll_Lock
FL_Escape
FL_Home
FL_Left
FL_Up
FL_Right
FL_Down
FL_Page_Up
FL_Page_Down
FL_End
FL_Print
FL_Insert
FL_Menu
FL_Num_Lock
FL_KP - One of the keypad numbers; use FL_KP + n for number n.
FL_KP_Enter - The enter key on the keypad.
FL_F - One of the function keys; use FL_F + n for function key n.
FL_Shift_L
FL_Shift_R
FL_Control_L
FL_Control_R
FL_Caps_Lock
FL_Meta_L
FL_Meta_R
FL_Alt_L
FL_Alt_R
FL_Delete
*/

