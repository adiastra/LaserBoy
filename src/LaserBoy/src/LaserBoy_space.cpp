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
// LaserBoy_space.cpp is part of LaserBoy.
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
bool LaserBoy_settings::load_wtf_file(string wtf_file)
{
    int           c,
                  dud;
    string        version,
                  GUID;
    std::ifstream in(wtf_file.c_str(), ios::in);
    if(in.is_open())
    {
        //----------------------------------------------------------------
        in >> version;
        if(version == LASERBOY_CFG_VERSION) // "LaserBoy.wtf-12-19-2023"
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> color_frame_shift       ;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> color_delay_microsec    ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> move_menu               ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> track_bmp_view          ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> end_vect_to_delta_max   ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> track_bmp_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_1_frame_bridge     ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_Z];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Z];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> wav_channel_map         ;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-09-25-2023")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> color_frame_shift       ;
            in >> dud                     ;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> color_delay_microsec    ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> move_menu               ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> track_bmp_view          ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> end_vect_to_delta_max   ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> track_bmp_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_1_frame_bridge     ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-01-04-2023")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> color_frame_shift       ;
            in >> dud                     ;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> move_menu               ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> track_bmp_view          ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> end_vect_to_delta_max   ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> track_bmp_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_1_frame_bridge     ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-08-27-2022")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> color_frame_shift       ;
            in >> dud;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> alt_menu_c              ;
            in >> alt_menu_h              ;
            in >> alt_menu_k              ;
            in >> alt_menu_l              ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_1_frame_bridge     ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-08-20-2021")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> color_frame_shift       ;
            in >> dud;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> alt_menu_c              ;
            in >> alt_menu_h              ;
            in >> alt_menu_k              ;
            in >> alt_menu_l              ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-07-28-2021")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> c                       ;
            mono_text.r = c               ;
            in >> c                       ;
            mono_text.g = c               ;
            in >> c                       ;
            mono_text.b = c               ;
            in >> c                       ;
            rendered_blank.r = c          ;
            in >> c                       ;
            rendered_blank.g = c          ;
            in >> c                       ;
            rendered_blank.b = c          ;
            in >> c                       ;
            rendered_black.r = c          ;
            in >> c                       ;
            rendered_black.g = c          ;
            in >> c                       ;
            rendered_black.b = c          ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> rendered_line_width     ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> dud;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> recolor_index_multiple  ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> rainbow_menus           ;
            in >> show_vectors            ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_direction          ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> infinite_vector         ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        else if(version == "LaserBoy.wtf-10-06-2020")
        {
            in >> bg_file_name            ;
            in >> c                       ;
            bg_color.r = c                ;
            in >> c                       ;
            bg_color.g = c                ;
            in >> c                       ;
            bg_color.b = c                ;
            in >> sample_rate             ;
            in >> lit_dwell_overhang      ;
            in >> dwell_on_dot            ;
            in >> font_size_factor        ;
            in >> background_bmp_scale    ;
            in >> displacement_step       ;
            in >> dud;
            in >> app_runs_count          ;
            in >> output_bmp_size         ;
            in >> transparent_menu_font   ;
            in >> wav_channel_i           ;
            in >> unframed_wav_stride     ;
            in >> fulcrum.x               ;
            in >> fulcrum.y               ;
            in >> fulcrum.z               ;
            in >> view_angle.x            ;
            in >> view_angle.y            ;
            in >> view_angle.z            ;
            in >> view_scale.x            ;
            in >> view_scale.y            ;
            in >> view_offset.x           ;
            in >> view_offset.y           ;
            in >> lit_delta_max           ;
            in >> blank_delta_max         ;
            in >> max_dwell_microsec      ;
            in >> max_wag_microsec        ;
            in >> insignificant_distance  ;
            in >> insignificant_angle     ;
            in >> recolor_span_factor     ;
            in >> onion_skin_fade         ;
            in >> rendered_arc_angle      ;
            in >> rendered_mono_font_space;
            in >> rendered_vari_font_gap  ;
            in >> rendered_vari_font_space;
            in >> rotation_step           ;
            in >> magnitude_step          ;
            magnitude_step_up = 1.00 + magnitude_step;
            magnitude_step_dn = 1.00 - magnitude_step;
            in >> frames_per_second       ;
            in >> still_frame_duration    ;
            in >> GUID                    ;
            in >> show_vertices           ;
            in >> show_blanking           ;
            in >> show_intro              ;
            in >> show_bridge             ;
            in >> show_coda               ;
            in >> show_cursors            ;
            in >> show_color_cursor       ;
            in >> show_palette            ;
            in >> show_target_palette     ;
            in >> show_stats              ;
            in >> show_fixed_bounds       ;
            in >> show_fixed_origin       ;
            in >> show_floating_axis      ;
            in >> show_floating_bounds    ;
            in >> destructive_clipping    ;
            in >> show_onion_skin         ;
            in >> show_onion_visuals      ;
            in >> show_indices            ;
            in >> show_effects_generation ;
            in >> show_Z_as_order         ;
            in >> show_quad_view          ;
            in >> show_cursor_limits_msg  ;
            in >> auto_scale_dxf          ;
            in >> maintain_real_origin    ;
            in >> save_true_color_dxf     ;
            in >> loop_animation          ;
            in >> approximate_frame_rate  ;
            in >> black_dwell_vertices    ;
            in >> auto_flatten_z          ;
            in >> dud                     ;
            in >> no_equivalent_vectors   ;
            in >> conglomerate_in_minimize;
            in >> fracture_b4_conglomerate;
            in >> dots_setting            ;
            in >> invert_wav_output       ;
            in >> auto_minimize           ;
            in >> auto_opti_ctn           ;
            in >> auto_apply_offsets      ;
            in >> auto_apply_rescales     ;
            in >> no_black_match          ;
            in >> allow_lit_black         ;
            in >> save_2D_as_3D           ;
            in >> save_ild_fmt_4_5        ;
            in >> save_txt_with_color     ;
            in >> save_txt_unit           ;
            in >> save_txt_color_hex      ;
            in >> save_txt_named_palettes ;
            in >> dud;
            in >> bond_word               ;
            in >> wav_show_vertices       ;
            in >> wav_show_blanks         ;
            in >> wav_real_time           ;
            in >> wav_show_inverted       ;
            in >> wav_clean_screen        ;
            in >> wav_loop                ;
            in >> wav_offsets[LASERBOY_CHANNEL_X];
            in >> wav_offsets[LASERBOY_CHANNEL_Y];
            in >> wav_offsets[LASERBOY_CHANNEL_R];
            in >> wav_offsets[LASERBOY_CHANNEL_G];
            in >> wav_offsets[LASERBOY_CHANNEL_B];
            in >> wav_offsets[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> black_level             ;
            in >> number_of_skins         ;
            in >> signal_bit_mask[LASERBOY_CHANNEL_X];
            in >> signal_bit_mask[LASERBOY_CHANNEL_Y];
            in >> signal_bit_mask[LASERBOY_CHANNEL_R];
            in >> signal_bit_mask[LASERBOY_CHANNEL_G];
            in >> signal_bit_mask[LASERBOY_CHANNEL_B];
            in >> signal_bit_mask[LASERBOY_CHANNEL_I];
            in >> dud;
            in >> dud;
            in >> color_rescale_file[0]   ;
            in >> color_rescale_file[1]   ;
            in >> color_rescale_file[2]   ;
            in >> color_rescale_file[3]   ;
        }
        //----------------------------------------------------------------
        in.close();
        return true;
    } // end if(in.is_open())
    signal_bit_mask[LASERBOY_CHANNEL_Z] = 0;
    if(wtf_file == (LASERBOY_WTF_SHARE + "LaserBoy.wtf"))
    {
        if(GUID.size() < 8)
            GUID = GUID8char();
        else
            GUID = GUID.substr(0, 8);
        install_GUID = GUID;
        save_wtf_file(wtf_file);
    }
    return false;
}

