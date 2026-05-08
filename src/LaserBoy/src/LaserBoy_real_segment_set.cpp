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
// LaserBoy_real_segment_set.cpp is part of LaserBoy.
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
LaserBoy_real_segment_set::LaserBoy_real_segment_set(const double    radius,
                                                     const double    rhodonea_numerator,
                                                     const double    rhodonea_denominator,
                                                     const double    start,
                                                     const double    duration,
                                                     const double    iterations,
                                                     const double    _radius,
                                                     const double    _rhodonea_numerator,
                                                     const double    _rhodonea_denominator,
                                                     const double    _start,
                                                     const double    _duration,
                                                     const double    _iterations,
                                                     const int       frames
                                                    ) // animated rhodonea
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating rhodonea frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::rhodonea(radius               * balance +               _radius * ratio,
                                                  rhodonea_numerator   * balance +   _rhodonea_numerator * ratio,
                                                  rhodonea_denominator * balance + _rhodonea_denominator * ratio,
                                                  start                * balance +                _start * ratio,
                                                  duration             * balance +             _duration * ratio,
                                                  iterations           * balance +           _iterations * ratio
                                                 )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated oscillator_rhodonea
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_rhodonea frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_rhodonea( LBO1.blend(_LBO1 , ratio),
                                                              LBO2.blend(_LBO2 , ratio),
                                                              LBO3.blend(_LBO3 , ratio),
                                                              LBO4.blend(_LBO4 , ratio),
                                                             LBO1_.blend(_LBO1_, ratio),
                                                             LBO2_.blend(_LBO2_, ratio),
                                                             LBO3_.blend(_LBO3_, ratio),
                                                             LBO4_.blend(_LBO4_, ratio),
                                                             radius               * balance +               _radius * ratio,
                                                             rhodonea_numerator   * balance +   _rhodonea_numerator * ratio,
                                                             rhodonea_denominator * balance + _rhodonea_denominator * ratio,
                                                             start                * balance +                _start * ratio,
                                                             duration             * balance +             _duration * ratio,
                                                             iterations           * balance +           _iterations * ratio
                                                            )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(const double    fixed_radius,
                                                     const double    roller_radius,
                                                     const double    roller_offset,
                                                     const double    start,
                                                     const double    duration,
                                                     const double    iterations,
                                                     const double    _fixed_radius,
                                                     const double    _roller_radius,
                                                     const double    _roller_offset,
                                                     const double    _start,
                                                     const double    _duration,
                                                     const double    _iterations,
                                                     const int       frames,
                                                     const char      dud
                                                    ) // animated epitrochoid epicycloid
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating epicycloid or epitrochoid frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::epicycloid(fixed_radius  * balance +  _fixed_radius * ratio,
                                                    roller_radius * balance + _roller_radius * ratio,
                                                    roller_offset * balance + _roller_offset * ratio,
                                                    start         * balance +         _start * ratio,
                                                    duration      * balance +      _duration * ratio,
                                                    iterations    * balance +    _iterations * ratio
                                                   )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated oscillator_epitrochoid oscillator_epicycloid
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_epicycloid or oscillator_epitrochoid frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_epicycloid( LBO1.blend(_LBO1 , ratio),
                                                                LBO2.blend(_LBO2 , ratio),
                                                                LBO3.blend(_LBO3 , ratio),
                                                                LBO4.blend(_LBO4 , ratio),
                                                               LBO1_.blend(_LBO1_, ratio),
                                                               LBO2_.blend(_LBO2_, ratio),
                                                               LBO3_.blend(_LBO3_, ratio),
                                                               LBO4_.blend(_LBO4_, ratio),
                                                               fixed_radius  * balance +  _fixed_radius * ratio,
                                                               roller_radius * balance + _roller_radius * ratio,
                                                               roller_offset * balance + _roller_offset * ratio,
                                                               start         * balance +         _start * ratio,
                                                               duration      * balance +      _duration * ratio,
                                                               iterations    * balance +    _iterations * ratio
                                                              )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(const double    fixed_radius,
                                                     const double    roller_radius,
                                                     const double    roller_offset,
                                                     const double    start,
                                                     const double    duration,
                                                     const double    iterations,
                                                     const double    _fixed_radius,
                                                     const double    _roller_radius,
                                                     const double    _roller_offset,
                                                     const double    _start,
                                                     const double    _duration,
                                                     const double    _iterations,
                                                     const int       frames,
                                                     const short     dud
                                                    ) // animated hypotrochoid hypocycloid
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating hypocycloid or hypotrochoid frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::hypocycloid(fixed_radius  * balance +  _fixed_radius * ratio,
                                                     roller_radius * balance + _roller_radius * ratio,
                                                     roller_offset * balance + _roller_offset * ratio,
                                                     start         * balance +         _start * ratio,
                                                     duration      * balance +      _duration * ratio,
                                                     iterations    * balance +    _iterations * ratio
                                                    )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated oscillator_hypotrochoid oscillator_hypocycloid
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_hypocycloid or oscillator_hypotrochoid frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_hypocycloid( LBO1.blend(_LBO1 , ratio),
                                                                 LBO2.blend(_LBO2 , ratio),
                                                                 LBO3.blend(_LBO3 , ratio),
                                                                 LBO4.blend(_LBO4 , ratio),
                                                                LBO1_.blend(_LBO1_, ratio),
                                                                LBO2_.blend(_LBO2_, ratio),
                                                                LBO3_.blend(_LBO3_, ratio),
                                                                LBO4_.blend(_LBO4_, ratio),
                                                                fixed_radius  * balance +  _fixed_radius * ratio,
                                                                roller_radius * balance + _roller_radius * ratio,
                                                                roller_offset * balance + _roller_offset * ratio,
                                                                start         * balance +         _start * ratio,
                                                                duration      * balance +      _duration * ratio,
                                                                iterations    * balance +    _iterations * ratio
                                                               )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _oscillator
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator( LBO1.blend( _LBO1, ratio),
                                                    LBO1_.blend(_LBO1_, ratio),
                                                    start      * balance + _start      * ratio,
                                                    duration   * balance + _duration   * ratio,
                                                    iterations * balance + _iterations * ratio
                                                   )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _oscillator_sum
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_sum frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_sum( LBO1.blend(_LBO1 , ratio),
                                                         LBO2.blend(_LBO2 , ratio),
                                                        LBO1_.blend(_LBO1_, ratio),
                                                        LBO2_.blend(_LBO2_, ratio),
                                                        start      * balance + _start      * ratio,
                                                        duration   * balance + _duration   * ratio,
                                                        iterations * balance + _iterations * ratio
                                                       )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator  LBO1,
                                                     LaserBoy_oscillator  LBO2,
                                                     LaserBoy_oscillator  LBO1_,
                                                     LaserBoy_oscillator  LBO2_,
                                                     const double         start,
                                                     const double         duration,
                                                     const double         iterations,
                                                     const double         acceleration,
                                                     LaserBoy_oscillator _LBO1,
                                                     LaserBoy_oscillator _LBO2,
                                                     LaserBoy_oscillator _LBO1_,
                                                     LaserBoy_oscillator _LBO2_,
                                                     const double        _start,
                                                     const double        _duration,
                                                     const double        _iterations,
                                                     const double        _acceleration,
                                                     const int           frames,
                                                     const char          dud
                                                    ) // animated _oscillator_xy
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_xy frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_xy( LBO1.blend(_LBO1 , ratio),
                                                        LBO2.blend(_LBO2 , ratio),
                                                       LBO1_.blend(_LBO1_, ratio),
                                                       LBO2_.blend(_LBO2_, ratio),
                                                       start         * balance + _start        * ratio,
                                                       duration      * balance + _duration     * ratio,
                                                         (iterations == _iterations)
                                                       ? (iterations)
                                                       : (iterations * balance + _iterations   * ratio),
                                                       acceleration  * balance + _acceleration * ratio
                                                      )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
                                                     LaserBoy_oscillator LBO2,
                                                     LaserBoy_oscillator LBO3,
                                                     LaserBoy_oscillator LBO1_,
                                                     LaserBoy_oscillator LBO2_,
                                                     LaserBoy_oscillator LBO3_,
                                                     const double        start,
                                                     const double        duration,
                                                     const double        iterations,
                                                     const double        acceleration,
                                                     LaserBoy_oscillator _LBO1,
                                                     LaserBoy_oscillator _LBO2,
                                                     LaserBoy_oscillator _LBO3,
                                                     LaserBoy_oscillator _LBO1_,
                                                     LaserBoy_oscillator _LBO2_,
                                                     LaserBoy_oscillator _LBO3_,
                                                     const double        _start,
                                                     const double        _duration,
                                                     const double        _iterations,
                                                     const double        _acceleration,
                                                     const int           frames
                                                    ) // animated oscillator_xyz
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating oscillator_xyz frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::oscillator_xyz( LBO1.blend(_LBO1 , ratio),
                                                         LBO2.blend(_LBO2 , ratio),
                                                         LBO3.blend(_LBO3 , ratio),
                                                        LBO1_.blend(_LBO1_, ratio),
                                                        LBO2_.blend(_LBO2_, ratio),
                                                        LBO3_.blend(_LBO3_, ratio),
                                                        start         * balance + _start        * ratio,
                                                        duration      * balance + _duration     * ratio,
                                                          (iterations == _iterations)
                                                        ? (iterations)
                                                        : (iterations * balance + _iterations   * ratio),
                                                        acceleration  * balance + _acceleration * ratio
                                                       )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated harmonograph
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating harmonograph frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::harmonograph( LBO1.blend(_LBO1 , ratio),
                                                       LBO2.blend(_LBO2 , ratio),
                                                       LBO3.blend(_LBO3 , ratio),
                                                       LBO4.blend(_LBO4 , ratio),
                                                      LBO1_.blend(_LBO1_, ratio),
                                                      LBO2_.blend(_LBO2_, ratio),
                                                      LBO3_.blend(_LBO3_, ratio),
                                                      LBO4_.blend(_LBO4_, ratio),
                                                      start      * balance + _start      * ratio,
                                                      duration   * balance + _duration   * ratio,
                                                      iterations * balance + _iterations * ratio
                                                     )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated harmonograph_xyz
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating harmonograph_xyz frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::harmonograph_xyz( LBO1.blend(_LBO1 , ratio),
                                                           LBO2.blend(_LBO2 , ratio),
                                                           LBO3.blend(_LBO3 , ratio),
                                                           LBO4.blend(_LBO4 , ratio),
                                                           LBO5.blend(_LBO5 , ratio),
                                                           LBO6.blend(_LBO6 , ratio),
                                                          LBO1_.blend(_LBO1_, ratio),
                                                          LBO2_.blend(_LBO2_, ratio),
                                                          LBO3_.blend(_LBO3_, ratio),
                                                          LBO4_.blend(_LBO4_, ratio),
                                                          LBO5_.blend(_LBO5_, ratio),
                                                          LBO6_.blend(_LBO6_, ratio),
                                                          start      * balance + _start      * ratio,
                                                          duration   * balance + _duration   * ratio,
                                                          iterations * balance + _iterations * ratio
                                                         )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _amplitude_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating amplitude_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::amplitude_mod( LBO1.blend(_LBO1 , ratio),
                                                        LBO2.blend(_LBO2 , ratio),
                                                       LBO1_.blend(_LBO1_, ratio),
                                                       LBO2_.blend(_LBO2_, ratio),
                                                       start      * balance + _start      * ratio,
                                                       duration   * balance + _duration   * ratio,
                                                       iterations * balance + _iterations * ratio
                                                      )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _amplitude_mod_xy
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating amplitude_mod_xy frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::amplitude_mod_xy( LBO1.blend(_LBO1 , ratio),
                                                           LBO2.blend(_LBO2 , ratio),
                                                           LBO3.blend(_LBO3 , ratio),
                                                           LBO4.blend(_LBO4 , ratio),
                                                          LBO1_.blend(_LBO1_, ratio),
                                                          LBO2_.blend(_LBO2_, ratio),
                                                          LBO3_.blend(_LBO3_, ratio),
                                                          LBO4_.blend(_LBO4_, ratio),
                                                          start      * balance + _start      * ratio,
                                                          duration   * balance + _duration   * ratio,
                                                          iterations * balance + _iterations * ratio
                                                         )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _amplitude_mod_xyz
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating amplitude_mod_xyz frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::amplitude_mod_xyz( LBO1.blend(_LBO1 , ratio),
                                                            LBO2.blend(_LBO2 , ratio),
                                                            LBO3.blend(_LBO3 , ratio),
                                                            LBO4.blend(_LBO4 , ratio),
                                                            LBO5.blend(_LBO5 , ratio),
                                                            LBO6.blend(_LBO6 , ratio),
                                                           LBO1_.blend(_LBO1_, ratio),
                                                           LBO2_.blend(_LBO2_, ratio),
                                                           LBO3_.blend(_LBO3_, ratio),
                                                           LBO4_.blend(_LBO4_, ratio),
                                                           LBO5_.blend(_LBO5_, ratio),
                                                           LBO6_.blend(_LBO6_, ratio),
                                                           start      * balance + _start      * ratio,
                                                           duration   * balance + _duration   * ratio,
                                                           iterations * balance + _iterations * ratio
                                                          )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _frequency_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating frequency_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::frequency_mod( LBO1.blend(_LBO1 , ratio),
                                                        LBO2.blend(_LBO2 , ratio),
                                                       LBO1_.blend(_LBO1_, ratio),
                                                       LBO2_.blend(_LBO2_, ratio),
                                                       start      * balance + _start      * ratio,
                                                       duration   * balance + _duration   * ratio,
                                                       iterations * balance + _iterations * ratio
                                                      )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _frequency_mod_xy
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating frequency_mod_xy frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::frequency_mod_xy( LBO1.blend(_LBO1 , ratio),
                                                           LBO2.blend(_LBO2 , ratio),
                                                           LBO3.blend(_LBO3 , ratio),
                                                           LBO4.blend(_LBO4 , ratio),
                                                          LBO1_.blend(_LBO1_, ratio),
                                                          LBO2_.blend(_LBO2_, ratio),
                                                          LBO3_.blend(_LBO3_, ratio),
                                                          LBO4_.blend(_LBO4_, ratio),
                                                          start      * balance + _start      * ratio,
                                                          duration   * balance + _duration   * ratio,
                                                          iterations * balance + _iterations * ratio
                                                         )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _frequency_mod_xyz
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating frequency_mod_xyz frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::frequency_mod_xyz( LBO1.blend(_LBO1 , ratio),
                                                            LBO2.blend(_LBO2 , ratio),
                                                            LBO3.blend(_LBO3 , ratio),
                                                            LBO4.blend(_LBO4 , ratio),
                                                            LBO5.blend(_LBO5 , ratio),
                                                            LBO6.blend(_LBO6 , ratio),
                                                           LBO1_.blend(_LBO1_, ratio),
                                                           LBO2_.blend(_LBO2_, ratio),
                                                           LBO3_.blend(_LBO3_, ratio),
                                                           LBO4_.blend(_LBO4_, ratio),
                                                           LBO5_.blend(_LBO5_, ratio),
                                                           LBO6_.blend(_LBO6_, ratio),
                                                           start      * balance + _start      * ratio,
                                                           duration   * balance + _duration   * ratio,
                                                           iterations * balance + _iterations * ratio
                                                          )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _phase_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating phase_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::phase_mod( LBO1.blend(_LBO1 , ratio),
                                                    LBO2.blend(_LBO2 , ratio),
                                                   LBO1_.blend(_LBO1_, ratio),
                                                   LBO2_.blend(_LBO2_, ratio),
                                                   start      * balance + _start      * ratio,
                                                   duration   * balance + _duration   * ratio,
                                                   iterations * balance + _iterations * ratio
                                                  )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                     const int          dud
                                                    ) // animated _phase_mod_xy
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating phase_mod_xy frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::phase_mod_xy( LBO1.blend(_LBO1 , ratio),
                                                       LBO2.blend(_LBO2 , ratio),
                                                       LBO3.blend(_LBO3 , ratio),
                                                       LBO4.blend(_LBO4 , ratio),
                                                      LBO1_.blend(_LBO1_, ratio),
                                                      LBO2_.blend(_LBO2_, ratio),
                                                      LBO3_.blend(_LBO3_, ratio),
                                                      LBO4_.blend(_LBO4_, ratio),
                                                      start      * balance + _start      * ratio,
                                                      duration   * balance + _duration   * ratio,
                                                      iterations * balance + _iterations * ratio
                                                     )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _phase_mod_xyz
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating phase_mod_xyz frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::phase_mod_xyz( LBO1.blend(_LBO1 , ratio),
                                                        LBO2.blend(_LBO2 , ratio),
                                                        LBO3.blend(_LBO3 , ratio),
                                                        LBO4.blend(_LBO4 , ratio),
                                                        LBO5.blend(_LBO5 , ratio),
                                                        LBO6.blend(_LBO6 , ratio),
                                                       LBO1_.blend(_LBO1_, ratio),
                                                       LBO2_.blend(_LBO2_, ratio),
                                                       LBO3_.blend(_LBO3_, ratio),
                                                       LBO4_.blend(_LBO4_, ratio),
                                                       LBO5_.blend(_LBO5_, ratio),
                                                       LBO6_.blend(_LBO6_, ratio),
                                                       start      * balance + _start      * ratio,
                                                       duration   * balance + _duration   * ratio,
                                                       iterations * balance + _iterations * ratio
                                                      )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _polar
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating polar frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::polar( LBO1.blend(_LBO1 , ratio),
                                               LBO1_.blend(_LBO1_, ratio),
                                               start      * balance + _start      * ratio,
                                               duration   * balance + _duration   * ratio,
                                               iterations * balance + _iterations * ratio
                                              )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _polar_sum
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating polar_sum frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::polar_sum( LBO1.blend(_LBO1 , ratio),
                                                    LBO2.blend(_LBO2 , ratio),
                                                   LBO1_.blend(_LBO1_, ratio),
                                                   LBO2_.blend(_LBO2_, ratio),
                                                   start      * balance + _start      * ratio,
                                                   duration   * balance + _duration   * ratio,
                                                   iterations * balance + _iterations * ratio
                                                  )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _polar_amplitude_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating polar_amplitude_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::polar_amplitude_mod( LBO1.blend(_LBO1 , ratio),
                                                              LBO2.blend(_LBO2 , ratio),
                                                             LBO1_.blend(_LBO1_, ratio),
                                                             LBO2_.blend(_LBO2_, ratio),
                                                             start      * balance + _start      * ratio,
                                                             duration   * balance + _duration   * ratio,
                                                             iterations * balance + _iterations * ratio
                                                            )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _polar_frequency_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating polar_frequency_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::polar_frequency_mod( LBO1.blend(_LBO1 , ratio),
                                                              LBO2.blend(_LBO2 , ratio),
                                                             LBO1_.blend(_LBO1_, ratio),
                                                             LBO2_.blend(_LBO2_, ratio),
                                                             start      * balance + _start      * ratio,
                                                             duration   * balance + _duration   * ratio,
                                                             iterations * balance + _iterations * ratio
                                                            )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment_set::LaserBoy_real_segment_set(LaserBoy_oscillator LBO1,
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
                                                    ) // animated _polar_phase_mod
        :    vector<LaserBoy_real_segment>(),
             been_used                    (false),
             name                         (""),
             real_segment_set_error       (LASERBOY_OK)
{
    double ratio,
           balance,
           all = frames - 1.0;
    p_space->p_GUI->display_state("calculating polar_phase_mod frames");
    for(int frame = 0; frame < frames; frame++)
    {
        ratio   = frame / all;
        balance = 1.0 - ratio;
        push_back(LaserBoy_real_segment::polar_phase_mod( LBO1.blend(_LBO1 , ratio),
                                                          LBO2.blend(_LBO2 , ratio),
                                                         LBO1_.blend(_LBO1_, ratio),
                                                         LBO2_.blend(_LBO2_, ratio),
                                                         start      * balance + _start      * ratio,
                                                         duration   * balance + _duration   * ratio,
                                                         iterations * balance + _iterations * ratio
                                                        )
                 );
        p_space->p_GUI->display_progress(frames - frame);
    }
}

