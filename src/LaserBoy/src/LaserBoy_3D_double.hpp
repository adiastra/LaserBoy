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
// LaserBoy_3D_double.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_3D_DOUBLE_DEFINITIONS__
#define __LASERBOY_3D_DOUBLE_DEFINITIONS__

//############################################################################
#include "LaserBoy_3D_short.hpp"

//############################################################################
class LaserBoy_3D_double
{
public:
    LaserBoy_3D_double(const double& f = 0)
               : x(f)
               , y(f)
               , z(f)
    {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const double& _x,
                       const double& _y,
                       const double& _z
                      )
               : x(_x)
               , y(_y)
               , z(_z)
    {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const LaserBoy_3D_double& f)
               : x(f.x)
               , y(f.y)
               , z(f.z)
    {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double(const LaserBoy_3D_short& s)
               : x(s.x)
               , y(s.y)
               , z(s.z)
    {}
    //------------------------------------------------------------------------
   ~LaserBoy_3D_double()
    {}
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator = (const LaserBoy_3D_double& f)
    {
        x = f.x;
        y = f.y;
        z = f.z;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator = (const double& f)
    {
        x = y = z = f;
        return *this;
    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_3D_double& f) const
    {
        if(    (x == f.x)
            && (y == f.y)
            && (z == f.z)
          )
            return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool operator != (const LaserBoy_3D_double& f) const
    {
        if(    (x == f.x)
            && (y == f.y)
            && (z == f.z)
          )
            return false;
        return true;
    }
    //------------------------------------------------------------------------
    bool operator == (const double& f) const
    {
        if(    (x == f)
            && (y == f)
            && (z == f)
          )
            return true;
        return false;
    }
    //------------------------------------------------------------------------
    bool operator != (const double& f) const
    {
        if(    (x == f)
            && (y == f)
            && (z == f)
          )
            return false;
        return true;
    }
    //------------------------------------------------------------------------
    bool operator > (const LaserBoy_3D_double& f) const // distance from the origin
    {
        return(   (  x *   x +   y *   y +   z *   z)
                > (f.x * f.x + f.y * f.y + f.z * f.z)
              );
    }
    //------------------------------------------------------------------------
    bool operator >= (const LaserBoy_3D_double& f) const // distance from the origin
    {
        return(    (  x *   x +   y *   y +   z *   z)
                >= (f.x * f.x + f.y * f.y + f.z * f.z)
              );
    }
    //------------------------------------------------------------------------
    bool operator < (const LaserBoy_3D_double& f) const // distance from the origin
    {
        return(   (  x *   x +   y *   y +   z *   z)
                < (f.x * f.x + f.y * f.y + f.z * f.z)
              );
    }
    //------------------------------------------------------------------------
    bool operator <= (const LaserBoy_3D_double& f) const // distance from the origin
    {
        return(    (  x *   x +   y *   y +   z *   z)
                <= (f.x * f.x + f.y * f.y + f.z * f.z)
              );
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator -  (const LaserBoy_3D_double& f) const
    {
        return LaserBoy_3D_double(x - f.x, y - f.y, z - f.z);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator -= (const LaserBoy_3D_double& f)
    {
        x -= f.x;
        y -= f.y;
        z -= f.z;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator -= (const double& f)
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator +  (const LaserBoy_3D_double& f) const
    {
        return LaserBoy_3D_double(x + f.x,
                                  y + f.y,
                                  z + f.z
                                 );
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator +  (const LaserBoy_3D_short& s) const
    {
        return LaserBoy_3D_double(x + s.x,
                                  y + s.y,
                                  z + s.z
                                 );
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator += (const LaserBoy_3D_double& f)
    {
        x += f.x;
        y += f.y;
        z += f.z;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator += (const double& f)
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator *  (const LaserBoy_3D_double& f) const
    {
        return LaserBoy_3D_double(x * f.x, y * f.y, z * f.z);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const LaserBoy_3D_double& f) const
    {
        LaserBoy_3D_double quotient;
        if(f.x)
            quotient.x /= f.x;
        else
            quotient.x = NAN;
        if(f.y)
            quotient.y /= f.y;
        else
            quotient.y = NAN;
        if(f.z)
            quotient.z /= f.z;
        else
            quotient.z = NAN;
        return quotient;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator *  (const double& f) const
    {
        return LaserBoy_3D_double(x * f, y * f, z * f);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const double& f) const
    {
        if(f)
            return LaserBoy_3D_double(x / f, y / f, z / f);
        return LaserBoy_3D_double(NAN, NAN, NAN);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double  operator /  (const int& i) const
    {
        if(i)
            return LaserBoy_3D_double(x / i, y / i, z / i);
        return LaserBoy_3D_double(NAN, NAN, NAN);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator *= (const LaserBoy_3D_double& f)
    {
        x *= f.x;
        y *= f.y;
        z *= f.z;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator *= (const double& f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator /= (const LaserBoy_3D_double& f)
    {
        if(f.x)
            x /= f.x;
        else
            x = NAN;
        if(f.y)
            y /= f.y;
        else
            y = NAN;
        if(f.z)
            z /= f.z;
        else
            z = NAN;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double& operator /= (const double& f)
    {
        if(f != 0.0)
        {
            x /= f;
            y /= f;
            z /= f;
        }
        else
            *this = LaserBoy_3D_double(NAN, NAN, NAN);
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double operator - ()
    {
        LaserBoy_3D_double negative;
        negative.x = -x;
        negative.y = -y;
        negative.z = -z;
        return negative;
    }
    //------------------------------------------------------------------------
    double operator | (const LaserBoy_3D_double& f) const // distance from each other
    {
        return( sqrt(   (double)(x - f.x) * (x - f.x)
                      + (double)(y - f.y) * (y - f.y)
                      + (double)(z - f.z) * (z - f.z)
                    )
              );
    }
    //------------------------------------------------------------------------
    bool is_NAN()
    {
        if(isnan(x)) return true;
        if(isnan(y)) return true;
        if(isnan(z)) return true;
        return false;
    }
    //------------------------------------------------------------------------
    friend ostream& operator << (ostream &out, const LaserBoy_3D_double& d)
    {
        ios::fmtflags old_settings = cout.flags(); //save previous format flags
        int old_precision = cout.precision(); // save previous precision setting
        cout << setprecision(3);
        cout << "x=";
        cout << std::right << fixed << setw(10) << d.x;
        cout << " : y=";
        cout << std::right << fixed << setw(10) << d.y;
        cout << " : z=";
        cout << std::right << fixed << setw(10) << d.z;
        cout.flags(old_settings);
        cout.precision(old_precision);
        return out;
    }
    //------------------------------------------------------------------------
    operator LaserBoy_3D_short() const // cast to LaserBoy_3D_short
    {
        return LaserBoy_3D_short( (short)round(x),
                                  (short)round(y),
                                  (short)round(z)
                                );
    }
    //------------------------------------------------------------------------
    double sum()
    {
        return x + y + z;
    }
    //------------------------------------------------------------------------
    double magnitude() const // distance from origin
    {
        LaserBoy_3D_double origin;
        return(origin | *this);
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double direction() const // z and y rotational angle from origin
    {
        LaserBoy_3D_double origin,
                           basline_vertex,
                           copy = *this,
                           rotation;
        //............................................................
        basline_vertex.x = 1.0;
        copy.z           = 0.0;
        rotation.z       = (double)delta_angle(  copy | basline_vertex
                                               , 1.0
                                               , copy | origin
                                              );
        if(rotation.y < 0) rotation.z  = two_pi - rotation.z;
        //............................................................
        copy             =  *this;
        copy.y           = 0.0;
        rotation.y       = (double)delta_angle(  copy | basline_vertex
                                               , 1.0
                                               , copy | origin
                                              );
        if(rotation.z < 0) rotation.y  = two_pi - rotation.y;
        //............................................................
        return rotation;
    }
    //------------------------------------------------------------------------
    double x, y, z;
};

//############################################################################
class LaserBoy_4D_double // 3D coordinate plus rotational angle from origin
{
public:
    LaserBoy_4D_double()
               : x(0.0)
               , y(0.0)
               , z(0.0)
               , r(0.0)
    {}
    //------------------------------------------------------------------------
    LaserBoy_4D_double(const LaserBoy_3D_double& f, const double& _r)
               : x(f.x)
               , y(f.y)
               , z(f.z)
               , r(_r)
    {}
    //------------------------------------------------------------------------
   ~LaserBoy_4D_double() {}
    //------------------------------------------------------------------------
    double x,
           y,
           z,
           r;
};

//############################################################################
inline bool least_rotation(const LaserBoy_4D_double p1, // for std::sort LaserBoy_4D_double_vector
                           const LaserBoy_4D_double p2
                          )
{
    return p1.r < p2.r; // least rotational angle
}

//############################################################################
class LaserBoy_4D_double_vector : public vector<LaserBoy_4D_double> // 3D coordinates plus rotational angle from origin
{
public:
    LaserBoy_4D_double_vector() {}
    //------------------------------------------------------------------------
   ~LaserBoy_4D_double_vector() {}
    //------------------------------------------------------------------------
    LaserBoy_4D_double_vector& sort()
    {
        std::sort(begin(), end(), least_rotation);
        return *this;
    }
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
