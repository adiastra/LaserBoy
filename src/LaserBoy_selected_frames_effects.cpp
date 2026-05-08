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
void finish_effect(LaserBoy_frame_set *p_frame_set, LaserBoy_frame_set& out);

//############################################################################
LaserBoy_Bounds lava(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               frame_count,
                            color_index,
                            vertex_index;
        int                 vertex_count;
        LaserBoy_3D_short   temp_position;
        LaserBoy_frame      frame,
                            object = p_frame_set->selected_frame_index(0),
                            colors = p_frame_set->selected_frame_index(1);
        LaserBoy_frame_set  out;
        //----------------------------------------------------------------
        p_space->display_please_wait();
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = vertex_count - (object.size() - frame_count + 1);
                vertex_index = vertex_count;
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //----------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)(object.size() + colors.size());
                vertex_count++
               )
            {
                color_index = vertex_count - (object.size() - frame_count + 1);
                vertex_index = vertex_count % object.size();
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //----------------------------------------------------------------
        for(frame_count = 0; frame_count < colors.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = colors.size() - (frame_count + 1);
                vertex_count >= 0;
                vertex_count--
               )
            {
                color_index = (colors.size() - 1) - (colors.size() - (frame_count + 1) - vertex_count);
                if(vertex_count < (int)object.size())
                {
                    frame.at(vertex_count).r = colors.at(color_index).r;
                    frame.at(vertex_count).g = colors.at(color_index).g;
                    frame.at(vertex_count).b = colors.at(color_index).b;
                    frame.at(vertex_count).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_count).blank();
                }
            }
            out += frame;
        }
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
        //----------------------------------------------------------------
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds lava2(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               frame_count,
                            color_index,
                            vertex_index;
        int                 vertex_count;
        LaserBoy_3D_short   temp_position;
        LaserBoy_frame      frame,
                            object = p_frame_set->selected_frame_index(0),
                            colors = p_frame_set->selected_frame_index(1);
        LaserBoy_frame_set  out;
        //----------------------------------------------------------------
        p_space->display_please_wait();
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - frame_count + 1;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = (   vertex_count
                                - (   object.size()
                                    - frame_count
                                    + 1
                                  )
                              )
                              % colors.size();

                vertex_index = vertex_count;
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //----------------------------------------------------------------
        for(frame_count = 0; frame_count < colors.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = 0;
                vertex_count < (int)object.size();
                vertex_count++
               )
            {
                color_index = (   (object.size() - 1)
                                - (object.size() - (frame_count + 1) - vertex_count)
                              )
                              % colors.size();
                vertex_index = vertex_count % object.size();
                if(color_index < colors.size())
                {
                    frame.at(vertex_index).r = colors.at(color_index).r;
                    frame.at(vertex_index).g = colors.at(color_index).g;
                    frame.at(vertex_index).b = colors.at(color_index).b;
                    frame.at(vertex_index).c = colors.at(color_index).c;
                    if(colors.at(color_index).is_blank())
                        frame.at(vertex_index).blank();
                }
            }
            out += frame;
        }
        //----------------------------------------------------------------
        for(frame_count = 0; frame_count < object.size(); frame_count++)
        {
            frame = object;
            frame.palette_index = colors.palette_index;
            frame.blacken_vertices();
            for(vertex_count = object.size() - (frame_count + 1);
                vertex_count >= 0;
                vertex_count--
               )
            {
                color_index = (   (object.size() - 1)
                                - (object.size() - (frame_count + 1) - vertex_count)
                              )
                              % colors.size();
                vertex_index = vertex_count % object.size();
                frame.at(vertex_index).r = colors.at(color_index).r;
                frame.at(vertex_index).g = colors.at(color_index).g;
                frame.at(vertex_index).b = colors.at(color_index).b;
                frame.at(vertex_index).c = colors.at(color_index).c;
                if(colors.at(color_index).is_blank())
                    frame.at(vertex_index).blank();
            }
            out += frame;
        }
        //----------------------------------------------------------------
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds gears(LaserBoy_frame_set* p_frame_set)
{ // requires a frame_set with three selected frames (Try in.ild, 1, 2 & 3)
    p_space->display_please_wait();
    if(p_frame_set->number_of_selected_frames() > 2)
    {
        p_space->display_please_wait();
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_frame_set  out;
        LaserBoy_frame g20 = p_frame_set->selected_frame_index(0);
        LaserBoy_frame g16 = p_frame_set->selected_frame_index(1);
        LaserBoy_frame g12 = p_frame_set->selected_frame_index(2);
        LaserBoy_frame f20;
        LaserBoy_frame f16;
        LaserBoy_frame f12;
        LaserBoy_3D_double a(0, 0, -pi/60);
        LaserBoy_3D_double b(0, 0,  pi/48);
        LaserBoy_3D_double c(0, 0,  pi/36);
        //----------------------------------------------------------------
        g20.scale_around_origin(LaserBoy_3D_double(0.500, 0.500, 0.500));
        g16.scale_around_origin(LaserBoy_3D_double(0.375, 0.375, 0.375));
        g12.scale_around_origin(LaserBoy_3D_double(0.300, 0.300, 0.300));
        g20.move(LaserBoy_3D_short(-10000,      0, 0));
        g16.move(LaserBoy_3D_short( 12200, -15400, 0));
        g12.move(LaserBoy_3D_short(  5150,  19300, 0));

        out += (g20 + g16 + g12);
        for(int i = 1; i < 480; i++)
        {
            f20 = g20;
            f16 = g16;
            f12 = g12;
            out_of_bounds |= f20.rotate(a * i);
            out_of_bounds |= f16.rotate(b * i);
            out_of_bounds |= f12.rotate(c * i);
            if(out_of_bounds)
                return out_of_bounds;
            out += (f20 + f16 + f12);
        }
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds blend(LaserBoy_frame_set* p_frame_set)
{ // requires a frame_set with at least two selected frames
    p_space->display_please_wait();
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        u_int               i,
                            k;
        LaserBoy_frame      f1,
                            f2,
                            frame;
        LaserBoy_frame_set  in(p_frame_set->selected_frames()),
                            out;
        p_space->display_please_wait();
        //----------------------------------------------------------------
        for(k = 0; k < in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            frame.clear();
            //----------------------------------------------------------------
            for(i = 0; i < 10; i++)
                out += f1;
            for(i = 0; i < 100; i++)
            {
                frame = f1.blend(f2, i / 99.0);
                out += frame;
            }
            //----------------------------------------------------------------
        }
        f1 = in[in.size() - 1];
        f2 = in[0];
        frame.clear();
        //----------------------------------------------------------------
        for(i = 0; i < 10; i++)
            out += f1;
        for(i = 0; i < 100; i++)
        {
            frame = f1.blend(f2, i / 99.0);
            out += frame;
        }
        //----------------------------------------------------------------
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds wipe(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        LaserBoy_frame      f1,
                            f2;
        LaserBoy_frame_set  s1,
                            s2,
                            in(p_frame_set->selected_frames()),
                            out;
        //----------------------------------------------------------------
        p_space->display_please_wait();
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(f1.at(j).x <= ((i * 661.98) - 32767.0))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            //----------------------------------------------------------------
            f2.flip(0);
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(f2.at(j).x <= ((i * 661.98) - 32767.0))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            //----------------------------------------------------------------
            s2.flip(0);
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //----------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //----------------------------------------------------------------
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(f1.at(j).x <= ((i * 661.98) - 32767.0))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        //----------------------------------------------------------------
        f2.flip(0);
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(f2.at(j).x <= ((i * 661.98) - 32767.0))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        //----------------------------------------------------------------
        s2.flip(0);
        s2.reverse();
        //----------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //----------------------------------------------------------------
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds radial_wipe(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        LaserBoy_frame      f1,
                            f2;
        LaserBoy_frame_set  s1,
                            s2,
                            in(p_frame_set->selected_frames()),
                            out;
        //----------------------------------------------------------------
        p_space->display_please_wait();
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(f1.at(j).magnitude() <= (i * 468.09))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            //----------------------------------------------------------------
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(f2.at(j).magnitude() >= (i * 468.09))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //----------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //----------------------------------------------------------------
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(f1.at(j).magnitude() <= (i * 468.09))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        //----------------------------------------------------------------
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(f2.at(j).magnitude() >= (i * 468.09))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        s2.reverse();
        //----------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //----------------------------------------------------------------
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds angle_wipe(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 1)
    {
        int                 i,
                            j,
                            k;
        LaserBoy_frame      f1,
                            f2;
        LaserBoy_frame_set  s1,
                            s2,
                            in(p_frame_set->selected_frames()),
                            out;
        //----------------------------------------------------------------
        p_space->display_please_wait();
        for(k = 0; k < (int)in.size() - 1; k++)
        {
            f1 = in[k];
            f2 = in[k + 1];
            //----------------------------------------------------------------
            f1.flip(0);
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f1.size(); j++)
                    if(((atan2(f1.at(j).y, f1.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                        f1.set_vertex_to_black(j);
                s1 += f1;
            }
            s1.flip(0);
            //----------------------------------------------------------------
            f2.quarter_turn(0, 2);
            for(i = 99; i >= 0; i--)
            {
                for(j = 0; j < (int)f2.size(); j++)
                    if(((atan2(f2.at(j).y, f2.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                        f2.set_vertex_to_black(j);
                s2 += f2;
            }
            s2.quarter_turn(0, 2);
            s2.reverse();
            //----------------------------------------------------------------
            for(i = 0; i < 100; i++)
                out += (s1[i] + s2[i]);
            //----------------------------------------------------------------
            s1.clear();
            s2.clear();
        }
        //----------------------------------------------------------------
        f1 = in[in.size() - 1];
        f2 = in[0];
        //----------------------------------------------------------------
        f1.flip(0);
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f1.size(); j++)
                if(((atan2(f1.at(j).y, f1.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                    f1.set_vertex_to_black(j);
            s1 += f1;
        }
        s1.flip(0);
        //----------------------------------------------------------------
        f2.quarter_turn(0, 2);
        for(i = 99; i >= 0; i--)
        {
            for(j = 0; j < (int)f2.size(); j++)
                if(((atan2(f2.at(j).y, f2.at(j).x) / two_pi) + 0.5) >= (i / 99.0))
                    f2.set_vertex_to_black(j);
            s2 += f2;
        }
        s2.quarter_turn(0, 2);
        s2.reverse();
        //----------------------------------------------------------------
        for(i = 0; i < 100; i++)
            out += (s1[i] + s2[i]);
        //----------------------------------------------------------------
        finish_effect(p_frame_set, out);
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
LaserBoy_Bounds film_strip(LaserBoy_frame_set* p_frame_set)
{
    if(p_frame_set->number_of_selected_frames() > 2)
    {
        char  number[12];
        u_int i = p_frame_set->number_of_selected_frames() * 128 + 129;
        sprintf(number, "%d", (int)i);
        if(    p_frame_set->number_of_selected_frames() <= 40
            || GUI.display_prompt_and_echo_bool("this will create " + string(number) + " frames! continue ? : y | [any]")
          )
        {
            LaserBoy_3D_double           d,
                                       max( 32767.0,  32767.0,  32767.0),
                                       min(-32767.0, -32767.0, -32767.0);
            LaserBoy_real_segment     rs_1(p_frame_set->selected_frames_palette_index(), false),
                                      rs_2(p_frame_set->selected_frames_palette_index(), false);
            LaserBoy_frame_set          in,
                                       out;
            //----------------------------------------------------------------
            p_space->display_please_wait();
            in = p_frame_set->selected_frames();
            d.x = -65535.0;
            for(i = 0; i < in.number_of_frames(); i++)
            {
                rs_2 = in.at(i).to_real_segment();
                rs_2.reduce_blank_vectors();
                rs_2.reduce_lit_vectors();
                rs_1 += (rs_2 + (d * i));
            }
            d.x = -65535.0;
            rs_1 += d;
            d.x = -65535.0 / 128.0;
            for(i = 0; i < in.number_of_frames() * 128 + 129; i++)
            {
                rs_2 = rs_1;
                out += rs_2.clip_around_coordinate(d * i, max, min, 65536.0);
            }
            finish_effect(p_frame_set, out);
            return LASERBOY_IN_BOUNDS;
        }
        else
        {
            p_space->display_message("nothing done.");
            GUI.wait_4_any_key();
        }
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_NOTHING_SELECTED;
}

//############################################################################
void LaserBoy_space::load_selected_frames_effects()
{
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(angle_wipe);
    selected_frames_effect_names.push_back(string("angle_wipe"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(blend);
    selected_frames_effect_names.push_back(string("blend"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(film_strip);
    selected_frames_effect_names.push_back(string("film_strip"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(gears);
    selected_frames_effect_names.push_back(string("gears"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(lava);
    selected_frames_effect_names.push_back(string("lava"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(lava2);
    selected_frames_effect_names.push_back(string("lava2"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(radial_wipe);
    selected_frames_effect_names.push_back(string("radial_wipe"));
    //------------------------------------------------------------------------
    selected_frames_effects.push_back(wipe);
    selected_frames_effect_names.push_back(string("wipe"));
    //------------------------------------------------------------------------
    return;
}

//############################################################################
#undef p_space
#undef GUI

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