//############################################################################
LaserBoy_real_segment LaserBoy_real_segment_set::collapse_frames()
{
    LaserBoy_real_segment collapsed;
    for(size_t i = 0; i < size(); i++)
        collapsed += at(i);
    collapsed.sync_rgb_and_palette();
    return collapsed;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment_set::collapse_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_real_segment  collapsed;
        int                    first_selected_frame_index = -1;
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_selected)
            {
                if(first_selected_frame_index == -1)
                    first_selected_frame_index = i;
                collapsed += at(i);
            }
        }
        if(first_selected_frame_index != -1)
        {
            at(first_selected_frame_index) = collapsed;
            at(first_selected_frame_index).is_selected = false;
            delete_selected_frames();
        }
    }
    return LASERBOY_OK;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::move(LaserBoy_3D_double displacement)
{
    p_space->p_GUI->display_state("applying move on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).move(displacement);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_move(LaserBoy_3D_double  displacement,
                                                            LaserBoy_3D_double _displacement,
                                                            double              move_acceleration
                                                           )
{
    double              ratio,
                        step,
                        dsize = size() - 1.0;
    LaserBoy_3D_double  diff = _displacement - displacement;
    p_space->p_GUI->display_state("applying _move on frames");
    if(move_acceleration == 0.0)
        for(size_t i = 0; i < size(); i++)
        {
            ratio = i / dsize;
            at(i).move(displacement * (1.0 - ratio) + _displacement * ratio);
            p_space->p_GUI->display_progress(size() - i);
        }
    else if(move_acceleration > 0.0)
    {
        step = move_acceleration / size();
        for(size_t i = 0; i < size(); i++)
            at(i).move(displacement + (diff * (i / dsize) * pow(_e_, -(move_acceleration - (i * step)))));
    }
    else // if(move_acceleration < 0.0)
    {
        move_acceleration = -move_acceleration;
        step = move_acceleration / size();
        for(size_t i = 0; i < size(); i++)
            at(dsize - i).move(_displacement - (diff * (i / dsize) * pow(_e_, -(move_acceleration - (i * step)))));
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::scale_around_origin(LaserBoy_3D_double scale)
{
    p_space->p_GUI->display_state("applying scale on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).scale_around_origin(scale);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_scale_around_origin(LaserBoy_3D_double  scale,
                                                                           LaserBoy_3D_double _scale,
                                                                           double              scale_acceleration
                                                                          )
{
    double              ratio,
                        step,
                        dsize = size() - 1.0;
    LaserBoy_3D_double  diff = _scale - scale;
    p_space->p_GUI->display_state("applying _scale on frames");
    if(scale_acceleration == 0.0)
        for(size_t i = 0; i < size(); i++)
        {
            ratio = i / dsize;
            at(i).scale_around_origin(scale * (1.0 - ratio) + _scale * ratio);
            p_space->p_GUI->display_progress(size() - i);
        }
    else if(scale_acceleration > 0.0)
    {
        step = scale_acceleration / size();
        for(size_t i = 0; i < size(); i++)
            at(i).scale_around_origin(scale + (diff * (i / dsize) * pow(_e_, -(scale_acceleration - (i * step)))));
    }
    else // if(scale_acceleration < 0.0)
    {
        scale_acceleration = -scale_acceleration;
        step = scale_acceleration / size();
        for(size_t i = 0; i < size(); i++)
            at(dsize - i).scale_around_origin(_scale - (diff * (i / dsize) * pow(_e_, -(scale_acceleration - (i * step)))));
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::clip()
{
    return clip(LaserBoy_3D_double( 32767.0,  32767.0,  32767.0), // max
                LaserBoy_3D_double(-32767.0, -32767.0, -32767.0), // min
                65536.0                                           // granularity
               );
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::clip(LaserBoy_3D_double  P1,
                                                           LaserBoy_3D_double  P0,
                                                           double              granularity
                                                          )
{
    p_space->p_GUI->display_state("applying clip on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).clip(P1, P0, granularity);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_clip(LaserBoy_3D_double  P1,
                                                            LaserBoy_3D_double _P1,
                                                            LaserBoy_3D_double  P0,
                                                            LaserBoy_3D_double _P0,
                                                            double              granularity
                                                           )
{
    double ratio,
           balance,
           all = size() - 1.0;
    p_space->p_GUI->display_state("applying _clip on frames");
    for(size_t i = 0; i < size(); i++)
    {
        ratio   = i / all;
        balance = 1.0 - ratio;
        at(i).clip(P1 * balance + _P1 * ratio,
                   P0 * balance + _P0 * ratio,
                   granularity
                  );
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::scale_on_coordinates(LaserBoy_3D_double point,
                                                                           LaserBoy_3D_double scale
                                                                          )
{
    p_space->p_GUI->display_state("applying scale on frames");
    if(size())
        for(size_t i = 0; i < size(); i++)
        {
            at(i).scale_on_coordinates(point, scale);
            p_space->p_GUI->display_progress(size() - i);
        }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::scale(LaserBoy_3D_double scale)
{
    p_space->p_GUI->display_state("applying scale on frames");
    if(size())
    {
        LaserBoy_3D_double center = rectangular_center_of(); // whole set
        for(size_t i = 0; i < size(); i++)
        {
            at(i).scale_on_coordinates(center, scale);
            p_space->p_GUI->display_progress(size() - i);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::rotate_around_origin(LaserBoy_3D_double angles)
{
    LaserBoy_3D_double origin;
    p_space->p_GUI->display_state("applying rotate on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).rotate_on_coordinates(origin, origin, 0.0, angles);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::rotate_on_coordinates(LaserBoy_3D_double fulcrum,
                                                                            LaserBoy_3D_double fulcrum_,
                                                                            double             fulcrum_acceleration,
                                                                            LaserBoy_3D_double angles
                                                                           )
{
    p_space->p_GUI->display_state("applying rotate on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).rotate_on_coordinates(fulcrum,
                                    fulcrum_,
                                    fulcrum_acceleration,
                                    angles
                                   );
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_rotate(LaserBoy_3D_double  fulcrum,  //  from
                                                              LaserBoy_3D_double _fulcrum,  // _from
                                                              LaserBoy_3D_double  fulcrum_, //  to
                                                              LaserBoy_3D_double _fulcrum_, // _to
                                                              double              fulcrum_acceleration,
                                                              double             _fulcrum_acceleration,
                                                              LaserBoy_3D_double  rotation,
                                                              LaserBoy_3D_double _rotation
                                                             )
{
    double              ratio,
                        dsize  =  size()  - 1.0;
    p_space->p_GUI->display_state("applying _rotate on frames");
    for(size_t i = 0; i < size(); i++)
    {
        ratio = i / dsize;
        at(i).rotate_on_coordinates(fulcrum              * (1.0 - ratio) + _fulcrum              * ratio,
                                    fulcrum_             * (1.0 - ratio) + _fulcrum_             * ratio,
                                    fulcrum_acceleration * (1.0 - ratio) + _fulcrum_acceleration * ratio,
                                    rotation             * (1.0 - ratio) + _rotation             * ratio
                                   );
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment_set::dimensions(LaserBoy_3D_double& real_offset, bool ignore_origin) const
{
    size_t             i,
                       j,
                       start    = 0;
    LaserBoy_3D_double real_min =  DBL_MAX,
                       real_max = -DBL_MAX;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("sizing vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() > 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)
                    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)
                    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)
                    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)
                    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)
                    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)
                    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
    real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
    real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));
    return LaserBoy_3D_double(real_max.x - real_min.x,
                              real_max.y - real_min.y,
                              real_max.z - real_min.z
                             );
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment_set::rectangular_center_of() const
{
    LaserBoy_3D_double center;
    dimensions(center, false);
    return center;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::center_x(bool individually)
{
    if(individually)
    {
        for(size_t i = 0; i < size(); i++)
            at(i).center_x();
    }
    else
    {
        LaserBoy_3D_double center;
        dimensions(center, false);
        for(size_t i = 0; i < size(); i++)
            at(i).move(LaserBoy_3D_double(-center.x, 0, 0));
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::center_y(bool individually)
{
    if(individually)
    {
        for(size_t i = 0; i < size(); i++)
            at(i).center_y();
    }
    else
    {
        LaserBoy_3D_double center;
        dimensions(center, false);
        for(size_t i = 0; i < size(); i++)
            at(i).move(LaserBoy_3D_double(0, -center.y, 0));
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::center_z(bool individually)
{
    if(individually)
    {
        for(size_t i = 0; i < size(); i++)
            at(i).center_z();
    }
    else
    {
        LaserBoy_3D_double center;
        dimensions(center, false);
        for(size_t i = 0; i < size(); i++)
            at(i).move(LaserBoy_3D_double(0, 0, -center.z));
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::center(bool individually)
{
    if(individually)
    {
        for(size_t i = 0; i < size(); i++)
            at(i).center();
    }
    else
    {
        LaserBoy_3D_double center;
        dimensions(center, false);
        for(size_t i = 0; i < size(); i++)
            at(i).move(-center);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::normalize(bool ignore_origin)
{
    int                 _x,
                        _y,
                        _z;
    size_t              i,
                        j,
                        start       =  0;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    LaserBoy_3D_double  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX,
                        real_offset =  0.0;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("sizing vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    if(p_space->maintain_real_origin)
    {
        if(fabs(real_max.x) > real_size)    real_size = fabs(real_max.x);
        if(fabs(real_min.x) > real_size)    real_size = fabs(real_min.x);
        if(fabs(real_max.y) > real_size)    real_size = fabs(real_max.y);
        if(fabs(real_min.y) > real_size)    real_size = fabs(real_min.y);
        if(fabs(real_max.z) > real_size)    real_size = fabs(real_max.z);
        if(fabs(real_min.z) > real_size)    real_size = fabs(real_min.z);
        real_size *= 2;
    }
    else // find the new center of the universe
    {
        real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
        real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
        real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));
        if(fabs(real_max.x - real_min.x) > real_size)
            real_size = fabs(real_max.x - real_min.x);
        if(fabs(real_max.y - real_min.y) > real_size)
            real_size = fabs(real_max.y - real_min.y);
        if(fabs(real_max.z - real_min.z) > real_size)
            real_size = fabs(real_max.z - real_min.z);
    }
    if(real_size)
        real_scale = 65535.0 / real_size;
    else
        real_scale = 1.0;
    //------------------------------------------------------------
    p_space->p_GUI->display_state("normalizing vectors");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                _x = (int)round((at(j)[i].x - real_offset.x) * real_scale);
                _y = (int)round((at(j)[i].y - real_offset.y) * real_scale);
                _z = (int)round((at(j)[i].z - real_offset.z) * real_scale);
                if(_x >  32767) _x =  32767;    if(_x < -32767) _x = -32767;
                if(_y >  32767) _y =  32767;    if(_y < -32767) _y = -32767;
                if(_z >  32767) _z =  32767;    if(_z < -32767) _z = -32767;
                at(j)[i].x = (double)(_x);
                at(j)[i].y = (double)(_y);
                at(j)[i].z = (double)(_z);
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    //--------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::normalize_vectors(bool ignore_origin)
{
    int                 _x,
                        _y,
                        _z;
    size_t              i,
                        j,
                        start       =  0;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    LaserBoy_3D_double  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX,
                        real_offset =  0.0;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("sizing vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
    real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
    real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));
    if(fabs(real_max.x - real_min.x) > real_size)
        real_size = fabs(real_max.x - real_min.x);
    if(fabs(real_max.y - real_min.y) > real_size)
        real_size = fabs(real_max.y - real_min.y);
    if(fabs(real_max.z - real_min.z) > real_size)
        real_size = fabs(real_max.z - real_min.z);
    if(real_size)
        real_scale = 65535.0 / real_size;
    else
        real_scale = 1.0;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("normalizing vectors");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                _x = (int)round((at(j)[i].x - real_offset.x) * real_scale);
                _y = (int)round((at(j)[i].y - real_offset.y) * real_scale);
                _z = (int)round((at(j)[i].z - real_offset.z) * real_scale);
                if(_x >  32767) _x =  32767;    if(_x < -32767) _x = -32767;
                if(_y >  32767) _y =  32767;    if(_y < -32767) _y = -32767;
                if(_z >  32767) _z =  32767;    if(_z < -32767) _z = -32767;
                at(j)[i].x = (double)(_x);
                at(j)[i].y = (double)(_y);
                at(j)[i].z = (double)(_z);
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::normalize_vectors_with_origin(bool ignore_origin)
{
    int                 _x,
                        _y,
                        _z;
    size_t              i,
                        j,
                        start       =  0;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    LaserBoy_3D_double  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("sizing vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    if(fabs(real_max.x) > real_size)    real_size = fabs(real_max.x);
    if(fabs(real_min.x) > real_size)    real_size = fabs(real_min.x);
    if(fabs(real_max.y) > real_size)    real_size = fabs(real_max.y);
    if(fabs(real_min.y) > real_size)    real_size = fabs(real_min.y);
    if(fabs(real_max.z) > real_size)    real_size = fabs(real_max.z);
    if(fabs(real_min.z) > real_size)    real_size = fabs(real_min.z);
    real_size *= 2;
    if(real_size)
        real_scale = 65535.0 / real_size;
    else
        real_scale = 1.0;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("normalizing vectors with origin");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                _x = (int)round((at(j)[i].x) * real_scale);
                _y = (int)round((at(j)[i].y) * real_scale);
                _z = (int)round((at(j)[i].z) * real_scale);
                if(_x >  32767) _x =  32767;    if(_x < -32767) _x = -32767;
                if(_y >  32767) _y =  32767;    if(_y < -32767) _y = -32767;
                if(_z >  32767) _z =  32767;    if(_z < -32767) _z = -32767;
                at(j)[i].x = (double)(_x);
                at(j)[i].y = (double)(_y);
                at(j)[i].z = (double)(_z);
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::to_unit(bool ignore_origin)
{
    size_t              i,
                        j,
                        start       =  0;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    LaserBoy_3D_double  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX,
                        real_offset =  0.0;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("to_unit vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++)
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    real_offset.x = (real_max.x - ((real_max.x - real_min.x) / 2));
    real_offset.y = (real_max.y - ((real_max.y - real_min.y) / 2));
    real_offset.z = (real_max.z - ((real_max.z - real_min.z) / 2));
    if(fabs(real_max.x - real_min.x) > real_size)
        real_size = fabs(real_max.x - real_min.x);
    if(fabs(real_max.y - real_min.y) > real_size)
        real_size = fabs(real_max.y - real_min.y);
    if(fabs(real_max.z - real_min.z) > real_size)
        real_size = fabs(real_max.z - real_min.z);
    if(real_size)
        real_scale = 2.0 / real_size;
    else
        real_scale = 1.0;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("to_unit vectors");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() > 2)
            for(i = start; i < at(j).size(); i++)
            {
                at(j)[i] *= real_scale;
                at(j)[i] -= (real_offset * real_scale);
            }
        p_space->p_GUI->display_progress(size() - j);
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::to_unit_with_origin(bool ignore_origin)
{
    size_t              i,
                        j,
                        start       =  0;
    double              real_size   =  0.0,
                        real_scale  =  1.0;
    LaserBoy_3D_double  real_min    =  DBL_MAX,
                        real_max    = -DBL_MAX;
    if(ignore_origin)
        start = 2;
    p_space->p_GUI->display_state("to_unit_with_origin vectors in frames");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() >= 2)
        {
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
            {
                if(at(j)[i].x > real_max.x)    real_max.x = at(j)[i].x;
                if(at(j)[i].x < real_min.x)    real_min.x = at(j)[i].x;
                if(at(j)[i].y > real_max.y)    real_max.y = at(j)[i].y;
                if(at(j)[i].y < real_min.y)    real_min.y = at(j)[i].y;
                if(at(j)[i].z > real_max.z)    real_max.z = at(j)[i].z;
                if(at(j)[i].z < real_min.z)    real_min.z = at(j)[i].z;
            }
        }
        p_space->p_GUI->display_progress(size() - j);
    }
    if(fabs(real_max.x) > real_size)    real_size = fabs(real_max.x);
    if(fabs(real_min.x) > real_size)    real_size = fabs(real_min.x);
    if(fabs(real_max.y) > real_size)    real_size = fabs(real_max.y);
    if(fabs(real_min.y) > real_size)    real_size = fabs(real_min.y);
    if(fabs(real_max.z) > real_size)    real_size = fabs(real_max.z);
    if(fabs(real_min.z) > real_size)    real_size = fabs(real_min.z);
    real_size *= 2;
    if(real_size)
        real_scale = 2.0 / real_size;
    else
        real_scale = 1.0;
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("to_unit_with_origin vectors");
    for(j = 0; j < size(); j++)
    {
        if(at(j).size() > 2)
            for(i = start; i < at(j).size(); i++) // ignore the origin vector
                at(j)[i] *= real_scale;
        p_space->p_GUI->display_progress(size() - j);
    }
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::reverse_frame_order()
{
    LaserBoy_real_segment_set reversed;
    if(size() > 1)
    {
        for(size_t i = 1; i <= size(); i++)
            reversed.push_back(at(size() - i));
        clear();
        insert(begin(), reversed.begin(), reversed.end());
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::reverse_vector_order()
{
    for(size_t i = 0; i < size(); i++)
        at(i).reverse();
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::swap_frames(int i, int j)
{
    if(    (i >= (int)size())
        || (i < 0)
        || (j >= (int)size())
        || (j < 0)
      )
        return *this;
    //------------------------------------------------------------------------
    LaserBoy_real_segment rs = at(i);
    at(i) = at(j);
    at(j) = rs;
    //------------------------------------------------------------------------
    return *this;
}

//############################################################################
void LaserBoy_real_segment_set::delete_selected_frames()
{
    if(number_of_selected_frames())
    {
        LaserBoy_real_segment_set reduced;
        for(size_t i = 0; i < size(); i++)
            if(!at(i).is_selected)
                reduced.push_back(at(i));
        if(reduced.size() == 0)
            reduced.push_back(LaserBoy_real_segment());
        *this = reduced;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::strip_color_rgb(const LaserBoy_color& c)
{
    p_space->p_GUI->display_state("coloring frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).strip_color_rgb(c);
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::index_hues(int index_multiple, int hues_shift)
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue indexing frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).index_hues(index_multiple, hues_offset);
        hues_offset += hues_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::span_hues(double span_factor, int hues_shift)
{
    static int hues_offset = 0;
    p_space->p_GUI->display_state("hue spanning frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).span_hues(span_factor, hues_offset);
        hues_offset += hues_shift;
        hues_offset %= 1530;
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::index_hues_set(int index_multiple)
{
    u_short hue_index = 0;
    p_space->p_GUI->display_state("hue indexing frame_set");
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        for(int j = 0; j < index_multiple; j++)
        {
            if((i + j) < size())
            {
                hue_index %= 1530;
                at(i + j).strip_color_rgb(LaserBoy_color(hue_index));
            }
        }
        p_space->p_GUI->display_progress(size() - i);
        hue_index++;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::span_hues_set(double span_factor)
{
    p_space->p_GUI->display_state("hue spanning frame_set");
    for(size_t i = 1; i < size(); i++)
    {
        at(i).strip_color_rgb(LaserBoy_color((u_short)(((int)(((i * span_factor) / (double)(size() - 1)) * 1530)) % 1530)));
        p_space->p_GUI->display_progress(size() - i);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment_set::sync_rgb_and_palette()
{
    for(size_t i = 0; i < size(); i++)
        at(i).sync_rgb_and_palette();
    return;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::move_  (LaserBoy_3D_double  displacement,
                                                              double              move_acceleration
                                                             )
{
    p_space->p_GUI->display_state("applying  move_   on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).move_(displacement, move_acceleration);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_move_ (LaserBoy_3D_double  displacement,
                                                              LaserBoy_3D_double _displacement,
                                                              double              move_acceleration,
                                                              double             _move_acceleration
                                                             )
{
    double  ratio = 1.0,
            dsize = size() - 1.0;
    p_space->p_GUI->display_state("applying _move_  on frames");
    if(     displacement   != 0.0
        || _displacement   != 0.0
      )
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            ratio = frame / dsize;
            at(frame).move_(displacement      * (1.0 - ratio) + _displacement      * ratio,
                            move_acceleration * (1.0 - ratio) + _move_acceleration * ratio
                           );
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::scale_ (LaserBoy_3D_double factor,
                                                              LaserBoy_3D_double factor_,
                                                              double             scale_acceleration
                                                             )
{
    p_space->p_GUI->display_state("applying  scale_  on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).scale_(factor, factor_, scale_acceleration);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_scale_ (LaserBoy_3D_double  factor,  //  from
                                                               LaserBoy_3D_double _factor,  // _from
                                                               LaserBoy_3D_double  factor_, //  to
                                                               LaserBoy_3D_double _factor_, // _to
                                                               double              scale_acceleration,
                                                               double             _scale_acceleration
                                                              )
{
    double  ratio = 1.0,
            dsize = size() - 1.0;
    p_space->p_GUI->display_state("applying _scale_  on frames");
    if(       factor  != 1.0
        ||   _factor  != 1.0
        ||    factor_ != 1.0
        ||   _factor_ != 1.0
      )
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            ratio = frame / dsize;
            at(frame).scale_(factor             * (1.0 - ratio) + _factor             * ratio,
                             factor_            * (1.0 - ratio) + _factor_            * ratio,
                             scale_acceleration * (1.0 - ratio) + _scale_acceleration * ratio
                            );
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::rotate_(LaserBoy_3D_double  fulcrum,
                                                              LaserBoy_3D_double  fulcrum_,
                                                              double              fulcrum_acceleration,
                                                              LaserBoy_3D_double  rotation,
                                                              double              rotate_acceleration
                                                             )
{
    p_space->p_GUI->display_state("applying  rotate_ on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).rotate_(fulcrum,
                            fulcrum_,
                            fulcrum_acceleration,
                            rotation,
                            rotate_acceleration
                           );
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_rotate_(LaserBoy_3D_double  fulcrum,  //  from
                                                               LaserBoy_3D_double _fulcrum,  // _from
                                                               LaserBoy_3D_double  fulcrum_, //  to
                                                               LaserBoy_3D_double _fulcrum_, // _to
                                                               double              fulcrum_acceleration,
                                                               double             _fulcrum_acceleration,
                                                               LaserBoy_3D_double  rotations,
                                                               LaserBoy_3D_double _rotations,
                                                               double              rotate_acceleration,
                                                               double             _rotate_acceleration
                                                              )
{
    double  ratio = 1.0,
            dsize = size() - 1.0;
    p_space->p_GUI->display_state("applying _rotate_ on frames");
    if(     rotations != 0.0
        || _rotations != 0.0
      )
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            ratio = frame / dsize;
            at(frame).rotate_(fulcrum               * (1.0 - ratio) + _fulcrum               * ratio,
                              fulcrum_              * (1.0 - ratio) + _fulcrum_              * ratio,
                              fulcrum_acceleration  * (1.0 - ratio) + _fulcrum_acceleration  * ratio,
                              rotations             * (1.0 - ratio) + _rotations             * ratio,
                              rotate_acceleration * (1.0 - ratio) + _rotate_acceleration * ratio
                             );
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::composite(LaserBoy_real_segment_set to_unite)
{
    double  dsize;
    p_space->p_GUI->display_state("applying composite   on frames");
    if(size() && to_unite.size())
    {
        if(size() == to_unite.size())
        {
            for(int frame = 0; frame < (int)size(); frame++)
            {
                at(frame) += to_unite.at(frame);
                p_space->p_GUI->display_progress(size() - frame);
            }
        }
        else if(size() > to_unite.size())
        {
            dsize = size() - 1;
            for(int frame = 0; frame < (int)size(); frame++)
            {
                at(frame) += to_unite.at((u_int)(frame / dsize * (to_unite.size() - 1)));
                p_space->p_GUI->display_progress((int)(dsize - frame));
            }
        }
        else
        {
            dsize = to_unite.size() - 1;
            for(int frame = 0; frame < (int)to_unite.size(); frame++)
            {
                to_unite.at(frame) = at((u_int)(frame / dsize * (size() - 1))) + to_unite.at(frame);
                p_space->p_GUI->display_progress((int)(dsize - frame));
            }
            *this = to_unite;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::add(LaserBoy_real_segment displacements)
{
    p_space->p_GUI->display_state("applying  add   on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).add(displacements);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_add(LaserBoy_real_segment_set rss_displacements)
{
    double dsize;
    p_space->p_GUI->display_state("applying _add   on frames");
    if(size() && rss_displacements.size())
    {
        if(size() == rss_displacements.size())
        {
            for(int frame = 0; frame < (int)size(); frame++)
            {
                at(frame).add(rss_displacements.at(frame));
                p_space->p_GUI->display_progress(size() - frame);
            }
        }
        else if(size() > rss_displacements.size())
        {
            dsize = size() - 1;
            for(int frame = 0; frame < (int)size(); frame++)
            {
                at(frame).add(rss_displacements.at((u_int)(frame / dsize * (rss_displacements.size() - 1))));
                p_space->p_GUI->display_progress((int)(dsize - frame));
            }
        }
        else
        {
            dsize = rss_displacements.size() - 1;
            for(int frame = 0; frame < (int)rss_displacements.size(); frame++)
            {
                rss_displacements.at(frame).add(at((u_int)(frame / dsize * (size() - 1))));
                p_space->p_GUI->display_progress((int)(dsize - frame));
            }
            *this = rss_displacements;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::multiply(LaserBoy_real_segment factors)
{
    p_space->p_GUI->display_state("applying  multiply  on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).multiply(factors);
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_multiply(LaserBoy_real_segment_set rss_multiply)
{
    p_space->p_GUI->display_state("applying _multiply  on frames");
    if(rss_multiply.size())
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            at(frame).multiply(rss_multiply.at((u_int)(((double)frame / (size() - 1)) * (rss_multiply.number_of_frames() - 1))));
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::warp(LaserBoy_3D_double    fulcrum,
                                                           LaserBoy_3D_double    fulcrum_,
                                                           double                fulcrum_acceleration,
                                                           LaserBoy_real_segment rs_rotations
                                                          )
{
    p_space->p_GUI->display_state("applying  warp  on frames");
    if(rs_rotations.size())
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            at(frame).warp(fulcrum,
                           fulcrum_,
                           fulcrum_acceleration,
                           rs_rotations
                          );
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}
//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_warp(LaserBoy_3D_double         fulcrum,
                                                            LaserBoy_3D_double         fulcrum_,
                                                            LaserBoy_3D_double        _fulcrum,
                                                            LaserBoy_3D_double        _fulcrum_,
                                                            double                     fulcrum_acceleration,
                                                            double                    _fulcrum_acceleration,
                                                            LaserBoy_real_segment_set rss_rotations
                                                           )
{
    double  ratio = 1.0;
    p_space->p_GUI->display_state("applying _warp on frames");
    if(rss_rotations.size())
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            at(frame).warp(fulcrum              * (1.0 - ratio) + _fulcrum              * ratio,
                           fulcrum_             * (1.0 - ratio) + _fulcrum_             * ratio,
                           fulcrum_acceleration * (1.0 - ratio) + _fulcrum_acceleration * ratio,
                           rss_rotations.at((u_int)(((double)frame / (size() - 1)) * (rss_rotations.number_of_frames() - 1)))
                          );
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::xyz_rgb(LaserBoy_real_segment rs_colors)
{
    p_space->p_GUI->display_state("applying  xyz_rgb    on frames");
    if(rs_colors.size())
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            at(frame).xyz_rgb(rs_colors);
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::_xyz_rgb(LaserBoy_real_segment_set rs_colors)
{
    p_space->p_GUI->display_state("applying _xyz_rgb    on frames");
    if(rs_colors.size())
    {
        for(int frame = 0; frame < (int)size(); frame++)
        {
            at(frame).xyz_rgb(rs_colors.at((u_int)(((double)frame / (size() - 1)) * (rs_colors.number_of_frames() - 1))));
            p_space->p_GUI->display_progress(size() - frame);
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::to_polar()
{
    p_space->p_GUI->display_state("applying to_polar         on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).to_polar();
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::differences()
{
    p_space->p_GUI->display_state("applying differences      on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).differences();
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::sums()
{
    p_space->p_GUI->display_state("applying sums             on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).sums();
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set& LaserBoy_real_segment_set::to_rectangular()
{
    p_space->p_GUI->display_state("applying to_rectangular   on frames");
    for(size_t i = 0; i < size(); i++)
    {
        at(i).to_rectangular();
        p_space->p_GUI->display_progress(size() - i);
    }
    return *this;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment_set::from_txt_file(const string& file_name, bool append)
{
    int                        line_number   = 1,
                               section_count = 0;
    string                     word;
    LaserBoy_palette           palette;
    LaserBoy_frame             frame;
    LaserBoy_real_segment      real_segment;
    //------------------------------------------------------------------------
    std::ifstream in       (file_name.c_str(), ios::in); // not binary!
    std::ofstream error_log("txt_in_real_segment_errors.txt", ios::out);
    if(in.is_open())
        real_segment_set_error = LASERBOY_OK;
    else
    {
        error_log << file_name
                  << " error: file does not exist"
                  << ENDL;
        error_log.close();
        real_segment_set_error |= LASERBOY_FILE_OPEN_FAILED;
        return real_segment_set_error;
    }
    //------------------------------------------------------------------------
    p_space->p_GUI->display_state("opening text file sections");
    while(in.good())
    {
        clear_to_alpha(in, line_number);
        if(get_next_word(in, word, line_number))
        {
            //----------------------------------------------------------------
            // frame
            //----------------------------------------------------------------
            if(word == "frame")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "xy") // 2D
                    {
                        // frame xy
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xy rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xy rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_RGB, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xy rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy rgb ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "hex")
                            {
                                // frame xy hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xy hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_HEX, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xy hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token.(frame xy hex ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "palette")
                            {
                                // frame xy palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xy palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_PALETTE, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xy palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. ( frame xy palette ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xy table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xy table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_2D_FRAME_TABLE, line_number);
                                        for(size_t i = 0; i < frame.size(); i++)
                                        {
                                            real_segment[i + 2].r = frame[i].r;
                                            real_segment[i + 2].g = frame[i].g;
                                            real_segment[i + 2].b = frame[i].b;
                                            real_segment[i + 2].c = frame[i].c;
                                            real_segment[i + 2].k = frame[i].k;
                                        }
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xy table real ERROR)"
                                                      << ENDL;
                                            real_segment_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xy table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xy table ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            } // end else if(word == "table")
                            else
                            {
                                // frame xy bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xy ERROR)"
                                          << ENDL;
                                real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                    }
                    else if(word == "xyz") // 3D
                    {
                        // frame xyz
                        if(get_next_word(in, word, line_number))
                        {
                            if(word == "rgb")
                            {
                                // frame xyz rgb
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xyz rgb real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_RGB, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xyz rgb bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz rgb ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "hex")
                            {
                                // frame xyz hex
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xyz hex real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_HEX, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xyz hex bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz hex ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "palette")
                            {
                                // frame xyz palette
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xyz palette real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_PALETTE, line_number);
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xyz palette bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz palette ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else if(word == "table")
                            {
                                // frame xyz table
                                if(get_next_word(in, word, line_number))
                                {
                                    if(    word == "real"
                                        || word == "short"
                                        || word == "unit"
                                      )
                                    {
                                        // frame xyz table real
                                        real_segment.from_ifstream_txt(in, LASERBOY_3D_FRAME_TABLE, line_number);
                                        if(real_segment.real_segment_error & LASERBOY_COLOR_VERTEX_MISSMATCH)
                                        {
                                            error_log << file_name
                                                      << " error before line: "
                                                      << line_number
                                                      << " color table to vertex count mismatch. (frame xyz table real ERROR)"
                                                      << ENDL;
                                            real_segment_set_error |= LASERBOY_COLOR_VERTEX_MISSMATCH;
                                        }
                                        for(size_t i = 0; i < frame.size(); i++)
                                        {
                                            real_segment[i + 2].r = frame[i].r;
                                            real_segment[i + 2].g = frame[i].g;
                                            real_segment[i + 2].b = frame[i].b;
                                            real_segment[i + 2].c = frame[i].c;
                                            real_segment[i + 2].k = frame[i].k;
                                        }
                                        push_back(real_segment);
                                    }
                                    else
                                    {
                                        // frame xyz table bad_word!
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is not a LaserBoy text format token. (frame xyz table ERROR)"
                                                  << ENDL;
                                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                                    }
                                }
                            }
                            else
                            {
                                // frame xyz bad_word!
                                error_log << file_name
                                          << " error on line: "
                                          << line_number
                                          << " \'"
                                          << word
                                          << "\' is not a LaserBoy text format token. (frame xyz ERROR)"
                                          << ENDL;
                                real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            }
                        }
                    }
                    else
                    {
                        // frame bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (frame ERROR)"
                                  << ENDL;
                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
            }
            //----------------------------------------------------------------
            // palette
            //----------------------------------------------------------------
            else if(word == "palette")
            {
                // palette
                if(get_next_word(in, word, line_number))
                {
                    if(word == "named")
                    {
                        if(get_next_word(in, word, line_number))
                        {   // palette named name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                    // this is what we expect.
                                    break;
                                case LASERBOY_OK:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' is not a valid LaserBoy palette name."
                                              << ENDL;
                                    error_log << file_name
                                              << " on line: "
                                              << line_number
                                              << " new named palette \'"
                                              << word
                                              << "\' added to set."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                    real_segment_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                    break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' is not a valid LaserBoy palette name."
                                              << ENDL;
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' palette name is more than 8 characters."
                                              << ENDL;
                                    error_log << file_name
                                              << " on line: "
                                              << line_number
                                              << " new named palette \'"
                                              << word
                                              << "\' added to set."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                    real_segment_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' is not a valid LaserBoy palette name."
                                              << ENDL;
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " new named palette \'"
                                              << word
                                              << "\' invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                        else
                        {
                            // palette named [no-name]
                            error_log << file_name
                                      << " error on line: "
                                      << line_number
                                      << " LaserBoy palette name is missing. (palette named [no-name])"
                                      << ENDL;
                            real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    // this is impossible
                                    break;
                                case LASERBOY_OK:
                                    error_log << file_name
                                              << " on line: "
                                              << line_number
                                              << " new named palette added to set."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                    }
                    else if(word == "rgb")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette rgb name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' is already named in the LaserBoy palette set."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
                                    break;
                                case LASERBOY_OK:
                                    // this is what we expect.
                                    break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' palette name is more than 8 characters."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                        else // palette rgb [no-name]
                        {
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    // impossible
                                    break;
                                case LASERBOY_OK:
                                    // this is what we expect.
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                    }
                    else if(word == "hex")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette hex name
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' is already named in the LaserBoy palette set."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
                                    break;
                                case LASERBOY_OK:
                                    // this is what we expect.
                                    break;
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " \'"
                                              << word
                                              << "\' palette name is more than 8 characters."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_NAME_TOO_LONG;
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                        else // palette hex [no-name]
                        {
                            switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                                    // impossible
                                    break;
                                case LASERBOY_OK:
                                    // this is what we expect.
                                    break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                    error_log << file_name
                                              << " error on line: "
                                              << line_number
                                              << " invalid palette data."
                                              << ENDL;
                                    real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                    break;
                            }
                        }
                    }
                    else
                    {
                        // palette bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (palette ERROR)"
                                  << ENDL;
                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number))
                else // there was no word after palette
                {
                    // palette rgb [no-name]
                    switch(p_space->palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                    {
                        case LASERBOY_PALETTE_NAME_IN_USE:
                        case LASERBOY_TXT_PALETTE_NAME_TOO_LONG:
                            // impossible
                            break;
                        case LASERBOY_OK:
                            // this is what we expect.
                            break;
                        case LASERBOY_TXT_PALETTE_FAILED:
                            error_log << file_name
                                      << " error on line: "
                                      << line_number
                                      << " invalid palette data."
                                      << ENDL;
                            real_segment_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                            break;
                    }
                }
            }
            //----------------------------------------------------------------
            // table
            //----------------------------------------------------------------
            else if(word == "table")
            {
                if(get_next_word(in, word, line_number))
                {
                    if(word == "rgb")
                        // table rgb
                        frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_RGB, line_number);
                    else if(word == "hex")
                        // table hex
                        frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_HEX, line_number);
                    else
                    {
                        // table bad_word!
                        error_log << file_name
                                  << " error on line: "
                                  << line_number
                                  << " \'"
                                  << word
                                  << "\' is not a LaserBoy text format token. (table ERROR)"
                                  << ENDL;
                        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                }
                else
                    // table rgb
                    frame.from_ifstream_txt(in, LASERBOY_COLOR_TABLE, LASERBOY_RGB, line_number);
            }
            //----------------------------------------------------------------
            //----------------------------------------------------------------
            else
            {
                // bad_word!
                error_log << file_name
                          << " error on line: "
                          << line_number
                          << " \'"
                          << word
                          << "\' is not a LaserBoy text format token. (ERROR)"
                          << ENDL;
                real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
            }
        } // end if(get_next_word(in, word, line_number))
        p_space->p_GUI->display_progress(++section_count);
    } // end while(in.good())
    if(section_count == 0)
    {
        error_log << file_name
                  << " contains nothing to render!"
                  << ENDL;
        real_segment_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
    }
    //------------------------------------------------------------------------
    in.close();
    error_log.close();
    return real_segment_set_error;
}

//############################################################################
bool LaserBoy_real_segment_set::save_as_txt(const string& file)
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        txt_tag(out);
        to_ofstream_txt(out);
        out.close();
    }
    else
        return false;
    return true;
}

//############################################################################
void LaserBoy_real_segment_set::to_ofstream_txt(std::ofstream& out)
{
    int  local_palette_index = LASERBOY_TRUE_COLOR;
    p_space->p_GUI->display_state("writing text file");
    for(size_t i = 0; i < size(); i++)
    {
        p_space->p_GUI->display_progress(size() - i);
        if(!p_space->save_txt_with_color)
        {
            if(    at(i).palette_index != local_palette_index
                && at(i).palette_index != LASERBOY_TRUE_COLOR
              )
            {
                if(    p_space->save_txt_named_palettes
                    || at(i).palette_index >= LASERBOY_BUILT_IN_PALETTES
                  )
                    p_space->palette_picker(at(i).palette_index).to_ofstream_txt(out);
                else
                {
                    out << "palette named "
                        << p_space->palette_picker(at(i).palette_index).name
                        << ENDL
                        << ENDL;
                }
                local_palette_index = at(i).palette_index;
            }
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                at(i).to_ofstream_txt_color_table(out);
        }
        //----------------------------------------------------------------
        at(i).to_ofstream_txt(out, i);
    }
    //------------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment_set::from_dxf_directory(const string& dir)
{
    LaserBoy_Error_Code  error_code = LASERBOY_OK;
    int                  opened_frames = 0;
    DIR                 *pDIR;
    struct dirent       *pDirEnt;
    vector<string>       file_list;
    pDIR = opendir(dir.c_str());
    if(pDIR == NULL)
        return LASERBOY_DIR_OPEN_FAILED;
    else
    {
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
            LaserBoy_real_segment real_vertices;
            // for systems that don't return file lists in alpha-numeric order!
            sort(file_list.begin(), file_list.end());
            p_space->p_GUI->display_state("opening DXF files");
            for(int i = 0; i < (int)file_list.size(); i++)
            {
                if(    file_list[i].size() >= 5
                    && iends_with(file_list[i], ".dxf")
                  ) // ?.dxf
                {
                    std::ifstream in((dir + '/' + file_list[i]).c_str(), ios::in); // each file
                    if(in.is_open())
                    {
                        error_code |= real_vertices.from_ifstream_dxf(in);
                        if(real_vertices.size() >= 2) // there is a picture in there
                        {
                            opened_frames++;
                            push_back(real_vertices);
                        }
                        real_vertices.clear();
                        in.close();
                        p_space->p_GUI->display_progress(file_list.size() - i);
                    }
                    else
                        error_code |= LASERBOY_FILE_OPEN_FAILED;
                }
            } // end for(int i = 0; i < (int)file_list.size(); i++)
        } // end if(file_list.size())
    } // end else of if(pDIR == NULL)
    if(!opened_frames)
        return error_code |= LASERBOY_NO_FRAMES_FOUND;
    return error_code;
}

//############################################################################
bool LaserBoy_real_segment_set::save_as_dxf_directory(const string& dir,
                                                      const string& name,
                                                      bool          true_color_dxf
                                                     ) const
{
    u_int frame_count = 0;
    bool  all_saved = true;
    char  number[16];
    if(!directory_exists((dir + name), p_space->absolute_path_of_LaserBoy))
    {
#if defined __MINGW_LASERBOY__
        mkdir((dir + name).c_str());
#else
        mkdir((dir + name).c_str(), 0777);
#endif
    }
    else
    {
        string                   last_file_name;
        vector<directory_entry>  directory_list;
        directory_iterator       end_itr;
        for(directory_iterator itr(dir + name); itr != end_itr; itr++)
            if(    is_regular_file(dir + name + '/' + itr->path().filename().string())
                && iends_with(itr->path().filename().string(), ".dxf")
              )
                directory_list.push_back(*itr);
        if(directory_list.size())
        {
            sort(directory_list.begin(), directory_list.end()); // ext4 does not return the list in alphanumeric order!
            last_file_name = directory_list[directory_list.size() - 1].path().filename().string();
            frame_count = atoi(last_file_name.substr(name.size() + 1, last_file_name.size() - (name.size() + 5)).c_str());
            frame_count++;
        }
    }
    p_space->p_GUI->display_state("writing DXF file");
    for(size_t i = 0; i < size(); i++)
    {
        sprintf(number, "%06d", frame_count);
        all_saved &= at(i).save_as_dxf(string(dir + name + '/' + name + '_' + number + ".dxf"), true_color_dxf);
        p_space->p_GUI->display_progress(size() - i);
        frame_count++;
    }
    return all_saved;
}

//############################################################################
bool LaserBoy_real_segment_set::save_as_bmp_directory(const string&      dir,
                                                      const string&      name,
                                                      u_int              x_res,
                                                      u_int              y_res,
                                                      bool               norm_frames_individually,
                                                      u_int              rendered_line_width,
                                                      bool               bmp_render_vectors,
                                                      bool               bmp_render_vertices
                                                     ) const
{
    u_int frame_count = 0;
    double scale = 0;
    char  number[16];
    LaserBoy_real_segment_set copy(*this);
    if(!directory_exists((dir + name), p_space->absolute_path_of_LaserBoy))
    {
#if defined __MINGW_LASERBOY__
        mkdir((dir + name).c_str());
#else
        mkdir((dir + name).c_str(), 0777);
#endif
    }
    else
    {
        string                   last_file_name;
        vector<directory_entry>  directory_list;
        directory_iterator       end_itr;
        for(directory_iterator itr(dir + name); itr != end_itr; itr++)
            if(    is_regular_file(dir + name + '/' + itr->path().filename().string())
                && iends_with(itr->path().filename().string(), ".bmp")
              )
                directory_list.push_back(*itr);
        if(directory_list.size())
        {
            sort(directory_list.begin(), directory_list.end()); // ext4 does not return the list in alphanumeric order!
            last_file_name = directory_list[directory_list.size() - 1].path().filename().string();
            frame_count = atoi(last_file_name.substr(name.size() + 1, last_file_name.size() - (name.size() + 5)).c_str());
            frame_count++;
        }
    }
    if(!norm_frames_individually)
    {
        copy.to_unit(false);
        LaserBoy_3D_double WxHxD,
                           center;
        WxHxD = copy.dimensions(center, false);
        scale =   (    x_res / WxHxD.x
                    <= y_res / WxHxD.y
                  )
                ? (x_res / WxHxD.x)
                : (y_res / WxHxD.y);
        copy.move(-center);
    }
    p_space->p_GUI->display_state("writing BMP file");
    for(size_t i = 0; i < copy.size(); i++)
    {
        sprintf(number, "%06d", frame_count);
        copy.at(i).save_as_bmp(string(dir + name + '/' + name + '_' + number + ".bmp"),
                               x_res,
                               y_res,
                               rendered_line_width,
                               bmp_render_vectors,
                               bmp_render_vertices,
                               scale
                              );
        p_space->p_GUI->display_progress(size() - i);
        frame_count++;
    }
    return true;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
