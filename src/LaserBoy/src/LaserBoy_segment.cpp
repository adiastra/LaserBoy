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
// LaserBoy_segment.cpp is part of LaserBoy.
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
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short from,
                                   LaserBoy_vertex   to
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D line constructor, linear steps
    int steps = linear_steps(to, from, (   (to.is_lit())
                                         ? (p_space->lit_delta_max)
                                         : (p_space->blank_delta_max)
                                       )
                            );
    //------------------------------------------------------------------------
    if(steps)
    {
        LaserBoy_3D_double _to        (to), // convert 3D short to 3D double
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //--------------------------------------------------------------------
        reserve(steps);
        for(int i = 1; i <= steps; i++)
            push_back(LaserBoy_vertex(LaserBoy_3D_short(_from + (delta * i)),
                                      (LaserBoy_color)to,
                                      to.k,
                                      to.c
                                     )
                     );
    }
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short from,
                                   LaserBoy_vertex   to,
                                   const u_int&      max_d
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D line constructor, linear steps
    int steps = linear_steps(to, from, max_d);
    //------------------------------------------------------------------------
    if(steps)
    {
        LaserBoy_3D_double _to        (to), // convert 3D short to 3D double
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //--------------------------------------------------------------------
        reserve(steps);
        for(int i = 1; i <= steps; i++)
            push_back(LaserBoy_vertex(LaserBoy_3D_short(_from + (delta * i)),
                                      (LaserBoy_color)to,
                                      to.k,
                                      to.c
                                     )
                     );
    }
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_vertex p0,
                                   LaserBoy_vertex p1,
                                   string          font,
                                   string          text
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (p_space->current_frame().palette_index),
                  segment_error          (LASERBOY_OK)
{   // mono spaced font constructor
    //------------------------------------------------------------------------
    wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    //------------------------------------------------------------------------
    size_t                     i;
    long int                   bytes_skipped = 0;
    LaserBoy_ild_header_count  counter;
    u32string                  unicode_char_list;
    LaserBoy_frame_set         font_frames;
    //--------------------------------------------------------------------
    font_frames.from_ild_file(font, bytes_skipped, counter);
    if(font_frames.get_unicode_index(unicode_char_list))
        unicode_char_list = conv_utf8_utf32.from_bytes(string("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
    //--------------------------------------------------------------------
    LaserBoy_real_segment     rs(font_frames,
                                 unicode_char_list,
                                 text,
                                 1.0,
                                 p_space->rendered_mono_font_space,
                                 0.07,
                                 p_space->bond_word,
                                 false
                                );
    //--------------------------------------------------------------------
    rs *= ((p0 | p1) / rs.width());
    rs += LaserBoy_3D_double(p0.x, p0.y, p0.z);
    rs.clip();
    for(i = 0; i < rs.size(); i++)
    {
        push_back(LaserBoy_vertex(rs[i]));
        if(rs[i].is_color())
        {
            back().c = p_space->selected_color_index;
            back().r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
            back().g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
            back().b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
        }
        else if(rs[i].is_black(p_space->black_level))
        {
            back().c = p_space->palette_picker(palette_index).black;
            back().r = 0x00;
            back().g = 0x00;
            back().b = 0x00;
        }
    }
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_vertex p0,
                                   LaserBoy_vertex p1,
                                   string          font,
                                   string          text,
                                   int = 0
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (p_space->current_frame().palette_index),
                  segment_error          (LASERBOY_OK)
{   // variable spaced font constructor
    //------------------------------------------------------------------------
    wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    //------------------------------------------------------------------------
    size_t                     i;
    long int                   bytes_skipped = 0;
    LaserBoy_ild_header_count  counter;
    u32string                  unicode_char_list;
    LaserBoy_frame_set         font_frames;
    //------------------------------------------------------------------------
    font_frames.from_ild_file(font, bytes_skipped, counter);
    if(font_frames.get_unicode_index(unicode_char_list))
        unicode_char_list = conv_utf8_utf32.from_bytes(string("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"));
    //------------------------------------------------------------------------
    LaserBoy_real_segment     rs(font_frames,
                                 unicode_char_list,
                                 text,
                                 1.0,
                                 p_space->rendered_vari_font_gap,
                                 p_space->rendered_vari_font_space,
                                 0.07,
                                 p_space->bond_word,
                                 false
                                );
    //----------------------------------------------------------------
    rs *= ((p0 | p1) / rs.width());
    rs += LaserBoy_3D_double(p0.x, p0.y, p0.z);
    rs.clip();
    for(i = 0; i < rs.size(); i++)
    {
        push_back(LaserBoy_vertex(rs[i]));
        if(rs[i].is_color())
        {
            back().c = p_space->selected_color_index;
            back().r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
            back().g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
            back().b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
        }
        else if(rs[i].is_black(p_space->black_level))
        {
            back().c = p_space->palette_picker(palette_index).black;
            back().r = 0x00;
            back().g = 0x00;
            back().b = 0x00;
        }
    }
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   LaserBoy_3D_short arc_start,
                                   double            arc_angle
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D circular arc constructor
    while(arc_angle <= 0)
        arc_angle += 360;
    size_t total_vertices = (size_t)ceil(arc_angle / p_space->rendered_arc_angle);
    double arc_step       = arc_angle / total_vertices,
           radius         = center | arc_start, // distance between
           y_rotation     = atan2(arc_start.z - center.z, arc_start.x - center.x),
           z_rotation;
    LaserBoy_real_vertex  rv(arc_start);
    LaserBoy_real_segment rs;
    //------------------------------------------------------------------------
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rv.blank();
    rs.push_back(LaserBoy_real_vertex(rotate_vertex_on_coordinates_y(rv, center, -y_rotation), (LaserBoy_color)rv));
    rv.unblank();
    //------------------------------------------------------------------------
    z_rotation = atan2(rs.back().y - center.y, rs.back().x - center.x);
    arc_step *= one_degree; // convert to radians!
    for(size_t a = 1; a <= total_vertices; a++)
    {
        rv.x = radius * cos(a * arc_step + z_rotation) +  + center.x;
        rv.y = radius * sin(a * arc_step + z_rotation) +  + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.rotate_on_coordinates_y(center, y_rotation);
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   LaserBoy_3D_short arc_start,
                                   double            arc_angle,
                                   double            radii_ratio
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D elliptical arc constructor
    while(arc_angle <= 0)
        arc_angle += 360;
    size_t total_vertices = (size_t)ceil(arc_angle / p_space->rendered_arc_angle);
    double arc_step       = arc_angle / total_vertices,
           radius_1       = center | arc_start,
           radius_2       = radius_1 * radii_ratio,
           y_rotation     = atan2(arc_start.z - center.z, arc_start.x - center.x),
           z_rotation;
    LaserBoy_real_vertex  rv(arc_start);
    LaserBoy_real_segment rs;
    //------------------------------------------------------------------------
    rv.c   = p_space->selected_color_index;
    rv.r   = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g   = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b   = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rv     = rotate_vertex_on_coordinates_y(arc_start, center, -y_rotation);
    z_rotation = atan2(rv.y - center.y, rv.x - center.x);
    rv.x   = radius_1;
    rv.y   = 0;
    rv.z   = center.z;
    rv.blank();
    rs.push_back(LaserBoy_real_vertex(rv + center));
    rv.unblank();
    arc_step *= one_degree; // convert to radians!
    //------------------------------------------------------------------------
    for(size_t a = 1; a <= total_vertices; a++)
    {
        rv.x = radius_1 * cos(a * arc_step) + center.x;
        rv.y = radius_2 * sin(a * arc_step) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.rotate_on_coordinates_z(center, z_rotation);
    rs.rotate_on_coordinates_y(center, y_rotation);
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   LaserBoy_3D_short first_vertex,
                                   u_int             number_of_sides
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D polygon constructor
    double arc_step   = two_pi / number_of_sides,
           radius     = center | first_vertex,
           y_rotation = atan2(first_vertex.z - center.z, first_vertex.x - center.x),
           z_rotation;
    LaserBoy_real_vertex  rv(first_vertex);
    LaserBoy_real_segment rs;
    rv.blank();
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rs.push_back(LaserBoy_real_vertex(rotate_vertex_on_coordinates_y(rv, center, -y_rotation), (LaserBoy_color)rv));
    rv.unblank();
    z_rotation = atan2(rs.back().y - center.y, rs.back().x - center.x);
    for(u_int a = 1; a <= number_of_sides; a++)
    {
        rv.x = radius * cos(a * arc_step + z_rotation) + center.x;
        rv.y = radius * sin(a * arc_step + z_rotation) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.rotate_on_coordinates_y(center, y_rotation);
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   LaserBoy_3D_short first_vertex,
                                   u_int             number_of_points,
                                   double            ratio
                                  )
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{   // 3D polystar constructor
    u_int  a;
    double arc_step   = two_pi / number_of_points,
           radius     = center | first_vertex,
           y_rotation = atan2(first_vertex.z - center.z, first_vertex.x - center.x),
           z_rotation;
    LaserBoy_real_vertex  rv(first_vertex);
    LaserBoy_real_segment rs;
    rv.blank();
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rs.push_back(LaserBoy_real_vertex(rotate_vertex_on_coordinates_y(rv, center, -y_rotation), (LaserBoy_color)rv));
    rv.unblank();
    z_rotation = atan2(rs.back().y - center.y, rs.back().x - center.x);
    for(a = 0; a <= number_of_points - 1; a++)
    {
        rv.x = radius * cos(a * arc_step + z_rotation) + center.x;
        rv.y = radius * sin(a * arc_step + z_rotation) + center.y;
        rv.z = center.z;
        rs += rv;
        rv.x = radius * ratio * cos(a * arc_step + arc_step / 2 + z_rotation) + center.x;
        rv.y = radius * ratio * sin(a * arc_step + arc_step / 2 + z_rotation) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rv.x = radius * cos(a * arc_step + z_rotation) + center.x;
    rv.y = radius * sin(a * arc_step + z_rotation) + center.y;
    rv.z = center.z;
    rs += rv;
    rs.rotate_on_coordinates_y(center, y_rotation);
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   u_int             radius,
                                   u_int             rhodonea_numerator,
                                   u_int             rhodonea_denominator
                                  ) // rhodonea
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{
    if(rhodonea_denominator != 0.0)
    {
        u_int  total_vertices;
        double arc_step,
               arc_angle,
               ratio = rhodonea_numerator / (double)rhodonea_denominator;
        LaserBoy_real_vertex  rv;
        LaserBoy_real_segment rs;
        arc_angle = rhodonea_denominator / greatest_common_devisor(rhodonea_numerator, rhodonea_denominator) * 360;
        total_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
        arc_step = arc_angle / total_vertices;
        arc_step *= one_degree; // convert to radians!
        rv.blank();
        rv.c = p_space->selected_color_index;
        rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
        rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
        rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
        rv.x = (short)(radius);
        rv.y = 0;
        rv.z = center.z;
        rs.push_back(LaserBoy_real_vertex(rv + center));
        rv.unblank();
        for(u_int a = 0; a <= total_vertices; a++)
        {
            rv.x = radius * cos(ratio * a * arc_step) * cos(a * arc_step) + center.x;
            rv.y = radius * cos(ratio * a * arc_step) * sin(a * arc_step) + center.y;
            rv.z = center.z;
            rs += rv;
        }
        rs.clip();
        *this = rs;
        sync_rgb_and_palette();
    }
    else
        segment_error = LASERBOY_DIVIDE_BY_ZERO_ERROR;
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(u_int             fixed_radius,
                                   LaserBoy_3D_short center,
                                   u_int             roller_radius,
                                   int               roller_offset
                                  ) // epitrochoid and epicycloid when roller_radius == roller_offset
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = fixed_radius / (double)roller_radius;
    LaserBoy_real_vertex  rv;
    LaserBoy_real_segment rs;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360.0;
    else
        arc_angle = roller_radius / greatest_common_devisor(fixed_radius, roller_radius) * 360.0;
    total_vertices = (u_int)ceil(arc_angle / p_space->rendered_arc_angle);
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    rv.blank();
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rv.x = roller_radius * (ratio + 1) - roller_offset + center.x;
    rv.y = center.y;
    rv.z = center.z;
    rs.push_back(rv);
    rv.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        rv.x = roller_radius * (ratio + 1) * cos(a * arc_step) - roller_offset * cos((ratio + 1) * a * arc_step) + center.x;
        rv.y = roller_radius * (ratio + 1) * sin(a * arc_step) - roller_offset * sin((ratio + 1) * a * arc_step) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(u_int             fixed_radius,
                                   u_int             roller_radius,
                                   int               roller_offset,
                                   LaserBoy_3D_short center
                                  ) // hypotrochoid and hypocycloid when roller_radius == roller_offset
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = fixed_radius / (double)roller_radius;
    LaserBoy_real_vertex  rv;
    LaserBoy_real_segment rs;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360.0;
    else
        arc_angle = roller_radius / greatest_common_devisor(fixed_radius, roller_radius) * 360.0;
    total_vertices = (u_int)ceil(arc_angle / p_space->rendered_arc_angle);
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    rv.blank();
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rv.x = (fixed_radius - roller_radius) + roller_offset + center.x;
    rv.y = center.y;
    rv.z = center.z;
    rs.push_back(rv);
    rv.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        rv.x = (fixed_radius - roller_radius) * cos(a * arc_step) + roller_offset * cos((ratio - 1) * a * arc_step) + center.x;
        rv.y = (fixed_radius - roller_radius) * sin(a * arc_step) - roller_offset * sin((ratio - 1) * a * arc_step) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_segment::LaserBoy_segment(LaserBoy_3D_short center,
                                   u_int             amplitude_x,
                                   u_int             amplitude_y,
                                   u_int             frequency_x,
                                   u_int             frequency_y,
                                   double            phase_x,
                                   double            phase_y
                                  ) // Lissajous curve
                : vector<LaserBoy_vertex>(),
                  palette_index          (LASERBOY_ILDA_DEFAULT),
                  segment_error          (LASERBOY_OK)
{
    u_int  total_vertices;
    double arc_angle,
           arc_step,
           ratio = frequency_x / frequency_x;
    LaserBoy_real_vertex  rv;
    LaserBoy_real_segment rs;
    if(    ratio > 0
        && (ratio - (int)ratio) == 0
      ) // it's a positive int!
        arc_angle = 360.0;
    else
        arc_angle = frequency_x / greatest_common_devisor(frequency_y, frequency_x) * 360.0;
    total_vertices = (u_int)ceil(arc_angle / p_space->rendered_arc_angle);
    arc_step = arc_angle / total_vertices;
    arc_step *= one_degree; // convert to radians!
    phase_x  *= one_degree;
    phase_y  *= one_degree;
    rv.blank();
    rv.c = p_space->selected_color_index;
    rv.r = p_space->palette_picker(p_space->palette_index)[rv.c].r;
    rv.g = p_space->palette_picker(p_space->palette_index)[rv.c].g;
    rv.b = p_space->palette_picker(p_space->palette_index)[rv.c].b;
    rv.x = amplitude_x * sin(phase_x) + center.x;
    rv.y = amplitude_y * sin(phase_y) + center.y;
    rv.z = center.z;
    rs.push_back(rv);
    rv.unblank();
    for(u_int a = 0; a <= total_vertices; a++)
    {
        rv.x = amplitude_x * sin(frequency_x * a * arc_step + phase_x) + center.x;
        rv.y = amplitude_y * sin(frequency_y * a * arc_step + phase_y) + center.y;
        rv.z = center.z;
        rs += rv;
    }
    rs.clip();
    *this = rs;
    sync_rgb_and_palette();
}

//############################################################################
size_t LaserBoy_segment::number_of_color_vectors() const
{
    size_t count = 0;
    if(size() > 1)
      for(size_t i = 1; i < size(); i++)
          if(at(i).is_color(p_space->black_level))
              count++;
    return count;
}

//############################################################################
size_t LaserBoy_segment::number_of_dark_vectors() const
{
    size_t count = 0;
    if(size() > 1)
      for(size_t i = 1; i < size(); i++)
          if(at(i).is_dark(p_space->black_level))
              count++;
    return count;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_front() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short front_;
        front_.z = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z >= front_.z)
                front_ = at(i);
        return front_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_back() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short back_;
        back_.z = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z <= back_.z)
                back_ = at(i);
        return back_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_top() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short top_;
        top_.y = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y >= top_.y)
                top_ = at(i);
        return top_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_bottom() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short bottom_;
        bottom_.y = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y <= bottom_.y)
                bottom_ = at(i);
        return bottom_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_right() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short right_;
        right_.x = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x >= right_.x)
                right_ = at(i);
        return right_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::segment_left() const
{
    if(size() > 1)
    {
        LaserBoy_3D_short left_;
        left_.x = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x <= left_.x)
                left_ = at(i);
        return left_;
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
short LaserBoy_segment::max_x() const
{
    if(size() > 1)
    {
        short max;
        max = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x >= max)
                max = at(i).x;
        return max;
    }
    else
        return 0;
}

//############################################################################
short LaserBoy_segment::max_y() const
{
    if(size() > 1)
    {
        short max;
        max = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y >= max)
                max = at(i).y;
        return max;
    }
    else
        return 0;
}

//############################################################################
short LaserBoy_segment::max_z() const
{
    if(size() > 1)
    {
        short max;
        max = LASERBOY_MIN_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z >= max)
                max = at(i).z;
        return max;
    }
    else
        return 0;
}

//############################################################################
short LaserBoy_segment::min_x() const
{
    if(size() > 1)
    {
        short min;
        min = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x <= min)
                min = at(i).x;
        return min;
    }
    else
        return 0;
}

//############################################################################
short LaserBoy_segment::min_y() const
{
    if(size() > 1)
    {
        short min;
        min = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y <= min)
                min = at(i).y;
        return min;
    }
    else
        return 0;
}

