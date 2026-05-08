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
// LaserBoy_utility.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_UTILITY_DEFINITIONS__
#define __LASERBOY_UTILITY_DEFINITIONS__

//############################################################################
#include "LaserBoy_vertex.hpp"

//############################################################################
enum LaserBoy_clip_code
{
    no_line = 0,
    p1_in_p2_in,
    p1_in_p2_out,
    p1_out_p2_in,
    p1_out_p2_out
};


//############################################################################
inline bool clip_check(const LaserBoy_3D_double& max,
                       const LaserBoy_3D_double& min
                      )
{
    LaserBoy_3D_double dim(max - min); // dimensions of bounds
    if(    (dim.x == 0 && dim.y == 0)
        || (dim.x == 0 && dim.z == 0)
        || (dim.y == 0 && dim.z == 0)
        || (min.x > max.x)
        || (min.y > max.y)
        || (min.z > max.z)
      )
        return false;
      return true;
}

//############################################################################
inline bool close_to_int(double &d)
{
    double integer,
           fraction = modf(d, &integer);
    if(fraction < LASERBOY_EPSILON)
    {
        d = integer;
        return true;
    }
    if((1.0 - fraction) < LASERBOY_EPSILON)
    {
        d = integer + 1;
        return true;
    }
    return false;
}

//############################################################################
inline bool less_than(const LaserBoy_3D_double p1, // for std::sort
                      const LaserBoy_3D_double p2
                     )
{
    return p1 < p2;
}

//############################################################################
inline int linear_steps(const LaserBoy_3D_double p1,
                        const LaserBoy_3D_double p2,
                        const double delta_max
                       )
{
    return (int)ceil((p1 | p2) / delta_max);
}

