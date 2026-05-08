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
// LaserBoy_oscillator.cpp is part of LaserBoy.
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
#include "LaserBoy_oscillator.hpp"

//############################################################################
double LaserBoy_oscillator::sin_output(double time)
{
    double _phase = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x      = time * frequency + _phase,
           decay  = pow(_e_, -damping * time),
           fp,
           fn;
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(duty_cycle == 0)
        return -amplitude * sin(x / 2) * decay + offset;
    if(duty_cycle == 1)
        return  amplitude * sin(x / 2) * decay + offset;
    fp = 0.5 / duty_cycle;
    fn = 0.5 / (1 - duty_cycle);
    if(x > 0 && x < (duty_cycle * two_pi))
        return amplitude * sin(x * fp) * decay + offset;
    if(x >= (duty_cycle * two_pi) && x <= two_pi)
    {
        x -= (duty_cycle * two_pi);
        return amplitude * sin(x * -fn) * decay + offset;
    }
    return amplitude * sin(x) * decay + offset;
}

//############################################################################
double LaserBoy_oscillator::triangle_output(double time)
{
    double _phase  = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x       = time * frequency + _phase,
           decay   = pow(_e_, -damping * time),
           cross_x = duty_cycle * two_pi,
           peak_x  = cross_x / 2,
           dip_x   = (two_pi - cross_x) / 2;
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(duty_cycle == 0)
    {
        if(x >= 0 && x < pi)
            return amplitude * ((-1 / pi) * x) * decay + offset;
        if(x >= pi)
            return amplitude * ((1 / pi) * (x - pi) - 1) * decay + offset;
    }
    if(duty_cycle == 1)
    {
        if(x >= 0 && x < pi)
            return amplitude * ((1 / pi) * x) * decay + offset;
        if(x >= pi)
            return amplitude * ((-1 / pi) * (x - pi) + 1) * decay + offset;
    }
    if(x >= 0 && x < peak_x)
        return amplitude * ((1 / peak_x) * x) * decay + offset;
    if(x >= peak_x && x < cross_x)
        return amplitude * ((-1 / peak_x) * (x - peak_x) + 1) * decay + offset;
    if(x >= cross_x && x < (cross_x + dip_x))
        return amplitude * ((-1 / dip_x) * (x - (dip_x + cross_x)) - 1) * decay + offset;
    if(x >= (cross_x + dip_x) && x < two_pi)
        return amplitude * ((1 / dip_x) * (x - (cross_x + dip_x)) - 1) * decay + offset;
    return offset;
}

//############################################################################
double LaserBoy_oscillator::ramp_output(double time)
{
    double _phase = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x      = time * frequency + _phase,
           decay  = pow(_e_, -damping * time),
           peak_x;
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(x < pi) // positive phase
    {
        peak_x = (pi * duty_cycle);
        if(peak_x > x)
            return  amplitude * ((1 / peak_x) * x) * decay + offset;
        else if(peak_x < x)
            return  amplitude * ((-1 / (pi - peak_x)) * (x - peak_x) + 1) * decay + offset;
        else
            return  amplitude * decay + offset;
    }
    else if(x > pi)
    {
        peak_x = two_pi - pi * duty_cycle;
        if(peak_x > x)
            return  amplitude * ((-1 / (peak_x - pi)) * (x - pi)) * decay + offset;
        else if(peak_x < x)
            return  amplitude * ((1 / (two_pi - peak_x)) * (x - peak_x) - 1) * decay + offset;
        else
            return -amplitude * decay + offset;
    }
    else
        return offset;
}

//############################################################################
double LaserBoy_oscillator::square_output(double time)
{
    double _phase = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x      = time * frequency + _phase,
           decay  = pow(_e_, -damping * time);
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    return   ((duty_cycle * two_pi) < x)
           ? (-amplitude * decay + offset)
           : ( amplitude * decay + offset);
}

//############################################################################
double LaserBoy_oscillator::pulse_output(double time)
{
    double _phase  = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x       = time * frequency + _phase,
           decay   = pow(_e_, -damping * time),
           width   = duty_cycle * pi;
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(x == 0)
        return  offset;
    if(x > 0 && x <= width)
        return  amplitude * decay + offset;
    if(x > width && x < pi)
        return offset;
    if(x >= pi && x <= (pi + width))
        return -amplitude * decay + offset;
    if(x > (pi + width))
        return offset;
    return offset;
}

//############################################################################
double LaserBoy_oscillator::trapezoid_output(double time)
{
    double _phase  = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x       = time * frequency + _phase,
           decay   = pow(_e_, -damping * time),
           width   = duty_cycle * pi,
           p1      = half_pi - (width / 2),
           p2      = half_pi + (width / 2),
           p3      = pi_and_a_half - (width / 2),
           p4      = pi_and_a_half + (width / 2);
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(x == 0)
        return offset;
    if(p1 > 0 && x > 0 && x < p1)
        return amplitude * ((1 / p1) * x) * decay + offset;
    if(x >= p1 && x <= p2)
        return  amplitude * decay + offset;
    if(width < pi && x > p2 && x < p3)
        return amplitude * ((-2 / (pi - width)) * (x - p2) + 1) * decay + offset;
    if(x >= p3 && x <= p4)
        return offset - amplitude;
    if(p4 < two_pi && x > p4 && x < two_pi)
        return amplitude * ((1 / (two_pi - p4)) * (x - p4) - 1) * decay + offset;
    return offset;
}

