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
// LaserBoy_frame_set_effects.cpp is part of LaserBoy.
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

#define GUI (*(p_space->p_GUI))

//############################################################################
// void finish_effect(LaserBoy_frame *p_frame, LaserBoy_frame_set& out)
void finish_effect(LaserBoy_frame_set *p_frame_set, LaserBoy_frame_set& out)
{
    if(p_space->file_name != LASERBOY_NO_FILE_SAVED)
    {
        if(out.is_ild_busted())
        {
            do
            {
                p_space->display_error("Too many vertices, cannot be saved as ild!");
                GUI.wait_4_Esc();
                p_space->clean_screen();
                if(GUI.display_prompt_and_echo_bool("save as unoptomized LaserBoy wav each frame once ? : y | [any]"))
                {
                    GUI.space->get_file_list(LASERBOY_WAV_SHARE, ".wav");
                    GUI.space->clean_screen();
                    GUI.space->render_space();
                    p_space->file_name.clear();
                    GUI.space->overlay_file_list();
                    p_space->file_name = GUI.display_prompt_and_echo_name("new wav file name", 64);
                    if(p_space->file_name.size() == 0)
                        break;
                    else if(p_space->file_name.size() < 5 || !iends_with(p_space->file_name, p_space->current_extension))
                        p_space->file_name += ".wav";
                    GUI.space->TUI_clue = "saving wav";
                    if(    file_exists(LASERBOY_WAV_SHARE + p_space->file_name)
                        && !GUI.display_prompt_and_echo_bool("file exists! overwrite ? (y | [any])")
                      )
                    {
                        GUI.space->clean_screen();
                        GUI.space->render_space();
                        GUI.space->overlay_file_list();
                        p_space->display_error(LASERBOY_WAV_SHARE + p_space->file_name + " not overwritten! file NOT saved!");
                        GUI.wait_4_Esc();
                        break;
                    }
                    else
                    {
                        if(!out.save_as_wav(LASERBOY_WAV_SHARE + p_space->file_name, false, false))
                        {
                            p_space->display_error(LASERBOY_WAV_SHARE + p_space->file_name + " failed to save");
                            GUI.wait_4_Esc();
                            break;
                        }
                        GUI.space->TUI_clue = "wav saved";
                        GUI.space->clean_screen();
                        GUI.space->render_space();
                        if(!GUI.space->overlay_wav_header_stats(LASERBOY_WAV_SHARE + p_space->file_name))
                            p_space->display_error(LASERBOY_WAV_SHARE + p_space->file_name + " failed to open");
                        else
                            GUI.display_space();
                        GUI.wait_4_Esc();
                        break;
                    }
                }
            } while(0);
        } // end if(out.is_ild_busted())
        else
        {
            bool                      temp_save_as_4_5 = false;
            LaserBoy_ild_header_count counter;
            do
            {
                if(!p_space->save_ild_fmt_4_5 && out.true_color_frames())
                {
                    if(!GUI.display_prompt_and_echo_bool("save ild file containing format-3 ? : y | [any]"))
                    {
                        if(GUI.display_prompt_and_echo_bool("save ild file as format-4 or 5 ? : y | [any]"))
                        {
                            p_space->save_ild_fmt_4_5 = true;
                            temp_save_as_4_5          = true;
                        }
                        else
                        {
                            p_space->display_message("NO ild file saved! see menu x option h");
                            GUI.wait_4_any_key();
                            break;
                        }
                    }
                } // end if(!p_space->save_ild_fmt_4_5 && out.true_color_frames())
                p_space->display_please_wait();
                out.save_as_ild(LASERBOY_ILD_SHARE + p_space->working_subdirectory + p_space->file_name, counter);
                p_space->display_ild_file_stats(counter);
                GUI.wait_4_Esc();
                if(temp_save_as_4_5)
                    p_space->save_ild_fmt_4_5 = false;
                break;
            } while(0);
        }
        if(GUI.display_prompt_and_echo_bool("replace current frame_set with result ? (y | [any])"))
            *(p_space->p_frame_set) = out;
    } // end if(p_space->file_name != LASERBOY_NO_FILE_SAVED)
    else
        *(p_space->p_frame_set) = out;
    return;
}