//############################################################################
bool LaserBoy_settings::save_wtf_file(string wtf_file)
{
    std::ofstream out(wtf_file.c_str(), ios::out);
    if(out.is_open())
    {
        out << LASERBOY_CFG_VERSION     << ENDL;
        out << bg_file_name.c_str()     << ENDL;
        out << (int)(bg_color.r)        << ENDL;
        out << (int)(bg_color.g)        << ENDL;
        out << (int)(bg_color.b)        << ENDL;
        out << (int)(mono_text.r)       << ENDL;
        out << (int)(mono_text.g)       << ENDL;
        out << (int)(mono_text.b)       << ENDL;
        out << (int)(rendered_blank.r)  << ENDL;
        out << (int)(rendered_blank.g)  << ENDL;
        out << (int)(rendered_blank.b)  << ENDL;
        out << (int)(rendered_black.r)  << ENDL;
        out << (int)(rendered_black.g)  << ENDL;
        out << (int)(rendered_black.b)  << ENDL;
        out << sample_rate              << ENDL;
        out << lit_dwell_overhang       << ENDL;
        out << dwell_on_dot             << ENDL;
        out << font_size_factor         << ENDL;
        out << rendered_line_width      << ENDL;
        out << background_bmp_scale     << ENDL;
        out << displacement_step        << ENDL;
        out << color_frame_shift        << ENDL;
        out << app_runs_count           << ENDL;
        out << output_bmp_size          << ENDL;
        out << transparent_menu_font    << ENDL;
        out << wav_channel_i            << ENDL;
        out << unframed_wav_stride      << ENDL;
        out << fulcrum.x                << ENDL;
        out << fulcrum.y                << ENDL;
        out << fulcrum.z                << ENDL;
        out << view_angle.x             << ENDL;
        out << view_angle.y             << ENDL;
        out << view_angle.z             << ENDL;
        out << view_scale.x             << ENDL;
        out << view_scale.y             << ENDL;
        out << view_offset.x            << ENDL;
        out << view_offset.y            << ENDL;
        out << lit_delta_max            << ENDL;
        out << blank_delta_max          << ENDL;
        out << max_dwell_microsec       << ENDL;
        out << max_wag_microsec         << ENDL;
        out << color_delay_microsec     << ENDL;
        out << insignificant_distance   << ENDL;
        out << insignificant_angle      << ENDL;
        out << recolor_span_factor      << ENDL;
        out << recolor_index_multiple   << ENDL;
        out << onion_skin_fade          << ENDL;
        out << rendered_arc_angle       << ENDL;
        out << rendered_mono_font_space << ENDL;
        out << rendered_vari_font_gap   << ENDL;
        out << rendered_vari_font_space << ENDL;
        out << rotation_step            << ENDL;
        out << magnitude_step           << ENDL;
        out << frames_per_second        << ENDL;
        out << still_frame_duration     << ENDL;
        out << install_GUID             << ENDL;
        out << rainbow_menus            << ENDL;
        out << move_menu                << ENDL;
        out << show_vectors             << ENDL;
        out << show_vertices            << ENDL;
        out << show_blanking            << ENDL;
        out << show_intro               << ENDL;
        out << show_bridge              << ENDL;
        out << show_coda                << ENDL;
        out << show_direction           << ENDL;
        out << show_stats               << ENDL;
        out << track_bmp_view           << ENDL;
        out << show_fixed_bounds        << ENDL;
        out << show_fixed_origin        << ENDL;
        out << show_floating_axis       << ENDL;
        out << show_floating_bounds     << ENDL;
        out << infinite_vector          << ENDL;
        out << end_vect_to_delta_max    << ENDL;
        out << destructive_clipping     << ENDL;
        out << show_onion_skin          << ENDL;
        out << show_onion_visuals       << ENDL;
        out << show_indices             << ENDL;
        out << show_effects_generation  << ENDL;
        out << show_Z_as_order          << ENDL;
        out << show_quad_view           << ENDL;
        out << track_bmp_view           << ENDL;
        out << show_cursor_limits_msg   << ENDL;
        out << auto_scale_dxf           << ENDL;
        out << maintain_real_origin     << ENDL;
        out << save_true_color_dxf      << ENDL;
        out << loop_animation           << ENDL;
        out << approximate_frame_rate   << ENDL;
        out << black_dwell_vertices     << ENDL;
        out << auto_flatten_z           << ENDL;
        out << no_equivalent_vectors    << ENDL;
        out << conglomerate_in_minimize << ENDL;
        out << fracture_b4_conglomerate << ENDL;
        out << dots_setting             << ENDL;
        out << invert_wav_output        << ENDL;
        out << auto_minimize            << ENDL;
        out << auto_opti_ctn            << ENDL;
        out << auto_apply_offsets       << ENDL;
        out << auto_apply_rescales      << ENDL;
        out << no_black_match           << ENDL;
        out << allow_lit_black          << ENDL;
        out << save_2D_as_3D            << ENDL;
        out << save_1_frame_bridge      << ENDL;
        out << save_ild_fmt_4_5         << ENDL;
        out << save_txt_with_color      << ENDL;
        out << save_txt_unit            << ENDL;
        out << save_txt_color_hex       << ENDL;
        out << save_txt_named_palettes  << ENDL;
        out << bond_word                << ENDL;
        out << wav_show_vertices        << ENDL;
        out << wav_show_blanks          << ENDL;
        out << wav_real_time            << ENDL;
        out << wav_show_inverted        << ENDL;
        out << wav_clean_screen         << ENDL;
        out << wav_loop                 << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_X] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_Y] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_Z] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_R] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_G] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_B] << ENDL;
        out << wav_offsets[LASERBOY_CHANNEL_I] << ENDL;
        out << black_level              << ENDL;
        out << number_of_skins          << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_X] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_Y] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_Z] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_R] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_G] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_B] << ENDL;
        out << signal_bit_mask[LASERBOY_CHANNEL_I] << ENDL;
        out << wav_channel_map          << ENDL;
        out << color_rescale_file[0]    << ENDL;
        out << color_rescale_file[1]    << ENDL;
        out << color_rescale_file[2]    << ENDL;
        out << color_rescale_file[3]    << ENDL;
        out << ENDL;
        out << "If this file is corrupted, delete it!" << ENDL;
        out.close();
        return true;
    }
    return false;
}

//############################################################################
LaserBoy_space::LaserBoy_space(LaserBoy_GUI* gui)
            : LaserBoy_settings      (   )
            , LaserBoy_frame_set     (   )
            , LaserBoy_palette_set   (   )
            , p_GUI                  (gui)
            , bmp                    (   ) // LaserBoy_bmp
            , background             (   ) // LaserBoy_bmp
            , background_bmp_2D      (   ) // LaserBoy_bmp
            , background_bmp_3D      (   ) // LaserBoy_bmp
            , working_directory_list (   ) // vector<directory_entry>
            , bmp_file_list          (   ) // vector<directory_entry>
            , last_ild_file_name     (   ) // string
{
    u_int i;
    for(i = LASERBOY_CHANNEL_X; i <= LASERBOY_CHANNEL_I; i++)
    {
        wav_offsets   [i] =  0;
        signal_polarity[i] = -1;
        signal_bit_mask[i] =  0;
    }
    for(i = LASERBOY_CHANNEL_R; i <= LASERBOY_CHANNEL_I; i++)
    {
        wav_offsets   [i] = -5;
        signal_bit_mask[i] =  7;
    }
    color_rescale_file[0] =
    color_rescale_file[1] =
    color_rescale_file[2] =
    color_rescale_file[3] = ".";
    for(i = 0; i < 256; i++)
    {
        color_rescale_r[i] =
        color_rescale_g[i] =
        color_rescale_b[i] =
        color_rescale_i[i] = 0;
    }
    color_rescale[0]  = color_rescale_r;
    color_rescale[1]  = color_rescale_g;
    color_rescale[2]  = color_rescale_b;
    color_rescale[3]  = color_rescale_i;
    load_wtf_file(LASERBOY_WTF_SHARE + "LaserBoy.wtf");
    app_runs_count++;
    save_wtf_file(LASERBOY_WTF_SHARE + "LaserBoy.wtf");
    populate_palette_set         ();
    load_frame_effects           ();
    load_selected_frames_effects ();
    load_frame_set_effects       ();
    load_color_rescale_files     ();
}

//############################################################################
bool LaserBoy_space::load_wtf_file(string wtf_file)
{
    bool ret = LaserBoy_settings::load_wtf_file(wtf_file);
    recolor_background();
    return ret;
}

