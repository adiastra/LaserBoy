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
// LaserBoy_oscillator.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_OSCILLATOR_DEFINITIONS__
#define __LASERBOY_OSCILLATOR_DEFINITIONS__

//############################################################################
#include "LaserBoy_common.hpp"

//############################################################################
class LaserBoy_oscillator
{
public:
    LaserBoy_oscillator()
    : amplitude       (  1.0),
      frequency       (  1.0),
      phase           (  0.0),
      duty_cycle      (  0.5),
      damping         (  0.0),
      offset          (  0.0),
      sin_level       (  1.0),
      triangle_level  (  0.0),
      ramp_level      (  0.0),
      square_level    (  0.0),
      pulse_level     (  0.0),
      trapezoid_level (  0.0),
      circle_level    (  0.0),
      phase_cycle     (360.0)
    {}
    //------------------------------------------------------------------------
    LaserBoy_oscillator(double _amplitude,
                        double _frequency,
                        double _phase,
                        double _duty_cycle,
                        double _damping,
                        double _offset,
                        double _sin_level,
                        double _triangle_level,
                        double _ramp_level,
                        double _square_level,
                        double _pulse_level,
                        double _trapezoid_level,
                        double _circle_level,
                        double _phase_cycle
                       )
    : amplitude       (_amplitude      ),
      frequency       (_frequency      ),
      phase           (_phase          ),
      duty_cycle      (_duty_cycle     ),
      damping         (_damping        ),
      offset          (_offset         ),
      sin_level       (_sin_level      ),
      triangle_level  (_triangle_level ),
      ramp_level      (_ramp_level     ),
      square_level    (_square_level   ),
      pulse_level     (_pulse_level    ),
      trapezoid_level (_trapezoid_level),
      circle_level    (_circle_level   ),
      phase_cycle     (_phase_cycle    )
    {}
    //------------------------------------------------------------------------
    LaserBoy_oscillator(const LaserBoy_oscillator& lo)
    : amplitude       (lo.amplitude      ),
      frequency       (lo.frequency      ),
      phase           (lo.phase          ),
      duty_cycle      (lo.duty_cycle     ),
      damping         (lo.damping        ),
      offset          (lo.offset         ),
      sin_level       (lo.sin_level      ),
      triangle_level  (lo.triangle_level ),
      ramp_level      (lo.ramp_level     ),
      square_level    (lo.square_level   ),
      pulse_level     (lo.pulse_level    ),
      trapezoid_level (lo.trapezoid_level),
      circle_level    (lo.circle_level   ),
      phase_cycle     (lo.phase_cycle    )
    {}
    //------------------------------------------------------------------------
   ~LaserBoy_oscillator()
    {}
    //------------------------------------------------------------------------
    LaserBoy_oscillator& operator = (const LaserBoy_oscillator& lo)
    {
        amplitude       = lo.amplitude      ;
        frequency       = lo.frequency      ;
        phase           = lo.phase          ;
        duty_cycle      = lo.duty_cycle     ;
        damping         = lo.damping        ;
        offset          = lo.offset         ;
        sin_level       = lo.sin_level      ;
        triangle_level  = lo.triangle_level ;
        ramp_level      = lo.ramp_level     ;
        square_level    = lo.square_level   ;
        pulse_level     = lo.pulse_level    ;
        trapezoid_level = lo.trapezoid_level;
        circle_level    = lo.circle_level   ;
        phase_cycle     = lo.phase_cycle    ;
        return *this;
    }
    //------------------------------------------------------------------------
    double sin_output        (double time);
    double triangle_output   (double time);
    double ramp_output       (double time);
    double square_output     (double time);
    double pulse_output      (double time);
    double trapezoid_output  (double time);
    double circle_output     (double time);
    //------------------------------------------------------------------------
    double operator ()       (double time)
    {
        double level_total =    sin_level
                              + triangle_level
                              + ramp_level
                              + square_level
                              + pulse_level
                              + trapezoid_level
                              + circle_level;

        return   (sin_level       / level_total) * sin_output       (time)
               + (triangle_level  / level_total) * triangle_output  (time)
               + (ramp_level      / level_total) * ramp_output      (time)
               + (square_level    / level_total) * square_output    (time)
               + (pulse_level     / level_total) * pulse_output     (time)
               + (trapezoid_level / level_total) * trapezoid_output (time)
               + (circle_level    / level_total) * circle_output    (time);
    }
    //------------------------------------------------------------------------
    LaserBoy_oscillator blend(const LaserBoy_oscillator& lo, double ratio);
    //------------------------------------------------------------------------
    bool  function           (string type);
    void  reset              ();
    //------------------------------------------------------------------------
    double amplitude,
           frequency,
           phase,
           duty_cycle,
           damping,
           offset,
           sin_level,
           triangle_level,
           ramp_level,
           square_level,
           pulse_level,
           trapezoid_level,
           circle_level,
           phase_cycle;
};

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################