//############################################################################
LaserBoy_Bounds outlines(LaserBoy_frame_set* p_frame_set)
{
    bool                  inc_vecs = GUI.display_prompt_and_echo_bool("include original vectors ? : (y) | [any]"),
                          inc_cnts = GUI.display_prompt_and_echo_bool("include centroids of vectors ? : (y) | [any]");
    double                area;
    LaserBoy_color        c;
    LaserBoy_3D_double    p,
                          front(    0.0,     0.0, 32767.0),
                          side (32767.0,     0.0,     0.0),
                          top  (    0.0, 32767.0,     0.0);
    LaserBoy_real_segment rs;
    LaserBoy_frame_set    out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(size_t i = 0; i < p_frame_set->number_of_frames(); i++)
    {
        if(inc_vecs)
            out += p_frame_set->at(i);
        else
            out.push_back(LaserBoy_frame());
        p = (p_frame_set->at(i).to_real_segment(false)).centroid_of_coordinates_xy(area, rs);
        out.back() += (rs + front);
        if(inc_cnts && area != 0.0)
        {
            c = LaserBoy_color(255, 0, 0);
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y,  32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y,  32767.0, c.r, c.g, c.b,                     0, 0));
            out.back().push_back(LaserBoy_real_vertex(     p.x, -32767.0,  32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0,  32767.0, c.r, c.g, c.b,                     0, 0));
            out.back().push_back(LaserBoy_real_vertex(-32767.0,      p.y,  32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y,  32767.0, c.r, c.g, c.b,                     0, 0));
        }
        p = (p_frame_set->at(i).to_real_segment(false)).centroid_of_coordinates_zy(area, rs);
        out.back() += (rs + side);
        if(inc_cnts && area != 0.0)
        {
            c = LaserBoy_color(0, 255, 0);
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 24));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y,      p.z, c.r, c.g, c.b,                     0, 24));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y, -32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 24));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y,  32767.0, c.r, c.g, c.b,                     0, 24));
            out.back().push_back(LaserBoy_real_vertex( 32767.0, -32767.0,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 24));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,  32767.0,      p.z, c.r, c.g, c.b,                     0, 24));
        }
        p = (p_frame_set->at(i).to_real_segment(false)).centroid_of_coordinates_xz(area, rs);
        out.back() += (rs + top);
        if(inc_cnts && area != 0.0)
        {
            c = LaserBoy_color(0, 0, 255);
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 40));
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0,      p.z, c.r, c.g, c.b,                     0, 40));
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0, -32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 40));
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0,  32767.0, c.r, c.g, c.b,                     0, 40));
            out.back().push_back(LaserBoy_real_vertex(-32767.0,  32767.0,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 40));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,  32767.0,      p.z, c.r, c.g, c.b,                     0, 40));
        }
        if(inc_cnts)
        {
            p = (p_frame_set->at(i).to_real_segment(false)).centroid_of_coordinates();
            c = LaserBoy_color(255, 255, 0);
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 16));
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y,      p.z, c.r, c.g, c.b,                     0, 16));
            out.back().push_back(LaserBoy_real_vertex(-32767.0,      p.y,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 16));
            out.back().push_back(LaserBoy_real_vertex( 32767.0,      p.y,      p.z, c.r, c.g, c.b,                     0, 16));
            out.back().push_back(LaserBoy_real_vertex(     p.x, -32767.0,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 16));
            out.back().push_back(LaserBoy_real_vertex(     p.x,  32767.0,      p.z, c.r, c.g, c.b,                     0, 16));
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y, -32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 16));
            out.back().push_back(LaserBoy_real_vertex(     p.x,      p.y,  32767.0, c.r, c.g, c.b,                     0, 16));
        }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds kaleidoscope(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count,
                        number_of_reflection = 0,
                        reflection;
    LaserBoy_3D_double  a;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out,
                        copy;
    //------------------------------------------------------------------------
    while(number_of_reflection == 0)
    {
        number_of_reflection = GUI.display_prompt_and_echo_u_int(   "Number of reflections [7]"
                                                                  , 7
                                                                  , 32767
                                                                );
        if(GUI.prompt_escaped)
        {
            p_space->display_error("number of reflections must be 1 or more!");
            return LASERBOY_EFFECT_PROMPT_ESC;
        }
    }
    p_space->display_please_wait();
    //------------------------------------------------------------------------
    copy = *(p_frame_set);
    copy.normalize_vectors_with_origin(32760.0 / copy.max_distance_from_origin_xy());
    a.z = pi / (number_of_reflection / 2.0);
    //------------------------------------------------------------------------
    for(frame_count = 0; frame_count < copy.number_of_frames(); frame_count++)
    {
        out += copy.at(frame_count);
        for(reflection = 1; reflection < number_of_reflection; reflection++)
        {
            frame = copy.at(frame_count);
            frame.rotate_around_origin(a * reflection);
            out[frame_count] += frame;
        }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds gyro(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count;
    LaserBoy_3D_double  a;
    LaserBoy_frame_set  out(*p_frame_set);
    //------------------------------------------------------------------------
    a.x    = GUI.display_prompt_and_echo_double("rotate degrees around X ? [1.0]", 1.0, 180.0, -180.0) * one_degree;
    a.y    = GUI.display_prompt_and_echo_double("rotate degrees around Y ? [1.0]", 1.0, 180.0, -180.0) * one_degree;
    a.z    = GUI.display_prompt_and_echo_double("rotate degrees around Z ? [1.0]", 1.0, 180.0, -180.0) * one_degree;
    p_space->display_please_wait();
    out.normalize_vectors_with_origin(32760.0 / out.max_distance_from_origin());
    for(frame_count = 0; frame_count < out.number_of_frames(); frame_count++)
        out.at(frame_count).rotate_around_origin(a * frame_count);
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds glitter(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count,
                        vertex_count;
    LaserBoy_frame      frame_1,
                        frame_2;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    frame_1.promote_to_true_color();
    frame_2.promote_to_true_color();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame_1 = p_frame_set->at(frame_count);
        frame_2.clear();
        for(vertex_count = 0; vertex_count < frame_1.size(); vertex_count++)
        {
            frame_2.push_back(frame_1.at(vertex_count));
            if(frame_1.at(vertex_count).is_color(0))
            {
                frame_2.push_back(frame_1.at(vertex_count));
                frame_2.back().r =
                frame_2.back().g =
                frame_2.back().b = 255;
                frame_2.push_back(frame_1.at(vertex_count));
                frame_2.back().blank();
            }
        }
        out += frame_2;
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rocker(LaserBoy_frame_set* p_frame_set)
{
    bool                       flip_flop = true;
    int                        i = 0;
    u_int                      frame_count;
    LaserBoy_3D_double         a(0, 0, one_degree);
    LaserBoy_real_segment      rs;
    LaserBoy_real_segment_set  rss;
    LaserBoy_frame_set         out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        rs = p_frame_set->at(frame_count).to_real_segment(true);
        rs.rotate_around_origin(a * i);
        if(!((frame_count - 45) % 90))
            flip_flop = !flip_flop;
        if(flip_flop)
            i++;
        else
            i--;
        rss += rs;
    }
    rss.normalize_vectors_with_origin();
    out = rss;
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds echos(LaserBoy_frame_set* p_frame_set)
{
    u_int               i,
                        echo      = 0,
                        delay     = 0,
                        num_echos = 0;
    LaserBoy_frame      empty_frame(p_frame_set->frame_set_palette_index(), true), // not really!
                        frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    while(delay == 0)
    {
        delay = GUI.display_prompt_and_echo_u_int(   "delay in frames [1]"
                                                   , 1
                                                   , GUI.space->number_of_frames() - 1
                                                 );
        if(GUI.prompt_escaped)
        {
            p_space->display_error("delay in frames must be 1 or more!");
            return LASERBOY_EFFECT_PROMPT_ESC;
        }
    }
    while(num_echos == 0)
    {
        num_echos = GUI.display_prompt_and_echo_u_int(   "number of echos [10]"
                                                       , 10
                                                       , 32767
                                                     );
        if(GUI.prompt_escaped)
        {
            p_space->display_error("number of echos must be 1 or more!");
            return LASERBOY_EFFECT_PROMPT_ESC;
        }
    }
    bool cycle_colors = GUI.display_prompt_and_echo_bool("shift echoed frame's palette indexes: ? (y) | [any]");
    p_space->display_please_wait();
    for(i = 0; i < (p_frame_set->number_of_frames() + (delay * num_echos)); i++)
    {
        if(i < p_frame_set->number_of_frames())
            out += p_frame_set->at(i);
        else
            out += empty_frame;
        for(echo = 1; echo <= num_echos; echo++)
            if(    i >= (delay * echo)
                && (i - delay * echo < p_frame_set->number_of_frames())
              )
            {
                frame = p_frame_set->at(i - delay * echo);
                if(cycle_colors)
                    frame.rotate_colors(echo);
                out[i] += frame;
            }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds move_l2r(LaserBoy_frame_set* p_frame_set)
{
    bool                clipping_was = p_space->destructive_clipping;
    int                 i,
                        j,
                        frame_index;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    p_space->destructive_clipping = true;
    frame_index = in.number_of_frames() - 1;
    d.x = -662;
    for(i = 0; i < 100; i++)
    {
        frame = in.at(frame_index);
        frame.minimize(0);
        for(j = 0; j < i; j++)
            frame.move(d);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x <= LASERBOY_MIN_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(--frame_index < 0)
            frame_index = in.number_of_frames() - 1;
    }
    out.reverse();
    //------------------------------------------------------------------------
    d.x = 662;
    frame_index = 0;
    for(i = 1; i <= 100; i++)
    {
        frame = in.at(frame_index);
        frame.minimize(0);
        for(j = 0; j < i; j++)
            frame.move(d);
        for(j = 0; j < (int)frame.size(); j++)
            if(frame.at(j).x >= LASERBOY_MAX_SHORT)
                frame.set_vertex_to_black(j);
        out += frame;
        if(++frame_index >= (int)in.number_of_frames())
            frame_index = 0;
    }
    //------------------------------------------------------------------------
    p_space->destructive_clipping = clipping_was;
    out.minimize();
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds move_t2b(LaserBoy_frame_set* p_frame_set)
{
    bool                clipping_was = p_space->destructive_clipping;
    int                 i,
                        j,
                        frame_index;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    p_space->destructive_clipping = true;
    frame_index = in.number_of_frames() - 1;
    d.y = 662;
    for(i = 0; i < 100; i++)
    {
        frame = in.at(frame_index);
        frame.minimize(0);
        for(j = 0; j < i; j++)
            frame.move(d);
        out += frame;
        if(--frame_index < 0)
            frame_index = in.number_of_frames() - 1;
    }
    out.reverse();
    //------------------------------------------------------------------------
    frame_index = 0;
    d.y = -662;
    for(i = 1; i <= 100; i++)
    {
        frame = in.at(frame_index);
        frame.minimize(0);
        for(j = 0; j < i; j++)
            frame.move(d);
        out += frame;
        if(++frame_index >= (int)in.number_of_frames())
            frame_index = 0;
    }
    //------------------------------------------------------------------------
    p_space->destructive_clipping = clipping_was;
    out.minimize();
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds vortex(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    LaserBoy_3D_double  s(0.80, 0.80, 0.80);
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 19; shrink_frame_count++)
        {
            out += frame;
            frame.scale_around_origin(s);
        }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds fader(LaserBoy_frame_set* p_frame_set)
{
    double              frame_count,
                        total = p_frame_set->number_of_frames();
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        frame.shade((u_char)(frame_count / total * 255.0));
        out += frame;
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds fader_z(LaserBoy_frame_set* p_frame_set)
{
    u_int               i,
                        shade;
    double              frame_count,
                        total = p_frame_set->number_of_frames();
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        frame.promote_to_true_color();
        for(i = 1; i < frame.size(); i++)
        {
            shade = (int)(255 - (frame.at(i).z + 22768.0) / 45536.0 * 255.0);
            if(shade > 254)    shade = 255;
            if(shade < 1  )    shade = 0  ;
            (frame.at(i).r - shade > 0) ? (frame.at(i).r -= shade) : (frame.at(i).r = 0);
            (frame.at(i).g - shade > 0) ? (frame.at(i).g -= shade) : (frame.at(i).g = 0);
            (frame.at(i).b - shade > 0) ? (frame.at(i).b -= shade) : (frame.at(i).b = 0);
        }
        out += frame;
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds depth(LaserBoy_frame_set* p_frame_set)
{
    u_int               i;
    double              frame_count,
                        scale,
                        total = p_frame_set->number_of_frames();
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < total; frame_count++)
    {
        frame = p_frame_set->at((u_int)frame_count);
        for(i = 0; i < frame.size(); i++)
        {
            scale = 0.8 + (frame.at(i).z + 32768.0) / 131077.0;
            frame.at(i).x = (short)(frame.at(i).x * scale);
            frame.at(i).y = (short)(frame.at(i).y * scale);
            frame.at(i).z = (short)(frame.at(i).z * scale);
        }
        out += frame;
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds drain(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    LaserBoy_3D_double  s(0.85, 0.85, 0.85),
                        a(0.0,  0.0,  pi/15.0);
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 29; shrink_frame_count++)
        {
            out += frame;
            frame.scale_around_origin(s);
            frame.rotate_around_origin(a);
        }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds diminish(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count,
                        shrink_frame_count;
    LaserBoy_3D_double  s(0.75, 0.75, 0.75);
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        for(shrink_frame_count = 0; shrink_frame_count < 19; shrink_frame_count++)
        {
            out += frame;
            frame.scale(s);
        }
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds rainbow(LaserBoy_frame_set* p_frame_set)
{
    u_int               frame_count;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    for(frame_count = 0; frame_count < p_frame_set->number_of_frames(); frame_count++)
    {
        frame = p_frame_set->at(frame_count);
        frame.rotate_colors(frame_count);
        out += frame;
    }
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds half_cylinder(LaserBoy_frame_set* p_frame_set)
{
    int                 i,
                        j;
    double              angle;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        frame.palette_index = in[i].palette_index;
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle = (((in[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            frame += LaserBoy_vertex(cos(angle) * 32767.0,
                                     in[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds cylinder(LaserBoy_frame_set* p_frame_set)
{
    int                 i,
                        j;
    double              angle;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        frame.palette_index = in[i].palette_index;
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle = (((in[i].at(j).x + 32767.0) / 32767.0) * pi) + pi; // pi to 3pi
            frame += LaserBoy_vertex(cos(angle) * 32767.0,
                                     in[i].at(j).y,
                                     sin(angle) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds hemisphere(LaserBoy_frame_set* p_frame_set)
{
    int                 i,
                        j;
    double              angle_theta,
                        angle_row;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        frame.palette_index = in[i].palette_index;
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle_theta = (((in[i].at(j).x + 32767.0) / 65535.0) * pi) + pi; // pi to 2pi
            angle_row   = (((in[i].at(j).y) / 65535.0) * pi); // -pi to pi
            frame += LaserBoy_vertex(cos(angle_row) * cos(angle_theta) * 32767.0,
                                     sin(angle_row) * 32767.0,
                                     cos(angle_row) * sin(angle_theta) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds sphere(LaserBoy_frame_set* p_frame_set)
{
    int                 i,
                        j;
    double              angle_theta,
                        angle_row;
    LaserBoy_3D_double  d;
    LaserBoy_frame      frame;
    LaserBoy_frame_set  in(*p_frame_set),
                        out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    in.flatten_z();
    for(i = 0; i < (int)in.number_of_frames(); i++)
    {
        frame.clear();
        frame.palette_index = in[i].palette_index;
        for(j = 0; j < (int)in[i].size(); j++)
        {
            angle_theta = (((in[i].at(j).x + 32767.0) / 32767.0) * pi) + pi; // pi to 4pi
            angle_row   = (((in[i].at(j).y) / 65535.0) * pi); // -pi to pi
            frame += LaserBoy_vertex(cos(angle_row) * cos(angle_theta) * 32767.0,
                                     sin(angle_row) * 32767.0,
                                     cos(angle_row) * sin(angle_theta) * -32767.0,
                                     in[i].at(j).r,
                                     in[i].at(j).g,
                                     in[i].at(j).b,
                                     in[i].at(j).k,
                                     in[i].at(j).c
                                    );
        }
        out += frame;
    }
    //------------------------------------------------------------------------
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds clip_negative_z(LaserBoy_frame_set* p_frame_set)
{
    bool                clipping_was = p_space->destructive_clipping;
    LaserBoy_3D_double  d(0.0, 0.0, 32767.0);
    LaserBoy_frame      frame;
    LaserBoy_frame_set  out;
    //------------------------------------------------------------------------
    p_space->display_please_wait();
    p_space->destructive_clipping = true;
    for(int i = 0; i < (int)p_frame_set->number_of_frames(); i++)
    {
        frame = p_frame_set->at(i);
        frame.move(-d);
        frame.move( d);
        out += frame;
    }
    p_space->destructive_clipping = clipping_was;
    out.reduce_blank_vectors();
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds quadrants(LaserBoy_frame_set* p_frame_set)
{
    int                        i,
                               x,
                               y;
    LaserBoy_real_segment      rs;
    LaserBoy_real_segment_set  rss;
    LaserBoy_frame_set         out;
    //------------------------------------------------------------------------
    bool mirror = GUI.display_prompt_and_echo_bool("mirror on axis ? : y | [any]");
    p_space->display_please_wait();
    rss.reserve(p_frame_set->number_of_frames());
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        rss.push_back(rs);
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        for(x = 0; x < 2; x++)
            for(y = 0; y < 2; y++)
            {
                rs =  p_frame_set->at(i).to_real_segment(true);
                if(mirror && x == 1)    rs.flip(0);
                if(mirror && y == 1)    rs.flip(1);
                rs.move(LaserBoy_3D_double(x * -65535.0, y * -65535.0, 0.0));
                rss.at(i) += rs;
            }
    rss.normalize_vectors();
    out = rss;
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds n_squared(LaserBoy_frame_set* p_frame_set)
{
    int                        i,
                               n = 0,
                               x,
                               y;
    LaserBoy_real_segment      rs(p_frame_set->frame_set_palette_index(), false);
    LaserBoy_real_segment_set  rss;
    LaserBoy_frame_set         out;
    //------------------------------------------------------------------------
    while(n == 0 || n == 1)
    {
        n = GUI.display_prompt_and_echo_u_int(   "n [4]"
                                               , 4
                                               , 30
                                             );
        if(GUI.prompt_escaped)
        {
            p_space->display_error("n must be between 2 and 30.");
            return LASERBOY_EFFECT_PROMPT_ESC;
        }
    }
    p_space->display_please_wait();
    rss.reserve(p_frame_set->number_of_frames());
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        rss.push_back(rs); // empty frames
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        for(x = 0; x < n; x++)
            for(y = 0; y < n; y++)
            {
                rs =  p_frame_set->at(i).to_real_segment(true);
                rs.move(LaserBoy_3D_double(x * -65535.0, y * -65535.0, 0.0));
                rss.at(i) += rs;
            }
    rss.normalize_vectors();
    out = rss;
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds n_cubed(LaserBoy_frame_set* p_frame_set)
{
    int                        i,
                               n = 0,
                               x,
                               y,
                               z;
    LaserBoy_real_segment      rs(p_frame_set->frame_set_palette_index(), false);
    LaserBoy_real_segment_set  rss;
    LaserBoy_frame_set         out;
    //------------------------------------------------------------------------
    while(n == 0 || n == 1)
    {
        n = GUI.display_prompt_and_echo_u_int(   "n [3]"
                                               , 3
                                               , 10
                                             );
        if(GUI.prompt_escaped)
        {
            p_space->display_error("n must be between 2 and 10.");
            return LASERBOY_EFFECT_PROMPT_ESC;
        }
    }
    p_space->display_please_wait();
    rss.reserve(p_frame_set->number_of_frames());
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        rss.push_back(rs); // empty frames
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        for(x = 0; x < n; x++)
            for(y = 0; y < n; y++)
                for(z = 0; z < n; z++)
                {
                    rs =  p_frame_set->at(i).to_real_segment(true);
                    rs.move(LaserBoy_3D_double(x * -65535.0, y * -65535.0, z * -65535.0));
                    rss.at(i) += rs;
                }
    rss.normalize_vectors();
    out = rss;
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds octants(LaserBoy_frame_set* p_frame_set)
{
    int                        i,
                               x,
                               y,
                               z;
    LaserBoy_real_segment      rs;
    LaserBoy_real_segment_set  rss;
    LaserBoy_frame_set         out;
    //------------------------------------------------------------------------
    bool mirror = GUI.display_prompt_and_echo_bool("mirror on axis ? : y | [any]");
    p_space->display_please_wait();
    rss.reserve(p_frame_set->number_of_frames());
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        rss.push_back(rs);
    for(i = 0; i < (int)p_frame_set->number_of_frames(); i++)
        for(x = 0; x < 2; x++)
            for(y = 0; y < 2; y++)
                for(z = 0; z < 2; z++)
                {
                    rs =  p_frame_set->at(i).to_real_segment(true);
                    if(mirror && x == 1)    rs.flip(0);
                    if(mirror && y == 1)    rs.flip(1);
                    if(mirror && z == 1)    rs.flip(2);
                    rs.move(LaserBoy_3D_double(x * -65535.0, y * -65535.0, z * -65535.0));
                    rss.at(i) += rs;
                }
    rss.normalize_vectors();
    out = rss;
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds chil(LaserBoy_frame_set* p_frame_set)
{
    int                        i,
                               j,
                               k;
    LaserBoy_3D_double         offset(0.0, 0.0, 1000.0); // 1000 points off the Z axis
    LaserBoy_frame             frame; // a temp container to build a frame out of individual vertices
    LaserBoy_frame_set         in(*p_frame_set), // a copy of the currently loaded frame_set
                               out; // a container for the resulting modified frame_set
    //-----------------------------------------------------------------------
    p_space->display_please_wait();
    for(i = 0; i < (int)in.number_of_frames(); i++) // i is the index of each frame in the set
    {
        frame.clear(); // empty the temp frame container
        if(in[i].size() > 1) // if this frame has vertices
        {
            for(k = 0; k < (int)round(p_space->dwell_on_dot); k++)
                frame.push_back(in[i][0]); // add the original vertex of this frame to the temp frame
            //-----------------------------------------------------------------
            for(j = 1; j < (int)in[i].size(); j++) // j is every vertex in this frame after the zeroth
            {
                // if this vertex is lit and the previous one is not
                if(    in[i][j - 1].is_blank() // look at the previous vertex
                    && in[i][j].is_lit()
                  )
                {
                    for(k = 0; k < (int)round(p_space->dwell_on_dot); k++)
                        frame += in[i][j - 1];
                }
                //-------------------------------------------------------------
                // if this vertex is blank and the previous one is not
                else if(    in[i][j - 1].is_lit() // look at the previous vertex
                         && in[i][j].is_blank()
                       )
                {
                    for(k = 0; k < (int)round(p_space->dwell_on_dot); k++)
                        frame += in[i][j - 1];
                }
                //-------------------------------------------------------------
                frame += in[i][j]; // no matter what, add this vertex to the frame
            } // end for(j = 1; j < (int)in[i].size(); j++)
            //-----------------------------------------------------------------
            for(k = 0; k < (int)round(p_space->dwell_on_dot); k++)
                frame += in[i].back(); // add the last vertex to the frame (again)
        } // end if(in[i].size() > 1)
        //-----------------------------------------------------------------
        out += frame; // add this frame to the new resulting frame_set
    } // end for(i = 0; i < (int)in.number_of_frames(); i++)
    //-------------------------------------------------------------------------
    out.best_match_palette(LASERBOY_ILDA_TTL); // reduce to 7 color TTL
    out.to_palette_by_index(LASERBOY_ILDA_DEFAULT); // go back to using default palette of 63 colors
    //-------------------------------------------------------------------------
    for(i = 0; i < (int)out.number_of_frames(); i++) // i is the index of each frame in the set (of out)
    {
        if(out[i].is_2D())
            out[i].move(offset); // make it 3D (format 0)
    }
    //-------------------------------------------------------------------------
    finish_effect(p_frame_set, out);
    return LASERBOY_IN_BOUNDS; // there is no way this new frame_set could be out of bounds.
}

//############################################################################
void LaserBoy_space::load_frame_set_effects()
{
    //------------------------------------------------------------------------
    frame_set_effects.push_back(chil);
    frame_set_effect_names.push_back(string("chil"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(clip_negative_z);
    frame_set_effect_names.push_back(string("clip_negative_z"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(cylinder);
    frame_set_effect_names.push_back(string("cylinder"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(depth);
    frame_set_effect_names.push_back(string("depth"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(diminish);
    frame_set_effect_names.push_back(string("diminish"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(drain);
    frame_set_effect_names.push_back(string("drain"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(echos);
    frame_set_effect_names.push_back(string("echos"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(fader);
    frame_set_effect_names.push_back(string("fader"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(fader_z);
    frame_set_effect_names.push_back(string("fader_z"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(glitter);
    frame_set_effect_names.push_back(string("glitter"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(gyro);
    frame_set_effect_names.push_back(string("gyro"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(half_cylinder);
    frame_set_effect_names.push_back(string("half_cylinder"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(hemisphere);
    frame_set_effect_names.push_back(string("hemisphere"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(kaleidoscope);
    frame_set_effect_names.push_back(string("kaleidoscope"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(move_l2r);
    frame_set_effect_names.push_back(string("move_l2r"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(move_t2b);
    frame_set_effect_names.push_back(string("move_t2b"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(n_cubed);
    frame_set_effect_names.push_back(string("n_cubed"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(n_squared);
    frame_set_effect_names.push_back(string("n_squared"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(octants);
    frame_set_effect_names.push_back(string("octants"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(outlines);
    frame_set_effect_names.push_back(string("outlines"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(quadrants);
    frame_set_effect_names.push_back(string("quadrants"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(rainbow);
    frame_set_effect_names.push_back(string("rainbow"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(rocker);
    frame_set_effect_names.push_back(string("rocker"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(sphere);
    frame_set_effect_names.push_back(string("sphere"));
    //------------------------------------------------------------------------
    frame_set_effects.push_back(vortex);
    frame_set_effect_names.push_back(string("vortex"));
    //------------------------------------------------------------------------
    return;
}

//############################################################################
#undef p_space
#undef GUI

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