//############################################################################
short LaserBoy_segment::min_z() const
{
    if(size() > 1)
    {
        short min;
        min = LASERBOY_MAX_SHORT;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z <= min)
                min = at(i).z;
        return min;
    }
    else
        return 0;
}

//############################################################################
LaserBoy_3D_short LaserBoy_segment::max_off_axis_xyz() const
{
    if(size() > 1)
    {
        short _x = (max_x() >= abs(min_x())) ? (max_x()) : (abs(min_x())),
              _y = (max_y() >= abs(min_y())) ? (max_y()) : (abs(min_y())),
              _z = (max_z() >= abs(min_z())) ? (max_z()) : (abs(min_z()));
        return LaserBoy_3D_short(_x, _y, _z);
    }
    else
        return LaserBoy_3D_short(0, 0, 0);
}

//############################################################################
double LaserBoy_segment::max_distance_from_origin() const
{
    if(size() > 1)
    {
        double            max = 0,
                          d;
        LaserBoy_3D_short origin(0, 0, 0);
        for(size_t i = 0; i < size(); i++)
        {
            d = origin | at(i);
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_segment::max_distance_from_origin_xy() const
{
    if(size() > 1)
    {
        double            max = 0,
                          d;
        LaserBoy_3D_short origin(0, 0, 0),
                          vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_segment::max_distance_from_origin_zy() const
{
    if(size() > 1)
    {
        double            max = 0,
                          d;
        LaserBoy_3D_short origin(0, 0, 0),
                          vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.x = 0;
            d = origin | vertex;
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_segment::min_distance_from_origin() const
{
    if(size() > 1)
    {
        double            min = 80000,
                          d;
        LaserBoy_3D_short origin(0, 0, 0);
        for(size_t i = 0; i < size(); i++)
        {
            d = origin | at(i);
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_segment::min_distance_from_origin_xy() const
{
    if(size() > 1)
    {
        double            min = 80000,
                          d;
        LaserBoy_3D_short origin(0, 0, 0),
                          vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
u_short LaserBoy_segment::height() const
{
    return abs(max_y() - min_y());
}
//############################################################################
u_short LaserBoy_segment::width() const
{
    return abs(max_x() - min_x());
}

//############################################################################
u_short LaserBoy_segment::depth() const
{
    return abs(max_z() - min_z());
}

//############################################################################
u_short LaserBoy_segment::max_dimension() const
{
    u_short max = 0;
    if(height() >= max)    max = height();
    if(width () >= max)    max = width ();
    if(depth () >= max)    max = depth ();
    return max;
}

//############################################################################
double LaserBoy_segment::relative_volume() const
{
    return (max_dimension() / 65535.0);
}

//############################################################################
double LaserBoy_segment::relative_volume_to_origin() const
{
    short max = LASERBOY_MIN_SHORT;
    LaserBoy_3D_short vertex = max_off_axis_xyz();
    if(abs(vertex.x) >= max)    max = abs(vertex.x);
    if(abs(vertex.y) >= max)    max = abs(vertex.y);
    if(abs(vertex.z) >= max)    max = abs(vertex.z);
    return ((2.0 * max) / 65535.0);
}

//############################################################################
bool LaserBoy_segment::is_closed_polygon() const
{
    if(    (first_lit_anchor() | last_lit_vector())
        <= p_space->insignificant_distance
      )
        return true;
    return false;
}

//############################################################################
bool LaserBoy_segment::segment_passes_through_origin() const
{
  if(size() > 1)
      for(size_t i = 1; i < size(); i++)
          if(vector_passes_through_origin(at(i - 1).as_3D_double(),
                                          at(i).as_3D_double(),
                                          65536.0
                                         )
            )
              return true;
    return false;
}

//############################################################################
void LaserBoy_segment::blank_all_vertices()
{
  if(size() > 1)
      for(size_t i = 0; i < size(); i++)
          at(i).blank();
    return;
}

//############################################################################
void LaserBoy_segment::unblank_all_vertices()
{
  if(size() > 1)
      for(size_t i = 1; i < size(); i++)
          at(i).unblank();
    return;
}

//############################################################################
void LaserBoy_segment::blacken_vertices()
{
  if(size() > 1)
      for(size_t i = 0; i < size(); i++)
      {
          at(i).r = 0;
          at(i).g = 0;
          at(i).b = 0;
          at(i).c = p_space->palette_picker(palette_index).black;
      }
  return;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::reverse()
{
    int               i;
    LaserBoy_segment  reversed(palette_index, false);
    LaserBoy_vertex   vertex;
    if(size() > 2)
    {
        reversed.reserve(size());
        i = size() - 1;
        //----------------------------------------------------------------
        vertex = at(i--);
        vertex.blank();
        reversed.push_back(vertex);
        //----------------------------------------------------------------
        while(i >= 0)
        {
            vertex   = at(i    );
            vertex.r = at(i + 1).r;
            vertex.g = at(i + 1).g;
            vertex.b = at(i + 1).b;
            vertex.c = at(i + 1).c;
            vertex.k = at(i + 1).k;
            reversed.push_back(vertex);
            i--; // i goes to -1 !
        }
        //----------------------------------------------------------------
        *this = reversed;
    }
    else if(size() == 2)
    {
        //----------------------------------------------------------------
        reversed.reserve(2);
        vertex   = at(1);
        vertex.blank();
        reversed.push_back(vertex);
        vertex   = at(0);
        vertex.r = at(1).r;
        vertex.g = at(1).g;
        vertex.b = at(1).b;
        vertex.c = at(1).c;
        vertex.k = at(1).k;
        reversed.push_back(vertex);
        *this = reversed;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::reverse_sub_segment(size_t p1, size_t p2) // vertex indices
{
    //------------------------------------------------------------------------
  if(size() > 1)
      if(p1 != p2)
      {
          if((int)size() == 2)
              reverse();
          else
          {
              size_t            i;
              LaserBoy_segment  segment (palette_index, false),
                                reversed(palette_index, false);
              //----------------------------------------------------------------
              if(p2 < p1)
              {
                  i  = p1;
                  p1 = p2 - 1;
                  p2 = i;
              }
              //----------------------------------------------------------------
              for(i = 0; i < p1; i++)
                  segment += at(i);
              //----------------------------------------------------------------
              for(i = p1; i <= p2; i++)
                  reversed += at(i);
              //----------------------------------------------------------------
              reversed.reverse();
              segment += reversed;
              //----------------------------------------------------------------
              for(i = p2 + 1; i < size(); i++)
                  segment += at(i);
              //----------------------------------------------------------------
              *this = segment;
          }
      }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::reorder_from(size_t vertex_index)
{
    if(    vertex_index
        && vertex_index < size()
        && size() > 1
      )
    {
        size_t            i;
        LaserBoy_segment  reordered(palette_index, false);
        //----------------------------------------------------------------
        for(i = vertex_index; i < size(); i++)
            reordered += at(i);
        reordered.front().blank();
        for(i = 0; i <= vertex_index; i++)
            reordered += at(i);
        *this = reordered;
        //----------------------------------------------------------------
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::randomize_segments()
{
    if(number_of_segments() > 1)
    {
        vector<bool>     been_here(number_of_segments());
        size_t           i,
                         total_segments = number_of_segments(),
                         random_index   = rand() % total_segments;
        LaserBoy_segment segment   (palette_index, false),
                         randomized(palette_index, false);
        //----------------------------------------------------------------
        for(i = 0; i < total_segments; i++)
            been_here[i] = false;
        //----------------------------------------------------------------
        for(i = 0; i < total_segments; i++)
        {
            while(been_here[random_index])
            {
                random_index = rand() % total_segments;
            }
            segment = copy_segment(random_index);
            if(rand() % 2)
                segment.reverse();
            randomized += segment;
            been_here[random_index] = true;
        }
        //----------------------------------------------------------------
        *this = randomized;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::conglomerate_lit_segments()
{
    if(number_of_segments() > 1)
    {
        bool                match_found = false;
        int                 i,
                            j,
                            least_angle_index;
        double              angle,
                            least_angle;
        vector<int>         match_index;
        vector<char>        match_index_type;
        LaserBoy_segment    copy(*this);
        LaserBoy_frame      frame(palette_index, false);
        LaserBoy_frame_set  lit_vectors;
        //----------------------------------------------------------------
        if(p_space->fracture_b4_conglomerate)
            copy.fracture_segments();
        lit_vectors = copy.explode_segments();
        lit_vectors.minimize();
        //----------------------------------------------------------------
        p_space->p_GUI->display_state("conglomerating segments");
        do
        {
            p_space->p_GUI->display_progress(lit_vectors.number_of_frames());
            for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            {
                match_found = false;
                match_index.clear();
                match_index_type.clear();
                for(j = i + 1; j < (int)lit_vectors.number_of_frames(); j++)
                {
                    if(         lit_vectors[i].last_lit_vector().as_3D_short()
                             == lit_vectors[j].first_lit_anchor().as_3D_short()
                      )
                    {
                        match_found = true;
                        match_index_type.push_back('a');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].last_lit_vector().as_3D_short()
                             == lit_vectors[j].last_lit_vector().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('b');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_short()
                             == lit_vectors[j].last_lit_vector().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('c');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_short()
                             == lit_vectors[j].first_lit_anchor().as_3D_short()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('d');
                        match_index.push_back(j);
                    }
                } // end for(j = i; j < (int)lit_vectors.number_of_frames(); j++)
                //------------------------------------------------------------
                angle             = pi;
                least_angle       = pi;
                least_angle_index = 0;
                //------------------------------------------------------------
                if(match_index.size())
                {
                    for(j = 0; j < (int)match_index.size(); j++)
                    {
                        switch(match_index_type[j])
                        {
                            case 'a':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].first_lit_vertex()
                                                         );
                                break;
                            case 'b':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'c':
                                angle = three_point_angle(lit_vectors[i].first_lit_vertex(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'd':
                                angle = three_point_angle(lit_vectors[i].first_lit_vertex(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].first_lit_vertex()
                                                         );
                                break;
                        } // end switch(match_index_type[j])
                        if(angle <= least_angle)
                        {
                            least_angle = angle;
                            least_angle_index = j;
                        }
                    } // end for(j = 0; j < (int)match_index.size(); j++)
                    //--------------------------------------------------------
                    switch(match_index_type[least_angle_index])
                    {
                        case 'a':
                            break;
                        case 'b':
                            lit_vectors[match_index[least_angle_index]].reverse();
                            break;
                        case 'c':
                            lit_vectors.swap_frames(i, match_index[least_angle_index]);
                            break;
                        case 'd':
                            lit_vectors[i].reverse();
                            break;
                    } // end switch(match_index_type[least_angle_index])
                    //--------------------------------------------------------
                    lit_vectors[i].is_selected = true;
                    lit_vectors[match_index[least_angle_index]].is_selected = true;
                    lit_vectors.collapse_selected_frames();
                    lit_vectors[i].bond_segments();
                    //--------------------------------------------------------
                    break;
                } // end if(match_found)
                //------------------------------------------------------------
            } // end for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
        } while(match_found);
        //----------------------------------------------------------------
        lit_vectors.minimize();
        for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            frame += lit_vectors[i];
        *this = frame;
    }
    return *this;
}

//############################################################################
LaserBoy_segment LaserBoy_segment::shortest_path_of_segments(const LaserBoy_segment& previous_frame)
{
    LaserBoy_frame_set  segments = explode_segments();
    long int            j,
                        points_away,
                        points_away_temp;
    LaserBoy_3D_double  _0,
                        _1,
                        _2;
    //------------------------------------------------------------------------
    if(segments.number_of_frames() > 1)
    {
        vector<bool>        been_here;
        int                 i,
                            segments_index,
                            closest_segment_index = 0;
        double              temp_distance,
                            shortest_distance = 1000000.0; // more positive than possible
        LaserBoy_segment    reordered(palette_index, true);
        //----------------------------------------------------------------
        reordered.reserve(size());
        //----------------------------------------------------------------
        for(i = 0; i < (int)segments.number_of_frames(); i++)
        {
            been_here.push_back(false);
            // ---------------------------------------------------------------
            temp_distance =   segments[i].first_lit_anchor()
                            | previous_frame.last_lit_vector();
            if(temp_distance < shortest_distance)
            {
                shortest_distance = temp_distance;
                closest_segment_index = (i + 1); // can not be zero
            }
            else if(temp_distance == shortest_distance)
            {
                if(   segments[   (closest_segment_index > 0)
                                ? (closest_segment_index - 1)
                                : (-(closest_segment_index - 1))
                              ].front_to_back_magnitude()
                    > segments[i].front_to_back_magnitude()
                  )
                   closest_segment_index = (i + 1); // can not be zero
            }
            // ---------------------------------------------------------------
            temp_distance =   segments[i].last_lit_vector()
                            | previous_frame.last_lit_vector();
            if(temp_distance < shortest_distance)
            {
                shortest_distance = temp_distance;
                closest_segment_index = -(i + 1);
            }
            else if(temp_distance == shortest_distance)
            {
                if(   segments[   (closest_segment_index > 0)
                                ? (closest_segment_index - 1)
                                : (-(closest_segment_index - 1))
                              ].front_to_back_magnitude()
                    > segments[i].front_to_back_magnitude()
                  )
                    closest_segment_index = -(i + 1);
            }
        } // end for(i = 0; i < segments.number_of_frames(); i++)
        //----------------------------------------------------------------
        if(closest_segment_index > 0)
        {
            reordered = segments[closest_segment_index - 1];
            been_here[closest_segment_index - 1] = true;
        }
        else
        {
            reordered = (segments[(-closest_segment_index) - 1].reverse());
            been_here[(-closest_segment_index) - 1] = true;
        }
        //----------------------------------------------------------------
        for(segments_index = 1; segments_index < (int)segments.number_of_frames(); segments_index++)
        {
            points_away           = LONG_MAX;
            closest_segment_index = 0;
            for(i = 0; i < (int)segments.number_of_frames(); i++)
            {
                if(!been_here[i])
                {
                    //--------------------------------------------------------
                    // this end of the next segment
                    //--------------------------------------------------------
                    _0 = reordered.last_lit_anchor();
                    _1 = reordered.last_lit_vector();
                    _2 = segments[i].first_lit_anchor();
                    j  = reordered.last_lit_anchor_index();
                    while(    (_0 == _1)
                           && (--j >= 0)
                         )
                        _0 = reordered.at(j);
                    if(    (_0 == _1)
                        || (_1 == _2)
                      )
                        points_away_temp = 0;
                    else if(_0 == _2)
                        points_away_temp = end_dwell_vertices(pi,
                                                              p_space->sample_rate,
                                                              p_space->max_dwell_microsec
                                                             );
                    else
                        points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                          _0 | _1,
                                                                          _1 | _2
                                                                         ),
                                                              p_space->sample_rate,
                                                              p_space->max_dwell_microsec
                                                             );
                    //--------------------------------------------------------
                    if((reordered.last_lit_vector() | segments[i].first_lit_anchor()) >= p_space->insignificant_distance)
                    {
                        //----------------------------------------------------
                        points_away_temp += linear_steps(reordered.last_lit_vector(),
                                                         segments[i].first_lit_anchor(),
                                                         p_space->blank_delta_max
                                                        );
                        //----------------------------------------------------
                        _0 = reordered.last_lit_vector();
                        _1 = segments[i].first_lit_anchor();
                        _2 = segments[i].first_lit_vertex();
                        j  = segments[i].first_lit_vector_index();
                        while(    (_1 == _2)
                               && (++j < (int)segments[i].size())
                             )
                            _2 = segments[i].at(j);

                        j  = reordered.last_lit_vector_index();
                        while(    (_0 == _1)
                               && (--j >= 0)
                             )
                            _0 = reordered.at(j);


                        if(    (_0 == _1)
                            || (_1 == _2)
                          )
                            points_away_temp += 0;
                        else if(_0 == _2)
                            points_away_temp += start_dwell_vertices(pi,
                                                                     p_space->sample_rate,
                                                                     p_space->max_dwell_microsec
                                                                    );
                        else
                            points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                                 _0 | _1,
                                                                                 _1 | _2
                                                                                ),
                                                                     p_space->sample_rate,
                                                                     p_space->max_dwell_microsec
                                                                    );
                    }
                    //--------------------------------------------------------
                    if(points_away_temp < points_away)
                    {
                        points_away = points_away_temp;
                        closest_segment_index = (i + 1);
                    }
                    else if(points_away_temp == points_away) // it's a toss up between two positions
                    {
                        if(   segments[   (closest_segment_index > 0)
                                        ? (closest_segment_index - 1)
                                        : (-(closest_segment_index - 1))
                                      ].front_to_back_magnitude()
                            > segments[i].front_to_back_magnitude()
                          )
                        {
                            points_away = points_away_temp;
                            closest_segment_index = (i + 1);
                        }
                    }
                    //--------------------------------------------------------
                    // if the next segment ends are far enough apart
                    //--------------------------------------------------------
                    if(segments[i].front_to_back_magnitude() >= p_space->insignificant_distance)
                    {
                        //----------------------------------------------------
                        // look at the other end of the next segment
                        //----------------------------------------------------
                        _0 = reordered.last_lit_anchor();
                        _1 = reordered.last_lit_vector();
                        _2 = segments[i].last_lit_vector();
                        j  = segments[i].last_lit_vector_index();
                        while(    (_1 == _2)
                               && (--j >= 0)
                             )
                            _2 = segments[i].at(j);
                        j  = reordered.last_lit_anchor_index();
                        while(    (_0 == _1)
                               && (--j >= 0)
                             )
                            _0 = reordered.at(j);
                        if(    (_0 == _1)
                            || (_1 == _2)
                          )
                            points_away_temp = 0;
                        else if(_0 == _2)
                            points_away_temp = end_dwell_vertices(pi,
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                        else
                            points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                              _0 | _1,
                                                                              _1 | _2
                                                                             ),
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                        //----------------------------------------------------
                        if((reordered.last_lit_vector() | segments[i].last_lit_vector()) >= p_space->insignificant_distance)
                        {
                            //------------------------------------------------
                            points_away_temp += linear_steps(reordered.last_lit_vector(),
                                                             segments[i].last_lit_vector(),
                                                             p_space->blank_delta_max
                                                            );
                            //------------------------------------------------
                            _0 = reordered.last_lit_vector();
                            _1 = segments[i].last_lit_vector();
                            _2 = segments[i].last_lit_anchor();
                            j  = segments[i].last_lit_anchor_index();
                            while(    (_1 == _2)
                                   && (++j < (int)segments[i].size())
                                 )
                                _2 = segments[i].at(j);
                            j  = reordered.last_lit_vector_index();
                            while(    (_0 == _1)
                                   && (--j >= 0)
                                 )
                                _0 = reordered.at(j);
                            if(    (_0 == _1)
                                || (_1 == _2)
                              )
                                points_away_temp += 0;
                            else if(_0 == _2)
                                points_away_temp += start_dwell_vertices(pi,
                                                                         p_space->sample_rate,
                                                                         p_space->max_dwell_microsec
                                                                        );
                            else
                                points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                                     _0 | _1,
                                                                                     _1 | _2
                                                                                    ),
                                                                         p_space->sample_rate,
                                                                         p_space->max_dwell_microsec
                                                                        );
                        }
                        //----------------------------------------------------
                        if(points_away_temp < points_away)
                        {
                            points_away = points_away_temp;
                            closest_segment_index = -(i + 1);
                        }
                        else if(points_away_temp == points_away) // it's a toss up between two positions
                        {
                            if(   segments[   (closest_segment_index > 0)
                                            ? (closest_segment_index - 1)
                                            : (-(closest_segment_index - 1))
                                          ].front_to_back_magnitude()
                                > segments[i].front_to_back_magnitude()
                              )
                            {
                                points_away = points_away_temp;
                                closest_segment_index = -(i + 1);
                            }
                        }
                    } // end if the segment ends are far enough apart
                }
            }
            if(closest_segment_index > 0)
            {
                reordered += segments[closest_segment_index - 1];
                been_here[closest_segment_index - 1] = true;
            }
            else // (closest_segment_index < 0)
            {
                reordered += (segments[(-closest_segment_index) - 1].reverse());
                been_here[(-closest_segment_index) - 1] = true;
            }
        }
        //----------------------------------------------------------------
//        while(reordered.back().is_blank())
//            reordered.pop_back();
        return reordered;
        //----------------------------------------------------------------
    } // end if(segments.number_of_frames() > 1)
    else // there is only one lit segment
    {
        //--------------------------------------------------------
        // this end of the segment
        //--------------------------------------------------------
        _0 = previous_frame.last_lit_anchor();
        _1 = previous_frame.last_lit_vector();
        _2 = segments[0].first_lit_anchor();
        j  = previous_frame.last_lit_anchor_index();
        while(    (_0 == _1)
               && (--j >= 0)
             )
            _0 = previous_frame.at(j);
        if(    (_0 == _1)
            || (_1 == _2)
          )
            points_away = 0;
        else if(_0 == _2)
            points_away = end_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
        else
            points_away = end_dwell_vertices(delta_angle(_0 | _2,
                                                         _0 | _1,
                                                         _1 | _2
                                                        ),
                                             p_space->sample_rate,
                                             p_space->max_dwell_microsec
                                            );
        //--------------------------------------------------------
        if(    (   previous_frame.last_lit_vector()
                 | segments[0].first_lit_anchor()
               )
            >= p_space->insignificant_distance
          )
        {
            //----------------------------------------------------
            points_away += linear_steps(previous_frame.last_lit_vector(),
                                        segments[0].first_lit_anchor(),
                                        p_space->blank_delta_max
                                       );
            //----------------------------------------------------
            _0 = previous_frame.last_lit_vector();
            _1 = segments[0].first_lit_anchor();
            _2 = segments[0].first_lit_vertex();
            j  = segments[0].first_lit_vector_index();
            while(    (_1 == _2)
                   && (++j < (int)segments[0].size())
                 )
                _2 = segments[0].at(j);

            j  = previous_frame.last_lit_vector_index();
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = previous_frame.at(j);


            if(    (_0 == _1)
                || (_1 == _2)
              )
                points_away += 0;
            else if(_0 == _2)
                points_away += start_dwell_vertices(pi,
                                                    p_space->sample_rate,
                                                    p_space->max_dwell_microsec
                                                   );
            else
                points_away += start_dwell_vertices(delta_angle(_0 | _2,
                                                                _0 | _1,
                                                                _1 | _2
                                                               ),
                                                    p_space->sample_rate,
                                                    p_space->max_dwell_microsec
                                                   );
        }
        //--------------------------------------------------------
        // if the segment ends are far enough apart
        //--------------------------------------------------------
        if(segments[0].front_to_back_magnitude() >= p_space->insignificant_distance)
        {
            //----------------------------------------------------
            // look at the other end of the segment
            //----------------------------------------------------
            _0 = previous_frame.last_lit_anchor();
            _1 = previous_frame.last_lit_vector();
            _2 = segments[0].last_lit_vector();
            j  = segments[0].last_lit_vector_index();
            while(    (_1 == _2)
                   && (--j >= 0)
                 )
                _2 = segments[0].at(j);
            j  = previous_frame.last_lit_anchor_index();
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = previous_frame.at(j);
            if(    (_0 == _1)
                || (_1 == _2)
              )
                points_away_temp = 0;
            else if(_0 == _2)
                points_away_temp = end_dwell_vertices(pi,
                                                      p_space->sample_rate,
                                                      p_space->max_dwell_microsec
                                                     );
            else
                points_away_temp = end_dwell_vertices(delta_angle(_0 | _2,
                                                                  _0 | _1,
                                                                  _1 | _2
                                                                 ),
                                                      p_space->sample_rate,
                                                      p_space->max_dwell_microsec
                                                     );
            //----------------------------------------------------
            if((previous_frame.last_lit_vector() | segments[0].last_lit_vector()) >= p_space->insignificant_distance)
            {
                //------------------------------------------------
                points_away_temp += linear_steps(previous_frame.last_lit_vector(),
                                                 segments[0].last_lit_vector(),
                                                 p_space->blank_delta_max
                                                );
                //------------------------------------------------
                _0 = previous_frame.last_lit_vector();
                _1 = segments[0].last_lit_vector();
                _2 = segments[0].last_lit_anchor();
                j  = segments[0].last_lit_anchor_index();
                while(    (_1 == _2)
                       && (++j < (int)segments[0].size())
                     )
                    _2 = segments[0].at(j);
                j  = previous_frame.last_lit_vector_index();
                while(    (_0 == _1)
                       && (--j >= 0)
                     )
                    _0 = previous_frame.at(j);
                if(    (_0 == _1)
                    || (_1 == _2)
                  )
                    points_away_temp += 0;
                else if(_0 == _2)
                    points_away_temp += start_dwell_vertices(pi,
                                                             p_space->sample_rate,
                                                             p_space->max_dwell_microsec
                                                            );
                else
                    points_away_temp += start_dwell_vertices(delta_angle(_0 | _2,
                                                                         _0 | _1,
                                                                         _1 | _2
                                                                        ),
                                                             p_space->sample_rate,
                                                             p_space->max_dwell_microsec
                                                            );
            }
            //----------------------------------------------------
            if(points_away_temp < points_away)
                segments[0].reverse();
        } // end if the segment ends are far enough apart
        return segments[0];
    }
}

//############################################################################
void LaserBoy_segment::reduce_blank_vectors()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  minimum_blanking(palette_index, false);
        //----------------------------------------------------------------
        minimum_blanking.reserve(size());
        //----------------------------------------------------------------
        minimum_blanking.push_back(front());
        for(i = 1; i < size(); i++)
        {
            if(    at(i).is_blank()
                && minimum_blanking.back().is_blank()
              )
                minimum_blanking.back() = at(i); // the new, last blank
            else
                minimum_blanking.push_back(at(i));
        }
        //----------------------------------------------------------------
        while(minimum_blanking.size() && minimum_blanking.back().is_blank())
            minimum_blanking.pop_back();
        *this = minimum_blanking;
    }
    return;
}

//############################################################################
void LaserBoy_segment::omit_equivalent_vectors()
{
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set  segments;
        size_t              i,
                            j;
        //----------------------------------------------------------------
        fracture_segments();
        segments = explode_segments();
        segments.minimize();
        //----------------------------------------------------------------
        for(i = 0; i < segments.number_of_frames() - 1; i++)
        {
            for(j = i + 1; j < segments.number_of_frames(); j++)
                if(    segments[i] == segments[j]
                    || segments[i] == segments[j].reverse()
                  )
                    segments[j].clear();
        }
        //----------------------------------------------------------------
        *this = segments.sum_of_frames();
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_segment::remove_dots()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  no_dots(palette_index, false);
        //----------------------------------------------------------------
        no_dots.reserve(size());
        //----------------------------------------------------------------
        no_dots.push_back(front());
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at_index_has_magnitude(i))
                no_dots.push_back(at(i));
        }
        //----------------------------------------------------------------
        *this = no_dots;
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::enhance_dots()
{
    if(size() > 1)
    {
        LaserBoy_segment  more_dots(palette_index, false);
        //----------------------------------------------------------------
        more_dots.reserve(size());
        //----------------------------------------------------------------
        more_dots.push_back(front());
        //----------------------------------------------------------------
        for(size_t i = 1; i < size(); i++)
        {
            if(    at(i).is_color(p_space->black_level)
                && !at_index_has_magnitude(i)
              )
            {
                for(u_int j = 1; j < (u_int)round(p_space->dwell_on_dot); j++)
                    more_dots.push_back(at(i));
            }
            more_dots.push_back(at(i));
        }
        //----------------------------------------------------------------
        *this = more_dots;
    }
    return LASERBOY_OK;
}

//############################################################################
void LaserBoy_segment::remove_dwell_vertices()
{
    if(size() > 2)
    {
        size_t            i;
        LaserBoy_segment  no_dwell_vertices(palette_index, false);
        //----------------------------------------------------------------
        no_dwell_vertices.reserve(size());
        no_dwell_vertices.push_back(front());
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
            if(at(i) != at(i - 1)) // checks color and location
                no_dwell_vertices.push_back(at(i));
        //----------------------------------------------------------------
        *this = no_dwell_vertices;
    }
    return;
}

//############################################################################
void LaserBoy_segment::remove_short_vectors()
{
    if(size() > 2)
    {
        int               i,
                          index;
        LaserBoy_segment  no_short_vectors(*this);
        //----------------------------------------------------------------
        if(    p_space->insignificant_distance > 0.00
            && no_short_vectors.size() > 2
          )
        {
            do
            {
                index = -1;
                for(i = no_short_vectors.size() - 2; i > 1; i--)
                {
                    if(    no_short_vectors.at(i).is_color(p_space->black_level)
                        &&     no_short_vectors.at(i    ).color_of(p_space->black_level)
                            == no_short_vectors.at(i + 1).color_of(p_space->black_level)
                        && (no_short_vectors.at(i) | no_short_vectors.at(i + 1)) <= p_space->insignificant_distance
                      )
                    {
                        index = i;
                        break;
                    }
                }
                if(index != -1)
                {
                    if(index > 1)
                        no_short_vectors.at(index - 1) = (LaserBoy_3D_short) // just coordinates
                                                         no_short_vectors.at(index - 1).blend(no_short_vectors.at(index), 0.50);
                    no_short_vectors.remove_vertex(index);
                }
            } while(index != -1);
            //----------------------------------------------------------------
            *this = no_short_vectors;
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::reduce_lit_vectors()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  segment_1(palette_index, false),
                          segment_2(palette_index, false);
        //----------------------------------------------------------------
        segment_1.reserve(size());
        segment_2.reserve(size());
        //----------------------------------------------------------------
        segment_1.push_back(front());
        for(i = 1; i < size(); i++)
        {
            if(at(i) != segment_1.back())
                segment_1.push_back(at(i));
        }
        //----------------------------------------------------------------
        while(segment_1.size() && segment_1.back().is_blank())
            segment_1.pop_back();
        //----------------------------------------------------------------
        if(segment_1.size())
        {
            segment_2.push_back(segment_1.front());
            for(i = 1; i < segment_1.size() - 1; i++)
            {
                if(    (    segment_1.at(i    ).color_of(p_space->black_level)
                         != segment_1.at(i + 1).color_of(p_space->black_level)
                       )
                    || (    segment_1.vector_angle(i)
                          > p_space->insignificant_angle
                       )
                  )
                    segment_2.push_back(segment_1.at(i));
            }
            //----------------------------------------------------------------
            segment_2.push_back(segment_1.at(i));
            while(segment_2.size() && segment_2.back().is_blank())
                segment_2.pop_back();
        }
        //----------------------------------------------------------------
        *this = segment_2;
    }
    return;
}

//############################################################################
void LaserBoy_segment::impose_bit_resolution()
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
        {
            at(i).x &=  short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_X]];
            at(i).y &=  short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_Y]];
            at(i).z &=  short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_Z]];
            at(i).r &= (short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_R]] >> 7);
            at(i).g &= (short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_G]] >> 7);
            at(i).b &= (short_bit_mask[p_space->signal_bit_mask[LASERBOY_CHANNEL_B]] >> 7);
        }
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::reorder_segments(const LaserBoy_segment& previous_frame)
{
    LaserBoy_segment reordered = shortest_path_of_segments(previous_frame);
    *this = reordered;
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::fracture_segments()
{
    if(number_of_lit_vectors() > 1)
    {
        size_t            i;
        LaserBoy_segment  fractured(palette_index, false);
        fractured.reserve(3 * size());
        reduce_blank_vectors();
        //----------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            fractured.push_back(at(i));
            fractured.push_back(at(i));
            fractured.back().blank();
        }
        //----------------------------------------------------------------
        fractured.pop_back();
        fractured.reduce_blank_vectors();
        *this = fractured;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::bond_segments()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  bonded(palette_index, false);
        //----------------------------------------------------------------
        bonded.reserve(size());
        bonded.push_back(front());
        for(i = 1; i < size() - 1; i++)
        {
            if( !(     at(i).is_blank()
                   &&     (   at(i)
                            | at(i - 1)
                          )
                       <= p_space->insignificant_distance
              )  )
                bonded += at(i);
        }
        bonded.push_back(back());
        //----------------------------------------------------------------
        *this = bonded;
    }
    return LASERBOY_OK;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_vertex(size_t vertex_index)
{
    if(size() < 2) // there is nothing
    {
        clear();
        reserve(2);
        push_back(LaserBoy_vertex());
        push_back(LaserBoy_vertex()); // add blank, zero vector
    }
    else if(vertex_index < size())
    {
        size_t            i;
        LaserBoy_segment  segment(palette_index, false);
        segment.reserve(size() + 1);
        if(vertex_index < size() - 1) // not the last vertex
        {
            for(i = 0; i <= vertex_index; i++)
                segment += at(i);
            if((at(vertex_index) | at(vertex_index + 1)) > p_space->insignificant_distance)
                segment += LaserBoy_vertex(scale_vertex_on_coordinates(at(vertex_index + 1),
                                                                       at(vertex_index),
                                                                       LaserBoy_3D_double(0.5, 0.5, 0.5)
                                                                      ),
                                           at(vertex_index + 1), // is an rgb
                                           at(vertex_index + 1).k,
                                           at(vertex_index + 1).c
                                          );
            for(i = vertex_index + 1; i < size(); i++)
                segment += at(i);
        }
        else // it is the last vertex
        {
            segment = *this;
            if(p_space->end_vect_to_delta_max)
            {
                LaserBoy_3D_double  vector_scale = (p_space->lit_delta_max / (at(size() - 2) | back()));
                LaserBoy_vertex     new_vertex   = LaserBoy_vertex(scale_vertex_on_coordinates(back(),
                                                                                               at(size() - 2),
                                                                                               vector_scale
                                                                                              ),
                                                                   back(), // is a LaserBoy_color
                                                                   back().k,
                                                                   back().c
                                                                  );
                new_vertex += (back() - at(size() - 2));
                segment += new_vertex;
            }
            else
                segment += LaserBoy_vertex(scale_vertex_on_coordinates(back(),
                                                                       at(size() - 2),
                                                                       LaserBoy_3D_double(2.0, 2.0, 2.0)
                                                                      ),
                                           back(), // is a LaserBoy_color
                                           back().k,
                                           back().c
                                          );
        }
        *this = segment;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::break_segment(size_t& vertex_index)
{
    if(    vertex_index > 0
        && vertex_index < size() - 1
        && size() > 1
      )
    {
        size_t            i;
        LaserBoy_segment  segment(palette_index, false);
        segment.reserve(size() + 1);
        for(i = 0; i <= vertex_index; i++)
            segment += at(i);

        segment += at(vertex_index);
        segment.back().blank();
        for(i = vertex_index + 1; i < size(); i++)
            segment += at(i);
        vertex_index += 2; // changes the value of spider in frame class
        *this = segment;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::connect_the_dots(size_t p0, size_t p1) // vertex indices
{
    if(    p0 != p1
        && p0 < size()
        && p1 < size()
        && (   (   at(p0)
                 | at(p1)
               )
            >= p_space->insignificant_distance
           )
      )
    {
        push_back(at(p0));
        back().blank();
        push_back(at(p1));
        back().unblank();
        back().c = p_space->selected_color_index;
        back().r = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].r;
        back().g = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].g;
        back().b = p_space->palette_picker(p_space->palette_index)[p_space->selected_color_index].b;
    }
    return segment_error;
}

//############################################################################
double LaserBoy_segment::vector_angle(size_t vertex_index) const
{
    double angle = 0.0;
    if(vertex_index != 0)
    {
        if(size() > 1)
        {
            int _0  = 0,
                _1  = 0,
                _2  = 0;
           //-----------------------------------------------------------------
            if(vertex_index > 0 && vertex_index < size() - 1)
            {
                _0 = vertex_index - 1;
                _1 = vertex_index    ;
                _2 = vertex_index + 1;
            }
           //-----------------------------------------------------------------
            else if(vertex_index == size() - 1)
            {
                _0 = size() - 2;
                _1 = 0                     ;
                _2 = 1                     ;
            }
            //----------------------------------------------------------------
            angle = delta_angle( at(_0) | at(_2), // distance between vertices 0,2
                                 at(_0) | at(_1),
                                 at(_1) | at(_2)
                               );
        }
    }
    return angle;
}

//############################################################################
double LaserBoy_segment::total_angle() const
{
    double total = 0.0;
    size_t i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            total += vector_angle(i);
    return total;
}

//############################################################################
double LaserBoy_segment::max_angle() const
{
    double max = 0.0;
    size_t i;
    if(size() > 1)
      for(i = 0; i < size() - 1; i++)
          if(max < vector_angle(i))
              max = vector_angle(i);
    return max;
}

//############################################################################
double LaserBoy_segment::total_magnitude() const
{
    double total = 0.0;
    size_t i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            total += (at(i) | at(i + 1));
    return total;
}

//############################################################################
double LaserBoy_segment::max_magnitude() const
{
    double max = 0.0,
           d;
    size_t i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
        {
            d = (at(i) | at(i + 1));
            if(max < d)
                max = d;
        }
    return max;
}

//############################################################################
double LaserBoy_segment::max_color_magnitude() const
{
    double max = 0.0,
           d;
    size_t i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            if(at(i + 1).is_color(p_space->black_level))
            {
                d = (at(i) | at(i + 1));
                if(max < d)
                    max = d;
            }
    return max;
}

//############################################################################
double LaserBoy_segment::max_dark_magnitude() const
{
    double max = 0.0,
           d;
    size_t i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            if(at(i + 1).is_dark(p_space->black_level))
            {
                d = (at(i) | at(i + 1));
                if(max < d)
                    max = d;
            }
    return max;
}

//############################################################################
double LaserBoy_segment::length_in_seconds() const
{
    return double(size()) / p_space->sample_rate;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_dwell()
{
    if(size() > 1)
    {
        int                 i,
                            j,
                            dwell_vertex_count,
                            dwell_vertex_total;
        LaserBoy_3D_double  _0,
                            _1,
                            _2;
        LaserBoy_vertex     vertex,
                            black_vertex;
        LaserBoy_segment    angle_optimized(palette_index, false);
        //----------------------------------------------------------------
        remove_dwell_vertices();
        //----------------------------------------------------------------
        angle_optimized.push_back(front());
        for(i = 1; i < (int)size() - 1; i++)
        {
            angle_optimized.push_back(at(i));
            _0 = at(i - 1);
            _1 = at(i);
            _2 = at(i + 1);
            j  = i - 1;
            while(    (_0 == _1)
                   && (--j >= 0)
                 )
                _0 = at(j);
            if(_0 == _1)
                _0 = 0.0;
            if(    (_0 == _1)
                || (_1 == _2)
              )
                dwell_vertex_total = 0;
            else
            {
                if(    at(i    ).is_lit()
                    && at(i + 1).is_blank()
                  )
                {
                    if(_0 == _2)
                        dwell_vertex_total = end_dwell_vertices(pi,
                                                                p_space->sample_rate,
                                                                p_space->max_dwell_microsec
                                                               );
                    else
                        dwell_vertex_total = end_dwell_vertices(delta_angle(_0 | _2,
                                                                            _0 | _1,
                                                                            _1 | _2
                                                                           ),
                                                                p_space->sample_rate,
                                                                p_space->max_dwell_microsec
                                                               );
                    if(dwell_vertex_total)
                    {
                        vertex = black_vertex = at(i);
                        if(    p_space->black_dwell_vertices
                            && black_vertex.color_of(p_space->black_level) // not blank or black
                          )
                        {
                            black_vertex.c = p_space->palette_picker(palette_index).black;
                            black_vertex.r =
                            black_vertex.g =
                            black_vertex.b = 0x00;
                        }
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            if(dwell_vertex_count < (int)p_space->lit_dwell_overhang)
                                angle_optimized.push_back(vertex);
                            else
                                angle_optimized.push_back(black_vertex);
                    }
                }
                else if(    at(i    ).is_blank()
                         && at(i + 1).is_lit()
                       )
                {
                    if(_0 == _2)
                        dwell_vertex_total = start_dwell_vertices(pi,
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                    else
                        dwell_vertex_total = start_dwell_vertices(delta_angle(_0 | _2,
                                                                              _0 | _1,
                                                                              _1 | _2
                                                                             ),
                                                                  p_space->sample_rate,
                                                                  p_space->max_dwell_microsec
                                                                 );
                    if(dwell_vertex_total)
                    {
                        vertex = at(i);
                        vertex.blank();
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            angle_optimized.push_back(vertex);
                    }
                }
                else if(    at(i    ).is_lit()
                         && at(i + 1).is_lit()
                       )
                {
                    if(_0 == _2)
                        dwell_vertex_total = inline_dwell_vertices(pi,
                                                                   p_space->sample_rate,
                                                                   p_space->max_dwell_microsec
                                                                  );
                    else
                        dwell_vertex_total = inline_dwell_vertices(delta_angle(_0 | _2,
                                                                               _0 | _1,
                                                                               _1 | _2
                                                                              ),
                                                                   p_space->sample_rate,
                                                                   p_space->max_dwell_microsec
                                                                  );
                    if(dwell_vertex_total)
                    {
                        vertex = black_vertex = at(i);
                        if(    p_space->black_dwell_vertices
                            && black_vertex.color_of(p_space->black_level) > 0x00 // not blank or black
                          )
                        {
                            black_vertex.c = p_space->palette_picker(palette_index).black;
                            black_vertex.r =
                            black_vertex.g =
                            black_vertex.b = 0x00;
                        }
                        for(dwell_vertex_count = 0; dwell_vertex_count < dwell_vertex_total; dwell_vertex_count++)
                            if(dwell_vertex_count < (int)p_space->lit_dwell_overhang)
                                angle_optimized.push_back(vertex);
                            else
                                angle_optimized.push_back(black_vertex);
                    }
                }
            }
        }
        angle_optimized.push_back(back());
        //----------------------------------------------------------------
        *this = angle_optimized;
    }
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_lit_span_vertices()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  distance_optimized,
                          line;
        //----------------------------------------------------------------
        for(i = 0; i < size() - 1; i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_lit()
                && (at(i) | at(i + 1)) > p_space->lit_delta_max
              )
            {
                line = LaserBoy_segment(at(i), at(i + 1));
                line.pop_back();
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(at(size() - 1));
        //----------------------------------------------------------------
        distance_optimized.palette_index = palette_index;
        *this = distance_optimized;

    }
    return segment_error;
}

//############################################################################
void LaserBoy_segment::add_lit_span_vertices(const u_int& max_d)
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  distance_optimized,
                          line;
        //----------------------------------------------------------------
        for(i = 0; i < size() - 1; i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_lit()
                && (at(i) | at(i + 1)) > max_d
              )
            {
                line = LaserBoy_segment(at(i), at(i + 1), max_d);
                line.pop_back();
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(at(size() - 1));
        //----------------------------------------------------------------
        distance_optimized.palette_index = palette_index;
        *this = distance_optimized;
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::add_blank_span_vertices()
{
    if(size() > 1)
    {
        size_t            i;
        LaserBoy_segment  distance_optimized,
                          line;
        //----------------------------------------------------------------
        for(i = 0; i < size() - 1; i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_blank()
                && (at(i) | at(i + 1)) > p_space->blank_delta_max
              )
            {
                line = LaserBoy_segment(at(i), at(i + 1));
                line.remove_vertex(line.size() - 1);
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(at(size() - 1));
        //----------------------------------------------------------------
        distance_optimized.palette_index = palette_index;
        *this = distance_optimized;
    }
    return segment_error;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::flip(u_int plane)
{
    if(size() > 1)
    {
        size_t i;
        switch(plane)
        {
           case 0: // X
                   for(i = 0; i < size(); i++)
                       at(i).x = -at(i).x;
                   break;
           //-----------------------------------------------------------------
           case 1: // Y
                   for(i = 0; i < size(); i++)
                       at(i).y = -at(i).y;
                   break;
           //-----------------------------------------------------------------
           case 2: // Z
                   for(i = 0; i < size(); i++)
                       at(i).z = -at(i).z;
                   break;
           //-----------------------------------------------------------------
           case 3: // X, Y
                   for(i = 0; i < size(); i++)
                   {
                       at(i).x = -at(i).x;
                       at(i).y = -at(i).y;
                   }
                   break;
           //-----------------------------------------------------------------
           case 4: // X, Y, Z
                   for(i = 0; i < size(); i++)
                       at(i) = -at(i);
                   break;
           //-----------------------------------------------------------------
        }
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::normalize(double s)
{
    if(size() > 1)
    {
        if(s > 0.0)
        {
            LaserBoy_real_segment rs(palette_index, false);
            rs = to_real_segment(true);
            rs.normalize(false);
            rs.remove_vertex(0);
            rs.remove_vertex(0); // delete the origin vector
            *this = rs;
            scale_around_origin(s);
        }
        else
        {
            LaserBoy_real_segment rs(palette_index, false);
            rs = to_real_segment(true);
            rs.center();
            *this = rs;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::normalize_vectors(double s)
{
    if(size() > 1)
    {
        if(s > 0.0)
        {
            LaserBoy_real_segment rs(palette_index, false);
            rs = to_real_segment(true);
            rs.normalize_vectors();
            rs.remove_vertex(0);
            rs.remove_vertex(0); // delete the origin vector
            *this = rs;
            scale_around_origin(s);
        }
        else
        {
            LaserBoy_real_segment rs(palette_index, false);
            rs = to_real_segment(false);
            rs.center();
            *this = rs;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::normalize_vectors_with_origin(double s)
{
    if(size() > 1 && s > 0.0)
    {
        LaserBoy_real_segment rs(palette_index, false);
        rs = to_real_segment(true);
        rs.normalize_vectors_with_origin();
        rs.remove_vertex(0);
        rs.remove_vertex(0); // delete the origin vector
        *this = rs;
        scale_around_origin(s);
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::quarter_turn(u_int plane, u_int turns)
{
    if(size() > 1)
    {
        size_t  i;
        u_int   j;
        short   temp;
        switch(plane)
        {
           case 0: // X Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 1: // Z Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).z;
                           at(i).z = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 2: // X Z
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).z;
                           at(i).z = temp;
                       }
                   break;
           //-----------------------------------------------------------------
        }
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::z_order_vertices(unsigned short span)
{
    if(size() > 1)
    {
        size_t i,
              step = span / size();
        for(i = 0; i < size(); i++)
            at(i).z = (short)((i * step) - (span / 2));
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::flatten_z()
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
            at(i).z = 0;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::flat_to_2D()
{
    if(size() > 1 && is_flat_in_z())
    {
        for(size_t i = 0; i < size(); i++)
            at(i).z = 0;
    }
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::center_x()
{
    LaserBoy_real_segment rs = to_real_segment(false);
    rs.center_x();
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::center_y()
{
    LaserBoy_real_segment rs = to_real_segment(false);
    rs.center_y();
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::center_z()
{
    LaserBoy_real_segment rs = to_real_segment(false);
    rs.center_z();
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::center()
{
    LaserBoy_real_segment rs = to_real_segment(false);
    rs.center();
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  center = centroid_of_coordinates();
        if(!p_space->destructive_clipping)
        {
          for(i = 0; i < size(); i++)
          {
              out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i), center, a), LASERBOY_CUBE);
              if(out_of_bounds)
                return out_of_bounds;
          }
          for(i = 0; i < size(); i++)
              at(i) = rotate_vertex_on_coordinates(at(i), center, a);
          return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate(a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_around_origin(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
          for(i = 0; i < size(); i++)
          {
            out_of_bounds |= LaserBoy_bounds_check(rotate_vertex(at(i), a), LASERBOY_CUBE);
            if(out_of_bounds)
                return out_of_bounds;
          }
          for(i = 0; i < size(); i++)
              at(i) = rotate_vertex(at(i), a);
          return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate_around_origin(a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates(LaserBoy_3D_double p, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
          for(i = 0; i < size(); i++)
          {
              out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i), p, a), LASERBOY_CUBE);
              if(out_of_bounds)
                  return out_of_bounds;
          }
          for(i = 0; i < size(); i++)
              at(i) = rotate_vertex_on_coordinates(at(i), p, a);
          return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate_on_coordinates(p, p, 0, a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_x(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
            {
                out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_x(at(i), p, a), LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates_x(at(i), p, a);
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate_on_coordinates_x(p, a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_y(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
            {
                out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_y(at(i), p, a), LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates_y(at(i), p, a);
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate_on_coordinates_y(p, a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_on_coordinates_z(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        size_t           i;
        LaserBoy_Bounds  out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
            {
                out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates_z(at(i), p, a), LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates_z(at(i), p, a);
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.rotate_on_coordinates_z(p, a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_around_origin()
{
    return rotate_around_origin(p_space->view_angle);
}

//############################################################################
void LaserBoy_segment::ripple(int direction, double amplitude, double freq, double phase)
{
    if(size() > 1)
    {
        size_t i;
        switch(direction)
        {
            case 0: for(i = 0; i < size(); i++) // parellel through x
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).x / 32768.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 1: for(i = 0; i < size(); i++) // parellel through y
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).y / 32768.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 2: for(i = 0; i < size(); i++) // x * y, the egg carton effect
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(at(i).x * at(i).y / 1073741824.0 * two_pi * freq + phase));
                    break;
            //----------------------------------------------------------------
            case 3: for(i = 0; i < size(); i++) // circular, concentric to the origin
                        at(i).z = short(amplitude * (16384.0 / pi) * sin(sqrt(double(at(i).x * at(i).x + at(i).y * at(i).y)) / 32768.0 * two_pi * freq + phase));
            //----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
LaserBoy_palette LaserBoy_segment::as_color_table() const
{
    LaserBoy_palette color_table;
    color_table.reserve(size());
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_lit())
                color_table.push_back(at(i));
            else
                color_table.push_back(LaserBoy_color(0,0,0));
        }
    return color_table;
}

//############################################################################
void LaserBoy_segment::strip_color()
{
    palette_index = LASERBOY_ILDA_DEFAULT;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).k & LASERBOY_BLANKING_BIT)
            {
                at(i).c = 0;
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
            else if(at(i).is_black(p_space->black_level))
            {
                at(i).c = 64;
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
            else
            {
                at(i).c =  55; // LASERBOY_ILDA_DEFAULT white
                at(i).r = 255;
                at(i).g = 255;
                at(i).b = 255;
            }
        }
    p_space->palette_index = LASERBOY_ILDA_DEFAULT;
    return;
}

//############################################################################
void LaserBoy_segment::strip_color_rgb(const LaserBoy_color& c)
{
    palette_index = LASERBOY_TRUE_COLOR;
    for(size_t i = 1; i < size(); i++)
    {
        if(at(i).is_color(p_space->black_level))
        {
            at(i).r = c.r;
            at(i).g = c.g;
            at(i).b = c.b;
        }
    }
    sync_rgb_and_palette();
}

//############################################################################
void LaserBoy_segment::strip_color_or()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR) // if it is alread palette
            set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
        for(size_t i = 1; i < size(); i++)
        {
            at(i).r =
            at(i).g =
            at(i).b = (at(i).r | at(i).g | at(i).b);
        }
        best_match_palette(LASERBOY_GRAYS);
    }
    return;
}

//############################################################################
void LaserBoy_segment::strip_color_avg()
{
    if(size() > 1)
    {
        if(palette_index != LASERBOY_TRUE_COLOR) // if it is alread palette
            set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
        for(size_t i = 1; i < size(); i++)
        {
            at(i).r =
            at(i).g =
            at(i).b = ((76 * at(i).r + 150 * at(i).g + 28 * at(i).b) / 254);
        }
        best_match_palette(LASERBOY_GRAYS);
    }
    return;
}

//############################################################################
void LaserBoy_segment::to_palette_by_index(int index)
{
    if(    index != LASERBOY_TRUE_COLOR
        && index != palette_index
        && index < p_space->number_of_palettes()
      )
    {
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::to_target_palette_by_index()
{
    if(    (int)p_space->target_palette_index != LASERBOY_TRUE_COLOR
        && (int)p_space->target_palette_index != palette_index
        && (int)p_space->target_palette_index < p_space->number_of_palettes()
      )
    {
        palette_index = p_space->target_palette_index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::best_match_palette(int index)
{
    if(    index != palette_index
        && index <  p_space->number_of_palettes()
      )
    {
        if(size() > 1)
        {
            if(!p_space->allow_lit_black)
                convert_black_to_blank();
            else
                impose_black_level();

            for(size_t i = 1; i < size(); i++)
                 at(i).c = p_space->palette_picker(index).best_match(at(i));
        }
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::best_match_target_palette()
{
    if(    (int)p_space->target_palette_index != palette_index
        && (int)p_space->target_palette_index < p_space->number_of_palettes()
      )
    {
        if(size() > 1)
            for(size_t i = 1; i < size(); i++)
                 at(i).c = p_space->palette_picker(p_space->target_palette_index).best_match(at(i));
        palette_index = p_space->target_palette_index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_segment::convert_black_to_blank()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).is_black(p_space->black_level))
            {
                at(i).r = 255;
                at(i).g = 0;
                at(i).b = 0;
                at(i).blank();
            }
        }
    return;
}

//############################################################################
void LaserBoy_segment::convert_black_to_color()
{
    if(size() > 1)
    {
        if(palette_index == LASERBOY_TRUE_COLOR)
        {
            for(size_t i = 1; i < size(); i++)
            {
                if(at(i).is_black(p_space->black_level))
                {
                    at(i).c = 55;
                    at(i).r = 255;
                    at(i).g = 255;
                    at(i).b = 255;
                    at(i).unblank();
                }
            }
        }
        else
        {
            for(size_t i = 1; i < size(); i++)
            {
                if(at(i).is_black(p_space->black_level))
                {
                    at(i).c = p_space->palette_picker(palette_index).white;
                    at(i).r = p_space->palette_picker(palette_index).at(at(i).c).r;
                    at(i).g = p_space->palette_picker(palette_index).at(at(i).c).g;
                    at(i).b = p_space->palette_picker(palette_index).at(at(i).c).b;
                    at(i).unblank();
                }
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::convert_blank_to_black()
{
    if(size() > 1)
    {
        at(0).r = 0;
        at(0).g = 0;
        at(0).b = 0;
        at(0).c = p_space->palette_picker(palette_index).black;
        for(size_t i = 1; i < size(); i++) // Leave the zero index blank!
        {
            if(at(i).is_blank())
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
                at(i).c = p_space->palette_picker(palette_index).black;
                at(i).unblank();
            }
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::set_vertex_to_black(int index)
{
    if(size() > 1 && index > 0 && index < (int)size())
    {
        at(index).r = 0;
        at(index).g = 0;
        at(index).b = 0;
        at(index).c = p_space->palette_picker(palette_index).black;
    }
    return;
}

//############################################################################
void LaserBoy_segment::impose_black_level()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).as_color().average() < p_space->black_level)
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
        }
    return;
}

//############################################################################
void LaserBoy_segment::index_palette(int index_multiple, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    u_char  color_index = offset;
    size_t  start =   p_space->palette_picker(palette_index).first,
            span  =   p_space->palette_picker(palette_index).last
                    - p_space->palette_picker(palette_index).first
                    + 1;
    //-----------------------------------------------------------------------
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        for(int j = 0; j < index_multiple; j++)
        {
            if(    (i + j) < size()
                && at(i + j).is_color(p_space->black_level)
              )
                at(i + j).c = color_index % span + start;
        }
        color_index++;
    }
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::index_segments_palette(int index_multiple, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    u_char  color_index = offset;
    size_t  start =   p_space->palette_picker(palette_index).first,
            span  =   p_space->palette_picker(palette_index).last
                    - p_space->palette_picker(palette_index).first
                    + 1;
    //-----------------------------------------------------------------------
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i += index_multiple)
        {
            for(int j = 0; j < index_multiple; j++)
                if((i + j) < segments.size())
                    for(size_t k = 0; k < segments[i + j].size(); k++)
                        if(segments[i + j].at(k).is_color(p_space->black_level))
                            segments[i + j].at(k).c = color_index % span + start;
            color_index++;
        }
       *this = segments.sum_of_frames();
    }
    else
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = color_index % span + start;
    }
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   (i * span_factor)
                                              / (double)(size() - 1)
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_x_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   ((at(i).x + 32767) * span_factor)
                                              / 65535.0
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_y_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   ((at(i).y + 32767) * span_factor)
                                              / 65535.0
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_z_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   ((at(i).z + 32767) * span_factor)
                                              / 65535.0
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_radial_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   (at(i).magnitude())
                                              * p_space->recolor_span_factor
                                              / 32767.0
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_axial_palette(double span_factor, int offset)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = (char)((u_int)(   (   (atan2(at(i).y, at(i).x) + two_pi)
                                              * p_space->recolor_span_factor
                                              / two_pi
                                              * span
                                            ) + offset
                                        )
                                      % span
                                      + start
                                );
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_segments_palette(double span_factor, int offset)
{
    u_char color_index = offset;
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i++)
        {
            color_index += u_char (    (i * span_factor)
                                     / (segments.size() - 1)
                                     * span
                                  ) % span + start;
            for(size_t j = 1; j < segments[i].size(); j++)
                if(segments[i].at(j).is_color(p_space->black_level))
                    segments[i].at(j).c = color_index;
        }
        *this = segments.sum_of_frames();
    }
    else
    {
        for(size_t j = 1; j < size(); j++)
            if(at(j).is_color(p_space->black_level))
                at(j).c = color_index;
    }
    set_rgb_from_palette();
    return;
}


//############################################################################
void LaserBoy_segment::random_color_vectors_palette(int index_multiple)
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    size_t  start =   p_space->palette_picker(palette_index).first,
            span  =   p_space->palette_picker(palette_index).last
                    - p_space->palette_picker(palette_index).first
                    + 1;
    //-----------------------------------------------------------------------
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        for(int j = 0; j < index_multiple; j++)
        {
            if(    (i + j) < size()
                && at(i + j).is_color(p_space->black_level)
              )
                at(i + j).c = (rand() % span) + start;
        }
    }
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::random_color_segments_palette()
{
    if(palette_index == LASERBOY_TRUE_COLOR)
        palette_index = p_space->target_palette_index;
    u_char color_index;
    size_t start =   p_space->palette_picker(palette_index).first,
           span  =   p_space->palette_picker(palette_index).last
                   - p_space->palette_picker(palette_index).first
                   + 1;
    //-----------------------------------------------------------------------
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i++)
        {
            color_index = (rand() % span) + start;
            for(size_t j = 1; j < segments[i].size(); j++)
                if(segments[i].at(j).is_color(p_space->black_level))
                    segments[i].at(j).c = color_index;
        }
        *this = segments.sum_of_frames();
    }
    else
    {
        color_index = (rand() % span) + start;
        for(size_t j = 1; j < size(); j++)
            if(at(j).is_color(p_space->black_level))
                at(j).c = color_index;
    }
    set_rgb_from_palette();
    return;
}

//############################################################################
void LaserBoy_segment::index_hues(int index_multiple, int offset)
{
    u_short hue_index = offset;
    palette_index   = LASERBOY_TRUE_COLOR;
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        for(int j = 0; j < index_multiple; j++)
        {
            if(    (i + j) < size()
                && at(i + j).is_color(p_space->black_level)
              )
            {
                hue_index = hue_index % 1530;
                at(i + j) = LaserBoy_color(hue_index);
            }
        }
        hue_index++;
    }
    return;
}

//############################################################################
void LaserBoy_segment::index_segments_hues(int index_multiple, int offset)
{
    u_short color_index = offset;
    palette_index = LASERBOY_TRUE_COLOR;
    //-----------------------------------------------------------------------
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i += index_multiple)
        {
            for(int j = 0; j < index_multiple; j++)
                if((i + j) < segments.size())
                    for(size_t k = 0; k < segments[i + j].size(); k++)
                        if(segments[i + j].at(k).is_color(p_space->black_level))
                            segments[i + j].at(k) = LaserBoy_color(u_short(color_index % 1530));
            color_index++;
        }
       *this = segments.sum_of_frames();
    }
    else
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color(u_short(color_index % 1530));
    }
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(((int)(((i * span_factor) / (double)(size() - 1)) * 1530) + offset) % 1530));
    return;
    sync_rgb_and_palette();
}

//############################################################################
void LaserBoy_segment::span_x_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(((int)(   ((at(i).x + 32767) * span_factor) / 65535.0
                                                         * 1530
                                                       )
                                                     + offset
                                                 )
                                               % 1530));
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_y_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(((int)(   ((at(i).y + 32767) * span_factor) / 65535.0
                                                         * 1530
                                                       )
                                                     + offset
                                                 )
                                               % 1530));
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_z_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(((int)(   ((at(i).z + 32767) * span_factor) / 65535.0
                                                         * 1530
                                                       )
                                                     + offset
                                                 )
                                               % 1530));
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_radial_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(   (int)(   at(i).magnitude()
                                                           * p_space->recolor_span_factor
                                                           / 32767.0
                                                           * 1530
                                                           + offset
                                                         )
                                                  % 1530
                                                )
                                      );
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_axial_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(   (int)(  (atan2(at(i).y, at(i).x) + two_pi)
                                                           * p_space->recolor_span_factor
                                                           / two_pi
                                                           * 1530
                                                           + offset
                                                         )
                                                  % 1530
                                                )
                                      );
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::span_segments_hues(double span_factor, int offset)
{
    u_short color_index = offset;
    palette_index = LASERBOY_TRUE_COLOR;
    //-----------------------------------------------------------------------
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i++)
        {
            color_index += u_short(    (i * span_factor)
                                     / (segments.size() - 1)
                                     * 1530
                                  ) % 1530;
            for(size_t j = 1; j < segments[i].size(); j++)
                if(segments[i].at(j).is_color(p_space->black_level))
                    segments[i].at(j) = LaserBoy_color(color_index);
        }
        *this = segments.sum_of_frames();
    }
    else
    {
        for(size_t j = 1; j < size(); j++)
            if(at(j).is_color(p_space->black_level))
                at(j) = LaserBoy_color(color_index);
    }
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_segment::random_color_vectors_hues(int index_multiple)
{
    u_short hue_index = 0;
    palette_index   = LASERBOY_TRUE_COLOR;
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        hue_index = rand() % 1530;
        for(int j = 0; j < index_multiple; j++)
        {
            if(    (i + j) < size()
                && at(i + j).is_color(p_space->black_level)
              )
                at(i + j) = LaserBoy_color(hue_index);
        }
        hue_index++;
    }
    return;
}

//############################################################################
void LaserBoy_segment::random_color_segments_hues()
{
    u_short hue_index = 0;
    palette_index = LASERBOY_TRUE_COLOR;
    if(number_of_segments() > 1)
    {
        LaserBoy_frame_set segments = explode_segments();
        for(size_t i = 0; i < segments.size(); i++)
        {
            hue_index = rand() % 1530;
            for(size_t j = 1; j < segments[i].size(); j++)
                if(segments[i].at(j).is_color(p_space->black_level))
                    segments[i].at(j) = LaserBoy_color(hue_index);
        }
        *this = segments.sum_of_frames();
    }
    else
    {
        hue_index = rand() % 1530;
        for(size_t j = 1; j < size(); j++)
            if(at(j).is_color(p_space->black_level))
                at(j) = LaserBoy_color(hue_index);
    }
    sync_rgb_and_palette();
    return;
}

//############################################################################
LaserBoy_segment& LaserBoy_segment::rotate_colors(int steps)
{
    if(    palette_index != LASERBOY_TRUE_COLOR
        && size() > 1
      )
    {
        int offset =   p_space->palette_picker(palette_index).first,

            span   =   p_space->palette_picker(palette_index).last
                     - p_space->palette_picker(palette_index).first;
        //----------------------------------------------------------------
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i).c = ((at(i).c - offset + steps) % span) + offset;
        set_rgb_from_palette();
    }
    return *this;
}

//############################################################################
bool LaserBoy_segment::find_rgb_in_palette(const LaserBoy_palette& palette)
{
    bool all_colors_found = true;
    if(size() > 1)
    {
        size_t        i,
                      j;
        vector<bool>  match(size(), false);
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            for(j = 0; j < palette.number_of_colors(); j++)
                if((LaserBoy_color)at(i) == palette.at(j))
                {
                    at(i).c = (u_char)j;
                    match[i] = true;
                }
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            all_colors_found &= match[i];
        //--------------------------------------------------------------------
    }
    return all_colors_found;
}

//############################################################################
void LaserBoy_segment::set_rgb_from_palette()
{
    if(    palette_index != LASERBOY_TRUE_COLOR
        && size() > 1
      )
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_color(p_space->black_level))
            {
                at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
            }
            else if(at(i).is_black(p_space->black_level))
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
        }
    return;
}

//############################################################################
void LaserBoy_segment::set_palette_to_332()
{
    if(size() > 1)
    {
        front().c = 0x00;
        for(size_t i = 1; i < size(); i++)
            at(i).c =    (at(i).r & 0xe0)
                      | ((at(i).g & 0xe0) >> 3)
                      | ((at(i).b & 0xc0) >> 6);
        // does NOT set palette_index to LASERBOY_REDUCED_332
    }
    return;
}

//############################################################################
void LaserBoy_segment::sync_rgb_and_palette()
{
    if(size() > 1)
    {
        if(palette_index == LASERBOY_ILDA_DEFAULT)
        {
            set_rgb_from_palette();
            return;
        }
        //----------------------------------------------------------------
        int i;
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            set_rgb_from_palette();
            for(i = 0; i < p_space->number_of_palettes(); i++)
                if(p_space->palette_picker(palette_index).is_in(p_space->palette_picker(i)))
                {
                    best_match_palette(i);
                    break;
                }
            return;
        }
        //----------------------------------------------------------------
        size_t            j;
        LaserBoy_palette  palette;
        //----------------------------------------------------------------
        if(first_lit_vector_index() > -1) // there are lit vectors!
        {
            palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
            //----------------------------------------------------------------
            for(i = first_lit_vector_index() + 1; i < (int)size(); i++)
            {
                for(j = 0; j < palette.number_of_colors(); j++)
                    if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                        break;
                if(    j == palette.number_of_colors()
                    && at(i).is_lit()
                  )
                    palette.push_back((LaserBoy_color)at(i));
                if(palette.number_of_colors() > LASERBOY_PALETTE_MAX) // cannot be reduced to a palette
                {
                    set_palette_to_332(); // palette_index is still LASERBOY_TRUE_COLOR
                    return;
                }
            }
            //----------------------------------------------------------------
            if(palette.is_in(p_space->palette_picker(LASERBOY_ILDA_DEFAULT)))
            {
                best_match_palette(LASERBOY_ILDA_DEFAULT);
                return;
            }
            else
            {
                for(i = p_space->number_of_palettes() - 1; i >= 1; i--)
                    if(palette.is_in(p_space->palette_picker(i)))
                    {
                        best_match_palette(i);
                        return;
                    }
                palette.reorder();
                palette.find_factors();
                find_rgb_in_palette(palette);
                palette.name = GUID8char();
                p_space->push_back_palette(palette);
                palette_index = p_space->number_of_palettes() - 1;
                return;
            }
        } // end if(first_lit_vector_index() > -1)
    //------------------------------------------------------------------------
    } // end if(size() > 1)
    palette_index = LASERBOY_ILDA_DEFAULT;
    return;
}

//############################################################################
void LaserBoy_segment::bit_reduce_to_palette()
{
    if(palette_index == LASERBOY_TRUE_COLOR)
    {
        set_palette_to_332();
        palette_index = LASERBOY_REDUCED_332;
        set_rgb_from_palette();
    }
    return;
}

//############################################################################
void LaserBoy_segment::best_reduce_to_palette()
{
    if(number_of_color_vectors() >= 1)
    {
        int               local_palette_index;
        size_t            i,
                          j;
        LaserBoy_palette  palette;
        //----------------------------------------------------------------
        if(!p_space->allow_lit_black)
            convert_black_to_blank();
        else
            impose_black_level();
        //----------------------------------------------------------------
        palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
        //----------------------------------------------------------------
        for(i = first_lit_vector_index() + 1; i < size(); i++)
        {
            for(j = 0; j < palette.number_of_colors(); j++)
                if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                    break;
            if(    j == palette.number_of_colors()
                && at(i).is_lit()
              )
                palette.push_back((LaserBoy_color)at(i));
        }
        //----------------------------------------------------------------
        local_palette_index = palette.best_reduction();
        if(local_palette_index == -1)
        {
            palette.name = GUID8char();
            p_space->push_back_palette(palette);
            best_match_palette(p_space->number_of_palettes() - 1);
        }
        else
            best_match_palette(local_palette_index);
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_segment::promote_to_true_color()
{
    if(palette_index != LASERBOY_TRUE_COLOR)
    {
        sync_rgb_and_palette();
        palette_index = LASERBOY_TRUE_COLOR;
    }
    return;
}

//############################################################################
void LaserBoy_segment::shade(u_char shade) // 0 shade is no change 255 is black
{
    if(shade)
    {
        sync_rgb_and_palette();
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            LaserBoy_palette  palette(p_space->palette_picker(palette_index));
            //----------------------------------------------------------------
            palette.shade(shade);
            //----------------------------------------------------------------
            palette.name = GUID8char();
            p_space->push_back_palette(palette);
            //----------------------------------------------------------------
            palette_index = p_space->number_of_palettes() - 1;
            set_rgb_from_palette();
        }
        else
        {
            if(size() > 1)
                for(size_t i = 1; i < size(); i++)
                {
                    (at(i).r - shade > 0) ? (at(i).r -= shade) : (at(i).r = 0);
                    (at(i).g - shade > 0) ? (at(i).g -= shade) : (at(i).g = 0);
                    (at(i).b - shade > 0) ? (at(i).b -= shade) : (at(i).b = 0);
                }
        }
    }
    return;
}

//############################################################################
void LaserBoy_segment::tint(u_char tint) // 0 tint is no change 255 is white
{
    if(tint)
    {
        sync_rgb_and_palette();
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            LaserBoy_palette   palette(p_space->palette_picker(palette_index));
            //----------------------------------------------------------------
            palette.tint(tint);
            //----------------------------------------------------------------
            palette.name = GUID8char();
            p_space->push_back_palette(palette);
            //----------------------------------------------------------------
            palette_index = p_space->number_of_palettes() - 1;
            set_rgb_from_palette();
        }
        else
        {
            if(size() > 1)
                for(size_t i = 1; i < size(); i++)
                {
                    (at(i).r + tint < 255) ? (at(i).r += tint) : (at(i).r = 255);
                    (at(i).g + tint < 255) ? (at(i).g += tint) : (at(i).g = 255);
                    (at(i).b + tint < 255) ? (at(i).b += tint) : (at(i).b = 255);
                }
        }
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::color_from_bmp(const string& file)
{
    char file_name[256];
    struct LaserBoy_bmp bmp = {0};
    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        color_from_bmp(&bmp);
        bmp_free(&bmp);
        return LASERBOY_OK;
    }
    return LASERBOY_FILE_OPEN_FAILED;
}

//############################################################################
void LaserBoy_segment::color_from_bmp(struct LaserBoy_bmp* bmp)
{
    if(size() > 1)
    {
        size_t            i;
        u_int             color,
                          x_off  = 0,
                          y_off  = 0;
        double            factor =  bmp->xres > bmp->yres
                                  ? 65535.0 / bmp->xres
                                  : 65535.0 / bmp->yres;
        LaserBoy_segment  line;
        LaserBoy_vertex   vertex;
        //----------------------------------------------------------------
        if(bmp->xres > bmp->yres)
            y_off = (bmp->xres - bmp->yres) / 2;
        else if(bmp->xres < bmp->yres)
            x_off = (bmp->yres - bmp->xres) / 2;
        //----------------------------------------------------------------
        if(bmp->bpp <= 8)
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                color = bmp->get_pixel(  bmp,
                                         (int)((vertex.x + 32767.0) / factor - x_off),
                                         (int)((vertex.y + 32767.0) / factor - y_off)
                                      );
                at(i).r = bmp_get_palette_index_r(bmp, color);
                at(i).g = bmp_get_palette_index_g(bmp, color);
                at(i).b = bmp_get_palette_index_b(bmp, color);
            }
        //----------------------------------------------------------------
        else
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                color = bmp->get_pixel(  bmp,
                                         (int)((vertex.x + 32767.0) / factor - x_off),
                                         (int)((vertex.y + 32767.0) / factor - y_off)
                                      );
                at(i).r = bmp->r_from_rgb(color);
                at(i).g = bmp->g_from_rgb(color);
                at(i).b = bmp->b_from_rgb(color);
            }
        //----------------------------------------------------------------
        palette_index = LASERBOY_TRUE_COLOR;
        sync_rgb_and_palette();
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_segment::subtract_bmp(const string& file)
{
    char file_name[256];
    struct LaserBoy_bmp bmp = {0};
    strcpy(file_name, (file).c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        subtract_bmp(&bmp);
        bmp_free(&bmp);
        return LASERBOY_OK;
    }
    return LASERBOY_FILE_OPEN_FAILED;
}

//############################################################################
void LaserBoy_segment::subtract_bmp(struct LaserBoy_bmp* bmp)
{
    if(size() > 1)
    {
        u_char            mask;
        size_t            i;
        u_int             pixle_color,
                          x_off  = 0,
                          y_off  = 0;
        double            factor =  bmp->xres > bmp->yres
                                  ? 65535.0 / bmp->xres
                                  : 65535.0 / bmp->yres;
        LaserBoy_segment  line;
        LaserBoy_vertex   vertex;
        //----------------------------------------------------------------
        if(bmp->xres > bmp->yres)
            y_off = (bmp->xres - bmp->yres) / 2;
        else if(bmp->xres < bmp->yres)
            x_off = (bmp->yres - bmp->xres) / 2;
        //----------------------------------------------------------------
        if(bmp->bpp <= 8)
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                pixle_color = bmp->get_pixel(  bmp,
                                               (int)((vertex.x + 32767.0) / factor - x_off),
                                               (int)((vertex.y + 32767.0) / factor - y_off)
                                            );
                mask = 255 - bmp_get_palette_index_r(bmp, pixle_color);
                (at(i).r - mask > 0) ? (at(i).r -= mask) : (at(i).r = 0);
                mask = 255 - bmp_get_palette_index_g(bmp, pixle_color);
                (at(i).g - mask > 0) ? (at(i).g -= mask) : (at(i).g = 0);
                mask = 255 - bmp_get_palette_index_b(bmp, pixle_color);
                (at(i).b - mask > 0) ? (at(i).b -= mask) : (at(i).b = 0);
            }
        //----------------------------------------------------------------
        else
            for(i = 1; i < size(); i++)
            {
                line.clear();
                line.push_back(at(i - 1));
                line.push_back(at(i    ));
                vertex = line.rectangular_center_of();
                pixle_color = bmp->get_pixel(  bmp,
                                               (int)((vertex.x + 32767.0) / factor - x_off),
                                               (int)((vertex.y + 32767.0) / factor - y_off)
                                            );
                mask = 255 - bmp->r_from_rgb(pixle_color);
                (at(i).r - mask > 0) ? (at(i).r -= mask) : (at(i).r = 0);
                mask = 255 - bmp->g_from_rgb(pixle_color);
                (at(i).g - mask > 0) ? (at(i).g -= mask) : (at(i).g = 0);
                mask = 255 - bmp->b_from_rgb(pixle_color);
                (at(i).b - mask > 0) ? (at(i).b -= mask) : (at(i).b = 0);
            }
        //----------------------------------------------------------------
        palette_index = LASERBOY_TRUE_COLOR;
        sync_rgb_and_palette();
    }
    return;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::apply_view()
{
    LaserBoy_real_segment rs = to_real_segment(false);
    rs.rotate_around_origin(p_space->view_angle );
    rs.scale_around_origin (p_space->view_scale );
    rs.move                (p_space->view_offset * p_space->view_scale);
    if(p_space->destructive_clipping)
    {
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    else
    {
        LaserBoy_3D_double max( 32767,  32767,  32767);
        LaserBoy_3D_double min(-32767, -32767, -32767);
        for(size_t i = 0; i < size(); i++)
            if(LaserBoy_bounds_check(rs.at(i), max, min))
                return LaserBoy_bounds_check(rs.at(i), max, min);
    }
    *this = rs;
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move(LaserBoy_3D_double d)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  _d;
        //----------------------------------------------------------------
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
            {
                _d = d + at(i).as_3D_double();
                out_of_bounds = LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = 0; i < size(); i++)
            {
                _d = d + at(i).as_3D_double();
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.move(d);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move()
{
    return move(p_space->view_offset);
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale(LaserBoy_3D_double s)
{
    return scale_on_coordinates(centroid_of_coordinates(), s);
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_on_coordinates(LaserBoy_3D_double p, LaserBoy_3D_double s)
{
    if(size() > 1)
    {
        size_t              i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  d;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if((    out_of_bounds
                     |= LaserBoy_bounds_check(((at(i).as_3D_double() - p) * s) + p, LASERBOY_CUBE)
                  ))
                    return out_of_bounds;
            //----------------------------------------------------------------
            for(i = 0; i < size(); i++)
                at(i) = ((at(i).as_3D_double() - p) * s) + p;
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.scale_on_coordinates(p, s);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_around_origin(LaserBoy_3D_double f)
{
    if(size() > 1)
    {
        size_t             i;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double _d;
        if(!p_space->destructive_clipping)
        {
            for(i = 1; i < size(); i++)
            {
                if(at(i).is_lit())
                {
                    _d = f * at(i).as_3D_double();
                    out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                    _d = f * at(i - 1).as_3D_double();
                    out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                    if(out_of_bounds)
                        return out_of_bounds;
                }
            }
            for(i = 0; i < size(); i++)
            {
               _d = f * at(i).as_3D_double();
               at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        rs.scale_around_origin(f);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_around_origin()
{
    return scale_around_origin(p_space->view_scale);
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::rectangular_center_of() const
{
    LaserBoy_3D_double center;
    if(size() > 1)
    {
        double         max_x = -40000, // more negative than any short
                       min_x =  40000, // more positive than any short
                       max_y = -40000,
                       min_y =  40000,
                       max_z = -40000,
                       min_z =  40000;
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).x > max_x)    max_x = at(i).x;
            if(at(i).x < min_x)    min_x = at(i).x;
            if(at(i).y > max_y)    max_y = at(i).y;
            if(at(i).y < min_y)    min_y = at(i).y;
            if(at(i).z > max_z)    max_z = at(i).z;
            if(at(i).z < min_z)    min_z = at(i).z;
        }
        center.x = max_x - ((max_x - min_x) / 2.0);
        center.y = max_y - ((max_y - min_y) / 2.0);
        center.z = max_z - ((max_z - min_z) / 2.0);
    }
    return center;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::mean_of_coordinates() const
{
    LaserBoy_3D_double mean;
    if(size() > 1)
    {
        mean = front();
        for(size_t i = 1; i < size(); i++)
            mean += at(i);
        mean /= size();
    }
    return mean;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_coordinates_xy(double& area, LaserBoy_segment& s) const
{
    LaserBoy_real_segment rs;
    LaserBoy_3D_double c = to_real_segment(false).centroid_of_coordinates_xy(area, rs);
    s = rs;
    return c;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_coordinates_zy(double& area, LaserBoy_segment& s) const
{
    LaserBoy_real_segment rs;
    LaserBoy_3D_double c = to_real_segment(false).centroid_of_coordinates_zy(area, rs);
    s = rs;
    return c;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_coordinates_xz(double& area, LaserBoy_segment& s) const
{
    LaserBoy_real_segment rs;
    LaserBoy_3D_double c = to_real_segment(false).centroid_of_coordinates_xz(area, rs);
    s = rs;
    return c;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_coordinates() const
{
    return to_real_segment(false).centroid_of_coordinates();
}

//############################################################################
size_t LaserBoy_segment::number_of_segments() const // a segment is a series of lit vertices
{
    size_t i,
           segment_count = 0;
    if(size() > 1)
    {
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                segment_count++;
            }
        }
    }
    return segment_count;
}

//############################################################################
LaserBoy_segment LaserBoy_segment::blend(const LaserBoy_segment& segment, double ratio)
{
    if(ratio == 0.0)
        return *this;
    else if(ratio == 1.0)
        return segment;
    else if(    size() > 1
             && segment.size() > 1
           )
    {
        size_t           i,
                         diff;
        LaserBoy_segment this_one(*this),
                         other_one(segment),
                         combo;
        this_one.convert_blank_to_black();
        other_one.convert_blank_to_black();
        combo.palette_index = LASERBOY_TRUE_COLOR;
        if(this_one.size() > other_one.size())
            while(this_one.size() / other_one.size() > 1)
                other_one += other_one;
        else
            while(other_one.size() / this_one.size() > 1)
                this_one += this_one;
        if(this_one.size() > other_one.size())
        {
            diff = (this_one.size() - other_one.size());
            for(i = 0; i < diff; i++)
                other_one += other_one[i];
        }
        else if(this_one.size() < other_one.size())
        {
            diff = (other_one.size() - this_one.size());
            for(i = 0; i < diff; i++)
                this_one += this_one[i];
        }
        combo.reserve(this_one.size());
        for(i = 0; i < this_one.size(); i++)
            combo += this_one.at(i).blend(other_one.at(i), ratio);
        return combo;
    }
    else
        return *this;
}

//############################################################################
bool LaserBoy_segment::find_segment_at_index(size_t segment_index, size_t& start, size_t& end) const
{   // the first segment is number zero!
    if(size() > 1)
    {
        size_t i;
        int    segment_count = -1;
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1; // anchor of first lit vector in segment
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                segment_count++;
                if(segment_count == (int)segment_index)
                    return true;
            }
        } // segment_index out of range
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                return false; // and set start, end to first segment
            }
        }
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
bool LaserBoy_segment::find_segment_of_vertex(size_t vertex_index, size_t& start, size_t& end, size_t& segment_index) const
{
    start = end = segment_index = 0;
    if(size() > 1)
    {
        size_t i,
               segment_count = 0; // the first segment is number zero!
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                end = i - 1;

                if(    vertex_index >= start
                    && vertex_index <= end
                  )
                {
                    segment_index = segment_count;
                    return true;
                }

                if(vertex_index < start)
                {
                    segment_index = segment_count;
                    return false;
                }
                segment_count++;
            }
        }
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
bool LaserBoy_segment::same_as(const LaserBoy_segment& segment) const
{
    LaserBoy_segment s1(*this),
                     s2(segment);
    s1.reduce_blank_vectors();
    s1.reduce_lit_vectors();
    s1.remove_dwell_vertices();
    s2.reduce_blank_vectors();
    s2.reduce_lit_vectors();
    s2.remove_dwell_vertices();
    s2 += s1;
    s2.omit_equivalent_vectors();
    s2.reduce_blank_vectors();
    s2.reduce_lit_vectors();
    s2.remove_dwell_vertices();
    return (s1.size() == s2.size());
}

//############################################################################
LaserBoy_segment LaserBoy_segment::copy_segment(size_t segment_index) const
{
    if(size() > 1)
    {
        size_t            start,
                          end;
        LaserBoy_segment segment(palette_index, false);
        find_segment_at_index(segment_index, start, end);
        segment.reserve((end - start) + 1);
        for(size_t i = start; i <= end; i++)
            segment.push_back(at(i));
        return segment;
    }
    return LaserBoy_segment(palette_index, true);
}

//############################################################################
LaserBoy_frame_set LaserBoy_segment::explode_segments() const
{
    LaserBoy_frame_set segments;
    //------------------------------------------------------------------------
    if(size() > 1)
    {
        size_t          i,
                        j,
                        start,
                        end;
        LaserBoy_frame  frame(palette_index, false);
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1; // anchor to the first lit vertex
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                frame.clear();
                frame.reserve(end - start + 1);
                for(j = start; j <= end; j++)
                    frame += at(j);
                segments.push_back(frame);
            }
        }
    }
    //------------------------------------------------------------------------
    return segments;
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::rectangular_center_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).rectangular_center_of();
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::mean_of_coordinates_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).mean_of_coordinates();
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_segment_xy(size_t segment_index, double& area, LaserBoy_segment& s) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_xy(area, s);
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_segment_zy(size_t segment_index, double& area, LaserBoy_segment& s) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_zy(area, s);
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_segment_xz(size_t segment_index, double& area, LaserBoy_segment& s) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_xz(area, s);
}

//############################################################################
LaserBoy_3D_double LaserBoy_segment::centroid_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates();
}

//############################################################################
size_t LaserBoy_segment::segment_index_of_vertex(size_t vertex_index) const
{
    size_t start, end, segment_index;
    find_segment_of_vertex(vertex_index, start, end, segment_index);
    return segment_index;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::move_segment(size_t segment_index, LaserBoy_3D_double f)
{
    if(size() > 1)
    {
        size_t             i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double _d;
        find_segment_at_index(segment_index, start, end);
        if(!p_space->destructive_clipping)
        {
            for(i = start; i <= end; i++)
            {
                _d = f + at(i).as_3D_double();
                out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = start; i <= end; i++)
            {
                _d = f + at(i).as_3D_double();
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = start; i <= end; i++)
            rs.at(i) += f;
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_segment(size_t segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t             i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double _d,
                           center;
        find_segment_at_index(segment_index, start, end);
        center = centroid_of_segment(segment_index);
        if(!p_space->destructive_clipping)
        {
            for(i = start; i <= end; i++)
            {
                _d = rotate_vertex_on_coordinates(at(i), center, a);
                out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = start; i <= end; i++)
            {
                _d = rotate_vertex_on_coordinates(at(i), center, a);
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = start; i <= end; i++)
            rs.at(i) = rotate_vertex_on_coordinates(rs.at(i), center, a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::rotate_segment_around_origin(size_t segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t             i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double _d;
        find_segment_at_index(segment_index, start, end);
        if(!p_space->destructive_clipping)
        {
            for(i = start; i <= end; i++)
            {
                _d = rotate_vertex(at(i), a);
                out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_SPHERE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = start; i <= end; i++)
            {
                _d = rotate_vertex(at(i), a);
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = start; i <= end; i++)
            rs.at(i) = rotate_vertex(rs.at(i), a);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_segment(size_t segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        size_t             i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short  center;
        LaserBoy_3D_double _d;
        find_segment_at_index(segment_index, start, end);
        center = centroid_of_segment(segment_index);
        if(!p_space->destructive_clipping)
        {
            for(i = start; i <= end; i++)
            {
                _d = scale_vertex_on_coordinates(at(i), center, m);
                out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = start; i <= end; i++)
            {
                _d = scale_vertex_on_coordinates(at(i), center, m);
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = start; i <= end; i++)
            rs.at(i) = scale_vertex_on_coordinates(rs.at(i), center, m);
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_segment::scale_segment_around_origin(size_t segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        size_t             i,
                           start,
                           end;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double _d;
        find_segment_at_index(segment_index, start, end);
        if(!p_space->destructive_clipping)
        {
            for(i = start; i <= end; i++)
            {
                _d = m * at(i).as_3D_double();
                out_of_bounds |= LaserBoy_bounds_check(_d, LASERBOY_CUBE);
                if(out_of_bounds)
                    return out_of_bounds;
            }
            for(i = start; i <= end; i++)
            {
                _d = m * at(i).as_3D_double();
                at(i) = _d;
            }
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = start; i <= end; i++)
            rs.at(i) *= m;
        rs.clip();
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_real_segment LaserBoy_segment::to_real_segment(bool add_origin) const
{
    LaserBoy_real_vertex  rv;
    LaserBoy_real_segment rs;
    rs.palette_index      = palette_index;
    rs.real_segment_error = segment_error;
    if(add_origin)
    {
        rs.reserve(size() + 2);
        rs.push_back(rv);
        rs.push_back(rv); // origin vector
    }
    else
        rs.reserve(size());
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
        {
            rv.x = at(i).x;
            rv.y = at(i).y;
            rv.z = at(i).z;
            rv.r = at(i).r;
            rv.g = at(i).g;
            rv.b = at(i).b;
            rv.k = at(i).k;
            rv.c = at(i).c;
            rs.push_back(rv);
        }
    return rs;
}

//############################################################################
void LaserBoy_segment::to_fstream_wav(std::fstream&        out,
                                      LaserBoy_wav_header& header,
                                      bool                 end_of_frame,
                                      bool                 unique_frame
                                     )
{
    if(size() > 1)
    {
        size_t i;
        for(i = 0; i < size() - 1; i++)
            at(i).to_fstream_wav(out,
                                 header,
                                 p_space->signal_bit_mask,
                                 false,
                                 false,
                                 p_space->invert_wav_output
                                );
        at(i).to_fstream_wav(out,
                             header,
                             p_space->signal_bit_mask,
                             end_of_frame,
                             unique_frame,
                             p_space->invert_wav_output
                            );
        if(end_of_frame)
            header.num_frames++;
    }
    return;
}

//############################################################################
LaserBoy_segment blank_segment()
{
    LaserBoy_segment segment(LASERBOY_ILDA_DEFAULT, false);
    segment.reserve(2);
    segment.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
    segment.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
    return segment;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
