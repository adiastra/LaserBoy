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
// LaserBoy_real_segment.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_REAL_SEGMENT_DEFINITIONS__
#define __LASERBOY_REAL_SEGMENT_DEFINITIONS__

//############################################################################
#include "LaserBoy_utility.hpp"
#include "LaserBoy_oscillator.hpp"
#include "LaserBoy_palette.hpp"

//############################################################################
class LaserBoy_real_segment : public vector<LaserBoy_real_vertex>
{
public:
    LaserBoy_real_segment()
                : vector<LaserBoy_real_vertex>(),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK          )
    {}
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const int& palette_index,
                          bool       add_origin
                         )
                : vector<LaserBoy_real_vertex>(),
                  palette_index               (palette_index),
                  real_segment_error          (LASERBOY_OK  )
    {
        if(add_origin)
        {
            push_back(LaserBoy_real_vertex());
            push_back(LaserBoy_real_vertex());
        }
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const LaserBoy_real_segment& rs)
                : vector<LaserBoy_real_vertex>(),
                  palette_index               (rs.palette_index),
                  real_segment_error          (LASERBOY_OK     )
    {
        clear();
        if(rs.size() > 1)
        {
            reserve(rs.size());
            insert(begin(), rs.begin(), rs.end());
        }
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_real_vertex from,
                          LaserBoy_real_vertex to
                         ); // 3D line function
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_real_vertex from,
                          LaserBoy_real_vertex to,
                          const u_int&         max_d
                         ); // 3D line function
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const LaserBoy_3D_double& p,
                          const LaserBoy_color      c
                         ); // 3D cross function
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_frame_set&  font_frames,
                          const u32string&     character_list,
                          const string&        text,
                          const double         char_width,
                          const double         mono_font_space,
                          const double         super_diacritic_gap,
                          const bool           bond_word = false,
                          const bool           bond_line = false
                         );   // mono spaced font constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_frame_set&  font_frames,
                          const u32string&     character_list,
                          const string&        text,
                          const double         char_width,
                          const double         vari_font_gap,
                          const double         vari_font_space,
                          const double         super_diacritic_gap,
                          const bool           bond_word = false,
                          const bool           bond_line = false
                         );   // variable spaced font constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(char32_t unichar); // uncicode not found frame
    //------------------------------------------------------------------------
   ~LaserBoy_real_segment()
    {}
    //------------------------------------------------------------------------
protected:
    LaserBoy_real_segment(const double     radius,
                          const double     rhodonea_numerator,
                          const double     rhodonea_denominator,
                          const double     start,
                          const double     duration,
                          const double     iterations
                         ); // rhodonea
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const double        iterations
                         ); // oscillator_rhodonea
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const double       fixed_radius,
                          const double       roller_radius,
                          const double       roller_offset,
                          const double       start,
                          const double       duration,
                          const double       iterations,
                          const char         dud
                         ); // epitrochoid epicycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const char          dud
                         ); // oscillator_epitrochoid oscillator_epicycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment(const double       fixed_radius,
                          const double       roller_radius,
                          const double       roller_offset,
                          const double       start,
                          const double       duration,
                          const double       iterations,
                          const short        dud
                         ); // hypotrochoid hypocycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const short         dud
                         ); // oscillator_hypotrochoid oscillator_hypocycloid
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO1_,
                          const double        start,
                          const double        duration,
                          const double        iterations
                         ); // oscillator constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations
                         ); // oscillator_sum constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const char          dud
                         ); // oscillator_xy constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO3,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          LaserBoy_oscillator LBO3_,
                          const double        start,
                          const double        duration,
                          const double        iterations
                         ); // oscillator_xyz constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO3,
                          LaserBoy_oscillator LBO4,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          LaserBoy_oscillator LBO3_,
                          LaserBoy_oscillator LBO4_,
                          const double        start,
                          const double        duration,
                          const double        iterations
                         ); // harmonograph constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const double        iterations
                         ); // harmonograph_3D constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const short         dud
                         ); // amplitude_mod constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const char          dud
                         ); // amplitude_mod_xy constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const char          dud
                         ); // amplitude_mod_xyz constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const int           dud
                         ); // frequency_mod constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const short         dud
                         ); // frequency_mod_xy constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const short         dud
                         ); // frequency_mod_xyz constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const float         dud
                         ); // phase_mod constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const int           dud
                         ); // phase_mod_xy constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                          const int           dud
                         ); // phase_mod_xyz constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO1_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const char          dud
                         ); // polar constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const char          dud1,
                          const char          dud2
                         ); // polar_sum constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const short         dud1,
                          const char          dud2
                         ); // polar_amplitude_mod constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const int           dud1,
                          const char          dud2
                         ); // polar_frequency_mod constructor
    //------------------------------------------------------------------------
    LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                          LaserBoy_oscillator LBO2,
                          LaserBoy_oscillator LBO1_,
                          LaserBoy_oscillator LBO2_,
                          const double        start,
                          const double        duration,
                          const double        iterations,
                          const float         dud1,
                          const char          dud2
                         ); // polar_phase_mod constructor
    //------------------------------------------------------------------------
