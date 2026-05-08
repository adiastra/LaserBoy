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
// LaserBoy_TUI.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_TEXT_USER_INTERFACE_DEFINITIONS__
#define __LASERBOY_TEXT_USER_INTERFACE_DEFINITIONS__

#include "LaserBoy_space.hpp"

//############################################################################
class LaserBoy_TUI : public LaserBoy_space
{
public:
    LaserBoy_TUI(LaserBoy_GUI* gui)
                : LaserBoy_space            (gui),
                  TUI_palette_index         (  0),
                  current_menu_name_bc      (  0),
                  white_text                (LaserBoy_color(255,255,255)),
                  current_menu_name         (   ),
                  TUI_clue                  (   )
    {
        if(rainbow_menus)
        {
            if(bg_color.sum() >= 384)
                white_text = LaserBoy_color(0, 0, 0);
        }
        else
            white_text = mono_text;
        absolute_path_of_LaserBoy = current_path().string();
        absolute_path_of_LaserBoy += "/";
        p_space = this;
    }
    //------------------------------------------------------------------------
   ~LaserBoy_TUI()
    {};
    //------------------------------------------------------------------------
    void    render_space                            ();
    void    overlay_palette                         ();
    void    overlay_target_palette                  ();
    void    overlay_palettes                        ();
    void    overlay_frame_stats                     ();
    void    overlay_frame_set_stats                 ();
    void    overlay_palette_set_stats               ();
    void    overlay_main_menu                       ();
    void    overlay_in_file_type_menu               ();
    void    overlay_file_in_cd_menu                 (string LB_dir);
    void    overlay_in_ild_action_menu              ();
    void    overlay_in_dxf_action_menu              ();
    void    overlay_in_wav_action_menu              ();
    void    overlay_in_txt_action_menu              ();
    void    overlay_in_ctn_action_menu              ();
    void    overlay_in_bmp_action_menu              ();
    void    overlay_wav_channel_i_options           ();
    void    overlay_dots_options                    ();
    void    overlay_out_file_type_menu              ();
    void    overlay_ild_out_cd_menu                 ();
    void    overlay_out_ild_menu                    ();
    void    overlay_out_dxf_menu                    ();
    void    overlay_out_wav_menu                    ();
    void    overlay_out_txt_menu                    ();
    void    overlay_out_ctn_menu                    ();
    void    overlay_out_bmp_menu                    ();
    void    overlay_settings_menu                   ();
    void    overlay_optimization_factors_menu       ();
    void    overlay_vector_editor_values_menu       ();
    void    overlay_timing_and_wav_settings_menu    ();
    void    overlay_display_options_menu            ();
    void    overlay_view_menu                       ();
    void    overlay_visuals_menu                    ();
    void    overlay_file_type_specific_options_menu ();
    void    overlay_frame_color_tranz_menu          ();
    void    overlay_palette_set_tranz_menu          ();
    void    overlay_frame_set_tranz_menu            ();
    void    overlay_frame_tranz_menu                ();
    void    overlay_draw_color_blank_menu           ();
    void    overlay_move_scale_rotate_menu          ();
    void    overlay_draw_by_coordinates_menu        ();
    void    overlay_oscillator_stats                (LaserBoy_oscillator lbo,
                                                     string              name,
                                                     u_int               number
                                                    );
    void    overlay_effect_functions_menu           ();
    void    overlay_wav_utilities_menu              ();
    void    overlay_wav_raster_display_menu         ();
    void    overlay_wav_show_settings_menu          ();
    //------------------------------------------------------------------------
    bool    overlay_wav_analysis                    (string file_name);
    void    overlay_LaserBoy_wav_analysis           (std::fstream& in, const LaserBoy_wav_header& header);
    void    overlay_unformatted_wav_analysis        (std::fstream& in, const LaserBoy_wav_header& header);
    bool    overlay_wav_header_stats                (string file_name);
    u_int   overlay_wav_signal_polarities           (string file_name, short wav_signal_id[8]);
    void    overlay_wav_offsets                     (u_int i);
    void    overlay_color_rescale_stats             (bool active);
    void    overlay_bit_resolution_settings         ();
    //------------------------------------------------------------------------
    void    get_file_list                           (string dir = "",
                                                     string ext = ""
                                                    );
    //------------------------------------------------------------------------
    void    get_subdirectory_list                   (string dir = "");
    //------------------------------------------------------------------------
    void    sort_wav_dir                            (size_t &files, size_t &new_dirs);
    //------------------------------------------------------------------------
    int     overlay_file_list                       ();
    int     overlay_subdirectory_list               ();
    int     overlay_frame_effects                   ();
    int     overlay_selected_frames_effects         ();
    int     overlay_frame_set_effects               ();
    //------------------------------------------------------------------------
    bool    report_ild_file_open                    (LaserBoy_frame_set&        frame_set,
                                                     string&                    file_name,
                                                     LaserBoy_ild_header_count& counter
                                                    );
    //------------------------------------------------------------------------
    bool    report_ctn_file_open                    (LaserBoy_frame_set& frame_set,
                                                     string& file_name
                                                    );
    //------------------------------------------------------------------------
    bool    report_wtf_changes                      ();
    //------------------------------------------------------------------------
    void    display_error                           (const string& error  );
    void    display_message                         (const string& message);
    void    display_please_wait                     (                     );
    void    display_ild_file_stats                  (LaserBoy_ild_header_count counter);
    void    display_rescale_file_error              (LaserBoy_Rescale_Error_Code error);
    //------------------------------------------------------------------------
    void    set_current_menu_name                   (string name, int bc, bool erase_clue = false)
    {
        current_menu_name_bc = bc;
        current_menu_name    = name;
        if(erase_clue)
            TUI_clue.erase();
        return;
    }
    //------------------------------------------------------------------------
    LaserBoy_TUI& operator = (const LaserBoy_frame_set& frames)
    {
        LaserBoy_frame_set::clear();
        LaserBoy_frame_set::reserve(frames.number_of_frames());
        LaserBoy_frame_set::insert (LaserBoy_frame_set::end(),
                                    frames.LaserBoy_frame_set::begin(),
                                    frames.LaserBoy_frame_set::end()
                                   );
        LaserBoy_frame_set::shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    u_int next_hue()
    {
        if(rainbow_menus)
        {
             TUI_palette_index += 12;
             TUI_palette_index %= 254;
             return bmp.make_rgb(palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).r,
                                 palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).g,
                                 palette_picker(LASERBOY_LASERBOY_HUES).at(TUI_palette_index).b
                                );
        }
        else
            return bmp.make_rgb(mono_text.r, mono_text.g, mono_text.b);
    }
    //------------------------------------------------------------------------
public:
    int              TUI_palette_index,
                     current_menu_name_bc;
    LaserBoy_color   white_text;
    string           current_menu_name,
                     TUI_clue;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
