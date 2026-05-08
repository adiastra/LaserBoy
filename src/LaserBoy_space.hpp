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
// LaserBoy_space.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SPACE_DEFINITIONS__
#define __LASERBOY_SPACE_DEFINITIONS__

//############################################################################
#include "LaserBoy_frame_set.hpp"
#include "LaserBoy_palette_set.hpp"

//############################################################################
class LaserBoy_FLTK_GUI;

//############################################################################
class LaserBoy_settings
{
public:
    LaserBoy_settings()
            : bg_file_name                ("."          ) // string
            , install_GUID                (GUID8char()  ) // string
            , effect_name                 (             ) // string
            , working_directory           (             ) // string
            , working_subdirectory        (             ) // string
            , file_name                   (             ) // string
            , font_name                   ("laserellipse") // string
            , current_extension           (             ) // string
            , bg_color                    (  0,   0,   0) // LaserBoy_color
            , mono_text                   (  0, 128,   0) // LaserBoy_color
            , rendered_bounds             (160, 160, 160) // LaserBoy_color
            , rendered_clipping           (255,   0,   0) // LaserBoy_color
            , rendered_blank              (128, 128, 128) // LaserBoy_color
            , rendered_black              ( 16,  16,  16) // LaserBoy_color
            , fulcrum                     (             ) // LaserBoy_3D_double
            , view_angle                  (             ) // LaserBoy_3D_double
            , view_scale                  (1.0          ) // LaserBoy_3D_double
            , view_offset                 (             ) // LaserBoy_3D_double
            , view_has_changed            (false        ) // bool
            , rainbow_menus               (true         )
            , alt_menu_c                  (false        )
            , alt_menu_h                  (false        )
            , alt_menu_k                  (false        )
            , alt_menu_l                  (false        )
            , show_vectors                (true         )
            , show_vertices               (false        )
            , show_blanking               (false        )
            , show_intro                  (false        )
            , show_bridge                 (false        )
            , show_coda                   (false        )
            , show_cursors                (false        )
            , show_direction              (false        )
            , show_color_cursor           (false        )
            , show_fixed_bounds           (false        )
            , show_fixed_origin           (false        )
            , show_floating_axis          (false        )
            , show_floating_bounds        (true         )
            , infinite_vector             (true         )
            , destructive_clipping        (false        )
            , show_fulcrum                (false        )
            , show_indices                (false        )
            , show_effects_generation     (false        )
            , show_background_bitmap      (true         )
            , background_bmp_negative     (false        )
            , show_onion_skin             (false        )
            , show_onion_visuals          (false        )
            , show_menu                   (true         )
            , show_palette                (false        )
            , show_target_palette         (false        )
            , show_stats                  (true         )
            , show_TUI_clue               (true         )
            , track_bmp_view              (true         )
            , show_frame_stats            (false        )
            , show_frame_set_stats        (false        )
            , show_Z_as_order             (false        )
            , show_quad_view              (false        )
            , show_cursor_limits_msg      (true         )
            , ui_visuals_flip_flop        (true         )
            , loop_animation              (true         )
            , approximate_frame_rate      (true         )
            , wave_show_vertices          (false        )
            , wave_show_blanks            (false        )
            , wave_real_time              (true         )
            , wave_show_inverted          (true         )
            , wave_clean_screen           (true         )
            , wave_loop                   (false        )
            , auto_scale_dxf              (true         )
            , maintain_real_origin        (false        )
            , save_true_color_dxf         (false        )
            , black_dwell_vertices        (false        )
            , auto_flatten_z              (false        )
            , z_from_unformatted          (false        )
            , no_equivalent_vectors       (false        )
            , conglomerate_in_minimize    (false        )
            , fracture_b4_conglomerate    (false        )
            , invert_wave_output          (true         )
            , auto_minimize               (false        )
            , auto_opti_ctn               (false        )
            , auto_apply_offsets          (true         )
            , auto_apply_rescales         (false        )
            , no_black_match              (true         )
            , allow_lit_black             (true         )
            , save_2D_as_3D               (false        )
            , save_1_frame_bridge         (false        )
            , save_ild_fmt_4_5            (false        )
            , save_txt_with_color         (false        )
            , save_txt_unit               (false        )
            , save_txt_color_hex          (false        )
            , save_txt_named_palettes     (false        )
            , save_fmtd_wav_splits        (true         )
            , bond_word                   (false        )
            , black_level                 (0            ) // int
            , number_of_skins             (4            ) // int
            , sample_rate                 (48000        ) // int Hz
            , lit_dwell_overhang          (2            ) // vertices
            , dots_setting                (LASERBOY_DOTS_IGNORE)
            , dwell_on_dot                (10           ) // vertices
            , recolor_index_multiple      (1            ) // color index
            , output_bmp_size             (1000         ) // pixels square
            , transparent_menu_font       (0            )
            , font_size_factor            (1            )
            , rendered_line_width         (1            )
            , signal_if_not_z             (LASERBOY_SIGNAL_MONO_AVG_ANALOG)
            , unframed_wave_stride        (4800         ) // samples 1/10 of sec
            , channels_of_wav_out         (6            )
            , app_runs_count              (1            )
            , displacement_step           (100          ) // int points
            , color_frame_shift           (1            )
            , bmp_file_index              (-1           ) // no bmp dir loaded
            , lit_delta_max               (800          ) // points
            , blank_delta_max             (1000         ) // points
            , max_dwell_millisec          (333          ) // microseconds
            , max_wag_microsec            (50000.0      ) // double 1/20th second
            , insignificant_distance      (0.0          ) // points
            , insignificant_angle         (one_degree   ) // radians
            , recolor_span_factor         (1.0          )
            , onion_skin_fade             (0.70         ) // ratio 0.0 ~ 1.0
            , background_bmp_scale        (1.00         )
            , rendered_arc_angle          (6.0          ) // degrees
            , rendered_mono_font_space    (0.70         ) // percent of glyph space
            , rendered_vari_font_gap      (0.20         ) // percent of glyph space
            , rendered_vari_font_space    (0.50         ) // percent of glyph space
            , frames_per_second           (16.0         ) // 0 == every frame is shown exactly once
            , still_frame_duration        (10.0         ) // seconds
            , rotation_step               (one_degree   ) // radians
            , magnitude_step              (0.01         )
            , magnitude_step_up           (1.01         )
            , magnitude_step_dn           (0.99         )
    {};
    //------------------------------------------------------------------------
   ~LaserBoy_settings  ()
    {};
    //------------------------------------------------------------------------
    bool load_wtf_file (string wtf_file);
    bool save_wtf_file (string wtf_file);
    //------------------------------------------------------------------------
    string             bg_file_name         ,
                       install_GUID         ,
                       effect_name          ,
                       color_rescale_file[4], // txt file names for each color channel
                       working_directory,
                       working_subdirectory,
                       file_name,
                       font_name,
                       current_extension,
                       absolute_path_of_LaserBoy;