public:
    //------------------------------------------------------------------------
    // named constructors
    //------------------------------------------------------------------------
    static LaserBoy_real_segment rhodonea(const double     radius,
                                          const double     rhodonea_numerator,
                                          const double     rhodonea_denominator,
                                          const double     start,
                                          const double     duration,
                                          const double     iterations
                                         ) // uddddd
    {
        return LaserBoy_real_segment(radius,
                                     rhodonea_numerator,
                                     rhodonea_denominator,
                                     start,
                                     duration,
                                     iterations
                                    );
    }
  //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_rhodonea(LaserBoy_oscillator LBO1,
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
                                                     const double        iterations
                                                    )
    {
        return LaserBoy_real_segment(LBO1,
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
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment epicycloid(const double     fixed_radius,
                                            const double     roller_radius,
                                            const double     roller_offset,
                                            const double     start,
                                            const double     duration,
                                            const double     iterations
                                           )
    {
        char _dud = 0;
        return LaserBoy_real_segment(fixed_radius,
                                     roller_radius,
                                     roller_offset,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_epicycloid(LaserBoy_oscillator LBO1,
                                                       LaserBoy_oscillator LBO2,
                                                       LaserBoy_oscillator LBO3,
                                                       LaserBoy_oscillator LBO4,
                                                       LaserBoy_oscillator LBO1_,
                                                       LaserBoy_oscillator LBO2_,
                                                       LaserBoy_oscillator LBO3_,
                                                       LaserBoy_oscillator LBO4_,
                                                       const double     fixed_radius,
                                                       const double     roller_radius,
                                                       const double     roller_offset,
                                                       const double     start,
                                                       const double     duration,
                                                       const double     iterations
                                                      )
    {
        char _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment hypocycloid(const double     fixed_radius,
                                             const double     roller_radius,
                                             const double     roller_offset,
                                             const double     start,
                                             const double     duration,
                                             const double     iterations
                                            )
    {
        short _dud = 0;
        return LaserBoy_real_segment(fixed_radius,
                                     roller_radius,
                                     roller_offset,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_hypocycloid(LaserBoy_oscillator LBO1,
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
                                                        const double        iterations
                                                       )
    {
        short _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator(LaserBoy_oscillator LBO1,
                                            LaserBoy_oscillator LBO1_,
                                            const double        start,
                                            const double        duration,
                                            const double        iterations
                                           )
    {
        return LaserBoy_real_segment(LBO1,
                                     LBO1_,
                                     start,
                                     duration,
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_sum(LaserBoy_oscillator LBO1,
                                                LaserBoy_oscillator LBO2,
                                                LaserBoy_oscillator LBO1_,
                                                LaserBoy_oscillator LBO2_,
                                                const double        start,
                                                const double        duration,
                                                const double        iterations
                                               )
    {
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_xy(LaserBoy_oscillator LBO1,
                                               LaserBoy_oscillator LBO2,
                                               LaserBoy_oscillator LBO1_,
                                               LaserBoy_oscillator LBO2_,
                                               const double        start,
                                               const double        duration,
                                               const double        iterations
                                              )
    {
        char _dud = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment oscillator_xyz(LaserBoy_oscillator LBO1,
                                                LaserBoy_oscillator LBO2,
                                                LaserBoy_oscillator LBO3,
                                                LaserBoy_oscillator LBO1_,
                                                LaserBoy_oscillator LBO2_,
                                                LaserBoy_oscillator LBO3_,
                                                const double        start,
                                                const double        duration,
                                                const double        iterations
                                               )
    {
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO3,
                                     LBO1_,
                                     LBO2_,
                                     LBO3_,
                                     start,
                                     duration,
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment harmonograph(LaserBoy_oscillator LBO1,
                                              LaserBoy_oscillator LBO2,
                                              LaserBoy_oscillator LBO3,
                                              LaserBoy_oscillator LBO4,
                                              LaserBoy_oscillator LBO1_,
                                              LaserBoy_oscillator LBO2_,
                                              LaserBoy_oscillator LBO3_,
                                              LaserBoy_oscillator LBO4_,
                                              const double        start,
                                              const double        duration,
                                              const double        iterations
                                             )
    {
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO3,
                                     LBO4,
                                     LBO1_,
                                     LBO2_,
                                     LBO3_,
                                     LBO4_,
                                     start,
                                     duration,
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment harmonograph_xyz(LaserBoy_oscillator LBO1,
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
                                                  const double        iterations
                                                 )
    {
        return LaserBoy_real_segment(LBO1,
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
                                     iterations
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment amplitude_mod(LaserBoy_oscillator LBO1,
                                               LaserBoy_oscillator LBO2,
                                               LaserBoy_oscillator LBO1_,
                                               LaserBoy_oscillator LBO2_,
                                               const double        start,
                                               const double        duration,
                                               const double        iterations
                                              )
    {
        short _dud = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment amplitude_mod_xy(LaserBoy_oscillator LBO1,
                                                  LaserBoy_oscillator LBO2,
                                                  LaserBoy_oscillator LBO3,
                                                  LaserBoy_oscillator LBO4,
                                                  LaserBoy_oscillator LBO1_,
                                                  LaserBoy_oscillator LBO2_,
                                                  LaserBoy_oscillator LBO3_,
                                                  LaserBoy_oscillator LBO4_,
                                                  const double        start,
                                                  const double        duration,
                                                  const double        iterations
                                                 )
    {
        char _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment amplitude_mod_xyz(LaserBoy_oscillator LBO1,
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
                                                   const double        iterations
                                                  )
    {
        char _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment frequency_mod(LaserBoy_oscillator LBO1,
                                               LaserBoy_oscillator LBO2,
                                               LaserBoy_oscillator LBO1_,
                                               LaserBoy_oscillator LBO2_,
                                               const double        start,
                                               const double        duration,
                                               const double        iterations
                                              )
    {
        int _dud = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment frequency_mod_xy(LaserBoy_oscillator LBO1,
                                                  LaserBoy_oscillator LBO2,
                                                  LaserBoy_oscillator LBO3,
                                                  LaserBoy_oscillator LBO4,
                                                  LaserBoy_oscillator LBO1_,
                                                  LaserBoy_oscillator LBO2_,
                                                  LaserBoy_oscillator LBO3_,
                                                  LaserBoy_oscillator LBO4_,
                                                  const double        start,
                                                  const double        duration,
                                                  const double        iterations
                                                 )
    {
        short _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment frequency_mod_xyz(LaserBoy_oscillator LBO1,
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
                                                   const double        iterations
                                                  )
    {
        short _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment phase_mod(LaserBoy_oscillator LBO1,
                                           LaserBoy_oscillator LBO2,
                                           LaserBoy_oscillator LBO1_,
                                           LaserBoy_oscillator LBO2_,
                                           const double        start,
                                           const double        duration,
                                           const double        iterations
                                          )
    {
        float _dud = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment phase_mod_xy(LaserBoy_oscillator LBO1,
                                              LaserBoy_oscillator LBO2,
                                              LaserBoy_oscillator LBO3,
                                              LaserBoy_oscillator LBO4,
                                              LaserBoy_oscillator LBO1_,
                                              LaserBoy_oscillator LBO2_,
                                              LaserBoy_oscillator LBO3_,
                                              LaserBoy_oscillator LBO4_,
                                              const double        start,
                                              const double        duration,
                                              const double        iterations
                                             )
    {
        int _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment phase_mod_xyz(LaserBoy_oscillator LBO1,
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
                                               const double        iterations
                                              )
    {
        int _dud = 0;
        return LaserBoy_real_segment(LBO1,
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
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar(LaserBoy_oscillator LBO1,
                                       LaserBoy_oscillator LBO1_,
                                       const double        start,
                                       const double        duration,
                                       const double        iterations
                                      )
    {
        char _dud = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO1_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_sum(LaserBoy_oscillator LBO1,
                                           LaserBoy_oscillator LBO2,
                                           LaserBoy_oscillator LBO1_,
                                           LaserBoy_oscillator LBO2_,
                                           const double        start,
                                           const double        duration,
                                           const double        iterations
                                          )
    {
        char _dud1 = 0,
             _dud2 = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud1,
                                     _dud2
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_amplitude_mod(LaserBoy_oscillator LBO1,
                                                     LaserBoy_oscillator LBO2,
                                                     LaserBoy_oscillator LBO1_,
                                                     LaserBoy_oscillator LBO2_,
                                                     const double        start,
                                                     const double        duration,
                                                     const double        iterations
                                                    )
    {
        short _dud1 = 0;
        char  _dud2 = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud1,
                                     _dud2
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_frequency_mod(LaserBoy_oscillator LBO1,
                                                     LaserBoy_oscillator LBO2,
                                                     LaserBoy_oscillator LBO1_,
                                                     LaserBoy_oscillator LBO2_,
                                                     const double        start,
                                                     const double        duration,
                                                     const double        iterations
                                                    )
    {
        int   _dud1 = 0;
        char  _dud2 = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud1,
                                     _dud2
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_phase_mod(LaserBoy_oscillator LBO1,
                                                 LaserBoy_oscillator LBO2,
                                                 LaserBoy_oscillator LBO1_,
                                                 LaserBoy_oscillator LBO2_,
                                                 const double        start,
                                                 const double        duration,
                                                 const double        iterations
                                                )
    {
        float _dud1 = 0;
        char  _dud2 = 0;
        return LaserBoy_real_segment(LBO1,
                                     LBO2,
                                     LBO1_,
                                     LBO2_,
                                     start,
                                     duration,
                                     iterations,
                                     _dud1,
                                     _dud2
                                    );
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment unit_reference()
    {
        double                arc_step = two_pi / 60;
        LaserBoy_3D_double    double_3D;
        LaserBoy_real_segment rs;
        rs.push_back(LaserBoy_real_vertex( 0.0,  0.0,  0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex( 0.0,  0.0,  0.0, 255, 255, 255,                     0, 55));
        rs.push_back(LaserBoy_real_vertex(-1.0,  0.0,  0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex( 1.0,  0.0,  0.0, 255, 255, 255,                     0, 55));
        rs.push_back(LaserBoy_real_vertex( 0.0, -1.0,  0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex( 0.0,  1.0,  0.0, 255, 255, 255,                     0, 55));
        rs.push_back(LaserBoy_real_vertex( 0.0,  0.0, -1.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex( 0.0,  0.0,  1.0, 255, 255, 255,                     0, 55));
        double_3D.x   = cos(0);
        double_3D.y   = sin(0);
        double_3D.z   = 0.0;
        rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
        for(int a = 1; a <= 60; a++)
        {
            double_3D.x = cos(a * arc_step);
            double_3D.y = sin(a * arc_step);
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
        }
        double_3D.x   = cos(0);
        double_3D.y   = 0.0;
        double_3D.z   = sin(0);
        rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
        for(int a = 1; a <= 60; a++)
        {
            double_3D.x = cos(a * arc_step);
            double_3D.z = sin(a * arc_step);
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
        }
        double_3D.x   = 0.0;
        double_3D.y   = sin(0);
        double_3D.z   = cos(0);
        rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
        for(int a = 1; a <= 60; a++)
        {
            double_3D.y = sin(a * arc_step);
            double_3D.z = cos(a * arc_step);
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
        }
        rs.convert_blank_to_black();
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment line(LaserBoy_3D_double P0, LaserBoy_3D_double P1)
    {
        LaserBoy_real_segment rs;
        rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P1.z, 255, 255, 255,                     0, 55));
        rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P1.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment rectangular_grid_xy(LaserBoy_3D_double P0,
                                                     LaserBoy_3D_double P1,
                                                     double             x_units,
                                                     double             y_units
                                                    )
    {
        LaserBoy_real_segment rs;
        if(P0.x != P1.x && P0.y != P1.y && x_units != 0 && y_units != 0)
        {
            double i;
            x_units = abs(x_units);
            x_units = (P0.x < P1.x) ? (x_units) : (-x_units);
            y_units = abs(y_units);
            y_units = (P0.y < P1.y) ? (y_units) : (-y_units);
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.y; i <= P1.y; i += y_units)
            {
                rs.push_back(LaserBoy_real_vertex(P0.x, i, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P1.x, i, P0.z, 255, 255, 255, 0,  55));
            }
            if(i != P1.y)
            {
                rs.push_back(LaserBoy_real_vertex(P0.x, P1.y, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P0.z, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.x; i <= P1.x; i += x_units)
            {
                rs.push_back(LaserBoy_real_vertex(i, P0.y, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(i, P1.y, P0.z, 255, 255, 255, 0,  55));
            }
            if(i != P1.x)
            {
                rs.push_back(LaserBoy_real_vertex(P1.x, P0.y, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P0.z, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(P1.x, P1.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        }
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment rectangular_grid_yz(LaserBoy_3D_double P0,
                                                     LaserBoy_3D_double P1,
                                                     double             y_units,
                                                     double             z_units
                                                    )
    {
        LaserBoy_real_segment rs;
        if(P0.z != P1.z && P0.y != P1.y && z_units != 0 && y_units != 0)
        {
            double i;
            y_units = abs(y_units);
            y_units = (P0.y < P1.y) ? (y_units) : (-y_units);
            z_units = abs(z_units);
            z_units = (P0.z < P1.z) ? (z_units) : (-z_units);
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.y; i <= P1.y; i += y_units)
            {
                rs.push_back(LaserBoy_real_vertex(P0.x, i, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P0.x, i, P1.z, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(P0.x, i - y_units, P1.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.z; i <= P1.z; i += z_units)
            {
                rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, i,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P0.x, P1.y, i, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(P0.x, P1.y, i - z_units, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        }
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment rectangular_grid_xz(LaserBoy_3D_double P0,
                                                     LaserBoy_3D_double P1,
                                                     double             x_units,
                                                     double             z_units
                                                    )
    {
        LaserBoy_real_segment rs;
        if(P0.x != P1.x && P0.z != P1.z && x_units != 0 && z_units != 0)
        {
            double i;
            x_units = abs(x_units);
            x_units = (P0.x < P1.x) ? (x_units) : (-x_units);
            z_units = abs(z_units);
            z_units = (P0.z < P1.z) ? (z_units) : (-z_units);
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.z; i <= P1.z; i += z_units)
            {
                rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, i,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(P1.x, P0.y, i, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(P1.x, P0.y, i - z_units, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
            for(i = P0.x; i <= P1.x; i += x_units)
            {
                rs.push_back(LaserBoy_real_vertex(i, P0.y, P0.z,   0,   0,   0, 0, 255));
                rs.push_back(LaserBoy_real_vertex(i, P0.y, P1.z, 255, 255, 255, 0,  55));
            }
            rs.push_back(LaserBoy_real_vertex(i - x_units, P0.y, P1.z, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        }
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_grid_xy(LaserBoy_3D_double P0,
                                               double             radius_max,
                                               double             radii_units,
                                               double             angle_units
                                              )
    {
        double                i,
                              arc_step = two_pi / 60;
        LaserBoy_3D_double    double_3D;
        LaserBoy_real_segment rs;
        radius_max  = abs(radius_max);
        radii_units = abs(radii_units);
        angle_units = abs(angle_units);
        double_3D.z = P0.z;
        for(i = radii_units; i <= radius_max; i += radii_units)
        {
            double_3D.x = i + P0.x;
            double_3D.y = P0.y;
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
            for(int a = 1; a <= 60; a++)
            {
                double_3D.x = i * cos(a * arc_step) + P0.x;
                double_3D.y = i * sin(a * arc_step) + P0.y;
                rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
            }
        }
        for(i = 0; i <= two_pi; i += angle_units)
        {
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z,   0,   0,   0, 0, 255));
            rs.push_back(LaserBoy_real_vertex(radius_max * cos(i) + P0.x, radius_max * sin(i) + P0.y, P0.z, 255, 255, 255, 0,  55));
        }
        rs.push_back(LaserBoy_real_vertex(radius_max, 0.0, 0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_grid_xz(LaserBoy_3D_double P0,
                                               double             radius_max,
                                               double             radii_units,
                                               double             angle_units
                                              )
    {
        double                i,
                              arc_step = two_pi / 60;
        LaserBoy_3D_double    double_3D;
        LaserBoy_real_segment rs;
        radius_max  = abs(radius_max);
        radii_units = abs(radii_units);
        angle_units = abs(angle_units);
        double_3D.y = P0.y;
        for(i = radii_units; i <= radius_max; i += radii_units)
        {
            double_3D.x = i + P0.x;
            double_3D.z = P0.z;
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
            for(int a = 1; a <= 60; a++)
            {
                double_3D.x = i * cos(a * arc_step) + P0.x;
                double_3D.z = i * sin(a * arc_step) + P0.z;
                rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
            }
        }
        for(i = 0; i <= two_pi; i += angle_units)
        {
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z,   0,   0,   0, 0, 255));
            rs.push_back(LaserBoy_real_vertex(radius_max * cos(i) + P0.x, P0.y, radius_max * sin(i) + P0.z, 255, 255, 255, 0,  55));
        }
        rs.push_back(LaserBoy_real_vertex(radius_max, 0.0, 0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        return rs;
    }
    //------------------------------------------------------------------------
    static LaserBoy_real_segment polar_grid_yz(LaserBoy_3D_double P0,
                                               double             radius_max,
                                               double             radii_units,
                                               double             angle_units
                                              )
    {
        double                i,
                              arc_step = two_pi / 60;
        LaserBoy_3D_double    double_3D;
        LaserBoy_real_segment rs;
        radius_max  = abs(radius_max);
        radii_units = abs(radii_units);
        angle_units = abs(angle_units);
        double_3D.x = P0.x;
        for(i = radii_units; i <= radius_max; i += radii_units)
        {
            double_3D.y = i + P0.y;
            double_3D.z = P0.z;
            rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), LASERBOY_BLANKING_BIT, 55));
            for(int a = 1; a <= 60; a++)
            {
                double_3D.y = i * cos(a * arc_step) + P0.y;
                double_3D.z = i * sin(a * arc_step) + P0.z;
                rs.push_back(LaserBoy_real_vertex(double_3D, LaserBoy_color(255, 255, 255), 0, 55));
            }
        }
        for(i = 0; i <= two_pi; i += angle_units)
        {
            rs.push_back(LaserBoy_real_vertex(P0.x, P0.y, P0.z,   0,   0,   0, 0, 255));
            rs.push_back(LaserBoy_real_vertex(P0.x, radius_max * cos(i) + P0.y, radius_max * sin(i) + P0.z, 255, 255, 255, 0,  55));
        }
        rs.push_back(LaserBoy_real_vertex(0.0, radius_max, 0.0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        return rs;
    }
    //------------------------------------------------------------------------
    bool is_2D() const
    {
        if(size() > 1)
        {
            for(size_t i = 0; i < size(); i++)
                if(at(i).z != 0.0)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool is_flat_in_x() const
    {
        if(size() > 1)
        {
            double _x = first_lit_anchor().x;
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
            double _y = first_lit_anchor().y;
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
            double _z = first_lit_anchor().z;
            for(size_t i = 1; i < size(); i++)
                if(at(i).is_lit() && at(i).z != _z)
                    return false;
        }
        return true;
    }
    //------------------------------------------------------------------------
    bool operator == (const LaserBoy_real_segment& rs)
    {
        if(palette_index != rs.palette_index)
            return false;
        if(size() != rs.size())
            return false;
        for(size_t i = 0; i < size(); i++)
            if(at(i) != rs.at(i))
                return false;
        return true;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator += (LaserBoy_real_segment rs)
    {
        if(palette_index != rs.palette_index)
        {
            promote_to_true_color();
            rs.promote_to_true_color();
        }
        insert(end(), rs.begin(), rs.end());
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator += (const LaserBoy_real_vertex& vertex)
    {
        push_back(vertex);
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator += (const LaserBoy_3D_double& float_3D)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) += float_3D;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator -= (const LaserBoy_3D_double& float_3D)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) -= float_3D;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator *= (const LaserBoy_3D_double& float_3D)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) *= float_3D;
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   operator + (LaserBoy_real_segment rs) const
    {
        LaserBoy_real_segment sum(*this);
        if(sum.palette_index != rs.palette_index)
        {
            sum.promote_to_true_color();
            rs.promote_to_true_color();
        }
        sum.insert(sum.end(), rs.begin(), rs.end());
        sum.sync_rgb_and_palette();
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   operator + (const LaserBoy_3D_double& float_3D) const
    {
        LaserBoy_real_segment real_segment;
        for(size_t i = 0; i < size(); i++)
            real_segment.push_back(at(i) + float_3D);
        return real_segment;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   operator * (const LaserBoy_3D_double& float_3D) const
    {
        LaserBoy_real_segment real_segment;
        for(size_t i = 0; i < size(); i++)
            real_segment.push_back(at(i) * float_3D);
        return real_segment;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  operator =  (const LaserBoy_real_segment& rs)
    {
        palette_index      = rs.palette_index;
        real_segment_error = rs.real_segment_error;
        clear();
        reserve(rs.size());
        insert(begin(), rs.begin(), rs.end());
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment   sub(LaserBoy_real_segment::const_iterator begin,
                                LaserBoy_real_segment::const_iterator last
                               )
    {
        LaserBoy_real_segment sub(palette_index, false);
        sub.insert(sub.begin(), begin, last);
        return sub;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex  first_lit_vertex() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return at(i);
        return LaserBoy_real_vertex(0);
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
    LaserBoy_real_vertex first_lit_anchor() const
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                return at(i - 1);
        return LaserBoy_real_vertex(0);
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
    LaserBoy_real_vertex last_lit_anchor() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return at(i - 1);
        return LaserBoy_real_vertex(0);
    }
    //------------------------------------------------------------------------
    LaserBoy_real_vertex last_lit_vector() const
    {
        for(size_t i = size() - 1; i > 0; i--)
            if(at(i).is_lit())
                return at(i);
        return LaserBoy_real_vertex(0);
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
    size_t number_of_lit_vectors() const
    {
        size_t count = 0;
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_lit())
                count++;
        return count;
    }
    //------------------------------------------------------------------------
    size_t number_of_color_vectors() const;
    size_t number_of_dark_vectors () const;
    //------------------------------------------------------------------------
    size_t number_of_blank_vectors() const
    {
        size_t count = 0;
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_blank())
                count++;
        return count;
    }
    //------------------------------------------------------------------------
    size_t number_of_unique_colors() const
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
    LaserBoy_real_segment& remove_vertex(size_t vertex_index)
    {
        if(size() < 3) // takes 2 vertices to make a vector!
            clear();
        else if(vertex_index < size())
        {
            size_t                i;
            LaserBoy_real_segment rs(palette_index, false);
            rs.reserve(size());
            for(i = 0; i < vertex_index; i++)
                rs += at(i);
            for(i = vertex_index + 1; i < size(); i++)
                rs += at(i);
            *this = rs;
        }
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_real_segment& sort()
    {
        if(size() < 2) // takes 2 vertices to make a vector!
            clear();
        else
            std::sort(begin(), end(), less_than);
        return *this;
    }
    //------------------------------------------------------------------------
    bool is_closed_polygon() const
    {
        if(     (LaserBoy_3D_double)first_lit_anchor()
             == (LaserBoy_3D_double)last_lit_vector()
          )
            return true;
        return false;
    }
    //------------------------------------------------------------------------
    u_int bmp_bpp() const
    {
        if(number_of_unique_colors() <= 1)
            return 1;
        if(number_of_unique_colors() <= 15)
            return 4;
        if(number_of_unique_colors() <= 255)
            return 8;
        return 24;
    }
    //------------------------------------------------------------------------
    LaserBoy_3D_double      dimensions                    (LaserBoy_3D_double& real_offset,
                                                           bool ignore_origin
                                                          )  const ;
    LaserBoy_3D_double      segment_front                 () const ;
    LaserBoy_3D_double      segment_back                  () const ;
    LaserBoy_3D_double      segment_top                   () const ;
    LaserBoy_3D_double      segment_bottom                () const ;
    LaserBoy_3D_double      segment_right                 () const ;
    LaserBoy_3D_double      segment_left                  () const ;
    double                  max_x                         () const ;
    double                  max_y                         () const ;
    double                  max_z                         () const ;
    double                  min_x                         () const ;
    double                  min_y                         () const ;
    double                  min_z                         () const ;
    double                  height                        () const ;
    double                  width                         () const ;
    double                  depth                         () const ;
    double                  max_dimension                 () const ;
    double                  max_distance_from_origin      () const ;
    double                  max_distance_from_origin_xy   () const ;
    double                  max_distance_from_origin_zy   () const ;
    double                  max_distance_from_origin_xz   () const ;
    size_t                  i_max_distance_from_origin_xy () const ;
    size_t                  i_max_distance_from_origin_zy () const ;
    size_t                  i_max_distance_from_origin_xz () const ;
    double                  min_distance_from_origin      () const ;
    double                  min_distance_from_origin_xy   () const ;
    double                  min_distance_from_origin_zy   () const ;
    double                  min_distance_from_origin_xz   () const ;
    size_t                  i_min_distance_from_origin_xy () const ;
    size_t                  i_min_distance_from_origin_zy () const ;
    size_t                  i_min_distance_from_origin_xz () const ;
    bool                    segment_passes_through_origin (const double& granularity) const ;
    LaserBoy_real_segment&  reverse                       ();
    LaserBoy_real_segment&  reorder_from                  (size_t index);
    LaserBoy_real_segment&  center_x                      ();
    LaserBoy_real_segment&  center_y                      ();
    LaserBoy_real_segment&  center_z                      ();
    LaserBoy_real_segment&  center                        ();
    void                    blank_all_vertices            ();
    void                    unblank_all_vertices          ();
    void                    strip_color                   ();
    void                    strip_color_rgb               (const LaserBoy_color& c);
    void                    index_hues                    (int index_multiple, int offset);
    void                    span_hues                     (double span_factor, int offset);
    void                    flip                          (u_int plane);
    void                    quarter_turn                  (u_int plane, u_int turns);
//    void                    z_order_vertices              (short span);
    void                    flatten_z                     ();
    //------------------------------------------------------------------------
    double                  vector_angle                  (size_t vertex) const ;
    double                  total_angle                   () const ;
    double                  max_angle                     () const ;
    double                  total_magnitude               () const ;
    double                  max_magnitude                 () const ;
    double                  max_color_magnitude           () const ;
    double                  max_dark_magnitude            () const ;
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  move_                         (LaserBoy_3D_double  displacement,
                                                           double              move_acceleration
                                                          );
    LaserBoy_real_segment&  scale_                        (LaserBoy_3D_double  factor,  // from
                                                           LaserBoy_3D_double  factor_, // to
                                                           double              scale_acceleration
                                                          );
    LaserBoy_real_segment&  rotate_                       (LaserBoy_3D_double  fulcrum,  // from
                                                           LaserBoy_3D_double  fulcrum_, // to
                                                           double              fulcrum_acceleration,
                                                           LaserBoy_3D_double  rotation,
                                                           double              rotate_acceleration
                                                          );
    //------------------------------------------------------------------------
    LaserBoy_real_vertex    interpolated_value_at         (double relative_position) const;
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  add                           (LaserBoy_real_segment  rs_displacements);
    LaserBoy_real_segment&  multiply                      (LaserBoy_real_segment  rs_factors);
    LaserBoy_real_segment&  warp                          (LaserBoy_3D_double     fulcrum,
                                                           LaserBoy_3D_double     fulcrum_,
                                                           double                 fulcrum_acceleration,
                                                           LaserBoy_real_segment  rs_rotations);
    LaserBoy_real_segment&  xyz_rgb                       (LaserBoy_real_segment  rs_colors);
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  to_polar                      ();
    LaserBoy_real_segment&  differences                   ();
    LaserBoy_real_segment&  sums                          ();
    LaserBoy_real_segment&  to_rectangular                ();
    //------------------------------------------------------------------------
    void                    rotate                        (LaserBoy_3D_double a);
    void                    rotate_around_origin          (LaserBoy_3D_double a);
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  rotate_on_coordinates         (LaserBoy_3D_double fulcrum,  // from
                                                           LaserBoy_3D_double fulcrum_, // to
                                                           double             fulcrum_acceleration,
                                                           LaserBoy_3D_double rotation
                                                          );
    void                    rotate_on_coordinates_x       (LaserBoy_3D_double fulcrum,
                                                           double             angle
                                                          );
    void                    rotate_on_coordinates_y       (LaserBoy_3D_double fulcrum,
                                                           double             angle
                                                          );
    void                    rotate_on_coordinates_z       (LaserBoy_3D_double fulcrum,
                                                           double             angle
                                                          );
    //------------------------------------------------------------------------
    bool                    find_rgb_in_palette           (const LaserBoy_palette& palette);
    void                    promote_to_true_color         ();
    void                    set_rgb_from_palette          ();
    void                    set_palette_to_332            ();
    void                    sync_rgb_and_palette          ();
    void                    best_match_palette            (int index);
    void                    bit_reduce_to_palette         ();
    void                    best_reduce_to_palette        ();
    void                    convert_black_to_blank        ();
    void                    convert_blank_to_black        ();
    void                    impose_black_level            ();
    void                    reduce_blank_vectors          ();
    void                    reduce_lit_vectors            ();
    void                    omit_equivalent_vectors       ();
    LaserBoy_real_segment&  move                          (LaserBoy_3D_double d);
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  scale_around_origin           (LaserBoy_3D_double s);
    LaserBoy_real_segment&  scale_on_coordinates          (LaserBoy_3D_double p,
                                                           LaserBoy_3D_double s
                                                          );
    LaserBoy_real_segment&  scale                         (LaserBoy_3D_double s);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     add_lit_span_vertices         ();
    void                    add_lit_span_vertices         (const u_int& max_d);
    LaserBoy_Error_Code     add_blank_span_vertices       ();
    //------------------------------------------------------------------------
    LaserBoy_3D_double      rectangular_center_of         () const ;
    LaserBoy_3D_double      mean_of_coordinates           () const ;
    LaserBoy_3D_double      centroid_of_coordinates_xy    (double &area, LaserBoy_real_segment &rs) const ;
    LaserBoy_3D_double      centroid_of_coordinates_zy    (double &area, LaserBoy_real_segment &rs) const ;
    LaserBoy_3D_double      centroid_of_coordinates_xz    (double &area, LaserBoy_real_segment &rs) const ;
    LaserBoy_3D_double      centroid_of_coordinates       () const ;
    //------------------------------------------------------------------------
    LaserBoy_real_segment   polygon_outline_xy            (double &area) const ;
    LaserBoy_real_segment   polygon_outline_zy            (double &area) const ;
    LaserBoy_real_segment   polygon_outline_xz            (double &area) const ;
    //------------------------------------------------------------------------
    size_t                  number_of_segments            () const ;
    //------------------------------------------------------------------------
    bool                    find_segment_at_index         (size_t  index,
                                                           size_t& start,
                                                           size_t& end
                                                          )             const ;
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  fracture_segments             ();
    LaserBoy_real_segment&  bond_segments                 ();
    LaserBoy_real_segment&  conglomerate_lit_segments     ();
    LaserBoy_real_segment&  sort_by_rotation_xy           ();
    LaserBoy_real_segment&  sort_by_rotation_zy           ();
    LaserBoy_real_segment&  sort_by_rotation_xz           ();
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set explode_segments            () const ;
    //------------------------------------------------------------------------
    LaserBoy_real_segment   copy_segment                  (size_t index) const ;
    LaserBoy_3D_double      rectangular_center_of_segment (size_t index) const ;
    LaserBoy_3D_double      mean_of_coordinates_of_segment(size_t index) const ;
    LaserBoy_3D_double      centroid_of_segment_xy        (size_t index, double& area, LaserBoy_real_segment& rs) const ;
    LaserBoy_3D_double      centroid_of_segment_xz        (size_t index, double& area, LaserBoy_real_segment& rs) const ;
    LaserBoy_3D_double      centroid_of_segment_zy        (size_t index, double& area, LaserBoy_real_segment& rs) const ;
    LaserBoy_3D_double      centroid_of_segment           (size_t index) const ;
    void                    move_segment                  (size_t index, LaserBoy_3D_double f);
    void                    center_segment_x              (size_t index);
    void                    center_segment_y              (size_t index);
    void                    center_segment_z              (size_t index);
    void                    center_segment                (size_t index);
    void                    rotate_segment                (size_t index, LaserBoy_3D_double a);
    void                    rotate_segment_around_origin  (size_t index, LaserBoy_3D_double a);
    void                    scale_segment                 (size_t index, LaserBoy_3D_double m);
    void                    scale_segment_around_origin   (size_t index, LaserBoy_3D_double m);
    //------------------------------------------------------------------------
    bool                    save_as_bmp                   (const string&      file,
                                                           u_int              x_res,
                                                           u_int              y_res,
                                                           u_int              rendered_line_width,
                                                           bool               bmp_render_vectors,
                                                           bool               bmp_render_vertices,
                                                           double             scale = 0
                                                          ) const ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     from_ifstream_txt             (std::ifstream&  in,
                                                           const u_int&    group_type,
                                                           int&            line_number
                                                          );
    void                    to_ofstream_txt_color_table   (std::ofstream& out);
    void                    to_ofstream_txt               (std::ofstream& out,
                                                           int frame_index
                                                          );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     from_ifstream_dxf             (std::ifstream& in);
    bool                    save_as_dxf                   (const string& file,
                                                           bool  save_true_color_dxf
                                                          ) const;
    //------------------------------------------------------------------------
    void                    normalize                     (bool ignore_origin = true); // -32767 to +32767
    void                    normalize_vectors             (bool ignore_origin = true);
    void                    normalize_vectors_with_origin (bool ignore_origin = true);
    void                    to_unit                       (bool ignore_origin = true); // -1.0 to +1.0
    void                    to_unit_with_origin           (bool ignore_origin = true);
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  clip                          (const LaserBoy_3D_double& max,
                                                           const LaserBoy_3D_double& min,
                                                           const double&             granularity
                                                          );
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  clip                          ();
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  clip_around_coordinate        (const LaserBoy_3D_double& center,
                                                           const LaserBoy_3D_double& max,
                                                           const LaserBoy_3D_double& min,
                                                           const double&             granularity
                                                          );
    //------------------------------------------------------------------------
    LaserBoy_real_segment&  clip_around_coordinate        (const LaserBoy_3D_double& center,
                                                           const double              range,
                                                           const double&             granularity
                                                          );
    //------------------------------------------------------------------------
    bool                 is_selected       ;
    int                  palette_index     ;
    LaserBoy_Error_Code  real_segment_error;
    //------------------------------------------------------------------------
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
