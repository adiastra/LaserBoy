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
// LaserBoy_frame.cpp is part of LaserBoy.
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
#include "LaserBoy_SDL_GUI.hpp"

//############################################################################
LaserBoy_frame::LaserBoy_frame(LaserBoy_vertex(*F)(int, int),
                               int vertices_per_frame,
                               int frame_index
                              )
               : LaserBoy_ild_header(     )
               , LaserBoy_segment   (     )
               , is_selected        (false)
               , is_unique          (true )
               , is_wagged          (false)
               , egg                (0    )
               , spider             (0    )
               , intro              (     )
               , bridge             (     )
               , coda               (     )
{
    is_selected            = false;
    is_unique              = true;
    is_wagged              = false;
    palette_index          = LASERBOY_LASERBOY_HUES;
    format                 = LASERBOY_3D_FRAME;
    LaserBoy_vertex vertex = F(0, frame_index);
    vertex.blank();
    reserve(vertices_per_frame + 2);
    push_back(vertex);
    for(int p = 0; p < vertices_per_frame; p++)
        push_back(F(p, frame_index));
    push_back(vertex);
    quantity = size();
}

//############################################################################
bool LaserBoy_frame::save_as_ild(const string& file, LaserBoy_ild_header_count& counter)
{
    if(size() > 1)
    {
        std::ofstream out(file.c_str(), ios::out | ios::binary);
        if(out.is_open())
        {
            LaserBoy_ild_header header;
            header.total  = 1;
            header.owner  = "LaserBoy";
            owner         = "LaserBoy";
            if(name.size() == 0 || name == "ESI.....")
                name = "ESI00000";
            if(p_space->save_2D_as_3D)
                format = LASERBOY_3D_FRAME;
            if(!p_space->save_ild_fmt_4_5)
            {
                if(palette_index == LASERBOY_TRUE_COLOR)
                {
                    header.format       = LASERBOY_PALETTE;
                    header.name         = p_space->palette_picker(LASERBOY_REDUCED_332).name;
                    header.quantity     = p_space->palette_picker(LASERBOY_REDUCED_332).number_of_colors();
                    header.identity     = 0;
                    header.total        = 3;
                    header.to_ofstream_ild(out);
                    p_space->palette_picker(LASERBOY_REDUCED_332).to_ofstream_ild(out);
                    counter._2++;

                    header.format = LASERBOY_COLOR_TABLE;
                    header.name         = name;
                    header.quantity     = size();
                    header.identity     = 1;
                    header.to_ofstream_ild(out);
                    as_color_table().to_ofstream_ild(out);
                    counter._3++;
                }
                else if(palette_index != LASERBOY_ILDA_DEFAULT)
                {
                    header.format       = LASERBOY_PALETTE;
                    header.name         = p_space->palette_picker(palette_index).name;
                    header.quantity     = p_space->palette_picker(palette_index).number_of_colors();
                    header.identity     = 0;
                    header.total        = 2;
                    header.to_ofstream_ild(out);
                    p_space->palette_picker(palette_index).to_ofstream_ild(out);
                    counter._2++;
                }
            } // end if(!p_space->save_ild_fmt_4_5)
            //----------------------------------------------------------------
            LaserBoy_frame copy(*this);
            if(p_space->auto_minimize)
                copy.minimize(0);
            else if(p_space->save_1_frame_bridge && bridge.size() > 2)
                copy += bridge;
            copy.sync_rgb_and_palette();
            copy.quantity = copy.size();
            copy.total    = header.total;
            copy.identity = header.total - 1;
            if(p_space->save_ild_fmt_4_5)
            {
                if(copy.format == LASERBOY_3D_FRAME) copy.format = LASERBOY_3D_FRAME_RGB;
                if(copy.format == LASERBOY_2D_FRAME) copy.format = LASERBOY_2D_FRAME_RGB;
            }
            copy.LaserBoy_ild_header::to_ofstream_ild(out);
            for(size_t i = 0; i < copy.size() - 1; i++)
                copy.at(i).to_ofstream_ild(out, copy.format, false);
            copy.back().to_ofstream_ild(out, copy.format, true); // last vertex
            switch(copy.format)
            {
                case 0:
                    counter._0++;
                    break;
                case 1:
                    counter._1++;
                    break;
                case 4:
                    counter._4++;
                    break;
                case 5:
                    counter._5++;
                    break;
                default:
                    break;
            }
            //----------------------------------------------------------------
            if(p_space->save_ild_fmt_4_5)
                header.format = LASERBOY_3D_FRAME_RGB;
            else
                header.format = LASERBOY_3D_FRAME;
            header.name     = "ILDA_End";
            header.owner    = p_space->install_GUID;
            header.quantity = 0;
            header.identity = header.total;
            header.to_ofstream_ild(out);
            counter._e++;
            out.close();
            is_2D();
        } // end if(out.is_open())
        else
            return false;
        //--------------------------------------------------------------------
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_frame::save_as_ctn(const string& file)
{
    std::ofstream out(file.c_str(), ios::out | ios::binary);
    if(out.is_open())
    {
        to_ofstream_ctn(out);
        out.close();
    }
    else
        return false;
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
bool LaserBoy_frame::save_as_txt(const string& file)
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        //----------------------------------------------------------------
        LaserBoy_frame copy(*this);
        if(p_space->auto_minimize)
            copy.minimize(0);
        //----------------------------------------------------------------
        if(!p_space->save_txt_with_color)
        {
            if(copy.palette_index != LASERBOY_TRUE_COLOR)
            {
                if(    p_space->save_txt_named_palettes
                    || copy.palette_index >= LASERBOY_BUILT_IN_PALETTES
                  )
                    p_space->palette_picker(copy.palette_index).to_ofstream_txt(out);
                else
                {
                    out << "palette named "
                        << p_space->palette_picker(copy.palette_index).name
                        << ENDL
                        << ENDL;
                }
            }
            else
                copy.to_ofstream_txt_color_table(out);
        }
        copy.to_ofstream_txt(out, 0);
        out.close();
    }
    else
        return false;
        //----------------------------------------------------------------
    return true;
}

//############################################################################
bool LaserBoy_frame::save_as_txt_table(const string& file)
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        to_ofstream_txt_color_table(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::optimize(LaserBoy_3D_short point_of_entry) // last point of previous frame
{
    int                  i,
                         j,
                         dwell_samples;
    LaserBoy_3D_double   _0,
                         _1,
                         _2;
    LaserBoy_vertex      vertex,
                         black_vertex;
    LaserBoy_Error_Code  stat = LASERBOY_OK;
    //------------------------------------------------------------------------
    vertex = front();
    vertex.blank();
    //------------------------------------------------------------------------
    stat |= add_dwell();
    if(p_space->dots_setting == LASERBOY_DOTS_ENHANCE)
        stat |= enhance_dots();
    stat |= add_lit_span_vertices();
    stat |= add_blank_span_vertices();
    //------------------------------------------------------------------------
    is_wagged = false;
    //------------------------------------------------------------------------
    // intro from the origin to the first vertex
    //------------------------------------------------------------------------
    intro.clear();
    intro += LaserBoy_segment(point_of_entry, vertex); // line from entry point to first vertex
    //------------------------------------------------------------------------
    _0 = point_of_entry;
    _1 =
    _2 = front();
    //------------------------------------------------------------------------
    j  = 0;
    while(    (_1 == _2)
           && (++j < (int)size())
         )
        _2 = at(j);
    //------------------------------------------------------------------------
    if(_0 == _1)
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = start_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_millisec
                                            );
    else
        dwell_samples = start_dwell_vertices(delta_angle(_0 | _2,
                                                         _0 | _1,
                                                         _1 | _2
                                                        ),
                                             p_space->sample_rate,
                                             p_space->max_dwell_millisec
                                            );
    if(dwell_samples) // dwell from the line to the first lit vector
        for(i = 0; i < dwell_samples; i++)
            intro.push_back(vertex); // front() all blank
    //------------------------------------------------------------------------
    // bridge to join the end back to the biginning
    //------------------------------------------------------------------------
    bridge.clear();
    _0 =
    _1 = back();
    _2 = front();
    //------------------------------------------------------------------------
    j  = 0;
    while(    (_1 == _2)
           && (++j < (int)size())
         )
        _2 = at(j);
    //------------------------------------------------------------------------
    j  = size() - 1;
    while(    (_0 == _1)
           && (--j >= 0)
         )
        _0 = at(j);
    //------------------------------------------------------------------------
    if(    (_0 == _1)
        || (_1 == _2)
      )
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = end_dwell_vertices(pi,
                                           p_space->sample_rate,
                                           p_space->max_dwell_millisec
                                          );
    else
        dwell_samples = end_dwell_vertices(delta_angle(_0 | _2,
                                                       _0 | _1,
                                                       _1 | _2
                                                      ),
                                           p_space->sample_rate,
                                           p_space->max_dwell_millisec
                                          );
    if(dwell_samples)
    {
        vertex = black_vertex = back();
        if(p_space->black_dwell_vertices)
        {
            black_vertex.c = p_space->palette_picker(palette_index).black;
            black_vertex.r =
            black_vertex.g =
            black_vertex.b = 0x00;
        }
        for(i = 0; i < dwell_samples; i++)
            if(i < (int)p_space->lit_dwell_overhang)
                bridge.push_back(vertex); // dwell on last lit vector
            else
                bridge.push_back(black_vertex);
    } // end if(dwell_samples)
    //------------------------------------------------------------------------
    if(front_to_back_magnitude() >= p_space->insignificant_distance)
    {
        vertex = front();
        vertex.blank();
        bridge += LaserBoy_segment(back(), vertex); // line from last vertex to first for repete
        //----------------------------------------------------------------
        _0 = back();
        _1 =
        _2 = front();
        //----------------------------------------------------------------
        j  = 0;
        while(    (_1 == _2)
               && (++j < (int)size())
             )
            _2 = at(j);
        //----------------------------------------------------------------
        j  = size() - 1;
        while(    (_0 == _1)
               && (--j >= 0)
             )
            _0 = at(j);
        //----------------------------------------------------------------
        if(    (_0 == _1)
            || (_1 == _2)
          )
            dwell_samples = 0;
        else if(_0 == _2)
            dwell_samples = start_dwell_vertices(pi,
                                                 p_space->sample_rate,
                                                 p_space->max_dwell_millisec
                                                );
        else
            dwell_samples = start_dwell_vertices(delta_angle(_0 | _2,
                                                             _0 | _1,
                                                             _1 | _2
                                                            ),
                                                 p_space->sample_rate,
                                                 p_space->max_dwell_millisec
                                                );
        if(dwell_samples) // dwell from the line to the front()
        {
            vertex = front();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
        }
    }
    //------------------------------------------------------------------------
    if(length_in_seconds() < (p_space->max_wag_microsec / 1000000.0))
    {
        dwell_samples = start_dwell_vertices(pi,
                                             p_space->sample_rate,
                                             p_space->max_dwell_millisec
                                            );
        if((int)bridge.size() > 2 * dwell_samples)
        {
            bridge.clear();
            vertex = front();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
            bridge += *this;
            vertex = back();
            vertex.blank();
            for(i = 0; i < dwell_samples; i++)
                bridge.push_back(vertex);
            bridge.reverse();
            is_wagged = true;
        }
    }
    quantity = size();
    return stat;
}

//############################################################################
void LaserBoy_frame::add_coda(LaserBoy_3D_short next_frame_entry_point)
{
    int                 i,
                        j,
                        dwell_samples;
    LaserBoy_3D_double  _0,
                        _1,
                        _2;
    LaserBoy_vertex     vertex,
                        black_vertex;
    //------------------------------------------------------------------------
    // coda is dwell only at the last point before moving toward next frame entry
    //------------------------------------------------------------------------
    coda.clear();
    //------------------------------------------------------------------------
    _0 = at(size() - 2);
    _1 = back();
    _2 = next_frame_entry_point;
    j  = size() - 1;
    while(    (_0 == _1)
           && (--j >= 0)
         )
        _0 = at(j);
    //------------------------------------------------------------------------
    if(_1 == _2)
        dwell_samples = 0;
    else if(_0 == _2)
        dwell_samples = end_dwell_vertices(pi,
                                           p_space->sample_rate,
                                           p_space->max_dwell_millisec
                                          );
    else
        dwell_samples = end_dwell_vertices(delta_angle(_0 | _2,
                                                       _0 | _1,
                                                       _1 | _2
                                                      ),
                                           p_space->sample_rate,
                                           p_space->max_dwell_millisec
                                          );
    //------------------------------------------------------------------------
    vertex = black_vertex = back();
    if(p_space->black_dwell_vertices)
    {
        black_vertex.c = p_space->palette_picker(palette_index).black;
        black_vertex.r =
        black_vertex.g =
        black_vertex.b = 0x00;
    }
    if(dwell_samples > 2)
        for(i = 0; i < (int)dwell_samples; i++)
        {
            if(i < (int)p_space->lit_dwell_overhang)
                coda.push_back(vertex);
            else
                coda.push_back(black_vertex);
        }
    else // coda must exist to convey end_of_frame in optimized wave!
    {
       coda.push_back(black_vertex);
       coda.push_back(black_vertex);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::from_ifstream_format_3(std::ifstream&             in,
                                            const LaserBoy_ild_header& header,
                                            long int&                  bytes_skipped
                                           )
{
    clear();
    *this = header;
    //------------------------------------------------------------------------
    if(quantity)
    {
        u_int                i;
        LaserBoy_color       color;
        LaserBoy_vertex      vertex;
        LaserBoy_ild_header  next_header;
        //----------------------------------------------------------------
        reserve(quantity);
        for(i = 0; i < quantity; i++)
            if(color.from_ifstream_ild(in))
                push_back((LaserBoy_vertex)color);
            else
                return false;
        //----------------------------------------------------------------
        if(next_header.from_ifstream_ild(in, bytes_skipped))
        {
            if(quantity != next_header.quantity)
                segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
            if(quantity < next_header.quantity)
                reserve(next_header.quantity);
            *this = next_header;
            for(i = 0; i < next_header.quantity; i++)
            {
                if(vertex.from_ifstream_ild(in, next_header.format))
                {
                    at(i).x = vertex.x;
                    at(i).y = vertex.y;
                    at(i).z = vertex.z;
                    at(i).k = vertex.k;
                    at(i).c = vertex.c;
                }
                else
                    return false;
            }
            //----------------------------------------------------------------
            front().blank();
            //----------------------------------------------------------------
            palette_index = LASERBOY_TRUE_COLOR;
            sync_rgb_and_palette();
        }
        else
            return false;
    }
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::from_ifstream_ild(std::ifstream&             in,
                                                      const LaserBoy_ild_header& header
                                                     )
{
    u_int            i;
    LaserBoy_vertex  vertex;
    //------------------------------------------------------------------------
    clear();
    *this = header;
    //------------------------------------------------------------------------
    if(    format == LASERBOY_3D_FRAME_RGB
        || format == LASERBOY_2D_FRAME_RGB
      )
        palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    if(quantity)
    {
        reserve(quantity);
        for(i = 0; i < quantity; i++)
        {
            if(vertex.from_ifstream_ild(in, format))
                push_back(vertex);
            else
            {
                segment_error |= LASERBOY_QUANTITY_DATA_MISSMATCH;
                break;
            }
        }
        //----------------------------------------------------------------
        if(size() == 0)
            segment_error |= LASERBOY_EOF;
        //----------------------------------------------------------------
        front().blank();
    }
    //------------------------------------------------------------------------
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::from_ifstream_dxf(std::ifstream& in)
{
    u_int                 i;
    LaserBoy_vertex       vertex;
    LaserBoy_real_segment real_vertices;
    //------------------------------------------------------------------------
    format        = LASERBOY_3D_FRAME ;
    palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    segment_error |= real_vertices.from_ifstream_dxf(in);
    //------------------------------------------------------------------------
    if(real_vertices.size() > 2) // more than the default original vector
    {
        if(p_space->auto_scale_dxf)
            real_vertices.normalize();
        reserve(real_vertices.size() - 2);
        for(i = 2; i < real_vertices.size(); i++)
            push_back((LaserBoy_vertex)real_vertices[i]);
    }
    else
    {
        reserve(2);
        push_back(vertex);
        push_back(vertex);
    }
    //------------------------------------------------------------------------
    is_2D(); // if so change format to LASERBOY_2D_FRAME
    quantity = size();
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::from_ifstream_txt(std::ifstream& in,
                                                      const u_int&   group_type,
                                                      const u_int&   element_type,
                                                      int&           line_number
                                                     )
{   //------------------------------------------------------------------------
    switch(group_type)
    {
        case LASERBOY_COLOR_TABLE:
            format        = LASERBOY_COLOR_TABLE;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_3D_FRAME_RGB:
            format        = LASERBOY_3D_FRAME_RGB;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_2D_FRAME_RGB:
            format        = LASERBOY_2D_FRAME_RGB;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_3D_FRAME_HEX:
            format        = LASERBOY_3D_FRAME_RGB;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_2D_FRAME_HEX:
            format        = LASERBOY_2D_FRAME_RGB;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_3D_FRAME_PALETTE:
            format        = LASERBOY_3D_FRAME;
            palette_index = p_space->palette_index;
            break;
        //----------------------------------------------------------------
        case LASERBOY_2D_FRAME_PALETTE:
            format        = LASERBOY_2D_FRAME;
            palette_index = p_space->palette_index;
            break;
        //----------------------------------------------------------------
        case LASERBOY_3D_FRAME_TABLE:
            format        = LASERBOY_3D_FRAME;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
        //----------------------------------------------------------------
        case LASERBOY_2D_FRAME_TABLE:
            format        = LASERBOY_2D_FRAME;
            palette_index = LASERBOY_TRUE_COLOR;
            break;
    }
    //------------------------------------------------------------------------
    if(    group_type == LASERBOY_3D_FRAME_TABLE
        || group_type == LASERBOY_2D_FRAME_TABLE // we already have r g b
      )
    {
        u_int i;
        LaserBoy_vertex vertex;
        for(i = 0; i < size(); i++)
            if(!at(i).from_ifstream_txt(in, group_type, element_type, line_number))
                break;
        if(i < size()) // too few vertices
            segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
        i = 0;
        while(vertex.from_ifstream_txt(in, group_type, element_type, line_number))
            i++;
        if(i > 0)
            segment_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
    }
    //------------------------------------------------------------------------
    else
    {
        LaserBoy_vertex vertex;
        clear();
        if(palette_index == LASERBOY_TRUE_COLOR)
            while(vertex.from_ifstream_txt(in, group_type, element_type, line_number))
                push_back(vertex);
        else
            while(vertex.from_ifstream_txt(in, group_type, element_type, line_number))
            {
                vertex.r = p_space->palette_picker(palette_index)[vertex.c].r;
                vertex.g = p_space->palette_picker(palette_index)[vertex.c].g;
                vertex.b = p_space->palette_picker(palette_index)[vertex.c].b;
                push_back(vertex);
            }
    }
    //------------------------------------------------------------------------
    quantity = size();
    return segment_error;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::move_selection(LaserBoy_3D_double d)
{
    if(size() > 1 && size_of_selection())
    {
        u_int           i;
        LaserBoy_Bounds out_of_bounds = LASERBOY_IN_BOUNDS;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if(    is_index_selected(i)
                    && (out_of_bounds = LaserBoy_bounds_check(at(i).as_3D_double() + d, LASERBOY_CUBE))
                  )
                    return out_of_bounds;
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                    at(i) = at(i).as_3D_double() + d;
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = 0; i < rs.size(); i++)
            if(is_index_selected(i))
                rs.at(i) += d;
        rs.clip();
        if(   (rs.size())
            > (size() - size_of_selection())
          )
            spider = spider - (size() - rs.size());
        else
            egg = spider = 0;
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::scale_selection(LaserBoy_3D_double m)
{
    if(size() > 1 && size_of_selection())
    {
        u_int              i;
        LaserBoy_Bounds    out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short  origin;
        LaserBoy_3D_double center = selected_segment().centroid_of_coordinates();
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                {
                    if(    is_index_selected(i)
                        && (out_of_bounds |= LaserBoy_bounds_check((m * (at(i).as_3D_double() - center)) + center, LASERBOY_CUBE))
                      )
                        return out_of_bounds;
                }
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                    at(i) = (m * (at(i).as_3D_double() - center)) + center;
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = 0; i < rs.size(); i++)
            if(is_index_selected(i))
                rs.at(i) = (m * (rs.at(i) - center)) + center;
        rs.clip();
        if(   (rs.size())
            > (size() - size_of_selection())
          )
            spider = spider - (size() - rs.size());
        else
            egg = spider = 0;
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::scale_selection_on_fulcrum(LaserBoy_3D_double magnitude)
{
    if(size() > 1 && size_of_selection())
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short   origin;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if(    is_index_selected(i)
                    && (out_of_bounds |= LaserBoy_bounds_check((magnitude * (at(i).as_3D_double() - p_space->fulcrum)) + p_space->fulcrum, LASERBOY_CUBE))
                  )
                    return out_of_bounds;
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                    at(i) = (magnitude * (at(i).as_3D_double() - p_space->fulcrum)) + p_space->fulcrum;
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = 0; i < rs.size(); i++)
            if(is_index_selected(i))
                rs.at(i) = (magnitude * (rs.at(i) - p_space->fulcrum)) + p_space->fulcrum;
        rs.clip();
        if(   (rs.size())
            > (size() - size_of_selection())
          )
            spider = spider - (size() - rs.size());
        else
            egg = spider = 0;
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::rotate_selection(LaserBoy_3D_double a)
{
    if(size() > 1 && size_of_selection())
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_short   origin;
        LaserBoy_3D_double  center = selected_segment().centroid_of_coordinates();
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if(    is_index_selected(i)
                    && (out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i).as_3D_double(), center, a), LASERBOY_CUBE))
                  )
                    return out_of_bounds;
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                    at(i) = rotate_vertex_on_coordinates(at(i).as_3D_double(), center, a);
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = 0; i < rs.size(); i++)
            if(is_index_selected(i))
                rs.at(i) = rotate_vertex_on_coordinates(rs.at(i), center, a);
        rs.clip();
        if(   (rs.size())
            > (size() - size_of_selection())
          )
            spider = spider - (size() - rs.size());
        else
            egg = spider = 0;
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
LaserBoy_Bounds LaserBoy_frame::rotate_selection_on_fulcrum(LaserBoy_3D_double a)
{
    if(size() > 1 && size_of_selection())
    {
        u_int               i;
        LaserBoy_Bounds     out_of_bounds = LASERBOY_IN_BOUNDS;
        LaserBoy_3D_double  _d;
        if(!p_space->destructive_clipping)
        {
            for(i = 0; i < size(); i++)
                if(    is_index_selected(i)
                    && (out_of_bounds |= LaserBoy_bounds_check(rotate_vertex_on_coordinates(at(i).as_3D_double(), p_space->fulcrum, a), LASERBOY_CUBE))
                  )
                    return out_of_bounds;
            for(i = 0; i < size(); i++)
                if(is_index_selected(i))
                    at(i) = rotate_vertex_on_coordinates(at(i).as_3D_double(), p_space->fulcrum, a);
            return LASERBOY_IN_BOUNDS;
        }
        LaserBoy_real_segment rs = to_real_segment(false);
        for(i = 0; i < rs.size(); i++)
            if(is_index_selected(i))
                rs.at(i) = rotate_vertex_on_coordinates(rs.at(i), p_space->fulcrum, a);
        rs.clip();
        if(   (rs.size())
            > (size() - size_of_selection())
          )
            spider = spider - (size() - rs.size());
        else
            egg = spider = 0;
        *this = rs;
        return LASERBOY_IN_BOUNDS;
    }
    return LASERBOY_IN_BOUNDS;
}

//############################################################################
void LaserBoy_frame::next_segment_select()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    if(spider < end)
        find_segment_at_index(segment_index, egg, spider);
    else
        find_segment_at_index(segment_index + 1, egg, spider);
    return;
}

//############################################################################
void LaserBoy_frame::next_segment_egg()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    find_segment_at_index(segment_index + 1, egg, end);
    return;
}

//############################################################################
void LaserBoy_frame::next_segment_spider()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(spider, start, end, segment_index);
    find_segment_at_index(segment_index + 1, start, spider);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_select()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, egg, spider);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_egg()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(egg, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, egg, end);
    return;
}

//############################################################################
void LaserBoy_frame::previous_segment_spider()
{
    size_t segment_index, start, end;
    find_segment_of_vertex(spider, start, end, segment_index);
    if(segment_index == 0)
        segment_index = number_of_segments();
    find_segment_at_index(segment_index - 1, start, spider);
    return;
}

//############################################################################
void LaserBoy_frame::color_select()
{
    if(size() > 1)
    {
        u_int i;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //----------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_index_select()
{
    if(size() > 1)
    {
        u_int i,
              num_colors,
              j = 0;

        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            num_colors = p_space->palette_picker(palette_index).last;
            num_colors++; // you can't do modulus with ZERO!
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[j % num_colors].r;
                    at(i).g = p_space->palette_picker(palette_index)[j % num_colors].g;
                    at(i).b = p_space->palette_picker(palette_index)[j % num_colors].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
        else
        {
            num_colors = p_space->palette_picker(p_space->target_palette_index).last;
            num_colors++; // you can't do modulus with ZERO!
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[j % num_colors].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_span_select()
{
    if(size() > 1)
    {
        u_int i,
              j = 0,
              selected = 0,
              span     = egg,
              offset   = spider;
        //----------------------------------------------------------------
        for(i = span; i <= offset; i++)
            if(at(i).is_color(p_space->black_level))
                selected++;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
        else
        {
            span   =    p_space->palette_picker(p_space->target_palette_index).last
                      - p_space->palette_picker(p_space->target_palette_index).first,
            offset =    p_space->palette_picker(p_space->target_palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[at(i).c].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_index_black()
{
    if(size() > 1)
    {
        u_int i,
              j = 0;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].r;
                    at(i).g = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].g;
                    at(i).b = p_space->palette_picker(palette_index)[j % p_space->palette_picker(palette_index).last].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[j % p_space->palette_picker(p_space->target_palette_index).last].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_span_black()
{
    if(size() > 1)
    {
        u_int i,
              j = 0,
              selected = 0,
              span     = egg,
              offset   = spider;
        //----------------------------------------------------------------
        for(i = span; i <= offset; i++)
            if(at(i).is_black(p_space->black_level))
                selected++;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
        else
        {
            span   =    p_space->palette_picker(p_space->target_palette_index).last
                      - p_space->palette_picker(p_space->target_palette_index).first,
            offset =    p_space->palette_picker(p_space->target_palette_index).first;
            for(i = 0; i < size(); i++)
                if(    at(i).is_black(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (u_char)  (   j
                                          * p_space->recolor_span_factor
                                          / selected
                                          * span
                                        )
                                      % span
                                      + offset;
                    at(i).r = p_space->palette_picker(p_space->target_palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(p_space->target_palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(p_space->target_palette_index)[at(i).c].b;
                    j++;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_black_in_select()
{
    if(size() > 1)
    {
        u_int i;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < size(); i++)
                if(is_index_selected(i) && at(i).is_black(p_space->black_level))
                {
                    at(i).c = p_space->selected_color_index;
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //----------------------------------------------------------------
        else
        {
            for(i = 0; i < size(); i++)
                if(is_index_selected(i) && at(i).is_black(p_space->black_level))
                {
                    at(i).r = p_space->palette_picker(palette_index)[p_space->selected_color_index].r;
                    at(i).g = p_space->palette_picker(palette_index)[p_space->selected_color_index].g;
                    at(i).b = p_space->palette_picker(palette_index)[p_space->selected_color_index].b;
                }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::color_cycle_select(int steps)
{
    if(size() > 1)
    {
        u_int i,
              span,
              offset;
        //----------------------------------------------------------------
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            span   =    p_space->palette_picker(palette_index).last
                      - p_space->palette_picker(palette_index).first,
            offset =    p_space->palette_picker(palette_index).first;
            while(span > 0 && steps < 0)
                steps += span;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    at(i).c = (at(i).c - offset + steps) % span + offset;
                    at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                    at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                    at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
                }
        } // end if(palette_index != LASERBOY_TRUE_COLOR)
        //----------------------------------------------------------------
        else // palette_index == LASERBOY_TRUE_COLOR
        {
            LaserBoy_segment selected_vectors;
            vector<int>      selected_vectors_index;
            selected_vectors.palette_index = LASERBOY_TRUE_COLOR;
            for(i = 0; i < size(); i++)
                if(    at(i).is_color(p_space->black_level)
                    && is_index_selected(i)
                  )
                {
                    selected_vectors.push_back(at(i));
                    selected_vectors_index.push_back(i);
                }
            while(selected_vectors.size() > 0 && steps < 0)
                steps += selected_vectors.size();
            for(i = 0; i < selected_vectors.size(); i++)
            {
                at(selected_vectors_index[i]).r = selected_vectors[(i + steps) % selected_vectors.size()].r;
                at(selected_vectors_index[i]).g = selected_vectors[(i + steps) % selected_vectors.size()].g;
                at(selected_vectors_index[i]).b = selected_vectors[(i + steps) % selected_vectors.size()].b;
            }
        }
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::blank_vertices()
{
    if(size() > 1)
    {
        vector <u_int> selected_indecies;
        u_int i;
        for(i = 1; i < size(); i++)
            if(is_index_selected(i))
                selected_indecies.push_back(i);
        if(selected_indecies.size() == 1)
            at(selected_indecies[0]).blank();
        else if(selected_indecies.size() > 1)
        {
            if(selected_indecies[0] == 1)
                at(selected_indecies[0]).blank();
            for(i = 1; i < selected_indecies.size(); i++)
                at(selected_indecies[i]).blank();
        }
    }
    return;
}

//############################################################################
void LaserBoy_frame::unblank_vertices()
{
    if(size() > 1)
    {
        vector <u_int> selected_indecies;
        u_int i;
        for(i = 1; i < size(); i++)
            if(is_index_selected(i))
                selected_indecies.push_back(i);
        if(selected_indecies.size() == 1)
            at(selected_indecies[0]).unblank();
        else if(selected_indecies.size() > 1)
        {
            if(selected_indecies[0] == 1)
                at(selected_indecies[0]).unblank();
            for(i = 1; i < selected_indecies.size(); i++)
                at(selected_indecies[i]).unblank();
        }
    }
    return;
}

//############################################################################
void LaserBoy_frame::black_vertices()
{
    if(size() > 1)
    {
        vector <u_int> selected_indecies;
        u_int i;
        for(i = 1; i < size(); i++)
            if(is_index_selected(i))
                selected_indecies.push_back(i);
        if(selected_indecies.size() == 1 && at(selected_indecies[0]).is_blank())
        {
            at(selected_indecies[0]).r = 0x00;
            at(selected_indecies[0]).g = 0x00;
            at(selected_indecies[0]).b = 0x00;
            at(selected_indecies[0]).c = p_space->palette_picker(palette_index).black;
            at(selected_indecies[0]).unblank();
        }
        else if(selected_indecies.size() > 1)
            for(i = 1; i < selected_indecies.size(); i++)
                if(at(selected_indecies[i]).is_blank())
                {
                    at(selected_indecies[i]).r = 0x00;
                    at(selected_indecies[i]).g = 0x00;
                    at(selected_indecies[i]).b = 0x00;
                    at(selected_indecies[i]).c = p_space->palette_picker(palette_index).black;
                    at(selected_indecies[i]).unblank();
                }
    }
    return;
}

//############################################################################
void LaserBoy_frame::unblack_vertices()
{
    if(size() > 1)
    {
        vector <u_int> selected_indecies;
        u_int i;
        for(i = 1; i < size(); i++)
            if(is_index_selected(i))
                selected_indecies.push_back(i);
        if(selected_indecies.size() == 1 && at(selected_indecies[0]).color_of(p_space->black_level) == 0x00) // lit black
        {
            at(selected_indecies[0]).r = 0xff;
            at(selected_indecies[0]).g = 0xff;
            at(selected_indecies[0]).b = 0xff;
            at(selected_indecies[0]).c = p_space->palette_picker(palette_index).white;
            at(selected_indecies[0]).blank();
        }
        else if(selected_indecies.size() > 1)
            for(i = 1; i < selected_indecies.size(); i++)
                if(at(selected_indecies[i]).color_of(p_space->black_level) == 0x00) // lit black
                {
                    at(selected_indecies[i]).r = 0xff;
                    at(selected_indecies[i]).g = 0xff;
                    at(selected_indecies[i]).b = 0xff;
                    at(selected_indecies[i]).c = p_space->palette_picker(palette_index).white;
                    at(selected_indecies[i]).blank();
                }
    }
    return;
}

//############################################################################
void LaserBoy_frame::minimize(int frame_index)
{
    LaserBoy_segment entry;
    //------------------------------------------------------------------------
    if(frame_index <= 0)
    {
        entry.push_back(LaserBoy_vertex());
        entry.push_back(LaserBoy_vertex()); // the orginal vector
    }
    else
        entry = p_space->frame_picker(frame_index - 1);
    //------------------------------------------------------------------------
    if(entry.size() < 2)
    {
        entry.push_back(LaserBoy_vertex());
        entry.push_back(LaserBoy_vertex()); // the orginal vector
    }
    //------------------------------------------------------------------------
    intro.clear();
    bridge.clear();
    coda.clear();
    //------------------------------------------------------------------------
    if(!p_space->allow_lit_black)
        convert_black_to_blank();
    //------------------------------------------------------------------------
    remove_dwell_vertices();
    reduce_blank_vectors ();
    remove_short_vectors ();
    reduce_lit_vectors   ();
    //------------------------------------------------------------------------
    if(p_space->no_equivalent_vectors)
    {
        omit_equivalent_vectors ();
        reduce_blank_vectors    ();
    }
    //------------------------------------------------------------------------
    if(p_space->dots_setting == LASERBOY_DOTS_REMOVE)
        remove_dots();
    //------------------------------------------------------------------------
    if(p_space->conglomerate_in_minimize)
        conglomerate_lit_segments();
    //------------------------------------------------------------------------
    reorder_segments(entry);
    bond_segments    ();
    normalize_cursors();
    //------------------------------------------------------------------------
    quantity = size();
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::add_vertex(u_int vertex_index)
{
    if(    !LaserBoy_segment::add_vertex(vertex_index)
        && size() > 2
        && vertex_index < size()
      )
    {
        if((at(vertex_index) | at(vertex_index + 1)) >= p_space->insignificant_distance)
            spider = vertex_index + 1;
        else
            spider = vertex_index;
    }
    else
        egg = spider = vertex_index = 0;
    quantity = size();
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::break_segment()
{
    LaserBoy_segment::break_segment(spider); // sets spider
    quantity = size();
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::set_spider_to_egg()
{
    at(spider) = (LaserBoy_3D_short)at(egg);
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::set_egg_to_spider()
{
    at(egg) = (LaserBoy_3D_short)at(spider);
    return segment_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_frame::connect_the_dots()
{
    LaserBoy_segment::connect_the_dots(egg, spider);
    quantity = size();
    return segment_error;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::to_dots()
{
    if(size() > 1)
    {
        u_int           i;
        LaserBoy_frame  frame(palette_index, false),
                        copy(*this);
        //----------------------------------------------------------------
        copy.reduce_blank_vectors();
        frame.reserve(copy.size() + 1);
        frame.push_back(copy.at(0));
        frame.push_back(copy.at(1));
        frame.back() = copy.at(0).as_3D_short();
        //----------------------------------------------------------------
        for(i = 1; i < copy.size(); i++)
        {
            if(    (copy.at(i).is_lit() && copy.at_index_has_magnitude(i))
                || copy.at(i - 1).is_blank()
              )
            {
                frame += copy.at(i);
                frame.back().r = 0;
                frame.back().g = 0;
                frame.back().b = 0;
                frame.back().c = p_space->palette_picker(palette_index).black;
            }
            frame += copy.at(i);
        }
        //----------------------------------------------------------------
        if(at(0).as_3D_short() == frame.back().as_3D_short())
            frame.pop_back();
        *this    = frame;
        quantity = size();
        //----------------------------------------------------------------
    }
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::selected_to_dots()
{
    u_int           i,
                    new_spider = spider;
    LaserBoy_frame  frame(palette_index, false);
    //------------------------------------------------------------------------
    frame.reserve(size() + size_of_selection());
    //------------------------------------------------------------------------
    frame.push_back(front());
    for(i = 1; i < size(); i++)
    {
        if(    is_index_selected(i)
            && (    (at(i).is_lit() && at_index_has_magnitude(i))
                 || at(i).is_blank()
               )
          )
        {
            frame += at(i);
            frame.back().r = 0;
            frame.back().g = 0;
            frame.back().b = 0;
            frame.back().c = p_space->palette_picker(palette_index).black;
            new_spider++;
        }
        frame += at(i);
    }
    //------------------------------------------------------------------------
    spider   = new_spider;
    *this    = frame;
    quantity = size();
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::remove_at_spider()
{
    int new_spider = spider;
    LaserBoy_segment::remove_vertex(spider);
    spider = new_spider - 1;
    normalize_cursors();
    quantity = size();
    return *this;
}

//############################################################################
LaserBoy_frame& LaserBoy_frame::remove_vertex(u_int vertex_index)
{
    LaserBoy_segment::remove_vertex(vertex_index);
    normalize_cursors();
    quantity = size();
    return *this;
}

//############################################################################
void LaserBoy_frame::to_ofstream_ild(std::ofstream& out, u_short& id_num, LaserBoy_ild_header_count& counter) // only called from LaserBoy_frame_set
{
    if(size() > 1)
    {
        quantity = (u_short)size();
        owner    = "LaserBoy";
        //--------------------------------------------------------------------
        is_2D();
        if(p_space->save_2D_as_3D)
            format = LASERBOY_3D_FRAME;
        //--------------------------------------------------------------------
        if(name.size() == 0 || name == "ESI.....")
        {
            char number[12];
            sprintf(number, "%05d", id_num);
            name = "ESI" + string(number);
        }
        //--------------------------------------------------------------------
        if(p_space->save_ild_fmt_4_5)
        {
            if(format == LASERBOY_3D_FRAME) format = LASERBOY_3D_FRAME_RGB;
            if(format == LASERBOY_2D_FRAME) format = LASERBOY_2D_FRAME_RGB;
        }
        //--------------------------------------------------------------------
        else if(palette_index == LASERBOY_TRUE_COLOR)
        {
            LaserBoy_ild_header header = (LaserBoy_ild_header)*this;
            header.format = LASERBOY_COLOR_TABLE;
            header.identity = id_num++;
            header.to_ofstream_ild(out);
            as_color_table().to_ofstream_ild_fmt3(out);
            counter._3++;
        }
        //--------------------------------------------------------------------
        identity = id_num++;
        LaserBoy_ild_header::to_ofstream_ild(out); // *this
        for(size_t i = 0; i < size() - 1; i++)
            at(i).to_ofstream_ild(out, format, false);
        back().to_ofstream_ild(out, format, true); // last vertex
        switch(format)
        {
            case 0:
                counter._0++;
                break;
            case 1:
                counter._1++;
                break;
            case 4:
                counter._4++;
                break;
            case 5:
                counter._5++;
                break;
            default:
                break;
        }
        //--------------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_frame::to_ofstream_ctn(std::ofstream& out)
{
    u_int           i,
                    j;
    LaserBoy_frame  copy(*this),
                    frame;
    if(copy.palette_index != LASERBOY_ILDA_DEFAULT)
        copy.best_match_palette(LASERBOY_ILDA_DEFAULT);
    frame.clear();
    if(copy.size() > 1)
    {
        if(p_space->auto_opti_ctn)
        {
            copy.flatten_z();
            copy.minimize(0);
            copy.optimize(0);
            //-----------------------------------------------------------------
            for(j = 0; j < p_space->dwell_on_dot; j++)
                frame.push_back(copy.at(0)); // copy the origin vector
            //-----------------------------------------------------------------
            for(i = 1; i < copy.size(); i++)
            {
                // if this vertex is lit and the previous one is not
                if(    copy.at(i - 1).is_blank() // look at the previous vertex
                    && copy.at(i).is_lit()
                  )
                {
                    for(j = 0; j < p_space->dwell_on_dot; j++)
                        frame += copy.at(i - 1);
                }
                //-------------------------------------------------------------
                // if this vertex is blank and the previous one is not
                else if(    copy.at(i - 1).is_lit() // look at the previous vertex
                         && copy.at(i).is_blank()
                       )
                {
                    for(j = 0; j < p_space->dwell_on_dot; j++)
                        frame += copy.at(i - 1);
                }
                //-------------------------------------------------------------
                frame += copy.at(i);
            } // end for(i = 1; i < (int)copy.size(); i++)
            //-----------------------------------------------------------------
            for(j = 0; j < p_space->dwell_on_dot; j++)
                frame += copy.back();
            //-----------------------------------------------------------------
        } // end if(p_space->auto_opto_ctn)
        else
            frame = copy;
    }
    //------------------------------------------------------------------------
    frame.quantity = (u_short)frame.size();
    frame.LaserBoy_ild_header::to_ofstream_ctn(out);
    //------------------------------------------------------------------------
    for(size_t i = 0; i < frame.size() - 1; i++)
        frame.at(i).to_ofstream_ild(out, LASERBOY_3D_FRAME, false);
    frame.back().to_ofstream_ild(out, LASERBOY_3D_FRAME, true);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_frame::to_ofstream_txt_color_table(std::ofstream& out)
{
    out << "# ---------------------------------------------------------------"
        << ENDL
        << "table ";
    if(p_space->save_txt_color_hex)
        out << "hex";
    else
        out << "rgb";
    out << ENDL;
    //------------------------------------------------------------------------
    for(size_t i = 0; i < size(); i++)
        if(at(i).is_lit())
            ((LaserBoy_color)at(i)).to_ofstream_txt(out, p_space->save_txt_color_hex);
        else
            out << "    -1"
                << ENDL;
    //------------------------------------------------------------------------
    out << ENDL;
    return;
}

//############################################################################
void LaserBoy_frame::to_ofstream_txt(std::ofstream& out, int frame_index)
{
    int output_format = (    (is_2D())
                           ? (   (p_space->save_txt_with_color)
                               ? (LASERBOY_2D_FRAME_RGB)
                               : (   (palette_index == LASERBOY_TRUE_COLOR)
                                   ? (LASERBOY_2D_FRAME_TABLE)
                                   : (LASERBOY_2D_FRAME_PALETTE)
                                 )
                             ) // 3D
                           : (   (p_space->save_txt_with_color)
                               ? (LASERBOY_3D_FRAME_RGB)
                               : (   (palette_index == LASERBOY_TRUE_COLOR)
                                   ? (LASERBOY_3D_FRAME_TABLE)
                                   : (LASERBOY_3D_FRAME_PALETTE)
                                 )
                             )
                        );
    //------------------------------------------------------------------------
    if(frame_index >= 0)
        out << "# " << setw(6) << frame_index << " ---------------------------------------------------------" << ENDL;
    out << "frame ";
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_2D_FRAME_PALETTE:
            out << "xy ";
            break;
        case LASERBOY_3D_FRAME_RGB:
        case LASERBOY_3D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_PALETTE:
            out << "xyz ";
            break;
    }
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_3D_FRAME_RGB:
            if(p_space->save_txt_color_hex)
                out << "hex";
            else
                out << "rgb";
            break;
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_TABLE:
            out << "table";
            break;
        case LASERBOY_2D_FRAME_PALETTE:
        case LASERBOY_3D_FRAME_PALETTE:
            out << "palette";
            break;
    }
    //------------------------------------------------------------------------
    if(p_space->save_txt_unit)
    {
        out << " unit" << ENDL;
        switch(output_format)
        {
            case LASERBOY_3D_FRAME_PALETTE:
                for(size_t i = 0; i < size(); i++)
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                        << noshowpos
                        << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_PALETTE:
                for(size_t i = 0; i < size(); i++)
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << noshowpos
                        << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_TABLE: // nothing but x y z
                for(size_t i = 0; i < size(); i++)
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                        << noshowpos
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_TABLE: // nothing but x y
                for(size_t i = 0; i < size(); i++)
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << noshowpos
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_RGB:
                for(size_t i = 0; i < size(); i++)
                {
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).z / (double)LASERBOY_MAX_SHORT
                        << noshowpos;
                    if(p_space->save_txt_color_hex)
                    {
                        if(at(i).is_lit())
                            out << "    0x"
                                << hex
                                << setw(6)
                                << right
                                << setfill('0')
                                << at(i).as_uint()
                                << dec
                                << left
                                << setfill(' ');
                        else
                            out << "   -1";
                    }
                    else
                    {
                        if(at(i).is_lit())
                            out << setw(7) << (int)at(i).r
                                << setw(7) << (int)at(i).g
                                << setw(7) << (int)at(i).b;
                        else
                            out << "   -1";
                    }
                    out << ENDL;
                }
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_RGB:
                for(size_t i = 0; i < size(); i++)
                {
                    out << "    "
                        << showpos
                        << setw(15) << left << at(i).x / (double)LASERBOY_MAX_SHORT
                        << setw(15) << left << at(i).y / (double)LASERBOY_MAX_SHORT
                        << noshowpos;
                    if(p_space->save_txt_color_hex)
                    {
                        if(at(i).is_lit())
                            out << "    0x"
                                << hex
                                << setw(6)
                                << right
                                << setfill('0')
                                << at(i).as_uint()
                                << dec
                                << left
                                << setfill(' ');
                        else
                            out << "   -1";
                    }
                    else
                    {
                        if(at(i).is_lit())
                            out << setw(7) << (int)at(i).r
                                << setw(7) << (int)at(i).g
                                << setw(7) << (int)at(i).b;
                        else
                            out << "   -1";
                    }
                    out << ENDL;
                }
                break;
        }   //----------------------------------------------------------------
    }
    else
    {
        out << " short" << ENDL;
        switch(output_format)
        {
            case LASERBOY_3D_FRAME_PALETTE:
                for(size_t i = 0; i < size(); i++)
                    out << setw(9) << at(i).x
                        << setw(9) << at(i).y
                        << setw(9) << at(i).z
                        << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_PALETTE:
                for(size_t i = 0; i < size(); i++)
                    out << setw(9) << at(i).x
                        << setw(9) << at(i).y
                        << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_TABLE:
                for(size_t i = 0; i < size(); i++)
                    out << setw(9) << at(i).x
                        << setw(9) << at(i).y
                        << setw(9) << at(i).z
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_TABLE:
                for(size_t i = 0; i < size(); i++)
                    out << setw(9) << at(i).x
                        << setw(9) << at(i).y
                        << ENDL;
                break;
            //----------------------------------------------------------------
            case LASERBOY_3D_FRAME_RGB:
                for(size_t i = 0; i < size(); i++)
                {
                    out << dec
                        << setw(9) << at(i).x
                        << setw(9) << at(i).y
                        << setw(9) << at(i).z;
                    if(p_space->save_txt_color_hex)
                    {
                        if(at(i).is_lit())
                            out << "    0x"
                                << hex
                                << setw(6)
                                << right
                                << setfill('0')
                                << at(i).as_uint()
                                << dec
                                << left
                                << setfill(' ');
                        else
                            out << "   -1";
                    }
                    else
                    {
                        if(at(i).is_lit())
                            out << setw(7) << (int)at(i).r
                                << setw(7) << (int)at(i).g
                                << setw(7) << (int)at(i).b;
                        else
                            out << "   -1";
                    }
                    out << ENDL;
                }
                break;
            //----------------------------------------------------------------
            case LASERBOY_2D_FRAME_RGB:
                for(size_t i = 0; i < size(); i++)
                {
                    out << dec
                        << setw(9) << at(i).x
                        << setw(9) << at(i).y;
                    if(p_space->save_txt_color_hex)
                    {
                        if(at(i).is_lit())
                            out << "    0x"
                                << hex
                                << setw(6)
                                << right
                                << setfill('0')
                                << at(i).as_uint()
                                << dec
                                << left
                                << setfill(' ');
                        else
                            out << "   -1";
                    }
                    else
                    {
                        if(at(i).is_lit())
                            out << setw(7) << (int)at(i).r
                                << setw(7) << (int)at(i).g
                                << setw(7) << (int)at(i).b;
                        else
                            out << "   -1";
                    }
                    out << ENDL;
                }
                break;
        }   //----------------------------------------------------------------
    }
    out << ENDL;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_frame::render(int skin, struct LaserBoy_bmp *bmp) const
{
    LaserBoy_3D_double _0,
                       _1;
    LaserBoy_color     lb_color;
    bool     rotate_view = (p_space->view_angle  != 0.0),
             offset_view = (p_space->view_offset != 0  ),
             scale_view  = (p_space->view_scale  != 1.0);
    u_int    i,
             bmp_color,
             bmp_blank    = bmp->make_rgb(p_space->rendered_blank.r,
                                          p_space->rendered_blank.g,
                                          p_space->rendered_blank.b
                                         ),
             bmp_black    = bmp->make_rgb(p_space->rendered_black.r,
                                          p_space->rendered_black.g,
                                          p_space->rendered_black.b
                                         ),
             bmp_white    = bmp->white,
             bmp_bg_color = bmp->make_rgb(p_space->bg_color.r,
                                          p_space->bg_color.g,
                                          p_space->bg_color.b
                                         );
    int      xx,
             yy,
             bmp_center = (   (p_space->show_quad_view)
                            ? (bmp->yres / 4)
                            : (bmp->yres / 2)
                          ),
             bmp_3X_center = bmp_center * 3,
             bigger =   (   ((p_space->show_bridge) ? (bridge.size()) : (0))
                          > ((p_space->show_coda)   ? (coda.size())   : (0))
                        )
                      ? ((p_space->show_bridge) ? (bridge.size()) : (0))
                      : ((p_space->show_coda)   ? (coda.size())   : (0));
    double   fade,
             deep = LASERBOY_SHORT_SPAN,
             half_deep = deep / 2.0,
             z_depth_scale =   deep
                             / (   size()
                                 + ((p_space->show_intro)  ? (intro.size())  : (0))
                                 + bigger
                               ),
             z_depth_body_offset =   half_deep
                                   - (   z_depth_scale
                                       * ((p_space->show_intro) ? (intro.size()) : (0))
                                     ),
             z_depth_coda_offset =   half_deep
                                   - (   (z_depth_scale * ((p_space->show_intro) ? (intro.size()) : (0)))
                                       + (z_depth_scale * size())
                                     ),
             scale = (   (p_space->show_quad_view)
                       ? (bmp->yres / (deep * 2.0))
                       : (bmp->yres / deep)
                     );
    //------------------------------------------------------------------------
    if(skin)
    {
        fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
        if(fade > 1.0)
            fade = 1.0;
        lb_color  = p_space->rendered_blank;
        lb_color  = lb_color.blend(bmp_bg_color, fade);
        bmp_blank = bmp->make_rgb(lb_color.r,
                                  lb_color.g,
                                  lb_color.b
                                 );
        lb_color  = p_space->rendered_black;
        lb_color  = lb_color.blend(bmp_bg_color, fade);
        bmp_black = bmp->make_rgb(lb_color.r,
                                  lb_color.g,
                                  lb_color.b
                                 );
        lb_color  = bmp->white;
        lb_color  = lb_color.blend(bmp_bg_color, fade);
        bmp_white = bmp->make_rgb(lb_color.r,
                                  lb_color.g,
                                  lb_color.b
                                 );
    }
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // blank vectors
    //------------------------------------------------------------------------
    if(    p_space->show_blanking
        && (    skin == 0
             || p_space->show_onion_visuals
           )
      )
    {
        //----------------------------------------------------------------
        // blank in intro
        //----------------------------------------------------------------
        if(p_space->show_intro)
        {
            for(i = 1; i < intro.size(); i++)
            {
                if(intro.at(i).is_blank())
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    //--------------------------------------------------------
                    // normal view
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(p_space->show_quad_view)
                    {
                        _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - half_deep;
                            _1.z =  i      * z_depth_scale - half_deep;
                        }
                        //----------------------------------------------------
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                   );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x * scale) + bmp_center,
                                                    bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                   );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.z * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                   );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                    } // end if(p_space->show_quad_view)
                } // end if(intro.at(i).is_blank())
            } // end for(i = 1; i < (int)intro.size(); i++)
        } // end if(p_space->show_intro)
        //----------------------------------------------------------------
        // blank in frame
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_blank())
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                    //--------------------------------------------------------
                // normal view
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_blank,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_blank,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_blank,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_blank,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                        _1.z =  i      * z_depth_scale - z_depth_body_offset;
                    }
                    //--------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(    p_space->show_cursors
                        && is_index_selected(i)
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_select(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_indices
                        && (skin == 0)
                      )
                        bmp_printf(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_blank,
                                   bmp_bg_color,
                                   0,
                                   p_space->font_size_factor,
                                   "%d",
                                   i
                                  );
                } // end if(p_space->show_quad_view)
            } // end if(at(i).is_blank())
        } // end for(i = 1; i < size(); i++)
        //----------------------------------------------------------------
        // blank in bridge
        //----------------------------------------------------------------
        if(p_space->show_bridge)
            for(i = 1; i < bridge.size(); i++)
            {
                if(bridge.at(i).is_blank())
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    //--------------------------------------------------------
                    // normal view
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );

                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(p_space->show_quad_view)
                    {
                        _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                            _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                        }
                        //----------------------------------------------------
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                   );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x *  scale) + bmp_center,
                                                    bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                  );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.z * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                  );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                    } // end if(p_space->show_quad_view)
                } // if(bridge.at(i).is_blank())
            } // end for(i = 1; i < (int)bridge.size(); i++)
        //----------------------------------------------------------------
        // blank in coda
        //----------------------------------------------------------------
        if(p_space->show_coda)
            for(i = 1; i < coda.size(); i++)
            {
                if(coda.at(i).is_blank())
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    if(rotate_view)
                    {
                        _0 = rotate_vertex(_0, p_space->view_angle);
                        _1 = rotate_vertex(_1, p_space->view_angle);
                    }
                    if(offset_view)
                    {
                        _0 += p_space->view_offset;
                        _1 += p_space->view_offset;
                    }
                    if(scale_view)
                    {
                        _0 *= p_space->view_scale;
                        _1 *= p_space->view_scale;
                    }
                    xx = (int)(_1.x * scale) + bmp_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    //--------------------------------------------------------
                    // normal view
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_blank,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_blank,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_blank,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    if(p_space->show_quad_view)
                    {
                        _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                        _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                        if(p_space->show_Z_as_order)
                        {
                            _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                            _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                        }
                        //----------------------------------------------------
                        // front
                        xx = (int)(_1.x * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                  );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - z_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // top
                        xx = (int)(_1.x *  scale) + bmp_center;
                        yy = (int)(_1.z * -scale) + bmp_3X_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.x *  scale) + bmp_center,
                                                    bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                  );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - y_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                        //----------------------------------------------------
                        // side
                        xx = (int)(_1.z * scale) + bmp_3X_center;
                        yy = (int)(_1.y * scale) + bmp_center;
                        if(p_space->show_vectors)
                            bmp_put_thick_line_fast(bmp,
                                                                 (int)(_0.z * scale) + bmp_3X_center,
                                                    bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                    xx,
                                                    bmp->yres - yy,
                                                    bmp_blank,
                                                    p_space->rendered_line_width
                                                  );
                        else
                            bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_blank, p_space->rendered_line_width);
                        //----------------------------------------------------
                        if(    p_space->show_vertices
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                            bmp_put_vertex(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           bmp_blank,
                                           p_space->rendered_line_width
                                          );
                        //----------------------------------------------------
                        if(    p_space->show_direction
                            && (    skin == 0
                                 || p_space->show_onion_visuals
                               )
                          )
                        {
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) + (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                            bmp_put_thick_ray_fast(bmp,
                                                   xx,
                                                   bmp->yres - yy,
                                                   700 * scale,
                                                   pi - x_direction(_0, _1) - (pi / 8),
                                                   bmp_blank,
                                                   p_space->rendered_line_width
                                                  );
                        }
                    } // end if(p_space->show_quad_view)
                } // end if(coda.at(i).is_blank())
            } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_blanking)
    //------------------------------------------------------------------------
    // black vectors
    //------------------------------------------------------------------------
    if(p_space->show_intro)
    {
        //----------------------------------------------------------------
        // black in intro
        //----------------------------------------------------------------
        for(i = 1; i < intro.size(); i++)
        {
            if(intro.at(i).is_black(p_space->black_level))
            {
                _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - half_deep;
                    _1.z =  i      * z_depth_scale - half_deep;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                //------------------------------------------------------------
                // normal view
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    //--------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                } // end if(p_space->show_quad_view)
            } // if(intro.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)intro.size(); i++)
    } // end if(p_space->show_intro)
    //------------------------------------------------------------------------
    // black in frame
    //------------------------------------------------------------------------
    for(i = 1; i < size(); i++)
    {
        if(at(i).is_black(p_space->black_level))
        {
            _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                _1.z =  i      * z_depth_scale - z_depth_body_offset;
            }
            if(rotate_view)
            {
                _0 = rotate_vertex(_0, p_space->view_angle);
                _1 = rotate_vertex(_1, p_space->view_angle);
            }
            if(offset_view)
            {
                _0 += p_space->view_offset;
                _1 += p_space->view_offset;
            }
            if(scale_view)
            {
                _0 *= p_space->view_scale;
                _1 *= p_space->view_scale;
            }
            // normal view
            xx = (int)(_1.x * scale) + bmp_center;
            yy = (int)(_1.y * scale) + bmp_center;
            //----------------------------------------------------------------
            if(p_space->show_vectors)
                bmp_put_thick_line_fast(bmp,
                                                     (int)(_0.x * scale) + bmp_center,
                                        bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                        xx,
                                        bmp->yres - yy,
                                        bmp_black,
                                        p_space->rendered_line_width
                                       );
            else
                bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
            if(    p_space->show_vertices
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_vertex(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_black,
                               p_space->rendered_line_width
                              );
            //----------------------------------------------------------------
            if(    p_space->show_direction
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
            {
                bmp_put_thick_ray_fast(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       700 * scale,
                                       pi - z_direction(_0, _1) + (pi / 8),
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                bmp_put_thick_ray_fast(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       700 * scale,
                                       pi - z_direction(_0, _1) - (pi / 8),
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
            }
            //----------------------------------------------------------------
            if(    p_space->show_cursors
                && is_index_selected(i)
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_select(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_white,
                               p_space->rendered_line_width
                              );
            //----------------------------------------------------------------
            if(    p_space->show_indices
                && (skin == 0)
              )
                bmp_printf(bmp,
                           xx,
                           bmp->yres - yy,
                           bmp_white,
                           bmp_bg_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           i
                          );
            //----------------------------------------------------------------
            if(p_space->show_quad_view)
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                // front
                xx = (int)(_1.x * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_3X_center;
                bmp_color = bmp->make_rgb(p_space->rendered_black.r,
                                          p_space->rendered_black.g,
                                          p_space->rendered_black.b
                                         );
                //----------------------------------------------------------------
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_3X_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //----------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //----------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                //----------------------------------------------------------------
                // top
                xx = (int)(_1.x *  scale) + bmp_center;
                yy = (int)(_1.z * -scale) + bmp_3X_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x *  scale) + bmp_center,
                                            bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //----------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - y_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - y_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //----------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                //----------------------------------------------------------------
                // side
                xx = (int)(_1.z * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.z * scale) + bmp_3X_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //----------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - x_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - x_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //----------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_white,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
            }
        } // if(at(i).is_black(p_space->black_level))
    } // end for(i = 1; i < size(); i++)
    //------------------------------------------------------------------------
    // black in bridge
    //------------------------------------------------------------------------
    if(p_space->show_bridge)
    {
        for(i = 1; i < bridge.size(); i++)
        {
            if(bridge.at(i).is_black(p_space->black_level))
            {
                _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //------------------------------------------------------------
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    //------------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //------------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //------------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                } // end if(p_space->show_quad_view)
            } // if(bridge.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)bridge.size(); i++)
    } // end if(p_space->show_bridge)
    //------------------------------------------------------------------------
    // black in coda
    //------------------------------------------------------------------------
    if(p_space->show_coda)
    {
        for(i = 1; i < coda.size(); i++)
        {
            if(coda.at(i).is_black(p_space->black_level))
            {
                _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //----------------------------------------------------------------
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_black,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                //----------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_black,
                                   p_space->rendered_line_width
                                  );
                //----------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_black,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    //------------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //------------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                    //------------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_black,
                                                p_space->rendered_line_width
                                               );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_black, p_space->rendered_line_width);
                    //------------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_black,
                                       p_space->rendered_line_width
                                      );
                    //------------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_black,
                                               p_space->rendered_line_width
                                              );
                    }
                } // end if(p_space->show_quad_view)
            } // if(coda.at(i).is_black(p_space->black_level))
        } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_coda)
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // color vectors
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // color in intro
    //------------------------------------------------------------------------
    if(p_space->show_intro)
    {
        for(i = 1; i < intro.size(); i++)
        {
            if(intro.at(i).is_color(p_space->black_level))
            {
                _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - half_deep;
                    _1.z =  i      * z_depth_scale - half_deep;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //------------------------------------------------------------
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;

                bmp_color = bmp->make_rgb(intro.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                          intro.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                          intro.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                          );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = intro.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = bmp->make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = intro.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = intro.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - half_deep;
                        _1.z =  i      * z_depth_scale - half_deep;
                    }
                    //--------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                } // end if(p_space->show_quad_view)
            } // if(intro.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)intro.size(); i++)
    } // end if(p_space->show_intro)
    //------------------------------------------------------------------------
    // color in frame
    //------------------------------------------------------------------------
    for(i = 1; i < size(); i++)
    {
        if(at(i).is_color(p_space->black_level))
        {
            _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
            _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
            if(p_space->show_Z_as_order)
            {
                _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                _1.z =  i      * z_depth_scale - z_depth_body_offset;
            }
            if(rotate_view)
            {
                _0 = rotate_vertex(_0, p_space->view_angle);
                _1 = rotate_vertex(_1, p_space->view_angle);
            }
            if(offset_view)
            {
                _0 += p_space->view_offset;
                _1 += p_space->view_offset;
            }
            if(scale_view)
            {
                _0 *= p_space->view_scale;
                _1 *= p_space->view_scale;
            }
            //----------------------------------------------------------------
            // normal view
            xx = (int)(_1.x * scale) + bmp_center;
            yy = (int)(_1.y * scale) + bmp_center;
            bmp_color = bmp->make_rgb(at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                      at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                      at(i).bit_masked_color(p_space->signal_bit_mask).b
                                     );
            if(skin)
            {
                fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                if(fade > 1.0)
                    fade = 1.0;
                lb_color = at(i).bit_masked_color(p_space->signal_bit_mask);
                lb_color = lb_color.blend(bmp_bg_color, fade);
                bmp_color = bmp->make_rgb(lb_color.r,
                                          lb_color.g,
                                          lb_color.b
                                         );
            }
            //----------------------------------------------------------------
            if(p_space->show_vectors)
                bmp_put_thick_line_fast(bmp,
                                                     (int)(_0.x * scale) + bmp_center,
                                        bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                        xx,
                                        bmp->yres - yy,
                                        bmp_color,
                                        p_space->rendered_line_width
                                      );
            else
                bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
            //----------------------------------------------------------------
            if(    p_space->show_vertices
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_vertex(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_color,
                               p_space->rendered_line_width
                              );
            //----------------------------------------------------------------
            if(    p_space->show_cursors
                && is_index_selected(i)
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
                bmp_put_select(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_color,
                               p_space->rendered_line_width
                              );
            //----------------------------------------------------------------
            if(    p_space->show_direction
                && (    skin == 0
                     || p_space->show_onion_visuals
                   )
              )
            {
                bmp_put_thick_ray_fast(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       700 * scale,
                                       pi - z_direction(_0, _1) + (pi / 8),
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                bmp_put_thick_ray_fast(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       700 * scale,
                                       pi - z_direction(_0, _1) - (pi / 8),
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
            }
            //----------------------------------------------------------------
            if(    p_space->show_indices
                && (skin == 0)
              )
                bmp_printf(bmp,
                           xx,
                           bmp->yres - yy,
                           bmp_color,
                           bmp_bg_color,
                           0,
                           p_space->font_size_factor,
                           "%d",
                           i
                          );
            //----------------------------------------------------------------
            if(p_space->show_quad_view)
            {
                _0 = at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_body_offset;
                    _1.z =  i      * z_depth_scale - z_depth_body_offset;
                }
                //------------------------------------------------------------
                // front
                xx = (int)(_1.x * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_3X_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_3X_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_color,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                //------------------------------------------------------------
                // top
                xx = (int)(_1.x *  scale) + bmp_center;
                yy = (int)(_1.z * -scale) + bmp_3X_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x *  scale) + bmp_center,
                                            bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - y_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - y_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_color,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
                //------------------------------------------------------------
                // side
                xx = (int)(_1.z * scale) + bmp_3X_center;
                yy = (int)(_1.y * scale) + bmp_center;
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.z * scale) + bmp_3X_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - x_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - x_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(    p_space->show_cursors
                    && is_index_selected(i)
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_select(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_indices
                    && (skin == 0)
                  )
                    bmp_printf(bmp,
                               xx,
                               bmp->yres - yy,
                               bmp_color,
                               bmp_bg_color,
                               0,
                               p_space->font_size_factor,
                               "%d",
                               i
                              );
            } // end if(p_space->show_quad_view)
        } // if(at(i).is_color(p_space->black_level))
    } // end for(i = 1; i < size(); i++)
    //------------------------------------------------------------------------
    // color in bridge
    //------------------------------------------------------------------------
    if(p_space->show_bridge)
    {
        for(i = 1; i < bridge.size(); i++)
        {
            if(bridge.at(i).is_color(p_space->black_level))
            {
                _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //------------------------------------------------------------
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                bmp_color = bmp->make_rgb(bridge.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                                  bridge.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                                  bridge.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                                 );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = bridge.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = bmp->make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = bridge.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = bridge.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    //--------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_3X_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                }
            } // if(bridge.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)bridge.size(); i++)
    } // end if(p_space->show_bridge)
    //------------------------------------------------------------------------
    // color in coda
    //------------------------------------------------------------------------
    if(p_space->show_coda)
    {
        for(i = 1; i < coda.size(); i++)
        {
            if(coda.at(i).is_color(p_space->black_level))
            {
                _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                if(p_space->show_Z_as_order)
                {
                    _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                    _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                }
                if(rotate_view)
                {
                    _0 = rotate_vertex(_0, p_space->view_angle);
                    _1 = rotate_vertex(_1, p_space->view_angle);
                }
                if(offset_view)
                {
                    _0 += p_space->view_offset;
                    _1 += p_space->view_offset;
                }
                if(scale_view)
                {
                    _0 *= p_space->view_scale;
                    _1 *= p_space->view_scale;
                }
                //------------------------------------------------------------
                // normal view
                xx = (int)(_1.x * scale) + bmp_center;
                yy = (int)(_1.y * scale) + bmp_center;
                bmp_color = bmp->make_rgb(coda.at(i).bit_masked_color(p_space->signal_bit_mask).r,
                                          coda.at(i).bit_masked_color(p_space->signal_bit_mask).g,
                                          coda.at(i).bit_masked_color(p_space->signal_bit_mask).b
                                         );
                if(skin)
                {
                    fade = p_space->onion_skin_fade + (skin - 1) * (p_space->onion_skin_fade / 10);
                    if(fade > 1.0)
                        fade = 1.0;
                    lb_color = coda.at(i).bit_masked_color(p_space->signal_bit_mask);
                    lb_color = lb_color.blend(bmp_bg_color, fade);
                    bmp_color = bmp->make_rgb(lb_color.r,
                                                      lb_color.g,
                                                      lb_color.b
                                                     );
                }
                //------------------------------------------------------------
                if(p_space->show_vectors)
                    bmp_put_thick_line_fast(bmp,
                                                         (int)(_0.x * scale) + bmp_center,
                                            bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                            xx,
                                            bmp->yres - yy,
                                            bmp_color,
                                            p_space->rendered_line_width
                                          );
                else
                    bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                //------------------------------------------------------------
                if(    p_space->show_vertices
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                    bmp_put_vertex(bmp,
                                   xx,
                                   bmp->yres - yy,
                                   bmp_color,
                                   p_space->rendered_line_width
                                  );
                //------------------------------------------------------------
                if(    p_space->show_direction
                    && (    skin == 0
                         || p_space->show_onion_visuals
                       )
                  )
                {
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) + (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                    bmp_put_thick_ray_fast(bmp,
                                           xx,
                                           bmp->yres - yy,
                                           700 * scale,
                                           pi - z_direction(_0, _1) - (pi / 8),
                                           bmp_color,
                                           p_space->rendered_line_width
                                          );
                }
                //------------------------------------------------------------
                if(p_space->show_quad_view)
                {
                    _0 = coda.at(i - 1).bit_masked_position(p_space->signal_bit_mask);
                    _1 = coda.at(i    ).bit_masked_position(p_space->signal_bit_mask);
                    if(p_space->show_Z_as_order)
                    {
                        _0.z = (i - 1) * z_depth_scale - z_depth_coda_offset;
                        _1.z =  i      * z_depth_scale - z_depth_coda_offset;
                    }
                    //--------------------------------------------------------
                    // front
                    xx = (int)(_1.x * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - z_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // top
                    xx = (int)(_1.x *  scale) + bmp_center;
                    yy = (int)(_1.z * -scale) + bmp_3X_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.x *  scale) + bmp_center,
                                                bmp->yres - ((int)(_0.z * -scale) + bmp_3X_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - y_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                    //--------------------------------------------------------
                    // side
                    xx = (int)(_1.z * scale) + bmp_3X_center;
                    yy = (int)(_1.y * scale) + bmp_center;
                    if(p_space->show_vectors)
                        bmp_put_thick_line_fast(bmp,
                                                             (int)(_0.z * scale) + bmp_3X_center,
                                                bmp->yres - ((int)(_0.y * scale) + bmp_center),
                                                xx,
                                                bmp->yres - yy,
                                                bmp_color,
                                                p_space->rendered_line_width
                                              );
                    else
                        bmp_put_thick_pixel(bmp, xx, bmp->yres - yy, bmp_color, p_space->rendered_line_width);
                    //--------------------------------------------------------
                    if(    p_space->show_vertices
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                        bmp_put_vertex(bmp,
                                       xx,
                                       bmp->yres - yy,
                                       bmp_color,
                                       p_space->rendered_line_width
                                      );
                    //--------------------------------------------------------
                    if(    p_space->show_direction
                        && (    skin == 0
                             || p_space->show_onion_visuals
                           )
                      )
                    {
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) + (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                        bmp_put_thick_ray_fast(bmp,
                                               xx,
                                               bmp->yres - yy,
                                               700 * scale,
                                               pi - x_direction(_0, _1) - (pi / 8),
                                               bmp_color,
                                               p_space->rendered_line_width
                                              );
                    }
                }
            } // if(coda.at(i).is_color(p_space->black_level))
        } // end for(i = 1; i < (int)coda.size(); i++)
    } // end if(p_space->show_coda)
    //------------------------------------------------------------------------
    // END ACTUAL VECTOR ART !!!
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::save_as_wave(const string& file, bool optimized, bool timed, bool clip)
{
    std::fstream out(file.c_str(), ios::out | ios::binary);
    //------------------------------------------------------------------------
    if(out.is_open() && size() > 1)
    {
        int                   i;
        LaserBoy_frame        copy (*this);
        LaserBoy_wave_header  header(p_space->sample_rate,
                                       LASERBOY_WAVE_POSITIVE
                                     | LASERBOY_WAVE_END_OF_FRAME
                                     | LASERBOY_WAVE_UNIQUE_FRAME
                                     | LASERBOY_WAVE_SIGNAL_MATRIX
                                     | LASERBOY_SIGNAL_BIT_RESOLUTION
                                     | LASERBOY_WAVE_NUM_FRAMES
                                     | p_space->color_rescales_flags(),
                                     p_space->channels_of_wav_out,
                                     LASERBOY_WAVE_VERSION
                                    );
        //----------------------------------------------------------------
        if(p_space->invert_wave_output)
            header.LaserBoy_wave_mode &= ~LASERBOY_WAVE_POSITIVE;
        //----------------------------------------------------------------
        header.signal_id [0] = LASERBOY_SIGNAL_X_POSITION; // default values
        header.signal_id [1] = LASERBOY_SIGNAL_Y_POSITION;
        header.signal_id [2] = LASERBOY_SIGNAL_RED_ANALOG;
        header.signal_id [3] = LASERBOY_SIGNAL_GREEN_ANALOG;
        header.signal_id [4] = LASERBOY_SIGNAL_BLUE_ANALOG;
        header.signal_id [5] = LASERBOY_SIGNAL_Z_POSITION;
        header.signal_id [6] = LASERBOY_SIGNAL_UNDEFINED;
        header.signal_id [7] = LASERBOY_SIGNAL_UNDEFINED;
        //----------------------------------------------------------------
        header.LSB_tag   [0] = LASERBOY_LSB_NOT_USED; // default values
        header.LSB_tag   [1] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [2] = LASERBOY_LSB_END_OF_FRAME;
        header.LSB_tag   [3] = LASERBOY_LSB_UNIQUE_FRAME;
        header.LSB_tag   [4] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [5] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [6] = LASERBOY_LSB_NOT_USED;
        header.LSB_tag   [7] = LASERBOY_LSB_NOT_USED;
        //----------------------------------------------------------------
        header.resolution[0] = 16 - p_space->signal_bit_mask[0];
        header.resolution[1] = 16 - p_space->signal_bit_mask[1];
        header.resolution[2] = 16 - p_space->signal_bit_mask[2];
        header.resolution[3] = 16 - p_space->signal_bit_mask[3];
        header.resolution[4] = 16 - p_space->signal_bit_mask[4];
        header.resolution[5] = 16 - p_space->signal_bit_mask[5];
        header.resolution[6] = 16 - p_space->signal_bit_mask[6];
        header.resolution[7] = 16 - p_space->signal_bit_mask[7];
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_R)
            for(i = 0; i < 256; i++)
                header.color_rescale_r[i] = p_space->color_rescale_r[i];
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_G)
            for(i = 0; i < 256; i++)
                header.color_rescale_g[i] = p_space->color_rescale_g[i];
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_B)
            for(i = 0; i < 256; i++)
                header.color_rescale_b[i] = p_space->color_rescale_b[i];
        //----------------------------------------------------------------
        if(header.LaserBoy_wave_mode & LASERBOY_COLOR_RESCALE_I)
            for(i = 0; i < 256; i++)
                header.color_rescale_i[i] = p_space->color_rescale_i[i];
        //----------------------------------------------------------------
        if(p_space->view_angle != 0.0)
            copy.rotate_around_origin();
        //----------------------------------------------------------------
        if(copy.is_2D() || p_space->auto_flatten_z)
        {
            header.signal_id [5] = p_space->signal_if_not_z;
            header.resolution[5] = 9;
            copy.flatten_z();
        }
        //----------------------------------------------------------------
        if(optimized)
        {
            bool             max_wag_microsec_was = p_space->max_wag_microsec;
            LaserBoy_vertex  origin;

            header.LaserBoy_wave_mode |= LASERBOY_WAVE_OPTIMIZED;
            header.parms.lit_dwell_overhang     =        p_space->lit_dwell_overhang;
            header.parms.lit_delta_max          = (float)p_space->lit_delta_max;
            header.parms.blank_delta_max        = (float)p_space->blank_delta_max;
            header.parms.max_dwell_millisec     = (float)p_space->max_dwell_millisec;
            header.parms.max_wag_microsec       = (float)p_space->max_wag_microsec;
            header.parms.insignificant_distance = (float)p_space->insignificant_distance;
            header.parms.insignificant_angle    = (float)p_space->insignificant_angle;
            header.parms.frames_per_second      =    (timed)
                                                   ? ((float)p_space->frames_per_second)
                                                   : (0);
            header.to_fstream_wave(out); // take up the space!

            if(clip)
                p_space->max_wag_microsec = 0;
            copy.optimize(0); // as a single frame
            p_space->max_wag_microsec = max_wag_microsec_was;
            copy.add_coda(origin);
            copy.coda += LaserBoy_segment(copy.coda.back(), origin);
            copy.to_fstream_wave(out, header, true, timed, clip);
        }
        else
        {
            header.to_fstream_wave(out); // take up the space!
            copy.to_fstream_wave(out, header, false, timed, false);
        }
        //----------------------------------------------------------------
        header.to_fstream_wave(out);
        //----------------------------------------------------------------
        out.close();
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_frame::to_fstream_wave(std::fstream&         out,
                                     LaserBoy_wave_header& header,
                                     bool                  optimized,
                                     bool                  timed,
                                     bool                  clip
                                    )
{
    if(optimized)
    {
        int  chill_samples = (int)ceil(   p_space->sample_rate
                                        * (   p_space->max_dwell_millisec
                                            / 500000.0 // make it twice the max_dwell_millisec
                                          )
                                      );
        LaserBoy_vertex   origin;
        LaserBoy_segment  chill;
        if(chill_samples < 10)
            chill_samples = 10;
        for(int i = 0; i < chill_samples; i++)
            chill.push_back(origin);
        //----------------------------------------------------------------
        p_space->p_GUI->display_state("saving frame as wave");
        //----------------------------------------------------------------
        if(timed)
        {
            int  total_frame_scans = (int)ceil(   (p_space->sample_rate * p_space->still_frame_duration)
                                                / (size() + bridge.size())
                                              );
            chill.to_fstream_wave(out, header, false, false);
            intro.to_fstream_wave(out, header, false, false);
            LaserBoy_segment::to_fstream_wave(out, header, is_wagged, is_wagged);
            if(total_frame_scans > 1)
            {
                for(int frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
                {
                    bridge.to_fstream_wave(out, header, true, (!is_wagged) && (frame_scan == 1)); // frame scan 1 is unique
                    LaserBoy_segment::to_fstream_wave(out, header, is_wagged && (frame_scan < (total_frame_scans - 1)), false);
                    p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
                }
                coda.to_fstream_wave(out, header, false, false);
            }
            chill.to_fstream_wave(out, header, true, total_frame_scans == 1);
        } // end if(timed)
        else // single frame
        {
            if(clip)
            {
                bridge.to_fstream_wave(out, header, false, false);
                LaserBoy_segment::to_fstream_wave(out, header, true, true);
            }
            else // with lead and tail
            {
                chill.to_fstream_wave(out, header, false, false);
                intro.to_fstream_wave(out, header, false, false);
                LaserBoy_segment::to_fstream_wave(out, header, false, false);
                coda.to_fstream_wave(out, header, false, false);
                chill.to_fstream_wave(out, header, true, true);
            }
        }
    } // end if(optimized)
    //------------------------------------------------------------------------
    else if(timed) // no intro, bridge or coda!
    {
        p_space->p_GUI->display_state("saving frame as wave");
        //----------------------------------------------------------------
        int  total_frame_scans = (int)ceil(   (   p_space->sample_rate
                                                * p_space->still_frame_duration
                                              )
                                            / size()
                                          );
        LaserBoy_segment::to_fstream_wave(out, header, true, true);
        for(int frame_scan = 1; frame_scan < total_frame_scans; frame_scan++)
        {
            LaserBoy_segment::to_fstream_wave(out, header, true, false);
            p_space->p_GUI->display_progress(total_frame_scans - frame_scan);
        }
    } // end else if(timed)
    //------------------------------------------------------------------------
    else // no intro, bridge or coda!
        LaserBoy_segment::to_fstream_wave(out, header, true, true);
    //------------------------------------------------------------------------
    return;
}

//############################################################################
bool LaserBoy_frame::save_as_dxf(const string& file) const
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        u_int               segment_vector_index,
                            vertex_index;

        LaserBoy_frame_set  segments;
        //----------------------------------------------------------------
        if(p_space->auto_minimize)
        {
            LaserBoy_frame  copy(*this);
            copy.minimize(0);
            segments = copy.explode_segments();
        }
        else
            segments = explode_segments();
        //----------------------------------------------------------------
        out << " 0\n"
                "SECTION\n"
                " 2\n"
                "ENTITIES\n";
        //----------------------------------------------------------------
        for(segment_vector_index = 0; segment_vector_index < segments.size(); segment_vector_index++)
        {
            if(segments[segment_vector_index].number_of_color_vectors() == 1)
            {
                if(    (   segments[segment_vector_index].first_lit_anchor().as_3D_short()
                         | segments[segment_vector_index].first_lit_vertex().as_3D_short()
                       )
                    <= p_space->insignificant_distance
                  )
                {
                    out << " 0\n"
                           "POINT\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].first_lit_vertex().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].first_lit_vertex());
                    out << "\n 10\n"
                        << segments[segment_vector_index].first_lit_anchor().x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].first_lit_anchor().y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].first_lit_anchor().z
                        << ".00\n";
                }
                else
                {
                    out << " 0\n"
                           "LINE\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].first_lit_vertex().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].first_lit_vertex());
                    out << "\n 10\n"
                        << segments[segment_vector_index].first_lit_anchor().x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].first_lit_anchor().y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].first_lit_anchor().z
                        << ".00\n 11\n"
                        << segments[segment_vector_index].first_lit_vertex().x
                        << ".00\n 21\n"
                        << segments[segment_vector_index].first_lit_vertex().y
                        << ".00\n 31\n"
                        << segments[segment_vector_index].first_lit_vertex().z
                        << ".00\n";
                }
            }
            //----------------------------------------------------------------
            else
            {
                out << " 0\n"
                        "POLYLINE\n"
                        " 70\n"
                    << segments[segment_vector_index].is_closed_polygon()
                    << "\n 10\n"
                       "0.00\n"
                       " 20\n"
                       "0.00\n"
                       " 30\n"
                       "0.00\n"
                       //---------
                       " 0\n"
                       "VERTEX\n";
                if(p_space->save_true_color_dxf)
                    out << " 420\n"
                        << segments[segment_vector_index].at(1).color_of(p_space->black_level);
                else
                    out << " 62\n"
                        << p_space->
                           palette_picker(LASERBOY_DXF)
                           .best_match(segments[segment_vector_index].at(1));
                out << "\n 10\n"
                    << segments[segment_vector_index].first_lit_anchor().x
                    << ".00\n 20\n"
                    << segments[segment_vector_index].first_lit_anchor().y
                    << ".00\n 30\n"
                    << segments[segment_vector_index].first_lit_anchor().z
                    << ".00\n";

                for(vertex_index = 1; vertex_index < segments[segment_vector_index].size() - 1; vertex_index++)
                {
                    out << " 0\n"
                        << "VERTEX\n";
                    if(p_space->save_true_color_dxf)
                        out << " 420\n"
                            << segments[segment_vector_index].at(vertex_index + 1).color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(segments[segment_vector_index].at(vertex_index + 1));
                    out << "\n 10\n"
                        << segments[segment_vector_index].at(vertex_index).x
                        << ".00\n 20\n"
                        << segments[segment_vector_index].at(vertex_index).y
                        << ".00\n 30\n"
                        << segments[segment_vector_index].at(vertex_index).z
                        << ".00\n";
                }
                out << " 0\n"
                       "VERTEX\n";
                if(p_space->save_true_color_dxf)
                    out << " 420\n";
                else
                    out << " 62\n";
                out << "0\n"
                       " 10\n"
                    << segments[segment_vector_index].at(vertex_index).x
                    << ".00\n 20\n"
                    << segments[segment_vector_index].at(vertex_index).y
                    << ".00\n 30\n"
                    << segments[segment_vector_index].at(vertex_index).z
                    << ".00\n";
            }
        }
        //----------------------------------------------------------------
        out << " 0\n"
               "ENDSEC\n"
               " 0\n"
               "EOF\n";
        //----------------------------------------------------------------
        out.close();
        return true;
    }
    return false;
}

//############################################################################
bool LaserBoy_frame::save_as_bmp(const string& file) const
{
    char                file_name[80];
    struct LaserBoy_bmp bmp_out = {0};
    LaserBoy_frame      copy(*this);
    if(size() > 1)
    {
        //----------------------------------------------------------------
        p_space->p_GUI->display_state("rendering bmp file " + file);
        //----------------------------------------------------------------
        if(copy.number_of_unique_colors() == 1)
        {
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  1);
            bmp_clear(&bmp_out, 1);
            copy.promote_to_true_color();
            copy.best_reduce_to_palette();
            if(p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out))
            {
                bmp_set_palette_index(&bmp_out, 0, 255, 255, 255);
                bmp_set_palette_index(&bmp_out, 1,   0,   0,   0);
            }
        }
        else if(copy.number_of_unique_colors() <= 15)
        {
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  4);
            bmp_clear(&bmp_out, 15);
            copy.promote_to_true_color();
            copy.best_reduce_to_palette();
            if(p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out))
                return false;
        }
        else if(copy.number_of_unique_colors() <= 255)
        {
            if(copy.palette_index == LASERBOY_TRUE_COLOR)
                copy.best_reduce_to_palette();
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  8);
            bmp_clear(&bmp_out, 255);
            p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out);
        }
        else
        {
            bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size, 24);
            bmp_clear(&bmp_out, 0x00000000);
        }
        //----------------------------------------------------------------
        u_int                i;
        LaserBoy_3D_double   _0,
                             _1;
        int                  bmp_center  = bmp_out.yres / 2;
        double               scale       = bmp_out.yres / 65535.0;
        //----------------------------------------------------------------
        for(i = 1; i < copy.size(); i++)
        {
            if(copy.at(i).is_color(p_space->black_level))
            {
                _0 = copy.at(i - 1);
                _1 = copy.at(i    );
                //------------------------------------------------------------

                if(copy.palette_index == LASERBOY_TRUE_COLOR)
                    bmp_put_thick_line_fast(    &bmp_out
                                             ,  (int)(_0.x * scale) + bmp_center
                                             , ((int)(_0.y * scale) + bmp_center)
                                             ,  (int)(_1.x * scale) + bmp_center
                                             , ((int)(_1.y * scale) + bmp_center)
                                             , bmp_out.make_rgb(copy.at(i).r, copy.at(i).g, copy.at(i).b)
                                             , p_space->rendered_line_width
                                           );
                else
                    bmp_put_thick_line_fast(   &bmp_out
                                             ,  (int)(_0.x * scale) + bmp_center
                                             , ((int)(_0.y * scale) + bmp_center)
                                             ,  (int)(_1.x * scale) + bmp_center
                                             , ((int)(_1.y * scale) + bmp_center)
                                             , copy.at(i).c
                                             , p_space->rendered_line_width
                                           );
            }
        }
    } // end if(size() > 1)
    else
    {
        bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  1);
        bmp_set_palette_index(&bmp_out, 0, 255, 255, 255);
        bmp_set_palette_index(&bmp_out, 1,   0,   0,   0);
        bmp_clear(&bmp_out, 1);
    }
    sprintf(file_name, "%s", file.c_str());
    p_space->p_GUI->display_state("writing   bmp file " + string(file_name));
    bmp_save_as(&bmp_out, file_name);
    bmp_free(&bmp_out);
    return true;
}

//############################################################################
void LaserBoy_frame::dump() const
{
    cout << "format   = " << (int)format        << ENDL;
    cout << "palette  = " << (int)palette_index << ENDL;
    cout << "name     = " << name               << ENDL;
    cout << "owner    = " << owner              << ENDL;
    cout << "quantity = " << quantity           << ENDL;
    cout << "identity = " << identity           << ENDL;
    cout << "total    = " << total              << ENDL;
//    cout << "scanner  = " << (int)scanner  << ENDL;
//    cout << "future   = " << (int)future   << ENDL;
    cout << ENDL << ENDL;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
