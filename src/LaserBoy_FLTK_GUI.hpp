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
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>

#include <chrono>
#include <deque>
#include <string>

#define LASERBOY_GUI_KEYDOWN 1
#define LASERBOY_GUI_MOUSEMOTION 2
#define LASERBOY_GUI_PRESSED 3
#define LASERBOY_GUI_RELEASED 4
#define LASERBOY_GUI_SYSTEM_WINDOW_MANAGER_EVENT 5
#define LASERBOY_GUI_QUIT 6
#define LASERBOY_GUI_IGNORE 0

#define LASERBOY_KEY_MOD_SHIFT FL_SHIFT

#define LASERBOY_KEY_ESCAPE FL_Escape
#define LASERBOY_KEY_RETURN FL_Enter
#define LASERBOY_KEY_DELETE FL_Delete
#define LASERBOY_KEY_BACKSPACE FL_BackSpace
#define LASERBOY_KEY_UP FL_Up
#define LASERBOY_KEY_DOWN FL_Down
#define LASERBOY_KEY_LEFT FL_Left
#define LASERBOY_KEY_RIGHT FL_Right
#define LASERBOY_KEY_LSHIFT FL_Shift_L
#define LASERBOY_KEY_RSHIFT FL_Shift_R
#define LASERBOY_KEY_LCTRL FL_Control_L
#define LASERBOY_KEY_RCTRL FL_Control_R
#define LASERBOY_KEY_LALT FL_Alt_L
#define LASERBOY_KEY_RALT FL_Alt_R
#define LASERBOY_KEY_PAGEUP FL_Page_Up
#define LASERBOY_KEY_PAGEDOWN FL_Page_Down
#define LASERBOY_KEY_HOME FL_Home
#define LASERBOY_KEY_END FL_End
#define LASERBOY_KEY_INSERT FL_Insert
#define LASERBOY_KEY_TAB FL_Tab
#define LASERBOY_KEY_F1 FL_F + 1
#define LASERBOY_KEY_F2 FL_F + 2
#define LASERBOY_KEY_F3 FL_F + 3
#define LASERBOY_KEY_F4 FL_F + 4
#define LASERBOY_KEY_F5 FL_F + 5
#define LASERBOY_KEY_F6 FL_F + 6
#define LASERBOY_KEY_F7 FL_F + 7
#define LASERBOY_KEY_F8 FL_F + 8
#define LASERBOY_KEY_F9 FL_F + 9
#define LASERBOY_KEY_F10 FL_F + 10
#define LASERBOY_KEY_F11 FL_F + 11
#define LASERBOY_KEY_F12 FL_F + 12
#define LASERBOY_KEY_BACKQUOTE '`'
#define LASERBOY_KEY_MINUS '-'
#define LASERBOY_KEY_EQUALS '='
#define LASERBOY_KEY_LEFTBRACKET '['
#define LASERBOY_KEY_RIGHTBRACKET ']'
#define LASERBOY_KEY_BACKSLASH '\\'
#define LASERBOY_KEY_SEMICOLON ';'
#define LASERBOY_KEY_QUOTE '\''
#define LASERBOY_KEY_COMMA ','
#define LASERBOY_KEY_PERIOD '.'
#define LASERBOY_KEY_SLASH '/'
#define LASERBOY_KEY_0 '0'
#define LASERBOY_KEY_1 '1'
#define LASERBOY_KEY_2 '2'
#define LASERBOY_KEY_3 '3'
#define LASERBOY_KEY_4 '4'
#define LASERBOY_KEY_5 '5'
#define LASERBOY_KEY_6 '6'
#define LASERBOY_KEY_7 '7'
#define LASERBOY_KEY_8 '8'
#define LASERBOY_KEY_9 '9'
#define LASERBOY_KEY_a 'a'
#define LASERBOY_KEY_z 'z'

typedef int LaserBoy_GUI_Keycode;
typedef int LaserBoy_GUI_Key;

