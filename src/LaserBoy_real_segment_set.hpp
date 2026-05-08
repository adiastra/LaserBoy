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
// LaserBoy_real_segment_set.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_REAL_SEGMENT_SET_DEFINITIONS__
#define __LASERBOY_REAL_SEGMENT_SET_DEFINITIONS__

//############################################################################
#include "LaserBoy_real_segment.hpp"

//############################################################################
class LaserBoy_real_segment_set : public vector<LaserBoy_real_segment>
{
public:
    LaserBoy_real_segment_set()
        :    vector<LaserBoy_real_segment>(),
             been_used              (false),
             name                   (""),
             frame_index            (0),
             frame_select_start     (-1),
             frame_select_end       (-1),
             real_segment_set_error (LASERBOY_OK)
    {}
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(const LaserBoy_real_segment_set& rss)
        :    vector<LaserBoy_real_segment>(),
             been_used              (false),
             name                   (""),
             frame_index            (rss.frame_index),
             frame_select_start     (rss.frame_select_start),
             frame_select_end       (rss.frame_select_end),
             real_segment_set_error (LASERBOY_OK)
    {
        assign(rss.begin(), rss.end());
        shrink_to_fit();
    }
    //------------------------------------------------------------------------
   ~LaserBoy_real_segment_set()
    {}
    //------------------------------------------------------------------------
protected:
    LaserBoy_real_segment_set(const double      radius,
                              const double      rhodonea_numerator,
                              const double      rhodonea_denominator,
                              const double      start,
                              const double      duration,
                              const double      iterations,
                              const double      _radius,
                              const double      _rhodonea_numerator,
                              const double      _rhodonea_denominator,
                              const double      _start,
                              const double      _duration,
                              const double      _iterations,
                              const int         frames
                             );  // animated _rhodonea
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        radius,
                              const double        rhodonea_numerator,
                              const double        rhodonea_denominator,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _radius,
                              const double        _rhodonea_numerator,
                              const double        _rhodonea_denominator,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             );  // animated _oscillator_rhodonea
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(const double      fixed_radius,
                              const double      roller_radius,
                              const double      roller_offset,
                              const double      start,
                              const double      duration,
                              const double      iterations,
                              const double      _fixed_radius,
                              const double      _roller_radius,
                              const double      _roller_offset,
                              const double      _start,
                              const double      _duration,
                              const double      _iterations,
                              const int         frames,
                              const char        dud
                             ); // animated epitrochoids _epicycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        fixed_radius,
                              const double        roller_radius,
                              const double        roller_offset,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _fixed_radius,
                              const double        _roller_radius,
                              const double        _roller_offset,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud
                             ); // animated oscillator_epitrochoids _oscillator_epicycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(const double      fixed_radius,
                              const double      roller_radius,
                              const double      roller_offset,
                              const double      start,
                              const double      duration,
                              const double      iterations,
                              const double      _fixed_radius,
                              const double      _roller_radius,
                              const double      _roller_offset,
                              const double      _start,
                              const double      _duration,
                              const double      _iterations,
                              const int         frames,
                              const short       dud
                             ); // animated hypotrochoids _hypocycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        fixed_radius,
                              const double        roller_radius,
                              const double        roller_offset,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _fixed_radius,
                              const double        _roller_radius,
                              const double        _roller_offset,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const short         dud
                             ); // animated oscillator_hypotrochoids _oscillator_hypocycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO1_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO1_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             ); // animated _oscillator
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             ); // animated _oscillator_sum
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud
                             ); // animated _oscillator_xy
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             ); // animated _oscillator_xyz constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             ); // animated _harmonograph constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO5,
                              LaserBoy_oscillator LBO6,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              LaserBoy_oscillator LBO5_,
                              LaserBoy_oscillator LBO6_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO5,
                              LaserBoy_oscillator _LBO6,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              LaserBoy_oscillator _LBO5_,
                              LaserBoy_oscillator _LBO6_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames
                             ); // animated _harmonograph_xyz
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const short         dud
                             ); // animated _amplitude_mod
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud
                             ); // animated _amplitude_mod_xy
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO5,
                              LaserBoy_oscillator LBO6,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              LaserBoy_oscillator LBO5_,
                              LaserBoy_oscillator LBO6_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO5,
                              LaserBoy_oscillator _LBO6,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              LaserBoy_oscillator _LBO5_,
                              LaserBoy_oscillator _LBO6_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud
                             ); // animated _amplitude_mod_xyz
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const int           dud
                             ); // animated frequency_mod
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const short         dud
                             ); // animated frequency_mod_xy
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO5,
                              LaserBoy_oscillator LBO6,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              LaserBoy_oscillator LBO5_,
                              LaserBoy_oscillator LBO6_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO5,
                              LaserBoy_oscillator _LBO6,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              LaserBoy_oscillator _LBO5_,
                              LaserBoy_oscillator _LBO6_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const short         dud
                             ); // animated frequency_mod_xyz
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const float         dud
                             ); // animated phase_mod
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const int           dud
                             ); // animated phase_mod_xy
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO3,
                              LaserBoy_oscillator LBO4,
                              LaserBoy_oscillator LBO5,
                              LaserBoy_oscillator LBO6,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              LaserBoy_oscillator LBO3_,
                              LaserBoy_oscillator LBO4_,
                              LaserBoy_oscillator LBO5_,
                              LaserBoy_oscillator LBO6_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO3,
                              LaserBoy_oscillator _LBO4,
                              LaserBoy_oscillator _LBO5,
                              LaserBoy_oscillator _LBO6,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              LaserBoy_oscillator _LBO3_,
                              LaserBoy_oscillator _LBO4_,
                              LaserBoy_oscillator _LBO5_,
                              LaserBoy_oscillator _LBO6_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const int           dud
                             ); // animated phase_mod_xyz
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO1_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO1_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud
                             ); // animated _polar
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const char          dud1,
                              const char          dud2
                             ); // animated _polar_sum
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const short         dud1,
                              const char          dud2
                             ); // animated _polar_amplitude_mod
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const int           dud1,
                              const char          dud2
                             ); // animated _polar_frequency_mod
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                              LaserBoy_oscillator LBO2,
                              LaserBoy_oscillator LBO1_,
                              LaserBoy_oscillator LBO2_,
                              const double        start,
                              const double        duration,
                              const double        iterations,
                              LaserBoy_oscillator _LBO1,
                              LaserBoy_oscillator _LBO2,
                              LaserBoy_oscillator _LBO1_,
                              LaserBoy_oscillator _LBO2_,
                              const double        _start,
                              const double        _duration,
                              const double        _iterations,
                              const int           frames,
                              const float         dud1,
                              const char          dud2
                             ); // animated _polar_phase_mod
    //------------------------------------------------------------------------
    // named constructors
    //------------------------------------------------------------------------