    LaserBoy_color     bg_color         ,
                       mono_text        ,
                       rendered_bounds  ,
                       rendered_clipping,
                       rendered_blank   ,
                       rendered_black   ;

    LaserBoy_3D_double fulcrum     ,
                       view_angle  ,
                       view_scale  ,
                       view_offset ;

    bool    view_has_changed         ,
            rainbow_menus            ,
            alt_menu_c               ,
            alt_menu_h               ,
            alt_menu_k               ,
            alt_menu_l               ,
            show_vectors             ,
            show_vertices            ,
            show_blanking            ,
            show_intro               ,
            show_bridge              ,
            show_coda                ,
            show_cursors             ,
            show_direction           ,
            show_color_cursor        ,
            show_fixed_bounds        ,
            show_fixed_origin        ,
            show_floating_axis       ,
            show_floating_bounds     ,
            infinite_vector          ,
            destructive_clipping     ,
            show_fulcrum             ,
            show_indices             ,
            show_effects_generation  ,
            show_background_bitmap   ,
            background_bmp_negative  ,
            show_onion_skin          ,
            show_onion_visuals       ,
            show_menu                ,
            show_palette             ,
            show_target_palette      ,
            show_stats               ,
            show_TUI_clue            ,
            track_bmp_view           ,
            show_frame_stats         ,
            show_frame_set_stats     ,
            show_Z_as_order          ,
            show_quad_view           ,
            show_cursor_limits_msg   ,
            ui_visuals_flip_flop     ,
            loop_animation           ,
            approximate_frame_rate   ,
            wave_show_vertices       ,
            wave_show_blanks         ,
            wave_real_time           ,
            wave_show_inverted       ,
            wave_clean_screen        ,
            wave_loop                ,
            auto_scale_dxf           ,
            maintain_real_origin     ,
            save_true_color_dxf      ,
            black_dwell_vertices     ,
            auto_flatten_z           ,
            z_from_unformatted       ,
            no_equivalent_vectors    ,
            conglomerate_in_minimize ,
            fracture_b4_conglomerate ,
            invert_wave_output       ,
            auto_minimize            ,
            auto_opti_ctn            ,
            auto_apply_offsets       ,
            auto_apply_rescales      ,
            no_black_match           ,
            allow_lit_black          ,
            save_2D_as_3D            ,
            save_1_frame_bridge      ,
            save_ild_fmt_4_5         ,
            save_txt_with_color      ,
            save_txt_unit            ,
            save_txt_color_hex       ,
            save_txt_named_palettes  ,
            save_fmtd_wav_splits     ,
            bond_word                ;