struct LaserBoy_GUI_Keysym
{
    int sym;
    int mod;
    int unicode;
};

struct LaserBoy_GUI_Event
{
    int type;
    struct
    {
        LaserBoy_GUI_Keysym keysym;
    } key;
};

enum LaserBoy_FLTK_Viewport_Tool
{
    LASERBOY_FLTK_VIEWPORT_TOOL_SEGMENT,
    LASERBOY_FLTK_VIEWPORT_TOOL_VERTEX,
    LASERBOY_FLTK_VIEWPORT_TOOL_PAN_ORBIT
};

enum LaserBoy_FLTK_Viewport_Drag_Action
{
    LASERBOY_FLTK_VIEWPORT_DRAG_NONE,
    LASERBOY_FLTK_VIEWPORT_DRAG_MOVE_SELECTION,
    LASERBOY_FLTK_VIEWPORT_DRAG_MOVE_VERTEX,
    LASERBOY_FLTK_VIEWPORT_DRAG_PAN,
    LASERBOY_FLTK_VIEWPORT_DRAG_ORBIT,
    LASERBOY_FLTK_VIEWPORT_DRAG_RUBBER_BAND
};

struct LaserBoy_GUI_Rect
{
    int x;
    int y;
    int w;
    int h;
};

struct LaserBoy_GUI_Palette
{
    void* colors;
};

struct LaserBoy_GUI_PixelFormat
{
    int BitsPerPixel;
    LaserBoy_GUI_Palette* palette;
};

struct LaserBoy_Screen_Buffer
{
    int w;
    int h;
    LaserBoy_GUI_PixelFormat* format;
    void* pixels;
};

inline void laserboy_gui_delay(unsigned int ms)
{
    Fl::wait(ms / 1000.0);
}

inline int lock_screen_buffer(LaserBoy_Screen_Buffer*)
{
    return 0;
}

inline void unlock_screen_buffer(LaserBoy_Screen_Buffer*)
{
}

inline unsigned int LaserBoy_GetTicks()
{
    using clock = std::chrono::steady_clock;
    static const clock::time_point start = clock::now();
    return (unsigned int)std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();
}

#include "LaserBoy_TUI.hpp"

//############################################################################
// One entry on the undo / redo stacks.
//
// The viewport view (pan / zoom / orbit) is part of the document in LaserBoy:
// it is literally the camera the laser show is rendered through, and it can
// change between frames to drive animation. So view ops are first-class
// authoring operations and live on the same history as vertex edits.
//
//   FRAME_EDIT  - snapshot of one frame in `space[frame_index]`.
//   VIEW_EDIT   - snapshot of `space.view_offset / view_scale / view_angle`.
struct LaserBoy_FLTK_Edit_Snapshot
{
    enum Kind { FRAME_EDIT = 0, VIEW_EDIT = 1 };
    Kind               kind;
    // FRAME_EDIT:
    u_int              frame_index;
    LaserBoy_frame     frame;
    // VIEW_EDIT:
    LaserBoy_3D_double view_offset;
    LaserBoy_3D_double view_scale;
    LaserBoy_3D_double view_angle;
    std::string        label;
};

#define LASERBOY_FLTK_EDIT_HISTORY_LIMIT 64

//############################################################################
class LaserBoy_FLTK_GUI;

class LaserBoy_FLTK_Display : public Fl_Widget
{
public:
    LaserBoy_FLTK_Display(int x, int y, int w, int h, LaserBoy_FLTK_GUI* gui);
    //------------------------------------------------------------------------
    int  handle(int event);
    void draw();

private:
    LaserBoy_FLTK_GUI* gui;
};

//############################################################################
class LaserBoy_FLTK_Palette_Display : public Fl_Widget
{
public:
    LaserBoy_FLTK_Palette_Display(int x, int y, int w, int h, LaserBoy_FLTK_GUI* gui);
    //------------------------------------------------------------------------
    int  handle(int event);
    void draw();

private:
    LaserBoy_FLTK_GUI* gui;
};

