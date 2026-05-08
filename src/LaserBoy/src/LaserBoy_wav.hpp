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
// LaserBoy_wav.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_WAVE_DEFINITIONS__
#define __LASERBOY_WAVE_DEFINITIONS__

//############################################################################
#include "LaserBoy_common.hpp"

//############################################################################
class LaserBoy_wav_optimization_parameters
{
public:
    LaserBoy_wav_optimization_parameters()
              : lit_dwell_overhang     (-1), // -1 indicates undefined
                lit_delta_max          (-1),
                blank_delta_max        (-1),
                max_dwell_microsec     (-1),
                max_wag_microsec       (-1),
                insignificant_distance (-1),
                insignificant_angle    (-1),
                frames_per_second      (-1)
    {}
    //------------------------------------------------------------------------
    LaserBoy_wav_optimization_parameters(const LaserBoy_wav_optimization_parameters& parms)
              : lit_dwell_overhang     (parms.lit_dwell_overhang    ),
                lit_delta_max          (parms.lit_delta_max         ),
                blank_delta_max        (parms.blank_delta_max       ),
                max_dwell_microsec     (parms.max_dwell_microsec    ),
                max_wag_microsec       (parms.max_wag_microsec      ),
                insignificant_distance (parms.insignificant_distance),
                insignificant_angle    (parms.insignificant_angle   ),
                frames_per_second      (parms.frames_per_second     )

    {}
    //------------------------------------------------------------------------
   ~LaserBoy_wav_optimization_parameters()
    {}
    //------------------------------------------------------------------------
    int    lit_dwell_overhang;
    float  lit_delta_max,
           blank_delta_max,
           max_dwell_microsec,
           max_wag_microsec,
           insignificant_distance,
           insignificant_angle,
           frames_per_second;
};