    u_int   black_level              ,
            number_of_skins          ,
            sample_rate              ,
            lit_dwell_overhang       ,
            dots_setting             ,
            dwell_on_dot             ,
            recolor_index_multiple   ,
            output_bmp_size          ,
            transparent_menu_font    ,
            font_size_factor         ,
            rendered_line_width      ,
            signal_if_not_z          ,
            unframed_wave_stride     ,
            channels_of_wav_out      ,
            app_runs_count           ,
            signal_bit_mask       [8]; // number of bits to mask from the LSB end

    int     displacement_step        ,
            color_frame_shift        ,
            bmp_file_index           ,
            signal_polarity       [8], // -1 unknown, 0 positive, 1 negative
            wave_offsets          [8]; // in samples

    double  lit_delta_max            ,
            blank_delta_max          ,
            max_dwell_millisec       ,
            max_wag_microsec         ,
            insignificant_distance   ,
            insignificant_angle      ,
            recolor_span_factor      ,
            onion_skin_fade          ,
            background_bmp_scale     ,
            rendered_arc_angle       , // in degrees
            rendered_mono_font_space ,
            rendered_vari_font_gap   ,
            rendered_vari_font_space ,
            frames_per_second        ,
            still_frame_duration     ,
            rotation_step            , // in radians!
            magnitude_step           ,
            magnitude_step_up        ,
            magnitude_step_dn        ;
};