//############################################################################
void LaserBoy_space::clean_screen()
{
    bmp_copy(&bmp, &background);
    if(    show_background_bitmap
        && bg_file_name != "."
      )
    {
        if(view_has_changed)
        {
            render_background_bitmap();
            view_has_changed = false;
        }
        bmp_copy(&bmp, &background_bmp_3D);
    }
}

//############################################################################
void LaserBoy_space::rename_all_frames(string prefix)
{
    char   number[12];
    size_t i;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
    {
        sprintf(number, "%05d", (int)i);
        (*this)[i].name = prefix + number;
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_space::omit_palette(int index)
{
    size_t  i;
    //------------------------------------------------------------------------
    if(index < LASERBOY_BUILT_IN_PALETTES)
        return false;
    //------------------------------------------------------------------------
    if(index >= (int)LaserBoy_palette_set::size())
        return false;
    //------------------------------------------------------------------------
    for(i = 0; i < number_of_frames(); i++)
        if(index == frame_picker(i).palette_index)
            return false;
    //------------------------------------------------------------------------
    if(index == (int)LaserBoy_palette_set::size() - 1)
        LaserBoy_palette_set::erase(LaserBoy_palette_set::end());
    else
    {
        LaserBoy_palette_set::erase(LaserBoy_palette_set::begin() + index);
        for(i = 0; i < number_of_frames(); i++)
            if(frame_picker(i).palette_index > index)
                frame_picker(i).palette_index--;
    }
    //------------------------------------------------------------------------
    if(target_palette_index >= LaserBoy_palette_set::size())
        target_palette_index = LaserBoy_palette_set::size() - 1;
    if(palette_index >= (int)LaserBoy_palette_set::size())
        palette_index = (int)LaserBoy_palette_set::size() - 1;
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void LaserBoy_space::clear_unused_palettes()
{
    size_t  i;
    p_GUI->display_state("clearing unused palettes");
    for(i = number_of_palettes() - 1; i >= LASERBOY_BUILT_IN_PALETTES; i--)
        omit_palette(i);
    return;
}

//############################################################################
void LaserBoy_space::sync_rgb_and_palette()
{
    int current_frame_index = frame_index;
    p_GUI->display_state("syncing rgb & palettes");
    for(size_t i = 0; i < number_of_frames(); i++)
    {
        frame(i).sync_rgb_and_palette();
        p_GUI->display_progress(number_of_frames() - i);
    }
    frame_index = current_frame_index;
    return;
}

//############################################################################
void LaserBoy_space::sync_frames_to_target()
{
    int current_frame_index = frame_index;
    p_GUI->display_state("syncing frames to target");
    for(size_t i = 0; i < number_of_frames(); i++)
    {
        if(frame(i).palette_index == (int)target_palette_index)
            frame(i).set_rgb_from_palette();
        p_GUI->display_progress(number_of_frames() - i);
    }
    frame_index = current_frame_index;
    return;
}

//############################################################################
void LaserBoy_space::minimize_tables_and_palettes()
{
    size_t                i = 0,
                          j,
                          super_palette_first_frame = 0,
                          super_palette_last_frame  = 0,
                          current_frame_index       = frame_index;
    LaserBoy_palette_set  reduced_palette_set;
    LaserBoy_palette      super_palette;
    //------------------------------------------------------------------------
    sync_rgb_and_palette();
    //------------------------------------------------------------------------
    p_GUI->display_state("minimizing tables & palettes");
    while(i < number_of_frames())
    {
        if(    frame_picker(i).palette_index != LASERBOY_TRUE_COLOR
            && frame_picker(i).palette_index != LASERBOY_ILDA_DEFAULT
          )
        {
            //----------------------------------------------------------------
            super_palette_first_frame = i;
            super_palette = palette_picker(frame_picker(i).palette_index);
            super_palette.name = GUID8char();
            //----------------------------------------------------------------
            while(    i < number_of_frames()
                   && super_palette.unite(palette_picker(frame_picker(i).palette_index))
                 )
                i++;
            //----------------------------------------------------------------
            reduced_palette_set.push_back(super_palette);
            super_palette_last_frame  = i - 1;
            //----------------------------------------------------------------
            for(j = super_palette_first_frame; j <= super_palette_last_frame; j++)
            {
                frame(j).find_rgb_in_palette(super_palette);
                frame(j).palette_name = super_palette.name;
            }
            //----------------------------------------------------------------
        }
        else
            i++;
        p_GUI->display_progress(number_of_frames() - i);
    }
    //------------------------------------------------------------------------
    frame(current_frame_index);
    replace_palette_set(reduced_palette_set);
    set_palette_indexes_by_name();
    palette_index = current_frame().palette_index;
    return;
}

//############################################################################
void LaserBoy_space::best_reduce_all_to_palette()
{
    size_t            i,
                      j;
    vector<int>       used_palette_indices;
    LaserBoy_palette  super_palette;
    //------------------------------------------------------------------------
    if(frame_set_palette_index() == LASERBOY_TRUE_COLOR)
    {
        p_GUI->display_state("reducing to best palette");
        for(i = 0; i < number_of_frames(); i++)
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
                        super_palette += palette_picker(frame_picker(i).palette_index);
                    }
                }
                else
                {
                    used_palette_indices.push_back(frame_picker(i).palette_index); // count it
                    super_palette += palette_picker(frame_picker(i).palette_index);
                }
            }
            else
                super_palette += (frame_picker(i).as_color_table().reorder());
            p_GUI->display_progress(number_of_frames() - i);
        }
        //--------------------------------------------------------------------
        super_palette.best_reduction(); // squeeze all that in <= 256 colors
        super_palette.name = GUID8char();
        push_back_palette(super_palette);
        //--------------------------------------------------------------------
        best_match_palette(LaserBoy_palette_set::size() - 1);
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_space::set_palette_indexes_by_name()
{
    for(size_t i = 0; i < number_of_frames(); i++)
        if(frame_picker(i).palette_name.size())
            frame_picker(i).palette_index = name_to_palette_index(frame_picker(i).palette_name);
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_space::call_frame_effect()
{
    size_t i;
    //------------------------------------------------------------------------
    for(i = 0; i < frame_effect_names.size(); i++)
        if(frame_effect_names[i] == effect_name)
            break;
    //------------------------------------------------------------------------
    if(i < frame_effect_names.size())
        return frame_effects[i](&(current_frame()));
    return LASERBOY_EFFECT_NAME_NOT_FOUND;
}

//############################################################################
LaserBoy_Bounds LaserBoy_space::call_selected_frames_effect()
{
    size_t i;
    //------------------------------------------------------------------------
    for(i = 0; i < selected_frames_effect_names.size(); i++)
        if(selected_frames_effect_names[i] == effect_name)
            break;
    //------------------------------------------------------------------------
    if(i < selected_frames_effect_names.size())
        return selected_frames_effects[i](this);
    return LASERBOY_EFFECT_NAME_NOT_FOUND;
}

//############################################################################
LaserBoy_Bounds LaserBoy_space::call_frame_set_effect()
{
    size_t i;
    //------------------------------------------------------------------------
    for(i = 0; i < frame_set_effect_names.size(); i++)
        if(frame_set_effect_names[i] == effect_name)
            break;
    //------------------------------------------------------------------------
    if(i < frame_set_effect_names.size())
        return frame_set_effects[i](this);
    return LASERBOY_EFFECT_NAME_NOT_FOUND;
}

//############################################################################
void LaserBoy_space::load_background_bitmap(struct LaserBoy_bmp* bmp)
{
    bmp_copy(&background_bmp_2D, &background);
    bmp_copy(&background_bmp_3D, &background);
    bmp_scale_into_bmp(&background_bmp_3D, bmp, background_bmp_scale);    
    for(int y = 0; y < background_bmp_3D.yres; y++)
        for(int x = 0; x < background_bmp_3D.xres; x++)
            background_bmp_2D.put_pixel(&background_bmp_2D,
                                        x,
                                        background_bmp_3D.yres - y,
                                        background_bmp_2D.make_rgb(background_bmp_3D.r_from_rgb(background_bmp_3D.get_pixel_rgb(&background_bmp_3D, x, y)),
                                                                   background_bmp_3D.g_from_rgb(background_bmp_3D.get_pixel_rgb(&background_bmp_3D, x, y)),
                                                                   background_bmp_3D.b_from_rgb(background_bmp_3D.get_pixel_rgb(&background_bmp_3D, x, y))
                                                                  )
                                       );
    bmp_copy(&background_bmp_3D, &background);
    render_background_bitmap();
    show_background_bitmap = true;
    return;
}

//############################################################################
void LaserBoy_space::render_background_bitmap()
{
    if(track_bmp_view)
    {
        bool               rotate_view = (view_angle  != 0.0),
                           offset_view = (view_offset != 0.0),
                           scale_view  = (view_scale  != 1.0);
        u_int              bmp_color,
                           weight_x = ceil(view_scale.x),
                           weight_y = ceil(view_scale.y);
        int                x,
                           y,
                           xx,
                           yy;
        double             d_bmp_yres = bmp.yres - 1.0,
                           bmp_center = bmp.yres / 2.0;
        LaserBoy_3D_double upper_left,
                           upper_right,
                           lower_left,
                           delta_x,
                           delta_y,
                           pixel_row,
                           pixel,
                           angle  = -view_angle,
                           offset =  view_offset * (bmp.yres / 65535.0);
        angle.y  = -angle.y;
        offset.y = -offset.y;
        upper_left  = LaserBoy_3D_double(-bmp_center, -bmp_center, 0.0);
        upper_right = LaserBoy_3D_double( bmp_center, -bmp_center, 0.0);
        lower_left  = LaserBoy_3D_double(-bmp_center,  bmp_center, 0.0);
        if(rotate_view)
        {
            upper_left  = rotate_vertex_xy(upper_left,  angle);
            upper_right = rotate_vertex_xy(upper_right, angle);
            lower_left  = rotate_vertex_xy(lower_left,  angle);
        }
        if(offset_view)
        {
            upper_left.x  += offset.x;
            upper_left.y  += offset.y;
            upper_right.x += offset.x;
            upper_right.y += offset.y;
            lower_left.x  += offset.x;
            lower_left.y  += offset.y;
        }
        if(scale_view)
        {
            upper_left.x  *= view_scale.x;
            upper_left.y  *= view_scale.y;
            upper_right.x *= view_scale.x;
            upper_right.y *= view_scale.y;
            lower_left.x  *= view_scale.x;
            lower_left.y  *= view_scale.y;
        }
        upper_left  += bmp_center;
        upper_right += bmp_center;
        lower_left  += bmp_center;
        delta_x = upper_right - upper_left;
        delta_y = lower_left  - upper_left;
        bmp_copy(&background_bmp_3D, &background); // erase it!
        if(!show_quad_view)
            for(y = 0; y < (int)bmp.yres; y++)
            {
                pixel_row = upper_left + delta_y * (y / d_bmp_yres);
                for(x = 0; x < (int)bmp.yres; x++)
                {
                    pixel = pixel_row + (delta_x * (x / d_bmp_yres));
                    xx = (int)round(pixel.x);
                    yy = (int)round(pixel.y);
                    if(xx <= (int)bmp.xres && xx >= 0 && yy <= (int)bmp.yres && yy >= 0)
                    {
                        bmp_color = background_bmp_2D.make_rgb(background_bmp_2D.r_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                               background_bmp_2D.g_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                               background_bmp_2D.b_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y))
                                                              );
                        bmp_put_rectangle(&background_bmp_3D,
                                          xx,
                                          yy,
                                          xx + weight_x,
                                          yy + weight_y,
                                          bmp_color,
                                          true
                                         );
                    }
                }
            }
        else // end if(!show_quad_view)
        {
            for(y = 0; y < (int)bmp.yres; y++)
            {
                pixel_row = upper_left + delta_y * (y / d_bmp_yres);
                for(x = 0; x < (int)bmp.yres; x++)
                {
                    pixel = pixel_row + (delta_x * (x / d_bmp_yres));
                    xx = (int)round(pixel.x / 2);
                    yy = (int)round(pixel.y / 2) + bmp_center;
                    bmp_color = background_bmp_2D.make_rgb(background_bmp_2D.r_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                           background_bmp_2D.g_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                           background_bmp_2D.b_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y))
                                                          );
                    if(xx <= (int)bmp.xres && xx >= 0 && yy <= (int)bmp.yres && yy >= 0)
                    {
                        bmp_put_rectangle(&background_bmp_3D,
                                          xx,
                                          yy,
                                          xx + weight_x,
                                          yy + weight_y,
                                          bmp_color,
                                          true
                                       );
                    }
                }
            }
            for(y = 0; y < (int)bmp.yres; y += 2)
                for(x = 0; x < (int)bmp.yres; x += 2)
                {
                    bmp_color = background_bmp_2D.make_rgb(background_bmp_2D.r_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                           background_bmp_2D.g_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y)),
                                                           background_bmp_2D.b_from_rgb(background_bmp_2D.get_pixel_rgb(&background_bmp_2D, x, y))
                                                          );
                    bmp.put_pixel(&background_bmp_3D, x / 2 + bmp_center, y / 2, bmp_color);
                }
        } // end if(!show_quad_view) [else]
    } // end if(track_bmp_view)
    else
        bmp_copy(&background_bmp_3D, &background_bmp_2D);
}

