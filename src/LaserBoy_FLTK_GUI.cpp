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
// LaserBoy_FLTK_GUI.cpp is part of LaserBoy.
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

#include <FL/fl_draw.H>

#include <cstdlib>
#include <cstring>

namespace
{
LaserBoy_FLTK_GUI* active_fltk_gui = NULL;

//----------------------------------------------------------------------------
int fltk_global_event_handler(int fltk_event)
{
    if(    active_fltk_gui
        && (fltk_event == FL_KEYDOWN || fltk_event == FL_SHORTCUT)
      )
    {
        active_fltk_gui->push_key_event(Fl::event_key(), Fl::event_state());
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
class LaserBoy_FLTK_Window : public Fl_Window
{
public:
    LaserBoy_FLTK_Window(int w, int h, const char* label, LaserBoy_FLTK_GUI* gui_ptr)
            : Fl_Window(w, h, label)
            , gui(gui_ptr)
    {
    }
    //------------------------------------------------------------------------
    int handle(int fltk_event) override
    {
        if(fltk_event == FL_PUSH)
        {
            gui->focus_display();
            return 1;
        }
        if(fltk_event == FL_KEYDOWN || fltk_event == FL_SHORTCUT)
        {
            gui->push_key_event(Fl::event_key(), Fl::event_state());
            return 1;
        }
        if(fltk_event == FL_CLOSE)
        {
            gui->fltk_request_close();
            return 1;
        }
        return Fl_Window::handle(fltk_event);
    }

private:
    LaserBoy_FLTK_GUI* gui;
};

//----------------------------------------------------------------------------
void fltk_close_callback(Fl_Widget*, void* data)
{
    ((LaserBoy_FLTK_GUI*)data)->fltk_request_close();
}
}

//############################################################################
LaserBoy_FLTK_Display::LaserBoy_FLTK_Display(int x, int y, int w, int h, LaserBoy_FLTK_GUI* gui_ptr)
        : Fl_Widget(x, y, w, h)
        , gui(gui_ptr)
{
}

//############################################################################
int LaserBoy_FLTK_Display::handle(int fltk_event)
{
    if(fltk_event == FL_FOCUS || fltk_event == FL_UNFOCUS)
        return 1;
    if(fltk_event == FL_PUSH)
    {
        take_focus();
        return 1;
    }
    if(fltk_event == FL_KEYDOWN || fltk_event == FL_SHORTCUT)
    {
        gui->push_key_event(Fl::event_key(), Fl::event_state());
        return 1;
    }
    return Fl_Widget::handle(fltk_event);
}

//############################################################################
void LaserBoy_FLTK_Display::draw()
{
    if(gui && gui->screen && gui->screen->pixels)
        fl_draw_image((const unsigned char*)gui->screen->pixels,
                      x(),
                      y(),
                      gui->screen->w,
                      gui->screen->h,
                      4
                     );
}

//############################################################################
LaserBoy_FLTK_GUI::LaserBoy_FLTK_GUI(int x, int y)
        : running        (false),
          prompt_escaped (false),
          xres           (x    ),
          yres           (y    ),
          space          (this ),
          screen         (NULL )
{
    window = new LaserBoy_FLTK_Window(xres, yres, "LaserBoy by James Lehman : FLTK", this);
    display = new LaserBoy_FLTK_Display(0, 0, xres, yres, this);
    window->resizable(display);
    window->callback(fltk_close_callback, this);
    window->end();
    window->show();
    active_fltk_gui = this;
    Fl::add_handler(fltk_global_event_handler);
    display->take_focus();
    Fl::focus(display);

    screen_format.BitsPerPixel = 32;
    screen_format.palette      = NULL;
    screen = new LaserBoy_Screen_Buffer;
    screen->w      = xres;
    screen->h      = yres;
    screen->format = &screen_format;
    screen->pixels = std::calloc((size_t)xres * (size_t)yres, 4);
    if(screen->pixels == NULL)
        std::exit(2);
    if(lock_screen_buffer(screen) < 0)
    {
        std::exit(2);
    }
    bmp_clone_screen_buffer(space.p_bmp(), false);
    bmp_copy(space.p_background()       , space.p_bmp()); // initialize bitmaps to all the same size as screen
    bmp_copy(space.p_background_bmp_2D(), space.p_bmp());
    bmp_copy(space.p_background_bmp_3D(), space.p_bmp());
    space.recolor_background();
    if(LASERBOY_OK == space.font_frames.from_ild_file(LASERBOY_FONT_SHARE + space.font_name + ".ild"))
    {
        if(space.font_frames.get_unicode_index(space.font_index) & LASERBOY_INVALID_UNICODE)
            space.font_frames.clear();
    }
    else
        space.font_frames.clear();
    if(y < 960)
        space.font_size_factor = 1;
}

//############################################################################
LaserBoy_FLTK_GUI::~LaserBoy_FLTK_GUI()
{
    if(active_fltk_gui == this)
        active_fltk_gui = NULL;
    Fl::remove_handler(fltk_global_event_handler);
    if(screen)
    {
        std::free(screen->pixels);
        delete screen;
    }
    delete window;
}

//############################################################################
int LaserBoy_FLTK_GUI::wait_for_gui_event(LaserBoy_GUI_Event* fltk_event)
{
    while(event_queue.empty() && window->shown())
        Fl::wait();
    if(event_queue.empty())
    {
        running        = false;
        prompt_escaped = true;
        fltk_event->type                 = LASERBOY_GUI_KEYDOWN;
        fltk_event->key.keysym.sym       = LASERBOY_KEY_ESCAPE;
        fltk_event->key.keysym.mod       = 0;
        fltk_event->key.keysym.unicode   = LASERBOY_KEY_ESCAPE;
        return 1;
    }
    *fltk_event = event_queue.front();
    event_queue.pop_front();
    return 1;
}

//############################################################################
int LaserBoy_FLTK_GUI::poll_gui_event(LaserBoy_GUI_Event* fltk_event)
{
    if(event_queue.empty())
        Fl::check();
    if(event_queue.empty())
        return 0;
    *fltk_event = event_queue.front();
    event_queue.pop_front();
    return 1;
}

//############################################################################
void LaserBoy_FLTK_GUI::present_screen()
{
    if(display)
    {
        display->redraw();
        Fl::check();
    }
}

//############################################################################
void LaserBoy_FLTK_GUI::present_screen_region(LaserBoy_Screen_Buffer*, int, int, int, int)
{
    present_screen();
}

//############################################################################
void LaserBoy_FLTK_GUI::focus_display()
{
    if(display)
    {
        display->take_focus();
        Fl::focus(display);
    }
}

//############################################################################
void LaserBoy_FLTK_GUI::push_key_event(int key, int state)
{
    LaserBoy_GUI_Event fltk_event;
    fltk_event.type = LASERBOY_GUI_KEYDOWN;
    fltk_event.key.keysym.sym = fltk_key_to_laserboy_key(key);
    fltk_event.key.keysym.mod = state;
    fltk_event.key.keysym.unicode = fltk_event.key.keysym.sym;
    event_queue.push_back(fltk_event);
}

//############################################################################
void LaserBoy_FLTK_GUI::fltk_request_close()
{
    running        = false;
    prompt_escaped = true;
    event_queue.clear();
    push_key_event(FL_Escape, 0);
    if(window)
        window->hide();
}

//############################################################################
int LaserBoy_FLTK_GUI::fltk_key_to_laserboy_key(int key)
{
    switch(key)
    {
        case FL_Escape:    return LASERBOY_KEY_ESCAPE;
        case FL_Enter:
        case FL_KP_Enter:  return LASERBOY_KEY_RETURN;
        case FL_BackSpace: return LASERBOY_KEY_BACKSPACE;
        case FL_Delete:    return LASERBOY_KEY_DELETE;
        case FL_Up:        return LASERBOY_KEY_UP;
        case FL_Down:      return LASERBOY_KEY_DOWN;
        case FL_Left:      return LASERBOY_KEY_LEFT;
        case FL_Right:     return LASERBOY_KEY_RIGHT;
        case FL_Page_Up:   return LASERBOY_KEY_PAGEUP;
        case FL_Page_Down: return LASERBOY_KEY_PAGEDOWN;
        case FL_Home:      return LASERBOY_KEY_HOME;
        case FL_End:       return LASERBOY_KEY_END;
        case FL_Insert:    return LASERBOY_KEY_INSERT;
        case FL_Tab:       return LASERBOY_KEY_TAB;
        default:           break;
    }
    const char* text = Fl::event_text();
    if(text && text[0] && !text[1])
        return text[0];
    return key;
}

//############################################################################
void LaserBoy_FLTK_GUI::capture_screen()
{
    int x, y;
    struct LaserBoy_bmp screen_capture = {0};
    bmp_init(&screen_capture, space.p_bmp()->xres, space.p_bmp()->yres, 24);
    for(y = 0; y < space.p_bmp()->yres; y++)
        for(x = 0; x < space.p_bmp()->xres; x++)
            screen_capture.put_pixel(   &screen_capture
                                      , x
                                      , screen_capture.yres - y - 1 // flipped
                                      , screen_capture.make_rgb(   space.p_bmp()->r_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                                 , space.p_bmp()->g_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                                 , space.p_bmp()->b_from_rgb(space.p_bmp()->get_pixel_rgb(space.p_bmp(), x, y))
                                                               )
                                    );
    bmp_save_as(&screen_capture, (char*)("screen_capture.bmp"));
    bmp_free(&screen_capture);
}

//############################################################################
void LaserBoy_FLTK_GUI::display_prompt_file_with_auto_complete(const string& prompt, const string& value)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = space.file_name.size(),
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input;
    prompt_escaped      = false;
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, bmp_bg_color, bmp_bg_color, 0, space.font_size_factor,
               "                                                                                " // clear the line!
              );
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               (space.file_name.size()) ? (space.file_name.c_str()) : ("_") // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
        {
            if(space.file_name.size() == 0)
                space.file_name = value;
            done = true;
        }
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(space.file_name.size())
                    {
                        space.file_name = space.file_name.substr(0, space.file_name.size() - 1);
                        if(character > 0)
                            character--;
                        if(space.file_name.size() == 0)
                        {
                            space.clean_screen();
                            space.overlay_file_list();
                        }
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s_                                                                ",
                                   space.file_name.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    {
                        pre_input = space.file_name;
                        pre_input += event.key.keysym.unicode;
                        if(STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), pre_input))
                        {
                            space.file_name = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_file_list() == 1)
                            {
                                for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                                {
                                    if(STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), space.file_name))
                                    {
                                        space.file_name = space.working_directory_list[list_item].path().filename().string();
                                        character = space.file_name.size() - 1;
                                        y_off -= y_line;
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   "                                                                                "
                                                  );
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   prompt.c_str()
                                                  );
                                        y_off += y_line;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   (space.rainbow_menus) ? (space.bmp.make_rgb(255,255,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   space.file_name.c_str()
                                                  );
                                        break;
                                    }
                                }
                            } // if(space.overlay_file_list() == 1)
                            else
                            {
                                y_off -= y_line;
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           "                                                                                "
                                          );
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           prompt.c_str()
                                          );
                                y_off += y_line;
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s_",
                                           space.file_name.c_str()
                                          );
                            }
                            display_space();
                            break;
                        } // if(event.key.keysym.unicode == space.working_directory_list[list_item].path().filename().string()[character])
                    } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               space.file_name.c_str()
              );
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_prompt_dir_with_auto_complete(const string& prompt)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = 0,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(space.working_subdirectory.size())
                    {
                        space.working_subdirectory = space.working_subdirectory.substr(0, space.working_subdirectory.size() - 1);
                        if(character > 0)
                            character--;
                        if(space.working_subdirectory.size() == 0)
                        {
                            space.clean_screen();
                            space.overlay_subdirectory_list();
                        }
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s_                                                                ",
                                   space.working_subdirectory.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    {
                        pre_input = space.working_subdirectory;
                        pre_input += event.key.keysym.unicode;
                        if(    is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string())
                            && STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), pre_input)
                          )
                        {
                            space.working_subdirectory = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_subdirectory_list() == 1)
                            {
                                for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                                {
                                    if(    is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string())
                                        && STARTS_WITH(space.working_directory_list[list_item].path().filename().string(), space.working_subdirectory))
                                    {
                                        space.working_subdirectory = space.working_directory_list[list_item].path().filename().string();
                                        character = space.working_subdirectory.size() - 1;
                                        y_off -= y_line;
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   "                                                                                "
                                                  );
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   prompt.c_str()
                                                  );
                                        y_off += y_line;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   (space.rainbow_menus) ? (space.bmp.make_rgb(255,255,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   space.working_subdirectory.c_str()
                                                  );
                                        break;
                                    }
                                } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                            } // end if(space.overlay_subdirectory_list() == 1)
                            else
                            {
                                y_off -= y_line;
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           "                                                                                "
                                          );
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           prompt.c_str()
                                          );
                                y_off += y_line;
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s_",
                                           space.working_subdirectory.c_str()
                                          );
                            }
                            display_space();
                            break;
                        } // END if(is_directory(space.working_directory + space.working_directory_list[list_item].path().filename().string()) ....
                    } // end for(list_item = 0; list_item < space.working_directory_list.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               space.working_subdirectory.c_str()
              );
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_prompt_f_effect_with_auto_complete(const string& prompt)
{
    bool   done           = false;
    u_int  x_off          = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off          = space.bmp.yres - 24 * space.font_size_factor,
           x_gap          = 8 * space.font_size_factor,
           y_line         = x_gap,
           list_item      = 0,
           character      = space.effect_name.size(),
           bmp_bg_color   = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s_",
               space.effect_name.c_str() // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(space.effect_name.size())
                    {
                        space.effect_name = space.effect_name.substr(0, space.effect_name.size() - 1);
                        if(character > 0)
                            character--;
                        if(space.effect_name.size() == 0)
                        {
                            space.clean_screen();
                            space.overlay_frame_effects();
                        }
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s_                                                                ",
                                   space.effect_name.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    {
                        pre_input = space.effect_name;
                        pre_input += event.key.keysym.unicode;
                        if(starts_with(space.frame_effect_names[list_item], pre_input))
                        {
                            space.effect_name = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_frame_effects() == 1)
                            {
                                for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                                {
                                    if(starts_with(space.frame_effect_names[list_item], space.effect_name))
                                    {
                                        space.effect_name = space.frame_effect_names[list_item];
                                        character = space.effect_name.size() - 1;
                                        y_off -= y_line;
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   "                                                                                "
                                                  );
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   prompt.c_str()
                                                  );
                                        y_off += y_line;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   (space.rainbow_menus) ? (space.bmp.make_rgb(255,255,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   space.effect_name.c_str()
                                                  );
                                    }
                                }
                            } // end if(space.overlay_frame_effects() == 1)
                            else
                            {
                                y_off -= y_line;
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           "                                                                                "
                                          );
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           prompt.c_str()
                                          );
                                y_off += y_line;
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s_",
                                           space.effect_name.c_str()
                                          );
                            }
                            display_space();
                            break;
                         }// end if(starts_with(space.frame_effect_names[list_item], pre_input))
                    } // end for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               space.effect_name.c_str()
              );
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_prompt_sf_effect_with_auto_complete(const string& prompt)
{
    bool   done           = false;
    u_int  x_off          = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off          = space.bmp.yres - 24 * space.font_size_factor,
           x_gap          = 8 * space.font_size_factor,
           y_line         = x_gap,
           list_item      = 0,
           character      = space.effect_name.size(),
           bmp_bg_color   = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s_",
               space.effect_name.c_str() // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(space.effect_name.size())
                    {
                        space.effect_name = space.effect_name.substr(0, space.effect_name.size() - 1);
                        if(character > 0)
                            character--;
                        if(space.effect_name.size() == 0)
                        {
                            space.clean_screen();
                            space.overlay_selected_frames_effects();
                        }
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s_                                                                ",
                                   space.effect_name.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.selected_frames_effect_names.size(); list_item++)
                    {
                        pre_input = space.effect_name;
                        pre_input += event.key.keysym.unicode;
                        if(starts_with(space.selected_frames_effect_names[list_item], pre_input))
                        {
                            space.effect_name = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_selected_frames_effects() == 1)
                            {
                                for(list_item = 0; list_item < space.selected_frames_effect_names.size(); list_item++)
                                {
                                    if(starts_with(space.selected_frames_effect_names[list_item], space.effect_name))
                                    {
                                        space.effect_name = space.selected_frames_effect_names[list_item];
                                        character = space.effect_name.size() - 1;
                                        y_off -= y_line;
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   "                                                                                "
                                                  );
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   prompt.c_str()
                                                  );
                                        y_off += y_line;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   (space.rainbow_menus) ? (space.bmp.make_rgb(255,255,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   space.effect_name.c_str()
                                                  );
                                    }
                                }
                            } // end if(space.overlay_frame_effects() == 1)
                            else
                            {
                                y_off -= y_line;
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           "                                                                                "
                                          );
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           prompt.c_str()
                                          );
                                y_off += y_line;
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s_",
                                           space.effect_name.c_str()
                                          );
                            }
                            display_space();
                            break;
                         }// end if(starts_with(space.frame_effect_names[list_item], pre_input))
                    } // end for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               space.effect_name.c_str()
              );
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_prompt_fs_effect_with_auto_complete(const string& prompt)
{
    bool   done         = false;
    u_int  x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap,
           list_item    = 0,
           character    = space.effect_name.size(),
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string pre_input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s_",
               space.effect_name.c_str() // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(space.effect_name.size())
                    {
                        space.effect_name = space.effect_name.substr(0, space.effect_name.size() - 1);
                        if(character > 0)
                            character--;
                        if(space.effect_name.size() == 0)
                        {
                            space.clean_screen();
                            space.overlay_frame_set_effects();
                        }
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%s_                                                                ",
                                   space.effect_name.c_str()
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    for(list_item = 0; list_item < space.frame_set_effect_names.size(); list_item++)
                    {
                        pre_input = space.effect_name;
                        pre_input += event.key.keysym.unicode;
                        if(starts_with(space.frame_set_effect_names[list_item], pre_input))
                        {
                            space.effect_name = pre_input;
                            character++;
                            space.clean_screen();
                            if(space.overlay_frame_set_effects() == 1)
                            {
                                for(list_item = 0; list_item < space.frame_set_effect_names.size(); list_item++)
                                {
                                    if(starts_with(space.frame_set_effect_names[list_item], space.effect_name))
                                    {
                                        space.effect_name = space.frame_set_effect_names[list_item];
                                        character = space.effect_name.size() - 1;
                                        y_off -= y_line;
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   "                                                                                "
                                                  );
                                        bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                                   prompt.c_str()
                                                  );
                                        y_off += y_line;
                                        bmp_printf(&(space.bmp),
                                                   x_off,
                                                   y_off,
                                                   (space.rainbow_menus) ? (space.bmp.make_rgb(255,255,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                                   bmp_bg_color,
                                                   0,
                                                   space.font_size_factor,
                                                   "%s",
                                                   space.effect_name.c_str()
                                                  );
                                    }
                                }
                            } // end if(space.overlay_frame_effects() == 1)
                            else
                            {
                                y_off -= y_line;
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           "                                                                                "
                                          );
                                bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                           prompt.c_str()
                                          );
                                y_off += y_line;
                                bmp_printf(&(space.bmp),
                                           x_off,
                                           y_off,
                                           (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                           bmp_bg_color,
                                           0,
                                           space.font_size_factor,
                                           "%s_",
                                           space.effect_name.c_str()
                                          );
                            }
                            display_space();
                            break;
                        } // end if(event.key.keysym.unicode == space.frame_effect_names[list_item][character])
                    } // end for(list_item = 0; list_item < space.frame_effect_names.size(); list_item++)
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_off, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               space.effect_name.c_str()
              );
    display_space();
    return;
}

//############################################################################
string LaserBoy_FLTK_GUI::display_prompt_and_echo_name(const string& prompt, u_int max_len)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = x_gap;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor
             , "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor
             , prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor
             , "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor
             , "_" // text cursor
              );
    //------------------------------------------------------------------------
    display_space();
    //------------------------------------------------------------------------
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }   while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    input.size() < max_len
                        && (     isalnum(event.key.keysym.unicode)
                             || event.key.keysym.unicode == '_'
                             || event.key.keysym.unicode == '-'
                             || event.key.keysym.unicode == '.'
                           )
                      )
                    {
                        input += event.key.keysym.unicode;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        x_off += x_gap;
                        display_space();
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    //------------------------------------------------------------------------
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "%s",
               input.c_str()
              );
    display_space();
    while(input.size() && input[0] == '.')
        input.erase(input.begin());
    return input;
}

//############################################################################
string LaserBoy_FLTK_GUI::display_prompt_and_echo_string(const string& prompt)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                  space.current_palette()[space.selected_color_index].g,
                                  space.current_palette()[space.selected_color_index].b
                                 ),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isgraph(event.key.keysym.unicode)
                        || event.key.keysym.unicode == ' '
                      )
                    {
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        display_space();
                        input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    return input;
}

//############################################################################
string LaserBoy_FLTK_GUI::display_prompt_and_echo_unicode(const string& prompt)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    double number;
    string input;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                  space.current_palette()[space.selected_color_index].g,
                                  space.current_palette()[space.selected_color_index].b
                                 ),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return "";
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(isxdigit(event.key.keysym.unicode) && input.size() < 8)
                    {
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   space.bmp.make_rgb(space.current_palette()[space.selected_color_index].r,
                                                      space.current_palette()[space.selected_color_index].g,
                                                      space.current_palette()[space.selected_color_index].b
                                                     ),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                   event.key.keysym.unicode
                                  );
                        display_space();
                        input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    hex_string(input, number);
    input = as_hex_string((char32_t)number);
    return input;
}

//############################################################################
double LaserBoy_FLTK_GUI::display_prompt_and_echo_double(const string& prompt, double value, double v_max, double v_min)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    double result       = 0.0,
           check        = 0.0;
    string input,
           check_string;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return 0.0;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isdigit(event.key.keysym.unicode)
                        || (    event.key.keysym.unicode == '.'
                             && input.find('.') == string::npos
                           )
                        || (    event.key.keysym.unicode == '+'
                             && !input.size()
                           )
                        || (    event.key.keysym.unicode == '-'
                             && !input.size()
                           )
                      )
                    {
                        check_string = input;
                        check_string += event.key.keysym.unicode;
                        sscanf(check_string.c_str(), "%lf", &check);
                        if(check <= v_max && check >= v_min)
                        {
                            y_off -= y_line;
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       "                                                                                "
                                      );
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       prompt.c_str()
                                      );
                            y_off += y_line;
                            bmp_printf(&(space.bmp),
                                       x_off,
                                       y_off,
                                       (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                       bmp_bg_color,
                                       0,
                                       space.font_size_factor,
                                       "%c",
                                       event.key.keysym.unicode
                                      );
                            display_space();
                            input += event.key.keysym.unicode;
                            x_off += x_gap;
                        }


                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%lf", &result);
    else
        result = value;
    return result;
}

//############################################################################
u_char LaserBoy_FLTK_GUI::display_prompt_and_echo_u_char(const string& prompt, u_char default_value)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b),
           check        = 0,
           result       = 0;
    string input,
           check_string;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return 0;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(isdigit(event.key.keysym.unicode))
                    {
                        check = 0;
                        check_string = input;
                        check_string += event.key.keysym.unicode;
                        sscanf(check_string.c_str(), "%u", &check);
                        if(check < 256)
                        {
                            y_off -= y_line;
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       "                                                                                "
                                      );
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       prompt.c_str()
                                      );
                            y_off += y_line;
                            bmp_printf(&(space.bmp),
                                       x_off,
                                       y_off,
                                       (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                       bmp_bg_color,
                                       0,
                                       space.font_size_factor,
                                       "%c",
                                       event.key.keysym.unicode
                                      );
                            display_space();
                            input += event.key.keysym.unicode;
                            x_off += x_gap;
                        }
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%u", &result);
    else
        result = default_value;
    return (u_char)result;
}

//############################################################################
short LaserBoy_FLTK_GUI::display_prompt_and_echo_short(const string& prompt, short default_value)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           check        = 0,
           result       = 0;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input,
           check_string;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return 0;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isdigit(event.key.keysym.unicode)
                        || (    event.key.keysym.unicode == '+'
                             && !input.size()
                           )
                        || (    event.key.keysym.unicode == '-'
                             && !input.size()
                           )
                      )
                    {
                        check = 0;
                        check_string = input;
                        check_string += event.key.keysym.unicode;
                        sscanf(check_string.c_str(), "%u", &check);
                        if(check <= LASERBOY_MAX_SHORT && check >= LASERBOY_MIN_SHORT)
                        {
                            y_off -= y_line;
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       "                                                                                "
                                      );
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       prompt.c_str()
                                      );
                            y_off += y_line;
                            bmp_printf(&(space.bmp),
                                       x_off,
                                       y_off,
                                       (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                       bmp_bg_color,
                                       0,
                                       space.font_size_factor,
                                       "%c",
                                       event.key.keysym.unicode
                                      );
                            display_space();
                            input += event.key.keysym.unicode;
                            x_off += x_gap;
                        }
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%u", &result);
    else
        result = default_value;
    return (short)result;
}

//############################################################################
int LaserBoy_FLTK_GUI::display_prompt_and_echo_int(const string& prompt, int default_value)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           result       = 0;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    string input;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return 0;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(    isdigit(event.key.keysym.unicode)
                        || (    event.key.keysym.unicode == '+'
                             && !input.size()
                           )
                        || (    event.key.keysym.unicode == '-'
                             && !input.size()
                           )
                      )
                    {
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   "%c",
                                       event.key.keysym.unicode
                                  );
                        display_space();
                            input += event.key.keysym.unicode;
                        x_off += x_gap;
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%u", &result);
    else
        result = default_value;
    return result;
}

//############################################################################
u_int LaserBoy_FLTK_GUI::display_prompt_and_echo_u_int(const string& prompt, u_int default_value, u_int v_max)
{
    bool   done         = false;
    int    x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           x_set        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           x_gap        = 8 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           result       = 0;
    u_int  bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b),
           check = 0;
    string input,
           check_string;
    prompt_escaped = false;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();

    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            space.file_name.clear();
            prompt_escaped = true;
            return 0;
        }
        if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
            done = true;
        else
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_DELETE:
                case LASERBOY_KEY_BACKSPACE:
                    if(input.size())
                    {
                        input = input.substr(0, input.size() - 1);
                        x_off -= x_gap;
                        y_off -= y_line;
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   "                                                                                "
                                  );
                        bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                   prompt.c_str()
                                  );
                        y_off += y_line;
                        bmp_printf(&(space.bmp),
                                   x_off,
                                   y_off,
                                   (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                   bmp_bg_color,
                                   0,
                                   space.font_size_factor,
                                   " "
                                  );
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                default:
                    if(isdigit(event.key.keysym.unicode))
                    {
                        check_string = input;
                        check_string += event.key.keysym.unicode;
                        sscanf(check_string.c_str(), "%u", &check);
                        if(check <= v_max)
                        {
                            y_off -= y_line;
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       "                                                                                "
                                      );
                            bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
                                       prompt.c_str()
                                      );
                            y_off += y_line;
                            bmp_printf(&(space.bmp),
                                       x_off,
                                       y_off,
                                       (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
                                       bmp_bg_color,
                                       0,
                                       space.font_size_factor,
                                       "%c",
                                       event.key.keysym.unicode
                                      );
                            display_space();
                            input += event.key.keysym.unicode;
                            x_off += x_gap;
                        }
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else of if(event.key.keysym.unicode == LASERBOY_KEY_RETURN)
    } // end while(!done)
    y_off -= y_line;
    bmp_printf(&(space.bmp), x_set, y_off, space.white_text.as_uint(), bmp_bg_color, 0, space.font_size_factor,
               "                                                                                "
              );
    bmp_printf(&(space.bmp),
               x_set,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off - x_gap * input.size(),
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(255,0,0)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               input.c_str()
              );
    display_space();
    if(input.size())
        sscanf(input.c_str(), "%u", &result);
    else
        result = default_value;
    return result;
}