public:
    static LaserBoy_real_segment_set _rhodonea(const double     radius,
                                               const double     rhodonea_numerator,
                                               const double     rhodonea_denominator,
                                               const double     start,
                                               const double     duration,
                                               const double     iterations,
                                               const double     _radius,
                                               const double     _rhodonea_numerator,
                                               const double     _rhodonea_denominator,
                                               const double     _start,
                                               const double     _duration,
                                               const double     _iterations,
                                               const int        frames
                                              )
    {
        return LaserBoy_real_segment_set(radius,
                                         rhodonea_numerator,
                                         rhodonea_denominator,
                                         start,
                                         duration,
                                         iterations,
                                         _radius,
                                         _rhodonea_numerator,
                                         _rhodonea_denominator,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_rhodonea(LaserBoy_oscillator LBO1,
                                                          LaserBoy_oscillator LBO2,
                                                          LaserBoy_oscillator LBO3,
                                                          LaserBoy_oscillator LBO4,
                                                          LaserBoy_oscillator LBO1_,
                                                          LaserBoy_oscillator LBO2_,
                                                          LaserBoy_oscillator LBO3_,
                                                          LaserBoy_oscillator LBO4_,
                                                          const double        radius,
                                                          const double        rhodonea_numerator,
                                                          const double        rhodonea_denominator,
                                                          const double        start,
                                                          const double        duration,
                                                          const double        iterations,
                                                          LaserBoy_oscillator _LBO1,
                                                          LaserBoy_oscillator _LBO2,
                                                          LaserBoy_oscillator _LBO3,
                                                          LaserBoy_oscillator _LBO4,
                                                          LaserBoy_oscillator _LBO1_,
                                                          LaserBoy_oscillator _LBO2_,
                                                          LaserBoy_oscillator _LBO3_,
                                                          LaserBoy_oscillator _LBO4_,
                                                          const double        _radius,
                                                          const double        _rhodonea_numerator,
                                                          const double        _rhodonea_denominator,
                                                          const double        _start,
                                                          const double        _duration,
                                                          const double        _iterations,
                                                          const int           frames
                                                         )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         radius,
                                         rhodonea_numerator,
                                         rhodonea_denominator,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _radius,
                                         _rhodonea_numerator,
                                         _rhodonea_denominator,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _epicycloid(const double     fixed_radius,
                                                 const double     roller_radius,
                                                 const double     roller_offset,
                                                 const double     start,
                                                 const double     duration,
                                                 const double     iterations,
                                                 const double     _fixed_radius,
                                                 const double     _roller_radius,
                                                 const double     _roller_offset,
                                                 const double     _start,
                                                 const double     _duration,
                                                 const double     _iterations,
                                                 const int        frames
                                                )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(fixed_radius,
                                         roller_radius,
                                         roller_offset,
                                         start,
                                         duration,
                                         iterations,
                                         _fixed_radius,
                                         _roller_radius,
                                         _roller_offset,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_epicycloid(LaserBoy_oscillator LBO1,
                                                            LaserBoy_oscillator LBO2,
                                                            LaserBoy_oscillator LBO3,
                                                            LaserBoy_oscillator LBO4,
                                                            LaserBoy_oscillator LBO1_,
                                                            LaserBoy_oscillator LBO2_,
                                                            LaserBoy_oscillator LBO3_,
                                                            LaserBoy_oscillator LBO4_,
                                                            const double        fixed_radius,
                                                            const double        roller_radius,
                                                            const double        roller_offset,
                                                            const double        start,
                                                            const double        duration,
                                                            const double        iterations,
                                                            LaserBoy_oscillator _LBO1,
                                                            LaserBoy_oscillator _LBO2,
                                                            LaserBoy_oscillator _LBO3,
                                                            LaserBoy_oscillator _LBO4,
                                                            LaserBoy_oscillator _LBO1_,
                                                            LaserBoy_oscillator _LBO2_,
                                                            LaserBoy_oscillator _LBO3_,
                                                            LaserBoy_oscillator _LBO4_,
                                                            const double        _fixed_radius,
                                                            const double        _roller_radius,
                                                            const double        _roller_offset,
                                                            const double        _start,
                                                            const double        _duration,
                                                            const double        _iterations,
                                                            const int           frames
                                                           )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         fixed_radius,
                                         roller_radius,
                                         roller_offset,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _fixed_radius,
                                         _roller_radius,
                                         _roller_offset,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _hypocycloid(const double     fixed_radius,
                                                  const double     roller_radius,
                                                  const double     roller_offset,
                                                  const double     start,
                                                  const double     duration,
                                                  const double     iterations,
                                                  const double     _fixed_radius,
                                                  const double     _roller_radius,
                                                  const double     _roller_offset,
                                                  const double     _start,
                                                  const double     _duration,
                                                  const double     _iterations,
                                                  const int        frames
                                                 )
    {
        short _dud = 0;
        return LaserBoy_real_segment_set(fixed_radius,
                                         roller_radius,
                                         roller_offset,
                                         start,
                                         duration,
                                         iterations,
                                         _fixed_radius,
                                         _roller_radius,
                                         _roller_offset,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_hypocycloid(LaserBoy_oscillator LBO1,
                                                             LaserBoy_oscillator LBO2,
                                                             LaserBoy_oscillator LBO3,
                                                             LaserBoy_oscillator LBO4,
                                                             LaserBoy_oscillator LBO1_,
                                                             LaserBoy_oscillator LBO2_,
                                                             LaserBoy_oscillator LBO3_,
                                                             LaserBoy_oscillator LBO4_,
                                                             const double       fixed_radius,
                                                             const double       roller_radius,
                                                             const double       roller_offset,
                                                             const double       start,
                                                             const double       duration,
                                                             const double       iterations,
                                                             LaserBoy_oscillator _LBO1,
                                                             LaserBoy_oscillator _LBO2,
                                                             LaserBoy_oscillator _LBO3,
                                                             LaserBoy_oscillator _LBO4,
                                                             LaserBoy_oscillator _LBO1_,
                                                             LaserBoy_oscillator _LBO2_,
                                                             LaserBoy_oscillator _LBO3_,
                                                             LaserBoy_oscillator _LBO4_,
                                                             const double       _fixed_radius,
                                                             const double       _roller_radius,
                                                             const double       _roller_offset,
                                                             const double       _start,
                                                             const double       _duration,
                                                             const double       _iterations,
                                                             const int          frames
                                                            )
    {
        short _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         fixed_radius,
                                         roller_radius,
                                         roller_offset,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _fixed_radius,
                                         _roller_radius,
                                         _roller_offset,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator(LaserBoy_oscillator LBO1,
                                                 LaserBoy_oscillator LBO1_,
                                                 const double        start,
                                                 const double        duration,
                                                 const double        iterations,
                                                 LaserBoy_oscillator _LBO1,
                                                 LaserBoy_oscillator _LBO1_,
                                                 const double        _start,
                                                 const double        _duration,
                                                 const double        _iterations,
                                                 const int           frames
                                                )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO1_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO1_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_sum(LaserBoy_oscillator LBO1,
                                                     LaserBoy_oscillator LBO2,
                                                     LaserBoy_oscillator LBO1_,
                                                     LaserBoy_oscillator LBO2_,
                                                     const double        start,
                                                     const double        duration,
                                                     const double        iterations,
                                                     LaserBoy_oscillator _LBO1,
                                                     LaserBoy_oscillator _LBO2,
                                                     LaserBoy_oscillator _LBO1_,
                                                     LaserBoy_oscillator _LBO2_,
                                                     const double        _start,
                                                     const double        _duration,
                                                     const double        _iterations,
                                                     const int           frames
                                                    )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_xy(LaserBoy_oscillator LBO1,
                                                    LaserBoy_oscillator LBO2,
                                                    LaserBoy_oscillator LBO1_,
                                                    LaserBoy_oscillator LBO2_,
                                                    const double        start,
                                                    const double        duration,
                                                    const double        iterations,
                                                    LaserBoy_oscillator _LBO1,
                                                    LaserBoy_oscillator _LBO2,
                                                    LaserBoy_oscillator _LBO1_,
                                                    LaserBoy_oscillator _LBO2_,
                                                    const double        _start,
                                                    const double        _duration,
                                                    const double        _iterations,
                                                    const int           frames
                                                   )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _oscillator_xyz(LaserBoy_oscillator LBO1,
                                                     LaserBoy_oscillator LBO2,
                                                     LaserBoy_oscillator LBO3,
                                                     LaserBoy_oscillator LBO1_,
                                                     LaserBoy_oscillator LBO2_,
                                                     LaserBoy_oscillator LBO3_,
                                                     const double        start,
                                                     const double        duration,
                                                     const double        iterations,
                                                     LaserBoy_oscillator _LBO1,
                                                     LaserBoy_oscillator _LBO2,
                                                     LaserBoy_oscillator _LBO3,
                                                     LaserBoy_oscillator _LBO1_,
                                                     LaserBoy_oscillator _LBO2_,
                                                     LaserBoy_oscillator _LBO3_,
                                                     const double        _start,
                                                     const double        _duration,
                                                     const double        _iterations,
                                                     const int           frames
                                                  )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _harmonograph(LaserBoy_oscillator LBO1,
                                                   LaserBoy_oscillator LBO2,
                                                   LaserBoy_oscillator LBO3,
                                                   LaserBoy_oscillator LBO4,
                                                   LaserBoy_oscillator LBO1_,
                                                   LaserBoy_oscillator LBO2_,
                                                   LaserBoy_oscillator LBO3_,
                                                   LaserBoy_oscillator LBO4_,
                                                   const double        start,
                                                   const double        duration,
                                                   const double        iterations,
                                                   LaserBoy_oscillator _LBO1,
                                                   LaserBoy_oscillator _LBO2,
                                                   LaserBoy_oscillator _LBO3,
                                                   LaserBoy_oscillator _LBO4,
                                                   LaserBoy_oscillator _LBO1_,
                                                   LaserBoy_oscillator _LBO2_,
                                                   LaserBoy_oscillator _LBO3_,
                                                   LaserBoy_oscillator _LBO4_,
                                                   const double        _start,
                                                   const double        _duration,
                                                   const double        _iterations,
                                                   const int           frames
                                                  )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _harmonograph_xyz(LaserBoy_oscillator LBO1,
                                                       LaserBoy_oscillator LBO2,
                                                       LaserBoy_oscillator LBO3,
                                                       LaserBoy_oscillator LBO4,
                                                       LaserBoy_oscillator LBO5,
                                                       LaserBoy_oscillator LBO6,
                                                       LaserBoy_oscillator LBO1_,
                                                       LaserBoy_oscillator LBO2_,
                                                       LaserBoy_oscillator LBO3_,
                                                       LaserBoy_oscillator LBO4_,
                                                       LaserBoy_oscillator LBO5_,
                                                       LaserBoy_oscillator LBO6_,
                                                       const double        start,
                                                       const double        duration,
                                                       const double        iterations,
                                                       LaserBoy_oscillator _LBO1,
                                                       LaserBoy_oscillator _LBO2,
                                                       LaserBoy_oscillator _LBO3,
                                                       LaserBoy_oscillator _LBO4,
                                                       LaserBoy_oscillator _LBO5,
                                                       LaserBoy_oscillator _LBO6,
                                                       LaserBoy_oscillator _LBO1_,
                                                       LaserBoy_oscillator _LBO2_,
                                                       LaserBoy_oscillator _LBO3_,
                                                       LaserBoy_oscillator _LBO4_,
                                                       LaserBoy_oscillator _LBO5_,
                                                       LaserBoy_oscillator _LBO6_,
                                                       const double        _start,
                                                       const double        _duration,
                                                       const double        _iterations,
                                                       const int           frames
                                                      )
    {
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO5,
                                         LBO6,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         LBO5_,
                                         LBO6_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO5,
                                         _LBO6,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _LBO5_,
                                         _LBO6_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _amplitude_mod(LaserBoy_oscillator LBO1,
                                                    LaserBoy_oscillator LBO2,
                                                    LaserBoy_oscillator LBO1_,
                                                    LaserBoy_oscillator LBO2_,
                                                    const double        start,
                                                    const double        duration,
                                                    const double        iterations,
                                                    LaserBoy_oscillator _LBO1,
                                                    LaserBoy_oscillator _LBO2,
                                                    LaserBoy_oscillator _LBO1_,
                                                    LaserBoy_oscillator _LBO2_,
                                                    const double        _start,
                                                    const double        _duration,
                                                    const double        _iterations,
                                                    const int           frames
                                                   )
    {
        short _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _amplitude_mod_xy(LaserBoy_oscillator LBO1,
                                                       LaserBoy_oscillator LBO2,
                                                       LaserBoy_oscillator LBO3,
                                                       LaserBoy_oscillator LBO4,
                                                       LaserBoy_oscillator LBO1_,
                                                       LaserBoy_oscillator LBO2_,
                                                       LaserBoy_oscillator LBO3_,
                                                       LaserBoy_oscillator LBO4_,
                                                       const double        start,
                                                       const double        duration,
                                                       const double        iterations,
                                                       LaserBoy_oscillator _LBO1,
                                                       LaserBoy_oscillator _LBO2,
                                                       LaserBoy_oscillator _LBO3,
                                                       LaserBoy_oscillator _LBO4,
                                                       LaserBoy_oscillator _LBO1_,
                                                       LaserBoy_oscillator _LBO2_,
                                                       LaserBoy_oscillator _LBO3_,
                                                       LaserBoy_oscillator _LBO4_,
                                                       const double        _start,
                                                       const double        _duration,
                                                       const double        _iterations,
                                                       const int           frames
                                                      )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _amplitude_mod_xyz(LaserBoy_oscillator LBO1,
                                                        LaserBoy_oscillator LBO2,
                                                        LaserBoy_oscillator LBO3,
                                                        LaserBoy_oscillator LBO4,
                                                        LaserBoy_oscillator LBO5,
                                                        LaserBoy_oscillator LBO6,
                                                        LaserBoy_oscillator LBO1_,
                                                        LaserBoy_oscillator LBO2_,
                                                        LaserBoy_oscillator LBO3_,
                                                        LaserBoy_oscillator LBO4_,
                                                        LaserBoy_oscillator LBO5_,
                                                        LaserBoy_oscillator LBO6_,
                                                        const double        start,
                                                        const double        duration,
                                                        const double        iterations,
                                                        LaserBoy_oscillator _LBO1,
                                                        LaserBoy_oscillator _LBO2,
                                                        LaserBoy_oscillator _LBO3,
                                                        LaserBoy_oscillator _LBO4,
                                                        LaserBoy_oscillator _LBO5,
                                                        LaserBoy_oscillator _LBO6,
                                                        LaserBoy_oscillator _LBO1_,
                                                        LaserBoy_oscillator _LBO2_,
                                                        LaserBoy_oscillator _LBO3_,
                                                        LaserBoy_oscillator _LBO4_,
                                                        LaserBoy_oscillator _LBO5_,
                                                        LaserBoy_oscillator _LBO6_,
                                                        const double        _start,
                                                        const double        _duration,
                                                        const double        _iterations,
                                                        const int           frames
                                                       )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO5,
                                         LBO6,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         LBO5_,
                                         LBO6_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO5,
                                         _LBO6,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _LBO5_,
                                         _LBO6_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _frequency_mod(LaserBoy_oscillator LBO1,
                                                    LaserBoy_oscillator LBO2,
                                                    LaserBoy_oscillator LBO1_,
                                                    LaserBoy_oscillator LBO2_,
                                                    const double        start,
                                                    const double        duration,
                                                    const double        iterations,
                                                    LaserBoy_oscillator _LBO1,
                                                    LaserBoy_oscillator _LBO2,
                                                    LaserBoy_oscillator _LBO1_,
                                                    LaserBoy_oscillator _LBO2_,
                                                    const double        _start,
                                                    const double        _duration,
                                                    const double        _iterations,
                                                    const int           frames
                                                   )
    {
        int _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _frequency_mod_xy(LaserBoy_oscillator LBO1,
                                                       LaserBoy_oscillator LBO2,
                                                       LaserBoy_oscillator LBO3,
                                                       LaserBoy_oscillator LBO4,
                                                       LaserBoy_oscillator LBO1_,
                                                       LaserBoy_oscillator LBO2_,
                                                       LaserBoy_oscillator LBO3_,
                                                       LaserBoy_oscillator LBO4_,
                                                       const double        start,
                                                       const double        duration,
                                                       const double        iterations,
                                                       LaserBoy_oscillator _LBO1,
                                                       LaserBoy_oscillator _LBO2,
                                                       LaserBoy_oscillator _LBO3,
                                                       LaserBoy_oscillator _LBO4,
                                                       LaserBoy_oscillator _LBO1_,
                                                       LaserBoy_oscillator _LBO2_,
                                                       LaserBoy_oscillator _LBO3_,
                                                       LaserBoy_oscillator _LBO4_,
                                                       const double        _start,
                                                       const double        _duration,
                                                       const double        _iterations,
                                                       const int           frames
                                                      )
    {
        short _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _frequency_mod_xyz(LaserBoy_oscillator LBO1,
                                                        LaserBoy_oscillator LBO2,
                                                        LaserBoy_oscillator LBO3,
                                                        LaserBoy_oscillator LBO4,
                                                        LaserBoy_oscillator LBO5,
                                                        LaserBoy_oscillator LBO6,
                                                        LaserBoy_oscillator LBO1_,
                                                        LaserBoy_oscillator LBO2_,
                                                        LaserBoy_oscillator LBO3_,
                                                        LaserBoy_oscillator LBO4_,
                                                        LaserBoy_oscillator LBO5_,
                                                        LaserBoy_oscillator LBO6_,
                                                        const double        start,
                                                        const double        duration,
                                                        const double        iterations,
                                                        LaserBoy_oscillator _LBO1,
                                                        LaserBoy_oscillator _LBO2,
                                                        LaserBoy_oscillator _LBO3,
                                                        LaserBoy_oscillator _LBO4,
                                                        LaserBoy_oscillator _LBO5,
                                                        LaserBoy_oscillator _LBO6,
                                                        LaserBoy_oscillator _LBO1_,
                                                        LaserBoy_oscillator _LBO2_,
                                                        LaserBoy_oscillator _LBO3_,
                                                        LaserBoy_oscillator _LBO4_,
                                                        LaserBoy_oscillator _LBO5_,
                                                        LaserBoy_oscillator _LBO6_,
                                                        const double        _start,
                                                        const double        _duration,
                                                        const double        _iterations,
                                                        const int           frames
                                                       )
    {
        short _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO5,
                                         LBO6,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         LBO5_,
                                         LBO6_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO5,
                                         _LBO6,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _LBO5_,
                                         _LBO6_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _phase_mod(LaserBoy_oscillator LBO1,
                                                LaserBoy_oscillator LBO2,
                                                LaserBoy_oscillator LBO1_,
                                                LaserBoy_oscillator LBO2_,
                                                const double        start,
                                                const double        duration,
                                                const double        iterations,
                                                LaserBoy_oscillator _LBO1,
                                                LaserBoy_oscillator _LBO2,
                                                LaserBoy_oscillator _LBO1_,
                                                LaserBoy_oscillator _LBO2_,
                                                const double        _start,
                                                const double        _duration,
                                                const double        _iterations,
                                                const int           frames
                                               )
    {
        float _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _phase_mod_xy(LaserBoy_oscillator LBO1,
                                                   LaserBoy_oscillator LBO2,
                                                   LaserBoy_oscillator LBO3,
                                                   LaserBoy_oscillator LBO4,
                                                   LaserBoy_oscillator LBO1_,
                                                   LaserBoy_oscillator LBO2_,
                                                   LaserBoy_oscillator LBO3_,
                                                   LaserBoy_oscillator LBO4_,
                                                   const double        start,
                                                   const double        duration,
                                                   const double        iterations,
                                                   LaserBoy_oscillator _LBO1,
                                                   LaserBoy_oscillator _LBO2,
                                                   LaserBoy_oscillator _LBO3,
                                                   LaserBoy_oscillator _LBO4,
                                                   LaserBoy_oscillator _LBO1_,
                                                   LaserBoy_oscillator _LBO2_,
                                                   LaserBoy_oscillator _LBO3_,
                                                   LaserBoy_oscillator _LBO4_,
                                                   const double        _start,
                                                   const double        _duration,
                                                   const double        _iterations,
                                                   const int           frames
                                                  )
    {
        int _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _phase_mod_xyz(LaserBoy_oscillator LBO1,
                                                    LaserBoy_oscillator LBO2,
                                                    LaserBoy_oscillator LBO3,
                                                    LaserBoy_oscillator LBO4,
                                                    LaserBoy_oscillator LBO5,
                                                    LaserBoy_oscillator LBO6,
                                                    LaserBoy_oscillator LBO1_,
                                                    LaserBoy_oscillator LBO2_,
                                                    LaserBoy_oscillator LBO3_,
                                                    LaserBoy_oscillator LBO4_,
                                                    LaserBoy_oscillator LBO5_,
                                                    LaserBoy_oscillator LBO6_,
                                                    const double        start,
                                                    const double        duration,
                                                    const double        iterations,
                                                    LaserBoy_oscillator _LBO1,
                                                    LaserBoy_oscillator _LBO2,
                                                    LaserBoy_oscillator _LBO3,
                                                    LaserBoy_oscillator _LBO4,
                                                    LaserBoy_oscillator _LBO5,
                                                    LaserBoy_oscillator _LBO6,
                                                    LaserBoy_oscillator _LBO1_,
                                                    LaserBoy_oscillator _LBO2_,
                                                    LaserBoy_oscillator _LBO3_,
                                                    LaserBoy_oscillator _LBO4_,
                                                    LaserBoy_oscillator _LBO5_,
                                                    LaserBoy_oscillator _LBO6_,
                                                    const double        _start,
                                                    const double        _duration,
                                                    const double        _iterations,
                                                    const int           frames
                                                   )
    {
        int _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO3,
                                         LBO4,
                                         LBO5,
                                         LBO6,
                                         LBO1_,
                                         LBO2_,
                                         LBO3_,
                                         LBO4_,
                                         LBO5_,
                                         LBO6_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO3,
                                         _LBO4,
                                         _LBO5,
                                         _LBO6,
                                         _LBO1_,
                                         _LBO2_,
                                         _LBO3_,
                                         _LBO4_,
                                         _LBO5_,
                                         _LBO6_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _polar(LaserBoy_oscillator LBO1,
                                            LaserBoy_oscillator LBO1_,
                                            const double        start,
                                            const double        duration,
                                            const double        iterations,
                                            LaserBoy_oscillator _LBO1,
                                            LaserBoy_oscillator _LBO1_,
                                            const double        _start,
                                            const double        _duration,
                                            const double        _iterations,
                                            const int           frames
                                           )
    {
        char _dud = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO1_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO1_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _polar_sum(LaserBoy_oscillator LBO1,
                                                LaserBoy_oscillator LBO2,
                                                LaserBoy_oscillator LBO1_,
                                                LaserBoy_oscillator LBO2_,
                                                const double        start,
                                                const double        duration,
                                                const double        iterations,
                                                LaserBoy_oscillator _LBO1,
                                                LaserBoy_oscillator _LBO2,
                                                LaserBoy_oscillator _LBO1_,
                                                LaserBoy_oscillator _LBO2_,
                                                const double        _start,
                                                const double        _duration,
                                                const double        _iterations,
                                                const int           frames
                                               )
    {
        char _dud1 = 0,
             _dud2 = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud1,
                                         _dud2
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _polar_amplitude_mod(LaserBoy_oscillator LBO1,
                                                          LaserBoy_oscillator LBO2,
                                                          LaserBoy_oscillator LBO1_,
                                                          LaserBoy_oscillator LBO2_,
                                                          const double        start,
                                                          const double        duration,
                                                          const double        iterations,
                                                          LaserBoy_oscillator _LBO1,
                                                          LaserBoy_oscillator _LBO2,
                                                          LaserBoy_oscillator _LBO1_,
                                                          LaserBoy_oscillator _LBO2_,
                                                          const double        _start,
                                                          const double        _duration,
                                                          const double        _iterations,
                                                          const int           frames
                                                         )
    {
        short _dud1 = 0;
        char  _dud2 = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud1,
                                         _dud2
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _polar_frequency_mod(LaserBoy_oscillator LBO1,
                                                          LaserBoy_oscillator LBO2,
                                                          LaserBoy_oscillator LBO1_,
                                                          LaserBoy_oscillator LBO2_,
                                                          const double        start,
                                                          const double        duration,
                                                          const double        iterations,
                                                          LaserBoy_oscillator _LBO1,
                                                          LaserBoy_oscillator _LBO2,
                                                          LaserBoy_oscillator _LBO1_,
                                                          LaserBoy_oscillator _LBO2_,
                                                          const double        _start,
                                                          const double        _duration,
                                                          const double        _iterations,
                                                          const int           frames
                                                         )
    {
        int  _dud1 = 0;
        char _dud2 = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud1,
                                         _dud2
                                        );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment_set _polar_phase_mod(LaserBoy_oscillator LBO1,
                                                      LaserBoy_oscillator LBO2,
                                                      LaserBoy_oscillator LBO1_,
                                                      LaserBoy_oscillator LBO2_,
                                                      const double        start,
                                                      const double        duration,
                                                      const double        iterations,
                                                      LaserBoy_oscillator _LBO1,
                                                      LaserBoy_oscillator _LBO2,
                                                      LaserBoy_oscillator _LBO1_,
                                                      LaserBoy_oscillator _LBO2_,
                                                      const double        _start,
                                                      const double        _duration,
                                                      const double        _iterations,
                                                      const int           frames
                                                     )
    {
        float _dud1 = 0;
        char  _dud2 = 0;
        return LaserBoy_real_segment_set(LBO1,
                                         LBO2,
                                         LBO1_,
                                         LBO2_,
                                         start,
                                         duration,
                                         iterations,
                                         _LBO1,
                                         _LBO2,
                                         _LBO1_,
                                         _LBO2_,
                                         _start,
                                         _duration,
                                         _iterations,
                                         frames,
                                         _dud1,
                                         _dud2
                                        );
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set& operator  = (const LaserBoy_real_segment_set& rss)
    {
        clear();
        frame_index        = rss.frame_index;
        frame_select_start = rss.frame_select_start;
        frame_select_end   = rss.frame_select_end;
        assign(rss.begin(), rss.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set& operator += (const LaserBoy_real_segment_set& rss)
    {
        insert(end(), rss.begin(), rss.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set  operator +  (const LaserBoy_real_segment_set& rss)
    {
        LaserBoy_real_segment_set sum(*this);
        sum.insert(sum.end(), rss.begin(), rss.end());
        sum.shrink_to_fit();
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set& operator += (const LaserBoy_real_segment& rs)
    {
        push_back(rs);
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set  operator +  (const LaserBoy_real_segment& rs)
    {
        LaserBoy_real_segment_set sum(*this);
        sum.push_back(rs);
        sum.shrink_to_fit();
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  repeat_front(int count)
    {
        LaserBoy_real_segment_set rss;
        for(int i = 1; i < count; i++)
            rss.push_back(front());
        rss += *this;
        *this = rss;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  repeat_back (int count)
    {
        for(int i = 1; i < count; i++)
            push_back(back());
        return *this;
    }
    //------------------------------------------------------------------------
    void reduce_lit_vectors()
    {
        for(size_t i = 0; i < size(); i++)
            at(i).reduce_lit_vectors();
    }
    //------------------------------------------------------------------------
    void reduce_blank_vectors()
    {
        for(size_t i = 0; i < size(); i++)
            at(i).reduce_blank_vectors();
    }
    //------------------------------------------------------------------------
    void omit_equivalent_vectors()
    {
        for(size_t i = 0; i < size(); i++)
            at(i).omit_equivalent_vectors();
    }
    //------------------------------------------------------------------------
    size_t number_of_frames()
    {
        return size();
    }
    //------------------------------------------------------------------------
    int number_of_selected_frames() const
    {
        int   selected_count = 0;
        for(int i = 0; i < (int)size(); i++)
            if(at(i).is_selected)
                selected_count++;
        return selected_count;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment       sum_of_frames()
    {
        LaserBoy_real_segment rs;
        for(size_t i = 0; i < size(); i++)
            rs += at(i);
        return rs;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment        collapse_frames         ();
    //------------------------------------------------------------------------
    LaserBoy_Error_Code          collapse_selected_frames();
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   move                 (LaserBoy_3D_double  displacement);
    LaserBoy_real_segment_set&  _move                 (LaserBoy_3D_double  displacement,
                                                       LaserBoy_3D_double _displacement,
                                                       double              acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   scale_around_origin  (LaserBoy_3D_double  scale);
    LaserBoy_real_segment_set&  _scale_around_origin  (LaserBoy_3D_double  scale_acceleration,
                                                       LaserBoy_3D_double _scale,
                                                       double              acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   clip                 ();
    LaserBoy_real_segment_set&   clip                 (LaserBoy_3D_double  P1,
                                                       LaserBoy_3D_double  P0,
                                                       double              granularity
                                                      );
    LaserBoy_real_segment_set&  _clip                 (LaserBoy_3D_double  P1,
                                                       LaserBoy_3D_double _P1,
                                                       LaserBoy_3D_double  P0,
                                                       LaserBoy_3D_double _P0,
                                                       double              granularity
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  scale_on_coordinates  (LaserBoy_3D_double  point,
                                                       LaserBoy_3D_double  scale
                                                      );
    LaserBoy_real_segment_set&  scale                 (LaserBoy_3D_double  scale);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  rotate_around_origin  (LaserBoy_3D_double  angles);
    LaserBoy_real_segment_set&  rotate_on_coordinates (LaserBoy_3D_double  fulcrum,
                                                       LaserBoy_3D_double  fulcrum_,
                                                       double              fulcrum_acceleration,
                                                       LaserBoy_3D_double  rotation
                                                      );
    LaserBoy_real_segment_set&  _rotate               (LaserBoy_3D_double  fulcrum,  //  from
                                                       LaserBoy_3D_double _fulcrum,  // _from
                                                       LaserBoy_3D_double  fulcrum_, //  to
                                                       LaserBoy_3D_double _fulcrum_, // _to
                                                       double              fulcrum_acceleration,
                                                       double             _fulcrum_acceleration,
                                                       LaserBoy_3D_double  rotation,
                                                       LaserBoy_3D_double _rotation //,
//                                                       double              rotate_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_3D_double dimensions                     (LaserBoy_3D_double& real_offset,
                                                       bool ignore_origin = true
                                                      )  const;
    LaserBoy_3D_double rectangular_center_of          () const;
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  center_x                      (bool individually  = false);
    LaserBoy_real_segment_set&  center_y                      (bool individually  = false);
    LaserBoy_real_segment_set&  center_z                      (bool individually  = false);
    LaserBoy_real_segment_set&  center                        (bool individually  = false);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  normalize                     (bool ignore_origin = true );
    LaserBoy_real_segment_set&  normalize_vectors             (bool ignore_origin = true );
    LaserBoy_real_segment_set&  normalize_vectors_with_origin (bool ignore_origin = true );
    LaserBoy_real_segment_set&  to_unit                       (bool ignore_origin = true );
    LaserBoy_real_segment_set&  to_unit_with_origin           (bool ignore_origin = true );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  reverse_frame_order   ();
    LaserBoy_real_segment_set&  reverse_vector_order  ();
    LaserBoy_real_segment_set&  swap_frames           (int i, int j);
    void                        delete_selected_frames();
    void                        strip_color_rgb       (const LaserBoy_color& c);
    void                        index_hues            (int index_multiple, int hues_shift);
    void                        span_hues             (double span_factor, int hues_shift);
    void                        index_hues_set        (int index_multiple);
    void                        span_hues_set         (double span_factor);
    void                        sync_rgb_and_palette  ();
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   move_                (LaserBoy_3D_double  displacement,
                                                       double              move_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  _move_                (LaserBoy_3D_double  displacement,
                                                       LaserBoy_3D_double _displacement,
                                                       double              move_acceleration,
                                                       double             _move_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   scale_               (LaserBoy_3D_double  factor,
                                                       LaserBoy_3D_double  factor_,
                                                       double              scale_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  _scale_               (LaserBoy_3D_double  factor,  //  from
                                                       LaserBoy_3D_double _factor,  // _from
                                                       LaserBoy_3D_double  factor_, //  to
                                                       LaserBoy_3D_double _factor_, // _to
                                                       double              scale_acceleration,
                                                       double             _scale_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   rotate_              (LaserBoy_3D_double  fulcrum,
                                                       LaserBoy_3D_double  fulcrum_,
                                                       double              fulcrum_acceleration,
                                                       LaserBoy_3D_double  angles,
                                                       double              rotate_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  _rotate_              (LaserBoy_3D_double  fulcrum,  //  from
                                                       LaserBoy_3D_double _fulcrum,  // _from
                                                       LaserBoy_3D_double  fulcrum_, //  to
                                                       LaserBoy_3D_double _fulcrum_, // _to
                                                       double              fulcrum_acceleration,
                                                       double             _fulcrum_acceleration,
                                                       LaserBoy_3D_double  angles,
                                                       LaserBoy_3D_double _angles,
                                                       double              rotate_acceleration,
                                                       double             _rotate_acceleration
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   composite            (LaserBoy_real_segment_set  to_unite);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   add                  (LaserBoy_real_segment      displacements);
    LaserBoy_real_segment_set&  _add                  (LaserBoy_real_segment_set  rss_displacements);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   multiply             (LaserBoy_real_segment      factors);
    LaserBoy_real_segment_set&  _multiply             (LaserBoy_real_segment_set  rss_factors);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  warp                  (LaserBoy_3D_double    fulcrum,
                                                       LaserBoy_3D_double    fulcrum_,
                                                       double                fulcrum_acceleration,
                                                       LaserBoy_real_segment rs_rotations
                                                      );
    LaserBoy_real_segment_set&  _warp                 (LaserBoy_3D_double          fulcrum,
                                                       LaserBoy_3D_double          fulcrum_,
                                                       LaserBoy_3D_double         _fulcrum,
                                                       LaserBoy_3D_double         _fulcrum_,
                                                       double                      fulcrum_acceleration,
                                                       double                     _fulcrum_acceleration,
                                                       LaserBoy_real_segment_set  rss_rotations
                                                      );
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&   xyz_rgb              (LaserBoy_real_segment      rs_xyz_rgb);
    LaserBoy_real_segment_set&  _xyz_rgb              (LaserBoy_real_segment_set  rss_xyz_rgbs);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set&  to_polar             ();
    LaserBoy_real_segment_set&  differences          ();
    LaserBoy_real_segment_set&  sums                 ();
    LaserBoy_real_segment_set&  to_rectangular       ();
    //------------------------------------------------------------------------
    LaserBoy_Error_Code         from_txt_file         (const string& file,
                                                       bool          append = false
                                                      );
    bool                        save_as_txt           (const string   &file);
    void                        to_ofstream_txt       (std::ofstream  &out);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code         from_dxf_directory    (const string&  dir);
    bool                        save_as_dxf_directory (const string&  dir,
                                                       const string&  name,
                                                       bool           true_color_dxf
                                                      ) const;
    //------------------------------------------------------------------------
    bool                        save_as_bmp_directory (const string&  dir,
                                                       const string&  name,
                                                       u_int          x_res,
                                                       u_int          y_res,
                                                       bool           norm_frames_individually,
                                                       u_int          rendered_line_width,
                                                       bool           bmp_render_vectors,
                                                       bool           bmp_render_vertices
                                                      ) const;
    //------------------------------------------------------------------------
    bool                 been_used             ;
    string               name                  ;
    u_int                frame_index           ;
    int                  frame_select_start    ,
                         frame_select_end      ;
    LaserBoy_Error_Code  real_segment_set_error;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