//############################################################################
double LaserBoy_oscillator::circle_output(double time)
{
    double _phase   = (phase_cycle != 0.0) ? (phase * (two_pi / phase_cycle)) : (0.0),
           x        = time * frequency + _phase,
           decay    = pow(_e_, -damping * time),
           p_radius = duty_cycle * pi,
           n_radius = (1.0 - duty_cycle) * pi;
    while(x >= two_pi)
        x -= two_pi;
    while(x < 0)
        x += two_pi;
    if(duty_cycle == 0 && x <= pi)
        return    -amplitude
                * sqrt((pi * pi) - ((pi - x) * (pi - x))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(duty_cycle == 0 && x > pi)
        return    -amplitude
                * sqrt((pi * pi) - ((x - pi) * (x - pi))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(duty_cycle == 1 && x <= pi)
        return    amplitude
                * sqrt((pi * pi) - ((pi - x) * (pi - x))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(duty_cycle == 1 && x > pi)
        return    amplitude
                * sqrt((pi * pi) - ((x - pi) * (x - pi))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(x < p_radius)
        return    amplitude
                * sqrt((p_radius * p_radius) - ((p_radius - x) * (p_radius - x))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(x == p_radius)
        return  amplitude * decay + offset;
    if(x > p_radius && x < (p_radius * 2))
        return    amplitude
                * sqrt((p_radius * p_radius) - ((x - p_radius) * (x - p_radius))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(x == (p_radius * 2))
        return offset;
    if(x > (p_radius * 2) && x < ((p_radius * 2) + n_radius))
        return    -amplitude
                * sqrt((n_radius * n_radius) - ((((p_radius * 2) + n_radius) - x) * (((p_radius * 2) + n_radius) - x))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    if(x > ((p_radius * 2) + n_radius) && x <= two_pi)
        return    -amplitude
                * sqrt((n_radius * n_radius) - ((x - ((p_radius * 2) + n_radius)) * (x - ((p_radius * 2) + n_radius)))) / half_pi
                * decay
                + (amplitude * ((n_radius - p_radius) / pi))
                + offset;
    return offset;
}

//############################################################################
LaserBoy_oscillator LaserBoy_oscillator::blend(const LaserBoy_oscillator& lo, double ratio)
{
    double balance = 1.0 - ratio;
    return
    LaserBoy_oscillator (amplitude       * balance + lo.amplitude       * ratio,
                         frequency       * balance + lo.frequency       * ratio,
                         phase           * balance + lo.phase           * ratio,
                         duty_cycle      * balance + lo.duty_cycle      * ratio,
                         damping         * balance + lo.damping         * ratio,
                         offset          * balance + lo.offset          * ratio,
                         sin_level       * balance + lo.sin_level       * ratio,
                         triangle_level  * balance + lo.triangle_level  * ratio,
                         ramp_level      * balance + lo.ramp_level      * ratio,
                         square_level    * balance + lo.square_level    * ratio,
                         pulse_level     * balance + lo.pulse_level     * ratio,
                         trapezoid_level * balance + lo.trapezoid_level * ratio,
                         circle_level    * balance + lo.circle_level    * ratio,
                         phase_cycle
                        );
}

//############################################################################
bool LaserBoy_oscillator::function(string type)
{
    sin_level       = 0.0;
    triangle_level  = 0.0;
    ramp_level      = 0.0;
    square_level    = 0.0;
    pulse_level     = 0.0;
    trapezoid_level = 0.0;
    circle_level    = 0.0;
    if     (type == "sin")       { sin_level       = 1.0; return  true; }
    else if(type == "triangle")  { triangle_level  = 1.0; return  true; }
    else if(type == "ramp")      { ramp_level      = 1.0; return  true; }
    else if(type == "square")    { square_level    = 1.0; return  true; }
    else if(type == "pulse")     { pulse_level     = 1.0; return  true; }
    else if(type == "trapezoid") { trapezoid_level = 1.0; return  true; }
    else if(type == "circle")    { circle_level    = 1.0; return  true; }
    return false;
}

//############################################################################
void LaserBoy_oscillator::reset()
{
    amplitude       = 1.0;
    frequency       = 1.0;
    phase           = 0.0;
    duty_cycle      = 0.5;
    damping         = 0.0;
    offset          = 0.0;
    sin_level       = 1.0;
    triangle_level  = 0.0;
    ramp_level      = 0.0;
    square_level    = 0.0;
    pulse_level     = 0.0;
    trapezoid_level = 0.0;
    circle_level    = 0.0;
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################