//############################################################################
class LaserBoy_wav_header
{
public:
    LaserBoy_wav_header()
                  : num_channels       ( 0),
                    bits_per_sample    (16),
                    sample_rate        ( 0),
                    num_samples        ( 0),
                    num_frames         ( 0),
                    header_size        ( 0),
                    LaserBoy_wav_mode  (LASERBOY_WAVE_NO_MODE),
                    version            ("!LaserBoy!"),
                    file_name          (  ),
                    channel_map        (  ),
                    parms              (  )
    {
        u_int i;
        for(i = 0; i < 8; i++)
        {
            offset     [i] = 0;
            signal_id  [i] = LASERBOY_SIGNAL_UNDEFINED;
            LSB_tag    [i] = LASERBOY_LSB_NOT_USED;
            resolution [i] = 16;
        }
        for(i = 0; i < 256; i++)
        {
            color_rescale_r[i] =
            color_rescale_g[i] =
            color_rescale_b[i] =
            color_rescale_i[i] = i;
        }
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_header(const LaserBoy_wav_header& header);
    //------------------------------------------------------------------------
   ~LaserBoy_wav_header()
    {}
    //------------------------------------------------------------------------
    u_int    lboy_chunk_size  () const;
    //------------------------------------------------------------------------
    bool     from_fstream_wav (std::fstream& in, const string& file);
    void     to_fstream_wav   (std::fstream& out);
    //------------------------------------------------------------------------
    void     tell             (string label = "") const;
    //------------------------------------------------------------------------
    // data
    //------------------------------------------------------------------------
                                 u_short  num_channels   ,
                                          bits_per_sample;
                                   u_int  sample_rate,
                                          num_samples,
                                          num_frames ,
                                          header_size,
                                          LaserBoy_wav_mode; // up to 32 bit flags;
                                 u_short  color_rescale_r [256],
                                          color_rescale_g [256],
                                          color_rescale_b [256],
                                          color_rescale_i [256],
                                          LSB_tag         [  8];
                                     int  offset          [  8];
                                   short  signal_id       [  8];
                                  u_char  resolution      [  8];
                                  string  version,
                                          file_name,
                                          channel_map;
    LaserBoy_wav_optimization_parameters  parms;
};

//############################################################################
class LaserBoy_wav_sample
{
public:
    LaserBoy_wav_sample(u_int n)
                    : channel      (new short[n]),
                      num_channels (n           )
    {
        memset((void*)channel, 0x00, num_channels * sizeof(short));
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_sample(const LaserBoy_wav_sample& sample)
                    : channel      (new short[sample.num_channels]),
                      num_channels (sample.num_channels)
    {
        memcpy((void*)channel, (void*)sample.channel, num_channels * sizeof(short));
    }
    //------------------------------------------------------------------------
   ~LaserBoy_wav_sample()
    {
        delete channel;
    }
    //------------------------------------------------------------------------
    bool from_fstream_wav(std::fstream& in)
    {
        if(in.good())
        {
            in.read((char*)channel, num_channels * sizeof(signed short));
            return true;
        }
        return false; // returns false if EOF
    }
    //------------------------------------------------------------------------
    bool from_fstream_wav_inverted(std::fstream& in)
    {
        if(in.good())
        {
            in.read((char*)channel, num_channels * sizeof(signed short));
            negate();
            return true;
        }
        return false; // returns false if EOF
    }
    //------------------------------------------------------------------------
    bool from_fstream_wav_polarity(std::fstream& in, int polarity[8])
    {
        if(in.good())
        {
            in.read((char*)channel, num_channels * sizeof(signed short));
            for(u_int i = 0; i < num_channels; i++)
                if(polarity[i] == 1) // invert
                {
                    if(channel[i] >= LASERBOY_MIN_SHORT)
                        channel[i] = -channel[i];
                    else
                        channel[i] = LASERBOY_MAX_SHORT; // there is no +32768!
                }
            return true;
        }
        return false; // returns false if EOF
    }
    //------------------------------------------------------------------------
    bool from_fstream_wav_bit_resolution(std::fstream& in, u_int bit_mask[8])
    {
        if(in.good())
        {
            in.read((char*)channel, num_channels * sizeof(signed short));
            for(u_int i = 0; i < num_channels; i++)
                channel[i] &= bit_mask[i];
            return true;
        }
        return false; // returns false if EOF
    }
    //------------------------------------------------------------------------
    void to_fstream_wav(std::fstream& out, LaserBoy_wav_header& header)
    {
        out.write((char*)channel, num_channels * sizeof(signed short));
        header.num_samples++;
        return;
    }
    //------------------------------------------------------------------------
    void to_fstream_wav_inverted(std::fstream& out, LaserBoy_wav_header& header)
    {
        negate();
        out.write((char*)channel, num_channels * sizeof(signed short));
        negate();
        header.num_samples++;
        return;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_sample& operator = (const LaserBoy_wav_sample& sample)
    {
        delete channel;
        num_channels = sample.num_channels;
        channel = new short[num_channels];
        memcpy((void*)channel, (void*)sample.channel, num_channels * sizeof(short));
        return *this;
    }
    //------------------------------------------------------------------------
    void negate()
    {
        for(u_int i = 0; i < num_channels; i++)
        {
            if(channel[i] >= LASERBOY_MIN_SHORT)
                channel[i] = -channel[i];
            else
                channel[i] = LASERBOY_MAX_SHORT; // there is no +32768!
        }
        return;
    }
    //------------------------------------------------------------------------
    LaserBoy_wav_sample  operator + (const LaserBoy_wav_sample& s) // side-by-side as tracks
    {
        LaserBoy_wav_sample sum(num_channels + s.num_channels);
        memcpy(   (void*)sum.channel,
                  (void*)channel,
                  num_channels * sizeof(short)
              );
        memcpy(   (void*)(sum.channel + num_channels), // pointer math!
                  (void*)s.channel,
                  s.num_channels * sizeof(short)
              );
        return sum;
    }
    //------------------------------------------------------------------------
    signed short   *channel     ;
    u_int           num_channels;
};

//############################################################################
typedef LaserBoy_wav_sample*  LaserBoy_wav_sample_pointer;

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
