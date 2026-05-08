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
// LaserBoy_segment.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_SEGMENT_DEFINITIONS__
#define __LASERBOY_SEGMENT_DEFINITIONS__

//############################################################################
#include "LaserBoy_bmp.hpp"
#include "LaserBoy_real_segment_set.hpp"

//############################################################################
class LaserBoy_segment : public vector<LaserBoy_vertex>
{
public:
    LaserBoy_segment()
                     : vector<LaserBoy_vertex>(),
                       palette_index (LASERBOY_ILDA_DEFAULT),
                       segment_error (LASERBOY_OK)
    {}
    //------------------------------------------------------------------------
    LaserBoy_segment(const int& palette_index,
                     bool       add_origin
                    )
                     : vector<LaserBoy_vertex>(),
                       palette_index (palette_index),
                       segment_error (LASERBOY_OK)
    {
        if(add_origin)
        {
            reserve(2);
            push_back(LaserBoy_vertex());
            push_back(LaserBoy_vertex());
        }
    }
    //------------------------------------------------------------------------
    LaserBoy_segment(const LaserBoy_segment& segment)
                     : vector<LaserBoy_vertex>(),
                       palette_index (segment.palette_index),
                       segment_error (LASERBOY_OK)
    {
        reserve(segment.size());
        insert(end(), segment.begin(), segment.end());
    }
    //------------------------------------------------------------------------
    LaserBoy_segment(const LaserBoy_real_segment& rs)
                     : vector<LaserBoy_vertex>(),
                       palette_index (rs.palette_index),
                       segment_error (LASERBOY_OK)
    {
        reserve(rs.size());
        for(size_t i = 0; i < rs.size(); i++)
            push_back((LaserBoy_vertex)rs[i]);
    }
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short from,
                     LaserBoy_vertex   to
                    );   // the line
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short from,
                     LaserBoy_vertex   to,
                     const u_int&      max_d
                    );   // the line
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_vertex   p1,
                     LaserBoy_vertex   p2,
                     string            font,
                     string            text
                    );   // mono spaced font constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_vertex   p1,
                     LaserBoy_vertex   p2,
                     string            font,
                     string            text,
                     int               dud
                    );   // variable spaced font constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     LaserBoy_3D_short arc_start,
                     double            arc_angle // OK degrees, whatever!
                    );   // circular arc constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     LaserBoy_3D_short arc_start,
                     double            arc_angle,
                     double            radii_ratio
                    );   // elliptical arc constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     LaserBoy_3D_short first_vertex,
                     u_int             number_of_sides
                    );   // polygon constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     LaserBoy_3D_short first_vertex,
                     u_int             number_of_sides,
                     double            ratio
                    );   // polystar constructor
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     u_int             radius,
                     u_int             rhodonea_numerator,
                     u_int             rhodonea_denominator
                    );   // rhodonea
    //------------------------------------------------------------------------
    LaserBoy_segment(u_int             fixed_radius,
                     LaserBoy_3D_short center,
                     u_int             roller_radius,
                     int               roller_offset
                    );   // epitrochoid (and epicycloid)
    //------------------------------------------------------------------------
    LaserBoy_segment(u_int             fixed_radius,
                     u_int             roller_radius,
                     int               roller_offset,
                     LaserBoy_3D_short center
                    );   // hypotrochoid (and hypocycloid)
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_3D_short center,
                     u_int             amplitude_x,
                     u_int             amplitude_y,
                     u_int             frequency_x,
                     u_int             frequency_y,
                     double            phase_x,
                     double            phase_y
                    );   // Lissajous curve
    //------------------------------------------------------------------------
    LaserBoy_segment(LaserBoy_oscillator LBO1,
                     LaserBoy_oscillator LBO2,
                     LaserBoy_oscillator LBO3,
                     LaserBoy_oscillator LBO4,
                     const u_int         n,
                     const double        duration
                    ); // Harmonograph constructor
    //------------------------------------------------------------------------
   ~LaserBoy_segment()
    {}
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_segment& segment)
            {
                if(palette_index != segment.palette_index)
                    return false;
                if(size() != segment.size())
                    return false;
                for(size_t i = 0; i < size(); i++)
                    if(at(i) != segment.at(i))
                        return false;
                return true;
            }
    //------------------------------------------------------------------------
    LaserBoy_segment&  operator += (LaserBoy_segment segment)
    {
        if(palette_index != segment.palette_index)
        {
            promote_to_true_color();
            segment.promote_to_true_color();
        }
        insert(end(), segment.begin(), segment.end());
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment&  operator += (const LaserBoy_vertex& vertex)
    {
        push_back(vertex);
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment   operator + (LaserBoy_segment segment) const
    {
        LaserBoy_segment sum(*this);
        if(sum.palette_index != segment.palette_index)
        {
            sum.promote_to_true_color();
            segment.promote_to_true_color();
        }
        sum.insert(end(), segment.begin(), segment.end());
        sum.sync_rgb_and_palette();
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment   operator + (const LaserBoy_vertex& vertex) const
    {
        LaserBoy_segment sum(*this);
        sum.push_back(vertex);
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment&  operator =  (const LaserBoy_segment& segment)
    {
        palette_index = segment.palette_index;
        segment_error = segment.segment_error;
        clear();
        reserve(segment.size());
        insert(begin(), segment.begin(), segment.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment&  operator =  (const LaserBoy_real_segment& real_segment)
    {
        palette_index = real_segment.palette_index;
        segment_error = real_segment.real_segment_error;
        clear();
        reserve(real_segment.size());
        for(size_t i = 0; i < real_segment.size(); i++)
            push_back((LaserBoy_vertex)real_segment[i]);
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    bool is_2D() const
    {
        if(size() > 1)
        {
            for(size_t i = 0; i < size(); i++)
                if(at(i).z != 0)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool is_flat_in_x() const
    {
        if(size() > 1)
        {
            short _x = first_lit_anchor().x;
            for(size_t i = 1; i < size(); i++)
                if(at(i).is_lit() && at(i).x != _x)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool is_flat_in_y() const
    {
        if(size() > 1)
        {
            short _y = first_lit_anchor().y;
            for(size_t i = 1; i < size(); i++)
                if(at(i).is_lit() && at(i).y != _y)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool is_flat_in_z() const
    {
        if(size() > 1)
        {
            short _z = first_lit_anchor().z;
            for(size_t i = 1; i < size(); i++)
                if(at(i).is_lit() && at(i).z != _z)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool at_index_has_magnitude(size_t vertex_index) const
    {
        if(    vertex_index // != 0
            && vertex_index < size()
          )
        {
            return (bool)(at(vertex_index - 1) | at(vertex_index)); // distance
        }
        else
            return false;
    }
    //------------------------------------------------------------------------
    double front_to_back_magnitude() const
    {
        return (front() | back());
    }
    //------------------------------------------------------------------------
    LaserBoy_vertex first_lit_vertex() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return at(i);
        return LaserBoy_vertex(0);
    }
    //------------------------------------------------------------------------
    int first_lit_vector_index() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return i;
        return -1;
    }
    //------------------------------------------------------------------------
    LaserBoy_vertex first_lit_anchor() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return at(i - 1);
        return LaserBoy_vertex(0);
    }
    //------------------------------------------------------------------------
    int first_lit_anchor_index() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return (i - 1);
        return -1;
    }
    //------------------------------------------------------------------------
    int last_lit_anchor_index() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return i - 1;
        return -1;
    }
    //------------------------------------------------------------------------
    LaserBoy_vertex last_lit_anchor() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return at(i - 1);
        return LaserBoy_vertex(0);
    }
    //------------------------------------------------------------------------
    LaserBoy_vertex last_lit_vector() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return at(i);
        return LaserBoy_vertex(0);
    }
    //------------------------------------------------------------------------
    int last_lit_vector_index() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return i;
        return -1;
    }
    //------------------------------------------------------------------------
    int number_of_lit_vectors() const
    {
        int count = 0;
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                count++;
        return count;
    }
    //------------------------------------------------------------------------
    size_t number_of_color_vectors() const;
    size_t number_of_dark_vectors () const;
    //------------------------------------------------------------------------
    int number_of_blank_vectors() const
    {
        int count = 1; //first vertex is always an anchor
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_blank())
                count++;
        return count;
    }
    //------------------------------------------------------------------------
    int number_of_unique_colors() const
    {
        if(size() > 1)
        {
            size_t            i,
                              j;
            LaserBoy_palette  palette;
            if(first_lit_vector_index() > -1)
            {
                palette.push_back((LaserBoy_color)first_lit_vertex());
                for(i = first_lit_vector_index() + 1; i < size(); i++)
                {
                    for(j = 0; j < palette.number_of_colors(); j++)
                    {
                        if(palette[j] == (LaserBoy_color)at(i))
                            break;
                    }
                    if(    j == palette.number_of_colors() // color not found
                        && at(i).is_lit()
                      )
                        palette.push_back((LaserBoy_color)at(i));
                }
                return palette.number_of_colors();
            }
        }
        return 0;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment& remove_vertex(size_t vertex_index)
    {
        if(size() <= 2) // takes 2 vertices to make a vector!
            clear();
        else if(vertex_index < size())
        {
            size_t           i;
            LaserBoy_segment s(palette_index, false);
            s.reserve(size() - 1);
            for(i = 0; i < vertex_index; i++)
                s += at(i);
            for(i = vertex_index + 1; i < size(); i++)
                s += at(i);
            *this = s;
        }
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_short    segment_front                 () const ;
    LaserBoy_3D_short    segment_back                  () const ;
    LaserBoy_3D_short    segment_top                   () const ;
    LaserBoy_3D_short    segment_bottom                () const ;
    LaserBoy_3D_short    segment_right                 () const ;
    LaserBoy_3D_short    segment_left                  () const ;
    short                max_x                         () const ;
    short                max_y                         () const ;
    short                max_z                         () const ;
    short                min_x                         () const ;
    short                min_y                         () const ;
    short                min_z                         () const ;
    LaserBoy_3D_short    max_off_axis_xyz              () const ;
    double               max_distance_from_origin      () const ;
    double               max_distance_from_origin_xy   () const ;
    double               max_distance_from_origin_zy   () const ;
    double               min_distance_from_origin      () const ;
    double               min_distance_from_origin_xy   () const ;
    u_short              height                        () const ;
    u_short              width                         () const ;
    u_short              depth                         () const ;
    u_short              max_dimension                 () const ;
    double               relative_volume               () const ;
    double               relative_volume_to_origin     () const ;
    bool                 is_closed_polygon             () const ;
    bool                 segment_passes_through_origin () const ;
    LaserBoy_segment&    reverse                       ();
    LaserBoy_segment&    reverse_sub_segment           (size_t p1, size_t p2);
    LaserBoy_segment&    reorder_from                  (size_t vertex_index);
    LaserBoy_segment&    randomize_segments            ();
    LaserBoy_segment&    conglomerate_lit_segments     ();
    LaserBoy_segment     shortest_path_of_segments     (const LaserBoy_segment& previous_frame);
    void                 blank_all_vertices            ();
    void                 unblank_all_vertices          ();
    void                 blacken_vertices              ();
    void                 reduce_blank_vectors          ();
    void                 omit_equivalent_vectors       ();
    void                 remove_dots                   ();
    LaserBoy_Error_Code  enhance_dots                  ();
    void                 remove_dwell_vertices         ();
    void                 remove_short_vectors          ();
    void                 reduce_lit_vectors            ();
    void                 impose_bit_resolution         ();
    LaserBoy_Error_Code  reorder_segments              (const LaserBoy_segment& previous_frame);
    LaserBoy_Error_Code  fracture_segments             ();
    LaserBoy_Error_Code  bond_segments                 ();
    LaserBoy_Error_Code  add_vertex                    (size_t  vertex_index);
    LaserBoy_Error_Code  break_segment                 (size_t& vertex_index);
    LaserBoy_Error_Code  connect_the_dots              (size_t p1, size_t p2); // vertex indices
    double               vector_angle                  (size_t vertex) const ;
    double               total_angle                   () const ;
    double               max_angle                     () const ;
    double               total_magnitude               () const ;
    double               max_magnitude                 () const ;
    double               max_color_magnitude           () const ;
    double               max_dark_magnitude            () const ;
    double               length_in_seconds             () const ;
    LaserBoy_Error_Code  add_dwell                     ();
    LaserBoy_Error_Code  add_lit_span_vertices         ();
    void                 add_lit_span_vertices         (const u_int& max_d);
    LaserBoy_Error_Code  add_blank_span_vertices       ();
    LaserBoy_Error_Code  add_blank_span_vertices       (u_int max_d);
    LaserBoy_segment&    flip                          (u_int plane);
    LaserBoy_segment&    quarter_turn                  (u_int plane, u_int turns);
    LaserBoy_segment&    normalize                     (double scale);
    LaserBoy_segment&    normalize_vectors             (double scale);
    LaserBoy_segment&    normalize_vectors_with_origin (double scale);
    LaserBoy_segment&    z_order_vertices              (unsigned short span);
    LaserBoy_segment&    flatten_z                     ();
    LaserBoy_segment&    flat_to_2D                    ();
    LaserBoy_segment&    center_x                      ();
    LaserBoy_segment&    center_y                      ();
    LaserBoy_segment&    center_z                      ();
    LaserBoy_segment&    center                        ();
    //------------------------------------------------------------------------
    LaserBoy_Bounds      rotate                        (LaserBoy_3D_double a);
    LaserBoy_Bounds      rotate_around_origin          (LaserBoy_3D_double a);
    //------------------------------------------------------------------------
    LaserBoy_Bounds      rotate_on_coordinates         (LaserBoy_3D_double p,
                                                        LaserBoy_3D_double a
                                                       );
    //------------------------------------------------------------------------
    LaserBoy_Bounds      rotate_on_coordinates_x       (LaserBoy_3D_double p, double a);
    LaserBoy_Bounds      rotate_on_coordinates_y       (LaserBoy_3D_double p, double a);
    LaserBoy_Bounds      rotate_on_coordinates_z       (LaserBoy_3D_double p, double a);
    LaserBoy_Bounds      rotate_around_origin          ();
    //------------------------------------------------------------------------
    void                 ripple                        (int    direction,
                                                        double amplitude,
                                                        double freq     ,
                                                        double phase
                                                       );
    //------------------------------------------------------------------------
    LaserBoy_palette     as_color_table                () const ;
    void                 strip_color                   ();
    void                 strip_color_rgb               (const LaserBoy_color& c);
    void                 strip_color_or                ();
    void                 strip_color_avg               ();
    void                 to_palette_by_index           (int index);
    void                 to_target_palette_by_index    ();
    void                 best_match_palette            (int index);
    void                 best_match_target_palette     ();
    void                 convert_black_to_blank        ();
    void                 convert_black_to_color        ();
    void                 convert_blank_to_black        ();
    void                 set_vertex_to_black           (int index);
    void                 impose_black_level            ();
    //------------------------------------------------------------------------
    void                 index_palette                 (int index_multiple, int offset);
    void                 index_segments_palette        (int index_multiple, int offset);
    void                 span_palette                  (double span_factor, int offset);
    void                 span_x_palette                (double span_factor, int offset);
    void                 span_y_palette                (double span_factor, int offset);
    void                 span_z_palette                (double span_factor, int offset);
    void                 span_radial_palette           (double span_factor, int offset);
    void                 span_axial_palette            (double span_factor, int offset);
    void                 span_segments_palette         (double span_factor, int offset);
    void                 random_color_vectors_palette  (int index_multiple);
    void                 random_color_segments_palette ();
    //------------------------------------------------------------------------
    void                 index_hues                    (int index_multiple, int offset);
    void                 index_segments_hues           (int index_multiple, int offset);
    void                 span_hues                     (double span_factor, int offset);
    void                 span_x_hues                   (double span_factor, int offset);
    void                 span_y_hues                   (double span_factor, int offset);
    void                 span_z_hues                   (double span_factor, int offset);
    void                 span_radial_hues              (double span_factor, int offset);
    void                 span_axial_hues               (double span_factor, int offset);
    void                 span_segments_hues            (double span_factor, int offset);
    void                 random_color_vectors_hues     (int index_multiple);
    void                 random_color_segments_hues    ();
    //------------------------------------------------------------------------
    LaserBoy_segment&    rotate_colors                 (int steps );
    bool                 find_rgb_in_palette           (const LaserBoy_palette& palette);
    void                 set_rgb_from_palette          ();
    void                 set_palette_to_332            ();
    void                 sync_rgb_and_palette          ();
    void                 bit_reduce_to_palette         ();
    void                 best_reduce_to_palette        ();
    void                 promote_to_true_color         ();
    void                 shade                         (u_char shade);
    void                 tint                          (u_char tint);
    LaserBoy_Error_Code  color_from_bmp                (const string& file);
    void                 color_from_bmp                (struct LaserBoy_bmp* bmp);
    LaserBoy_Error_Code  subtract_bmp                  (const string& file);
    void                 subtract_bmp                  (struct LaserBoy_bmp* bmp);
    //------------------------------------------------------------------------
    LaserBoy_Bounds      apply_view                    ();
    //------------------------------------------------------------------------
    LaserBoy_Bounds      move                          (LaserBoy_3D_double d);
    LaserBoy_Bounds      move                          ();
    //------------------------------------------------------------------------
    LaserBoy_Bounds      scale                         (LaserBoy_3D_double s);
    LaserBoy_Bounds      scale_on_coordinates          (LaserBoy_3D_double p,
                                                        LaserBoy_3D_double s
                                                       );
    //------------------------------------------------------------------------
    LaserBoy_Bounds      scale_around_origin           (LaserBoy_3D_double s);
    LaserBoy_Bounds      scale_around_origin           ();
    LaserBoy_3D_double   rectangular_center_of         () const ;
    LaserBoy_3D_double   mean_of_coordinates           () const ;
    LaserBoy_3D_double   centroid_of_coordinates_xy    (double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_coordinates_zy    (double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_coordinates_xz    (double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_coordinates       () const ;
    size_t               number_of_segments            () const ;
    //------------------------------------------------------------------------
    LaserBoy_segment     blend                         (const LaserBoy_segment& segment,
                                                        double ratio
                                                       );
    //------------------------------------------------------------------------
    bool                 find_segment_at_index         (size_t  index,
                                                        size_t& start,
                                                        size_t& end
                                                       ) const ;
    //------------------------------------------------------------------------
    bool                 find_segment_of_vertex        (size_t  vertex_index,
                                                        size_t& start,
                                                        size_t& end,
                                                        size_t& segment_index
                                                       ) const ;
    //------------------------------------------------------------------------
    bool                 same_as                       (const LaserBoy_segment& segment) const ;
    //------------------------------------------------------------------------
    LaserBoy_segment     copy_segment                  (size_t index) const ;
    LaserBoy_frame_set   explode_segments              () const ;
    LaserBoy_3D_double   rectangular_center_of_segment (size_t index) const ;
    LaserBoy_3D_double   mean_of_coordinates_of_segment(size_t index) const ;
    LaserBoy_3D_double   centroid_of_segment_xy        (size_t index, double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_segment_zy        (size_t index, double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_segment_xz        (size_t index, double& area, LaserBoy_segment& s) const ;
    LaserBoy_3D_double   centroid_of_segment           (size_t index) const ;
    size_t               segment_index_of_vertex       (size_t index) const ;
    LaserBoy_Bounds      move_segment                  (size_t index, LaserBoy_3D_double d);
    LaserBoy_Bounds      rotate_segment                (size_t index, LaserBoy_3D_double a);
    LaserBoy_Bounds      rotate_segment_around_origin  (size_t index, LaserBoy_3D_double a);
    LaserBoy_Bounds      scale_segment                 (size_t index, LaserBoy_3D_double m);
    LaserBoy_Bounds      scale_segment_around_origin   (size_t index, LaserBoy_3D_double m);
    //------------------------------------------------------------------------
    LaserBoy_real_segment to_real_segment              (bool add_origin = false) const ;
    //------------------------------------------------------------------------
    void                 to_fstream_wave               (std::fstream&         out,
                                                        LaserBoy_wave_header& header,
                                                        bool                  end_of_frame = false,
                                                        bool                  unique_frame = false
                                                       );
    //------------------------------------------------------------------------
    int                  palette_index;
    LaserBoy_Error_Code  segment_error;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