//############################################################################
bool LaserBoy_space::load_background_bitmap(const string& file, bool scale_to_screen)
{
    char file_name[256];
    struct LaserBoy_bmp bmp = {0};
    background_bmp_negative = false;
    strcpy(file_name, file.c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        if(scale_to_screen)
            background_bmp_scale =   bmp.yres > bmp.xres
                                   ? (double)LaserBoy_space::bmp.yres / (double)bmp.yres
                                   : (double)LaserBoy_space::bmp.yres / (double)bmp.xres;
        load_background_bitmap(&bmp);
        bmp_free(&bmp);
        bg_file_name = file;
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_space::reload_background_bitmap()
{
    if(bg_file_name != ".")
    {
        char file_name[256];
        struct LaserBoy_bmp bmp = {0};
        strcpy(file_name, bg_file_name.c_str());
        if(bmp_from_file(&bmp, file_name))
        {
            if(background_bmp_negative)
                bmp_negative(&bmp);
            load_background_bitmap(&bmp);
            bmp_free(&bmp);
            return true;
        }
        return false;
    }
    return false;
}

//############################################################################
void LaserBoy_space::clear_background_bitmap()
{
    bmp_fill(&background       , bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp_2D, bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp_3D, bg_color.r, bg_color.g, bg_color.b);
    bg_file_name = ".";
    show_background_bitmap = false;
    background_bmp_scale = 1.0;
    return;
}

//############################################################################
void LaserBoy_space::recolor_background()
{
    bmp_fill(&background       , bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp_2D, bg_color.r, bg_color.g, bg_color.b);
    bmp_fill(&background_bmp_3D, bg_color.r, bg_color.g, bg_color.b);
    if(bg_file_name != ".")
        load_background_bitmap(bg_file_name, false);
    return;
}

//############################################################################
void LaserBoy_space::load_color_rescale_files()
{
    int flags = color_rescales_flags();
    if(flags & LASERBOY_COLOR_RESCALE_R)
        load_color_rescale_file(0);
    if(flags & LASERBOY_COLOR_RESCALE_G)
        load_color_rescale_file(1);
    if(flags & LASERBOY_COLOR_RESCALE_B)
        load_color_rescale_file(2);
    if(flags & LASERBOY_COLOR_RESCALE_I)
        load_color_rescale_file(3);
    return;
}

//############################################################################
LaserBoy_Rescale_Error_Code LaserBoy_space::load_color_rescale_file(size_t color_channel)
{
    int            i,
                   next_char,
                   counter = 0;
    double         number;
    std::ifstream  in;
    //------------------------------------------------------------------------
    if(color_channel > 3)
        return LASERBOY_RESCALE_BAD_CHANNEL_INDEX;
    //------------------------------------------------------------------------
    in.open(color_rescale_file[color_channel].c_str(), ios::in);
    if(in.is_open())
    {
        while(in.good() && counter < 256)
        {
            next_char = in.peek();
            if(next_char == '#') // beginning of a comment line
            {
                while(in.good())
                {
                    next_char = in.get();
                    if(next_char == '\n')
                        break;
                }
            }
            else if(    !isdigit(next_char)
                     && next_char != '-'
                   )
            {
                in.get(); // eat it!
            }
            else // it is a digit or '-'
            {
                in >> number;
                if(counter == 0 && number != 0)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_NONZERO_ZERO;
                }
                if(number < -32766 || number > 32766)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_IMPROPER_DATA;
                }
                if(short(number) & 0x0001)
                {
                    color_rescale_file[color_channel] = ".";
                    return LASERBOY_RESCALE_ODD_DATA;
                }
                color_rescale[color_channel][counter] = (short)number;
                for(i = 0; i < counter; i++)
                {
                    if(    color_rescale[color_channel][i]
                        == color_rescale[color_channel][counter]
                      )
                    {
                        color_rescale_file[color_channel] = ".";
                        return LASERBOY_RESCALE_REDUNDANT_DATA;
                    }
                }
                counter++;
            }
        }
        if(counter < 256)
        {
            color_rescale_file[color_channel] = ".";
            return LASERBOY_RESCALE_INCOMPLETE_DATA;
        }
        return LASERBOY_OK; // zero!
    }
    //------------------------------------------------------------------------
    color_rescale_file[color_channel] = ".";
    return LASERBOY_RESCALE_FILE_FAILED;
}

