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
// LaserBoy_SDL_GUI.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SDL_GUI_DEFINITIONS__
#define __LASERBOY_SDL_GUI_DEFINITIONS__

//############################################################################
#ifdef __WITH_SDL2__
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "LaserBoy_TUI.hpp"

//############################################################################
class LaserBoy_SDL_GUI
{
public:
    LaserBoy_SDL_GUI(int x, int y);
    //------------------------------------------------------------------------
   ~LaserBoy_SDL_GUI();
    //------------------------------------------------------------------------
    //  BITMAP MEMORY AND SDL
    //------------------------------------------------------------------------
    void bmp_clone_SDL_Surface(struct LaserBoy_bmp* bmp, bool copy)
    {
        bmp_init(bmp, screen->w, screen->h, screen->format->BitsPerPixel);
        if(screen->format->BitsPerPixel <= 8)
            memcpy((void*)(bmp->palette), (const void*)(screen->format->palette->colors), bmp->palette_size);
        if(copy)
            memcpy((void*)(bmp->image), (const void*)(screen->pixels), bmp->image_size);
        return;
    }
    //------------------------------------------------------------------------
    void bmp_to_SDL_Surface(struct LaserBoy_bmp* bmp)
    {
        if(screen->format->BitsPerPixel <= 8)
            memcpy((void*)(screen->format->palette->colors),
                   (const void*)(bmp->palette),
                   bmp->palette_size
                  );
        memcpy((void*)(screen->pixels),
               (const void*)(bmp->image),
               bmp->image_size
              );
    #ifdef __WITH_SDL2__
        SDL_UpdateWindowSurface(window);
    #else
        SDL_UnlockSurface(screen);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_LockSurface(screen);
    #endif
        return;
    }
    //------------------------------------------------------------------------
    void SDL_Surface_to_bmp(struct LaserBoy_bmp* bmp)
    {
        if(screen->format->BitsPerPixel <= 8)
            memcpy((void*)(bmp->palette),
                   (const void*)(screen->format->palette->colors),
                   bmp->palette_size
                  );
        memcpy((void*)(bmp->image),
               (const void*)(screen->pixels),
               bmp->image_size
              );
        return;
    }
    //------------------------------------------------------------------------
    void display_space()
    {
        if(screen->format->BitsPerPixel <= 8)
            memcpy((void*)(screen->format->palette->colors),
                   (const void*)(space.bmp.palette),
                   space.bmp.palette_size
                  );
        memcpy((void*)(screen->pixels),
               (const void*)(space.bmp.image),
               space.bmp.image_size
              );
#ifdef __WITH_SDL2__
        SDL_UpdateWindowSurface(window);
#else
        SDL_UnlockSurface(screen);
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_LockSurface(screen);
#endif
        return;
    }
    //------------------------------------------------------------------------
    void display_state(const string& state)
    {
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = space.bmp.xres;
        rect.h = 8 * space.font_size_factor;
        bmp_printf(&(space.bmp),
                   0,
                   0,
                   0x00ffff00,
                   0x00004000,
                   0,
                   space.font_size_factor,
                   "             %s                                                                ", // end string
                   state.c_str()
                  );
        //----------------------------------------------------
        memcpy((void*)(screen->pixels),
               (const void*)(space.bmp.image),
               space.bmp.bytes_per_line * 8 * space.font_size_factor
              );
#ifdef __WITH_SDL2__
        SDL_UpdateWindowSurfaceRects(window, &rect, 1);
#else
        SDL_UnlockSurface(screen);
        SDL_UpdateRect(screen,
                       8 * space.font_size_factor * 12,
                       0,
                       space.bmp.xres - 8 * space.font_size_factor * 12,
                       8 * space.font_size_factor
                      );
        SDL_LockSurface(screen);
#endif
        return;
    }
    //------------------------------------------------------------------------
    void display_progress(int countdown)
    {
        if(!(countdown % 10))
        {
            bmp_printf(&(space.bmp),
                       0,
                       0,
                       0x00ffff00,
                       0x00004000,
                       0,
                       space.font_size_factor,
                       "%12d",
                       countdown
                      );
            //----------------------------------------------------
            memcpy((void*)(screen->pixels),
                   (const void*)(space.bmp.image),
                   space.bmp.bytes_per_line * 8 * space.font_size_factor
                  );
#ifdef __WITH_SDL2__
            SDL_Rect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = 8 * space.font_size_factor * 12;
            rect.h = 8 * space.font_size_factor;
            SDL_UpdateWindowSurfaceRects(window, &rect, 1);
#else
            SDL_UnlockSurface(screen);
            SDL_UpdateRect(screen,
                           0,
                           0,
                           8 * space.font_size_factor * 12,
                           8 * space.font_size_factor
                          );
            SDL_LockSurface(screen);
#endif
        }
        return;
    }
    //------------------------------------------------------------------------
    void display_txt_line_number(int line_number)
    {
        bmp_printf(&(space.bmp),
                   space.bmp.xres - 8 * space.font_size_factor * 12,
                   0,
                   0x00ffff00,
                   0x00004000,
                   0,
                   space.font_size_factor,
                   "%12d",
                   line_number
                  );
        //----------------------------------------------------
        memcpy((void*)(screen->pixels),
               (const void*)(space.bmp.image),
               space.bmp.bytes_per_line * 8 * space.font_size_factor
              );
#ifdef __WITH_SDL2__
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = space.bmp.xres;
        rect.h = 8 * space.font_size_factor;
        SDL_UpdateWindowSurfaceRects(window, &rect, 1);
#else
        SDL_UnlockSurface(screen);
        SDL_UpdateRect(screen,
                       space.bmp.xres - 8 * space.font_size_factor * 12,
                       0,
                       8 * space.font_size_factor * 12,
                       8 * space.font_size_factor
                      );
        SDL_LockSurface(screen);
#endif
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
    // SDL_key menu system
    //------------------------------------------------------------------------
#ifdef __WITH_SDL2__
    void    check_for_nav_keys                   (struct SDL_Keysym* sdl_keysym);
    void    check_for_digit_keys                 (struct SDL_Keysym* sdl_keysym);
#else
    void    check_for_nav_keys                   (SDLKey key);
    void    check_for_digit_keys                 (SDLKey key);
#endif
    void    animate_forward                      ();
    void    animate_reverse                      ();
    void    rotate_forward                       ();
    void    rotate_reverse                       ();
    void    animate_rotate_forward               ();
    void    animate_rotate_reverse               ();
    //------------------------------------------------------------------------
    void    system_settings_menu                 ();
    void    optimization_factors_menu            ();
    void    vector_editor_values_menu            ();
    void    timing_and_wave_settings_menu        ();
    void    display_options_menu                 ();
    void    edit_background_color_menu           ();
    void    edit_mono_text_color_menu            ();
    void    edit_rendered_blank_menu             ();
    void    edit_rendered_black_menu             ();
    void    signal_if_not_z_menu                 ();
    void    dots_setting_menu                    ();
    void    bit_resolution_menu                  ();
    void    visuals_settings_menu                ();
    void    file_type_specific_options_menu      ();
    void    apply_view_menu                      ();
    bool    frame_color_trans_menu               ();
    bool    palette_set_trans_menu               ();
    void    add_color_to_target_palette_menu     ();
    void    insert_color_in_target_palette_menu  ();
    void    edit_color_in_target_palette_menu    ();
    void    frame_transforms_menu                ();
    void    frame_set_transforms_menu            ();
    void    draw_color_blank_menu                ();
    void    move_scale_rotate_menu               ();
    void    direct_draw_segments_menu            ();
    void    effect_functions_menu                ();
    void    draw_point_menu                      ();
    void    draw_line_menu                       ();
    void    draw_rectangle_menu                  ();
    void    draw_polygon_menu                    ();
    void    draw_polyline_menu                   ();
    void    draw_polystar_menu                   ();
    void    draw_circular_arc_menu               ();
    void    draw_elliptical_arc_menu             ();
    void    draw_rhodonea_menu                   ();
    void    draw_epicycloid_menu                 ();
    void    draw_epitrochoid_menu                ();
    void    draw_hypotrochoid_menu               ();
    void    draw_hypocycloid_menu                ();
    void    draw_lissajous_menu                  ();
    void    draw_harmonograph_menu               ();
    void    draw_animated_harmonograph_menu      ();
    void    draw_mono_spaced_font_menu           ();
    void    draw_variable_spaced_font_menu       ();
    void    wave_utilities_menu                  ();
    void    split_or_join_waves_menu             ();
    void    wave_show_settings_menu              ();
    void    show_wave_options_menu               ();
    void    set_wave_offsets_menu                ();
    void    set_color_rescales_menu              ();
    void    apply_color_rescales_menu            ();
    void    omit_color_rescales_menu             ();
    void    set_signal_polarities_menu           ();
    void    clear_wave_polarities_menu           ();
    void    black_level_to_wave_menu             ();
    void    bit_resolution_to_wave_menu          ();
    void    join_XY_r_g_b_i_waves_menu           ();
    void    join_XY_rg_b_waves_menu              ();
    void    join_XY_rg_b_LR_waves_menu           ();
    void    add_audio_to_wave_menu               ();
    void    file_open_menu                       ();
    bool    ild_in_cd_menu                       ();
    bool    ild_file_open_menu                   ();
    bool    dxf_file_open_menu                   ();
    bool    wav_file_open_menu                   ();
    bool    wav_qm_open_menu                     (bool append);
    bool    wav_unformatted_open_menu            (bool append);
    bool    txt_file_open_menu                   ();
    bool    ctn_file_open_menu                   ();
    bool    bmp_file_open_menu                   ();
    void    save_as_file_menu                    ();
    bool    ild_out_cd_menu                      ();
    bool    save_as_ild_menu                     ();
    bool    save_as_dxf_menu                     ();
    bool    save_as_wav_menu                     ();
    bool    save_as_txt_menu                     ();
    bool    save_as_ctn_menu                     ();
    bool    save_as_bmp_menu                     ();
    //------------------------------------------------------------------------
    void    Laserboy_terminating                 ();
    int     start_menu_loop                      ();
    //------------------------------------------------------------------------
    bool    display_bmp_directory                (string bmp_dir, bool scale_to_screen);
    //------------------------------------------------------------------------
    bool    display_wave                         (string wav_file);
    bool    display_unframed_wave                (string wav_file);
    void    display_LaserBoy_wave  (std::fstream& in, LaserBoy_wave_header header);
    void    display_unframed_wave  (std::fstream& in, LaserBoy_wave_header header);
    //------------------------------------------------------------------------
#ifdef __WITH_SDL2__
    SDL_Keycode SDL_Keysym_to_char(struct SDL_Keysym* sdl_keysym)
    {
        if(sdl_keysym->mod & KMOD_SHIFT)
        {
            if(sdl_keysym->sym >= SDLK_a && sdl_keysym->sym <= SDLK_z)
                return toupper((char)sdl_keysym->sym);
            else if(sdl_keysym->sym == SDLK_1)
                return '!';
            else if(sdl_keysym->sym == SDLK_2)
                return '@';
            else if(sdl_keysym->sym == SDLK_3)
                return '#';
            else if(sdl_keysym->sym == SDLK_4)
                return '$';
            else if(sdl_keysym->sym == SDLK_5)
                return '%';
            else if(sdl_keysym->sym == SDLK_6)
                return '^';
            else if(sdl_keysym->sym == SDLK_7)
                return '&';
            else if(sdl_keysym->sym == SDLK_8)
                return '*';
            else if(sdl_keysym->sym == SDLK_9)
                return '(';
            else if(sdl_keysym->sym == SDLK_0)
                return ')';
            else if(sdl_keysym->sym == SDLK_BACKQUOTE)
                return '~';
            else if(sdl_keysym->sym == SDLK_MINUS)
                return '_';
            else if(sdl_keysym->sym == SDLK_EQUALS)
                return '+';
            else if(sdl_keysym->sym == SDLK_LEFTBRACKET)
                return '{';
            else if(sdl_keysym->sym == SDLK_RIGHTBRACKET)
                return '}';
            else if(sdl_keysym->sym == SDLK_BACKSLASH)
                return '|';
            else if(sdl_keysym->sym == SDLK_SEMICOLON)
                return ':';
            else if(sdl_keysym->sym == SDLK_QUOTE)
                return '"';
            else if(sdl_keysym->sym == SDLK_COMMA)
                return '<';
            else if(sdl_keysym->sym == SDLK_PERIOD)
                return '>';
            else if(sdl_keysym->sym == SDLK_SLASH)
                return '?';
        }
        return sdl_keysym->sym;
    }
#endif
    //------------------------------------------------------------------------
    bool           running,
                   prompt_escaped;
    u_int          xres,
                   yres;
    LaserBoy_TUI   space;
#ifdef __WITH_SDL2__
    SDL_Window    *window;
#endif
    SDL_Surface   *screen; // an SDL graphics window
    SDL_Event      event ; // UI input (keys)
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################