//############################################################################
class LaserBoy_FLTK_GUI
{
    friend class LaserBoy_FLTK_Display;
    friend class LaserBoy_FLTK_Palette_Display;
public:
    LaserBoy_FLTK_GUI(int x, int y);
    //------------------------------------------------------------------------
   ~LaserBoy_FLTK_GUI();
    //------------------------------------------------------------------------
    //  BITMAP MEMORY AND FLTK SCREEN BUFFER
    //------------------------------------------------------------------------
    void bmp_clone_screen_buffer(struct LaserBoy_bmp* bmp, bool copy)
    {
        bmp_init(bmp, screen->w, screen->h, screen->format->BitsPerPixel);
        if(screen->format->BitsPerPixel <= 8)
            memcpy((void*)(bmp->palette), (const void*)(screen->format->palette->colors), bmp->palette_size);
        if(copy)
            memcpy((void*)(bmp->image), (const void*)(screen->pixels), bmp->image_size);
        return;
    }
    //------------------------------------------------------------------------
    void bmp_to_screen_buffer(struct LaserBoy_bmp* bmp)
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
        unlock_screen_buffer(screen);
        present_screen_region(screen, 0, 0, 0, 0);
        lock_screen_buffer(screen);
        return;
    }
    //------------------------------------------------------------------------
    void screen_buffer_to_bmp(struct LaserBoy_bmp* bmp)
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
        unlock_screen_buffer(screen);
        present_screen_region(screen, 0, 0, 0, 0);
        lock_screen_buffer(screen);
        // Keep the menu toggle button's icon in sync with the legacy menu's
        // actual visibility (space.show_menu) on every paint. Cheap; only swaps
        // the SVG image when the tracked state actually changes.
        refresh_menu_toggle_button();
        return;
    }
    //------------------------------------------------------------------------
    void display_state(const string& state)
    {
        LaserBoy_GUI_Rect rect;
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
        unlock_screen_buffer(screen);
        present_screen_region(screen,
                       8 * space.font_size_factor * 12,
                       0,
                       space.bmp.xres - 8 * space.font_size_factor * 12,
                       8 * space.font_size_factor
                      );
        lock_screen_buffer(screen);
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
            unlock_screen_buffer(screen);
            present_screen_region(screen,
                           0,
                           0,
                           8 * space.font_size_factor * 12,
                           8 * space.font_size_factor
                          );
            lock_screen_buffer(screen);
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
        unlock_screen_buffer(screen);
        present_screen_region(screen,
                       space.bmp.xres - 8 * space.font_size_factor * 12,
                       0,
                       8 * space.font_size_factor * 12,
                       8 * space.font_size_factor
                      );
        lock_screen_buffer(screen);
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
    // Keyboard menu system
    //------------------------------------------------------------------------
    void    check_for_nav_keys                   (LaserBoy_GUI_Key key);
    void    check_for_digit_keys                 (LaserBoy_GUI_Key key);
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
    void    open_import_wizard                   ();
    bool    show_import_task_modal               ();
    bool    show_import_task_modal               (int initial_file_type, const string& initial_source);
    bool    import_file_with_wizard              (const string& file);
    int     show_import_mode_dialog              (const string& file, const string& extension);
    bool    import_ild_file                      (const string& file, int mode);
    bool    apply_imported_frame_set             (LaserBoy_frame_set& frame_set, int mode, const string& clue);
    bool    import_wav_file                      (const string& file, int mode, bool global_polarity = false);
    bool    import_dxf_file                      (const string& file, int mode);
    bool    import_txt_file                      (const string& file, int mode);
    bool    import_ctn_file                      (const string& file, int mode);
    bool    import_wtf_file                      (const string& file);
    bool    execute_import_task                  (int file_type, int action, const string& source, bool option_a, bool option_b, bool option_c);
    bool    import_bmp_file                      (const string& source, int action, bool scale_to_screen);
    bool    import_utf8_file                     (const string& source, bool add_missing_frames, bool leave_extra_frames_unindexed);
    void    finish_import_refresh                (const string& clue);
    void    open_export_wizard                   ();
    bool    show_export_task_modal               ();
    bool    execute_export_task                  (int file_type, int action, const string& destination, bool option_a, bool option_b, bool option_c, bool option_d);
    bool    export_ild_file                      (const string& destination, int action, bool use_format_45, bool save_2D_as_3D, bool auto_minimize, bool save_1_frame_bridge);
    bool    export_dxf_file                      (const string& destination, int action, bool true_color_dxf);
    bool    export_wav_file                      (const string& destination, int action, bool invert_output, bool apply_offsets, bool use_8_channels, bool auto_flatten_z);
    bool    export_txt_file                      (const string& destination, int action, bool unit_coordinates, bool integrated_color, bool color_hex, bool named_palettes);
    bool    export_ctn_file                      (const string& destination, int action);
    bool    export_bmp_file                      (const string& destination, int action);
    bool    export_wtf_file                      (const string& destination);
    bool    export_utf8_file                     (const string& destination, bool cleanup);
    void    finish_export_refresh                (const string& clue);
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
    LaserBoy_GUI_Keycode gui_keysym_to_char(struct LaserBoy_GUI_Keysym* gui_keysym)
    {
        if(gui_keysym->mod & LASERBOY_KEY_MOD_SHIFT)
        {
            if(gui_keysym->sym >= LASERBOY_KEY_a && gui_keysym->sym <= LASERBOY_KEY_z)
                return toupper((char)gui_keysym->sym);
            else if(gui_keysym->sym == LASERBOY_KEY_1)
                return '!';
            else if(gui_keysym->sym == LASERBOY_KEY_2)
                return '@';
            else if(gui_keysym->sym == LASERBOY_KEY_3)
                return '#';
            else if(gui_keysym->sym == LASERBOY_KEY_4)
                return '$';
            else if(gui_keysym->sym == LASERBOY_KEY_5)
                return '%';
            else if(gui_keysym->sym == LASERBOY_KEY_6)
                return '^';
            else if(gui_keysym->sym == LASERBOY_KEY_7)
                return '&';
            else if(gui_keysym->sym == LASERBOY_KEY_8)
                return '*';
            else if(gui_keysym->sym == LASERBOY_KEY_9)
                return '(';
            else if(gui_keysym->sym == LASERBOY_KEY_0)
                return ')';
            else if(gui_keysym->sym == LASERBOY_KEY_BACKQUOTE)
                return '~';
            else if(gui_keysym->sym == LASERBOY_KEY_MINUS)
                return '_';
            else if(gui_keysym->sym == LASERBOY_KEY_EQUALS)
                return '+';
            else if(gui_keysym->sym == LASERBOY_KEY_LEFTBRACKET)
                return '{';
            else if(gui_keysym->sym == LASERBOY_KEY_RIGHTBRACKET)
                return '}';
            else if(gui_keysym->sym == LASERBOY_KEY_BACKSLASH)
                return '|';
            else if(gui_keysym->sym == LASERBOY_KEY_SEMICOLON)
                return ':';
            else if(gui_keysym->sym == LASERBOY_KEY_QUOTE)
                return '"';
            else if(gui_keysym->sym == LASERBOY_KEY_COMMA)
                return '<';
            else if(gui_keysym->sym == LASERBOY_KEY_PERIOD)
                return '>';
            else if(gui_keysym->sym == LASERBOY_KEY_SLASH)
                return '?';
        }
        return gui_keysym->sym;
    }
    //------------------------------------------------------------------------
    bool           running,
                   prompt_escaped;
    u_int          xres,
                   yres;
    LaserBoy_TUI   space;
    int            wait_for_gui_event(LaserBoy_GUI_Event* event);
    int            poll_gui_event(LaserBoy_GUI_Event* event);
    void           update_gui_regions();
    void           present_screen();
    void           present_screen_region(LaserBoy_Screen_Buffer* surface, int x, int y, int w, int h);
    void           focus_display();
    void           update_viewport_mouse_status(int mouse_x, int mouse_y, const char* action);
    LaserBoy_3D_double project_viewport_point(const LaserBoy_vertex& vertex, u_int vertex_index);
    bool           viewport_point_to_model_delta(int dx, int dy, LaserBoy_3D_double& delta) const;
    double         viewport_distance_to_segment(double mouse_x, double mouse_y, double x0, double y0, double x1, double y1) const;
    bool           select_viewport_vertex(int mouse_x, int mouse_y, bool extend_selection);
    bool           select_viewport_segment(int mouse_x, int mouse_y, bool extend_selection);
    bool           select_viewport_rect(int canvas_x1, int canvas_y1, int canvas_x2, int canvas_y2, bool extend_selection);
    bool           delete_selection_in_frame();
    bool           copy_selection_to_new_frame();
    bool           begin_viewport_selection_drag(int mouse_x, int mouse_y, bool extend_selection);
    bool           drag_viewport_selection_by_pixels(int dx, int dy);
    bool           begin_viewport_vertex_drag(int mouse_x, int mouse_y, bool extend_selection);
    bool           drag_viewport_vertex_by_pixels(int dx, int dy);
    void           pan_viewport_by_pixels(int dx, int dy);
    void           orbit_viewport_by_pixels(int dx, int dy);
    void           refresh_viewport_after_mouse_change();
    void           apply_view_control(char command);
    void           toggle_menu_overlay();
    void           refresh_menu_toggle_button();
    void           sync_ui_visual_menu_checks();
    void           set_viewport_tool(LaserBoy_FLTK_Viewport_Tool tool);
    LaserBoy_FLTK_Viewport_Tool active_viewport_tool() const { return viewport_tool; }
    //------------------------------------------------------------------------
    // Square viewport layout: largest square inside the display widget; world renders to screen buffer of side viewport_square_n.
    void           schedule_debounced_viewport_resize();
    void           commit_debounced_viewport_resize();
    void           layout_gui_regions(int window_w, int window_h);
    void           recompute_viewport_square_geometry();
    double         viewport_model_to_canvas_scale(int art_px) const;
    bool           viewport_widget_local_to_canvas(int widget_lx, int widget_ly, int& canvas_x, int& canvas_y) const;
    int            viewport_canvas_side() const { return viewport_square_n; }
    int            viewport_square_offset_x() const { return viewport_square_x; }
    int            viewport_square_offset_y() const { return viewport_square_y; }
    bool           viewport_buffers_ready() const { return screen != NULL && screen->pixels != NULL; }
    const char*    viewport_tool_label(LaserBoy_FLTK_Viewport_Tool tool) const;
    void           refresh_tool_buttons();
    bool           begin_viewport_edit_transaction(const std::string& label);
    bool           begin_viewport_view_transaction(const std::string& label);
    void           commit_viewport_edit_transaction(bool changed);
    bool           undo_frame_edit();
    bool           redo_frame_edit();
    bool           can_undo_frame_edit() const { return !undo_history.empty(); }
    bool           can_redo_frame_edit() const { return !redo_history.empty(); }
    void           clear_edit_history();
    void           push_command_key(int key, int state = 0);
    void           push_key_event(int key, int state);
    void           fltk_request_close();
    int            fltk_key_to_laserboy_key(int key);
    //------------------------------------------------------------------------
    Fl_Window*     window;
    Fl_Menu_Bar*   menu_bar;
    Fl_Group*      toolbar;
    Fl_Group*      main_viewport;
    Fl_Group*      frame_controls;
    Fl_Group*      stats_panel;
    Fl_Group*      palette_panel;
    Fl_Group*      status_bar;
    Fl_Box*        toolbar_label;
    Fl_Button*     previous_frame_button;
    Fl_Button*     next_frame_button;
    Fl_Button*     first_frame_button;
    Fl_Button*     last_frame_button;
    Fl_Button*     menu_toggle_button;
    Fl_Button*     segment_tool_button;
    Fl_Button*     vertex_tool_button;
    Fl_Button*     pan_orbit_tool_button;
    Fl_Button*     undo_button;
    Fl_Button*     redo_button;
    // "Include Camera" toggle next to undo/redo. When checked, pan / zoom /
    // orbit are pushed onto the undo stack just like vertex edits. When
    // unchecked (the default), they are treated as pure navigation.
    Fl_Check_Button* include_camera_check;
    bool             include_camera_in_history;
    Fl_Button*     delete_selection_button;
    Fl_Button*     copy_selection_button;
    Fl_Button*     zoom_in_button;
    Fl_Button*     zoom_out_button;
    Fl_Button*     zoom_reset_button;
    Fl_Button*     center_view_button;
    Fl_Button*     view_reset_button;
    Fl_Box*        frame_number_label;
    Fl_Box*        stats_text;
    Fl_Box*        palette_text;
    LaserBoy_FLTK_Palette_Display* palette_display;
    Fl_Box*        status_text;
    LaserBoy_FLTK_Display* display;
    // Tracks the most recently rendered legacy-menu state so we can skip redundant
    // image swaps. The authoritative source is space.show_menu.
    bool           menu_toggle_icon_shown;
    // FreeCAD SVG icons (owned, freed in destructor)
    Fl_SVG_Image*  icon_menu;
    Fl_SVG_Image*  icon_menu_off;
    Fl_SVG_Image*  icon_seg_tool;
    Fl_SVG_Image*  icon_vtx_tool;
    Fl_SVG_Image*  icon_pan_tool;
    Fl_SVG_Image*  icon_undo;
    Fl_SVG_Image*  icon_redo;
    Fl_SVG_Image*  icon_zoom_in;
    Fl_SVG_Image*  icon_zoom_out;
    Fl_SVG_Image*  icon_zoom_reset;
    Fl_SVG_Image*  icon_center_view;
    Fl_SVG_Image*  icon_view_reset;
    Fl_SVG_Image*  icon_first_frame;
    Fl_SVG_Image*  icon_prev_frame;
    Fl_SVG_Image*  icon_next_frame;
    Fl_SVG_Image*  icon_last_frame;
    LaserBoy_FLTK_Viewport_Tool viewport_tool;
    LaserBoy_FLTK_Viewport_Drag_Action viewport_drag_action;
    bool           viewport_mouse_dragging;
    bool           viewport_mouse_moved;
    int            viewport_mouse_button;
    int            viewport_mouse_down_x;
    int            viewport_mouse_down_y;
    int            viewport_mouse_last_x;
    int            viewport_mouse_last_y;
    size_t         viewport_drag_egg;
    size_t         viewport_drag_spider;
    size_t         viewport_drag_vertex;
    int            viewport_drag_start_canvas_x;
    int            viewport_drag_start_canvas_y;
    // Rubber-band selection state (coords are in canvas/square space, same as hit-tests).
    bool           rubber_band_active;
    int            rubber_band_x1;
    int            rubber_band_y1;
    int            rubber_band_x2;
    int            rubber_band_y2;
    bool           rubber_band_extend;
    LaserBoy_frame viewport_drag_start_frame;
    LaserBoy_3D_double viewport_drag_start_view_offset;
    bool           viewport_edit_transaction_active;
    bool           viewport_edit_transaction_committed;
    int            viewport_square_n;
    int            viewport_square_x;
    int            viewport_square_y;
    std::deque<LaserBoy_FLTK_Edit_Snapshot> undo_history;
    std::deque<LaserBoy_FLTK_Edit_Snapshot> redo_history;
    LaserBoy_GUI_PixelFormat screen_format;
    std::deque<LaserBoy_GUI_Event> event_queue;
    LaserBoy_Screen_Buffer   *screen; // FLTK-backed software screen buffer
    LaserBoy_GUI_Event      event ; // UI input (keys)
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################