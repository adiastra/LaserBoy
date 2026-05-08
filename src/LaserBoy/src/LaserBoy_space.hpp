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
class LaserBoy_GUI;

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
            , wav_channel_map             ("XYrgbi"     ) // string
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
            , move_menu                   (false        )
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
            , end_vect_to_delta_max       (false        )
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
            , wav_show_vertices           (false        )
            , wav_show_blanks             (false        )
            , wav_real_time               (true         )
            , wav_show_inverted           (true         )
            , wav_clean_screen            (true         )
            , wav_loop                    (false        )
            , auto_scale_dxf              (true         )
            , maintain_real_origin        (false        )
            , save_true_color_dxf         (false        )
            , black_dwell_vertices        (false        )
            , auto_flatten_z              (true         )
            , no_equivalent_vectors       (false        )
            , conglomerate_in_minimize    (false        )
            , fracture_b4_conglomerate    (false        )
            , invert_wav_output           (true         )
            , maintain_first_anchor       (false        )
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
            , bond_word                   (false        )
            , black_level                 (0            ) // int
            , number_of_skins             (4            ) // int
            , sample_rate                 (48000        ) // int Hz
            , lit_dwell_overhang          (2            ) // vertices
            , dots_setting                (LASERBOY_DOTS_IGNORE)
            , recolor_index_multiple      (1            ) // color index
            , output_bmp_size             (1000         ) // pixels square
            , transparent_menu_font       (0            )
            , font_size_factor            (1            )
            , rendered_line_width         (1            )
            , wav_channel_i               (LASERBOY_SIGNAL_MONO_OR_ANALOG)
            , unframed_wav_stride         (4800         ) // samples 1/10 of sec
            , app_runs_count              (1            )
            , displacement_step           (100          ) // int points
            , color_frame_shift           (1            )
            , bmp_file_index              (-1           ) // no bmp dir loaded
            , lit_delta_max               (800          ) // points
            , blank_delta_max             (1000         ) // points
            , max_dwell_microsec          (333          ) // microseconds
            , max_wag_microsec            (50000.0      ) // double 1/20th second
            , dwell_on_dot                (10.00        ) // vertices
            , color_delay_microsec        (100.00       ) // aprox -5/48KHz
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
                       absolute_path_of_LaserBoy,
                       wav_channel_map;

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
            move_menu                ,
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
            end_vect_to_delta_max    ,
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
            wav_show_vertices        ,
            wav_show_blanks          ,
            wav_real_time            ,
            wav_show_inverted        ,
            wav_clean_screen         ,
            wav_loop                 ,
            auto_scale_dxf           ,
            maintain_real_origin     ,
            save_true_color_dxf      ,
            black_dwell_vertices     ,
            auto_flatten_z           ,
            no_equivalent_vectors    ,
            conglomerate_in_minimize ,
            fracture_b4_conglomerate ,
            invert_wav_output        ,
            maintain_first_anchor    ,
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
            bond_word                ;

    u_int   black_level              ,
            number_of_skins          ,
            sample_rate              ,
            lit_dwell_overhang       ,
            dots_setting             ,
            recolor_index_multiple   ,
            output_bmp_size          ,
            transparent_menu_font    ,
            font_size_factor         ,
            rendered_line_width      ,
            wav_channel_i            ,
            unframed_wav_stride      ,
            app_runs_count           ,
            signal_bit_mask       [7]; // XYZrgbi number of bits to mask from the LSB end

    int     displacement_step        ,
            color_frame_shift        ,
            bmp_file_index           ,
            signal_polarity       [7], // -1 unknown, 0 positive, 1 negative
            wav_offsets           [7]; // in samples

    double  lit_delta_max            ,
            blank_delta_max          ,
            max_dwell_microsec       ,
            max_wag_microsec         ,
            dwell_on_dot             ,
            color_delay_microsec     ,
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
class LaserBoy_space : public LaserBoy_settings,
                       public LaserBoy_frame_set,
                       public LaserBoy_palette_set
{
public:
    LaserBoy_space(LaserBoy_GUI* gui);
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
    LaserBoy_space& operator  = (const LaserBoy_frame_set& frames)
    {
        LaserBoy_frame_set::clear();
        LaserBoy_frame_set::insert(LaserBoy_frame_set::end(),
                                   frames.begin(), frames.end()
                                  );
        LaserBoy_frame_set::shrink_to_fit();
        return *this;
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
        if(wav_offsets[0]) return true;
        if(wav_offsets[1]) return true;
        if(wav_offsets[2]) return true;
        if(wav_offsets[3]) return true;
        if(wav_offsets[4]) return true;
        if(wav_offsets[5]) return true;
        if(wav_offsets[6]) return true;
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
        if(directory_exists(working_directory + working_subdirectory, absolute_path_of_LaserBoy))
            return true;
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
    void set_palette_indexes_by_name                 ();
    //------------------------------------------------------------------------
    void load_color_rescale_files                    ();
    LaserBoy_Rescale_Error_Code  load_color_rescale_file (size_t color_channel);
    LaserBoy_Rescale_Error_Code  save_gamma_index_as_txt (double gamma);
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
        while(view_angle.x < 0     )  view_angle.x += two_pi;
        while(view_angle.x > two_pi)  view_angle.x -= two_pi;
        while(view_angle.y < 0     )  view_angle.y += two_pi;
        while(view_angle.y > two_pi)  view_angle.y -= two_pi;
        while(view_angle.z < 0     )  view_angle.z += two_pi;
        while(view_angle.z > two_pi)  view_angle.z -= two_pi;
    }
    //------------------------------------------------------------------------
    LaserBoy_bmp* p_bmp              () { return &bmp;               }
    LaserBoy_bmp* p_background       () { return &background;        }
    LaserBoy_bmp* p_background_bmp_2D() { return &background_bmp_2D; }
    LaserBoy_bmp* p_background_bmp_3D() { return &background_bmp_3D; }
    //------------------------------------------------------------------------
    void undo_wav_temp(std::fstream &in, std::fstream &out, const string& file_name)
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
    void set_wav_out_header            (LaserBoy_wav_header &header,
                                        bool optimized,
                                        bool timed
                                       );
    //------------------------------------------------------------------------
    bool apply_wav_offsets_prep        (const string& file);
    bool overwirte_wav_offsets         (const string& file);
    bool invert_wav_prep               (const string& file);
    bool invert_signals_prep           (const string& file);
    bool clear_wav_polarity_list       (const string& file, bool global_flip_flop);
    bool apply_color_rescales_prep     (const string& file);
    bool omit_color_rescales           (const string& file);
    bool save_wav_rescales_as_txt      (const string& file,    const string& out_name);
    bool bit_resolution_to_wav_prep    (const string& file_in, const string& file_out);
    //------------------------------------------------------------------------
    void apply_wav_offsets         (std::fstream&        wav_in,
                                    std::fstream&        wav_out,
                                    LaserBoy_wav_header  wav_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_wav                (std::fstream&        wav_in,
                                    std::fstream&        wav_out,
                                    LaserBoy_wav_header  wav_in_header
                                   );
    //------------------------------------------------------------------------
    void invert_signals            (std::fstream&        wav_in,
                                    std::fstream&        wav_out,
                                    LaserBoy_wav_header  wav_in_header
                                   );
    //------------------------------------------------------------------------
    void bit_resolution_to_wav     (std::fstream&        wav_in,
                                    std::fstream&        wav_out,
                                    LaserBoy_wav_header  wav_in_header
                                   );
    //------------------------------------------------------------------------
    void apply_color_rescales      (std::fstream&        wav_in,
                                    std::fstream&        wav_out,
                                    LaserBoy_wav_header  wav_in_header
                                   );
    //------------------------------------------------------------------------
                               LaserBoy_GUI *p_GUI;
                       struct  LaserBoy_bmp  bmp              , // a memory clone of screen
                                             background       , // empty for clearing screen
                                             background_bmp_2D, // possibly a bitmap picture
                                             background_bmp_3D; // possibly rotated, scaled, panned

                    vector<directory_entry>  working_directory_list,
                                             bmp_file_list         ;

                             vector<string>  frame_effect_names          ,
                                             selected_frames_effect_names,
                                             frame_set_effect_names      ;

              vector<LaserBoy_frame_effect>  frame_effects               ;
    vector<LaserBoy_selected_frames_effect>  selected_frames_effects     ;
          vector<LaserBoy_frame_set_effect>  frame_set_effects           ;

                                      short  color_rescale_r[256],
                                             color_rescale_g[256],
                                             color_rescale_b[256],
                                             color_rescale_i[256];

                                      short *color_rescale  [4];

                                     string  last_ild_file_name;

                         LaserBoy_frame_set  font_frames;
                                  u32string  font_index ;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