//############################################################################
inline int start_dwell_vertices(double angle, // zero to pi radians
                                int    sample_rate,
                                double max_dwell_microsec
                               )
{
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline int inline_dwell_vertices(double angle, // zero to pi radians
                                 int    sample_rate,
                                 double max_dwell_microsec
                                )
{
    if(angle < (pi / 8))
        return 0;
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline int end_dwell_vertices(double angle, // zero to pi radians
                              int    sample_rate,
                              double max_dwell_microsec
                             )
{
    return (int)(ceil(   (angle / pi) // zero to one
                       * (sample_rate * (max_dwell_microsec / 1000000.0))
                     )
                );
}

//############################################################################
inline double three_point_angle(LaserBoy_3D_double _0, LaserBoy_3D_double _1, LaserBoy_3D_double _2)
{
    return delta_angle( _0 | _2, // distance between vertices 0,2
                        _0 | _1,
                        _1 | _2
                      );
}

//############################################################################
inline double three_point_angle_xy(LaserBoy_3D_double center, LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(center == _0)    return 0.0;
    if(center == _1)    return 0.0;
    if(    _0 == _1)    return 0.0;
    double angle = atan2(_1.y - center.y, _1.x - center.x) -
                   atan2(_0.y - center.y, _0.x - center.x);
    if(angle < 0.0)
        angle = two_pi + angle;
    return angle;
}

//############################################################################
inline double three_point_angle_zy(LaserBoy_3D_double center, LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(center == _0)    return 0.0;
    if(center == _1)    return 0.0;
    if(    _0 == _1)    return 0.0;
    double angle = atan2(_1.y - center.y, _1.z - center.z) -
                   atan2(_0.y - center.y, _0.z - center.z);
    if(angle < 0.0)
        angle = two_pi + angle;
    return angle;
}

//############################################################################
inline double three_point_angle_xz(LaserBoy_3D_double center, LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(center == _0)    return 0.0;
    if(center == _1)    return 0.0;
    if(    _0 == _1)    return 0.0;
    double angle = atan2(_1.z - center.z, _1.x - center.x) -
                   atan2(_0.z - center.z, _0.x - center.x);
    if(angle < 0.0)
        angle = two_pi + angle;
    return angle;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex(LaserBoy_3D_double p, LaserBoy_3D_double a)
{
    LaserBoy_3D_double  sin_a(sin(a.x), sin(a.y), sin(a.z)),
                        cos_a(cos(a.x), cos(a.y), cos(a.z)),
                        rotated;
    //------------------------------------------------------------------------
    rotated.y =       p.y * cos_a.x -  p.z * sin_a.x;
    rotated.z =       p.z * cos_a.x +  p.y * sin_a.x; // rotate around x
    rotated.z = rotated.z * cos_a.y -  p.x * sin_a.y;
    rotated.x =       p.x * cos_a.y + (p.z * cos_a.x +  p.y * sin_a.x) * sin_a.y; // rotate around y
    rotated.x = rotated.x * cos_a.z - (p.y * cos_a.x -  p.z * sin_a.x) * sin_a.z;
    rotated.y = rotated.y * cos_a.z + (p.x * cos_a.y + (p.z * cos_a.x +  p.y * sin_a.x) * sin_a.y) * sin_a.z; // rotate around z
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_xy(LaserBoy_3D_double p, LaserBoy_3D_double a)
{
    LaserBoy_3D_double  sin_a(sin(a.x), sin(a.y), sin(a.z)),
                        cos_a(cos(a.x), cos(a.y), cos(a.z)),
                        rotated;
    //------------------------------------------------------------------------
    rotated.x = (p.x * cos_a.y + (p.y * sin_a.x) * sin_a.y) * cos_a.z - (p.y * cos_a.x) * sin_a.z;
    rotated.y = (p.y * cos_a.x) * cos_a.z + (p.x * cos_a.y + (p.y * sin_a.x) * sin_a.y) * sin_a.z;
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_x(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.y = p.y * cos_a - p.z * sin_a;
    rotated.z = p.z * cos_a + p.y * sin_a; // rotate around x
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_y(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.x = p.x * cos_a - p.z * sin_a;
    rotated.z = p.z * cos_a + p.x * sin_a; // rotate around y
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_around_z(LaserBoy_3D_double p, double a)
{
    double              sin_a(sin(a)),
                        cos_a(cos(a));
    LaserBoy_3D_double  rotated(p);
    //------------------------------------------------------------------------
    rotated.x = p.x * cos_a - p.y * sin_a;
    rotated.y = p.y * cos_a + p.x * sin_a; // rotate around z
    return rotated;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates(LaserBoy_3D_double p1, LaserBoy_3D_double p2, LaserBoy_3D_double a)
{
    return rotate_vertex(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_x(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_x(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_y(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_y(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double rotate_vertex_on_coordinates_z(LaserBoy_3D_double p1, LaserBoy_3D_double p2, double a)
{
    return rotate_vertex_around_z(p1 - p2, a) + p2;
}

//############################################################################
inline LaserBoy_3D_double scale_vertex_on_coordinates(LaserBoy_3D_double p1, LaserBoy_3D_double p2, LaserBoy_3D_double m)
{
    return (m * (p1 - p2)) + p2;
}

//############################################################################
inline LaserBoy_3D_double direction(const LaserBoy_3D_double& _0, const LaserBoy_3D_double& _1) // z and y rotational angle from origin
{
    if(_0 == _1)
        return LaserBoy_3D_double(0.0, 0.0, 0.0);
    //............................................................
    LaserBoy_3D_double basline_vertex,
                       copy_0,
                       copy_1,
                       rotation;
    //............................................................
    copy_0 = _0;
    copy_1 = _1;
    copy_0.z = 0.0;
    copy_1.z = 0.0;
    basline_vertex = copy_0;
    basline_vertex.x += 1.0;
    rotation.z       = (double)delta_angle(  copy_1 | basline_vertex
                                           , basline_vertex.x
                                           , copy_1 | copy_0
                                          );
    if(rotation.z < 0)
        rotation.z = two_pi - rotation.z;
    //............................................................
    copy_0 = _0;
    copy_1 = _1;
    copy_0.y = 0.0;
    copy_1.y = 0.0;
    basline_vertex = copy_0;
    basline_vertex.x += 1.0;
    rotation.y       = (double)delta_angle(  copy_1 | basline_vertex
                                           , basline_vertex.x
                                           , copy_1 | copy_0
                                          );
    if(rotation.y < 0)
        rotation.y = two_pi - rotation.y;
    //............................................................
    return rotation;
}

//############################################################################
inline double x_direction(LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(    _0.y == _1.y
        && _0.z == _1.z
      )
        return 0.0;
    return atan2(_1.z - _0.z, _1.y - _0.y);
}

//############################################################################
inline double y_direction(LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(    _0.x == _1.x
        && _0.z == _1.z
      )
        return 0.0;
    return atan2(_1.z - _0.z, _1.x - _0.x);
}

//############################################################################
inline double z_direction(LaserBoy_3D_double _0, LaserBoy_3D_double _1)
{
    if(    _0.x == _1.x
        && _0.y == _1.y
      )
        return 0.0;
    return atan2(_1.y - _0.y, _1.x - _0.x);
}

//############################################################################
inline bool vector_passes_through_origin(const LaserBoy_3D_double& _0, const LaserBoy_3D_double& _1, const double& granularity)
{
    if(_0 == 0.0 || _1 == 0.0)
        return true;
    if(    (_0.x > 0.0 && _1.x > 0.0)
        || (_0.x < 0.0 && _1.x < 0.0)
        || (_0.y > 0.0 && _1.y > 0.0)
        || (_0.y < 0.0 && _1.y < 0.0)
        || (_0.z > 0.0 && _1.z > 0.0)
        || (_0.z < 0.0 && _1.z < 0.0)
      )
        return false;
    LaserBoy_3D_double origin;
    if(((_0 + (_1 - _0) * ((_0 | origin) / (_0 | _1))) | origin) < (1.0 / granularity))
        return true;
    return false;
}

//############################################################################
inline bool vector_passes_through_vertex(const LaserBoy_3D_double& _0,
                                         const LaserBoy_3D_double& _1,
                                         const LaserBoy_3D_double& v,
                                         const double& granularity
                                        )
{
    if(_0 == v || _1 == v)
        return true;
    if(    (_0.x > v.x && _1.x > v.x)
        || (_0.x < v.x && _1.x < v.x)
        || (_0.y > v.y && _1.y > v.y)
        || (_0.y < v.y && _1.y < v.y)
        || (_0.z > v.z && _1.z > v.z)
        || (_0.z < v.z && _1.z < v.z)
      )
        return false;
    if(((_0 + (_1 - _0) * ((_0 | v) / (_0 | _1))) | v) < (1.0 / granularity))
        return true;
    return false;
}

//############################################################################
inline LaserBoy_Bounds LaserBoy_bounds_check(const LaserBoy_3D_double& f,
                                             const LaserBoy_3D_double& max,
                                             const LaserBoy_3D_double& min
                                            )
{
    LaserBoy_Bounds bounds_flags = LASERBOY_IN_BOUNDS;
    //------------------------------------------------------------------------
    if(f.x > max.x)  bounds_flags |= LASERBOY_OUT_POS_X;
    if(f.x < min.x)  bounds_flags |= LASERBOY_OUT_NEG_X;
    if(f.y > max.y)  bounds_flags |= LASERBOY_OUT_POS_Y;
    if(f.y < min.y)  bounds_flags |= LASERBOY_OUT_NEG_Y;
    if(f.z > max.z)  bounds_flags |= LASERBOY_OUT_POS_Z;
    if(f.z < min.z)  bounds_flags |= LASERBOY_OUT_NEG_Z;
    //------------------------------------------------------------------------
    return bounds_flags;
}

//############################################################################
inline LaserBoy_Bounds LaserBoy_bounds_check(const LaserBoy_3D_double& f, const int& space = LASERBOY_CUBE)
{
    LaserBoy_3D_double max, min;
    switch(space)
    {
        case LASERBOY_CUBE          : max =  32767.0;
                                      min = -32767.0;
                                      break;
        //----------------------------------------------------------------
        case LASERBOY_SPHERE        : max =  23169.0;
                                      min = -23169.0;
                                      break;
        //----------------------------------------------------------------
        case LASERBOY_7_8THS_CUBE   : max =  28672.0;
                                      min = -28672.0;
                                      break;
        //----------------------------------------------------------------
        case LASERBOY_7_8THS_SPHERE : max =  20274.0;
                                      min = -20274.0;
                                      break;
        //----------------------------------------------------------------
    }
    return LaserBoy_bounds_check(f, max, min);
}

//############################################################################
inline LaserBoy_clip_code clip_vector(const LaserBoy_3D_double& p1,
                                      const LaserBoy_3D_double& p2,
                                      const LaserBoy_3D_double& max,
                                      const LaserBoy_3D_double& min,
                                      const double&             granularity,
                                      LaserBoy_3D_double&       c1,
                                      LaserBoy_3D_double&       c2
                                     )
{
    LaserBoy_3D_double dim(max - min); // dimensions of bounds
    LaserBoy_Bounds    p1_bounds = LaserBoy_bounds_check(p1, max, min),
                       p2_bounds = LaserBoy_bounds_check(p2, max, min);
    //--------------------------------------------------------------------
    if((p1_bounds | p2_bounds) == LASERBOY_IN_BOUNDS) // both ends are in bounds
    {
        c1 = p1;
        c2 = p2;
        return p1_in_p2_in;
    }
    //--------------------------------------------------------------------
    if(p1_bounds & p2_bounds) // both ends are out and p1--p2 does not pass through space
    {
        c1 = 0.0;
        c2 = 0.0;
        return no_line;
    }
    //--------------------------------------------------------------------
    double             magnitude,
                       quantum,
                       max_dim = 0.0,
                       length = p1 | p2; // distance between
    LaserBoy_3D_double diff(p2 - p1), // direction vector
                       check_point;
    //--------------------------------------------------------------------
    if(max_dim < dim.x) max_dim = dim.x;
    if(max_dim < dim.y) max_dim = dim.y;
    if(max_dim < dim.z) max_dim = dim.z;
    quantum = 0.85 / ((length / max_dim) * granularity);
    c1 = p1;
    c2 = p2;
     //---------------------------------------------------------------------
    if(p1_bounds == LASERBOY_IN_BOUNDS) // p1 is in bounds
    {
        for(magnitude = 1.0; magnitude >= 0.0; magnitude -= quantum)
        {
            check_point = p1 + (diff * magnitude); // parametric line equasion p1 to p2 is 0.0 to 1.0
            if(LaserBoy_bounds_check(check_point, max, min) == LASERBOY_IN_BOUNDS) // check_point is in bounds
            {
                c2 = check_point;
                return p1_in_p2_out;
            }
        }
        c2 = p1;
        return p1_in_p2_out;
    } // end p1 is in bounds
     //--------------------------------------------------------------------
    if(p2_bounds == LASERBOY_IN_BOUNDS) // p2 is in bounds
    {
        for(magnitude = 1.0; magnitude >= 0.0; magnitude -= quantum)
        {
            check_point = p2 - (diff * magnitude); // move from p1 toward p2
            if(LaserBoy_bounds_check(check_point, max, min) == LASERBOY_IN_BOUNDS) // check_point is in bounds
            {
                c1 = check_point;
                return p1_out_p2_in;
            }
        }
        c1 = p2;
        c2 = p2;
        return p1_out_p2_in;
     } // end p2 is in bounds
     //---------------------------------------------------------------------
     // both ends are out
    bool check_point_found = false;
    for(magnitude = 1.0; magnitude >= 0.0; magnitude -= quantum)
    {
        check_point = p1 + (diff * magnitude); // move from p2 back to p1
        if(LaserBoy_bounds_check(check_point, max, min) == LASERBOY_IN_BOUNDS) // check_point is in bounds
        {
            c2 = check_point;
            check_point_found = true;
            break;
        }

    }
    if(!check_point_found)
    {
        c1 = 0.0;
        c2 = 0.0;
        return no_line;
    }
    check_point_found = false;
    for(magnitude = 1.0; magnitude >= 0.0; magnitude -= quantum)
    {
        check_point = c2 - (diff * magnitude); // move from p1 to c1
        if(LaserBoy_bounds_check(check_point, max, min) == LASERBOY_IN_BOUNDS) // check_point is in bounds
        {
            c1 = check_point;
            check_point_found = true;
            return p1_out_p2_out;
        }
    }
    if(!check_point_found) // not possible, c1 is in bounds
    {
        c1 = 0.0;
        c2 = 0.0;
        return no_line;
    }
    // end both ends are out
    c1 = 0.0;
    c2 = 0.0;
    return no_line; // to make the compiler happy
}

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################