//############################################################################
int LaserBoy_FLTK_GUI::display_prompt_and_echo_nibble(const string& prompt)
{
    bool   done         = false;
    u_int  result       = 0x00, // dud return value
           x_off        = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off        = space.bmp.yres - 24 * space.font_size_factor,
           y_line       = 8 * space.font_size_factor,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    prompt_escaped = false;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               space.white_text.as_uint(),
               bmp_bg_color,
               0,
               space.font_size_factor,
               prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(&(space.bmp),
               x_off,
               y_off,
               (space.rainbow_menus) ? (space.bmp.make_rgb(0,255,255)) : (space.bmp.make_rgb(space.mono_text.r, space.mono_text.g, space.mono_text.b)),
               bmp_bg_color,
               0,
               space.font_size_factor,
               "_"
              );
    display_space();
    while(!done)
    {
        do
        {
            wait_for_gui_event(&event);
        }   while(event.type != LASERBOY_GUI_KEYDOWN);
        if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
        {
            prompt_escaped = true;
            return -1;
        }
        switch(event.key.keysym.unicode)
        {
            case '0':    return 0x00;
            case '1':    return 0x01;
            case '2':    return 0x02;
            case '3':    return 0x03;
            case '4':    return 0x04;
            case '5':    return 0x05;
            case '6':    return 0x06;
            case '7':    return 0x07;
            case '8':    return 0x08;
            case '9':    return 0x09;
            case 'a':
            case 'A':    return 0x0a;
            case 'b':
            case 'B':    return 0x0b;
            case 'c':
            case 'C':    return 0x0c;
            case 'd':
            case 'D':    return 0x0d;
            case 'e':
            case 'E':    return 0x0e;
            case 'f':
            case 'F':    return 0x0f;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    } // end while(!done)
    return result;
}

//############################################################################
bool LaserBoy_FLTK_GUI::display_prompt_and_echo_bool(const string& prompt)
{
    u_int  x_off  = LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
           y_off  = space.bmp.yres - 24 * space.font_size_factor,
           y_line = 8 * space.font_size_factor,
           bmp_bg_color = space.bmp.make_rgb(space.bg_color.r, space.bg_color.g, space.bg_color.b);
    prompt_escaped = false;
    bmp_printf(   &(space.bmp)
                , x_off
                , y_off
                , bmp_bg_color
                , bmp_bg_color
                , 0
                , space.font_size_factor
                , "                                                                                "
              );
    bmp_printf(   &(space.bmp)
                , x_off
                , y_off
                , space.white_text.as_uint()
                , bmp_bg_color
                , 0
                , space.font_size_factor
                , prompt.c_str()
              );
    y_off += y_line;
    bmp_printf(   &(space.bmp)
                , x_off
                , y_off
                , bmp_bg_color
                , bmp_bg_color
                , 0
                , space.font_size_factor
                , "                                                                                "
              );
    display_space();
        do
        {
            wait_for_gui_event(&event);
        }
            while(event.type != LASERBOY_GUI_KEYDOWN);
    if(event.key.keysym.unicode == LASERBOY_KEY_ESCAPE)
    {
        prompt_escaped = true;
        return false;
    }
    if(    event.key.keysym.unicode == 'y'
        || event.key.keysym.unicode == 'Y' // this can only happen if the caps lock key is on!
        || event.key.keysym.unicode == '1'
      )
        return true;
    return false;
}

//############################################################################
void LaserBoy_FLTK_GUI::wait_4_Esc()
{
    while(true)
    {
        wait_for_gui_event(&event);
        if(!running)
            break;
        if(    event.type == LASERBOY_GUI_KEYDOWN
            && event.key.keysym.unicode == LASERBOY_KEY_ESCAPE
          )
            break;
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::wait_4_any_key()
{
    while(true)
    {
        wait_for_gui_event(&event);
        if(!running)
            break;
        if(event.type == LASERBOY_GUI_KEYDOWN)
            break;
    }
    return;
}

//############################################################################
// functions that require direct access to space.bmp
//############################################################################
bool LaserBoy_FLTK_GUI::display_bmp_directory(string bmp_dir, bool scale_to_screen)
{
    char                    file_name[128];
    DIR                    *pDIR;
    struct dirent          *pDirEnt;
    vector<string>          file_list;
    //------------------------------------------------------------------------
    pDIR = opendir(bmp_dir.c_str());
    //------------------------------------------------------------------------
    if(pDIR == NULL)
        return false;
    else
    {
        do // clear key strokes from event queue!
        {
            poll_gui_event(&event);
        } while(event.type == LASERBOY_GUI_KEYDOWN);
        display_state("loading bmp files into array");
        space.display_please_wait();
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
            u_int                       time_zero,
                                        i = 0;
            vector<struct LaserBoy_bmp> frame_array;
            //----------------------------------------------------------------
            // for systems that don't return file lists in alpha-numeric order!
            sort(file_list.begin(), file_list.end());
            //----------------------------------------------------------------
            for(i = 0; i < file_list.size(); i++)
            {
                struct LaserBoy_bmp frame = {0};
                strcpy(file_name, (bmp_dir + '/' + file_list[i]).c_str());
                if(bmp_from_file(&frame, file_name))
                {
                    struct LaserBoy_bmp to_screen = {0};
                    bmp_init(&to_screen, space.bmp.xres, space.bmp.yres, space.bmp.bpp);
                    bmp_flip(&frame);
                    if(scale_to_screen)
                    {
                        bmp_scale_into_bmp(&to_screen,
                                           &frame,
                                           frame.yres > frame.xres
                                           ? (double)to_screen.yres / (double)frame.yres
                                           : (double)to_screen.yres / (double)frame.xres
                                          );
                    }
                    else
                        bmp_into_bmp(&to_screen, &frame, 1);
                    frame_array.push_back(to_screen);
                    bmp_free(&frame);
                    display_progress(file_list.size() - i);
                }
            }
            space.clean_screen();
            laserboy_gui_delay(250);
            poll_gui_event(&event);
            i = 0;
            while(    (event.type != LASERBOY_GUI_KEYDOWN)
                   && (i < frame_array.size())
                 )
            {
                time_zero = LaserBoy_GetTicks();
                bmp_to_screen_buffer(&(frame_array.at(i)));
                i++;
                if(space.loop_animation && i >= frame_array.size())
                    i = 0;
                if(space.approximate_frame_rate)
                {
                    do{;}
                    while(   LaserBoy_GetTicks() - time_zero
                           < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                         );
                }
                poll_gui_event(&event);
            }
            for(i = 0; i < file_list.size(); i++)
                bmp_free(&(frame_array.at(i)));
            frame_array.clear();
        } // end if(file_list.size())
    } // end else (pDIR == NULL)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::display_wave(string wav_file)
{
    std::fstream  in;
    do // clear key strokes from event queue!
    {
        poll_gui_event(&event);
    } while(event.type == LASERBOY_GUI_KEYDOWN);
    in.open(wav_file.c_str(), ios::in | ios::binary);
    if(!in.is_open())
        return false;
    LaserBoy_wave_header header(in);
    if(    header.version == "!LaserBoy!"
        || !(header.LaserBoy_wave_mode & LASERBOY_WAVE_END_OF_FRAME)
      )
        display_unframed_wave(in, header);
    else
        display_LaserBoy_wave(in, header);
    in.close();
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::display_unframed_wave(string wav_file)
{
    std::fstream in;
    do // clear key strokes from event queue!
    {
        poll_gui_event(&event);
    } while(event.type == LASERBOY_GUI_KEYDOWN);
    in.open(wav_file.c_str(), ios::in | ios::binary);
    if(!in.is_open())
        return false;
    LaserBoy_wave_header header(in);
    display_unframed_wave(in, header);
    in.close();
    return true;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_LaserBoy_wave(std::fstream& in, LaserBoy_wave_header header)
{
    bool                     playing      = true         ;

    int                      i                           ,
                             span                        ,
                             last                        ,
                             red                         ,
                             green                       ,
                             blue                        ,
                             sample_count = 0            ,
                             offset_max                  ,
                             offset_min                  ,
                             center       = screen->h / 2;

    u_int                    time_zero                   ,
                             color        = space.white_text.as_uint(),
                             bmp_bg_color = space.bmp.make_rgb(space.bg_color.r,
                                                               space.bg_color.g,
                                                               space.bg_color.b
                                                              );
    double                  scale  = screen->h / 65536.0;
    struct  LaserBoy_bmp    frame_buffer = {0}          ;

    LaserBoy_wave_sample_pointer   roll_over    ;
    LaserBoy_wave_sample_pointer*  sample_window;
    prompt_escaped = false;
    //------------------------------------------------------------------------
    if(header.num_channels < 6)
    {
        bmp_printf(&(space.bmp),
                   LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
                   LASERBOY_STATS_DISPLAY_OFFSET * space.font_size_factor,
                   space.white_text.as_uint(),
                   bmp_bg_color,
                   0,
                   space.font_size_factor,
                   "this is not a LaserBoy wave        "
                  );
        display_space();
        prompt_escaped = true;
        return;
    }
    //------------------------------------------------------------------------
    if((!header.LaserBoy_wave_mode) & LASERBOY_WAVE_OFFSETS) // if no offsets in header
        for(i = 0; i < 6; i++)
            header.offset[i] = 0; // give me some!
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
    sample_window = new LaserBoy_wave_sample_pointer[span]; // at least 2
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    bmp_clone_screen_buffer(&frame_buffer, false);
    bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
    //------------------------------------------------------------------------
    while(playing)
    {
        time_zero = LaserBoy_GetTicks();
        //----------------------------------------------------------------
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        //----------------------------------------------------------------
        if(space.wave_show_inverted)
            playing = sample_window[last]->from_fstream_wave_inverted(in);
        else
            playing = sample_window[last]->from_fstream_wave(in);
        //----------------------------------------------------------------
        sample_count++;
        //----------------------------------------------------------------
        color = frame_buffer.make_rgb((   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
                                        ? (rescale_to_index(header.color_rescale_r, sample_window[red]->channel[2]))
                                        : (sample_window[red]->channel[2] >> 7)
                                      ),
                                      (   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
                                        ? (rescale_to_index(header.color_rescale_g, sample_window[green]->channel[3]))
                                        : (sample_window[green]->channel[3] >> 7)
                                      ),
                                      (   (header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
                                        ? (rescale_to_index(header.color_rescale_b, sample_window[blue]->channel[4]))
                                        : (sample_window[blue]->channel[4] >> 7)
                                      )
                                     );
        //----------------------------------------------------------------
        if(sample_window[red]->channel[2] & 0x0001)
            color = 0;
        //----------------------------------------------------------------
        if(color)
        {
            bmp_put_line_fast(   &frame_buffer
                               , (u_int)round((sample_window[0]->channel[0] *  scale) + center)
                               , (u_int)round((sample_window[0]->channel[1] * -scale) + center)
                               , (u_int)round((sample_window[1]->channel[0] *  scale) + center)
                               , (u_int)round((sample_window[1]->channel[1] * -scale) + center)
                               , color
                             );
            if(space.wave_show_vertices)
                bmp_put_vertex(   &frame_buffer
                                , (u_int)round((sample_window[1]->channel[0] *  scale) + center)
                                , (u_int)round((sample_window[1]->channel[1] * -scale) + center)
                                , color
                                , space.rendered_line_width
                              );
        }
        //----------------------------------------------------------------
        else if(space.wave_show_blanks)
        {
            bmp_put_line_fast(   &frame_buffer
                               , (u_int)round((sample_window[0]->channel[0] *  scale) + center)
                               , (u_int)round((sample_window[0]->channel[1] * -scale) + center)
                               , (u_int)round((sample_window[1]->channel[0] *  scale) + center)
                               , (u_int)round((sample_window[1]->channel[1] * -scale) + center)
                               , frame_buffer.gray
                             );
            if(space.wave_show_vertices)
                bmp_put_vertex(   &frame_buffer
                                , (u_int)round((sample_window[1]->channel[0] *  scale) + center)
                                , (u_int)round((sample_window[1]->channel[1] * -scale) + center)
                                , frame_buffer.gray
                                , space.rendered_line_width
                              );
        }
        //----------------------------------------------------------------
        if(sample_window[red]->channel[2] & 0x0001)
        {
            if(space.wave_real_time)
            {
                do
                {
                     laserboy_gui_delay(1); // based on 64hz, or 15.625 ms / tick
                }
                while(   LaserBoy_GetTicks() - time_zero
                       < (u_int)(sample_count / (header.sample_rate / 1000.0))
                     );
            }
            bmp_to_screen_buffer(&frame_buffer);
            if(space.wave_clean_screen)
                bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
            poll_gui_event(&event);
            if(event.type == LASERBOY_GUI_KEYDOWN)
                playing = false;
            sample_count = 0;
        }
        //----------------------------------------------------------------
    } // end while(playing)
    bmp_free(&frame_buffer);
    for(i = 0; i < span; i++)
        if(sample_window[i])
            delete sample_window[i];
    if(sample_window)
        delete sample_window;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_unframed_wave(std::fstream& in, LaserBoy_wave_header header)
{
    bool                     playing      = true         ;

    int                      i                           ,
                             span                        ,
                             last                        ,
                             red                         ,
                             green                       ,
                             blue                        ,
                             sample_count = 0            ,
                             offset_max                  ,
                             offset_min                  ,
                             center       = screen->h / 2;

    u_int                    time_zero                     ,
                             color        = space.white_text.as_uint();

    double                   scale  = screen->h / 65536.0;
    struct  LaserBoy_bmp     frame_buffer = {0}          ;

    LaserBoy_wave_sample_pointer   roll_over    ;
    LaserBoy_wave_sample_pointer*  sample_window;
    //------------------------------------------------------------------------
    offset_max = space.wave_offsets[0];
    for(i = 1; i < 6; i++)
        if(space.wave_offsets[i] > offset_max)
            offset_max = space.wave_offsets[i];
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        space.wave_offsets[i] -= offset_max;
    //------------------------------------------------------------------------
    for(i = 0; i < 6; i++)
        header.offset[i] = space.wave_offsets[i]; // use current system offsets
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
    sample_window = new LaserBoy_wave_sample_pointer[span]; // at least 2
    for(i = 0; i < span; i++)
        sample_window[i] = new LaserBoy_wave_sample(header.num_channels);
    //------------------------------------------------------------------------
    bmp_clone_screen_buffer(&frame_buffer, false);
    bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
    //------------------------------------------------------------------------
    while(playing)
    {
        time_zero = LaserBoy_GetTicks();
        //----------------------------------------------------------------
        roll_over = sample_window[0];
        for(i = 0; i < last; i++)
            sample_window[i] = sample_window[i + 1];
        sample_window[last] = roll_over;
        //----------------------------------------------------------------
        if(space.wave_show_inverted)
            playing = sample_window[last]->from_fstream_wave_inverted(in);
        else
            playing = sample_window[last]->from_fstream_wave(in);
        //----------------------------------------------------------------
        sample_count++;
        //----------------------------------------------------------------
        if(header.num_channels >= 6)
            color = frame_buffer.make_rgb(   (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[green]->channel[3] > 0)
                                               ? (sample_window[green]->channel[3] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[blue ]->channel[4] > 0)
                                               ? (sample_window[blue ]->channel[4] / 128)
                                               : (0)
                                             )
                                         );
        //----------------------------------------------------------------
        else if(header.num_channels >= 4)
            color = frame_buffer.make_rgb(   (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                           , (   (sample_window[red  ]->channel[2] > 0)
                                               ? (sample_window[red  ]->channel[2] / 128)
                                               : (0)
                                             )
                                         );
        //----------------------------------------------------------------
        else
            color = frame_buffer.make_rgb(255, 255, 255);
        //----------------------------------------------------------------
        bmp_put_line_fast(&frame_buffer,
                          (u_int)round((sample_window[0]->channel[0] *  scale) + center),
                          (u_int)round((sample_window[0]->channel[1] * -scale) + center),
                          (u_int)round((sample_window[1]->channel[0] *  scale) + center),
                          (u_int)round((sample_window[1]->channel[1] * -scale) + center),
                          color
                         );

        if(space.wave_show_vertices)
            bmp_put_vertex(&frame_buffer,
                           (u_int)round((sample_window[1]->channel[0] *  scale) + center),
                           (u_int)round((sample_window[1]->channel[1] * -scale) + center),
                           color,
                           space.rendered_line_width
                          );
        //----------------------------------------------------------------
        if(!(sample_count % space.unframed_wave_stride))
        {
            if(space.wave_real_time)
            {
                do
                {
                     laserboy_gui_delay(1); // based on 64hz, or 15.625 ms / tick
                }
                while(   LaserBoy_GetTicks() - time_zero
                       < (u_int)(sample_count / (header.sample_rate / 1000.0))
                     );
            }
            bmp_to_screen_buffer(&frame_buffer);
            if(space.wave_clean_screen)
                bmp_fill(&frame_buffer, space.bg_color.r, space.bg_color.g, space.bg_color.b);
            poll_gui_event(&event);
            if(event.type == LASERBOY_GUI_KEYDOWN)
                playing = false;
            sample_count = 0;
        }
        //----------------------------------------------------------------
    } // end while(playing)
    bmp_free(&frame_buffer);
    for(i = 0; i < span; i++)
        if(sample_window[i])
            delete sample_window[i];
    if(sample_window)
        delete sample_window;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::check_for_nav_keys(LaserBoy_GUI_Key key)
{
    switch(key)
    {
        default:
            space.TUI_clue = "bad key";
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_RSHIFT:
        case LASERBOY_KEY_LSHIFT:
            space.TUI_clue = "";
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_UP: // show next palette
            if(space.show_target_palette)
            {
                space.target_palette_index++;
                if((int)space.target_palette_index >= space.number_of_palettes())
                    space.target_palette_index = 0;
                space.TUI_clue = "+ target palette";
            }
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_DOWN: // show previous palette
            if(space.show_target_palette)
            {
                if(space.target_palette_index == 0)
                    space.target_palette_index = space.number_of_palettes();
                space.target_palette_index--;
                space.TUI_clue = "- target palette";
            }
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_RIGHT: // show previous frame
            if(    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
                space.pre_incr_current_frame();
            space.TUI_clue = "+ frame";
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_LEFT: // show next frame
            if(    space.loop_animation
                || (space.frame_index > 0)
              )
                space.pre_decr_current_frame();
            space.TUI_clue = "- frame";
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_TAB:
            space.set_current_menu_name("value settings", 0x00400000, true);
            system_settings_menu();
            break;
        //------------------------------------------------------------
        case ' ': // [Space] select, deselect current frame
            space.current_frame().is_selected = !(space.current_frame().is_selected);
            if(space.current_frame().is_selected)
                space.TUI_clue = "select frame";
            else
                space.TUI_clue = "deselect frame";
            break;
        //----------------------------------------------------------------
        case LASERBOY_KEY_F3:
            capture_screen();
            space.TUI_clue = "Screen Captured";
            break;
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::check_for_digit_keys(LaserBoy_GUI_Key key)
{
    char formatted_string[80] = {0};
    switch(key)
    {
        default:
            space.TUI_clue = "bad key";
            break;
        //------------------------------------------------------------
        case LASERBOY_KEY_RSHIFT:
        case LASERBOY_KEY_LSHIFT:
            space.TUI_clue = "";
            break;
        //------------------------------------------------------------
        case '1':
            space.view_angle.x += space.rotation_step;
            if(space.view_angle.x > two_pi)
                space.view_angle.x -= two_pi;
            space.TUI_clue = "+ orbit X";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '!':
            space.view_angle.x -= space.rotation_step;
            if(space.view_angle.x < -two_pi)
                space.view_angle.x += two_pi;
            space.TUI_clue = "- orbit X";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '2':
            space.view_angle.y += space.rotation_step;
            if(space.view_angle.y > two_pi)
                space.view_angle.y -= two_pi;
            space.TUI_clue = "+ orbit Y";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '@':
            space.view_angle.y -= space.rotation_step;
            if(space.view_angle.y < -two_pi)
                space.view_angle.y += two_pi;
            space.TUI_clue = "- orbit Y";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '3':
            space.view_angle.z += space.rotation_step;
            if(space.view_angle.z > two_pi)
                space.view_angle.z -= two_pi;
            space.TUI_clue = "+ orbit Z";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '#':
            space.view_angle.z -= space.rotation_step;
            if(space.view_angle.z < -two_pi)
                space.view_angle.z += two_pi;
            space.TUI_clue = "- orbit Z";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '4':
            space.view_angle += space.rotation_step;
            if(space.view_angle.x > two_pi)
                space.view_angle.x -= two_pi;
            if(space.view_angle.y > two_pi)
                space.view_angle.y -= two_pi;
            if(space.view_angle.z > two_pi)
                space.view_angle.z -= two_pi;
            space.TUI_clue = "+ orbit XYZ";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '$':
            space.view_angle -= space.rotation_step;
            if(space.view_angle.x < -two_pi)
                space.view_angle.x += two_pi;
            if(space.view_angle.y < -two_pi)
                space.view_angle.y += two_pi;
            if(space.view_angle.z < -two_pi)
                space.view_angle.z += two_pi;
            space.TUI_clue = "- orbit XYZ";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '5':
            space.view_angle   = 0;
            space.TUI_clue     = "front";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '%':
            space.view_angle.x = 0;
            space.view_angle.y = pi;
            space.view_angle.z = 0;
            space.TUI_clue     = "back";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '6':
            space.view_angle.x = 0;
            space.view_angle.y = half_pi;
            space.view_angle.z = 0;
            space.TUI_clue     = "left side";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '^':
            space.view_angle.x = 0;
            space.view_angle.y = -half_pi;
            space.view_angle.z = 0;
            space.TUI_clue     = "right side";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '7':
            space.view_angle.x = half_pi;
            space.view_angle.y = 0;
            space.view_angle.z = 0;
            space.TUI_clue     = "top";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '&':
            space.view_angle.x = -half_pi;
            space.view_angle.y = 0;
            space.view_angle.z = 0;
            space.TUI_clue     = "bottom";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '8':
            space.view_angle += space.rotation_step;
            if(    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
                space.pre_incr_current_frame();
            space.TUI_clue = "+ frame & orbit";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '*':
            space.view_angle -= space.rotation_step;
            if(    space.loop_animation
                || (space.frame_index > 0)
              )
                space.pre_decr_current_frame();
            space.TUI_clue = "- frame & orbit";
            space.view_has_changed = true;
            break;
        //------------------------------------------------------------
        case '9':
            space.frame(0);
            space.TUI_clue = "first frame";
            break;
        //------------------------------------------------------------
        case '0':
            space.frame(space.number_of_frames() - 1);
            space.TUI_clue = "last frame";
            break;
        //------------------------------------------------------------
        case '(': // set, unset start frame select
            space.toggle_frame_select_start();
            if(space.frame_select_start != -1)
            {
                sprintf(formatted_string, "start = %d", space.frame_select_start);
                space.TUI_clue = formatted_string;
            }
            else
                space.TUI_clue = "start unset";
            break;
        //------------------------------------------------------------
        case ')': // set, unset end frame select
            space.toggle_frame_select_end();
            if(space.frame_select_end != -1)
            {
                sprintf(formatted_string, "end = %d", space.frame_select_end);
                space.TUI_clue = formatted_string;
            }
            else
                space.TUI_clue = "end unset";
            break;
        //------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::animate_forward()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "+ show";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.pre_incr_current_frame();
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::animate_reverse()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "- show";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.pre_decr_current_frame();
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::rotate_forward()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "+ orbit";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.view_angle += one_degree;
        space.view_has_changed = true;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::rotate_reverse()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "- orbit";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.view_angle -= one_degree;
        space.view_has_changed = true;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::animate_rotate_forward()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "+ show & orbit";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index < space.number_of_frames() - 1)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.pre_incr_current_frame();
        space.view_angle += one_degree;
        space.view_has_changed = true;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::animate_rotate_reverse()
{
    u_int  time_zero;
    laserboy_gui_delay(250);
    poll_gui_event(&event);
    space.TUI_clue = "- show & orbit";
    while(    (event.type != LASERBOY_GUI_KEYDOWN)
           && (    space.loop_animation
                || (space.frame_index > 0)
              )
         )
    {
        time_zero = LaserBoy_GetTicks();
        space.pre_decr_current_frame();
        space.view_angle -= one_degree;
        space.view_has_changed = true;
        space.clean_screen();
        space.render_space();
        display_space();
        if(space.approximate_frame_rate)
        {
            do{;}
            while(   LaserBoy_GetTicks() - time_zero
                   < (u_int)((1.0 / space.frames_per_second) * 1000.0)
                 );
        }
        poll_gui_event(&event);
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::system_settings_menu()
{
    bool in_menu   = true;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_settings_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    space.set_current_menu_name("visual settings", 0x00000040, true);
                    visuals_settings_menu();
                    break;
                //------------------------------------------------------------
                case '2':
                    space.set_current_menu_name("apply view", 0x00300030, true);
                    apply_view_menu();
                    break;
                //------------------------------------------------------------
                case '3':
                    space.set_current_menu_name("display options", 0x00300030, true);
                    display_options_menu();
                    break;
                //------------------------------------------------------------
                case '4':
                    space.set_current_menu_name("vector editor values", 0x00202040, true);
                    vector_editor_values_menu();
                    break;
                //------------------------------------------------------------
                case '5':
                    space.set_current_menu_name("optimizations factors", 0x00402020, true);
                    optimization_factors_menu();
                    break;
                //------------------------------------------------------------
                case '6':
                    space.set_current_menu_name("timing & wave settings", 0x00204020, true);
                    timing_and_wave_settings_menu();
                    break;
                //------------------------------------------------------------
                case '7':
                    space.set_current_menu_name("file settings", 0x00003030, true);
                    file_type_specific_options_menu();
                    break;
                //------------------------------------------------------------
                case '8':
                    space.set_current_menu_name("bit resolution", 0x00400000, true);
                    bit_resolution_menu();
                    break;
                //------------------------------------------------------------
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::optimization_factors_menu()
{
    bool   in_menu         = true;
    u_int  temp_u_int;
    double temp_double;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_optimization_factors_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    temp_double = display_prompt_and_echo_double(   "insignificant angle in degrees 0.0 to 180.0"
                                                                   , space.insignificant_angle * one_radian
                                                                   , 180.0
                                                                   , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.insignificant_angle = temp_double * one_degree;
                    break;
                //------------------------------------------------------------
                case '2':
                    temp_double = display_prompt_and_echo_double(   "insignificant distance in points"
                                                                  , space.insignificant_distance
                                                                  , 65535.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    if(temp_double < 0.00)
                        temp_double = 0.00;
                    space.insignificant_distance = temp_double;
                    break;
                //------------------------------------------------------------
                case '3':
                    temp_double = display_prompt_and_echo_double(   "maximum lit vector length in points"
                                                                  , space.lit_delta_max
                                                                  , 65535.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.lit_delta_max = temp_double;
                    break;
                //------------------------------------------------------------
                case '4':
                    temp_double = display_prompt_and_echo_double(   "maximum blank vector length in points"
                                                                  , space.blank_delta_max
                                                                  , 65535.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.blank_delta_max = temp_double;
                    break;
                //------------------------------------------------------------
                case '5':
                    temp_u_int = display_prompt_and_echo_u_char("black level (0-255)", 0);
                    if(prompt_escaped)
                        break;
                    space.black_level = (u_char)temp_u_int;
                    break;
                //------------------------------------------------------------
                case '6':
                    temp_double = display_prompt_and_echo_double(   "max time for wagging in microseconds"
                                                                  , space.max_wag_microsec
                                                                  , 1000000.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.max_wag_microsec = temp_double;
                    break;
                //------------------------------------------------------------
                case '7':
                    temp_double = display_prompt_and_echo_double(   "dwell in microseconds for 180 degree angle"
                                                                  , space.max_dwell_millisec
                                                                  , 1000000.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.max_dwell_millisec = temp_double;
                    break;
                //------------------------------------------------------------
                case '8':
                    temp_u_int = display_prompt_and_echo_u_int(   "lit dwell overhang"
                                                                , space.lit_dwell_overhang
                                                                , 4800
                                                              );
                    if(prompt_escaped)
                        break;
                    space.lit_dwell_overhang = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '9':
                    temp_u_int = display_prompt_and_echo_u_int(   "dwell on dot"
                                                                , space.dwell_on_dot
                                                                , 4800
                                                              );
                    if(prompt_escaped)
                        break;
                    space.dwell_on_dot = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '0':
                    dots_setting_menu();
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.no_equivalent_vectors = !space.no_equivalent_vectors;
                    if(space.no_equivalent_vectors)
                        space.TUI_clue = "+ equal vector remove";
                    else
                        space.TUI_clue = "- equal vector remove";
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    space.conglomerate_in_minimize = !space.conglomerate_in_minimize;
                    if(space.conglomerate_in_minimize)
                        space.TUI_clue = "+ conglomerate in minimize";
                    else
                        space.TUI_clue = "- conglomerate in minimize";
                    break;
                //------------------------------------------------------------
                case 'c':
                case 'C':
                    space.fracture_b4_conglomerate = !space.fracture_b4_conglomerate;
                    if(space.fracture_b4_conglomerate)
                        space.TUI_clue = "+ fracture conglomerate";
                    else
                        space.TUI_clue = "- fracture conglomerate";
                    break;
                //------------------------------------------------------------
                case 'd':
                case 'D':
                    space.black_dwell_vertices = !space.black_dwell_vertices;
                    if(space.black_dwell_vertices)
                        space.TUI_clue = "black dwell";
                    else
                        space.TUI_clue = "lit dwell";
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    space.no_black_match = !space.no_black_match;
                    if(space.no_black_match)
                        space.TUI_clue = "no black match";
                    else
                        space.TUI_clue = "black match palette";
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.allow_lit_black = !space.allow_lit_black;
                    if(space.allow_lit_black)
                        space.TUI_clue = "allow lit black";
                    else
                        space.TUI_clue = "blank black";
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    space.save_1_frame_bridge = !space.save_1_frame_bridge;
                    if(space.save_1_frame_bridge)
                        space.TUI_clue = "bridge in single ild";
                    else
                        space.TUI_clue = "no bridge in ild save";
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    space.auto_minimize = !space.auto_minimize;
                    if(space.auto_minimize)
                        space.TUI_clue = "minimize ild, dxf, txt output";
                    else
                        space.TUI_clue = "raw ild, dxf, txt output";
                    break;
                //------------------------------------------------------------
                case 'i':
                case 'I':
                    space.auto_opti_ctn = !space.auto_opti_ctn;
                    if(space.auto_opti_ctn)
                        space.TUI_clue = "auto optimize ctn output";
                    else
                        space.TUI_clue = "raw ctn output";
                    break;
                //------------------------------------------------------------
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::vector_editor_values_menu()
{
    bool   in_menu         = true;
    int    temp_int;
    double temp_double;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_vector_editor_values_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    temp_int = display_prompt_and_echo_u_int(   "move points per key"
                                                              , space.displacement_step
                                                              , 65535
                                                            );
                    if(prompt_escaped)
                        break;
                    space.displacement_step = temp_int;
                    break;
                //------------------------------------------------------------
                case '2':
                    temp_double = 0.0;
                    while(temp_double == 0.0)
                    {
                        temp_double = display_prompt_and_echo_double(   "move 1 / [n] of space per key"
                                                                      , 65536.0 / space.displacement_step
                                                                      , 65536.0
                                                                      , 0.0
                                                                    );
                        if(prompt_escaped)
                            break;
                    }
                    if(temp_double == 0.0)
                        break;
                    space.displacement_step = (int)(65536.0 / temp_double);
                    break;
                //------------------------------------------------------------
                case '3':
                    temp_double = 0.0;
                    while(temp_double == 0.0)
                    {
                        temp_double = display_prompt_and_echo_double(   "rotate degrees per key hit"
                                                                       , space.insignificant_angle * one_radian
                                                                       , 180.0
                                                                       , 0.0
                                                                    );
                        if(prompt_escaped)
                            break;
                    }
                    if(temp_double == 0.0)
                    {
                        space.display_error("angle must non-zero up to 180 degrees.");
                        wait_4_Esc();
                        break;
                    }
                    space.rotation_step = temp_double * one_degree;
                    break;
                //------------------------------------------------------------
                case '4':
                    temp_double = display_prompt_and_echo_double(   "scale %% per key hit"
                                                                  , space.magnitude_step * 100.0
                                                                  , DBL_MAX
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.magnitude_step = temp_double / 100.0;
                    space.magnitude_step_up = 1.00 + space.magnitude_step;
                    space.magnitude_step_dn = 1.00 - space.magnitude_step;
                    break;
                //------------------------------------------------------------
                case '5':
                    temp_double = 0.0;
                    while(temp_double == 0.0)
                    {
                        temp_double = display_prompt_and_echo_double(   "rendered arc segment angle in degrees"
                                                                       , space.rendered_arc_angle
                                                                       ,  180.0
                                                                       , -180.0
                                                                    );
                        if(prompt_escaped)
                            break;
                    }
                    if(temp_double == 0.0)
                    {
                        space.display_error("angle must be non-zero, between -180 and +180 degrees.");
                        wait_4_Esc();
                        break;
                    }
                    space.rendered_arc_angle = temp_double;
                    break;
                //------------------------------------------------------------
                case '6':
                    temp_double = display_prompt_and_echo_double(   "%% rendered mono font field space"
                                                                  , space.rendered_mono_font_space * 100.0
                                                                  , DBL_MAX
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.rendered_mono_font_space = temp_double / 100.0;
                    break;
                //------------------------------------------------------------
                case '7':
                    temp_double = display_prompt_and_echo_double(   "%% rendered variable font gap"
                                                                  , space.rendered_vari_font_gap * 100.0
                                                                  , DBL_MAX
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.rendered_vari_font_gap = temp_double / 100.0;
                    break;
                //------------------------------------------------------------
                case '8':
                    temp_double = display_prompt_and_echo_double(   "%% rendered variable font space"
                                                                  , space.rendered_vari_font_space * 100.0
                                                                  , DBL_MAX
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.rendered_vari_font_space = temp_double / 100.0;
                    break;
                //------------------------------------------------------------
                case '9':
                    space.destructive_clipping = !space.destructive_clipping;
                    if(space.destructive_clipping)
                        space.TUI_clue = "destructive clipping";
                    else
                        space.TUI_clue = "solid bounds";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.maintain_real_origin = !space.maintain_real_origin;
                    if(space.maintain_real_origin)
                        space.TUI_clue = "normalize to origin";
                    else
                        space.TUI_clue = "normalize to itself";
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.bond_word = !space.bond_word;
                    if(space.bond_word)
                        space.TUI_clue = "bond word on";
                    else
                        space.TUI_clue = "bond word off";
                    break;
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::timing_and_wave_settings_menu()
{
    bool   in_menu         = true;
    u_int  temp_u_int;
    double temp_double;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_timing_and_wave_settings_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    space.set_current_menu_name("set wave offsets", 0x00002040, true);
                    set_wave_offsets_menu();
                    break;
                //------------------------------------------------------------
                case '2':
                    temp_double = display_prompt_and_echo_double(   "frames per second"
                                                                  , space.frames_per_second
                                                                  , space.sample_rate
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.frames_per_second = temp_double;
                    break;
                //------------------------------------------------------------
                case '3':
                    temp_double = display_prompt_and_echo_double(   "wave frame still output duration in seconds"
                                                                  , space.still_frame_duration
                                                                  , 3600.0 // one hour
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.still_frame_duration = temp_double;
                    break;
                //------------------------------------------------------------
                case '4':
                    temp_u_int = display_prompt_and_echo_u_int(   "output wave sample rate"
                                                                , space.sample_rate
                                                                , 2000000 // 2MHz
                                                              );
                    if(prompt_escaped)
                        break;
                    space.sample_rate = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '5':
                    temp_u_int = display_prompt_and_echo_u_int(   "unframed wave samples per frame"
                                                                , space.unframed_wave_stride
                                                                , 65536
                                                              );
                    if(prompt_escaped)
                        break;
                    space.unframed_wave_stride = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '6':
                    space.set_current_menu_name("signal if not z", 0x00003030, true);
                    signal_if_not_z_menu();
                    break;
                //------------------------------------------------------------
                case '7':
                    space.z_from_unformatted = !space.z_from_unformatted;
                    if(space.z_from_unformatted)
                        space.TUI_clue = "Z in unformatted wave";
                    else
                        space.TUI_clue = "no Z unformatted wave";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.auto_flatten_z = !space.auto_flatten_z;
                    if(space.auto_flatten_z)
                        space.TUI_clue = "flatten Z for wave";
                    else
                        space.TUI_clue = "keep Z for wave";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.invert_wave_output = !space.invert_wave_output;
                    if(space.invert_wave_output)
                        space.TUI_clue = "inverted wave output";
                    else
                        space.TUI_clue = "positive wave output";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.auto_apply_offsets = !space.auto_apply_offsets;
                    if(space.auto_apply_offsets)
                        space.TUI_clue = "apply system offsets to wave";
                    else
                        space.TUI_clue = "make wave with no offsets";
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.auto_apply_rescales = !space.auto_apply_rescales;
                    if(space.auto_apply_rescales)
                        space.TUI_clue = "apply system rescales to wave";
                    else
                        space.TUI_clue = "make wave linear color";
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    if(space.channels_of_wav_out == 8)
                    {
                        space.channels_of_wav_out = 6;
                        space.TUI_clue = "6 channel wave out";
                    }
                    else
                    {
                        space.channels_of_wav_out = 8;
                        space.TUI_clue = "8 channel wave out";
                    }
                    break;
                //------------------------------------------------------------
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::display_options_menu()
{
    bool   in_menu         = true;
    u_int  temp_u_int;
    double temp_double;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_display_options_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit value settings";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case '1':
                    temp_u_int = display_prompt_and_echo_u_int(   "rendered vector line width"
                                                                , space.rendered_line_width
                                                                , yres
                                                              );
                    if(prompt_escaped)
                        break;
                    if(temp_u_int == 0)
                        temp_u_int = 1;
                    space.rendered_line_width = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '2':
                    temp_u_int = display_prompt_and_echo_u_int(   "menu font size factor"
                                                                , space.font_size_factor
                                                                , 8
                                                              );
                    if(prompt_escaped)
                        break;
                    if(temp_u_int <= 0)
                        temp_u_int = 1;
                    space.font_size_factor = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '3':
                    temp_double = display_prompt_and_echo_double(   "background bitmap size %%"
                                                                  , space.background_bmp_scale * 100.0
                                                                  , 2500.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.background_bmp_scale = temp_double / 100.00;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case '4':
                    space.set_current_menu_name("background color", 0x00400000, true);
                    edit_background_color_menu();
                    break;
                //------------------------------------------------------------
                case '5':
                    space.set_current_menu_name("mono text color", 0x00300020, true);
                    edit_mono_text_color_menu();
                    break;
                //------------------------------------------------------------
                case '6':
                    space.set_current_menu_name("blank rgb", 0x00400020, true);
                    edit_rendered_blank_menu();
                    break;
                //------------------------------------------------------------
                case '7':
                    space.set_current_menu_name("black rgb", 0x00200040, true);
                    edit_rendered_black_menu();
                    break;
                //------------------------------------------------------------
                case '8':
                    temp_double = display_prompt_and_echo_double(   "onion skin fade %%"
                                                                  , space.onion_skin_fade * 100.0
                                                                  , 100.0
                                                                  , 0.0
                                                                );
                    if(prompt_escaped)
                        break;
                    space.onion_skin_fade = temp_double / 100.0;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case '9':
                    temp_u_int = display_prompt_and_echo_u_int(   "onion skin depth"
                                                                , space.number_of_skins
                                                                , 20
                                                              );
                    if(prompt_escaped)
                        break;
                    space.number_of_skins = (u_char)temp_u_int;
                    space.recolor_background();
                    break;
                //------------------------------------------------------------
                case '0':
                    temp_u_int = display_prompt_and_echo_u_int(   "output bitmap size in pixels square"
                                                                , space.output_bmp_size
                                                                , 37837 // ridiculous
                                                              );
                    if(prompt_escaped)
                        break;
                    space.output_bmp_size = temp_u_int;
                    break;
                //------------------------------------------------------------
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::edit_background_color_menu()
{
    prompt_escaped = false;
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.bg_color.r = r;
    if((space.white_text - space.bg_color).sum() < 384)
        space.white_text = LaserBoy_color();
    else
        space.white_text = LaserBoy_color(255,255,255);
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.bg_color.g = g;
    if((space.white_text - space.bg_color).sum() < 384)
        space.white_text = LaserBoy_color();
    else
        space.white_text = LaserBoy_color(255,255,255);
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    space.bg_color.b = b;
    if((space.white_text - space.bg_color).sum() < 384)
        space.white_text = LaserBoy_color();
    else
        space.white_text = LaserBoy_color(255,255,255);
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    space.TUI_clue = "background color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::edit_mono_text_color_menu()
{
    prompt_escaped = false;
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.mono_text.r = r;
    if(!space.rainbow_menus)
        space.white_text = space.mono_text;
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.mono_text.g = g;
    if(!space.rainbow_menus)
        space.white_text = space.mono_text;
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    space.mono_text.b = b;
    if(!space.rainbow_menus)
        space.white_text = space.mono_text;
    space.clean_screen();
    space.render_space();
    space.TUI_clue = "mono text color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::edit_rendered_blank_menu()
{
    prompt_escaped = false;
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_blank.r = r;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_blank.g = g;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_blank.b = b;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    space.TUI_clue = "blank line color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::edit_rendered_black_menu()
{
    prompt_escaped = false;
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_black.r = r;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_black.g = g;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    space.rendered_black.b = b;
    space.recolor_background();
    space.clean_screen();
    space.render_space();
    space.TUI_clue = "black line color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::signal_if_not_z_menu()
{
    bool  bad_key      = true;
    space.TUI_clue = "signal if not Z";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_if_not_z_options();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    space.TUI_clue = "action canceled";
                    return;
                //------------------------------------------------------------
                case '1':
                    space.signal_if_not_z = LASERBOY_NO_SIGNAL;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '2':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_TTL;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '3':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_AVG_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '4':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_OR_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '5':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '6':
                    space.signal_if_not_z = LASERBOY_SIGNAL_MONO_O_SCOPE;
                    bad_key = false;
                    break;
            }
        }
    }
    space.clean_screen();
    space.overlay_if_not_z_options();
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::dots_setting_menu()
{
    bool  bad_key           = true;
    space.TUI_clue = "dots setting";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_dots_options();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    space.TUI_clue = "action canceled";
                    return;
                //------------------------------------------------------------
                case '1':
                    space.dots_setting = LASERBOY_DOTS_IGNORE;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '2':
                    space.dots_setting = LASERBOY_DOTS_REMOVE;
                    bad_key = false;
                    break;
                //------------------------------------------------------------
                case '3':
                    space.dots_setting = LASERBOY_DOTS_ENHANCE;
                    bad_key = false;
                    break;
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::bit_resolution_menu()
{
    bool   in_menu = true;
    u_int  temp_u_int;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_bit_resolution_settings();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit bit resolution";
                    break;
                //------------------------------------------------------------
                default:
                    space.TUI_clue = "bad key";
                    check_for_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for X");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[0] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for Y");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[1] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'c':
                case 'C':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for Z or other");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[5] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'd':
                case 'D':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for red");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[2] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for green");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[3] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for blue");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[4] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for L audio");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[6] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    space.clean_screen();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    temp_u_int = display_prompt_and_echo_nibble("number of bits to mask (0 ~ f) for R audio");
                    if(prompt_escaped)
                        break;
                    space.signal_bit_mask[7] = temp_u_int;
                    break;
                //------------------------------------------------------------
                case 'i':
                    space.clean_screen();
                    space.current_frame().impose_bit_resolution();
                    space.render_space();
                    space.overlay_bit_resolution_settings();
                    display_space();
                    space.TUI_clue = "imposed bit resolution";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.TUI_clue = "imposing bit resolution";
                    space.clean_screen();
                    space.impose_bit_resolution();
                    space.render_space();
                    display_space();
                    space.TUI_clue = "imposed bit resolution all";
                    break;
                //------------------------------------------------------------
            } // end switch(gui_keysym_to_char(&event.key.keysym))
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::visuals_settings_menu()
{
    bool in_menu = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_visuals_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit UI visuals";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case '1':
                    space.show_vectors = !space.show_vectors;
                    if(space.show_vectors)
                        space.TUI_clue = "show vectors";
                    else
                        space.TUI_clue = "hide vectors";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.show_vertices = !space.show_vertices;
                    if(space.show_vertices)
                        space.TUI_clue = "show vertices";
                    else
                        space.TUI_clue = "hide vertices";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.show_direction = !space.show_direction;
                    if(space.show_direction)
                        space.TUI_clue = "show direction";
                    else
                        space.TUI_clue = "hide direction";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.show_blanking = !space.show_blanking;
                    if(space.show_blanking)
                        space.TUI_clue = "show blanking";
                    else
                        space.TUI_clue = "hide blanking";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.show_intro = !space.show_intro;
                    if(space.show_intro)
                        space.TUI_clue = "show intro";
                    else
                        space.TUI_clue = "hide intro";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.show_bridge = !space.show_bridge;
                    if(space.show_bridge)
                        space.TUI_clue = "show bridge";
                    else
                        space.TUI_clue = "hide bridge";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.show_coda = !space.show_coda;
                    if(space.show_coda)
                        space.TUI_clue = "show coda";
                    else
                        space.TUI_clue = "hide coda";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.show_indices = !space.show_indices;
                    if(space.show_indices)
                        space.TUI_clue = "show indices";
                    else
                        space.TUI_clue = "hide indices";
                    break;
                //------------------------------------------------------------
                case '9':
                    space.show_palette = !space.show_palette;
                    if(space.show_palette)
                        space.TUI_clue = "show palette";
                    else
                        space.TUI_clue = "hide palette";
                    break;
                //------------------------------------------------------------
                case '0':
                    space.show_target_palette = !space.show_target_palette;
                    if(space.show_target_palette)
                        space.TUI_clue = "show target palette";
                    else
                        space.TUI_clue = "hide target palette";
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.show_stats = !space.show_stats;
                    if(space.show_stats)
                        space.TUI_clue = "show stats";
                    else
                        space.TUI_clue = "hide stats";
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    space.transparent_menu_font = (!space.transparent_menu_font) * 4;
                    if(space.transparent_menu_font == 4)
                        space.TUI_clue = "transparent menu font";
                    else
                        space.TUI_clue = "opaque menu font";
                    break;
                //------------------------------------------------------------
                case 'c':
                case 'C':
                    space.rainbow_menus = !space.rainbow_menus;
                    if(space.rainbow_menus)
                    {
                        if(space.bg_color.sum() >= 384)
                            space.white_text = LaserBoy_color(0, 0, 0);
                        else
                            space.white_text = LaserBoy_color(255, 255, 255);
                        space.TUI_clue = "rainbow menus";
                    }
                    else
                    {
                        space.white_text = space.mono_text;
                        space.TUI_clue = "no rainbows for you";
                    }
                    break;
                //------------------------------------------------------------
                case 'd':
                case 'D':
                    space.show_fixed_bounds = !space.show_fixed_bounds;
                    if(space.show_fixed_bounds)
                        space.TUI_clue = "show fixed bounds";
                    else
                        space.TUI_clue = "hide fixed bounds";
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    space.show_fixed_origin = !space.show_fixed_origin;
                    if(space.show_fixed_origin)
                        space.TUI_clue = "show fixed origin";
                    else
                        space.TUI_clue = "hide fixed origin";
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.show_floating_axis = !space.show_floating_axis;
                    if(space.show_floating_axis)
                        space.TUI_clue = "show floating axis";
                    else
                        space.TUI_clue = "hide floating axis";
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    space.show_floating_bounds = !space.show_floating_bounds;
                    if(space.show_floating_bounds)
                        space.TUI_clue = "show floating bounds";
                    else
                        space.TUI_clue = "hide floating bounds";
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    space.show_onion_skin = !space.show_onion_skin;
                    if(space.show_onion_skin)
                        space.TUI_clue = "show onion skin";
                    else
                        space.TUI_clue = "hide onion skin";
                    break;
                //------------------------------------------------------------
                case 'i':
                case 'I':
                    space.show_onion_visuals = !space.show_onion_visuals;
                    if(space.show_onion_visuals)
                        space.TUI_clue = "show onion visuals";
                    else
                        space.TUI_clue = "hide onion visuals";
                    break;
                //------------------------------------------------------------
                case 'j':
                case 'J':
                    space.show_Z_as_order = !space.show_Z_as_order;
                    if(space.show_Z_as_order)
                        space.TUI_clue = "show order in Z";
                    else
                        space.TUI_clue = "show normal 3D";
                    break;
                //------------------------------------------------------------
                case 'k':
                case 'K':
                    space.show_quad_view = !space.show_quad_view;
                    space.view_has_changed = true;
                    if(space.show_quad_view)
                        space.TUI_clue = "show quad view";
                    else
                        space.TUI_clue = "hide quad view";
                    break;
                //------------------------------------------------------------
                case 'l':
                case 'L':
                    space.loop_animation = !space.loop_animation;
                    if(space.loop_animation)
                        space.TUI_clue = "loop animation";
                    else
                        space.TUI_clue = "stop animatiion at ends";
                    break;
                //------------------------------------------------------------
                case 'm':
                case 'M':
                    space.approximate_frame_rate = !space.approximate_frame_rate;
                    if(space.approximate_frame_rate)
                        space.TUI_clue = "aproximate frame rate";
                    else
                        space.TUI_clue = "as fast as possible";
                    break;
                //------------------------------------------------------------
                case 'n':
                case 'N':
                    space.show_effects_generation = !space.show_effects_generation;
                    if(space.show_effects_generation)
                        space.TUI_clue = "show live rendering";
                    else
                        space.TUI_clue = "hide live rendering";
                    break;
                //------------------------------------------------------------
                case 'o':
                case 'O':
                    space.show_cursor_limits_msg = !space.show_cursor_limits_msg;
                    if(space.show_cursor_limits_msg)
                        space.TUI_clue = "show cursor limits";
                    else
                        space.TUI_clue = "hide cursor limits";
                    break;
                //------------------------------------------------------------
                case 'p':
                case 'P':
                    space.infinite_vector = !space.infinite_vector;
                    if(space.infinite_vector)
                        space.TUI_clue = "show infinite vector";
                    else
                        space.TUI_clue = "hide infinite vector";
                    break;
                //------------------------------------------------------------
                case 'q':
                case 'Q':
                    space.show_TUI_clue = !space.show_TUI_clue;
                    if(space.show_TUI_clue)
                        space.TUI_clue = "show UI hint";
                    else
                        space.TUI_clue = "hide UI hint";
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    space.track_bmp_view = !space.track_bmp_view;
                    if(space.track_bmp_view)
                        space.TUI_clue = "track bmp view";
                    else
                        space.TUI_clue = "fixed bmp";
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::file_type_specific_options_menu()
{
    bool in_menu = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_file_type_specific_options_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_F3:
                    capture_screen();
                    space.TUI_clue = "Screen Captured";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit switch settings";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case '1':
                    space.save_ild_fmt_4_5 = !space.save_ild_fmt_4_5;
                    if(space.save_ild_fmt_4_5)
                        space.TUI_clue = "ild 4,5";
                    else
                        space.TUI_clue = "ild 0,1,2,(!3)";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.save_2D_as_3D = !space.save_2D_as_3D;
                    if(space.save_2D_as_3D)
                        space.TUI_clue = "ild save 2D as 3D";
                    else
                        space.TUI_clue = "ild save 2D as 2D";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.auto_scale_dxf = !space.auto_scale_dxf;
                    if(space.auto_scale_dxf)
                        space.TUI_clue = "scale dxf import";
                    else
                        space.TUI_clue = "no scale dxf import";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.save_true_color_dxf = !space.save_true_color_dxf;
                    if(space.save_true_color_dxf)
                        space.TUI_clue = "dxf 24-bit";
                    else
                        space.TUI_clue = "dxf palette";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.save_txt_unit = !space.save_txt_unit;
                    if(space.save_txt_unit)
                        space.TUI_clue = "save txt unit";
                    else
                        space.TUI_clue = "save txt short";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.save_txt_with_color = !space.save_txt_with_color;
                    if(space.save_txt_with_color)
                        space.TUI_clue = "save txt rgb";
                    else
                        space.TUI_clue = "save txt palette & tables";
                    break;
                //------------------------------------------------------------
                case '7':
                    space.save_txt_color_hex = !space.save_txt_color_hex;
                    if(space.save_txt_color_hex)
                        space.TUI_clue = "save txt hex";
                    else
                        space.TUI_clue = "save txt r g b";
                    break;
                //------------------------------------------------------------
                case '8':
                    space.save_txt_named_palettes = !space.save_txt_named_palettes;
                    if(space.save_txt_named_palettes)
                        space.TUI_clue = "save named palettes";
                    else
                        space.TUI_clue = "assume named palettes";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case ' ': // [Space] select, deselect current frame
                    space.current_frame().is_selected = !(space.current_frame().is_selected);
                    if(space.current_frame().is_selected)
                        space.TUI_clue = "select frame";
                    else
                        space.TUI_clue = "deselect frame";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::apply_view_menu()
{
    bool in_menu = true;
    space.simplify_view_angle();
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_view_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit view";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'q':
                    space.view_offset.x += space.displacement_step;
                    space.TUI_clue = "+ pan X";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'Q':
                    space.view_offset.x -= space.displacement_step;
                    space.TUI_clue = "- pan X";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'w':
                    space.view_offset.y += space.displacement_step;
                    space.TUI_clue = "+ pan Y";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'W':
                    space.view_offset.y -= space.displacement_step;
                    space.TUI_clue = "- pan Y";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'e':
                    space.view_offset += space.displacement_step;
                    space.TUI_clue = "+ pan XY";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'E':
                    space.view_offset -= space.displacement_step;
                    space.TUI_clue = "- pan XY";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    space.view_offset = 0;
                    space.TUI_clue = "on center";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.view_scale.x *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom X";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.view_scale.x *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom X";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 's':
                    space.view_scale.y *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom Y";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'S':
                    space.view_scale.y *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom Y";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'd':
                    space.view_scale *= space.magnitude_step_up;
                    space.TUI_clue = "+ zoom XY";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.view_scale *= space.magnitude_step_dn;
                    space.TUI_clue = "- zoom XY";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.view_scale = 1.0;
                    space.TUI_clue = "no zoom";
                    space.view_has_changed = true;
                    break;
                //------------------------------------------------------------
                case 'v':
                    if(!space.current_frame().apply_view())
                    {
                        space.view_angle       = 0.0;
                        space.view_offset      = 0;
                        space.view_scale       = 1.0;
                        space.TUI_clue         = "set view frame";
                        space.view_has_changed = true;
                    }
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'V':
                    space.TUI_clue = "setting all";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_frame_set current(space);
                        if(!space.apply_view())
                        {
                            space.view_angle       = 0.0;
                            space.view_offset      = 0;
                            space.view_scale       = 1.0;
                            space.TUI_clue         = "set view frame";
                            space.view_has_changed = true;
                        }
                        else
                        {
                            space = current;
                            space.display_error("out of bounds!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
bool LaserBoy_FLTK_GUI::frame_color_trans_menu()
{
    bool    in_menu                 = true,
            show_menu_was           = space.show_menu,
            show_palette_was        = space.show_palette,
            show_target_palette_was = space.show_target_palette;
    int     temp_int;
    u_int   temp_u_int;
    double  temp_double;
    space.show_menu                 = true;
    space.show_palette              = true;
    space.show_target_palette       = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_frame_color_tranz_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu                   = false;
                    space.show_color_cursor   = false;
                    space.show_menu           = show_menu_was;
                    space.show_palette        = show_palette_was;
                    space.show_target_palette = show_target_palette_was;
                    space.TUI_clue            = "exit palette transforms";
                    return false;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 't': // toggle menu mode
                case 'T':
                    space.alt_menu_c = !space.alt_menu_c;
                    if(space.alt_menu_c)
                        space.TUI_clue = "alt menu";
                    else
                        space.TUI_clue = "draw menu";
                    break;
                //------------------------------------------------------------
                // palette transforms (to frame)
                //------------------------------------------------------------
                case 'a':
                    if(space.alt_menu_c)
                        space.current_frame().span_x_hues(space.recolor_span_factor, 0);
                    else
                        space.current_frame().span_x_palette(space.recolor_span_factor, 0);
                    space.TUI_clue = "span X frame";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.TUI_clue = "span X all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_x_hues();
                    else
                        space.span_x_palette();
                    space.TUI_clue = "span X done";
                    break;
                //------------------------------------------------------------
                case 'b':
                    if(space.alt_menu_c)
                        space.current_frame().span_y_hues(space.recolor_span_factor, 0);
                    else
                        space.current_frame().span_y_palette(space.recolor_span_factor, 0);
                    space.TUI_clue = "span Y frame";
                    break;
                //------------------------------------------------------------
                case 'B':
                    space.TUI_clue = "span Y all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_y_hues();
                    else
                        space.span_y_palette();
                    space.TUI_clue = "span Y done";
                    break;
                //------------------------------------------------------------
                case 'c':
                    if(space.alt_menu_c)
                        space.current_frame().span_z_hues(space.recolor_span_factor, 0);
                    else
                        space.current_frame().span_z_palette(space.recolor_span_factor, 0);
                    space.TUI_clue = "span Z frame";
                    break;
                //------------------------------------------------------------
                case 'C':
                    space.TUI_clue = "span Z all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_z_hues();
                    else
                        space.span_z_palette();
                    space.TUI_clue = "span Z done";
                    break;
                //------------------------------------------------------------
                case 'd':
                    if(space.alt_menu_c)
                        space.current_frame().span_radial_hues(space.recolor_span_factor, space.frame_index);
                    else
                        space.current_frame().span_radial_palette(space.recolor_span_factor, space.frame_index);
                    space.TUI_clue = "span radially from origin";
                    break;
                //------------------------------------------------------------
                case 'D':
                    space.TUI_clue = "span radially from origin";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_radial_hues();
                    else
                        space.span_radial_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'e':
                    if(space.alt_menu_c)
                        space.current_frame().span_axial_hues(space.recolor_span_factor, space.frame_index);
                    else
                        space.current_frame().span_axial_palette(space.recolor_span_factor, space.frame_index);
                    space.TUI_clue = "span angular around origin";
                    break;
                //------------------------------------------------------------
                case 'E':
                    space.TUI_clue = "span angular around origin";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_axial_hues();
                    else
                        space.span_axial_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(space.alt_menu_c)
                        space.current_frame().span_hues(space.recolor_span_factor, space.frame_index);
                    else
                        space.current_frame().span_palette(space.recolor_span_factor, space.frame_index);
                    space.TUI_clue = "span vertices frame";
                    break;
                //------------------------------------------------------------
                case 'F':
                    space.TUI_clue = "span vertices all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_hues();
                    else
                        space.span_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(space.alt_menu_c)
                        space.current_frame().index_hues(space.recolor_index_multiple, space.frame_index);
                    else
                        space.current_frame().index_palette(space.recolor_index_multiple, space.frame_index);
                    space.TUI_clue = "index vertices frame";
                    break;
                //------------------------------------------------------------
                case 'G':
                    space.TUI_clue = "index vertices all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.index_hues();
                    else
                        space.index_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'h':
                    if(space.alt_menu_c)
                        space.current_frame().span_segments_hues(space.recolor_span_factor, space.frame_index);
                    else
                        space.current_frame().span_segments_palette(space.recolor_span_factor, space.frame_index);
                    space.TUI_clue = "span segments frame";
                    break;
                //------------------------------------------------------------
                case 'H':
                    if(space.alt_menu_c)
                    space.TUI_clue = "span segments all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.span_segments_hues();
                    else
                        space.span_segments_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'i':
                    if(space.alt_menu_c)
                        space.current_frame().index_segments_hues(space.recolor_index_multiple, space.frame_index);
                    else
                        space.current_frame().index_segments_palette(space.recolor_index_multiple, space.frame_index);
                    space.TUI_clue = "index segments frame";
                    break;
                //------------------------------------------------------------
                case 'I':
                    space.TUI_clue = "index segments all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.index_segments_hues();
                    else
                        space.index_segments_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'j':
                    if(space.alt_menu_c)
                        space.current_frame().random_color_vectors_hues(space.recolor_index_multiple);
                    else
                        space.current_frame().random_color_vectors_palette(space.recolor_index_multiple);
                    space.TUI_clue = "random vertices frame";
                    break;
                //------------------------------------------------------------
                case 'J':
                    space.TUI_clue = "random vertices all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.random_color_vectors_hues();
                    else
                        space.random_color_vectors_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'k':
                    if(space.alt_menu_c)
                        space.current_frame().random_color_segments_hues();
                    else
                        space.current_frame().random_color_segments_palette();
                    space.TUI_clue = "random segments frame";
                    break;
                //------------------------------------------------------------
                case 'K':
                    space.TUI_clue = "random segments all";
                    space.render_space();
                    display_space();
                    if(space.alt_menu_c)
                        space.random_color_segments_hues();
                    else
                        space.random_color_segments_palette();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                //------------------------------------------------------------
                case 'l':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().best_match_target_palette();
                        space.TUI_clue = "best match frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'L':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "best match all";
                        space.render_space();
                        display_space();
                        space.best_match_target_palette();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().to_target_palette_by_index();
                        space.TUI_clue = "by index frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "by index all";
                        space.render_space();
                        display_space();
                        space.to_target_palette_by_index();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'n':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().strip_color();
                        space.TUI_clue = "strip color frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'N':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "strip color all";
                        space.render_space();
                        display_space();
                        space.strip_color();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'o':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().strip_color_or();
                        space.TUI_clue = "strip color frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'O':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "strip color all";
                        space.render_space();
                        display_space();
                        space.strip_color_or();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'p':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().strip_color_avg();
                        space.TUI_clue = "strip color frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "strip color all";
                        space.render_space();
                        display_space();
                        space.strip_color_avg();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'q':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().promote_to_true_color();
                        space.TUI_clue = "promote to 24bit frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'Q':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "promote to 24bit all";
                        space.render_space();
                        display_space();
                        space.promote_to_true_color();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'r':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().sync_rgb_and_palette();
                        space.TUI_clue = "24-bit to palette frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'R':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "24-bit to palette all";
                        space.render_space();
                        display_space();
                        space.minimize_tables_and_palettes();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 's':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().bit_reduce_to_palette();
                        space.TUI_clue = "888 to 332 frame";
                    }
                    break;
                //------------------------------------------------------------
                case 'S':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "888 to 332 all";
                        space.render_space();
                        display_space();
                        space.bit_reduce_to_palette();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'v':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.current_frame().best_reduce_to_palette();
                        space.TUI_clue = "best reduce to minimum palette";
                    }
                    break;
                //------------------------------------------------------------
                case 'V':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "best reduce to minimum palette";
                        space.render_space();
                        display_space();
                        space.best_reduce_to_palette();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    if(space.alt_menu_c)
                        space.TUI_clue = "bad key";
                    else
                    {
                        space.TUI_clue = "best reduce all to 1";
                        space.render_space();
                        display_space();
                        space.best_reduce_all_to_palette();
                        space.TUI_clue = "done";
                    }
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case '.':
                    if(space.palette_index >= 0)
                    {
                        space.target_palette_index = space.palette_index;
                        space.TUI_clue = "target to current";
                    }
                    else
                        space.TUI_clue = "current is -24-bit-";
                    break;
                //------------------------------------------------------------
                case ',':
                    {
                        u_int i;
                        for(i = 0; i < space.number_of_frames(); i++)
                            if(space.frame_picker(i).palette_index == (int)space.target_palette_index)
                            {
                                space.frame_index = i;
                                break;
                            }
                        if(i == space.number_of_frames())
                        {
                            space.display_message("no frame uses this palette.");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case '=':
                    if(space.alt_menu_c)
                        temp_double = display_prompt_and_echo_double(   "recolor hues span factor"
                                                                      , space.recolor_span_factor
                                                                      , 1530.0
                                                                      , 0.0
                                                                    );
                    else
                        temp_double = display_prompt_and_echo_double(   "recolor palette span factor"
                                                                      , space.recolor_span_factor
                                                                      , 255.0
                                                                      , 0.0
                                                                    );
                    if(prompt_escaped)
                        break;
                    space.recolor_span_factor = temp_double;
                    break;
                //------------------------------------------------------------
                case '+':
                    if(space.alt_menu_c)
                        temp_u_int = display_prompt_and_echo_u_int( "recolor hues index multiple"
                                                                    , space.recolor_index_multiple
                                                                    , 65535
                                                                  );
                    else
                        temp_u_int = display_prompt_and_echo_u_int( "recolor palette index multiple"
                                                                    , space.recolor_index_multiple
                                                                    , 65535
                                                                  );
                    if(prompt_escaped)
                        break;
                    space.recolor_index_multiple = temp_u_int;
                    break;
                //------------------------------------------------------------
                case '-':
                    if(space.alt_menu_c)
                        temp_int = display_prompt_and_echo_u_int( "recolor hues index frame shift"
                                                                  , space.color_frame_shift
                                                                  , 1530
                                                                );
                    else
                        temp_int = display_prompt_and_echo_u_int( "recolor palette index frame shift"
                                                                  , space.color_frame_shift
                                                                  , 255
                                                                );
                    if(prompt_escaped)
                        break;
                    space.color_frame_shift = temp_int;
                    break;
                //------------------------------------------------------------
                case '<':
                    space.current_frame().convert_black_to_color();
                    space.TUI_clue = "black to color";
                    break;
                //------------------------------------------------------------
                case '>':
                    space.convert_black_to_color();
                    space.TUI_clue = "black to color all";
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            }
        }
    }
    space.show_menu = show_menu_was;
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::palette_set_trans_menu()
{
    bool  in_menu                 = true,
          show_menu_was           = space.show_menu,
          show_target_palette_was = space.show_target_palette;
    int   ps_was                  = 0;
    space.show_menu               = true;
    space.show_target_palette     = true;
    space.show_color_cursor       = true;
    prompt_escaped                = false;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_palette_set_tranz_menu();
        display_space();
        ps_was = space.number_of_palettes();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu                   = false;
                    space.show_color_cursor   = false;
                    space.show_menu           = show_menu_was;
                    space.show_target_palette = show_target_palette_was;
                    space.TUI_clue            = "exit palette transforms";
                    return false;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                // palette transforms (to palettes and set)
                case 'a': // import from txt
                case 'A':
                    { // scope
                        int current_palette_count = space.number_of_palettes(),
                            new_palette_count;
                        space.clean_screen();
                        space.file_name.clear();
                        space.get_file_list(LASERBOY_TXT_SHARE, ".txt");
                        space.overlay_file_list();
                        display_space();
                        while(!space.file_selected())
                        {
                            display_prompt_file_with_auto_complete("txt palette file name");
                            if(prompt_escaped)
                                break;
                        }
                        if(space.file_name.size() == 0) // [esc]
                            break;
                        if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                            space.file_name += ".txt";
                        space.TUI_clue = "loading " + space.file_name;
                        display_space();
                        space.palettes_from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                        new_palette_count = space.number_of_palettes() - current_palette_count;
                        if(space.palette_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                        {
                            space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                            wait_4_Esc();
                            return false;
                        }
                        else
                        {
                            if(space.palette_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                            {
                                space.display_error(   LASERBOY_TXT_SHARE
                                               + space.file_name
                                               + " format error (see txt_in_errors.txt)"
                                             );
                                wait_4_Esc();
                            }
                            if(space.palette_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                            {
                                space.display_error(   LASERBOY_TXT_SHARE
                                               + space.file_name
                                               + " palette name error (see txt_in_errors.txt)"
                                             );
                                wait_4_Esc();
                            }
                            if(space.palette_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                            {
                                space.display_error(   LASERBOY_TXT_SHARE
                                               + space.file_name
                                               + " palette name in use (see txt_in_errors.txt)"
                                             );
                                wait_4_Esc();
                            }
                            if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                            {
                                space.display_error(   LASERBOY_TXT_SHARE
                                               + space.file_name
                                               + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                             );
                                wait_4_Esc();
                            }
                            if(new_palette_count)
                            {
                                stringstream strings;
                                strings << new_palette_count
                                        << " palette"
                                        << (   (new_palette_count == 1)
                                             ? (" ")
                                             : ("s ")
                                           );
                                strings << "found in "
                                        << LASERBOY_TXT_SHARE
                                        << space.file_name;
                                space.display_message(strings.str());
                                space.target_palette_index = space.number_of_palettes() - 1;
                                wait_4_any_key();
                            }
                        } // end else (file opened)
                        space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                        break;
                    }
                //------------------------------------------------------------
                case 'b': // import from bmp
                case 'B':
                    space.clean_screen();
                    space.file_name.clear();
                    space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                    space.overlay_file_list();
                    display_space();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("bitmap file name");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".bmp";
                    space.TUI_clue = "loading " + space.file_name;
                    display_space();
                    switch(space.copy_palette_from_bmp(LASERBOY_BMP_SHARE + space.file_name, space.file_name.substr(0, space.file_name.size() - 4)))
                    {
                        default:
                        case LASERBOY_FILE_OPEN_FAILED:
                            space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                            wait_4_Esc();
                            break;
                        case LASERBOY_PALETTE_ALREADY_DEFINED:
                            space.display_error(LASERBOY_BMP_SHARE + space.file_name + " has already been loaded into the palette set");
                            wait_4_Esc();
                            break;
                        case LASERBOY_NO_PALETTES_FOUND:
                            space.display_error(LASERBOY_BMP_SHARE + space.file_name + " is not a palette bmp");
                            wait_4_Esc();
                            break;
                        case LASERBOY_OK:
                            space.TUI_clue = "bmp palette" + space.file_name;
                            break;
                    }
                    break;
                //------------------------------------------------------------
                case 'c': // export to txt
                case 'C':
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.get_file_list(LASERBOY_TXT_SHARE, ".txt");
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new palette file name [use palette name]", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        space.file_name = space.target_palette().name + ".txt";
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".txt";
                    space.TUI_clue = "saving txt";
                    if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                        && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!space.palette_picker(space.target_palette_index).save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                        {
                            space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.TUI_clue = "txt palette saved";
                    break;
                //------------------------------------------------------------
                case 'd': // export to txt all added palettes
                case 'D':
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.get_file_list(LASERBOY_TXT_SHARE, ".txt");
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new palette file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".txt";
                    space.TUI_clue = "saving txt";
                    if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                        && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        switch(space.save_new_palettes_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                        {
                            default:
                            case LASERBOY_FILE_OPEN_FAILED:
                                space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                                wait_4_Esc();
                                break;
                            case LASERBOY_NO_PALETTES_FOUND:
                                space.display_error(LASERBOY_TXT_SHARE + space.file_name + " no added palettes to save");
                                wait_4_Esc();
                                break;
                            case LASERBOY_OK:
                                space.TUI_clue = "txt palette saved";
                                break;
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    space.TUI_clue = "clear unused palettes";
                    space.render_space();
                    display_space();
                    space.clear_unused_palettes();
                    space.TUI_clue = "done";
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    if(space.target_palette_index <  LASERBOY_BUILT_IN_PALETTES)
                    {
                        space.display_error("built in");
                        wait_4_Esc();
                    }
                    else if(!space.omit_palette(space.target_palette_index))
                    {
                        space.display_error("in use");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    space.add_palette();
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    space.copy_palette(space.target_palette_index);
                    if(ps_was == space.number_of_palettes())
                    {
                        space.display_error("palette has no colors!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'i':
                case 'I':
                    space.copy_palette_reverse(space.target_palette_index);
                    if(ps_was == space.number_of_palettes())
                    {
                        space.display_error("palette has no colors!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'j':
                case 'J':
                    space.copy_palette_reorder(space.target_palette_index);
                    if(ps_was == space.number_of_palettes())
                    {
                        space.display_error("palette has no colors!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'k':
                case 'K':
                    space.copy_palette_negative(space.target_palette_index);
                    if(ps_was == space.number_of_palettes())
                    {
                        space.display_error("palette has no colors!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'l':
                case 'L':
                    space.copy_palette_expanded(space.target_palette_index);
                    if(ps_was == space.number_of_palettes())
                    {
                        space.display_error("palette has no colors!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'm':
                case 'M':
                    add_color_to_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 'n':
                case 'N':
                    edit_color_in_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 'o':
                case 'O':
                    insert_color_in_target_palette_menu();
                    break;
                //------------------------------------------------------------
                case 'q':
                case 'Q':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        space.add_palette();
                        space.palette_picker(space.number_of_palettes() - 1) = space.target_palette();
                        space.target_palette_index = space.number_of_palettes() - 1;
                        space.target_palette().name = GUID8char();
                        space.target_palette().erase_index(space.selected_color_index);
                    }
                    else
                        space.target_palette().erase_index(space.selected_color_index);
                    space.sync_rgb_and_palette();
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
                    {
                        int temp = space.target_palette_index;
                        space.add_palette();
                        string name = space.target_palette().name;
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = name;
                        space.target_palette().clear_from_index(space.selected_color_index);
                    }
                    else
                        space.target_palette().clear_from_index(space.selected_color_index);
                    space.sync_rgb_and_palette();
                    break;
                //------------------------------------------------------------
                case 's':
                case 'S':
                    if(space.target_palette_index >= LASERBOY_BUILT_IN_PALETTES)
                    {
                        string name;
                        do
                        {
                            name = display_prompt_and_echo_name("(unique) new target palette name", 8);
                            if(name.size() == 0)
                                break;
                            name += "________";
                            name = name.substr(0, 8);
                        } while(space.is_palette_name(name));
                        if(prompt_escaped)
                            break;
                        if(name.size() == 0)
                            break;
                        space.target_palette().name = name;
                    }
                    else
                    {
                        space.display_message("only added palettes can be renamed.");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case 't':
                case 'T':
                    {
                        if(space.target_palette().number_of_colors() > 7)
                            if(!display_prompt_and_echo_bool("target palette has more than 7 colors defined. bend anyway ? : y | [any]"))
                                break;
                        int temp = space.target_palette_index;
                        space.add_palette();
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = GUID8char();
                        space.target_palette().straight_blend();
                    }
                    break;
                //------------------------------------------------------------
                case 'u':
                case 'U':
                    {
                        if(space.target_palette().number_of_colors() > 7)
                            if(!display_prompt_and_echo_bool("target palette has more than 7 colors defined. bend anyway ? : y | [any]"))
                                break;
                        int temp = space.target_palette_index;
                        space.add_palette();
                        space.target_palette() = space.palette_picker(temp);
                        space.target_palette().name = GUID8char();
                        space.target_palette().circular_blend();
                    }
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.selected_color_index >= space.greater_number_of_colors())
                        space.selected_color_index = 0;
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.selected_color_index == 0)
                        space.selected_color_index = space.greater_number_of_colors();
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case '.': // set target to current
                    if(space.palette_index >= 0)
                    {
                        space.target_palette_index = space.palette_index;
                        space.TUI_clue = "target to current";
                    }
                    else
                        space.TUI_clue = "current is -24-bit-";
                    break;
                //------------------------------------------------------------
                case ',':
                    {
                        u_int i;
                        for(i = 0; i < space.number_of_frames(); i++)
                            if(space.frame_picker(i).palette_index == (int)space.target_palette_index)
                            {
                                space.frame_index = i;
                                break;
                            }
                        if(i == space.number_of_frames())
                        {
                            space.display_message("no frame uses this palette.");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            }
        }
    }
    space.show_menu = show_menu_was;
    return true;
}

//############################################################################
void LaserBoy_FLTK_GUI::add_color_to_target_palette_menu()
{
    prompt_escaped = false;
    if(space.target_palette().number_of_colors() >= 255)
    {
        space.display_error("palette is full");
        wait_4_Esc();
        return;
    }
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
    }
    space.target_palette().add_color(LaserBoy_color(r, g, b));
    space.TUI_clue = "new color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::insert_color_in_target_palette_menu()
{
    prompt_escaped = false;
    if(space.target_palette().number_of_colors() >= 255)
    {
        space.display_error("palette is full");
        wait_4_Esc();
        return;
    }
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)", 0);
    if(prompt_escaped)
        return;
    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
    }
    space.target_palette().insert_color(LaserBoy_color(r, g, b), space.selected_color_index);
    int frame_index = space.frame_index;
    space.sync_rgb_and_palette();
    space.frame_index = frame_index;
    space.TUI_clue = "new color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::edit_color_in_target_palette_menu()
{
    prompt_escaped = false;
    if(space.selected_color_index > space.target_palette().number_of_colors())
    {
        space.display_error("color index out of range");
        wait_4_Esc();
        return;
    }
    space.clean_screen();
    space.render_space();
    u_char r = display_prompt_and_echo_u_char("red (0-255)");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char g = display_prompt_and_echo_u_char("green (0-255)");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_char b = display_prompt_and_echo_u_char("blue (0-255)");
    if(prompt_escaped)
        return;
    if(space.target_palette_index < LASERBOY_BUILT_IN_PALETTES)
    {
        int temp = space.target_palette_index;
        space.add_palette();
        string name = space.target_palette().name;
        space.target_palette() = space.palette_picker(temp);
        space.target_palette().name = name;
        space.target_palette().set_index_color(space.selected_color_index, r, g, b);
    }
    else
        space.target_palette().set_index_color(space.selected_color_index, r, g, b);
    space.sync_frames_to_target();
    space.TUI_clue = "edit color";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::frame_transforms_menu()
{
    bool in_menu                  = true,
         show_vertices_was        = space.show_vertices,
         show_blanking_was        = space.show_blanking,
         show_menu_was            = space.show_menu,
         maintain_real_origin_was = space.maintain_real_origin;
    space.show_vertices           = true;
    space.show_blanking           = true;
    space.show_menu               = true;
    prompt_escaped                = false;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_frame_tranz_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu                    = false;
                    space.show_vertices        = show_vertices_was;
                    space.show_blanking        = show_blanking_was;
                    space.maintain_real_origin = maintain_real_origin_was;
                    space.TUI_clue = "exit frame transforms";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 't': // toggle menu mode
                    space.alt_menu_h = !space.alt_menu_h;
                    if(space.alt_menu_h)
                        space.TUI_clue = "alt menu";
                    else
                        space.TUI_clue = "draw menu";
                    break;
                //------------------------------------------------------------
                case 'T': // toggle
                    space.maintain_real_origin = !space.maintain_real_origin;
                    if(space.maintain_real_origin)
                        space.TUI_clue = "to orgin";
                    else
                        space.TUI_clue = "to itself";
                    break;
                //------------------------------------------------------------
                case 'a':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().flatten_z();
                        space.TUI_clue = "flatten Z";
                    }
                    else
                    {
                        space.current_frame().reduce_blank_vectors();
                        space.TUI_clue = "reduced blank span";
                    }
                    break;
                //------------------------------------------------------------
                case 'A':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "flattening Z";
                        space.render_space();
                        display_space();
                        space.flatten_z();
                        space.TUI_clue = "flatten Z all";
                    }
                    else
                    {
                        space.TUI_clue = "reduce blank vectors";
                        space.render_space();
                        display_space();
                        space.reduce_blank_vectors();
                        space.TUI_clue = "reduced blank span all";
                    }
                    break;
                //------------------------------------------------------------
                case 'b':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().z_order_vertices(65535);
                        space.TUI_clue = "z order vertices";
                    }
                    else
                    {
                        space.current_frame().remove_dwell_vertices();
                        space.TUI_clue = "removed angle dwell";
                    }
                    break;
                //------------------------------------------------------------
                case 'B':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "Z ordering vertices";
                        space.render_space();
                        display_space();
                        space.z_order_vertices(64000);
                        space.TUI_clue = "Z order vertices all";
                    }
                    else
                    {
                        space.TUI_clue = "removing dwell vertices";
                        space.render_space();
                        display_space();
                        space.remove_dwell_vertices();
                        space.TUI_clue = "removed angle dwell all";
                    }
                    break;
                //------------------------------------------------------------
                case 'c':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().flip(0);
                        space.TUI_clue = "mirror";
                    }
                    else
                    {
                        space.current_frame().remove_dots();
                        space.TUI_clue = "removed dots";
                    }
                    break;
                //------------------------------------------------------------
                case 'C':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "mirroring";
                        space.render_space();
                        display_space();
                        space.flip(0);
                        space.TUI_clue = "mirror all";
                    }
                    else
                    {
                        space.TUI_clue = "removing dots";
                        space.render_space();
                        display_space();
                        space.remove_dots();
                        space.TUI_clue = "removed dots all";
                    }
                    break;
                //------------------------------------------------------------
                case 'd':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().flip(1);
                        space.TUI_clue = "flip";
                    }
                    else
                    {
                        space.current_frame().remove_short_vectors();
                        space.TUI_clue = "removed short vectors";
                    }
                    break;
                //------------------------------------------------------------
                case 'D':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "flipping";
                        space.render_space();
                        display_space();
                        space.flip(1);
                        space.TUI_clue = "flip all";
                    }
                    else
                    {
                        space.TUI_clue = "removing short vectors";
                        space.render_space();
                        display_space();
                        space.remove_short_vectors();
                        space.TUI_clue = "removed short vectors all";
                    }
                    break;
                //------------------------------------------------------------
                case 'e':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().quarter_turn(0, 3);
                        space.TUI_clue = "quarter turn CCW";
                    }
                    else
                    {
                        space.current_frame().reduce_lit_vectors();
                        space.TUI_clue = "reduced vectors";
                    }
                    break;
                //------------------------------------------------------------
                case 'E':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "quarter turning CCW";
                        space.render_space();
                        display_space();
                        space.quarter_turn(0, 3);
                        space.TUI_clue = "quarter turn CCW all";
                    }
                    else
                    {
                        space.TUI_clue = "reducing vectors";
                        space.render_space();
                        display_space();
                        space.reduce_lit_vectors();
                        space.TUI_clue = "reduced vectors all";
                    }
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().flat_to_2D();
                        space.TUI_clue = "flat 3D to 2D";
                    }
                    else
                    {
                        space.current_frame().omit_equivalent_vectors();
                        space.TUI_clue = "removed equal vectors";
                    }
                    break;
                //------------------------------------------------------------
                case 'F':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "flattening 3D to 2D all";
                        space.render_space();
                        display_space();
                        space.flat_to_2D();
                        space.TUI_clue = "flat 3D to 2D all";
                    }
                    else
                    {
                        space.TUI_clue = "removing equal vectors";
                        space.render_space();
                        display_space();
                        space.omit_equivalent_vectors();
                        space.TUI_clue = "removed equal vectors";
                    }
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "normalize to %";
                        space.render_space();
                        display_space();
                        double scale = display_prompt_and_echo_double(   "normalize frame to %% of space ? [100%%]"
                                                                       , 100.0
                                                                       , 100.0
                                                                       , 0.0
                                                                     );
                        if(prompt_escaped)
                            break;
                        space.TUI_clue = "normalizing vector size";
                        space.current_frame().normalize(scale / 100.0);
                        if(scale == 0.0)
                            scale = 100.0;
                    }
                    else
                    {
                        space.current_frame().conglomerate_lit_segments();
                        space.TUI_clue = "conglomerate segments";
                    }
                    break;
                //------------------------------------------------------------
                case 'G':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "normalize to %";
                        space.render_space();
                        double scale = display_prompt_and_echo_double(   "normalize all frames to %% of space ? [100%%]"
                                                                       , 100.0
                                                                       , 100.0
                                                                       , 0.0
                                                                     );
                        if(prompt_escaped)
                            break;
                        if(scale == 0.0)
                            scale = 100.0;
                        if(display_prompt_and_echo_bool("each frame individually? (y) or all frames together? [any]"))
                            for(size_t i = 0; i < space.number_of_frames(); i++)
                                space.frame(i).normalize(scale / 100.0);
                        else
                            space.normalize(scale / 100.0);
                        space.TUI_clue = "normalize vectors all";
                    }
                    else
                    {
                        space.TUI_clue = "conglomerate segments";
                        space.render_space();
                        display_space();
                        space.conglomerate_lit_segments();
                        space.TUI_clue = "conglomerated segments all";
                    }
                    break;
                //------------------------------------------------------------
                case 'h':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "normalize to size";
                        space.render_space();
                        display_space();
                        {
                            u_int dimension = display_prompt_and_echo_u_int(   "normalize frame to ? [50000]"
                                                                              , 50000
                                                                              , 65535
                                                                            );
                            if(prompt_escaped)
                                break;
                            space.current_frame().normalize_vectors((double)dimension / 65535.0);
                            space.TUI_clue = "normalize vector size";
                        }
                    }
                    else
                    {
                        space.current_frame().bond_segments();
                        space.TUI_clue = "bonded segments";
                    }
                    break;
                //------------------------------------------------------------
                case 'H':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "normalize to size";
                        space.render_space();
                        {
                            u_int dimension = display_prompt_and_echo_u_int(   "normalize frames to ? [50000]"
                                                                              , 50000
                                                                              , 65535
                                                                            );
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("each frame individually? (y) or all frames together? [any]"))
                                for(size_t i = 0; i < space.number_of_frames(); i++)
                                    space.frame(i).normalize_vectors((double)dimension / 65535.0);
                            else
                                space.normalize_vectors((double)dimension / 65535.0);
                            space.TUI_clue = "normalize vectors all";
                        }
                    }
                    else
                    {
                        space.TUI_clue = "bonding segments";
                        space.render_space();
                        display_space();
                        space.bond_segments();
                        space.TUI_clue = "bonded segments all";
                    }
                    break;
                //------------------------------------------------------------
                case 'i':
                    if(space.alt_menu_h)
                    {
                        if(display_prompt_and_echo_bool("center in all X|Y|Z ?"))
                        {
                            space.current_frame().center();
                            space.clean_screen();
                            space.render_space();
                            display_space();
                        }
                        else
                        {
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("center in X     ?"))
                            {
                                space.current_frame().center_x();
                                space.clean_screen();
                                space.render_space();
                                display_space();
                            }
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("center in   Y   ?"))
                            {
                                space.current_frame().center_y();
                                space.clean_screen();
                                space.render_space();
                                display_space();
                            }
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("center in     Z ?"))
                            {
                                space.current_frame().center_z();
                                space.clean_screen();
                                space.render_space();
                                display_space();
                            }
                        }
                        space.TUI_clue = "center vectors";
                    } // end if(space.alt_menu_h)
                    else
                    {
                        space.current_frame().minimize(0);
                        space.TUI_clue = "minimized vectors";
                    }
                    break;
                //------------------------------------------------------------
                case 'I':
                    if(space.alt_menu_h)
                    {
                        size_t i;
                        if(display_prompt_and_echo_bool("each frame individually (y) or all frames together [any]"))
                        {
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("center in all X|Y|Z ?"))
                            {
                                for(i = 0; i < space.number_of_frames(); i++)
                                    space.frame(i).center();
                                space.clean_screen();
                                space.render_space();
                                display_space();
                            }
                            else
                            {
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in X     ?"))
                                {
                                    for(i = 0; i < space.number_of_frames(); i++)
                                        space.frame(i).center_x();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in   Y   ?"))
                                {
                                    for(i = 0; i < space.number_of_frames(); i++)
                                        space.frame(i).center_y();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in     Z ?"))
                                {
                                    for(i = 0; i < space.number_of_frames(); i++)
                                        space.frame(i).center_z();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                            }
                        }
                        else // if not individually
                        {
                            if(prompt_escaped)
                                break;
                            if(display_prompt_and_echo_bool("center in all X|Y|Z ?"))
                            {
                                space.center();
                                space.clean_screen();
                                space.render_space();
                                display_space();
                            }
                            else
                            {
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in X     ?"))
                                {
                                    space.center_x();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in   Y   ?"))
                                {
                                    space.center_y();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                                if(prompt_escaped)
                                    break;
                                if(display_prompt_and_echo_bool("center in     Z ?"))
                                {
                                    space.center_z();
                                    space.clean_screen();
                                    space.render_space();
                                    display_space();
                                }
                            }
                        } // end if(display_prompt_and_echo_bool("each frame individually (y) or all frames together [any]"))
                        space.TUI_clue = "center all";
                    } // end if(space.alt_menu_h)
                    else
                    {
                        space.TUI_clue = "minimizing vectors";
                        space.minimize();
                        space.clean_screen();
                        space.render_space();
                        display_space();
                        space.TUI_clue = "minimized vectors all";
                    }
                    break;
                //------------------------------------------------------------
                case 'j':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().reverse();
                        space.TUI_clue = "reverse vectors";
                    }
                    else
                    {
                        if(space.current_frame().fracture_segments())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "fracture segments fail";
                        }
                        else
                            space.TUI_clue = "fractured segments";
                    }
                    break;
                //------------------------------------------------------------
                case 'J':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "reversing vector order";
                        space.render_space();
                        display_space();
                        space.frame_reverse();
                        space.TUI_clue = "reverse vector order all";
                    }
                    else
                    {
                        space.TUI_clue = "fracturing segments";
                        space.render_space();
                        display_space();
                        space.fracture_segments();
                        space.TUI_clue = "fractured segments all";
                    }
                    break;
                //------------------------------------------------------------
                case 'k':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().unblank_all_vertices();
                        space.TUI_clue = "unblank all";
                    }
                    else
                    {
                        space.current_frame().randomize_segments();
                        space.TUI_clue = "randomize segments";
                    }
                    break;
                //------------------------------------------------------------
                case 'K':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "unblank all";
                        space.render_space();
                        display_space();
                        space.unblank_all_vertices();
                        space.TUI_clue = "unblank all";
                    }
                    else
                    {
                        space.TUI_clue = "randomizing segments";
                        space.render_space();
                        display_space();
                        space.randomize_segments();
                        space.TUI_clue = "randomized segments all";
                    }
                    break;
                //------------------------------------------------------------
                case 'l':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().convert_black_to_color();
                        space.TUI_clue = "black to color";
                    }
                    else
                    {
                        LaserBoy_segment origin;
                        origin.push_back(LaserBoy_vertex());
                        origin.push_back(LaserBoy_vertex());
                        if(space.current_frame().reorder_segments(origin))
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "reorder segments fail";
                        }
                        else
                            space.TUI_clue = "reordered segments";
                    }
                    break;
                //------------------------------------------------------------
                case 'L':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "black to color";
                        space.render_space();
                        display_space();
                        space.convert_black_to_color();
                        space.TUI_clue = "black to color all";
                    }
                    else
                    {
                        space.TUI_clue = "reordering segments";
                        space.render_space();
                        display_space();
                        if(space.reorder_segments())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "reorder segments fail";
                        }
                        else
                            space.TUI_clue = "reordered segments all";
                    }
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().convert_black_to_blank();
                        space.TUI_clue = "black to blank";
                    }
                    else
                    {
                        if(space.current_frame().add_dwell())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "angle dwell fail";
                        }
                        else
                            space.TUI_clue = "added angle dwell";
                    }
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "black to blank";
                        space.render_space();
                        display_space();
                        space.convert_black_to_blank();
                        space.TUI_clue = "black to blank all";
                    }
                    else
                    {
                        space.TUI_clue = "adding angle dwell";
                        space.render_space();
                        display_space();
                        if(space.add_dwell())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "angle dwell fail";
                        }
                        else
                            space.TUI_clue = "added angle dwell all";
                    }
                    break;
                //------------------------------------------------------------
                case 'n':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().convert_blank_to_black();
                        space.TUI_clue = "blank to black";
                    }
                    else
                    {
                        if(space.current_frame().add_lit_span_vertices())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "distance span fail";
                        }
                        else
                            space.TUI_clue = "added lit distance span";
                    }
                    break;
                //------------------------------------------------------------
                case 'N':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "black to blank";
                        space.render_space();
                        display_space();
                        space.convert_blank_to_black();
                        space.TUI_clue = "blank to black all";
                    }
                    else
                    {
                        space.TUI_clue = "adding lit distance span";
                        space.render_space();
                        display_space();
                        if(space.add_lit_span_vertices())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "distance span fail";
                        }
                        else
                            space.TUI_clue = "added lit distance span all";
                    }
                    break;
                //------------------------------------------------------------
                case 'o':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().impose_black_level();
                        space.TUI_clue = "apply black level";
                    }
                    else
                    {
                        if(space.current_frame().add_blank_span_vertices())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "distance span fail";
                        }
                        else
                            space.TUI_clue = "added blank distance span";
                    }
                    break;
                //------------------------------------------------------------
                case 'O':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "applying black level";
                        space.render_space();
                        display_space();
                        space.impose_black_level();
                        space.TUI_clue = "black leveled all";
                    }
                    else
                    {
                        space.TUI_clue = "adding blank distance span";
                        space.render_space();
                        display_space();
                        if(space.add_blank_span_vertices())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "distance span fail";
                        }
                        else
                            space.TUI_clue = "added blank distance span all";
                    }
                    break;
                //------------------------------------------------------------
                case 'p':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().to_dots();
                        space.TUI_clue = "to dots";
                    }
                    else
                    {
                        if(space.current_frame().enhance_dots())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "enhance dots fail";
                        }
                        else
                            space.TUI_clue = "enhanced dots";
                    }
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.alt_menu_h)
                    {
                        space.TUI_clue = "converting to dots";
                        space.render_space();
                        display_space();
                        space.to_dots();
                        space.TUI_clue = "to dots all";
                    }
                    else
                    {
                        space.TUI_clue = "enhancing dots";
                        space.render_space();
                        display_space();
                        if(space.enhance_dots())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "enhance dots fail";
                        }
                        else
                            space.TUI_clue = "enhanced dots";
                    }
                    break;
                //------------------------------------------------------------
                case 'q':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().name = display_prompt_and_echo_name("new frame name [8 char or less]", 8);
                        if(space.current_frame().name.size() == 0)
                        {
                            char number[12];
                            sprintf(number, "%05d", space.frame_index);
                            space.current_frame().name = string("LB_") + string(number);
                        }
                        space.TUI_clue = "rename";
                    }
                    else
                    {
                        space.current_frame().minimize(0);
                        if(space.current_frame().optimize(0))
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "optimize fail";
                        }
                        else
                            space.TUI_clue = "optimized";
                    }
                    break;
                //------------------------------------------------------------
                case 'Q':
                    if(space.alt_menu_h)
                    {
                        space.current_frame().name = display_prompt_and_echo_name("new frames prefix [3 char or less]", 3);
                        if(space.current_frame().name.size() == 0)
                            space.current_frame().name = "LB_";
                        space.rename_all_frames(space.current_frame().name);
                        space.TUI_clue = "rename";
                    }
                    else
                    {
                        space.TUI_clue = "optimizing";
                        space.render_space();
                        display_space();
                        if(space.optimize())
                        {
                            space.display_error("vertex count overflow");
                            wait_4_Esc();
                            space.TUI_clue = "optimize fail";
                        }
                        else
                            space.TUI_clue = "optimized all";
                    }
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame_set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            }
        }
    }
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::frame_set_transforms_menu()
{
    bool   in_menu              = true,
           show_menu_was        = space.show_menu;
    u_int  temp_u_int           = 0;
    space.show_menu             = true;
    prompt_escaped              = false;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_frame_set_tranz_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit frame_set transforms";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'a':
                case 'A':
                    space.TUI_clue = "inverting frame selections";
                    space.render_space();
                    display_space();
                    space.invert_frame_selections();
                    space.TUI_clue = "inverted frame selections";
                    break;
                //------------------------------------------------------------
                case 'b':
                case 'B':
                    space.TUI_clue = "deleting selected frames";
                    space.render_space();
                    display_space();
                    space.delete_selected_frames();
                    space.TUI_clue = "deleted selected frames";
                    break;
                //------------------------------------------------------------
                case 'c':
                case 'C':
                    space.TUI_clue = "trimming to selected frames";
                    space.render_space();
                    display_space();
                    space.trim_to_selected_frames();
                    space.TUI_clue = "trimmed to selected frames";
                    break;
                //------------------------------------------------------------
                case 'd':
                case 'D':
                    space.TUI_clue = "deleting redundant frames";
                    space.render_space();
                    display_space();
                    space.delete_redundant_frames();
                    space.TUI_clue = "redundant frames deleted";
                    break;
                //------------------------------------------------------------
                case 'e':
                case 'E':
                    space.TUI_clue = "copying selected frames";
                    space.render_space();
                    display_space();
                    space = space.selected_frames() + space;
                    space.TUI_clue = "copied frames to begining";
                    break;
                //------------------------------------------------------------
                case 'f':
                case 'F':
                    space.TUI_clue = "copying selected frames";
                    space.render_space();
                    display_space();
                    space += space.selected_frames();
                    space.TUI_clue = "copied frames to end";
                    break;
                //------------------------------------------------------------
                case 'g':
                case 'G':
                    space.TUI_clue = "moving selected frames";
                    space.render_space();
                    display_space();
                    {
                        LaserBoy_frame_set frame_set(space.selected_frames());
                        space.delete_selected_frames();
                        frame_set += space;
                        space = frame_set;
                    }
                    space.TUI_clue = "moved frames to begining";
                    break;
                //------------------------------------------------------------
                case 'h':
                case 'H':
                    space.TUI_clue = "moving selected frames";
                    space.render_space();
                    display_space();
                    {
                        LaserBoy_frame_set frame_set(space.selected_frames());
                        space.delete_selected_frames();
                        space += frame_set;
                    }
                    space.TUI_clue = "moved frames to end";
                    break;
                //------------------------------------------------------------
                case 'i':
                case 'I':
                    {
                        LaserBoy_frame_set frame_set;
                        frame_set += LaserBoy_frame(space.target_palette_index, true); // not quite an empty frame!
                        space = frame_set + space;
                    }
                    space.TUI_clue = "add frame to begining";
                    break;
                //------------------------------------------------------------
                case 'j':
                case 'J':
                    {
                        u_int              i;
                        LaserBoy_frame_set frame_set;
                        for(i = 0; i <= space.frame_index; i++)
                            frame_set += space[i];
                        frame_set += LaserBoy_frame(space.target_palette_index, true);
                        for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                            frame_set += space[i];
                        space = frame_set;
                    }
                    space.TUI_clue = "add after current frame";
                    break;
                //------------------------------------------------------------
                case 'k':
                case 'K':
                    {
                        LaserBoy_frame frame(space.target_palette_index, true);
                        LaserBoy_frame_set frame_set(space);
                        frame_set += frame;
                        space = frame_set;
                    }
                    space.TUI_clue = "add frame to end";
                    break;
                //------------------------------------------------------------
                case 'l':
                case 'L':
                    space.TUI_clue = "reversing order of frames";
                    space.render_space();
                    display_space();
                    space.reverse();
                    space.TUI_clue = "reverse order";
                    break;
                //------------------------------------------------------------
                case 'm':
                case 'M':
                    space.TUI_clue = "reversing selected frames";
                    space.render_space();
                    display_space();
                    space.reverse_selected();
                    space.TUI_clue = "reverse selected";
                    break;
                //------------------------------------------------------------
                case 'n':
                case 'N':
                    space.TUI_clue = "new first frame";
                    space.render_space();
                    display_space();
                    space.make_current_first();
                    space.TUI_clue = "new first frame";
                    break;
                //------------------------------------------------------------
                case 'o':
                case 'O':
                    space.TUI_clue = "random order frames";
                    space.render_space();
                    display_space();
                    space.random_order();
                    space.TUI_clue = "random order";
                    break;
                //------------------------------------------------------------
                case 'p':
                case 'P':
                    space.TUI_clue = "order frames by name";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_Error_Code  ret = LASERBOY_OK;
                        string               report;
                        ret = space.order_frames_by_name(report);
                        if(ret & LASERBOY_INVALID_UNICODE)
                        {
                            space.display_error(report);
                            wait_4_Esc();
                            break;
                        }
                        else if(ret & LASERBOY_REDUNDANT_UNICODE)
                        {
                            space.display_error(report);
                            wait_4_Esc();
                            if(!display_prompt_and_echo_bool("delete redundant named frames ? (y | [any])"))
                            {
                                space.display_error("frame_set order not changed.");
                                wait_4_Esc();
                                break;
                            }
                            else
                                space.order_frames_by_name(report, true);
                        }
                    }
                    space.TUI_clue = "frames ordered by name";
                    break;
                //------------------------------------------------------------
                case 'q':
                case 'Q':
                    space.TUI_clue = "remove every n-th frames";
                    space.render_space();
                    display_space();
                    temp_u_int = 0;
                    while(temp_u_int == 0 || temp_u_int == 1)
                    {
                        temp_u_int = display_prompt_and_echo_u_int(  "n"
                                                                    , 2
                                                                    , space.number_of_frames() - 1
                                                                  );
                        if(prompt_escaped)
                            break;
                    }
                    if(temp_u_int == 0 || temp_u_int == 1)
                        break;
                    space.delete_every_nth_frame(temp_u_int);
                    space.TUI_clue = "n-th frames removed";
                    break;
                //------------------------------------------------------------
                case 'r':
                case 'R':
                    space.TUI_clue = "keep only every n-th frames";
                    space.render_space();
                    display_space();
                    temp_u_int = display_prompt_and_echo_u_int(  "n"
                                                                , 2
                                                                , space.number_of_frames() - 1
                                                              );
                    if(prompt_escaped)
                        break;
                    space.keep_every_nth_frame(temp_u_int);
                    space.TUI_clue = "n-th frame keep";
                    break;
                //------------------------------------------------------------
                case 'y': // go to frame index
                {
                    u_int temp_u_int;
                    space.TUI_clue = "go to index";
                    temp_u_int = display_prompt_and_echo_u_int(   "frame index"
                                                                , 0
                                                                , space.number_of_frames() - 1
                                                              );
                    if(prompt_escaped)
                        break;
                    space.frame_index = temp_u_int % space.number_of_frames();
                    break;
                }
                //------------------------------------------------------------
                case 'Y': // go to frame unicode
                {
                    int     frame_index;
                    string  unihex;
                    space.TUI_clue = "go to unicode";
                    unihex = display_prompt_and_echo_unicode("frame unicode");
                    if(prompt_escaped)
                        break;
                    frame_index = space.index_of_name(unihex);
                    if(frame_index >= 0)
                        space.frame_index = frame_index;
                    else
                    {
                        space.display_error(unihex + " not found.");
                        wait_4_Esc();
                    }
                    break;
                }
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case '>': // explode current frame
                    space.explode_current_frame();
                    space.TUI_clue = "explode segments";
                    break;
                //------------------------------------------------------------
                case '<': // collapse selected frames
                    if(space.collapse_selected_frames())
                    {
                        space.display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "collapse segments fail";
                    }
                    else
                        space.TUI_clue = "collapse segments";
                    break;
                //------------------------------------------------------------
                case '=': // explode all frames
                    space.explode_all_frames();
                    space.TUI_clue = "explode segments";
                    break;
                //------------------------------------------------------------
                case '`':
                    animate_forward();
                    break;
                //------------------------------------------------------------
                case '~':
                    animate_reverse();
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame_set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            }
        }
    }
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_color_blank_menu()
{
                  bool  in_menu                 = true,
                        show_vertices_was       = space.show_vertices,
                        show_blanking_was       = space.show_blanking,
                        show_intro_was          = space.show_intro,
                        show_bridge_was         = space.show_bridge,
                        show_coda_was           = space.show_coda,
                        show_palette_was        = space.show_palette,
                        show_target_palette_was = space.show_target_palette,
                        show_menu_was           = space.show_menu;
                   int  temp_int;
                double  temp_double;
    LaserBoy_3D_double  temp_3D_double;
    //------------------------------------------------------------------------
    space.show_menu         = true;
    space.show_vertices     = true;
    space.show_blanking     = true;
    space.show_intro        = false;
    space.show_bridge       = false;
    space.show_coda         = false;
    space.show_palette      = true;
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
        space.show_target_palette = true;
    space.show_cursors      = true;
    space.show_color_cursor = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_draw_color_blank_menu();
        display_space();
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu                   = false;
                    space.show_cursors        = false;
                    space.show_color_cursor   = false;
                    space.show_vertices       = show_vertices_was;
                    space.show_blanking       = show_blanking_was;
                    space.show_intro          = show_intro_was;
                    space.show_bridge         = show_bridge_was;
                    space.show_coda           = show_coda_was;
                    space.show_palette        = show_palette_was;
                    space.show_target_palette = show_target_palette_was;
                    space.TUI_clue            = "exit draw color blank";
                    break;
                //------------------------------------------------------------
                case 't': // toggle menu mode
                case 'T':
                    space.alt_menu_k = !space.alt_menu_k;
                    if(space.alt_menu_k)
                        space.TUI_clue = "alt menu";
                    else
                        space.TUI_clue = "draw menu";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.current_frame().remove_at_spider();
                    space.TUI_clue = "delete vertex";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case '[':
                    if(    (space.current_frame().egg < space.current_frame().spider)
                        && (space.current_frame().egg < space.current_frame().size() - 1)
                      )
                    {
                        space.current_frame().increment_egg();
                        space.TUI_clue = "+ start vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select start must be behind or equal to select end");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case '{':
                    if(space.current_frame().egg > 0)
                    {
                        space.current_frame().deccrement_egg();
                        space.TUI_clue = "- start vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select start must be at zero or ahead");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case ']':
                    if(space.current_frame().spider < space.current_frame().size() - 1)
                    {
                        space.current_frame().increment_spider();
                        space.TUI_clue = "+ end vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select end must be behind or equal to last vertex");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case '}':
                    if(    (space.current_frame().spider > space.current_frame().egg)
                        && (space.current_frame().spider > 0)
                      )
                    {
                        space.current_frame().deccrement_spider();
                        space.TUI_clue = "- end vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select end must be ahead or equal to select start");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case'\\':
                    space.current_frame().spider = (space.current_frame().size() - 1);
                    space.TUI_clue = "last vertex select";
                    break;
                //------------------------------------------------------------
                case '|':
                    space.current_frame().egg = 0;
                    space.TUI_clue = "first vertex select";
                    break;
                //------------------------------------------------------------
                case 'u':
                    space.current_frame().next_segment_select();
                    space.TUI_clue = "+ lit segment";
                    break;
                //------------------------------------------------------------
                case 'U':
                    space.current_frame().previous_segment_select();
                    space.TUI_clue = "- lit segment";
                    break;
                //------------------------------------------------------------
                case 'i':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().egg, start, end, segment_index);
                        space.current_frame().find_segment_at_index(segment_index + 1, start, end);
                        if(    start <= space.current_frame().spider
                            && start < (space.current_frame().size() - 1)
                          )
                        {
                            space.current_frame().next_segment_egg();
                            space.TUI_clue = "+ start lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select start must be behind or equal to select end");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'I':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().egg, start, end, segment_index);
                        if(segment_index > 0)
                        {
                            space.current_frame().previous_segment_egg();
                            space.TUI_clue = "- start lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select start must be at zero or ahead");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'o':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().spider, start, end, segment_index);
                        if(segment_index < space.current_frame().number_of_segments() - 1)
                        {
                            space.current_frame().next_segment_spider();
                            space.TUI_clue = "+ end lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select end must be at or behind last vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'O':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().spider, start, end, segment_index);
                        if(    start > space.current_frame().egg
                            && segment_index > 0
                          )
                        {
                            space.current_frame().previous_segment_spider();
                            space.TUI_clue = "- end lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select end must be at or ahead of select start and first vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'K':
                    space.current_frame().spider = space.current_frame().egg;
                    space.TUI_clue = "spider to egg";
                    break;
                //------------------------------------------------------------
                case 'L':
                    space.current_frame().egg = space.current_frame().spider;
                    space.TUI_clue = "egg to spider";
                    break;
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(space.selected_color_index >= space.target_palette().size())
                            space.selected_color_index = 0;
                    }
                    else
                    {
                        if(space.selected_color_index >= space.current_palette().size())
                            space.selected_color_index = 0;
                    }
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':
                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(space.selected_color_index == 0)
                            space.selected_color_index = space.target_palette().size();
                    }
                    else
                    {
                        if(space.selected_color_index == 0)
                            space.selected_color_index = space.current_palette().size();
                    }
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case 'b':
                    space.current_frame().blank_vertices();
                    space.TUI_clue = "blank selected vectors";
                    break;
                //------------------------------------------------------------
                case 'B':
                    space.current_frame().unblank_vertices();
                    space.TUI_clue = "unblank selected vectors";
                    break;
                //------------------------------------------------------------
                case 'c':
                    space.current_frame().color_select();
                    space.TUI_clue = "color selected vectors";
                    break;
                //------------------------------------------------------------
                case 'C':
                    if(space.current_frame().palette_index != LASERBOY_TRUE_COLOR)
                    {
                        space.selected_color_index = space.current_frame().at(space.current_frame().spider).c;
                        space.TUI_clue = "get color";
                    }
                    else
                        space.TUI_clue = "no palette";
                    break;
                //------------------------------------------------------------
                case '\'':
                    {
                        int index = display_prompt_and_echo_int("place egg at index");
                        if(prompt_escaped)
                            break;
                        if(index <= (int)space.current_frame().spider && index >= 0)
                        {
                            space.current_frame().egg = index;
                            space.current_frame().normalize_cursors();
                        }
                        else
                        {
                            space.display_message("vertex select start must be between the first vertex and select end");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case '"':
                    {
                        int index = display_prompt_and_echo_int("place spider at index");
                        if(prompt_escaped)
                            break;
                        if(index >= (int)space.current_frame().egg)
                        {
                            if(index < (int)space.current_frame().size())
                            {
                                space.current_frame().spider = index;
                                space.current_frame().normalize_cursors();
                            }
                            else
                            {
                                space.current_frame().spider = space.current_frame().size() - 1;
                                space.current_frame().normalize_cursors();
                                space.display_message("vertex select end must be at or behind last vertex");
                                wait_4_any_key();
                            }
                        }
                        else
                        {
                            space.display_message("vertex select end must be between select start and last vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'j':
                    space.current_frame().set_spider_to_egg();
                    space.TUI_clue = "spider to egg";
                    break;
                //------------------------------------------------------------
                case 'J':
                    space.current_frame().set_egg_to_spider();
                    space.TUI_clue = "egg to spider";
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    space.TUI_clue = "frame_set stats";
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    space.TUI_clue = "frame stats";
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
                //------------------------------------------------------------
                case 'Q':
                    if(space.bmp_file_index != -1)
                    {
                        int prev =   (space.bmp_file_index == 0)
                                   ? (space.bmp_file_list.size() - 1)
                                   : (space.bmp_file_index - 1);
                        space.TUI_clue = "loading " + space.bmp_file_list[prev].path().filename().string();
                        display_space();
                        if(!space.load_background_bitmap(space.bmp_file_list[prev].path().string(),
                                                         false
                                                        )
                          )
                        {
                            space.display_error(space.bmp_file_list[prev].path().string() + " failed to open");
                            wait_4_Esc();
                            break;
                        }
                        space.bmp_file_index = prev;
                    }
                    else
                    {
                        space.TUI_clue = "no bmp dir loaded";
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                case 'W':
                    if(space.bmp_file_index != -1)
                    {
                        int next =   (space.bmp_file_index == (int)space.bmp_file_list.size() - 1)
                                   ? (0)
                                   : (space.bmp_file_index + 1);
                        space.TUI_clue = "loading " + space.bmp_file_list[next].path().filename().string();
                        display_space();
                        if(!space.load_background_bitmap(space.bmp_file_list[next].path().string(),
                                                         false
                                                        )
                          )
                        {
                            space.display_error(space.bmp_file_list[next].path().string() + " failed to open");
                            wait_4_Esc();
                            break;
                        }
                        space.bmp_file_index = next;
                    }
                    else
                    {
                        space.TUI_clue = "no bmp dir loaded";
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                case 'x':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).x + space.displacement_step;
                        if(temp_double <= LASERBOY_MAX_SHORT)
                            space.current_frame().at(space.current_frame().spider).x = (u_short)temp_double;
                        space.TUI_clue = "+ X";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'X':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).x - space.displacement_step;
                        if(temp_double >= LASERBOY_MIN_SHORT)
                            space.current_frame().at(space.current_frame().spider).x = (u_short)temp_double;
                        space.TUI_clue = "- X";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'y':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).y + space.displacement_step;
                        if(temp_double <= LASERBOY_MAX_SHORT)
                            space.current_frame().at(space.current_frame().spider).y = (u_short)temp_double;
                        space.TUI_clue = "+ Y";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'Y':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).y - space.displacement_step;
                        if(temp_double >= LASERBOY_MIN_SHORT)
                            space.current_frame().at(space.current_frame().spider).y = (u_short)temp_double;
                        space.TUI_clue = "- Y";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'z':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).z + space.displacement_step;
                        if(temp_double <= LASERBOY_MAX_SHORT)
                            space.current_frame().at(space.current_frame().spider).z = (u_short)temp_double;
                        space.TUI_clue = "+ Z";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'Z':
                    if(space.current_frame().size() > 1)
                    {
                        temp_double = space.current_frame().at(space.current_frame().spider).z - space.displacement_step;
                        if(temp_double >= LASERBOY_MIN_SHORT)
                            space.current_frame().at(space.current_frame().spider).z = (u_short)temp_double;
                        space.TUI_clue = "- Z";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case '.':
                    if(space.current_frame().add_vertex(space.current_frame().spider))
                    {
                        space.display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "add vertex fail";
                    }
                    else
                    {
                        if(space.current_frame().spider < (space.current_frame().size() - 1))
                            space.TUI_clue = "split vector";
                        else
                            space.TUI_clue = "add vector";
                    }
                    break;
                //------------------------------------------------------------
                case '>':
                    if(space.current_frame().break_segment())
                    {
                        space.display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "break segment fail";
                    }
                    else
                        space.TUI_clue = "break segment";
                    break;
                //------------------------------------------------------------
                case ',':
                    space.current_frame().reorder_from_egg();
                    space.TUI_clue = "reorder from egg";
                    break;
                //------------------------------------------------------------
                case '<':
                    space.current_frame().reorder_from_spider();
                    space.TUI_clue = "reorder from spider";
                    break;
                //------------------------------------------------------------
                case 'q':
                    if(space.current_frame().size() > 1)
                    {
                        temp_int = display_prompt_and_echo_short("set X");
                        if(prompt_escaped)
                            break;
                        space.current_frame().at(space.current_frame().spider).x = temp_int;
                        space.TUI_clue = "X set";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'w':
                    if(space.current_frame().size() > 1)
                    {
                        temp_int = display_prompt_and_echo_short("set Y");
                        if(prompt_escaped)
                            break;
                        space.current_frame().at(space.current_frame().spider).y = temp_int;
                        space.TUI_clue = "Y set";
                    }
                    else
                    {
                        space.display_error("ERROR NO VERTICES!");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case 'e':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().black_vertices();
                        space.TUI_clue = "bond black selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().size() > 1)
                        {
                            temp_int = display_prompt_and_echo_short("set Z");
                            if(prompt_escaped)
                                break;
                            space.current_frame().at(space.current_frame().spider).z = temp_int;
                            space.TUI_clue = "Z set";
                        }
                        else
                        {
                            space.display_error("ERROR NO VERTICES!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'E':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().unblack_vertices();
                        space.TUI_clue = "unbond unblack selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().size() > 1)
                        {
                            temp_int = display_prompt_and_echo_short("set Z");
                            if(prompt_escaped)
                                break;
                            space.current_frame().at(space.current_frame().spider).z = temp_int;
                            space.TUI_clue = "Z set";
                        }
                        else
                        {
                            space.display_error("ERROR NO VERTICES!");
                            wait_4_Esc();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'l':
                    if(space.current_frame().connect_the_dots())
                    {
                        space.display_error("vertex count overflow");
                        wait_4_Esc();
                        space.TUI_clue = "line egg to spider fail";
                    }
                    else
                        space.TUI_clue = "line egg to spider";
                    break;
                //------------------------------------------------------------
                case 'd':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().selected_to_dots();
                        space.TUI_clue = "to dots";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = rotate_vertex_on_coordinates_x(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "+ rotate on X axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'D':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().selected_to_dots();
                        space.TUI_clue = "to dots";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = rotate_vertex_on_coordinates_x(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             -space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "- rotate on X axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_index_select();
                        space.TUI_clue = "color index selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                           temp_3D_double
                            = rotate_vertex_on_coordinates_y(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "+ rotate on Z axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'F':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_span_select();
                        space.TUI_clue = "color span selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {

                            temp_3D_double
                            = rotate_vertex_on_coordinates_y(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             -space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "- rotate on Z axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_black_in_select();
                        space.TUI_clue = "color black selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = rotate_vertex_on_coordinates_z(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "+ rotate on Y axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'G':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_black_in_select();
                        space.TUI_clue = "color black selected vectors";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = rotate_vertex_on_coordinates_z(space.current_frame().at(space.current_frame().spider),
                                                             space.current_frame().at(space.current_frame().spider - 1),
                                                             -space.rotation_step
                                                            );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "- rotate on Y axis";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_index_black();
                        space.TUI_clue = "color index black";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = scale_vertex_on_coordinates(space.current_frame().at(space.current_frame().spider),
                                                          space.current_frame().at(space.current_frame().spider - 1),
                                                          LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)
                                                         );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "+ magnitude of vector";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_span_black();
                        space.TUI_clue = "color span black";
                    }
                    else
                    {
                        if(space.current_frame().spider) // is not the origin
                        {
                            temp_3D_double
                            = scale_vertex_on_coordinates(space.current_frame().at(space.current_frame().spider),
                                                          space.current_frame().at(space.current_frame().spider - 1),
                                                          LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)
                                                         );
                            if(!LaserBoy_bounds_check(temp_3D_double, LASERBOY_CUBE))
                                space.current_frame().at(space.current_frame().spider) = temp_3D_double;
                            space.TUI_clue = "- magnitude of vector";
                        }
                        else
                            space.TUI_clue = "not at zero vertex";
                    }
                    break;
                //------------------------------------------------------------
                case 'n':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_cycle_select(1);
                        space.TUI_clue = "+ color cycle";
                    }
                    break;
                //------------------------------------------------------------
                case 'N':
                    if(space.alt_menu_k)
                    {
                        space.current_frame().color_cycle_select(-1);
                        space.TUI_clue = "- color cycle";
                    }
                    break;
            } // end switch(event.key.keysym.unicode)
       } // end else if(event.type == LASERBOY_GUI_KEYDOWN)
   } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::move_scale_rotate_menu()
{
    bool    in_menu              = true,
            show_vertices_was    = space.show_vertices,
            show_blanking_was    = space.show_blanking,
            show_intro_was       = space.show_intro,
            show_bridge_was      = space.show_bridge,
            show_coda_was        = space.show_coda,
            show_menu_was        = space.show_menu;
    char    formatted_string[80] = {0};
    double  temp_double;
    //------------------------------------------------------------------------
    space.show_menu         = true;
    space.show_vertices     = true;
    space.show_blanking     = true;
    space.show_intro        = false;
    space.show_bridge       = false;
    space.show_coda         = false;
    space.show_cursors      = true;
    prompt_escaped          = false;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_move_scale_rotate_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu             = false;
                    space.show_vertices = show_vertices_was;
                    space.show_blanking = show_blanking_was;
                    space.show_intro    = show_intro_was;
                    space.show_bridge   = show_bridge_was;
                    space.show_coda     = show_coda_was;
                    space.show_cursors  = false;
                    space.show_fulcrum  = false;
                    space.TUI_clue = "exit move scale rotate";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 't': // toggle fulcrum mode
                case 'T':
                    space.alt_menu_l   = !space.alt_menu_l;
                    space.show_fulcrum = !space.show_fulcrum;
                    if(space.alt_menu_l)
                        space.TUI_clue = "on fulcrum";
                    else
                        space.TUI_clue = "on itself";
                    break;
                //------------------------------------------------------------
                case '[':
                    if(    (space.current_frame().egg < space.current_frame().spider)
                        && (space.current_frame().egg < space.current_frame().size() - 1)
                      )
                    {
                        space.current_frame().increment_egg();
                        space.TUI_clue = "+ start vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select start must be behind or equal to select end");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case '{':
                    if(space.current_frame().egg > 0)
                    {
                        space.current_frame().deccrement_egg();
                        space.TUI_clue = "- start vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select start must be at zero or ahead");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case ']':
                    if(space.current_frame().spider < space.current_frame().size() - 1)
                    {
                        space.current_frame().increment_spider();
                        space.TUI_clue = "+ end vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select end must be behind or equal to last vertex");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case '}':
                    if(    (space.current_frame().spider > space.current_frame().egg)
                        && (space.current_frame().spider > 0)
                      )
                    {
                        space.current_frame().deccrement_spider();
                        space.TUI_clue = "- end vertex select";
                    }
                    else if(space.show_cursor_limits_msg)
                    {
                        space.display_message("vertex select end must be ahead or equal to select start");
                        wait_4_any_key();
                    }
                    break;
                //------------------------------------------------------------
                case'\\':
                    space.current_frame().spider = (space.current_frame().size() - 1);
                    space.TUI_clue = "last vertex select";
                    break;
                //------------------------------------------------------------
                case '|':
                    space.current_frame().egg = 0;
                    space.TUI_clue = "first vertex select";
                    break;
                //------------------------------------------------------------
                case 'u':
                    space.current_frame().next_segment_select();
                    space.TUI_clue = "+ lit segment";
                    break;
                //------------------------------------------------------------
                case 'U':
                    space.current_frame().previous_segment_select();
                    space.TUI_clue = "- lit segment";
                    break;
                //------------------------------------------------------------
                case 'i':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().egg, start, end, segment_index);
                        space.current_frame().find_segment_at_index(segment_index + 1, start, end);
                        if(    (start <= space.current_frame().spider)
                            && (start < space.current_frame().size() - 1)
                          )
                        {
                            space.current_frame().next_segment_egg();
                            space.TUI_clue = "+ start lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select start must be behind or equal to select end");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'I':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().egg, start, end, segment_index);
                        if(segment_index > 0)
                        {
                            space.current_frame().previous_segment_egg();
                            space.TUI_clue = "- start lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select start must be at zero or ahead");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'o':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().spider, start, end, segment_index);
                        if(segment_index < space.current_frame().number_of_segments() - 1)
                        {
                            space.current_frame().next_segment_spider();
                            space.TUI_clue = "+ end lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select end must be at or behind last vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'O':
                    {
                        size_t segment_index, start, end;
                        space.current_frame().find_segment_of_vertex(space.current_frame().spider, start, end, segment_index);
                        if(    start > space.current_frame().egg
                            && segment_index > 0
                          )
                        {
                            space.current_frame().previous_segment_spider();
                            space.TUI_clue = "- end lit segment";
                        }
                        else if(space.show_cursor_limits_msg)
                        {
                            space.display_message("vertex select end must be at or ahead of select start and first vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case 'K':
                    space.current_frame().spider = space.current_frame().egg;
                    space.TUI_clue = "spider to egg";
                    break;
                //------------------------------------------------------------
                case 'L':
                    space.current_frame().egg = space.current_frame().spider;
                    space.TUI_clue = "egg to spider";
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case '<':
                    space.current_frame().reverse_selected();
                    space.TUI_clue = "reversed selection";
                    break;
                //------------------------------------------------------------
                case 'c':
                    switch(space.current_frame().append_selected_segment())
                    {
                        default:
                        case LASERBOY_NOTHING_SELECTED:
                            space.display_error("nothing selected");
                            wait_4_Esc();
                            space.TUI_clue = "copy segment fail";
                            break;
                        break;
                        case LASERBOY_OK:
                            sprintf(formatted_string, "copied %d vertices to end of frame", (int)space.current_frame().selected_segment().size());
                            space.TUI_clue = formatted_string;
                            break;
                    }
                    break;
                //------------------------------------------------------------
                case 'q':
                    temp_double = display_prompt_and_echo_double("set center X");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(temp_double - space.current_frame().selected_segment().rectangular_center_of().x,
                                                                                0,
                                                                                0
                                                                               )
                                                            )
                      )
                        space.TUI_clue = "set center X";
                    else
                    {
                        space.display_error("X out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'Q':
                    if(space.bmp_file_index != -1)
                    {
                        int prev =   (space.bmp_file_index == 0)
                                   ? (space.bmp_file_list.size() - 1)
                                   : (space.bmp_file_index - 1);
                        space.TUI_clue = "loading " + space.bmp_file_list[prev].path().filename().string();
                        display_space();
                        if(!space.load_background_bitmap(space.bmp_file_list[prev].path().string(),
                                                         false
                                                        )
                          )
                        {
                            space.display_error(space.bmp_file_list[prev].path().string() + " failed to open");
                            wait_4_Esc();
                            break;
                        }
                        space.bmp_file_index = prev;
                    }
                    else
                    {
                        space.TUI_clue = "no bmp dir loaded";
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                case 'w':
                    temp_double = display_prompt_and_echo_double("set center Y");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0,
                                                                                temp_double - space.current_frame().selected_segment().rectangular_center_of().y,
                                                                                0
                                                                               )
                                                            )
                      )
                        space.TUI_clue = "set center Y";
                    else
                    {
                        space.display_error("Y out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'W':
                    if(space.bmp_file_index != -1)
                    {
                        int next =   (space.bmp_file_index == (int)space.bmp_file_list.size() - 1)
                                   ? (0)
                                   : (space.bmp_file_index + 1);
                        space.TUI_clue = "loading " + space.bmp_file_list[next].path().filename().string();
                        display_space();
                        if(!space.load_background_bitmap(space.bmp_file_list[next].path().string(),
                                                         false
                                                        )
                          )
                        {
                            space.display_error(space.bmp_file_list[next].path().string() + " failed to open");
                            wait_4_Esc();
                            break;
                        }
                        space.bmp_file_index = next;
                    }
                    else
                    {
                        space.TUI_clue = "no bmp dir loaded";
                        display_space();
                    }
                    break;
                //------------------------------------------------------------
                case 'e':
                    temp_double = display_prompt_and_echo_double("set center Z");
                    if(prompt_escaped)
                        break;
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0,
                                                                                0,
                                                                                temp_double - space.current_frame().selected_segment().rectangular_center_of().z
                                                                               )
                                                            )
                      )
                        space.TUI_clue = "set center Z";
                    else
                    {
                        space.display_error("Z out of bounds!");
                        wait_4_Esc();
                        space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'x':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(space.displacement_step, 0, 0)))
                        space.TUI_clue = "+ X move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'X':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(-space.displacement_step, 0, 0)))
                        space.TUI_clue = "- X move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'y':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, space.displacement_step, 0)))
                        space.TUI_clue = "+ Y move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'Y':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, -space.displacement_step, 0)))
                        space.TUI_clue = "- Y move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'z':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, 0, space.displacement_step)))
                        space.TUI_clue = "+ Z move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'Z':
                    if(!space.current_frame().move_selection(LaserBoy_3D_double(0, 0,-space.displacement_step)))
                        space.TUI_clue = "- Z move";
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'd':
                    if(space.alt_menu_l)
                    {
                      if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(space.rotation_step, 0, 0)))
                          space.TUI_clue = "+ rotate on fulcrum X";
                      else
                          space.TUI_clue = "out of bounds!";
                       }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(space.rotation_step, 0, 0)))
                            space.TUI_clue = "+ rotate X";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'D':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(-space.rotation_step, 0, 0)))
                            space.TUI_clue = "- rotate on fulcrum X";
                        else
                            space.TUI_clue = "out of bounds!";
                         }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(-space.rotation_step, 0, 0)))
                            space.TUI_clue = "- rotate X";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'f':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, space.rotation_step, 0)))
                            space.TUI_clue = "+ rotate on fulcrum Y";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, space.rotation_step, 0)))
                            space.TUI_clue = "+ rotate Y";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'F':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, -space.rotation_step, 0)))
                            space.TUI_clue = "- rotate on fulcrum Y";
                        else
                            space.TUI_clue = "out of bounds!";
                     }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0,-space.rotation_step, 0)))
                            space.TUI_clue = "- rotate Y";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'g':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, 0, space.rotation_step)))
                            space.TUI_clue = "+ rotate on fulcrum Z";
                        else
                            space.TUI_clue = "out of bounds!";
                     }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, 0, space.rotation_step)))
                            space.TUI_clue = "+ rotate Z";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'G':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().rotate_selection_on_fulcrum(LaserBoy_3D_double(0, 0, -space.rotation_step)))
                            space.TUI_clue = "- rotate on fulcrum Z";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().rotate_selection(LaserBoy_3D_double(0, 0, -space.rotation_step)))
                            space.TUI_clue = "- rotate Z";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'm':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)))
                            space.TUI_clue = "+ scale XYZ by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_up, space.magnitude_step_up, space.magnitude_step_up)))
                            space.TUI_clue = "+ scale XYZ";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'M':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)))
                            space.TUI_clue = "- scale XYZ by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_dn, space.magnitude_step_dn, space.magnitude_step_dn)))
                            space.TUI_clue = "- scale XYZ";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'v':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_up, 1, 1)))
                            space.TUI_clue = "+ scale X by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_up, 1, 1)))
                            space.TUI_clue = "+ scale X";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'V':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(space.magnitude_step_dn, 1, 1)))
                            space.TUI_clue = "- scale X by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(space.magnitude_step_dn, 1, 1)))
                            space.TUI_clue = "- scale X";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'b':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, space.magnitude_step_up, 1)))
                            space.TUI_clue = "+ scale Y by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, space.magnitude_step_up, 1)))
                            space.TUI_clue = "+ scale Y";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'B':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, space.magnitude_step_dn, 1)))
                            space.TUI_clue = "- scale Y by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, space.magnitude_step_dn, 1)))
                            space.TUI_clue = "- scale Y";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'n':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, 1, space.magnitude_step_up)))
                            space.TUI_clue = "+ size Z by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, 1, space.magnitude_step_up)))
                            space.TUI_clue = "+ scale Z";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'N':
                    if(space.alt_menu_l)
                    {
                        if(!space.current_frame().scale_selection_on_fulcrum(LaserBoy_3D_double(1, 1, space.magnitude_step_dn)))
                            space.TUI_clue = "- scale Z by fulcrum";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    else
                    {
                        if(!space.current_frame().scale_selection(LaserBoy_3D_double(1, 1, space.magnitude_step_dn)))
                            space.TUI_clue = "- scale Z";
                        else
                            space.TUI_clue = "out of bounds!";
                    }
                    break;
                //------------------------------------------------------------
                case 'a':
                    space.fulcrum.x += space.displacement_step;
                    space.TUI_clue = "+ X fulcrum";
                    break;
                //------------------------------------------------------------
                case 'A':
                    space.fulcrum.x -= space.displacement_step;
                    space.TUI_clue = "- X fulcrum";
                    break;
                //------------------------------------------------------------
                case 's':
                    space.fulcrum.y += space.displacement_step;
                    space.TUI_clue = "+ Y fulcrum";
                    break;
                //------------------------------------------------------------
                case 'S':
                    space.fulcrum.y -= space.displacement_step;
                    space.TUI_clue = "- Y fulcrum";
                    break;
                //------------------------------------------------------------
                case 'r':
                    space.fulcrum.z += space.displacement_step;
                    space.TUI_clue = "+ Z fulcrum";
                    break;
                //------------------------------------------------------------
                case 'R':
                    space.fulcrum.z -= space.displacement_step;
                    space.TUI_clue = "- Z fulcrum";
                    break;
                //------------------------------------------------------------
                case 'j':
                    temp_double = display_prompt_and_echo_double(   "set fulcrum X (can be outside of space)"
                                                                  , 0
                                                                  , DBL_MAX
                                                                  , -DBL_MAX
                                                                );
                    if(prompt_escaped)
                        break;
                    space.fulcrum.x = temp_double;
                    space.TUI_clue = "vertex X set";
                    break;
                //------------------------------------------------------------
                case 'k':
                    temp_double = display_prompt_and_echo_double(   "set fulcrum Y (can be outside of space)"
                                                                  , 0
                                                                  , DBL_MAX
                                                                  , -DBL_MAX
                                                                );
                    if(prompt_escaped)
                        break;
                    space.fulcrum.y = temp_double;
                    space.TUI_clue = "vertex Y set";
                    break;
                //------------------------------------------------------------
                case 'l':
                    temp_double = display_prompt_and_echo_double(   "set fulcrum Z (can be outside of space)"
                                                                  , 0
                                                                  , DBL_MAX
                                                                  , -DBL_MAX
                                                                );
                    if(prompt_escaped)
                        break;
                    space.fulcrum.z = temp_double;
                    space.TUI_clue = "vertex Z set";
                    break;
                //------------------------------------------------------------
                case ',':
                    space.fulcrum = space.current_frame().at(space.current_frame().egg);
                    space.TUI_clue = "fulcrum to egg";
                    break;
                //------------------------------------------------------------
                case '.':
                    space.fulcrum = space.current_frame().at(space.current_frame().spider);
                    space.TUI_clue = "fulcrum to spider";
                    break;
                //------------------------------------------------------------
                case '\'':
                    {
                        int index = display_prompt_and_echo_int("place egg at index");
                        if(prompt_escaped)
                            break;
                        if(index <= (int)space.current_frame().spider && index >= 0)
                        {
                            space.current_frame().egg = index;
                            space.current_frame().normalize_cursors();
                        }
                        else
                        {
                            space.display_message("vertex select start must be between the first vertex and select end");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case '"':
                    {
                        int index = display_prompt_and_echo_int("place spider at index");
                        if(prompt_escaped)
                            break;
                        if(index >= (int)space.current_frame().egg)
                        {
                            if(index < (int)space.current_frame().size())
                            {
                                space.current_frame().spider = index;
                                space.current_frame().normalize_cursors();
                            }
                            else
                            {
                                space.current_frame().spider = space.current_frame().size() - 1;
                                space.current_frame().normalize_cursors();
                                space.display_message("vertex select end must be at or behind last vertex");
                                wait_4_any_key();
                            }
                        }
                        else
                        {
                            space.display_message("vertex select end must be between select start and last vertex");
                            wait_4_any_key();
                        }
                    }
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::direct_draw_segments_menu()
{
    bool    in_menu                  = true,
            show_palette_was         = space.show_palette,
            show_target_palette_was  = space.show_target_palette,
            show_menu_was            = space.show_menu;
    space.show_menu                  = true;
    space.show_palette               = true;
    space.show_color_cursor          = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.show_target_palette = space.current_frame().palette_index == LASERBOY_TRUE_COLOR ? true : false;
        space.clean_screen();
        space.render_space();
        if(space.show_menu)
            space.overlay_direct_draw_segments_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    in_menu                   = false;
                    space.show_color_cursor   = false;
                    space.show_palette        = show_palette_was;
                    space.show_target_palette = show_target_palette_was;
                    space.TUI_clue = "exit render segments";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_BACKSPACE:
                case LASERBOY_KEY_DELETE:
                    space.delete_current_frame();
                    space.TUI_clue = "deleted";
                    break;
                //------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
                //------------------------------------------------------------
                case 'a':
                    draw_point_menu();
                    break;
                //------------------------------------------------------------
                case 'b':
                    draw_line_menu();
                    break;
                //------------------------------------------------------------
                case 'c':
                    draw_rectangle_menu();
                    break;
                //------------------------------------------------------------
                case 'd':
                    draw_polygon_menu();
                    break;
                //------------------------------------------------------------
                case 'e':
                    draw_polyline_menu();
                    break;
                //------------------------------------------------------------
                case 'f':
                    draw_polystar_menu();
                    break;
                //------------------------------------------------------------
                case 'g':
                    draw_circular_arc_menu();
                    break;
                //------------------------------------------------------------
                case 'h':
                    draw_elliptical_arc_menu();
                    break;
                //------------------------------------------------------------
                case 'i':
                    draw_rhodonea_menu();
                    break;
                //------------------------------------------------------------
                case 'j':
                    draw_epicycloid_menu();
                    break;
                //------------------------------------------------------------
                case 'k':
                    draw_epitrochoid_menu();
                    break;
                //------------------------------------------------------------
                case 'l':
                    draw_hypocycloid_menu();
                    break;
                //------------------------------------------------------------
                case 'm':
                    draw_hypotrochoid_menu();
                    break;
                //------------------------------------------------------------
                case 'n':
                    draw_lissajous_menu();
                    break;
                //------------------------------------------------------------
                case 'o':
                    draw_harmonograph_menu();
                    break;
                //------------------------------------------------------------
                case 'O':
                    draw_animated_harmonograph_menu();
                    break;
                //------------------------------------------------------------
                case 'q':
                    draw_mono_spaced_font_menu();
                    break;
                //------------------------------------------------------------
                case 'r':
                    draw_variable_spaced_font_menu();
                    break;
                //------------------------------------------------------------
                case 'p':
                    space.selected_color_index++;
                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(space.selected_color_index >= space.target_palette().size())
                            space.selected_color_index = 0;
                    }
                    else
                    {
                        if(space.selected_color_index >= space.current_palette().size())
                            space.selected_color_index = 0;
                    }
                    space.TUI_clue = "+ color index";
                    break;
                //------------------------------------------------------------
                case 'P':

                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(space.selected_color_index == 0)
                            space.selected_color_index = space.target_palette().size();
                    }
                    else
                    {
                        if(space.selected_color_index == 0)
                            space.selected_color_index = space.current_palette().size();
                    }
                    space.selected_color_index--;
                    space.TUI_clue = "- color index";
                    break;
                //------------------------------------------------------------
                case 'z': // +100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(    space.loop_animation
                            || (space.frame_index <= (space.number_of_frames() - 101))
                          )
                            space.frame_index += 100;
                        while(space.frame_index >= space.number_of_frames())
                            space.frame_index = space.frame_index - space.number_of_frames();
                        space.TUI_clue = "+100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case 'Z': // -100 frames
                    if(space.number_of_frames() > 100)
                    {
                        if(space.frame_index >= 100)
                            space.frame_index -= 100;
                        else if(    space.loop_animation
                                 && (space.frame_index < 100)
                               )
                        {
                            space.frame_index = space.number_of_frames() + space.frame_index;
                            space.frame_index -= 100;
                        }
                        space.TUI_clue = "-100 frames";
                    }
                    break;
                //------------------------------------------------------------
                case '/': // show frame_set stats
                    space.show_frame_set_stats = !space.show_frame_set_stats;
                    break;
                //------------------------------------------------------------
                case '?': // show frame stats
                    space.show_frame_stats = !space.show_frame_stats;
                    break;
                //------------------------------------------------------------
                case ';': // toggle bitmap background
                    space.show_background_bitmap = !space.show_background_bitmap;
                    if(space.show_background_bitmap)
                        space.TUI_clue = "show bitmap";
                    else
                        space.TUI_clue = "hide bitmap";
                    break;
                //------------------------------------------------------------
                case ':': // negative bitmap background
                    space.show_background_bitmap = true;
                    space.background_bmp_negative = !space.background_bmp_negative;
                    space.reload_background_bitmap();
                    space.TUI_clue = "negative bitmap";
                    break;
                //------------------------------------------------------------
                case '`':
                    space.copy_frame();
                    space.TUI_clue = "copy frame";
                    break;
                //------------------------------------------------------------
                case '~':
                    space.new_frame();
                    space.TUI_clue = "new frame";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RETURN:
                    space.show_menu = !space.show_menu;
                    if(!space.show_menu)
                        space.TUI_clue = "[Enter] show menu";
                    else
                        space.TUI_clue = "[Enter] hide menu";
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end else if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    space.show_menu = show_menu_was;
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::effect_functions_menu()
{
    bool bad_key   = true;
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_effect_functions_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output file";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '1': // generate and save frame effect
                space.clean_screen();
                space.effect_name.clear();
                space.overlay_frame_effects();
                while(!space.f_effect_selected())
                {
                    display_prompt_f_effect_with_auto_complete("frame effect name");
                    if(prompt_escaped)
                        break;
                }
                if(space.effect_name.size() == 0)
                    break;
                space.TUI_clue = space.effect_name;
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = LASERBOY_NO_FILE_SAVED;
                display_space();
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && (    space.file_name.size() < 5
                         || !iends_with(space.file_name, space.current_extension)
                       )
                  )
                    space.file_name += ".ild";
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_effect();
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // generate and save selected frames effect
                if(space.number_of_selected_frames())
                {
                    space.clean_screen();
                    space.effect_name.clear();
                    space.overlay_selected_frames_effects();
                    while(!space.sf_effect_selected())
                    {
                      display_prompt_sf_effect_with_auto_complete("frame_set effect name");
                      if(prompt_escaped)
                          break;
                    }
                    if(space.effect_name.size() == 0)
                      break;
                    space.TUI_clue = space.effect_name;
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                    if(prompt_escaped)
                      break;
                    if(space.file_name.size() == 0)
                      space.file_name = LASERBOY_NO_FILE_SAVED;
                    display_space();
                    if(    space.file_name != LASERBOY_NO_FILE_SAVED
                      && (    space.file_name.size() < 5
                           || !iends_with(space.file_name, space.current_extension)
                         )
                    )
                      space.file_name += ".ild";
                    if(    space.file_name != LASERBOY_NO_FILE_SAVED
                      && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                      && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                    )
                    {
                      space.clean_screen();
                      space.render_space();
                      space.overlay_file_list();
                      space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                      wait_4_Esc();
                      break;
                    }
                    else
                    {
                      LaserBoy_Bounds out_of_bounds = space.call_selected_frames_effect();
                      if(out_of_bounds & LASERBOY_NOTHING_SELECTED)
                      {
                          space.display_error(space.effect_name + " requires selected frames");
                          wait_4_Esc();
                          break;
                      }
                      if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                      {
                          space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                          wait_4_Esc();
                          break;
                      }
                    }
                    bad_key = false;
                    break;
                }
                else
                {
                    space.display_error("requires selected frames");
                    wait_4_Esc();
                    break;
                }
            //----------------------------------------------------------------
            case '3': // generate and save frame_set effect
                space.clean_screen();
                space.effect_name.clear();
                space.overlay_frame_set_effects();
                while(!space.fs_effect_selected())
                {
                    display_prompt_fs_effect_with_auto_complete("frame_set effect name");
                    if(prompt_escaped)
                        break;
                }
                if(space.effect_name.size() == 0)
                    break;
                space.TUI_clue = space.effect_name;
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = LASERBOY_NO_FILE_SAVED;
                display_space();
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && (    space.file_name.size() < 5
                         || !iends_with(space.file_name, space.current_extension)
                       )
                  )
                    space.file_name += ".ild";
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_set_effect();
                    if(out_of_bounds & LASERBOY_NOTHING_SELECTED)
                    {
                        space.display_error(space.effect_name + " requires selected frames");
                        wait_4_Esc();
                        break;
                    }
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_point_menu()
{
    prompt_escaped = false;
    LaserBoy_vertex _0;
    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_short("X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_short("  Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.z = display_prompt_and_echo_short("    Z [0]", 0);
    if(prompt_escaped)
        return;
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _0.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _0.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _0.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _0.c = space.selected_color_index;
        _0.r = space.palette_picker(space.palette_index)[_0.c].r;
        _0.g = space.palette_picker(space.palette_index)[_0.c].g;
        _0.b = space.palette_picker(space.palette_index)[_0.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    space.current_frame() += _0;
    _0.unblank();
    space.current_frame() += _0;
    space.TUI_clue = "point";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_line_menu()
{
    LaserBoy_vertex _0,
                    _1;
    prompt_escaped = false;
    space.clean_screen();
    space.render_space();
    _0.x = display_prompt_and_echo_short("start X [-32767]", -32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_short("start   Y [0]");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.z = display_prompt_and_echo_short("start     Z [0]");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_short("end X [32767]", 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_short("end   Y [0]");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.z = display_prompt_and_echo_short("end     Z [0]");
    if(prompt_escaped)
        return;
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _1.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _1.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _1.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _1.c = space.selected_color_index;
        _1.r = space.palette_picker(space.palette_index)[_1.c].r;
        _1.g = space.palette_picker(space.palette_index)[_1.c].g;
        _1.b = space.palette_picker(space.palette_index)[_1.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    _1.unblank();
    space.current_frame() += _0;
    space.current_frame() += _1;
    space.TUI_clue = "line";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_rectangle_menu()
{
    LaserBoy_vertex _0,
                    _1,
                    _2;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    _0.x = display_prompt_and_echo_short("first corner X [-30000]", -30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_short("first corner   Y [10000]", 10000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_short("second corner X [30000]", 30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_short("second corner   Y [-10000]", -10000);
    if(prompt_escaped)
        return;
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _2.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _2.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _2.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _2.c = space.selected_color_index;
        _2.r = space.palette_picker(space.palette_index)[_2.c].r;
        _2.g = space.palette_picker(space.palette_index)[_2.c].g;
        _2.b = space.palette_picker(space.palette_index)[_2.c].b;
    }
    if(space.current_frame().size())
    {
        space.current_frame() += space.current_frame().back();
        space.current_frame().back().blank();
    }
    _0.blank();
    _2.unblank();
    space.current_frame() += _0;
    _2   = _0.as_3D_short();
    _2.x = _1.x;
    _2.z = _1.z;
    space.current_frame() += _2;
    _2   = _1.as_3D_short();
    space.current_frame() += _2;
    _2.x = _0.x;
    _2.z = _0.z;
    space.current_frame() += _2;
    _2   = _0.as_3D_short();
    space.current_frame() += _2;
    space.TUI_clue = "rectangle";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_polygon_menu()
{
    LaserBoy_3D_short center,
                      vertex;
    LaserBoy_segment  polygon;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]");
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    vertex.x = display_prompt_and_echo_short("first vertex X [30000]", 30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    vertex.y = display_prompt_and_echo_short("first vertex   Y [0]");
    if(prompt_escaped)
        return;
    u_int number_of_sides = 0;
    while(number_of_sides == 0 || number_of_sides == 1 || number_of_sides == 2)
    {
        number_of_sides = display_prompt_and_echo_u_int(   "number of sides [8] "
                                                         , 8
                                                         , 800
                                                       );
        if(prompt_escaped)
        {
            space.display_error("number of sides must be 3 or more!");
            return;
        }
    }
    polygon = LaserBoy_segment(center,
                               vertex,
                               number_of_sides
                              );
    polygon.palette_index = space.current_frame().palette_index;
    space.current_frame() += polygon;
    space.TUI_clue = "polygon";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_polyline_menu()
{
    int               number_of_vertecies = 0;
    char              text_number[16];
    LaserBoy_vertex   _0,
                      _1;
    _1.unblank();
    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
    {
        _1.r = space.palette_picker(space.target_palette_index)[space.selected_color_index].r;
        _1.g = space.palette_picker(space.target_palette_index)[space.selected_color_index].g;
        _1.b = space.palette_picker(space.target_palette_index)[space.selected_color_index].b;
    }
    else
    {
        _1.c = space.selected_color_index;
        _1.r = space.palette_picker(space.palette_index)[_1.c].r;
        _1.g = space.palette_picker(space.palette_index)[_1.c].g;
        _1.b = space.palette_picker(space.palette_index)[_1.c].b;
    }
    prompt_escaped = false;
    while(true)
    {
        sprintf(text_number, "%d", number_of_vertecies);
        space.clean_screen();
        space.render_space();
        _1.x = display_prompt_and_echo_short(string("vertex ") + text_number + " X [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;
        space.clean_screen();
        space.render_space();
        _1.y = display_prompt_and_echo_short(string("vertex ") + text_number + "   Y [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;
        space.clean_screen();
        space.render_space();
        _1.z = display_prompt_and_echo_short(string("vertex ") + text_number + "     Z [0] ([Esc] to finish)");
        if(prompt_escaped)
            break;
        space.current_frame() += _1;
        if(number_of_vertecies == 0)
        {
            _0 = _1;
            space.current_frame().back().blank();
        }
        number_of_vertecies++;
    }
    space.clean_screen();
    space.render_space();
    bool close_polyline = display_prompt_and_echo_bool("close polyline ? (y | [any])");
    if(prompt_escaped)
        return;
    if(close_polyline)
        space.current_frame() += _0;
    space.TUI_clue = "polyline";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_polystar_menu()
{
    LaserBoy_3D_short center,
                      vertex;
    LaserBoy_segment  polystar;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    vertex.x = display_prompt_and_echo_short("first vertex X [30000]", 30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    vertex.y = display_prompt_and_echo_short("first vertex   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int number_of_points = 0;
    while(number_of_points == 0 || number_of_points == 1 || number_of_points == 2)
    {
        number_of_points = display_prompt_and_echo_u_int(   "number of points [9]"
                                                          , 9
                                                          , 720
                                                        );
        if(prompt_escaped)
        {
            space.display_error("number of points must be 3 or more!");
            return;
        }
    }
    double ratio = display_prompt_and_echo_double(   "inner / outer ratio [0.5]"
                                                   , 0.5
                                                   , 1.0
                                                   , 0.0
                                                 );
    if(prompt_escaped)
        return;
    polystar = LaserBoy_segment(center,
                                vertex,
                                number_of_points,
                                ratio
                               );
    polystar.palette_index = space.current_frame().palette_index;
    space.current_frame() += polystar;
    space.TUI_clue = "polystar";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_circular_arc_menu()
{
    LaserBoy_3D_short center,
                      radius;
    LaserBoy_segment  arc;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    radius.x = display_prompt_and_echo_short("radius vertex X [32767]", 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    radius.y = display_prompt_and_echo_short("radius vertex  Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    double  arc_angle = display_prompt_and_echo_double(   "arc angle in degrees, (0 = 360) [0]"
                                                        , 0.0
                                                        , 360.0
                                                        , 0.0
                                                      );
    if(prompt_escaped)
        return;
    arc = LaserBoy_segment(center, radius, arc_angle);
    arc.palette_index = space.current_frame().palette_index;
    space.current_frame() += arc;
    space.TUI_clue = "circular arc";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_elliptical_arc_menu()
{
    LaserBoy_3D_short center,
                      radius;
    LaserBoy_segment  arc;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    radius.x = display_prompt_and_echo_short("radius vertex X [32767]", 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    radius.y = display_prompt_and_echo_short("radius vertex  Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    double  arc_angle = display_prompt_and_echo_double(   "arc angle in degrees, (0 = 360) [0]"
                                                        , 0.0
                                                        , 360.0
                                                        , 0.0
                                                      );
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    double radii_ratio = display_prompt_and_echo_double(   "minor / major radii ratio [0.5]"
                                                         , 0.5
                                                         , 1.0
                                                         , (1.0 / 32767.0)
                                                       );
    if(prompt_escaped)
        return;
    arc = LaserBoy_segment(center, radius, arc_angle, radii_ratio);
    arc.palette_index = space.current_frame().palette_index;
    space.current_frame() += arc;
    space.TUI_clue = "elliptical arc";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_rhodonea_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  rhodonea;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    int radius = display_prompt_and_echo_short("radius [30000]", 30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int rhodonea_numerator = 0;
    while(rhodonea_numerator == 0)
    {
        rhodonea_numerator = display_prompt_and_echo_u_int(   "pedal factor numerator [11]"
                                                           , 11
                                                           , UINT_MAX
                                                         );
        if(prompt_escaped)
        {
            space.display_error("pedal numerator must not be 0!");
            return;
        }
    }
    space.clean_screen();
    space.render_space();
    u_int rhodonea_denominator = 0;
    while(rhodonea_denominator == 0)
    {
        rhodonea_denominator = display_prompt_and_echo_u_int(   "pedal factor denominator [17]"
                                                             , 17
                                                             , UINT_MAX
                                                           );
        if(prompt_escaped)
        {
            space.display_error("pedal denominator must not be 0!");
            return;
        }
    }
    rhodonea = LaserBoy_segment(center,
                                radius,
                                rhodonea_numerator,
                                rhodonea_denominator
                               );
    rhodonea.palette_index = space.current_frame().palette_index;
    space.current_frame() += rhodonea;
    space.TUI_clue = "rhodonea";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_epicycloid_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  epicycloid;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int fixed_radius = display_prompt_and_echo_u_int("fixed circle radius [3000]", 3000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_radius = display_prompt_and_echo_u_int("rolling circle radius [10000]", 10000, 32767);
    if(prompt_escaped)
        return;

    epicycloid = LaserBoy_segment(fixed_radius,
                                  center,
                                  roller_radius,
                                  roller_radius
                                 );
    epicycloid.palette_index = space.current_frame().palette_index;
    space.current_frame() += epicycloid;
    space.TUI_clue = "epicycloid";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_epitrochoid_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  epitrochoid;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int fixed_radius = display_prompt_and_echo_u_int("fixed circle radius [3100]", 3100, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_radius = display_prompt_and_echo_u_int("rolling circle radius [10000]", 10000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_offset = display_prompt_and_echo_u_int("distance from rolling center [9000]", 9000, 32767);
    if(prompt_escaped)
        return;
    epitrochoid = LaserBoy_segment(fixed_radius,
                                   center,
                                   roller_radius,
                                   roller_offset
                                  );
    epitrochoid.palette_index = space.current_frame().palette_index;
    space.current_frame() += epitrochoid;
    space.TUI_clue = "epitrochoid";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_hypocycloid_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  hypocycloid;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int fixed_radius = display_prompt_and_echo_u_int("fixed circle radius [30000]", 30000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_radius = display_prompt_and_echo_u_int("rolling circle radius [13500]", 13500, 32767);
    if(prompt_escaped)
        return;
    hypocycloid = LaserBoy_segment(fixed_radius,
                                   roller_radius,
                                   roller_radius,
                                   center
                                  );
    hypocycloid.palette_index = space.current_frame().palette_index;
    space.current_frame() += hypocycloid;
    space.TUI_clue = "hypocycloid";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_hypotrochoid_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  hypotrochoid;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int fixed_radius = display_prompt_and_echo_u_int("fixed circle radius [15000]", 15000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_radius = display_prompt_and_echo_u_int("rolling circle radius [8000]", 8000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int roller_offset = display_prompt_and_echo_u_int("distance from rolling center [11000]", 11000, 32767);
    if(prompt_escaped)
        return;
    hypotrochoid = LaserBoy_segment(fixed_radius,
                                    roller_radius,
                                    roller_offset,
                                    center
                                   );
    hypotrochoid.palette_index = space.current_frame().palette_index;
    space.current_frame() += hypotrochoid;
    space.TUI_clue = "hypotrochoid";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_lissajous_menu()
{
    LaserBoy_3D_short center;
    LaserBoy_segment  lissaju;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    center.x = display_prompt_and_echo_short("center X [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.y = display_prompt_and_echo_short("center   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    center.z = display_prompt_and_echo_short("center     Z [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int amplitude_x = display_prompt_and_echo_u_int("amplitude in X [30000]", 30000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int amplitude_y = display_prompt_and_echo_u_int("amplitude in Y [30000]", 30000, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int frequency_x = display_prompt_and_echo_u_int("frequency in X [4]", 4, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    u_int frequency_y = display_prompt_and_echo_u_int("frequency in Y [5]", 5, 32767);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    double phase_x = display_prompt_and_echo_double(   "phase shift in X [0] degrees"
                                                     , 0.0
                                                     , 360.0
                                                     , 0.0
                                                   );
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    double phase_y = display_prompt_and_echo_double(   "phase shift in Y [0] degrees"
                                                     , 0.0
                                                     , 360.0
                                                     , 0.0
                                                   );
    if(prompt_escaped)
        return;
    lissaju = LaserBoy_segment(center,
                               amplitude_x,
                               amplitude_y,
                               frequency_x,
                               frequency_y,
                               phase_x,
                               phase_y
                              );
    lissaju.palette_index = space.current_frame().palette_index;
    space.current_frame() += lissaju;
    space.TUI_clue = "Lissajous curv";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_harmonograph_menu()
{
    u_int               n;
    LaserBoy_oscillator LBO1,
                        LBO2,
                        LBO3,
                        LBO4;
    double              duration;
    LaserBoy_segment    harmonograph;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    //------------------------------------------------------------------------
    LBO1.amplitude = display_prompt_and_echo_double("Amplitude 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO1, "P1", 1);
    LBO1.frequency = display_prompt_and_echo_double("Frequency 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO1, "P1", 1);
    LBO1.phase = display_prompt_and_echo_double("Phase 1 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO1, "P1", 1);
    LBO1.damping = display_prompt_and_echo_double("Damping factor 1 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO1, "P1", 1);
    //------------------------------------------------------------------------
    LBO2.amplitude = display_prompt_and_echo_double("Amplitude 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO2, "P2", 2);
    LBO2.frequency = display_prompt_and_echo_double("Frequency 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO2, "P2", 2);
    LBO2.phase = display_prompt_and_echo_double("Phase 2 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO2, "P2", 2);
    LBO2.damping = display_prompt_and_echo_double("Damping factor 2 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO2, "P2", 2);
    //------------------------------------------------------------------------
    LBO3.amplitude = display_prompt_and_echo_double("Amplitude 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO3, "P3", 3);
    LBO3.frequency = display_prompt_and_echo_double("Frequency 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO3, "P3", 3);
    LBO3.phase = display_prompt_and_echo_double("Phase 3 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO3, "P3", 3);
    LBO3.damping = display_prompt_and_echo_double("Damping factor 3 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO3, "P3", 3);
    //------------------------------------------------------------------------
    LBO4.amplitude = display_prompt_and_echo_double("Amplitude 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO4, "P4", 4);
    LBO4.frequency = display_prompt_and_echo_double("Frequency 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO4, "P4", 4);
    LBO4.phase = display_prompt_and_echo_double("Phase 4 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO4, "P4", 4);
    LBO4.damping = display_prompt_and_echo_double("Damping factor 4 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.render_space();
    space.overlay_oscillator_stats(LBO4, "P4", 4);
    //------------------------------------------------------------------------
    duration = display_prompt_and_echo_double("swing time [300]", 300.0);
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    n = display_prompt_and_echo_u_int("iterations [30000]", 30000);
    if(prompt_escaped)
        return;
    if(prompt_escaped)
        return;
    harmonograph = LaserBoy_segment(LBO1, LBO2, LBO3, LBO4, n, duration);
    space += harmonograph;
    space.TUI_clue = "harmonograph curv";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_animated_harmonograph_menu()
{
    u_int                      n,
                               frames;
    LaserBoy_oscillator        LBO1,
                               LBO2,
                               LBO3,
                               LBO4,
                              _LBO1,
                              _LBO2,
                              _LBO3,
                              _LBO4;
    double                     duration;
    LaserBoy_real_segment_set  animated_harmonograph;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    //------------------------------------------------------------------------
    LBO1.amplitude = display_prompt_and_echo_double("Amplitude 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO1, "LBO1 start", 1);
    LBO1.frequency = display_prompt_and_echo_double("Frequency 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO1, "LBO1 start", 1);
    LBO1.phase = display_prompt_and_echo_double("Phase 1 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO1, "LBO1 start", 1);
    LBO1.damping = display_prompt_and_echo_double("Damping factor 1 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO1, "LBO1 start", 1);
    //------------------------------------------------------------------------
    LBO2.amplitude = display_prompt_and_echo_double("Amplitude 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO2, "LBO2 start", 2);
    LBO2.frequency = display_prompt_and_echo_double("Frequency 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO2, "LBO2 start", 2);
    LBO2.phase = display_prompt_and_echo_double("Phase 2 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO2, "LBO2 start", 2);
    LBO2.damping = display_prompt_and_echo_double("Damping factor 2 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO2, "LBO2 start", 2);
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(LBO3, "LBO3 start", 3);
    LBO3.amplitude = display_prompt_and_echo_double("Amplitude 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO3, "LBO3 start", 3);
    LBO3.frequency = display_prompt_and_echo_double("Frequency 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO3, "LBO3 start", 3);
    LBO3.phase = display_prompt_and_echo_double("Phase 3 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO3, "LBO3 start", 3);
    LBO3.damping = display_prompt_and_echo_double("Damping factor 3 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO3, "LBO3 start", 3);
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(LBO4, "LBO4 start", 4);
    LBO4.amplitude = display_prompt_and_echo_double("Amplitude 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO4, "LBO4 start", 4);
    LBO4.frequency = display_prompt_and_echo_double("Frequency 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO4, "LBO4 start", 4);
    LBO4.phase = display_prompt_and_echo_double("Phase 4 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO4, "LBO4 start", 4);
    LBO4.damping = display_prompt_and_echo_double("Damping factor 4 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(LBO4, "LBO4 start", 4);
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(_LBO1, "LBO1 end  ", 6);
    _LBO1.amplitude = display_prompt_and_echo_double("Amplitude 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO1, "LBO1 end  ", 6);
    _LBO1.frequency = display_prompt_and_echo_double("Frequency 1 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO1, "LBO1 end  ", 6);
    _LBO1.phase = display_prompt_and_echo_double("Phase 1 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO1, "LBO1 end  ", 6);
    _LBO1.damping = display_prompt_and_echo_double("Damping factor 1 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO1, "LBO1 end  ", 6);
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(_LBO2, "LBO2 end  ", 7);
    _LBO2.amplitude = display_prompt_and_echo_double("Amplitude 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO2, "LBO2 end  ", 7);
    _LBO2.frequency = display_prompt_and_echo_double("Frequency 2 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO2, "LBO2 end  ", 7);
    _LBO2.phase = display_prompt_and_echo_double("Phase 2 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO2, "LBO2 end  ", 7);
    _LBO2.damping = display_prompt_and_echo_double("Damping factor 2 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO2, "LBO2 end  ", 7);
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(_LBO3, "LBO3 end  ", 8);
    _LBO3.amplitude = display_prompt_and_echo_double("Amplitude 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO3 end  ", 8);
    _LBO3.frequency = display_prompt_and_echo_double("Frequency 3 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO3 end  ", 8);
    _LBO3.phase = display_prompt_and_echo_double("Phase 3 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO3 end  ", 8);
    _LBO3.damping = display_prompt_and_echo_double("Damping factor 3 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO3 end  ", 8);
    //------------------------------------------------------------------------
    space.overlay_oscillator_stats(_LBO3, "LBO4 end  ", 9);
    _LBO4.amplitude = display_prompt_and_echo_double("Amplitude 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO4 end  ", 9);
    _LBO4.frequency = display_prompt_and_echo_double("Frequency 4 [1.0]", 1.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO4 end  ", 9);
    _LBO4.phase = display_prompt_and_echo_double("Phase 4 in degrees [0.0]", 0.0);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO4 end  ", 9);
    _LBO4.damping = display_prompt_and_echo_double("Damping factor 4 [0.001]", 0.001);
    if(prompt_escaped)
        return;
    space.overlay_oscillator_stats(_LBO3, "LBO4 end  ", 9);
    //------------------------------------------------------------------------
    duration = display_prompt_and_echo_double("swing time [300]", 300.0);
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    n = display_prompt_and_echo_u_int("iterations per frame [30000]", 30000);
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    frames = display_prompt_and_echo_u_int("number of frames [400]", 400);
    if(prompt_escaped)
        return;
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    animated_harmonograph = LaserBoy_real_segment_set::_harmonograph(LBO1,
                                                                     LBO2,
                                                                     LBO3,
                                                                     LBO4,
                                                                     LBO1,
                                                                     LBO2,
                                                                     LBO3,
                                                                     LBO4,
                                                                     0.0,
                                                                     duration,
                                                                     n,
                                                                     _LBO1,
                                                                     _LBO2,
                                                                     _LBO3,
                                                                     _LBO4,
                                                                     _LBO1,
                                                                     _LBO2,
                                                                     _LBO3,
                                                                     _LBO4,
                                                                     0.0,
                                                                     duration,
                                                                     n,
                                                                     frames
                                                                    );
    animated_harmonograph.normalize_vectors();
    space += animated_harmonograph;
    space.TUI_clue = "animated harmonograph";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_mono_spaced_font_menu()
{
    char buffer[30];
    string prompt;
    LaserBoy_3D_short _0,
                      _1;
    LaserBoy_segment  segment;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    _0.x = display_prompt_and_echo_short("baseline start X [-30000]", -30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_short("baseline start   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_short("baseline end X [30000]", 30000);
    if(prompt_escaped)
        return;
    sprintf(buffer, "%d", _0.y);
    prompt = "baseline end   Y [";
    prompt.append(buffer);
    prompt.append("]");
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_short(prompt.c_str(), _0.y);
    if(prompt_escaped)
        return;
    space.get_file_list(LASERBOY_ILD_SHARE + string("fonts/"), ".ild");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("font file name ? [lucida.ild]", "lucida.ild");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    else if(space.file_name.size() < 5 || !iends_with(space.file_name, ".ild"))
        space.file_name += ".ild";
    //------------------------------------------------------------------------
    space.clean_screen();
    space.render_space();
    string text = display_prompt_and_echo_string("text to render");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    segment = LaserBoy_segment(_0,
                               _1,
                               LASERBOY_ILD_SHARE + "fonts/" + space.file_name,
                               text
                               );
    segment.palette_index = space.current_frame().palette_index;
    space.current_frame() += segment;
    //------------------------------------------------------------------------
    space.TUI_clue = "text rendered";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::draw_variable_spaced_font_menu()
{
    char buffer[30];
    string prompt;
    LaserBoy_3D_short _0,
                      _1;
    LaserBoy_segment  segment;
    space.clean_screen();
    space.render_space();
    prompt_escaped = false;
    _0.x = display_prompt_and_echo_short("baseline start X [-30000]", -30000);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _0.y = display_prompt_and_echo_short("baseline start   Y [0]", 0);
    if(prompt_escaped)
        return;
    space.clean_screen();
    space.render_space();
    _1.x = display_prompt_and_echo_short("baseline end X [30000]", 30000);
    if(prompt_escaped)
        return;
    sprintf(buffer, "%d", _0.y);
    prompt = "baseline end   Y [";
    prompt.append(buffer);
    prompt.append("]");
    space.clean_screen();
    space.render_space();
    _1.y = display_prompt_and_echo_short(prompt.c_str(), _0.y);
    if(prompt_escaped)
        return;
    space.get_file_list(LASERBOY_ILD_SHARE + string("fonts/"), ".ild");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("font file name ? [wide_vector.ild]", "wide_vector.ild");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    else if(space.file_name.size() < 5 || !iends_with(space.file_name, ".ild"))
        space.file_name += ".ild";
    //------------------------------------------------------------------------
    space.clean_screen();
    space.render_space();
    string text = display_prompt_and_echo_string("text to render");
    if(prompt_escaped)
        return;
    //------------------------------------------------------------------------
    segment = LaserBoy_segment(_0,
                               _1,
                               LASERBOY_ILD_SHARE + string("fonts/") + space.file_name,
                               text,
                               0
                              );
    segment.palette_index = space.current_frame().palette_index;
    space.current_frame() += segment;
    //------------------------------------------------------------------------
    space.TUI_clue = "text rendered";
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::wave_utilities_menu()
{
    bool in_menu   = true,
         good_call = false;
    //------------------------------------------------------------------------
    prompt_escaped = false;
    while(in_menu)
    {
        space.set_current_menu_name("wave utilities", 0x00002040);
        space.clean_screen();
        space.overlay_wave_utilities_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit wave utilities";
                    break;
                //------------------------------------------------------------
                case '1': // wave utilities settings
                    space.set_current_menu_name("wave show settings", 0x00002040, true);
                    wave_show_settings_menu();
                    break;
                //------------------------------------------------------------
                case '2': // show wave on screen
                    space.set_current_menu_name("wave show options", 0x00002040, true);
                    show_wave_options_menu();
                    break;
                //------------------------------------------------------------
                case '3': // set LaserBoy wave offsets
                    space.set_current_menu_name("set system offsets", 0x00002040, true);
                    set_wave_offsets_menu();
                    break;
                //------------------------------------------------------------
                case '4': // apply LaserBoy wave sample offsets
                    space.set_current_menu_name("apply system offsets", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to apply offsets");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    display_space();
                    if(space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                        if(good_call)
                            display_space();
                        else
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '5': // overwrite LaserBoy wave sample offsets
                    space.set_current_menu_name("overwrite system offsets", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to overwrite offsets");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    display_space();
                    if(space.overwirte_wave_offsets(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                        if(good_call)
                            display_space();
                        else
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '6': // set color rescale files
                    space.set_current_menu_name("set color rescales", 0x00002040, true);
                    set_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '7': // apply color rescales to wave
                    space.set_current_menu_name("apply color rescales", 0x00002040, true);
                    apply_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '8': // omit color rescal tables in header
                    space.set_current_menu_name("remove color rescales", 0x00002040, true);
                    omit_color_rescales_menu();
                    break;
                //------------------------------------------------------------
                case '9': // save color rescal tables
                    space.set_current_menu_name("saving rescales", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to save color rescales");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    display_space();
                    if(space.save_color_rescales(LASERBOY_WAV_SHARE     + space.file_name,
                                                 LASERBOY_RESCALE_SHARE + space.file_name.substr(0, space.file_name.size() - 4))
                                                )
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                        if(good_call)
                            display_space();
                        else
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case '0': // impose byte value colors and on wave
                    space.set_current_menu_name("black level to wave", 0x00002040, true);
                    black_level_to_wave_menu();
                    break;
                //------------------------------------------------------------
                case 'a': // impose bit resolution reduction on wave
                    space.set_current_menu_name("bit resolution to wave", 0x00002040, true);
                    bit_resolution_to_wave_menu();
                    break;
                //------------------------------------------------------------
                case 'b': // split or join waves menu
                    space.set_current_menu_name("split or join waves", 0x00002040, true);
                    split_or_join_waves_menu();
                    break;
                //------------------------------------------------------------
                case 'c': // invert LaserBoy signal polarities
                    space.set_current_menu_name("set signal polarities", 0x00002040, true);
                    set_signal_polarities_menu();
                    break;
                //------------------------------------------------------------
                case 'd': // clear LaserBoy wave polarity matrix
                    space.set_current_menu_name("clear polarities list", 0x00002040, true);
                    clear_wave_polarities_menu();
                    break;
                //------------------------------------------------------------
                case 'e': // invert LaserBoy wave polarity
                    space.set_current_menu_name("invert wave", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to globally invert");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    display_space();
                    if(space.invert_wave_prep(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                        if(good_call)
                            display_space();
                        else
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'E': // invert unformatted wave polarity
                    space.set_current_menu_name("invert wave", 0x00002040, true);
                    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to globally invert");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    display_space();
                    if(space.invert_wave_prep(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        space.TUI_clue = "wave stats";
                        space.clean_screen();
                        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                        if(good_call)
                            display_space();
                        else
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'f': // Show LaserBoy wave stats
                    space.set_current_menu_name("show stats", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("LaserBoy wave file name to see stats");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
                    if(good_call)
                        display_space();
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'F': // Show unformatted wave stats
                    space.set_current_menu_name("show stats", 0x00002040, true);
                    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("unformatted wave file name to see stats");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.clean_screen();
                    good_call = space.overlay_wave_header_stats(LASERBOY_UNFORMATTED_SHARE + space.file_name);
                    if(good_call)
                        display_space();
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'g': // analyze LaserBoy wave
                    space.set_current_menu_name("analyse wave", 0x00002040, true);
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("LaserBoy wave file name to analyze");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.TUI_clue = "wave analysis";
                    space.clean_screen();
                    good_call = space.overlay_wave_analysis(LASERBOY_WAV_SHARE + space.file_name);
                    if(good_call)
                        display_space();
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
                case 'G': // analyze unfortmatted wave
                    space.set_current_menu_name("analyse wave", 0x00002040, true);
                    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("unformatted wave file name to analyze");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    space.TUI_clue = "wave analysis";
                    space.clean_screen();
                    good_call = space.overlay_wave_analysis(LASERBOY_UNFORMATTED_SHARE + space.file_name);
                    if(good_call)
                        display_space();
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::split_or_join_waves_menu()
{
    bool   in_menu = true;
    space.TUI_clue = "split or join waves";
    prompt_escaped = false;
    while(in_menu)
    {
        space.clean_screen();
        space.overlay_show_split_or_join_waves_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    in_menu = false;
                    space.TUI_clue = "exit split or join waves";
                    break;
                //------------------------------------------------------------
                case '1': // split wave
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to split");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "splitting wave";
                    display_space();
                    if(space.split_wave_XY_rg_bi_LR_prep(space.file_name,
                                                         LASERBOY_WAV_SHARE,
                                                         LASERBOY_UNFORMATTED_SHARE,
                                                         LASERBOY_AUDIO_SHARE
                                                        )
                      )
                    {
                        space.TUI_clue = "split wave";
                        display_space();
                    }
                    else
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case '2': // split wave
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to split");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    if(prompt_escaped)
                        break;
                    space.TUI_clue = "splitting wave";
                    display_space();
                    if(space.split_wave_XY_r_g_b_i_LR_prep(space.file_name,
                                                           LASERBOY_WAV_SHARE,
                                                           LASERBOY_UNFORMATTED_SHARE,
                                                           LASERBOY_AUDIO_SHARE
                                                          )
                      )
                    {
                        space.TUI_clue = "split wave";
                        display_space();
                    }
                    else
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                    }
                    break;
                //------------------------------------------------------------
                case '3': // add audio to wave
                    add_audio_to_wave_menu();
                    break;
                //------------------------------------------------------------


                //------------------------------------------------------------
                case '4': // join waves
                    join_XY_r_g_b_i_waves_menu();
                    break;
                //------------------------------------------------------------
                case '5': // join waves
                    join_XY_rg_b_waves_menu();
                    break;
                //------------------------------------------------------------
                case '6': // join waves
                    join_XY_rg_b_LR_waves_menu();
                    break;
                //------------------------------------------------------------


                //------------------------------------------------------------
                case '7': // join waves
                    break;
                //------------------------------------------------------------
                case '8': // join waves
                    break;
                //------------------------------------------------------------
                case '9': // join waves
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::wave_show_settings_menu()
{
    bool   in_menu         = true;
    //------------------------------------------------------------------------
    while(in_menu)
    {
        space.set_current_menu_name("wave show settings", 0x00002040);

        space.clean_screen();
        space.overlay_wave_show_settings_menu();
        display_space();
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                case LASERBOY_KEY_ESCAPE:
                    space.TUI_clue = "exit wave show settings";
                    in_menu = false;
                    break;
                //------------------------------------------------------------
                case '1':
                    space.wave_show_vertices = !space.wave_show_vertices;
                    if(space.wave_show_vertices)
                        space.TUI_clue = "show vertices";
                    else
                        space.TUI_clue = "hide vertices";
                    break;
                //------------------------------------------------------------
                case '2':
                    space.wave_show_blanks = !space.wave_show_blanks;
                    if(space.wave_show_blanks)
                        space.TUI_clue = "show blanking";
                    else
                        space.TUI_clue = "hide blanking";
                    break;
                //------------------------------------------------------------
                case '3':
                    space.wave_real_time = !space.wave_real_time;
                    if(space.wave_real_time)
                        space.TUI_clue = "real time";
                    else
                        space.TUI_clue = "as fast as possible";
                    break;
                //------------------------------------------------------------
                case '4':
                    space.wave_show_inverted = !space.wave_show_inverted;
                    if(space.wave_show_inverted)
                        space.TUI_clue = "inverted";
                    else
                        space.TUI_clue = "non-inverted";
                    break;
                //------------------------------------------------------------
                case '5':
                    space.wave_clean_screen = !space.wave_clean_screen;
                    if(space.wave_clean_screen)
                        space.TUI_clue = "clean screen";
                    else
                        space.TUI_clue = "no clean screen";
                    break;
                //------------------------------------------------------------
                case '6':
                    space.wave_loop = !space.wave_loop;
                    if(space.wave_loop)
                        space.TUI_clue = "loop";
                    else
                        space.TUI_clue = "no loop";
                    break;
            } // end switch(event.key.keysym.unicode)
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(in_menu)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::show_wave_options_menu()
{
    while(true)
    {
        space.set_current_menu_name("wave show options", 0x00002040, true);
        space.clean_screen();
        space.overlay_show_wave_options_menu();
        display_space();
        prompt_escaped = false;
        //----------------------------------------------------------------
        wait_for_gui_event(&event);
        if(event.type == LASERBOY_GUI_KEYDOWN)
        {
            switch(event.key.keysym.unicode)
            {
                default:
                    space.TUI_clue = "bad key";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_RSHIFT:
                case LASERBOY_KEY_LSHIFT:
                    space.TUI_clue = "";
                    break;
                //------------------------------------------------------------
                case LASERBOY_KEY_ESCAPE:
                    space.TUI_clue = "exit wave show options";
                    return;
                //------------------------------------------------------------
                case '1': // show LaserBoy wave
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to show");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    if(display_wave(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        while(space.wave_loop)
                        {
                            display_wave(LASERBOY_WAV_SHARE + space.file_name);
                            poll_gui_event(&event);
                            if(event.type == LASERBOY_GUI_KEYDOWN)
                                break;
                        }
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    break;
                //------------------------------------------------------------
                case '2': // show LaserBoy wave as unformatted
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to show");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    if(display_unframed_wave(LASERBOY_WAV_SHARE + space.file_name))
                    {
                        while(space.wave_loop)
                        {
                            display_unframed_wave(LASERBOY_WAV_SHARE + space.file_name);
                            poll_gui_event(&event);
                            if(event.type == LASERBOY_GUI_KEYDOWN)
                                break;
                        }
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    break;
                //------------------------------------------------------------
                case '3': // show unformatted wave
                    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("wave file name to show");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".wav";
                    if(display_unframed_wave(LASERBOY_UNFORMATTED_SHARE + space.file_name))
                    {
                        while(space.wave_loop)
                        {
                            display_unframed_wave(LASERBOY_UNFORMATTED_SHARE + space.file_name);
                            poll_gui_event(&event);
                            if(event.type == LASERBOY_GUI_KEYDOWN)
                                break;
                        }
                    }
                    else
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    break;
                //------------------------------------------------------------
            } // end switch(event.key.keysym.unicode)
            wait_4_Esc();
        } // end if(event.type == LASERBOY_GUI_KEYDOWN)
    } // end while(true)
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::set_wave_offsets_menu()
{
    int temp_int   = 0;
    prompt_escaped = false;
    //------------------------------------------------------------------------
    do
    {
        space.clean_screen();
        space.overlay_wave_offsets(0);
        temp_int = 0;
        temp_int = display_prompt_and_echo_int("wave sample offset for X");
        if(prompt_escaped)
            break;
        space.wave_offsets[0] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(1);
        temp_int = display_prompt_and_echo_int("wave sample offset for Y");
        if(prompt_escaped)
            break;
        space.wave_offsets[1] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(2);
        temp_int = display_prompt_and_echo_int("wave sample offset for red");
        if(prompt_escaped)
            break;
        space.wave_offsets[2] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(3);
        temp_int = display_prompt_and_echo_int("wave sample offset for green");
        if(prompt_escaped)
            break;
        space.wave_offsets[3] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(4);
        temp_int = display_prompt_and_echo_int("wave sample offset for blue");
        if(prompt_escaped)
            break;
        space.wave_offsets[4] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(5);
        temp_int = display_prompt_and_echo_int("wave sample offset for auxiliary signal");
        if(prompt_escaped)
            break;
        space.wave_offsets[5] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(6);
        temp_int = display_prompt_and_echo_int("wave sample offset for left audio");
        if(prompt_escaped)
            break;
        space.wave_offsets[6] = temp_int;
        //--------------------------------------------------------------------
        space.clean_screen();
        space.overlay_wave_offsets(7);
        temp_int = display_prompt_and_echo_int("wave sample offset for right audio");
        if(prompt_escaped)
            break;
        space.wave_offsets[7] = temp_int;
        //--------------------------------------------------------------------
    } while(0);
    space.clean_screen();
    space.overlay_wave_offsets(8);
    display_space();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::set_color_rescales_menu()
{
    LaserBoy_Rescale_Error_Code  rescale_file_error;
    space.get_file_list(LASERBOY_RESCALE_SHARE, ".txt");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("color rescale file for red", ".");
        if(prompt_escaped)
            break;
        if(space.file_name.size() == 0 || space.file_name == ".")
            break;
    }
    if(space.file_name.size() == 0)
        return;
    else if(space.file_name == ".")
        space.color_rescale_file[0] = ".";
    else
    {
        if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
            space.file_name += ".txt";
        space.color_rescale_file[0] = LASERBOY_RESCALE_SHARE + space.file_name;
        if((rescale_file_error = space.load_color_rescale_file(0)))
        {
            space.display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("color rescale file for green", ".");
        if(prompt_escaped)
            break;
        if(space.file_name.size() == 0 || space.file_name == ".")
            break;
    }
    if(space.file_name.size() == 0)
        return;
    else if(space.file_name == ".")
        space.color_rescale_file[1] = ".";
    else
    {
        if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
            space.file_name += ".txt";
        space.color_rescale_file[1] = LASERBOY_RESCALE_SHARE + space.file_name;
        if((rescale_file_error = space.load_color_rescale_file(1)))
        {
            space.display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("color rescale file for blue", ".");
        if(prompt_escaped)
            break;
        if(space.file_name.size() == 0 || space.file_name == ".")
            break;
    }
    if(space.file_name.size() == 0)
        return;
    else if(space.file_name == ".")
        space.color_rescale_file[2] = ".";
    else
    {
        if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
            space.file_name += ".txt";
        space.color_rescale_file[2] = LASERBOY_RESCALE_SHARE + space.file_name;
        if((rescale_file_error = space.load_color_rescale_file(2)))
        {
            space.display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    space.overlay_color_rescale_stats();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("color rescale file for auxilary", ".");
        if(prompt_escaped)
            break;
        if(space.file_name.size() == 0 || space.file_name == ".")
            break;
    }
    if(space.file_name.size() == 0)
        return;
    else if(space.file_name == ".")
        space.color_rescale_file[3] = ".";
    else
    {
        if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
            space.file_name += ".txt";
        space.color_rescale_file[3] = LASERBOY_RESCALE_SHARE + space.file_name;
        if((rescale_file_error = space.load_color_rescale_file(3)))
        {
            space.display_rescale_file_error(rescale_file_error);
            wait_4_Esc();
        }
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_color_rescale_stats();
    display_space();
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::apply_color_rescales_menu()
{
    bool good_call = false;
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to apply color rescales");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    //------------------------------------------------------------------------
    space.clean_screen();
    display_space();
    if(space.apply_color_rescales_prep(LASERBOY_WAV_SHARE + space.file_name))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
        if(good_call)
            display_space();
        else
            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::omit_color_rescales_menu()
{
    bool good_call = false;
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to remove color rescales");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    //------------------------------------------------------------------------
    space.clean_screen();
    display_space();
    if(space.omit_color_rescales(LASERBOY_WAV_SHARE + space.file_name))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        good_call = space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name);
        if(good_call)
            display_space();
        else
            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::set_signal_polarities_menu()
{
    short  wave_channels;
    //------------------------------------------------------------------------
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to signal invert");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + space.file_name))
    {
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    for(u_int i = 0; i < 8; i++)
        space.signal_polarity[i] = -1; // not set!
    //------------------------------------------------------------------------
    wave_channels = space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
    if(wave_channels)
    {
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[0] = display_prompt_and_echo_bool("invert X ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[1] = display_prompt_and_echo_bool("invert Y ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[2] = display_prompt_and_echo_bool("invert red ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[3] = display_prompt_and_echo_bool("invert green ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[4] = display_prompt_and_echo_bool("invert blue ? (y | [any])");
        if(prompt_escaped)
            return;
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        space.signal_polarity[5] = display_prompt_and_echo_bool("invert Z or other ? (y | [any])");
        if(prompt_escaped)
            return;
        if(wave_channels > 7)
        {
            space.clean_screen();
            space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
            space.signal_polarity[6] = display_prompt_and_echo_bool("invert L audio ? (y | [any])");
            if(prompt_escaped)
                return;
            space.clean_screen();
            space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
            space.signal_polarity[7] = display_prompt_and_echo_bool("invert R audio ? (y | [any])");
            if(prompt_escaped)
                return;
        }
        space.clean_screen();
        space.overlay_wave_signal_polarities(LASERBOY_WAV_SHARE + space.file_name, space.signal_polarity);
        display_space();
        laserboy_gui_delay(250);
        space.TUI_clue = "inverting signals";
        display_space();
        //----------------------------------------------------------------
        space.invert_signals_prep(LASERBOY_WAV_SHARE + space.file_name);
        //----------------------------------------------------------------
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
        else
            display_space();
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::clear_wave_polarities_menu()
{
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to clear");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + space.file_name))
    {
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    bool global_flip_flop = display_prompt_and_echo_bool("flip global polarity status? (y | [any])");
    if(prompt_escaped)
        return;

    space.TUI_clue = "clearing polarities";
    display_space();
    //------------------------------------------------------------------------
    if(space.clear_wave_polarity_list(LASERBOY_WAV_SHARE + space.file_name, global_flip_flop))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
        else
            display_space();
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::black_level_to_wave_menu()
{
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to black level");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_in = space.file_name;
    if(file_in.size() < 5 || file_in.substr(file_in.size() - 4) != ".wav")
        file_in += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_in))
    {
        space.display_error(LASERBOY_WAV_SHARE + file_in + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for output wave", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    else if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    space.TUI_clue = "black leveling";
    display_space();
    //------------------------------------------------------------------------
    if(space.black_level_to_wave_prep(LASERBOY_WAV_SHARE + file_in, LASERBOY_WAV_SHARE + file_out))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::bit_resolution_to_wave_menu()
{
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name to bit reduce");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_in = space.file_name;
    if(file_in.size() < 5 || file_in.substr(file_in.size() - 4) != ".wav")
        file_in += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_in))
    {
        space.display_error(LASERBOY_WAV_SHARE + file_in + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for output wave", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    space.TUI_clue = "bit reducing";
    display_space();
    //------------------------------------------------------------------------
    if(space.bit_resolution_to_wave_prep(LASERBOY_WAV_SHARE + file_in, LASERBOY_WAV_SHARE + file_out))
    {
        space.TUI_clue = "wave stats";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::join_XY_r_g_b_i_waves_menu()
{
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    //------------------------------------------------------------------------
    space.TUI_clue = "join XY r g b i";
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("XY wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_XY = space.file_name;
    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("red wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_r = space.file_name;
    if(file_r.size()   < 5 || file_r.substr(file_r.size() - 4) != ".wav")
        file_r   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_r))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_r + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("green wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_g = space.file_name;
    if(file_g.size()   < 5 || file_g.substr(file_g.size() - 4) != ".wav")
        file_g   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_g))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_g + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("blue wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_b = space.file_name;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("intensity wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_i = space.file_name;
    if(file_i.size()   < 5 || file_i.substr(file_i.size() - 4) != ".wav")
        file_i   += ".wav";
    if(prompt_escaped)
        return;
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_i))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_i + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    else if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return;
    space.TUI_clue = "joining waves";
    display_space();
    //------------------------------------------------------------------------
    if(space.join_XY_r_g_b_i_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                        LASERBOY_UNFORMATTED_SHARE + file_r,
                                        LASERBOY_UNFORMATTED_SHARE + file_g,
                                        LASERBOY_UNFORMATTED_SHARE + file_b,
                                        LASERBOY_UNFORMATTED_SHARE + file_i,
                                        LASERBOY_WAV_SHARE         + file_out,
                                        global_polarity
                                       )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::join_XY_rg_b_waves_menu()
{
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.TUI_clue = "join XY rg b";
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("XY wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_XY = space.file_name;
    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("red + green wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_rg = space.file_name;
    if(file_rg.size()  < 5 || file_rg.substr(file_rg.size() - 4) != ".wav")
        file_rg   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_rg))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_rg + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("blue wave");
        if(prompt_escaped)
            break;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_b = space.file_name;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    else if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    space.TUI_clue = "joining waves";
    display_space();
    //------------------------------------------------------------------------
    if(space.join_XY_rg_b_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                     LASERBOY_UNFORMATTED_SHARE + file_rg,
                                     LASERBOY_UNFORMATTED_SHARE + file_b,
                                     LASERBOY_WAV_SHARE         + file_out,
                                     global_polarity
                                    )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error("something horrible and unexplainable just happened.");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::join_XY_rg_b_LR_waves_menu()
{
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.TUI_clue = "join XY rg b LR";
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("XY wave");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_XY = space.file_name;
    if(file_XY.size()  < 5 || file_XY.substr(file_XY.size() - 4) != ".wav")
        file_XY  += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_XY))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_XY + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("red + green wave");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_rg = space.file_name;
    if(file_rg.size()  < 5 || file_rg.substr(file_rg.size() - 4) != ".wav")
        file_rg   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_rg))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_rg + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("blue wave");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_b = space.file_name;
    if(file_b.size()   < 5 || file_b.substr(file_b.size() - 4) != ".wav")
        file_b   += ".wav";
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + file_b))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + file_b + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.get_file_list(LASERBOY_AUDIO_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("stereo audio wave [no file = silence]");
        if(prompt_escaped)
            return;
        if(space.file_name.size() == 0)
            break;
    }
    string file_LR;
    if(space.file_name.size() == 0)
        file_LR = "_0";
    else
        file_LR = space.file_name;
    if(    file_LR != "_0"
        && (    file_LR.size() < 5
             || file_LR.substr(file_LR.size() - 4) != ".wav"
           )
      )
        file_LR += ".wav";
    if(    file_LR != "_0"
        && !file_exists(LASERBOY_AUDIO_SHARE + file_LR)
      )
    {
        space.display_error(LASERBOY_AUDIO_SHARE + file_LR + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    else if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return;
    space.TUI_clue = "joining waves";
    display_space();
    //------------------------------------------------------------------------
    if(space.join_XY_rg_b_LR_waves_prep(LASERBOY_UNFORMATTED_SHARE + file_XY,
                                        LASERBOY_UNFORMATTED_SHARE + file_rg,
                                        LASERBOY_UNFORMATTED_SHARE + file_b,
                                        LASERBOY_AUDIO_SHARE       + file_LR,
                                        LASERBOY_WAV_SHARE         + file_out,
                                        global_polarity
                                       )
      )
    {
        space.TUI_clue = "joined waves";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::add_audio_to_wave_menu()
{
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.TUI_clue = "add audio to wave";
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("6 channel laser wave");
        if(prompt_escaped)
            return;
    }
    if(space.file_name.size() == 0) // [esc]
        return;
    string file_laser = space.file_name;
    if(file_laser.size() < 5 || file_laser.substr(file_laser.size() - 4) != ".wav")
        file_laser += ".wav";
    if(!file_exists(LASERBOY_WAV_SHARE + file_laser))
    {
        space.display_error(LASERBOY_WAV_SHARE + file_laser + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.get_file_list(LASERBOY_AUDIO_SHARE, ".wav");
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("stereo audio wave [no file = silence]");
        if(prompt_escaped)
            return;
        if(space.file_name.size() == 0)
            break;
    }
    string file_LR;
    if(space.file_name.size() == 0)
        file_LR = "_0";
    else
        file_LR = space.file_name;
    if(    file_LR != "_0"
        && (    file_LR.size() < 5
             || file_LR.substr(file_LR.size() - 4) != ".wav"
           )
      )
        file_LR += ".wav";
    if(    file_LR != "_0"
        && !file_exists(LASERBOY_AUDIO_SHARE + file_LR)
      )
    {
        space.display_error(LASERBOY_AUDIO_SHARE + file_LR + " failed to open");
        wait_4_Esc();
        return;
    }
    //------------------------------------------------------------------------
    space.clean_screen();
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    space.overlay_file_list();
    string file_out = display_prompt_and_echo_name("new name for wave out", 64);
    if(prompt_escaped)
        return;
    if(file_out.size() == 0)
        return;
    else if(file_out.size() < 5 || file_out.substr(file_out.size() - 4) != ".wav")
        file_out += ".wav";
    if(prompt_escaped)
        return;
    space.TUI_clue = "adding audio";
    display_space();
    //------------------------------------------------------------------------
    if(space.add_audio_to_wave_prep(LASERBOY_WAV_SHARE   + file_laser,
                                    LASERBOY_AUDIO_SHARE + file_LR,
                                    LASERBOY_WAV_SHARE   + file_out
                                   )
      )
    {
        space.TUI_clue = "added audio";
        space.clean_screen();
        display_space();
        if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + file_out))
            space.display_error(LASERBOY_WAV_SHARE + file_out + " failed to open");
        else
            display_space();
    }
    else
        space.display_error("at least one of those failed to open");
    //------------------------------------------------------------------------
    wait_4_Esc();
    return;
}

//############################################################################
void LaserBoy_FLTK_GUI::file_open_menu()
{
    bool bad_key   = true;
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_file_type_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input file";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '1': // open ild
                space.current_menu_name = "open ild";
                if(ild_in_cd_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // open dxf
                space.current_menu_name = "open dxf";
                if(dxf_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open wav
                space.current_menu_name = "open wav";
                if(wav_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // open txt
                space.current_menu_name = "open txt";
                if(txt_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // open txt
                space.current_menu_name = "open ctn";
                if(ctn_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // open bmp
                space.current_menu_name = "open bmp";
                if(bmp_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '7': // open wtf
                space.current_menu_name = "open wtf";
                space.get_file_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wtf file name [LaserBoy.wtf]", "LaserBoy.wtf");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(!space.load_wtf_file(LASERBOY_WTF_SHARE + space.file_name))
                {
                    space.display_error(LASERBOY_WTF_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "wtf open";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8': // open utf8
                {   // scope
                    size_t     i;
                    u32string  unicode;
                    space.current_menu_name = "open utf8";
                    space.get_file_list(LASERBOY_UTF8_SHARE, ".utf8");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("utf8 file name");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    if(!get_utf8_index(unicode, LASERBOY_UTF8_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_UTF8_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        break;
                    }
                    if(unicode.size() > space.number_of_frames())
                    {
                        if(display_prompt_and_echo_bool("utf8 greater than frames! add missing frames? (y) | [any]"))
                        {
                            LaserBoy_frame frame;
                            for(i = 0; i < space.number_of_frames(); i++)
                                space.frame_picker(i).name = as_hex_string(unicode[i]);
                            for(i = space.number_of_frames(); i < unicode.size(); i++)
                            {
                                frame = LaserBoy_real_segment(unicode[i]);
                                frame.name = as_hex_string(unicode[i]);
                                space += frame;
                            }
                        }
                        else
                        {
                            bad_key = false;
                            break;
                        }
                    }
                    else if(unicode.size() < space.number_of_frames())
                    {
                        if(display_prompt_and_echo_bool("utf8 less than frames! tail-end un-indexed frames? (y) | [any]"))
                        {
                            for(i = 0; i < unicode.size(); i++)
                                space.frame_picker(i).name = as_hex_string(unicode[i]);
                            for(i = unicode.size(); i < space.number_of_frames(); i++)
                                space.frame_picker(i).name = "ffffffff";
                        }
                        else
                        {
                            bad_key = false;
                            break;
                        }
                    }
                    else
                    {
                        for(i = 0; i < unicode.size(); i++)
                            space.frame_picker(i).name = as_hex_string(unicode[i]);
                    }
                } // end scope
                space.TUI_clue = "utf8 open";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return;
}

//############################################################################
bool LaserBoy_FLTK_GUI::ild_in_cd_menu()
{
    bool   bad_key = true;
    prompt_escaped = false;
    space.clean_screen();
    while(bad_key)
    {
        space.clean_screen();
        space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
        if(!space.subdirectory_selected())
            space.working_subdirectory.clear();
        space.overlay_ild_in_cd_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input ild";
                return false;
            //----------------------------------------------------------------
            case '1': // stay in current ild directory
                space.TUI_clue = "open ild " + LASERBOY_ILD_SHARE + space.working_subdirectory;
                if(ild_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // go to root ild directory
                space.clean_screen();
                space.working_subdirectory.clear();
                space.overlay_subdirectory_list();
                space.TUI_clue = "ild cd " + LASERBOY_ILD_SHARE + space.working_subdirectory;
                if(ild_file_open_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // change current ild directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("cd to ild directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size()) // not zero
                        space.working_subdirectory += "/";
                    else
                        break;
                    if(!directory_exists(LASERBOY_ILD_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy)) // does not exists
                    {
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + " does not exists!");
                        wait_4_Esc();
                        space.working_subdirectory = subdirectory_copy; // stay in the current directory
                        break;
                    }
                    space.TUI_clue = "ild cd " + LASERBOY_ILD_SHARE + space.working_subdirectory;
                    if(ild_file_open_menu())
                        bad_key = false;
                }
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
        //----------------------------------------------------------------
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::ild_file_open_menu()
{
    bool bad_key = true;
    LaserBoy_ild_header_count counter;
    LaserBoy_frame_set        frame_set;
    space.get_file_list(LASERBOY_ILD_SHARE + space.working_subdirectory, ".ild");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("open ild file in " + LASERBOY_ILD_SHARE + space.working_subdirectory);
        if(prompt_escaped)
            return false;
    }
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".ild";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_ild_action_menu();
        display_space();
        //--------------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //--------------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input ild";
                return false;
            //----------------------------------------------------------------
            case '1': // replace
                space.TUI_clue = "ild replace";
                space.clean_screen();
                display_space();
                space.display_please_wait();
                if(!space.report_ild_file_open(frame_set, space.file_name, counter))
                    return false;
                space.frame_index = 0;
                space = frame_set;
                space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // prepend
                space.TUI_clue = "ild prepend";
                space.clean_screen();
                display_space();
                space.display_please_wait();
                if(!space.report_ild_file_open(frame_set, space.file_name, counter))
                    return false;
                space.frame_index = 0;
                frame_set += space;
                space = frame_set;
                space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // insert
                space.TUI_clue = "ild insert";
                space.clean_screen();
                display_space();
                space.display_please_wait();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set result;
                    if(!space.report_ild_file_open(frame_set, space.file_name, counter))
                        return false;
                    for(i = 0; i <= space.frame_index; i++)
                        result += space[i];
                    for(i = 0; i < frame_set.number_of_frames(); i++)
                        result += frame_set[i];
                    for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                        result += space[i];
                    space.frame_index = 0;
                    space = result;
                }
                space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append
                space.TUI_clue = "ild append";
                space.clean_screen();
                display_space();
                space.display_please_wait();
                if(!space.report_ild_file_open(frame_set, space.file_name, counter))
                    return false;
                space.frame_index = 0;
                space += frame_set;
                space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // superimpose
                space.TUI_clue = "ild superimpose";
                space.clean_screen();
                display_space();
                space.display_please_wait();
                if(!space.report_ild_file_open(frame_set, space.file_name, counter))
                    return false;
                space.frame_index = 0;
                space.superimpose_frame_set(frame_set);
                space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
        //--------------------------------------------------------------------
        space.display_ild_file_stats(counter);
        wait_4_Esc();
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::dxf_file_open_menu()
{
    bool   bad_key = true;
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_dxf_action_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input dxf";
                return false;
            //----------------------------------------------------------------
            case '1': // open dxf
                space.clean_screen();
                space.get_file_list(LASERBOY_DXF_SHARE, ".dxf");
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("dxf file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".dxf";
                space.TUI_clue = "dxf replace";
                space.clean_screen();
                display_space();
                if(space.from_dxf_file(LASERBOY_DXF_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_DXF_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // append dxf
                space.clean_screen();
                space.get_file_list(LASERBOY_DXF_SHARE, ".dxf");
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("dxf file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".dxf";
                space.TUI_clue = "dxf append";
                space.clean_screen();
                display_space();
                if(space.from_dxf_file(LASERBOY_DXF_SHARE + space.file_name, true) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_DXF_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open dxf directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_DXF_SHARE); // folders from ./dxf/
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("dxf directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "dxf set replace " + space.working_subdirectory;
                    display_space();
                    space.frame_index = 0;
                    if(!space.from_dxf_directory(LASERBOY_DXF_SHARE + space.working_subdirectory))
                    {
                        space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " failed to open");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "dxf set open " + space.working_subdirectory;
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
            case '4': // append dxf directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_DXF_SHARE); // folders from ./dxf/
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("dxf directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "dxf set append" + space.working_subdirectory;
                    display_space();
                    if(!space.from_dxf_directory(LASERBOY_DXF_SHARE + space.working_subdirectory, true))
                    {
                        space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " failed to open");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "dxf set append " + space.working_subdirectory;
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
            case '5': // open dxf
                space.clean_screen();
                space.get_file_list(LASERBOY_DXF_SHARE, ".dxf");
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("dxf file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".dxf";
                space.TUI_clue = "dxf font replace";
                space.clean_screen();
                display_space();
                if(space.from_dxf_font(LASERBOY_DXF_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_DXF_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    space.clean_screen();
                    space.render_space();
                    if(display_prompt_and_echo_bool(   "move "
                                                     + space.file_name.substr(0, space.file_name.size() - 4)
                                                     + ".utf8 from ./dxf to ./ild/fonts ? (y | [any])"
                                                   )
                      )
                    {
                        if(!file_exists(   LASERBOY_DXF_SHARE
                                         + space.file_name.substr(0, space.file_name.size() - 4)
                                         + ".utf8"
                                       )
                          )
                        {
                            space.clean_screen();
                            space.render_space();
                            space.display_error(   LASERBOY_DXF_SHARE
                                                 + space.file_name.substr(0, space.file_name.size() - 4)
                                                 + ".utf8 not found!"
                                               );
                            wait_4_Esc();
                            break;
                        }
                        else
                        {
                            if(    file_exists(   LASERBOY_FONT_SHARE
                                                + space.file_name.substr(0, space.file_name.size() - 4)
                                                + ".utf8"
                                              )
                                && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                              )
                            {
                                space.clean_screen();
                                space.render_space();
                                space.display_error(   LASERBOY_FONT_SHARE
                                                     + space.file_name.substr(0, space.file_name.size() - 4)
                                                     + ".utf8 not overwritten! file NOT saved!"
                                                   );
                                wait_4_Esc();
                                break;
                            }
                            else
                                remove(   LASERBOY_FONT_SHARE
                                        + space.file_name.substr(0, space.file_name.size() - 4)
                                        + ".utf8"
                                      );
                            rename(string(   LASERBOY_DXF_SHARE
                                           + space.file_name.substr(0, space.file_name.size() - 4)
                                           + ".utf8"
                                         ).c_str(),
                                   string(   LASERBOY_FONT_SHARE
                                           + space.file_name.substr(0, space.file_name.size() - 4)
                                           + ".utf8"
                                         ).c_str()
                                  );
                        }
                    }
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // append dxf
                space.clean_screen();
                space.get_file_list(LASERBOY_DXF_SHARE, ".dxf");
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("dxf file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".dxf";
                space.TUI_clue = "dxf font append";
                space.clean_screen();
                display_space();
                if(space.from_dxf_font(LASERBOY_DXF_SHARE + space.file_name, true) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_DXF_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    space.clean_screen();
                    space.render_space();
                    if(display_prompt_and_echo_bool(   "move "
                                                     + space.file_name.substr(0, space.file_name.size() - 4)
                                                     + ".utf8 from ./dxf to ./ild/fonts ? (y | [any])"
                                                   )
                      )
                    {
                        if(!file_exists(   LASERBOY_DXF_SHARE
                                         + space.file_name.substr(0, space.file_name.size() - 4)
                                         + ".utf8"
                                       )
                          )
                        {
                            space.clean_screen();
                            space.render_space();
                            space.display_error(   LASERBOY_DXF_SHARE
                                                 + space.file_name.substr(0, space.file_name.size() - 4)
                                                 + ".utf8 not found!"
                                               );
                            wait_4_Esc();
                            break;
                        }
                        else
                        {
                            if(    file_exists(   LASERBOY_FONT_SHARE
                                                + space.file_name.substr(0, space.file_name.size() - 4)
                                                + ".utf8"
                                              )
                                && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                              )
                            {
                                space.clean_screen();
                                space.render_space();
                                space.display_error(   LASERBOY_FONT_SHARE
                                                     + space.file_name.substr(0, space.file_name.size() - 4)
                                                     + ".utf8 not overwritten! file NOT saved!"
                                                   );
                                wait_4_Esc();
                                break;
                            }
                            else
                                remove(   LASERBOY_FONT_SHARE
                                        + space.file_name.substr(0, space.file_name.size() - 4)
                                        + ".utf8"
                                      );
                            rename(string(   LASERBOY_DXF_SHARE
                                           + space.file_name.substr(0, space.file_name.size() - 4)
                                           + ".utf8"
                                         ).c_str(),
                                   string(   LASERBOY_FONT_SHARE
                                           + space.file_name.substr(0, space.file_name.size() - 4)
                                           + ".utf8"
                                         ).c_str()
                                  );
                        }
                    }
                }
                space.TUI_clue = LASERBOY_DXF_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::wav_file_open_menu()
{
    bool bad_key   = true;
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_wav_action_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input wav";
                return false;
            //----------------------------------------------------------------
            case '1': // replace frame_set with LaserBoy formatted wave
                space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_WAV_SHARE + space.file_name))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        space.display_error(space.file_name + " is not a LaserBoy formatted wave");
                    else
                        space.display_error(space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // append frame_set with LaserBoy formatted wave
                space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_WAV_SHARE + space.file_name, true))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        space.display_error(space.file_name + " is not a LaserBoy formatted wave");
                    else
                        space.display_error(space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // open LaserBoy wave as unformatted
            {
                space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                if(!file_exists(LASERBOY_WAV_SHARE + space.file_name))
                {
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
                if(prompt_escaped)
                    break;
                space.clean_screen();
                display_space();
                if(!space.from_unformatted_wave_file(LASERBOY_WAV_SHARE + space.file_name, global_polarity, false))
                {
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_WAV_SHARE + space.file_name;
                bad_key = false;
                break;
            }
            //----------------------------------------------------------------
            case '4': // replace frame_set with LaserBoy formatted wave clip
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_CLIPS_SHARE + space.file_name))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        space.display_error(space.file_name + " is not a LaserBoy formatted wave");
                    else
                        space.display_error(space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_CLIPS_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // append frame_set with LaserBoy formatted wave clip
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                display_space();
                if(!space.from_LaserBoy_wave_file(LASERBOY_CLIPS_SHARE + space.file_name, true))
                {
                    if(space.frame_set_error & LASERBOY_LB_WAVE_FORMAT_ERROR)
                        space.display_error(space.file_name + " is not a LaserBoy formatted wave");
                    else
                        space.display_error(space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_CLIPS_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // open LaserBoy wave clip as unformatted
            {
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "opening " + space.file_name;
                if(!file_exists(LASERBOY_CLIPS_SHARE + space.file_name))
                {
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
                if(prompt_escaped)
                    break;
                space.clean_screen();
                display_space();
                if(!space.from_unformatted_wave_file(LASERBOY_CLIPS_SHARE + space.file_name, global_polarity, false))
                {
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = LASERBOY_CLIPS_SHARE + space.file_name;
                bad_key = false;
                break;
            }
            //----------------------------------------------------------------
            case '7': // replace frame_set with unformatted wave
                if(wav_unformatted_open_menu(false))
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8': // append frame_set with unformatted wave
                if(wav_unformatted_open_menu(true))
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '9': // open qm blanked wave
                if(wav_qm_open_menu(false))
                    bad_key = false;
                break;
        }
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::wav_unformatted_open_menu(bool append)
{
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    display_space();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name");
        if(prompt_escaped)
            return false;
    }
    if(space.file_name.size() == 0) // [esc]
        return false;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    space.TUI_clue = "opening wave";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return false;
    display_space();
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + space.file_name))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    if(!space.from_unformatted_wave_file(LASERBOY_UNFORMATTED_SHARE + space.file_name, global_polarity, append))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    space.TUI_clue = LASERBOY_UNFORMATTED_SHARE + space.file_name;
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::wav_qm_open_menu(bool append)
{
    space.get_file_list(LASERBOY_UNFORMATTED_SHARE, ".wav");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    display_space();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("wave file name");
        if(prompt_escaped)
            return false;
    }
    if(space.file_name.size() == 0) // [esc]
        return false;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".wav";
    space.TUI_clue = "opening wave";
    bool global_polarity = display_prompt_and_echo_bool("is this wave inverted ? (y | [any])");
    if(prompt_escaped)
        return false;
    display_space();
    if(!file_exists(LASERBOY_UNFORMATTED_SHARE + space.file_name))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    if(!space.from_qm_wave_file(LASERBOY_UNFORMATTED_SHARE + space.file_name, global_polarity, append))
    {
        space.display_error(LASERBOY_UNFORMATTED_SHARE + space.file_name + " failed to open");
        wait_4_Esc();
        return false;
    }
    space.TUI_clue = LASERBOY_UNFORMATTED_SHARE + space.file_name;
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::txt_file_open_menu()
{
    bool          bad_key = true;
    int           current_palette_count = space.number_of_palettes(),
                  new_palette_count,
                  new_frame_count  ;
    stringstream  strings;
    prompt_escaped = false;
    space.get_file_list(LASERBOY_TXT_SHARE, ".txt");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("ASCII txt file name");
        if(prompt_escaped)
            return false;
    }
    if(space.file_name.size() == 0) // [esc]
        return false;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".txt";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_txt_action_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        space.clean_screen();
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input txt";
                return false;
            //----------------------------------------------------------------
            case '1': // replace
                space.TUI_clue = "txt replace";
                display_space();
                {
                    LaserBoy_frame_set current;
                    current = space; // make a copy
                    space.from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = space.number_of_frames();
                    if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(space.number_of_frames() == 0) // no frames in txt file
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " no frames found"
                                         );
                            space = current;
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // syntax error in txt
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " syntax error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                          );
                            wait_4_Esc();
                        }
                        if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << space.file_name;
                            space.display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file open did not fail)
                }
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // prepend
                space.TUI_clue = "txt prepend";
                display_space();
                { // scope
                    LaserBoy_frame_set insert;
                    insert.from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                    new_frame_count   = insert.number_of_frames();
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            insert += space;
                            space = insert;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << space.file_name;
                            space.display_message(strings.str());
                            wait_4_any_key();
                        }
                    }
                }
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // insert
                space.TUI_clue = "txt insert";
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set insert,
                                       result;
                    insert.from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = insert.number_of_frames();
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            for(i = 0; i <= space.frame_index; i++)
                                result += space[i];
                            for(i = 0; i < insert.number_of_frames(); i++)
                                result += insert[i];
                            for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                                result += space[i];
                            space = result;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << space.file_name;
                            space.display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file opened)
                } // end scope
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append
                space.TUI_clue = "txt append";
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set insert,
                                       result;
                    insert.from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                    new_palette_count = space.number_of_palettes() - current_palette_count;
                    new_frame_count   = insert.number_of_frames();
                    if(insert.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        return false;
                    }
                    else
                    {
                        if(insert.number_of_frames() == 0) // no frames in txt file
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " no frames found"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " format error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name error (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " color table vertex mismatch (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " palette name in use (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(insert.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                        {
                            space.display_error(   LASERBOY_TXT_SHARE
                                           + space.file_name
                                           + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                         );
                            wait_4_Esc();
                        }
                        if(new_palette_count)
                        {
                            strings << new_palette_count
                                    << " palette"
                                    << (   (new_palette_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_frame_count)
                        {
                            for(i = 0; i < space.number_of_frames(); i++)
                                result += space[i];
                            for(i = 0; i < insert.number_of_frames(); i++)
                                result += insert[i];
                            space = result;
                            strings << new_frame_count
                                    << " frame"
                                    << (   (new_frame_count == 1)
                                         ? (" ")
                                         : ("s ")
                                       );
                        }
                        if(new_palette_count || new_frame_count)
                        {
                            strings << "found in "
                                    << LASERBOY_TXT_SHARE
                                    << space.file_name;
                            space.display_message(strings.str());
                            wait_4_any_key();
                        }
                    } // end else (file opened)
                } // end scope
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // superimpose
                space.TUI_clue = "txt superimpose";
                display_space();
                space.superimpose_from_txt_file(LASERBOY_TXT_SHARE + space.file_name, new_frame_count, new_palette_count);
                if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                {
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    return false;
                }
                else
                {
                    if(new_frame_count == 0) // no frames in txt file
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " no frames found"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " format error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " palette name error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH) // color table vertex mismatch
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " color table vertex mismatch (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " palette name in use (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(new_palette_count)
                    {
                        strings << new_palette_count
                                << " palette"
                                << (   (new_palette_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                    }
                    if(new_frame_count)
                    {
                        strings << new_frame_count
                                << " frame"
                                << (   (new_frame_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                    }
                    if(new_palette_count || new_frame_count)
                    {
                        strings << "found in "
                                << LASERBOY_TXT_SHARE
                                << space.file_name;
                        space.display_message(strings.str());
                        wait_4_any_key();
                    }
                } // end else (file opened)
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // superimpose
                space.TUI_clue = "txt palettes only";
                display_space();
                space.palettes_from_txt_file(LASERBOY_TXT_SHARE + space.file_name);
                new_palette_count = space.number_of_palettes() - current_palette_count;
                if(space.palette_set_error & LASERBOY_FILE_OPEN_FAILED) // open failed
                {
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    return false;
                }
                else
                {
                    if(space.palette_set_error & LASERBOY_TXT_SYNTAX_ERROR) // word error in txt
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " format error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.palette_set_error & LASERBOY_TXT_PALETTE_FAILED) // palette name error
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " palette name error (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.palette_set_error & LASERBOY_PALETTE_NAME_IN_USE)
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " palette name in use (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(space.frame_set_error & LASERBOY_TXT_PALETTE_NAME_TOO_LONG)
                    {
                        space.display_error(   LASERBOY_TXT_SHARE
                                       + space.file_name
                                       + " more than 8 characters in palette name (see txt_in_errors.txt)"
                                     );
                        wait_4_Esc();
                    }
                    if(new_palette_count)
                    {
                        strings << new_palette_count
                                << " palette"
                                << (   (new_palette_count == 1)
                                     ? (" ")
                                     : ("s ")
                                   );
                        strings << "found in "
                                << LASERBOY_TXT_SHARE
                                << space.file_name;
                        space.display_message(strings.str());
                        wait_4_any_key();
                    }
                } // end else (file opened)
                space.TUI_clue = LASERBOY_TXT_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::ctn_file_open_menu()
{
    bool bad_key = true;
    space.get_file_list(LASERBOY_CTN_SHARE, ".ctn");
    space.clean_screen();
    space.file_name.clear();
    space.overlay_file_list();
    prompt_escaped = false;
    while(!space.file_selected())
    {
        display_prompt_file_with_auto_complete("ctn file name");
        if(prompt_escaped)
            return false;
    }
    if(space.file_name.size() == 0) // [esc]
        return false;
    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
        space.file_name += ".ctn";
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_ctn_action_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input ctn";
                return false;
            //----------------------------------------------------------------
            case '1': // replace
                space.TUI_clue = "ctn replace";
                space.clean_screen();
                display_space();
                { // scope
                    LaserBoy_frame_set frame_set;
                    if(!space.report_ctn_file_open(frame_set, space.file_name))
                        return false;
                    space.frame_index = 0;
                    space = frame_set;
                } // end scope
                space.TUI_clue = LASERBOY_CTN_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // prepend
                space.TUI_clue = "ctn prepend";
                space.clean_screen();
                display_space();
                { // scope
                    LaserBoy_frame_set frame_set;
                    if(!space.report_ctn_file_open(frame_set, space.file_name))
                        return false;
                    space.frame_index = 0;
                    frame_set += space;
                    space = frame_set;
                }
                space.TUI_clue = LASERBOY_CTN_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // insert
                space.TUI_clue = "ctn insert";
                space.clean_screen();
                display_space();
                { // scope
                    u_int              i;
                    LaserBoy_frame_set frame_set,
                                       result;
                    if(!space.report_ctn_file_open(frame_set, space.file_name))
                        return false;
                    for(i = 0; i <= space.frame_index; i++)
                        result += space[i];
                    for(i = 0; i < frame_set.number_of_frames(); i++)
                        result += frame_set[i];
                    for(i = space.frame_index + 1; i < space.number_of_frames(); i++)
                        result += space[i];
                    space.frame_index = 0;
                    space = result;
                }
                space.TUI_clue = LASERBOY_CTN_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // append
                space.TUI_clue = "ctn append";
                space.clean_screen();
                display_space();
                {
                    LaserBoy_frame_set frame_set;
                    if(!space.report_ctn_file_open(frame_set, space.file_name))
                        return false;
                    space.frame_index = 0;
                    space += frame_set;
                }
                space.TUI_clue = LASERBOY_CTN_SHARE + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // superimpose
                space.TUI_clue = "ctn superimpose";
                space.clean_screen();
                display_space();
                {
                    LaserBoy_frame_set frame_set;
                    if(!space.report_ctn_file_open(frame_set, space.file_name))
                        return false;
                    space.frame_index = 0;
                    space.superimpose_frame_set(frame_set);
                }
                space.TUI_clue = LASERBOY_CTN_SHARE + space.file_name;
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
        //----------------------------------------------------------------
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::bmp_file_open_menu()
{
    bool bad_key   = true;
    prompt_escaped = false;
    space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
    while(bad_key)
    {
        space.clean_screen();
        space.overlay_in_bmp_action_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit input bmp";
                return false;
            //----------------------------------------------------------------
            case '1': // open as frame color map
                space.clean_screen();
                space.file_name.clear();
                space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("bitmap file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "loading " + space.file_name;
                display_space();
                if(space.current_frame().color_from_bmp(LASERBOY_BMP_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "color map " + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // open as frame_set color map
                space.clean_screen();
                space.file_name.clear();
                space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("bitmap file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "loading " + space.file_name;
                display_space();
                if(space.color_from_bmp(LASERBOY_BMP_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "color map all " + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // load bmp directory as map
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE);
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("bmp directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.get_file_list(LASERBOY_BMP_SHARE + space.working_subdirectory + '/', ".bmp");
                    if(space.working_directory_list.size() == 0)
                    {
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + "/ has no files");
                        wait_4_Esc();
                        break;
                    }
                    space.bmp_file_list = space.working_directory_list;
                    space.bmp_file_index = 0;
                    display_space();

                    if(space.number_of_frames() >= space.bmp_file_list.size())
                    {
                        for(size_t i = 0; i < space.number_of_frames(); i++)
                            space.frame_picker(i).color_from_bmp(space.bmp_file_list[i % space.bmp_file_list.size()].path().string());
                    }
                    else
                    {
                        for(size_t i = 0; i < space.number_of_frames(); i++)
                            space.frame_picker(i).color_from_bmp(space.bmp_file_list[i].path().string());
                        for(size_t i = space.number_of_frames(); i < space.bmp_file_list.size(); i++)
                        {
                            space += space.frame_picker(i - space.number_of_frames());
                            space.frame_picker(space.number_of_frames() - 1).color_from_bmp(space.bmp_file_list[i].path().string());
                        }
                    }
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
            case '4': // open as frame mask
                space.clean_screen();
                space.file_name.clear();
                space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("bitmap file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "loading " + space.file_name;
                display_space();
                if(space.current_frame().subtract_bmp(LASERBOY_BMP_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "frame - " + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // open as frame_set mask
                space.clean_screen();
                space.file_name.clear();
                space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("bitmap file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "loading " + space.file_name;
                display_space();
                if(space.subtract_bmp(LASERBOY_BMP_SHARE + space.file_name) != LASERBOY_OK)
                {
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                    wait_4_Esc();
                    break;
                }
                space.TUI_clue = "subtract bmp all";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // load bmp directory as mask
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE);
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("bmp directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.get_file_list(LASERBOY_BMP_SHARE + space.working_subdirectory + '/', ".bmp");
                    if(space.working_directory_list.size() == 0)
                    {
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + "/ has no files");
                        wait_4_Esc();
                        break;
                    }
                    space.bmp_file_list = space.working_directory_list;
                    space.bmp_file_index = 0;
                    display_space();

                    if(space.number_of_frames() >= space.bmp_file_list.size())
                    {
                        for(size_t i = 0; i < space.number_of_frames(); i++)
                            space.frame_picker(i).subtract_bmp(space.bmp_file_list[i % space.bmp_file_list.size()].path().string());
                    }
                    else
                    {
                        for(size_t i = 0; i < space.number_of_frames(); i++)
                            space.frame_picker(i).subtract_bmp(space.bmp_file_list[i].path().string());
                        for(size_t i = space.number_of_frames(); i < space.bmp_file_list.size(); i++)
                        {
                            space += space.frame_picker(i - space.number_of_frames());
                            space.frame_picker(space.number_of_frames() - 1).subtract_bmp(space.bmp_file_list[i].path().string());
                        }
                    }
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
            case '7': // open bmp palette into palette_set
                space.clean_screen();
                space.file_name.clear();
                space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("bitmap file name");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "loading " + space.file_name;
                display_space();
                switch(space.copy_palette_from_bmp(LASERBOY_BMP_SHARE + space.file_name, space.file_name.substr(0, space.file_name.size() - 4)))
                {
                    default:
                    case LASERBOY_FILE_OPEN_FAILED:
                        space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        break;
                    case LASERBOY_PALETTE_ALREADY_DEFINED:
                        space.display_error(LASERBOY_BMP_SHARE + space.file_name + " has already been loaded into the palette set");
                        wait_4_Esc();
                        break;
                    case LASERBOY_NO_PALETTES_FOUND:
                        space.display_error(LASERBOY_BMP_SHARE + space.file_name + " is not a palette bmp");
                        wait_4_Esc();
                        break;
                    case LASERBOY_OK:
                        space.TUI_clue = "bmp palette" + space.file_name;
                        break;
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8': // open as background
                {
                    bool scale_to_screen = false;
                    space.clean_screen();
                    space.file_name.clear();
                    space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
                    space.overlay_file_list();
                    while(!space.file_selected())
                    {
                        display_prompt_file_with_auto_complete("bitmap file name");
                        if(prompt_escaped)
                            break;
                    }
                    if(space.file_name.size() == 0) // [esc]
                        break;
                    if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".bmp";
                    space.TUI_clue = "loading " + space.file_name;
                    display_space();
                    scale_to_screen = display_prompt_and_echo_bool("scale bitmap to fit display? : (y | [any])");
                    if(!scale_to_screen)
                        space.background_bmp_scale = 1.00;
                    if(!space.load_background_bitmap(LASERBOY_BMP_SHARE + space.file_name,
                                                     scale_to_screen
                                                    )
                      )
                    {
                        space.display_error(LASERBOY_BMP_SHARE + space.file_name + " failed to open");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "background " + space.file_name;
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '9': // load bmp directory
                { // scope
                    bool   scale_to_screen   = false;
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE);
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("bmp directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.get_file_list(LASERBOY_BMP_SHARE + space.working_subdirectory + '/', ".bmp");
                    if(space.working_directory_list.size() == 0)
                    {
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + "/ has no files");
                        wait_4_Esc();
                        break;
                    }
                    space.bmp_file_list = space.working_directory_list;
                    space.bmp_file_index = 0;
                    space.file_name = space.bmp_file_list.front().path().filename().string();
                    space.TUI_clue = "loading " + space.file_name;
                    display_space();
                    scale_to_screen = display_prompt_and_echo_bool("scale bitmap to fit display? : (y | [any])");
                    if(!scale_to_screen)
                        space.background_bmp_scale = 1.00;
                    if(!space.load_background_bitmap(space.bmp_file_list.front().path().string(),
                                                     scale_to_screen
                                                    )
                      )
                    {
                        space.display_error(space.bmp_file_list.front().path().string() + " failed to open");
                        wait_4_Esc();
                        break;
                    }
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
            case '0': // clear background
                space.bmp_file_list.clear();
                space.bmp_file_index = -1;
                space.clear_background_bitmap();
                space.TUI_clue = "clear background";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case 'a': // display bmp directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE);
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("bmp directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "bmp display " + space.working_subdirectory;
                    display_space();
                    bool scale_to_screen = display_prompt_and_echo_bool("scale bmp files to screen ? : (y) | [any]");
                    space.clean_screen();
                    if(!display_bmp_directory(LASERBOY_BMP_SHARE + space.working_subdirectory, scale_to_screen))
                    {
                        space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + "/ failed to open");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "end bmp display " + space.working_subdirectory;
                    space.working_subdirectory = subdirectory_copy;
                    bad_key = false;
                }
                break;
        }
    }
    return true;
}

//############################################################################
void LaserBoy_FLTK_GUI::save_as_file_menu()
{
    bool bad_key   = true;
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_file_type_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output file";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '1':
                space.current_menu_name = "save as ild";
                if(ild_out_cd_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                space.current_menu_name = "save as dxf";
                if(save_as_dxf_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3':
                space.current_menu_name = "save as wav";
                if(save_as_wav_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.current_menu_name = "save as txt";
                if(save_as_txt_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                space.current_menu_name = "save as ctn";
                if(save_as_ctn_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.current_menu_name = "save as bmp";
                if(save_as_bmp_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '7':
                space.get_file_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wtf file name [LaserBoy.wtf]", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = "LaserBoy.wtf";
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                   space.file_name += ".wtf";
                if(    file_exists(LASERBOY_WTF_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WTF_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_wtf_file(LASERBOY_WTF_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_WTF_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "wtf saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8':
                {   // scope
                    space.current_menu_name = "save utf8";
                    space.get_file_list(LASERBOY_UTF8_SHARE, ".utf8");
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new utf8 file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 6 || !iends_with(space.file_name, space.current_extension))
                       space.file_name += ".utf8";
                    if(    file_exists(LASERBOY_UTF8_SHARE + space.file_name)
                        && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_UTF8_SHARE + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        string  report;
                        if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name, report))
                        {
                            if(space.frame_set_error & LASERBOY_INVALID_UNICODE)
                            {
                                if(!display_prompt_and_echo_bool("use ffffffff for non unicode named frames ? (y | [any])"))
                                {
                                    space.display_error(space.file_name.substr(0, space.file_name.size() - 4) + ".utf8 not saved!");
                                    wait_4_Esc();
                                    break;
                                }
                                else if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name, report, true))
                                {
                                    space.display_error(string("no unicode names found. ") + space.file_name.substr(0, space.file_name.size() - 4) + ".utf8 not saved!");
                                    wait_4_Esc();
                                    break;
                                }
                            }
                            else if(space.frame_set_error & LASERBOY_REDUNDANT_UNICODE)
                            {
                                if(!display_prompt_and_echo_bool("save unicode with redundant named frames ? (y | [any])"))
                                {
                                    space.display_error(space.file_name.substr(0, space.file_name.size() - 4) + ".utf8 not saved!");
                                    wait_4_Esc();
                                    break;
                                }
                                else if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name, report, true))
                                {
                                    space.display_error(string("no unicode names found. ") + space.file_name.substr(0, space.file_name.size() - 4) + ".utf8 not saved!");
                                    wait_4_Esc();
                                    break;
                                }
                            }
                            else
                            {
                                space.display_error(report);
                                wait_4_Esc();
                                break;
                            }
                        } // end if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name, report))
                    }
                    space.TUI_clue = "utf8 saved";
                    bad_key = false;
                }
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return;
}

//############################################################################
bool LaserBoy_FLTK_GUI::ild_out_cd_menu()
{
    bool bad_key        = true;
         prompt_escaped = false;
    space.clean_screen();
    while(bad_key)
    {
        space.clean_screen();
        space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
        if(!space.subdirectory_selected())
            space.working_subdirectory.clear();
        space.overlay_ild_out_cd_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output ild";
                return false;
            //----------------------------------------------------------------
            case '1': // stay in current ild directory
                space.TUI_clue = "save ild";
                if(save_as_ild_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // go to root ild directory
                space.clean_screen();
                space.working_subdirectory.clear();
                space.overlay_subdirectory_list();
                space.TUI_clue = "ild cd root" + LASERBOY_ILD_SHARE + space.working_subdirectory;
                if(save_as_ild_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // change current ild directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
                    space.overlay_subdirectory_list();
                    while(!space.subdirectory_selected())
                    {
                        display_prompt_dir_with_auto_complete("cd to ild directory name");
                        if(prompt_escaped)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                    }
                    if(space.working_subdirectory.size()) // not zero
                        space.working_subdirectory += "/";
                    else
                        break;
                }
                space.TUI_clue = "ild cd " + LASERBOY_ILD_SHARE + space.working_subdirectory;
                if(save_as_ild_menu())
                    bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // create new ild directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new ild directory name", 64);
                    if(prompt_escaped || space.working_subdirectory.size() == 0)
                    {
                        space.working_subdirectory = subdirectory_copy;
                        break;
                    }
                    else
                        space.working_subdirectory += "/";
                    if(!directory_exists(LASERBOY_ILD_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
#if defined __MINGW_LASERBOY__
                        mkdir((LASERBOY_ILD_SHARE + space.working_subdirectory).c_str());
#else
                        mkdir((LASERBOY_ILD_SHARE + space.working_subdirectory).c_str(), 0777);
#endif
                        if(save_as_ild_menu())
                            bad_key = false;
                    }
                    else
                    {
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + " already exists!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                    }
                }
                break;
            //----------------------------------------------------------------
            case '5': // split frame_set into new directory
                { // scope
                    bool                      temp_save_as_4_5 = false;
                    LaserBoy_ild_header_count counter;
                    if(!space.save_ild_fmt_4_5 && space.true_color_frames())
                    {
                        if(!display_prompt_and_echo_bool("save ild files with format-3 ? : y | [any]"))
                        {
                            if(display_prompt_and_echo_bool("save ild files as format-4 or 5 ? : y | [any]"))
                            {
                                space.save_ild_fmt_4_5 = true;
                                temp_save_as_4_5       = true;
                            }
                            else
                            {
                                space.display_message("NO ild file saved! see menu x option h");
                                wait_4_any_key();
                                break;
                            }
                        }
                    }
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_ILD_SHARE); // folders from ./ild/
                    space.overlay_subdirectory_list();
                    if(    space.number_of_selected_frames() > 1
                        || (    space.number_of_selected_frames() > 0
                             && !space.frame_picker(0).is_selected
                           )
                      ) // enough is selected
                    {
                        space.working_subdirectory = display_prompt_and_echo_name("new ild directory name", 64);
                        if(prompt_escaped || space.working_subdirectory.size() == 0)
                        {
                            space.working_subdirectory = subdirectory_copy;
                            break;
                        }
                        if(!directory_exists(LASERBOY_ILD_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                        {
#if defined __MINGW_LASERBOY__
                            mkdir((LASERBOY_ILD_SHARE + space.working_subdirectory).c_str());
#else
                            mkdir((LASERBOY_ILD_SHARE + space.working_subdirectory).c_str(), 0777);
#endif
                            { // scope
                                u_int               i,
                                                    j;
                                vector<u_int>       selected_frames_index;
                                LaserBoy_frame_set  out;
                                char                number[12];
                                //------------------------------------------------
                                selected_frames_index.push_back(0); // selected or not
                                for(i = 1; i < space.number_of_frames(); i++)
                                {
                                    if(space.frame_picker(i).is_selected)
                                        selected_frames_index.push_back(i);
                                }
                                //------------------------------------------------
                                for(i = 0; i < selected_frames_index.size() - 1; i++)
                                {
                                    for(j = selected_frames_index[i]; j < selected_frames_index[i + 1]; j++)
                                        out += space.frame_picker(j);
                                    sprintf(number, "%05d", (int)i);
                                    out.save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + string("/") + space.working_subdirectory + string("_") + number + string(".ild"), counter);
                                    out.clear();
                                }
                                //------------------------------------------------
                                if(selected_frames_index[i] == (space.number_of_frames() - 1)) // last frame is selected
                                {
                                    out = space.frame_picker(selected_frames_index[i]);
                                }
                                else
                                {
                                    for(j = selected_frames_index[i]; j < space.number_of_frames(); j++)
                                        out += space.frame_picker(j);
                                }
                                sprintf(number, "%05d", (int)i);
                                out.save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + string("/") + space.working_subdirectory + string("_") + number + string(".ild"), counter);
                                out.clear();
                                space.working_subdirectory += "/";
                                bad_key = false;
                                space.display_ild_file_stats(counter);
                                wait_4_Esc();
                            } // end scope
                        } // end if(!directory_exists(LASERBOY_ILD_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                        else
                        {
                            space.working_subdirectory += "/";
                            space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + " already exists!");
                            space.working_subdirectory = subdirectory_copy;
                            wait_4_Esc();
                        }
                    } // end if enough is selected
                    else
                    {
                        space.display_error("Not enough frames selected for split!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                    }
                    if(temp_save_as_4_5)
                        space.save_ild_fmt_4_5 = false;
                }
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
        //----------------------------------------------------------------
    } // end while(bad_key)
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_ild_menu()
{
    bool                      bad_key          = true,
                              temp_save_as_4_5 = false;
    LaserBoy_ild_header_count counter;
    prompt_escaped = false;
    space.get_file_list(LASERBOY_ILD_SHARE + space.working_subdirectory, ".ild");
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.file_name.clear();
        space.overlay_file_list();
        space.overlay_out_ild_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
            switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output ild";
                return false;
            //----------------------------------------------------------------
            case '1': // save current frame
                if(space.current_frame().size() > LASERBOY_MAX_SHORT)
                {
                    space.display_error("Too many vertices, cannot be saved as ild!");
                    wait_4_Esc();
                    space.clean_screen();
                    if(display_prompt_and_echo_bool("save as unoptomized LaserBoy wave (one frame) ? : y | [any]"))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.file_name.clear();
                        space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                        space.overlay_file_list();
                        space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                        if(prompt_escaped)
                            break;
                        if(space.file_name.size() == 0)
                            break;
                        else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                            space.file_name += ".wav";
                        space.TUI_clue = "saving wave";
                        if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                            && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                          )
                        {
                            space.clean_screen();
                            space.render_space();
                            space.overlay_file_list();
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                            wait_4_Esc();
                            break;
                        }
                        else
                        {
                            if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + space.file_name, false, false, false))
                            {
                                space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                                wait_4_Esc();
                                break;
                            }
                            space.TUI_clue = "wave saved";
                            space.clean_screen();
                            space.render_space();
                            if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                                space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                            else
                                display_space();
                            wait_4_Esc();
                        }
                    }
                    bad_key = false;
                    break;
                } // end if(space.current_frame().size() > LASERBOY_MAX_SHORT)
                else
                {
                    space.current_frame().sync_rgb_and_palette();
                    if(!space.save_ild_fmt_4_5 && space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(!display_prompt_and_echo_bool("save ild file containing format-3 ? : y | [any]"))
                        {
                            if(display_prompt_and_echo_bool("save ild file as format-4 or 5 ? : y | [any]"))
                            {
                                space.save_ild_fmt_4_5 = true;
                                temp_save_as_4_5       = true;
                            }
                            else
                            {
                                space.display_message("NO ild file saved! see menu x option h");
                                wait_4_any_key();
                                break;
                            }
                        }
                    }
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new ild file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".ild";
                    space.TUI_clue = "saving " + space.file_name;
                    display_space();
                    if(    file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                        && !display_prompt_and_echo_bool(space.file_name + " exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.display_please_wait();
                        if(!space.current_frame().save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name, counter))
                        {
                            space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.TUI_clue = "ild saved";
                    if(temp_save_as_4_5)
                        space.save_ild_fmt_4_5 = false;
                    bad_key = false;
                    space.display_ild_file_stats(counter);
                    wait_4_Esc();
                    break;
                }
            //----------------------------------------------------------------
            case '2': // save selected frames
                if(space.number_of_selected_frames()) // something is selected
                {
                    if(space.selected_frames().is_ild_busted())
                    {
                        space.display_error("Too many vertices, cannot be saved as ild!");
                        wait_4_Esc();
                        space.clean_screen();
                        if(display_prompt_and_echo_bool("save as unoptomized LaserBoy wave each frame once ? : y | [any]"))
                        {
                            space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                            space.clean_screen();
                            space.render_space();
                            space.file_name.clear();
                            space.overlay_file_list();
                            space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                            if(prompt_escaped)
                                break;
                            if(space.file_name.size() == 0)
                                break;
                            else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                                space.file_name += ".wav";
                            space.TUI_clue = "saving wave";
                            if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                                && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                              )
                            {
                                space.clean_screen();
                                space.render_space();
                                space.overlay_file_list();
                                space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                                wait_4_Esc();
                                break;
                            }
                            else
                            {
                                if(!space.selected_frames().save_as_wave(LASERBOY_WAV_SHARE + space.file_name, false, false))
                                {
                                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                                    wait_4_Esc();
                                    break;
                                }
                                space.TUI_clue = "wave saved";
                                space.clean_screen();
                                space.render_space();
                                if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                                else
                                    display_space();
                                wait_4_Esc();
                            }
                        }
                        bad_key = false;
                        break;
                    }
                    else
                    {
                        if(!space.save_ild_fmt_4_5 && space.selected_frames().true_color_frames())
                        {
                            if(!display_prompt_and_echo_bool("save ild file containing format-3 ? : y | [any]"))
                            {
                                if(display_prompt_and_echo_bool("save ild file as format-4 or 5 ? : y | [any]"))
                                {
                                    space.save_ild_fmt_4_5 = true;
                                    temp_save_as_4_5       = true;
                                }
                                else
                                {
                                    space.display_message("NO ild file saved! see menu x option h");
                                    wait_4_any_key();
                                    break;
                                }
                            }
                        }
                        space.clean_screen();
                        space.render_space();
                        space.file_name.clear();
                        space.overlay_file_list();
                        space.file_name = display_prompt_and_echo_name("new ild file name", 64);
                        if(prompt_escaped)
                            break;
                        if(space.file_name.size() == 0)
                            break;
                        else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                            space.file_name += ".ild";
                        space.TUI_clue = "saving selected " + space.file_name;
                        display_space();
                        if(    file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                            && !display_prompt_and_echo_bool(space.file_name + " exists! overwrite ? (y | [any])")
                          )
                        {
                            space.clean_screen();
                            space.render_space();
                            space.overlay_file_list();
                            space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                            wait_4_Esc();
                            break;
                        }
                        else
                        {
                            space.display_please_wait();
                            if(!space.selected_frames().save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name, counter))
                            {
                                space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " failed to save");
                                wait_4_Esc();
                                break;
                            }
                        }
                        space.TUI_clue = "selected ild saved";
                        bad_key = false;
                        space.display_ild_file_stats(counter);
                        wait_4_Esc();
                    }
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                if(temp_save_as_4_5)
                    space.save_ild_fmt_4_5 = false;
                break;
            //----------------------------------------------------------------
            case '3': // save all frames
                if(space.is_ild_busted())
                {
                    space.display_error("Too many vertices, cannot be saved as ild!");
                    wait_4_Esc();
                    space.clean_screen();
                    if(display_prompt_and_echo_bool("save as unoptomized LaserBoy wave each frame once ? : y | [any]"))
                    {
                        space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                        space.clean_screen();
                        space.render_space();
                        space.file_name.clear();
                        space.overlay_file_list();
                        space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                        if(prompt_escaped)
                            break;
                        if(space.file_name.size() == 0)
                            break;
                        else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                            space.file_name += ".wav";
                        space.TUI_clue = "saving wave";
                        if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                            && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                          )
                        {
                            space.clean_screen();
                            space.render_space();
                            space.overlay_file_list();
                            space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                            wait_4_Esc();
                            break;
                        }
                        else
                        {
                            if(!space.save_as_wave(LASERBOY_WAV_SHARE + space.file_name, false, false))
                            {
                                space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                                wait_4_Esc();
                                break;
                            }
                            space.TUI_clue = "wave saved";
                            space.clean_screen();
                            space.render_space();
                            if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                                space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                            else
                                display_space();
                            wait_4_Esc();
                        }
                    }
                    bad_key = false;
                    break;
                } // end if(space.is_ild_busted())
                else
                {
                    if(!space.save_ild_fmt_4_5 && space.true_color_frames())
                    {
                        if(!display_prompt_and_echo_bool("save ild file containing format-3 ? : y | [any]"))
                        {
                            if(display_prompt_and_echo_bool("save ild file as format-4 or 5 ? : y | [any]"))
                            {
                                space.save_ild_fmt_4_5 = true;
                                temp_save_as_4_5       = true;
                            }
                            else
                            {
                                space.display_message("NO ild file saved! see menu x option h");
                                wait_4_any_key();
                                break;
                            }
                        }
                    } // end if(!space.save_ild_fmt_4_5 && space.true_color_frames())
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new ild file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".ild";
                    space.TUI_clue = "saving " + space.file_name;
                    display_space();
                    if(    file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                        && !display_prompt_and_echo_bool(space.file_name + " exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.display_please_wait();
                        if(!space.save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name, counter))
                        {
                            space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.TUI_clue = "ild saved";
                    if(temp_save_as_4_5)
                        space.save_ild_fmt_4_5 = false;
                    space.display_ild_file_stats(counter);
                    wait_4_Esc();
                    bad_key = false;
                    break;
                }
            //----------------------------------------------------------------
            case '4': // generate and save frame effect
                space.clean_screen();
                space.effect_name.clear();
                space.overlay_frame_effects();
                while(!space.f_effect_selected())
                {
                    display_prompt_f_effect_with_auto_complete("frame effect name");
                    if(prompt_escaped)
                        break;
                }
                if(space.effect_name.size() == 0)
                    break;
                space.TUI_clue = space.effect_name;
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = LASERBOY_NO_FILE_SAVED;
                display_space();
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && (    space.file_name.size() < 5
                         || !iends_with(space.file_name, space.current_extension)
                       )
                  )
                    space.file_name += ".ild";
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_effect();
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // generate and save selected frames effect
                if(space.number_of_selected_frames())
                {
                    space.clean_screen();
                    space.effect_name.clear();
                    space.overlay_selected_frames_effects();
                    while(!space.sf_effect_selected())
                    {
                      display_prompt_sf_effect_with_auto_complete("frame_set effect name");
                      if(prompt_escaped)
                          break;
                    }
                    if(space.effect_name.size() == 0)
                      break;
                    space.TUI_clue = space.effect_name;
                    space.clean_screen();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                    if(prompt_escaped)
                      break;
                    if(space.file_name.size() == 0)
                      space.file_name = LASERBOY_NO_FILE_SAVED;
                    display_space();
                    if(    space.file_name != LASERBOY_NO_FILE_SAVED
                      && (    space.file_name.size() < 5
                           || !iends_with(space.file_name, space.current_extension)
                         )
                    )
                      space.file_name += ".ild";
                    if(    space.file_name != LASERBOY_NO_FILE_SAVED
                      && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                      && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                    )
                    {
                      space.clean_screen();
                      space.render_space();
                      space.overlay_file_list();
                      space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                      wait_4_Esc();
                      break;
                    }
                    else
                    {
                      LaserBoy_Bounds out_of_bounds = space.call_selected_frames_effect();
                      if(out_of_bounds & LASERBOY_NOTHING_SELECTED)
                      {
                          space.display_error(space.effect_name + " requires selected frames");
                          wait_4_Esc();
                          break;
                      }
                      if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                      {
                          space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                          wait_4_Esc();
                          break;
                      }
                    }
                    bad_key = false;
                    break;
                }
                else
                {
                    space.display_error("requires selected frames");
                    wait_4_Esc();
                    break;
                }
            //----------------------------------------------------------------
            case '6': // generate and save frame_set effect
                space.clean_screen();
                space.effect_name.clear();
                space.overlay_frame_set_effects();
                while(!space.fs_effect_selected())
                {
                    display_prompt_fs_effect_with_auto_complete("frame_set effect name");
                    if(prompt_escaped)
                        break;
                }
                if(space.effect_name.size() == 0)
                    break;
                space.TUI_clue = space.effect_name;
                space.clean_screen();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ild file name [no name no save] ? ", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = LASERBOY_NO_FILE_SAVED;
                display_space();
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && (    space.file_name.size() < 5
                         || !iends_with(space.file_name, space.current_extension)
                       )
                  )
                    space.file_name += ".ild";
                if(    space.file_name != LASERBOY_NO_FILE_SAVED
                    && file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    LaserBoy_Bounds out_of_bounds = space.call_frame_set_effect();
                    if(out_of_bounds & LASERBOY_NOTHING_SELECTED)
                    {
                        space.display_error(space.effect_name + " requires selected frames");
                        wait_4_Esc();
                        break;
                    }
                    if(out_of_bounds & LASERBOY_BOUNDS_MASK)
                    {
                        space.display_error(space.effect_name + " is out of bounds! NO FILE SAVED!");
                        wait_4_Esc();
                        break;
                    }
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '7': // ild save all frames and font index
                if(space.working_subdirectory == "fonts/")
                {
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new font name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".ild";
                    space.TUI_clue = "saving " + space.file_name;
                    display_space();
                    if(    file_exists(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name)
                        && !display_prompt_and_echo_bool(space.file_name + " exists! overwrite ? (y | [any])")
                      )
                    {
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        string  report;
                        if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name.substr(0, space.file_name.size() - 4) + ".utf8",
                                                        report
                                                       )
                          )
                        {
                            if(space.frame_set_error & LASERBOY_INVALID_UNICODE) //  "is not a unicode name.";
                            {
                                space.display_error(report);
                                wait_4_Esc();
                                if(!display_prompt_and_echo_bool("skip non unicode named frames ? (y | [any])"))
                                {
                                    space.display_error(space.file_name.substr(0, space.file_name.size() - 4) + ".utf8 not saved!");
                                    wait_4_Esc();
                                    break;
                                }
                                else if(!space.save_utf8_frame_index(LASERBOY_UTF8_SHARE + space.file_name.substr(0, space.file_name.size() - 4) + ".utf8",
                                                                     report,
                                                                     true
                                                                    )
                                       )
                                {
                                    space.display_error(report);
                                    wait_4_Esc();
                                    break;
                                }
                            }
                            else
                            {
                                space.display_error(report);
                                wait_4_Esc();
                                break;
                            }
                        }
                    }
                    space.display_please_wait();
                    if(!space.save_as_ild(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name, counter))
                    {
                        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "font and index saved";
                    space.display_ild_file_stats(counter);
                    wait_4_Esc();
                    bad_key = false;
                }
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_dxf_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save dxf";
    space.get_file_list(LASERBOY_DXF_SHARE, ".dxf");
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_dxf_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output dxf";
                return false;
            //----------------------------------------------------------------
            case '1': // save current frame as DXF file
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new dxf file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".dxf";
                space.TUI_clue = "saving dxf";
                space.render_space();
                display_space();
                if(    file_exists(LASERBOY_DXF_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_DXF_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_dxf(LASERBOY_DXF_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_DXF_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "dxf saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // save selected frames as DXF directory
                if(space.number_of_selected_frames()) // something is selected
                {
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.render_space();
                    space.get_subdirectory_list(LASERBOY_DXF_SHARE);
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new dxf directory name", 64);
                    if(prompt_escaped)
                    {
                        space.working_subdirectory = subdirectory_copy;
                        break;
                    }
                    space.TUI_clue = "saving dxf directory";
                    space.render_space();
                    display_space();
                    if(directory_exists(LASERBOY_DXF_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!space.selected_frames().save_as_dxf_directory(LASERBOY_DXF_SHARE, space.working_subdirectory))
                        {
                            space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " failed to save");
                            space.working_subdirectory = subdirectory_copy;
                            wait_4_Esc();
                            break;
                        }
                        space.TUI_clue = "dxf directory saved";
                        space.working_subdirectory = subdirectory_copy;
                    }
                    bad_key = false;
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                break;
            //----------------------------------------------------------------
            case '3': // save all frames as DXF directory
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.render_space();
                    space.get_subdirectory_list(LASERBOY_DXF_SHARE);
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new dxf directory name", 64);
                    if(prompt_escaped)
                    {
                        space.working_subdirectory = subdirectory_copy;
                        break;
                    }
                    space.TUI_clue = "saving dxf";
                    space.render_space();
                    display_space();
                    if(directory_exists(LASERBOY_DXF_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!space.save_as_dxf_directory(LASERBOY_DXF_SHARE, space.working_subdirectory))
                        {
                            space.display_error(LASERBOY_DXF_SHARE + space.working_subdirectory + " failed to save");
                            space.working_subdirectory = subdirectory_copy;
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.working_subdirectory = subdirectory_copy;
                    space.TUI_clue = "dxf directory saved";
                    bad_key = false;
                }
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_wav_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save as file";
    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_wav_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output wav";
                return false;
            //----------------------------------------------------------------
            case '1': // optimized wave of current frame once
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + space.file_name, true, false, false))
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + space.file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2': // optimized wave of current frame for duration
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_WAV_SHARE + space.file_name, true, true, false))
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + space.file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '3': // optimized wave each frame once
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + space.file_name, true, false))
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + space.file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4': // optimized wave at fps
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_WAV_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_WAV_SHARE + space.file_name, true, true))
                    {
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    if(space.auto_apply_offsets && space.have_nonzero_offsets())
                        space.apply_wave_offsets_prep(LASERBOY_WAV_SHARE + space.file_name);
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            // LASERBOY_CLIPS_SHARE
            //----------------------------------------------------------------
            case '5': // optimized wave clip of current frame
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                {
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    break;
                }
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_CLIPS_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_CLIPS_SHARE + space.file_name, true, false, true))
                    {
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_CLIPS_SHARE + space.file_name))
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6': // unoptimized wave of current frame once
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                {
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    break;
                }
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_CLIPS_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_CLIPS_SHARE + space.file_name, false, false, false))
                    {
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_CLIPS_SHARE + space.file_name))
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '7': // unoptimized wave current frame for duration
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                {
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    break;
                }
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_CLIPS_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_wave(LASERBOY_CLIPS_SHARE + space.file_name, false, true, false))
                    {
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_CLIPS_SHARE + space.file_name))
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '8': // unoptimized wave all frames once
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                {
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    break;
                }
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_CLIPS_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_CLIPS_SHARE + space.file_name, false, false))
                    {
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_CLIPS_SHARE + space.file_name))
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '9': // unoptimized wave all frames at fps
                space.get_file_list(LASERBOY_CLIPS_SHARE, ".wav");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wave file name", 64);
                if(prompt_escaped)
                {
                    space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    break;
                }
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving wave";
                if(    file_exists(LASERBOY_CLIPS_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_wave(LASERBOY_CLIPS_SHARE + space.file_name, false, true))
                    {
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "wave saved";
                    space.clean_screen();
                    space.render_space();
                    if(!space.overlay_wave_header_stats(LASERBOY_CLIPS_SHARE + space.file_name))
                        space.display_error(LASERBOY_CLIPS_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                    wait_4_Esc();
                }
                bad_key = false;
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_txt_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save ASCII";
    space.get_file_list(LASERBOY_TXT_SHARE, ".txt");
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_txt_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output txt";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new txt file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".txt";
                space.TUI_clue = "saving txt";
                if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                    space.TUI_clue = "txt saved";
                }

                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                if(space.number_of_selected_frames())
                {
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new txt file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".txt";
                    space.TUI_clue = "saving txt";
                    if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                        && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!space.selected_frames().save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                        {
                            space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.TUI_clue = "txt saved";
                    bad_key = false;
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new txt file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".txt";
                space.TUI_clue = "saving txt";
                if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '4':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new palette file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".txt";
                space.TUI_clue = "saving txt";
                if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(space.current_frame().palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(!space.current_frame().save_as_txt_table(LASERBOY_TXT_SHARE + space.file_name))
                        {
                            space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    else if(!space.palette_picker(space.current_frame().palette_index).save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new palette file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".txt";
                space.TUI_clue = "saving txt";
                if(    file_exists(LASERBOY_TXT_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_TXT_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.palette_picker(space.target_palette_index).save_as_txt(LASERBOY_TXT_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_TXT_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "txt saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '6':
                space.get_file_list(LASERBOY_WAV_SHARE, ".wav");
                space.file_name.clear();
                space.overlay_file_list();
                while(!space.file_selected())
                {
                    display_prompt_file_with_auto_complete("wave file name to save color rescales");
                    if(prompt_escaped)
                        break;
                }
                if(space.file_name.size() == 0) // [esc]
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".wav";
                space.TUI_clue = "saving rescales";
                space.clean_screen();
                space.render_space();
                display_space();
                if(space.save_color_rescales(LASERBOY_WAV_SHARE     + space.file_name,
                                             LASERBOY_RESCALE_SHARE + space.file_name.substr(0, space.file_name.size() - 4))
                                            )
                {
                    space.TUI_clue = "wave stats";
                    if(!space.overlay_wave_header_stats(LASERBOY_WAV_SHARE + space.file_name))
                        space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                    else
                        display_space();
                }
                else
                    space.display_error(LASERBOY_WAV_SHARE + space.file_name + " failed to open");
                wait_4_Esc();
                break;
            //----------------------------------------------------------------
            case '7':
                space.get_file_list(LASERBOY_WTF_SHARE, ".wtf");
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new wtf file name [LaserBoy.wtf]", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    space.file_name = "LaserBoy.wtf";
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                   space.file_name += ".wtf";
                if(    file_exists(LASERBOY_WTF_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_WTF_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_wtf_file(LASERBOY_WTF_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_WTF_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "wtf saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_ctn_menu()
{
    bool   bad_key = true;
    space.get_file_list(LASERBOY_CTN_SHARE, ".ctn");
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_ctn_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output ctn";
                return false;
            //----------------------------------------------------------------
            case '1':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ctn file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".ctn";
                space.TUI_clue = "saving ctn";
                display_space();
                if(    file_exists(LASERBOY_CTN_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CTN_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.current_frame().save_as_ctn(LASERBOY_CTN_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_CTN_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ctn saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '2':
                if(space.number_of_selected_frames())
                {
                    space.clean_screen();
                    space.render_space();
                    space.file_name.clear();
                    space.overlay_file_list();
                    space.file_name = display_prompt_and_echo_name("new ctn file name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.file_name.size() == 0)
                        break;
                    else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                        space.file_name += ".ctn";
                    space.TUI_clue = "saving ctn";
                    display_space();
                    if(    file_exists(LASERBOY_CTN_SHARE + space.file_name)
                        && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_file_list();
                        space.display_error(LASERBOY_CTN_SHARE + space.file_name + " not overwritten! file NOT saved!");
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!space.selected_frames().save_as_ctn(LASERBOY_CTN_SHARE + space.file_name))
                        {
                            space.display_error(LASERBOY_CTN_SHARE + space.file_name + " failed to save");
                            wait_4_Esc();
                            break;
                        }
                    }
                    space.TUI_clue = "ctn saved";
                    bad_key = false;
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                break;
            //----------------------------------------------------------------
            case '3':
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new ctn file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0)
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".ctn";
                space.TUI_clue = "saving ctn";
                display_space();
                if(    file_exists(LASERBOY_CTN_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_CTN_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    if(!space.save_as_ctn(LASERBOY_CTN_SHARE + space.file_name))
                    {
                        space.display_error(LASERBOY_CTN_SHARE + space.file_name + " failed to save");
                        wait_4_Esc();
                        break;
                    }
                }
                space.TUI_clue = "ctn saved";
                bad_key = false;
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}

//############################################################################
bool LaserBoy_FLTK_GUI::save_as_bmp_menu()
{
    bool   bad_key = true;
    space.TUI_clue = "save bmp";
    space.get_file_list(LASERBOY_BMP_SHARE, ".bmp");
    prompt_escaped = false;
    while(bad_key)
    {
        space.clean_screen();
        space.render_space();
        space.overlay_out_bmp_menu();
        display_space();
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while(event.type != LASERBOY_GUI_KEYDOWN);
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            default:
                space.TUI_clue = "bad key";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RSHIFT:
            case LASERBOY_KEY_LSHIFT:
                space.TUI_clue = "";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                space.TUI_clue = "exit output bmp";
                return false;
            //----------------------------------------------------------------
            case '1': // save current frame as bmp file
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new bitmap file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0) // [esc]
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "saving bitmap";
                if(    file_exists(LASERBOY_BMP_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                {
                    remove(LASERBOY_BMP_SHARE + space.file_name);
                    space.current_frame().save_as_bmp(LASERBOY_BMP_SHARE + space.file_name);
                    space.TUI_clue = "bitmap saved";
                    bad_key = false;
                    break;
                }
            //----------------------------------------------------------------
            case '2': // save selected frames as directory of bmp
                if(space.number_of_selected_frames())
                {
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE); // folders from ./ild/
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new bitmap directory name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "saving bitmap directory";
                    if(directory_exists(LASERBOY_BMP_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.selected_frames().save_as_bmp_directory(LASERBOY_BMP_SHARE + space.working_subdirectory, space.working_subdirectory);
                        space.working_subdirectory = subdirectory_copy;
                        space.TUI_clue = "bitmap directory saved";
                        bad_key = false;
                    }
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                break;
            //----------------------------------------------------------------
            case '3': // save all frames as directory of bmp
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE); // folders from ./bmp/
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new bitmap directory name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "saving bitmap directory";
                    if(directory_exists(LASERBOY_BMP_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.save_as_bmp_directory(LASERBOY_BMP_SHARE + space.working_subdirectory, space.working_subdirectory);
                        space.working_subdirectory = subdirectory_copy;
                        space.TUI_clue = "bitmap directory saved";
                        bad_key = false;
                    }
                }
                break;
            //----------------------------------------------------------------
            case '4': // save current frame as bmp file
                space.clean_screen();
                space.render_space();
                space.file_name.clear();
                space.overlay_file_list();
                space.file_name = display_prompt_and_echo_name("new bitmap file name", 64);
                if(prompt_escaped)
                    break;
                if(space.file_name.size() == 0) // [esc]
                    break;
                else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                    space.file_name += ".bmp";
                space.TUI_clue = "saving bitmap";
                if(    file_exists(LASERBOY_BMP_SHARE + space.file_name)
                    && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                  )
                {
                    space.clean_screen();
                    space.render_space();
                    space.overlay_file_list();
                    space.display_error(LASERBOY_BMP_SHARE + space.file_name + " not overwritten! file NOT saved!");
                    wait_4_Esc();
                    break;
                }
                else
                    space.save_as_bmp_view(space.frame_index, LASERBOY_BMP_SHARE + space.file_name);
                space.TUI_clue = "bitmap saved";
                bad_key = false;
                break;
            //----------------------------------------------------------------
            case '5': // save selected frames as directory of bmp
                if(space.number_of_selected_frames())
                {
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE); // folders from ./bmp/
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new bitmap directory name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "saving bitmap directory";
                    if(directory_exists(LASERBOY_BMP_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.selected_frames().save_as_bmp_directory_view(LASERBOY_BMP_SHARE + space.working_subdirectory, space.working_subdirectory);
                        space.working_subdirectory = subdirectory_copy;
                        space.TUI_clue = "bitmap directory saved";
                        bad_key = false;
                    }
                }
                else
                {
                    space.display_error("no frames selected to save!");
                    space.file_name.clear();
                    wait_4_Esc();
                }
                break;
            //----------------------------------------------------------------
            case '6': // save all frames as directory of bmp
                { // scope
                    string subdirectory_copy = space.working_subdirectory;
                    space.clean_screen();
                    space.working_subdirectory.clear();
                    space.get_subdirectory_list(LASERBOY_BMP_SHARE); // folders from ./bmp/
                    space.overlay_subdirectory_list();
                    space.working_subdirectory = display_prompt_and_echo_name("new bitmap directory name", 64);
                    if(prompt_escaped)
                        break;
                    if(space.working_subdirectory.size() == 0)
                        break;
                    space.TUI_clue = "saving bitmap directory";
                    if(directory_exists(LASERBOY_BMP_SHARE + space.working_subdirectory, space.absolute_path_of_LaserBoy))
                    {
                        space.clean_screen();
                        space.render_space();
                        space.overlay_subdirectory_list();
                        space.display_error(LASERBOY_BMP_SHARE + space.working_subdirectory + " exists! can not be overwritten! file(s) NOT saved!");
                        space.working_subdirectory = subdirectory_copy;
                        wait_4_Esc();
                        break;
                    }
                    else
                    {
                        space.save_as_bmp_directory_view(LASERBOY_BMP_SHARE + space.working_subdirectory, space.working_subdirectory);
                        space.working_subdirectory = subdirectory_copy;
                        space.TUI_clue = "bitmap directory saved";
                        bad_key = false;
                    }
                }
                break;
        } // end switch(event.key.keysym.unicode)
    }
    return true;
}
/*
//############################################################################
void LaserBoy_FLTK_GUI::display_rescale_file_error(LaserBoy_Rescale_Error_Code error)
{
    switch(error)
    {
        case LASERBOY_RESCALE_INCOMPLETE_DATA:
            space.display_error("Not enough data");
            break;
        case LASERBOY_RESCALE_IMPROPER_DATA:
            space.display_error("Data out of range");
            break;
        case LASERBOY_RESCALE_ODD_DATA:
            space.display_error("Data contains odd numbers");
            break;
        case LASERBOY_RESCALE_NONZERO_ZERO:
            space.display_error("First value is not zero");
            break;
        case LASERBOY_RESCALE_REDUNDANT_DATA:
            space.display_error("Redundant data elements");
            break;
        case LASERBOY_RESCALE_FILE_FAILED:
            space.display_error("File open failed");
            break;
    }
    return;
}
*/
//############################################################################
void LaserBoy_FLTK_GUI::Laserboy_terminating()
{
    space.TUI_clue = "LaserBoy terminated";
    display_space();
    laserboy_gui_delay(500);
    return;
}

//############################################################################
int LaserBoy_FLTK_GUI::start_menu_loop()
{
    long int                  bytes_skipped = 0;
    ostringstream             output_message;
    LaserBoy_ild_header_count counter;
    string                    version_string;
    srand(time(NULL));
    running = true;
    if(    !LaserBoy_version_check(version_string, space.install_GUID, space.app_runs_count) // zero means we got a version_string
        && (version_string.size() == 19)
        && (version_string != LASERBOY_VERSION)
        && (version_string.substr(0, 9) == "LaserBoy-")
      )
    {
        space.TUI_clue = version_string + string(" is now available (README.txt)");
    }
    space.TUI_clue = "NO DATA!";
    space.working_subdirectory.clear();
    space.file_name = "in.ild";
    space.from_ild_file(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name, bytes_skipped, counter);
    if(space.frame_set_error == LASERBOY_OK)
        space.TUI_clue = LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name;
    else if(space.frame_set_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
    {
        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " table to frame quantity mismatch");
        wait_4_Esc();
    }
    else if(space.frame_set_error & LASERBOY_NO_FRAMES_FOUND)
    {
        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " no frames found");
        wait_4_Esc();
    }
    else if(space.frame_set_error & LASERBOY_FILE_OPEN_FAILED)
    {
        space.display_error(LASERBOY_ILD_SHARE + space.working_subdirectory + space.file_name + " failed to open");
        wait_4_Esc();
    }
    //--------------------------------------------------------------------
    if(bytes_skipped > 0)
    {
        space.clean_screen();
        output_message << LASERBOY_ILD_SHARE
                       << space.working_subdirectory
                       << space.file_name
                       << " : "
                       << bytes_skipped
                       << " bytes unrecognized data";
        space.display_error(output_message.str());
        wait_4_Esc();
    }
    //--------------------------------------------------------------------
    display_space();
    space.display_ild_file_stats(counter);
    wait_4_Esc();
    while(running)
    {
        space.clean_screen();
        space.set_current_menu_name("main menu", 0x00000000);
        space.render_space();
        if(space.show_menu)
            space.overlay_main_menu();
        display_space();
        prompt_escaped = false;
        //----------------------------------------------------------------
        do
        {
            wait_for_gui_event(&event);
        } while((event.type != LASERBOY_GUI_KEYDOWN));
        if(!running)
            break;
        //----------------------------------------------------------------
        switch(event.key.keysym.unicode)
        {
            //----------------------------------------------------------------
            case LASERBOY_KEY_ESCAPE:
                { // scope
                    bool           show_background_bitmap_was = space.show_background_bitmap;
                    LaserBoy_color bg_was                     = space.bg_color,
                                   white_text_was             = space.white_text;
                    space.white_text = LaserBoy_color(255, 255, 255);
                    space.bg_color   = LaserBoy_color(255,   0,   0);
                    space.show_background_bitmap = false;
                    space.TUI_clue = "terminating";
                    bmp_fill(&(space.bmp), 255, 0, 0);
                    //----------------------------------------------------------------
                    if(display_prompt_and_echo_bool("Do you really want to terminate LaserBoy? (y | [any])"))
                    {
                        space.white_text = white_text_was;
                        space.bg_color   = bg_was;
                        space.clean_screen();
                        if(    space.report_wtf_changes()
                            && display_prompt_and_echo_bool("Settings differ from LaserBoy.wtf. Save new .wtf ? : y | [any]")
                          )
                        {
                            space.file_name = display_prompt_and_echo_name("new wtf file name [LaserBoy.wtf]", 64);
                            if(prompt_escaped)
                                break;
                            if(space.file_name.size() == 0)
                                space.file_name = "LaserBoy.wtf";
                            else if(space.file_name.size() < 5 || !iends_with(space.file_name, space.current_extension))
                               space.file_name += ".wtf";
                            if(    file_exists(LASERBOY_WTF_SHARE + space.file_name)
                                && !display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                              )
                            {
                                space.clean_screen();
                                space.render_space();
                                space.overlay_file_list();
                                space.display_error(LASERBOY_WTF_SHARE + space.file_name + " not overwritten! file NOT saved!");
                                wait_4_Esc();
                                break;
                            }
                            else
                            {
                                if(!space.save_wtf_file(LASERBOY_WTF_SHARE + space.file_name))
                                {
                                    space.display_error(LASERBOY_WTF_SHARE + space.file_name + " failed to save");
                                    wait_4_Esc();
                                    break;
                                }
                            }
                            space.display_message(LASERBOY_WTF_SHARE + space.file_name + " saved");
                            space.TUI_clue = "wtf saved";
                            running = false;
                        }
                        else
                        {
                            if(prompt_escaped)
                                break;
                            space.display_message("LaserBoy terminated.");
                            space.TUI_clue = "terminated";
                            running = false;
                        }
                    }
                    else
                    {
                        space.white_text = white_text_was;
                        space.bg_color   = bg_was;
                        space.TUI_clue   = "terminate canceled";
                        space.show_background_bitmap = show_background_bitmap_was;
                        space.clean_screen();
                        space.render_space();
                    }
                }
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_RETURN:
                space.show_menu = !space.show_menu;
                if(!space.show_menu)
                    space.TUI_clue = "[Enter] show menu";
                else
                    space.TUI_clue = "[Enter] hide menu";
                break;
            //----------------------------------------------------------------
            case LASERBOY_KEY_BACKSPACE:
            case LASERBOY_KEY_DELETE:
                space.delete_current_frame();
                space.TUI_clue = "deleted";
                break;
            //----------------------------------------------------------------
                default:
                    check_for_nav_keys(event.key.keysym.sym);
                    check_for_digit_keys(event.key.keysym.sym);
                    break;
            //----------------------------------------------------------------
            case '`':
                animate_forward();
                break;
            //----------------------------------------------------------------
            case '~':
                animate_reverse();
                break;
            //----------------------------------------------------------------
            case '-':
                rotate_forward();
                break;
            //----------------------------------------------------------------
            case '_':
                rotate_reverse();
                break;
            //----------------------------------------------------------------
            case '=':
                animate_rotate_forward();
                break;
            //----------------------------------------------------------------
            case '+':
                animate_rotate_reverse();
                break;
            //----------------------------------------------------------------
            case '/': // show frame_set stats
                space.show_frame_set_stats = !space.show_frame_set_stats;
                break;
            //----------------------------------------------------------------
            case '?': // show frame stats
                space.show_frame_stats = !space.show_frame_stats;
                break;
            //----------------------------------------------------------------
            case ';': // toggle bitmap background
                space.show_background_bitmap = !space.show_background_bitmap;
                if(space.show_background_bitmap)
                    space.TUI_clue = "show bitmap";
                else
                    space.TUI_clue = "hide bitmap";
                break;
            //----------------------------------------------------------------
            case ':': // negative bitmap background
                space.show_background_bitmap = true;
                space.background_bmp_negative = !space.background_bmp_negative;
                space.reload_background_bitmap();
                space.TUI_clue = "negative bitmap";
                break;
            //----------------------------------------------------------------
            case 'q':
                space.view_offset.x += space.displacement_step;
                space.TUI_clue = "+ pan X";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'Q':
                space.view_offset.x -= space.displacement_step;
                space.TUI_clue = "- pan X";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'w':
                space.view_offset.y += space.displacement_step;
                space.TUI_clue = "+ pan Y";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'W':
                space.view_offset.y -= space.displacement_step;
                space.TUI_clue = "- pan Y";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'e':
                space.view_offset += space.displacement_step;
                space.TUI_clue = "+ pan XY";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'E':
                space.view_offset -= space.displacement_step;
                space.TUI_clue = "- pan XY";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'r':
            case 'R':
                space.view_offset = 0;
                space.TUI_clue = "on center";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'a':
                space.view_scale.x *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom X";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'A':
                space.view_scale.x *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom X";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 's':
                space.view_scale.y *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom Y";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'S':
                space.view_scale.y *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom Y";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'd':
                space.view_scale *= space.magnitude_step_up;
                space.TUI_clue = "+ zoom XY";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'D':
                space.view_scale *= space.magnitude_step_dn;
                space.TUI_clue = "- zoom XY";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'f':
            case 'F':
                space.view_scale = 1.00;
                space.TUI_clue = "no zoom";
                space.view_has_changed = true;
                break;
            //----------------------------------------------------------------
            case 'g':
                space.clean_screen();
                space.render_space();
                space.report_wtf_changes();
                display_space();
                wait_4_Esc();
                break;
                //------------------------------------------------------------
                case 'v':
                    if(!space.current_frame().apply_view())
                    {
                        space.view_angle       = 0.0;
                        space.view_offset      = 0;
                        space.view_scale       = 1.0;
                        space.TUI_clue         = "set view frame";
                        space.view_has_changed = true;
                    }
                    else
                        space.TUI_clue = "out of bounds!";
                    break;
                //------------------------------------------------------------
                case 'V':
                    space.TUI_clue = "setting all";
                    space.render_space();
                    display_space();
                    { // scope
                        LaserBoy_frame_set current(space);
                        if(!space.apply_view())
                        {
                            space.view_angle       = 0.0;
                            space.view_offset      = 0;
                            space.view_scale       = 1.0;
                            space.TUI_clue         = "set view frame";
                            space.view_has_changed = true;
                        }
                        else
                        {
                            space = current;
                            space.display_error("out of bounds!");
                            wait_4_Esc();
                        }
                    }
                    break;
            //----------------------------------------------------------------
            case 'y': // go to frame index
            {
                u_int temp_u_int;
                space.TUI_clue = "go to index";
                temp_u_int = display_prompt_and_echo_u_int(   "frame index"
                                                            , 0
                                                            , space.number_of_frames() - 1
                                                          );
                if(prompt_escaped)
                    break;
                space.frame_index = temp_u_int % space.number_of_frames();
                break;
            }
            //----------------------------------------------------------------
            case 'Y': // go to frame unicode
            {
                int     frame_index;
                string  unihex;
                space.TUI_clue = "go to unicode";
                unihex = display_prompt_and_echo_unicode("frame unicode");
                if(prompt_escaped)
                    break;
                frame_index = space.index_of_name(unihex);
                if(frame_index >= 0)
                    space.frame_index = frame_index;
                else
                {
                    space.display_error(unihex + " not found.");
                    wait_4_Esc();
                }
                break;
            }
            //----------------------------------------------------------------
            case 'z': // +100 frames
                if(space.number_of_frames() > 100)
                {
                    if(    space.loop_animation
                        || (space.frame_index <= (space.number_of_frames() - 101))
                      )
                        space.frame_index += 100;
                    while(space.frame_index >= space.number_of_frames())
                        space.frame_index = space.frame_index - space.number_of_frames();
                    space.TUI_clue = "+100 frames";
                }
                break;
            //----------------------------------------------------------------
            case 'Z': // -100 frames
                if(space.number_of_frames() > 100)
                {
                    if(space.frame_index >= 100)
                        space.frame_index -= 100;
                    else if(    space.loop_animation
                             && (space.frame_index < 100)
                           )
                    {
                        space.frame_index = space.number_of_frames() + space.frame_index;
                        space.frame_index -= 100;
                    }
                    space.TUI_clue = "-100 frames";
                }
                break;
            //----------------------------------------------------------------
            // call out to another menu loop
            //----------------------------------------------------------------
            case 'c':
            case 'C':
                space.set_current_menu_name("frames to hues", 0x00200040, true);
                frame_color_trans_menu();
                break;
            //----------------------------------------------------------------
            case 'p':
            case 'P':
                space.set_current_menu_name("palette set edit", 0x00303000, true);
                palette_set_trans_menu();
                break;
            //----------------------------------------------------------------
            case 'h':
            case 'H':
                space.set_current_menu_name("frame transforms", 0x00202020, true);
                frame_transforms_menu();
                break;
            //----------------------------------------------------------------
            case 'j':
            case 'J':
                space.set_current_menu_name("frame_set transforms", 0x00400020, true);
                frame_set_transforms_menu();
                break;
            //----------------------------------------------------------------
            case 'k':
            case 'K':
                space.set_current_menu_name("draw color blank", 0x00200040, true);
                draw_color_blank_menu();
                break;
            //----------------------------------------------------------------
            case 'l':
            case 'L':
                space.set_current_menu_name("move scale rotate", 0x00204000, true);
                move_scale_rotate_menu();
                break;
            //----------------------------------------------------------------
            case 'm':
            case 'M':
                space.set_current_menu_name("render segments", 0x00402000, true);
                direct_draw_segments_menu();
                break;
            //----------------------------------------------------------------
            case 'n':
            case 'N':
                space.set_current_menu_name("effect functions", 0x00402000, true);
                effect_functions_menu();
                break;
            //----------------------------------------------------------------
            case 'b':
            case 'B':
                space.set_current_menu_name("wave utilities", 0x00002040, true);
                wave_utilities_menu();
                break;
            //----------------------------------------------------------------
            case 'i':
            case 'I':
                space.set_current_menu_name("input file", 0x00004020, true);
                file_open_menu();
                break;
            //----------------------------------------------------------------
            case 'o':
            case 'O':
                space.set_current_menu_name("output file", 0x00204020, true);
                save_as_file_menu();
                break;
            //----------------------------------------------------------------
        } // end switch(event.key.keysym.unicode)
        //----------------------------------------------------------------
    } // end while(running)
    //------------------------------------------------------------------------
    Laserboy_terminating();
    return 0;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################