//############################################################################
LaserBoy_Rescale_Error_Code LaserBoy_space::save_gamma_index_as_txt(double gamma)
{
    int            prev     = 0,
                   next     = 0,
                   i;
    string         out_name =   LASERBOY_RESCALE_SHARE
                              + string("gamma_")
                              + to_string(gamma)
                              + string(".txt");
    std::ofstream  out;
    out.open(out_name.c_str(), ios::out);
    if(out.is_open())
    {
        out << "#\n"
               "# LaserBoy color rescale from gamma\n"
               "# "
            << LASERBOY_RESCALE_SHARE
            << string("gamma_")
            << to_string(gamma)
            << string(".txt")
            << "\n#"
            << ENDL
            << ENDL;
        out << (short)0 << ENDL;
        for(i = 1; i < 256; i++)
        {
            next = ((short)(   32766.0
                             * pow(   i / (double)255.0
                                    , 1.0 / gamma
                                  )
                             + 0.5
                            ) & 0b1111111111111110
                   );
            if(next == prev)
            {
                out << "# fudge (+2)" << ENDL;
                next += 2;
            }
                next += 2;
            if(next > 32766)
            {
                out.close();
                std::remove(out_name.c_str());
                return LASERBOY_RESCALE_REDUNDANT_DATA;
            }
            out << next << ENDL;
            prev = next;
        }
        out << ENDL
            << ENDL;
        out.close();
        return LASERBOY_OK;
    }
    return LASERBOY_RESCALE_FILE_FAILED;
}

//############################################################################
int LaserBoy_space::color_rescales_flags()
{
    if(auto_apply_rescales)
    {
        int flags = 0;
        if(color_rescale_file[0] != ".")
            flags |= LASERBOY_COLOR_RESCALE_R;
        if(color_rescale_file[1] != ".")
            flags |= LASERBOY_COLOR_RESCALE_G;
        if(color_rescale_file[2] != ".")
            flags |= LASERBOY_COLOR_RESCALE_B;
        if(color_rescale_file[3] != ".")
            flags |= LASERBOY_COLOR_RESCALE_I;
        return flags;
    }
    //------------------------------------------------------------------------
    return 0;
}