//############################################################################
class LaserBoy_space : public LaserBoy_settings, public LaserBoy_frame_set, public LaserBoy_palette_set
{
public:
    LaserBoy_space(LaserBoy_FLTK_GUI* gui);
    //------------------------------------------------------------------------
   ~LaserBoy_space()
    {
        bmp_free (&bmp);
        bmp_free (&background);
        bmp_free (&background_bmp_2D);
        bmp_free (&background_bmp_3D);
        LaserBoy_frame_set::clear();
        LaserBoy_palette_set::clear();
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& operator [] (int index)    // does not set frame_index
    {                                          // eliminates ambiguity of space being
        if(number_of_frames())                 // both a frame_set and palette set
        {
            if(index >= (int)number_of_frames())
                index = 0;
            if(index < 0)
                index = number_of_frames() - 1;
            return LaserBoy_frame_set::at(index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_space& operator += (const LaserBoy_frame_set& frames)
    {
        LaserBoy_frame_set::insert(LaserBoy_frame_set::end(),
                                   frames.begin(), frames.end()
                                  );
        LaserBoy_frame_set::shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_space& operator += (const LaserBoy_frame& frame)
    {
        LaserBoy_frame_set::push_back(frame);
        LaserBoy_frame_set::shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_space& operator += (const LaserBoy_palette& palette)
    {
        LaserBoy_palette_set::push_back(palette);
        LaserBoy_palette_set::shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    bool have_nonzero_offsets()
    {
        if(wave_offsets[0]) return true;
        if(wave_offsets[1]) return true;
        if(wave_offsets[2]) return true;
        if(wave_offsets[3]) return true;
        if(wave_offsets[4]) return true;
        if(wave_offsets[5]) return true;
        if(wave_offsets[6]) return true;
        if(wave_offsets[7]) return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool f_effect_selected()
    {
        if(effect_name.size() == 0)
            return false;
        for(size_t i = 0; i < frame_effect_names.size(); i++)
            if(frame_effect_names[i] == effect_name)
                return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool sf_effect_selected()
    {
        if(effect_name.size() == 0)
            return false;
        for(size_t i = 0; i < selected_frames_effect_names.size(); i++)
            if(selected_frames_effect_names[i] == effect_name)
                return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool fs_effect_selected()
    {
        if(effect_name.size() == 0)
            return false;
        for(size_t i = 0; i < frame_set_effect_names.size(); i++)
            if(frame_set_effect_names[i] == effect_name)
                return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool file_selected()
    {
        if(file_name.size() == 0)
            return false;
        string list_item;
        for(size_t list_item_index = 0; list_item_index < working_directory_list.size(); list_item_index++)
        {
            list_item = working_directory_list[list_item_index].path().filename().string();
            if(list_item == file_name)
                return true;
        }
        return false;
    }
    //------------------------------------------------------------------------
    bool subdirectory_selected()
    {
        if(working_subdirectory.size() == 0)
            return false;
        string list_item,
               dir_name  = working_subdirectory;
        if(dir_name.at(dir_name.size() - 1) == '/')
            dir_name.erase(dir_name.size() - 1);
        for(size_t list_item_index = 0; list_item_index < working_directory_list.size(); list_item_index++)
        {
            if(working_directory_list[list_item_index].path().filename().string() == dir_name)
                return true;
        }
        return false;
    }
    //------------------------------------------------------------------------
    bool load_wtf_file                               (string wtf_file);
    //------------------------------------------------------------------------
    void clean_screen                                ();
    //------------------------------------------------------------------------
    void rename_all_frames                           (string prefix);
    //------------------------------------------------------------------------
    bool omit_palette                                (int index);
    void clear_unused_palettes                       ();
    void sync_rgb_and_palette                        ();
    void sync_frames_to_target                       ();
    void minimize_tables_and_palettes                ();
    void best_reduce_all_to_palette                  ();
    //------------------------------------------------------------------------
    void load_color_rescale_files                    ();
    LaserBoy_Rescale_Error_Code  load_color_rescale_file(int color_channel);
    int  color_rescales_flags                        ();
    //------------------------------------------------------------------------
    void load_frame_effects                          ();
    void load_selected_frames_effects                ();
    void load_frame_set_effects                      ();
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  call_frame_effect           ();
    LaserBoy_Error_Code  call_selected_frames_effect ();
    LaserBoy_Error_Code  call_frame_set_effect       ();
    //------------------------------------------------------------------------
    void load_background_bitmap                      (struct LaserBoy_bmp* bmp);
    void render_background_bitmap                    ();
    bool load_background_bitmap                      (const string& file, bool scale_to_screen);
    bool reload_background_bitmap                    ();
    void clear_background_bitmap                     ();
    void recolor_background                          ();
    //------------------------------------------------------------------------
    void simplify_view_angle()
    {
        while(view_angle.x < 0)
         view_angle.x += two_pi;
        while(view_angle.x > two_pi)
         view_angle.x -= two_pi;
        while(view_angle.y < 0)
         view_angle.y += two_pi;
        while(view_angle.y > two_pi)
         view_angle.y -= two_pi;
        while(view_angle.z < 0)
         view_angle.z += two_pi;
        while(view_angle.z > two_pi)
         view_angle.z -= two_pi;
    }
    //------------------------------------------------------------------------
    LaserBoy_bmp* p_bmp              () { return &bmp;               }
    LaserBoy_bmp* p_background       () { return &background;        }
    LaserBoy_bmp* p_background_bmp_2D() { return &background_bmp_2D; }
    LaserBoy_bmp* p_background_bmp_3D() { return &background_bmp_3D; }
    //------------------------------------------------------------------------
    void undo_wave_temp(std::fstream &in, std::fstream &out, const string& file_name)
    {
        if(in.is_open())
            in.close();
        if(out.is_open())
            out.close();
        remove(file_name.c_str());
        rename(  (file_name + ".tmp").c_str()
               , file_name.c_str()
              );
        return;
    }
    //------------------------------------------------------------------------
    bool apply_wave_offsets_prep       (const string& file);
    bool overwirte_wave_offsets        (const string& file);
    bool invert_wave_prep              (const string& file);
    bool invert_signals_prep           (const string& file);
    bool clear_wave_polarity_list      (const string& file, bool global_flip_flop);
    bool apply_color_rescales_prep     (const string& file);
    bool omit_color_rescales           (const string& file);
    bool save_color_rescales           (const string& file,    const string& out_name);
    bool black_level_to_wave_prep      (const string& file_in, const string& file_out);
    bool bit_resolution_to_wave_prep   (const string& file_in, const string& file_out);
    //------------------------------------------------------------------------
    bool split_wave_XY_r_g_b_i_LR_prep (      string  file,
                                        const string& dir_wav,
                                        const string& dir_unformatted,
                                        const string& dir_audio
                                       );
    //------------------------------------------------------------------------
    bool split_wave_XY_rg_bi_LR_prep   (      string  file,
                                        const string& dir_wav,
                                        const string& dir_unformatted,
                                        const string& dir_audio
                                       );
    //------------------------------------------------------------------------
    bool join_XY_r_g_b_i_waves_prep    (const string& file_XY,
                                        const string& file_r,
                                        const string& file_g,
                                        const string& file_b,
                                        const string& file_i,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool join_XY_rg_b_waves_prep       (const string& file_XY,
                                        const string& file_rg,
                                        const string& file_b,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool join_XY_rg_b_LR_waves_prep    (const string& file_XY,
                                        const string& file_rg,
                                        const string& file_b,
                                        const string& file_LR,
                                        const string& file_out,
                                        bool   global_polarity
                                       );
    //------------------------------------------------------------------------
    bool add_audio_to_wave_prep        (const string& file_laser,
                                        const string& file_audio,
                                        const string& file_out
                                       );
    //------------------------------------------------------------------------
    void apply_wave_offsets        (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_wave               (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_signals            (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void black_level_to_wave       (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void bit_resolution_to_wave    (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void apply_color_rescales      (std::fstream&        wave_in,
                                    std::fstream&        wave_out,
                                    LaserBoy_wave_header wave_in_header
                                   );
    //------------------------------------------------------------------------
    void split_wave_XY_r_g_b_i_LR  (LaserBoy_wave_header wave_in_header,
                                    std::fstream&        wave_in,
                                    std::fstream&        wave_out_XY,
                                    std::fstream&        wave_out_r,
                                    std::fstream&        wave_out_g,
                                    std::fstream&        wave_out_b,
                                    std::fstream&        wave_out_i,
                                    std::fstream&        wave_out_LR
                                   );
    //------------------------------------------------------------------------
    void split_wave_XY_rg_bi_LR    (LaserBoy_wave_header wave_in_header,
                                    std::fstream&        wave_in,
                                    std::fstream&        wave_out_XY,
                                    std::fstream&        wave_out_rg,
                                    std::fstream&        wave_out_bi,
                                    std::fstream&        wave_out_LR
                                   );
    //------------------------------------------------------------------------
    void join_XY_r_g_b_i_waves     (std::fstream& in_XY,
                                    std::fstream& in_r,
                                    std::fstream& in_g,
                                    std::fstream& in_b,
                                    std::fstream& in_i,
                                    std::fstream& out,
                                    bool          global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_waves        (std::fstream& in_XY,
                                    std::fstream& in_rg,
                                    std::fstream& in_b,
                                    std::fstream& out,
                                    bool          global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_LR_waves     (std::fstream& in_XY,
                                    std::fstream& in_rg,
                                    std::fstream& in_b,
                                    std::fstream& in_LR,
                                    std::fstream& out,
                                    bool          global_polartity
                                   );
    //------------------------------------------------------------------------
    void join_XY_rg_b_00_waves     (std::fstream& in_XY,
                                    std::fstream& in_rg,
                                    std::fstream& in_b,
                                    std::fstream& out,
                                    bool          global_polartity
                                   );
    //------------------------------------------------------------------------
    void add_audio_to_wave         (LaserBoy_wave_header wave_in_header_laser,
                                    std::fstream&        in_laser,
                                    std::fstream&        in_audio,
                                    std::fstream&        out
                                   );
    //------------------------------------------------------------------------
    void add_silence_to_wave       (LaserBoy_wave_header wave_in_header_laser,
                                    std::fstream&        in_laser,
                                    std::fstream&        out
                                   );
    //------------------------------------------------------------------------
    void format_wave               (LaserBoy_wave_header wave_in_header_laser,
                                    std::fstream&        in_laser,
                                    std::fstream&        out,
                                    bool                 global_polartity,
                                    bool                 write_eof_bits,
                                    int                  channel_5
                                   );
    //------------------------------------------------------------------------
    string LaserBoy_wave_signal_id_to_name  (const short&   signal_id) const;
    string LaserBoy_wave_LSB_tag_to_name    (const u_short& LSB_tag  ) const;
    string LaserBoy_dots_setting_id_to_name (const int&     id       ) const;
    //------------------------------------------------------------------------
//    void   tell(LaserBoy_wave_header header, string label) const;
    //------------------------------------------------------------------------
                           LaserBoy_FLTK_GUI *p_GUI;
                       struct  LaserBoy_bmp  bmp              , // a memory clone of screen
                                             background       , // empty for clearing screen
                                             background_bmp_2D, // possibly a bitmap picture
                                             background_bmp_3D; // possibly rotated, scaled, panned

                    vector<directory_entry>  working_directory_list,
                                             bmp_file_list;

                             vector<string>  frame_effect_names          ,
                                             selected_frames_effect_names,
                                             frame_set_effect_names      ;

             vector<LaserBoy_frame_effect>   frame_effects               ;
    vector<LaserBoy_selected_frames_effect>  selected_frames_effects     ;
    vector<LaserBoy_frame_set_effect>        frame_set_effects           ;

                                      short  color_rescale_r[256],
                                             color_rescale_g[256],
                                             color_rescale_b[256],
                                             color_rescale_i[256];

                                      short *color_rescale  [4];

                         LaserBoy_frame_set  font_frames;
                                  u32string  font_index;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