//############################################################################
void LaserBoy_space::set_wav_out_header(LaserBoy_wav_header &header, bool optimized, bool timed)
{
    int   r_index   = p_space->wav_channel_map.find('r'),
          g_index   = p_space->wav_channel_map.find('g'),
          b_index   = p_space->wav_channel_map.find('b'),
          i_index   = p_space->wav_channel_map.find('i'),
          O_index_1 = p_space->wav_channel_map.find('0'),
          O_index_2 = p_space->wav_channel_map.find('0', O_index_1 + 1);

    u_int i,
          j;
    //------------------------------------------------------------------------
    header.LaserBoy_wav_mode =   LASERBOY_WAVE_POSITIVE
                                | LASERBOY_WAVE_END_OF_FRAME
                                | LASERBOY_WAVE_UNIQUE_FRAME
                                | LASERBOY_WAVE_OFFSETS
                                | LASERBOY_WAVE_SIGNAL_MATRIX
                                | LASERBOY_SIGNAL_BIT_RESOLUTION
                                | LASERBOY_WAVE_NUM_FRAMES
                                | p_space->color_rescales_flags();
    header.sample_rate        = p_space->sample_rate;
    header.num_channels       = p_space->wav_channel_map.size();
    header.version            = LASERBOY_WAVE_VERSION;
    header.channel_map        = p_space->wav_channel_map;
    //------------------------------------------------------------------------
    if(p_space->invert_wav_output)
        header.LaserBoy_wav_mode &= ~LASERBOY_WAVE_POSITIVE;
    //------------------------------------------------------------------------
    if(r_index == (int)string::npos)
        header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_B;
    //------------------------------------------------------------------------
    if(g_index == (int)string::npos)
        header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_G;
    //------------------------------------------------------------------------
    if(b_index == (int)string::npos)
        header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_B;
    //------------------------------------------------------------------------
    if(i_index == (int)string::npos)
        header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_I;
    //------------------------------------------------------------------------
    for(i = 0; i < header.num_channels; i++)
    {
        switch(header.channel_map[i])
        {
            case 'X':
                header.signal_id [i] = LASERBOY_SIGNAL_X_POSITION; // default values
                header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_X];
                break;
            //----------------------------------------------------------------
            case 'Y':
                header.signal_id [i] = LASERBOY_SIGNAL_Y_POSITION; // default values
                header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_Y];
                break;
            //----------------------------------------------------------------
            case 'Z':
                header.signal_id [i] = LASERBOY_SIGNAL_Z_POSITION; // default values
                header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_Z];
                break;
            //----------------------------------------------------------------
            case 'r':
                header.signal_id [i] = LASERBOY_SIGNAL_RED_ANALOG;
                if(header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
                {
                    for(j = 0; j < 256; j++)
                        header.color_rescale_r[j] = p_space->color_rescale_r[j];
                    header.resolution[i] = 14;
                }
                else
                    header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_R];
                break;
            //----------------------------------------------------------------
            case 'g':
                header.signal_id [i] = LASERBOY_SIGNAL_GREEN_ANALOG; // default values
                if(header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
                {
                    for(j = 0; j < 256; j++)
                        header.color_rescale_g[j] = p_space->color_rescale_g[j];
                    header.resolution[i] = 14;
                }
                else
                    header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_G];
                break;
            //----------------------------------------------------------------
            case 'b':
                b_index = i;
                header.signal_id [i] = LASERBOY_SIGNAL_BLUE_ANALOG; // default values
                if(header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
                {
                    for(j = 0; j < 256; j++)
                        header.color_rescale_b[j] = p_space->color_rescale_b[j];
                    header.resolution[i] = 14;
                }
                else
                    header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_B];
                break;
            //----------------------------------------------------------------
            case 'i':
                header.signal_id [i] = wav_channel_i; // default values
                if(header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
                {
                    for(j = 0; j < 256; j++)
                        header.color_rescale_i[j] = p_space->color_rescale_i[j];
                    header.resolution[i] = 14;
                }
                else
                    header.resolution[i] = 16 - p_space->signal_bit_mask[LASERBOY_CHANNEL_I];
                break;
            //----------------------------------------------------------------
            case '0':
                header.signal_id [i] = LASERBOY_NO_SIGNAL; // default values
                header.resolution[i] = 16;
                break;
        } // end switch(header.channel_map[i])
    } // end for(i = 0; i < num_channels; i++)
    //------------------------------------------------------------------------
    if(r_index >= 0)
    {
        header.LSB_tag[r_index] = LASERBOY_LSB_END_OF_FRAME;
        if(g_index >= 0)
            header.LSB_tag[g_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(b_index >= 0)
            header.LSB_tag[b_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(i_index >= 0)
            header.LSB_tag[i_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(O_index_1 >= 0)
            header.LSB_tag[O_index_1] = LASERBOY_LSB_UNIQUE_FRAME;
    }
    else if(g_index >= 0)
    {
        header.LSB_tag[g_index] = LASERBOY_LSB_END_OF_FRAME;
        if(b_index >= 0)
            header.LSB_tag[b_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(i_index >= 0)
            header.LSB_tag[i_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(O_index_1 >= 0)
            header.LSB_tag[O_index_1] = LASERBOY_LSB_UNIQUE_FRAME;
    }
    else if(b_index >= 0)
    {
        header.LSB_tag[b_index] = LASERBOY_LSB_END_OF_FRAME;
        if(i_index >= 0)
            header.LSB_tag[i_index] = LASERBOY_LSB_UNIQUE_FRAME;
        else if(O_index_1 >= 0)
            header.LSB_tag[O_index_1] = LASERBOY_LSB_UNIQUE_FRAME;
    }
    else if(i_index >= 0)
    {
        header.LSB_tag[i_index] = LASERBOY_LSB_END_OF_FRAME;
        if(O_index_1 >= 0)
            header.LSB_tag[O_index_1] = LASERBOY_LSB_UNIQUE_FRAME;
    }
    else if(O_index_1 >= 0)
    {
        header.LSB_tag[O_index_1] = LASERBOY_LSB_END_OF_FRAME;
        if(O_index_2 >= 0)
            header.LSB_tag[O_index_2] = LASERBOY_LSB_UNIQUE_FRAME;
    }
    //------------------------------------------------------------------------
    if(optimized)
    {
        header.LaserBoy_wav_mode |= LASERBOY_WAVE_OPTIMIZED;
        header.parms.lit_dwell_overhang     =        p_space->lit_dwell_overhang;
        header.parms.lit_delta_max          = (float)p_space->lit_delta_max;
        header.parms.blank_delta_max        = (float)p_space->blank_delta_max;
        header.parms.max_dwell_microsec     = (float)p_space->max_dwell_microsec;
        header.parms.max_wag_microsec       = (float)p_space->max_wag_microsec;
        header.parms.insignificant_distance = (float)p_space->insignificant_distance;
        header.parms.insignificant_angle    = (float)p_space->insignificant_angle;
        header.parms.frames_per_second      =    (timed)
                                               ? ((float)p_space->frames_per_second)
                                               : (0);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_space::apply_wav_offsets_prep(const string& file)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    //------------------------------------------------------------------------
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wav_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wav_in.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    if(wav_in_header.version == "!LaserBoy!")
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    wav_out.open(file.c_str(), ios::out | ios::binary);
    if(!wav_out.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    apply_wav_offsets(wav_in, wav_out, wav_in_header);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::overwirte_wav_offsets(const string& file)
{
    std::fstream  wav_fstream;
    wav_fstream.open(file.c_str(), ios::in | ios::out | ios::binary);
    if(!wav_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_header;
    if(!wav_header.from_fstream_wav(wav_fstream, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(    wav_header.version == "!LaserBoy!"
        || !(wav_header.LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
      )
        return false;
    //------------------------------------------------------------------------
    for(u_int i = 0; i < wav_header.num_channels; i++)
    {
        switch((int)abs(wav_header.signal_id[i]))
        {
            case LASERBOY_SIGNAL_X_POSITION:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_X];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Y_POSITION:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_Y];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Z_POSITION:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_Z];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_RED_ANALOG:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_R];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_GREEN_ANALOG:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_G];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_BLUE_ANALOG:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_B];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_TTL:
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                wav_header.offset[i] = wav_offsets[LASERBOY_CHANNEL_I];
                break;
            //----------------------------------------------------------------
            case LASERBOY_NO_SIGNAL:
            default:
                wav_header.offset[i] = 0;
                break;
        } // end switch((int)abs(wav_header.signal_id[i]))
    } // end for(u_int i = 0; i < wav_header.num_channels; i++)
    wav_header.to_fstream_wav(wav_fstream);
    //------------------------------------------------------------------------
    wav_fstream.close();
    return true;
}

//############################################################################
bool LaserBoy_space::invert_wav_prep(const string& file)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wav_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wav_in.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    wav_out.open(file.c_str(), ios::out | ios::binary);
    if(!wav_out.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    invert_wav(wav_in, wav_out, wav_in_header);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::invert_signals_prep(const string& file)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wav_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wav_in.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    wav_out.open(file.c_str(), ios::out | ios::binary);
    if(!wav_out.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    invert_signals(wav_in, wav_out, wav_in_header);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::clear_wav_polarity_list(const string& file, bool global_flip_flop)
{
    std::fstream  wav_fstream;
    wav_fstream.open(file.c_str(), ios::in | ios::out | ios::binary);
    if(!wav_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_header;
    if(!wav_header.from_fstream_wav(wav_fstream, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(wav_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    if(global_flip_flop)
    {
        if(wav_header.LaserBoy_wav_mode & LASERBOY_WAVE_POSITIVE)
            wav_header.LaserBoy_wav_mode &= ~LASERBOY_WAVE_POSITIVE; // flip it to negative
        else
            wav_header.LaserBoy_wav_mode |= LASERBOY_WAVE_POSITIVE; // flip it to positive
    }
    //------------------------------------------------------------------------
    if(wav_header.LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
        for(u_int i = 0; i < wav_header.num_channels; i++)
            wav_header.signal_id[i] = (int)abs(wav_header.signal_id[i]);
    //------------------------------------------------------------------------
    wav_header.to_fstream_wav(wav_fstream);
    wav_fstream.close();
    return true;
}

//############################################################################
bool LaserBoy_space::apply_color_rescales_prep(const string& file)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    wav_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wav_in.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(    wav_in_header.version == "!LaserBoy!"
        || wav_in_header.num_channels < 6
      )
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    wav_out.open(file.c_str(), ios::out | ios::binary);
    if(!wav_out.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    apply_color_rescales(wav_in, wav_out, wav_in_header);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    remove((file + ".tmp").c_str());
    return true;
}

//############################################################################
bool LaserBoy_space::omit_color_rescales(const string& file)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    if(rename(file.c_str(), (file + ".tmp").c_str()))
        return false;
    //------------------------------------------------------------------------
    wav_in.open((file + ".tmp").c_str(), ios::in | ios::binary);
    if(!wav_in.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(    wav_in_header.version == "!LaserBoy!"
        || wav_in_header.num_channels < 6
      )
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    wav_out.open(file.c_str(), ios::out | ios::binary);
    if(!wav_out.is_open())
    {
        undo_wav_temp(wav_in, wav_out, file);
        return false;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_sample sample(wav_in_header.num_channels);
    LaserBoy_wav_header wav_out_header(wav_in_header);
    //------------------------------------------------------------------------
    wav_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wav_out_header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_R;
    wav_out_header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_G;
    wav_out_header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_B;
    wav_out_header.LaserBoy_wav_mode &= ~LASERBOY_COLOR_RESCALE_I;
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("omitting color rescale in wav");
    while(sample.from_fstream_wav(wav_in))
    {
        sample.to_fstream_wav(wav_out, wav_out_header);
        if(!(wav_out_header.num_samples % 4800))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    remove((file + ".tmp").c_str());
    return true;
}


//############################################################################
bool LaserBoy_space::save_wav_rescales_as_txt(const string& file, const string& out_name)
{
    bool           saved_something = false;
    int            i;
    std::fstream   wav_fstream;
    std::ofstream  out;
    wav_fstream.open(file.c_str(), ios::in | ios::binary);
    if(!wav_fstream.is_open())
        return false;
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_header;
    if(!wav_header.from_fstream_wav(wav_fstream, file))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(wav_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    if(wav_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
    {
        out.open((out_name + "_r.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wav\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wav_header.color_rescale_r[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wav_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
    {
        out.open((out_name + "_g.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wav\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wav_header.color_rescale_g[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wav_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
    {
        out.open((out_name + "_b.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wav\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wav_header.color_rescale_b[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    if(wav_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
    {
        out.open((out_name + "_i.txt").c_str(), ios::out);
        out << "#\n"
               "# color rescale from LaserBoy formatted wav\n"
               "#\n"
               "# "
            << file
            << "\n#"
            << ENDL
            << ENDL;
        for(i = 0; i < 256; i++)
            out << wav_header.color_rescale_i[i] << ENDL;
        out << ENDL
            << ENDL;
        out.close();
        saved_something = true;
    }
    //------------------------------------------------------------------------
    wav_fstream.close();
    return saved_something;
}

//############################################################################
bool LaserBoy_space::bit_resolution_to_wav_prep(const string& file_in, const string& file_out)
{
    std::fstream  wav_in;
    std::fstream  wav_out;
    wav_in.open(file_in.c_str(), ios::in | ios::binary);
    //------------------------------------------------------------------------
    LaserBoy_wav_header wav_in_header;
    if(!wav_in_header.from_fstream_wav(wav_in, file_in))
    {
        frame_set_error |= LASERBOY_WAV_HEADER_ERROR;
        return false;
    }
    //------------------------------------------------------------------------
    if(wav_in_header.version == "!LaserBoy!")
        return false;
    //------------------------------------------------------------------------
    wav_out.open(file_out.c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    bit_resolution_to_wav(wav_in, wav_out, wav_in_header);
    //------------------------------------------------------------------------
    wav_in.close();
    wav_out.close();
    return true;
}

//############################################################################
void LaserBoy_space::apply_wav_offsets(std::fstream&       wav_in,
                                       std::fstream&       wav_out,
                                       LaserBoy_wav_header wav_in_header
                                      )
{
    int                            i                     ,
                                   span                  ,
                                   last                  ,
                                   offset_in_max         ,
                                   offset_in_min         ,
                                   offset_applied_max    ,
                                   offset_applied_min    ,
                                   offset_applied     [8];

    LaserBoy_wav_sample_pointer   roll_over             ,
                                  *samples_window        ;
    LaserBoy_wav_sample           wav_sample(wav_in_header.num_channels);
    LaserBoy_wav_header           wav_out_header(wav_in_header);
    //------------------------------------------------------------------------
    wav_out_header.num_samples = 0;
    wav_out_header.LaserBoy_wav_mode |= LASERBOY_WAVE_OFFSETS;
    //------------------------------------------------------------------------
    for(i = 0; i < wav_in_header.num_channels; i++)
    {
        switch((int)abs(wav_in_header.signal_id[i]))
        {
            case LASERBOY_SIGNAL_X_POSITION:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_X];
                break;
            case LASERBOY_SIGNAL_Y_POSITION:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_Y];
                break;
            case LASERBOY_SIGNAL_Z_POSITION:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_Z];
                break;
            case LASERBOY_SIGNAL_RED_ANALOG:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_R];
                break;
            case LASERBOY_SIGNAL_GREEN_ANALOG:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_G];
                break;
            case LASERBOY_SIGNAL_BLUE_ANALOG:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_B];
                break;
            case LASERBOY_SIGNAL_MONO_TTL:
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                offset_applied[i] = wav_in_header.offset[i] - wav_offsets[LASERBOY_CHANNEL_I];
                break;
            case LASERBOY_NO_SIGNAL:
            default:
                offset_applied[i] = 0;
                break;
        } // end switch((int)abs(wav_in_header.signal_id[i]))
    } // end for(i = 0; i < wav_in_header.num_channels; i++)
    //------------------------------------------------------------------------
    offset_in_min      = wav_offsets   [0];
    offset_in_max      = wav_offsets   [0];
    offset_applied_min = offset_applied[0];
    offset_applied_max = offset_applied[0];
    //------------------------------------------------------------------------
    for(i = 0; i < wav_in_header.num_channels; i++)
    {
        if(wav_offsets  [i] > offset_in_max     ) offset_in_max      = wav_offsets  [i];
        if(wav_offsets  [i] < offset_in_min     ) offset_in_min      = wav_offsets  [i];
        if(offset_applied[i] > offset_applied_max) offset_applied_max = offset_applied[i];
        if(offset_applied[i] < offset_applied_min) offset_applied_min = offset_applied[i];
    }
    //------------------------------------------------------------------------
    span = (offset_applied_max - offset_applied_min) + 1;
    last = span - 1;
    //------------------------------------------------------------------------
    for(i = 0; i < wav_in_header.num_channels; i++)
    {
        switch((int)abs(wav_in_header.signal_id[i]))
        {
            case LASERBOY_SIGNAL_X_POSITION:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_X]);
                break;
            case LASERBOY_SIGNAL_Y_POSITION:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_Y]);
                break;
            case LASERBOY_SIGNAL_Z_POSITION:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_Z]);
                break;
            case LASERBOY_SIGNAL_RED_ANALOG:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_R]);
                break;
            case LASERBOY_SIGNAL_GREEN_ANALOG:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_G]);
                break;
            case LASERBOY_SIGNAL_BLUE_ANALOG:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_B]);
                break;
            case LASERBOY_SIGNAL_MONO_TTL:
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                wav_out_header.offset[i] = -(offset_in_max - wav_offsets[LASERBOY_CHANNEL_I]);
                break;
            case LASERBOY_NO_SIGNAL:
            default:
                wav_out_header.offset[i] = 0;
                break;
        } // end switch((int)abs(wav_in_header.signal_id[i]))
        offset_applied[i] = offset_applied_max - offset_applied[i];
    } // end for(i = 0; i < wav_in_header.num_channels; i++)
    //------------------------------------------------------------------------
    samples_window = new LaserBoy_wav_sample_pointer[span];
    for(i = 0; i < span; i++)
        samples_window[i] = new LaserBoy_wav_sample(wav_in_header.num_channels);
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("applying sample offsets");
    while(true)
    {
        if(last) // span > 1
        {
            roll_over = samples_window[0];
            for(i = 0; i < last; i++)
                samples_window[i] = samples_window[i + 1];
            samples_window[last] = roll_over;
        }
        if(!samples_window[last]->from_fstream_wav(wav_in))
            break;
        for(i = 0; i < wav_in_header.num_channels; i++) // each channel
            wav_sample.channel[i] = samples_window[offset_applied[i]]->channel[i];
        wav_sample.to_fstream_wav(wav_out, wav_out_header); // write it to disk
        if(!(wav_out_header.num_samples % wav_out_header.sample_rate))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    for(i = 0; i < span; i++)
        delete samples_window[i];
    delete samples_window;
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::invert_wav(std::fstream&       wav_in,
                                std::fstream&       wav_out,
                                LaserBoy_wav_header wav_in_header
                               )
{
    LaserBoy_wav_sample wav_sample    (wav_in_header.num_channels);
    LaserBoy_wav_header wav_out_header(wav_in_header);
    wav_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    if(wav_in_header.version != "!LaserBoy!")
    {
        if(wav_in_header.LaserBoy_wav_mode & LASERBOY_WAVE_POSITIVE)
            wav_out_header.LaserBoy_wav_mode &= ~LASERBOY_WAVE_POSITIVE;
        else
            wav_out_header.LaserBoy_wav_mode |= LASERBOY_WAVE_POSITIVE;
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("inverting wav");
    while(wav_sample.from_fstream_wav_inverted(wav_in))
    {
        wav_sample.to_fstream_wav(wav_out, wav_out_header);
        if(!(wav_out_header.num_samples % wav_out_header.sample_rate))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::invert_signals(std::fstream&       wav_in,
                                    std::fstream&       wav_out,
                                    LaserBoy_wav_header wav_in_header
                                   )
{
    u_int                i;
    LaserBoy_wav_sample  wav_sample    (wav_in_header.num_channels);
    LaserBoy_wav_header  wav_out_header(wav_in_header);
    int                  polarity[8];
    //------------------------------------------------------------------------
    wav_out_header.num_samples = 0;
    for(i = 0; i < 8; i++)
        polarity[i] = 0;
    //------------------------------------------------------------------------
    if(wav_in_header.version != "!LaserBoy!")
    {
        wav_out_header.LaserBoy_wav_mode |= LASERBOY_WAVE_SIGNAL_MATRIX;
        for(i = 0; i < wav_out_header.num_channels; i++)
        {
            switch((int)abs(wav_out_header.signal_id[i]))
            {
                case LASERBOY_SIGNAL_X_POSITION:
                    if(signal_polarity[LASERBOY_CHANNEL_X] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_Y_POSITION:
                    if(signal_polarity[LASERBOY_CHANNEL_Y] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_Z_POSITION:
                    if(signal_polarity[LASERBOY_CHANNEL_Z] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_RED_ANALOG:
                    if(signal_polarity[LASERBOY_CHANNEL_R] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_GREEN_ANALOG:
                    if(signal_polarity[LASERBOY_CHANNEL_G] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_BLUE_ANALOG:
                    if(signal_polarity[LASERBOY_CHANNEL_B] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_SIGNAL_MONO_TTL:
                case LASERBOY_SIGNAL_MONO_OR_ANALOG:
                case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
                case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
                case LASERBOY_SIGNAL_MONO_O_SCOPE:
                    if(signal_polarity[LASERBOY_CHANNEL_I] == 1)
                    {
                        wav_out_header.signal_id[i] = -wav_out_header.signal_id[i];
                        polarity[i] = 1;
                    }
                    break;
                //------------------------------------------------------------
                case LASERBOY_NO_SIGNAL:
                default:
                    polarity[i] = 0;
                    break;
            } // end switch((int)abs(wav_out_header.signal_id[i]))
        } // end for(u_int i = 0; i < wav_out_header.num_channels; i++)
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("inverting signals");
    while(wav_sample.from_fstream_wav_polarity(wav_in, polarity))
    {
        wav_sample.to_fstream_wav(wav_out, wav_out_header);
        if(!(wav_out_header.num_samples % wav_out_header.sample_rate))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::bit_resolution_to_wav(std::fstream&       wav_in,
                                           std::fstream&       wav_out,
                                           LaserBoy_wav_header wav_in_header
                                          )
{
    u_int                i,
                         bit_mask[8];
    LaserBoy_wav_sample  wav_sample    (wav_in_header.num_channels);
    LaserBoy_wav_header  wav_out_header(wav_in_header);
    //------------------------------------------------------------------------
    wav_in_header.LaserBoy_wav_mode |= LASERBOY_WAVE_SIGNAL_MATRIX;
    wav_in_header.LaserBoy_wav_mode |= LASERBOY_SIGNAL_BIT_RESOLUTION;
    wav_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    for(i = 0; i < 8; i++)
        bit_mask[i] = 0;
    //------------------------------------------------------------------------
    for(i = 0; i < wav_out_header.num_channels; i++)
    {
        switch((int)abs(wav_out_header.signal_id[i]))
        {
            case LASERBOY_SIGNAL_X_POSITION:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_X];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_X];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Y_POSITION:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_Y];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_Y];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_Z_POSITION:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_Z];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_Z];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_RED_ANALOG:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_R];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_R];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_GREEN_ANALOG:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_G];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_G];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_BLUE_ANALOG:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_B];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_B];
                break;
            //----------------------------------------------------------------
            case LASERBOY_SIGNAL_MONO_TTL:
            case LASERBOY_SIGNAL_MONO_OR_ANALOG:
            case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
            case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
            case LASERBOY_SIGNAL_MONO_O_SCOPE:
                wav_out_header.resolution[i] = 16 - signal_bit_mask[LASERBOY_CHANNEL_I];
                bit_mask[i] = signal_bit_mask[LASERBOY_CHANNEL_I];
                break;
            //----------------------------------------------------------------
            case LASERBOY_NO_SIGNAL:
            default:
                wav_out_header.resolution[i] = 16;
                bit_mask[i] = 0;
                break;
        } // end switch((int)abs(wav_out_header.signal_id[i]))
    } // end for(i = 0; i < wav_out_header.num_channels; i++)
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("bit reducing wav");
    while(wav_sample.from_fstream_wav_bit_resolution(wav_in, bit_mask))
    {
        wav_sample.to_fstream_wav(wav_out, wav_out_header);
        if(!(wav_out_header.num_samples % wav_out_header.sample_rate))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_space::apply_color_rescales(std::fstream&       wav_in,
                                          std::fstream&       wav_out,
                                          LaserBoy_wav_header wav_in_header
                                         )
{
    int i;
    LaserBoy_wav_sample sample_in (wav_in_header.num_channels),
                        sample_out(wav_in_header.num_channels);
    LaserBoy_wav_header wav_out_header(wav_in_header);
    wav_out_header.num_samples = 0;
    //------------------------------------------------------------------------
    wav_out_header.LaserBoy_wav_mode &= ~(    LASERBOY_COLOR_RESCALE_R
                                              | LASERBOY_COLOR_RESCALE_G
                                              | LASERBOY_COLOR_RESCALE_B
                                              | LASERBOY_COLOR_RESCALE_I
                                           ); // clear those bits
    wav_out_header.LaserBoy_wav_mode |= color_rescales_flags(); // set them


    if(wav_in_header.num_channels == 3)


    //------------------------------------------------------------------------
    if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
        for(i = 0; i < 256; i++)
            wav_out_header.color_rescale_r[i] = color_rescale_r[i];
    //------------------------------------------------------------------------
    if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
        for(i = 0; i < 256; i++)
            wav_out_header.color_rescale_g[i] = color_rescale_g[i];
    //------------------------------------------------------------------------
    if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
        for(i = 0; i < 256; i++)
            wav_out_header.color_rescale_b[i] = color_rescale_b[i];
    //------------------------------------------------------------------------
    if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
        for(i = 0; i < 256; i++)
            wav_out_header.color_rescale_i[i] = color_rescale_i[i];
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out); // take up the space!
    //------------------------------------------------------------------------
    p_GUI->display_state("color rescaling wav");
    while(sample_in.from_fstream_wav(wav_in))
    {
        if(wav_in_header.LaserBoy_wav_mode & LASERBOY_WAVE_NEGATIVE)
            sample_in.negate();
        sample_out = sample_in;
        //----------------------------------------------------------------
        if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
                sample_out.channel[2] = wav_out_header.color_rescale_r[rescale_to_index(wav_in_header.color_rescale_r, sample_in.channel[2])];
            else
                sample_out.channel[2] = wav_out_header.color_rescale_r[(sample_in.channel[2] >> 7) & 0x000000ff];
        }
        else
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
                sample_out.channel[2] = rescale_to_index(wav_in_header.color_rescale_r, sample_in.channel[2]) << 7;
            else
                sample_out.channel[2] = sample_in.channel[2];
        }
        //----------------------------------------------------------------
        if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
                sample_out.channel[3] = wav_out_header.color_rescale_g[rescale_to_index(wav_in_header.color_rescale_g, sample_in.channel[3])];
            else
                sample_out.channel[3] = wav_out_header.color_rescale_g[(sample_in.channel[3] >> 7) & 0x000000ff];
        }
        else
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
                sample_out.channel[3] = rescale_to_index(wav_in_header.color_rescale_g, sample_in.channel[3]) << 7;
            else
                sample_out.channel[3] = sample_in.channel[3];
        }
        //----------------------------------------------------------------
        if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
                sample_out.channel[4] = wav_out_header.color_rescale_b[rescale_to_index(wav_in_header.color_rescale_b, sample_in.channel[4])];
            else
                sample_out.channel[4] = wav_out_header.color_rescale_b[(sample_in.channel[4] >> 7) & 0x000000ff];
        }
        else
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
                sample_out.channel[4] = rescale_to_index(wav_in_header.color_rescale_b, sample_in.channel[4]) << 7;
            else
                sample_out.channel[4] = sample_in.channel[4];
        }
        //----------------------------------------------------------------
        if(wav_out_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
                sample_out.channel[5] = wav_out_header.color_rescale_i[rescale_to_index(wav_in_header.color_rescale_i, sample_in.channel[5])];
            else
                sample_out.channel[5] = wav_out_header.color_rescale_i[(sample_in.channel[5] >> 7) & 0x000000ff];
        }
        else
        {
            if(wav_in_header.LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
                sample_out.channel[5] = rescale_to_index(wav_in_header.color_rescale_i, sample_in.channel[5]) << 7;
            else
                sample_out.channel[5] = sample_in.channel[5];
        }
        //----------------------------------------------------------------
        if(wav_in_header.LaserBoy_wav_mode & LASERBOY_WAVE_NEGATIVE)
            sample_out.negate();
        sample_out.to_fstream_wav(wav_out, wav_out_header);
        if(!(wav_out_header.num_samples % 4800))
            p_GUI->display_progress(wav_in_header.num_samples - wav_out_header.num_samples);
    }
    //------------------------------------------------------------------------
    wav_out_header.to_fstream_wav(wav_out);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
