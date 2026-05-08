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
// LaserBoy_wav.cpp is part of LaserBoy.
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
#include "LaserBoy_wav.hpp"

//############################################################################
LaserBoy_wav_header::LaserBoy_wav_header(const LaserBoy_wav_header& header)
  : num_channels      (header.num_channels     ),
    bits_per_sample   (header.bits_per_sample  ),
    sample_rate       (header.sample_rate      ),
    num_samples       (header.num_samples      ),
    num_frames        (header.num_frames       ),
    header_size       (header.header_size      ),
    LaserBoy_wav_mode (header.LaserBoy_wav_mode),
    version           (header.version          ),
    file_name         (header.file_name        ),
    channel_map       (header.channel_map      ),
    parms             (header.parms            )
{
    memcpy((void*)offset         , (void*)header.offset         , sizeof(int)    * header.num_channels);
    memcpy((void*)resolution     , (void*)header.resolution     , sizeof(u_char) * header.num_channels);
    memcpy((void*)signal_id      , (void*)header.signal_id      , sizeof(short)  * header.num_channels);
    memcpy((void*)LSB_tag        , (void*)header.LSB_tag        , sizeof(short)  * header.num_channels);
    memcpy((void*)color_rescale_r, (void*)header.color_rescale_r, sizeof(short)  * 256);
    memcpy((void*)color_rescale_g, (void*)header.color_rescale_g, sizeof(short)  * 256);
    memcpy((void*)color_rescale_b, (void*)header.color_rescale_b, sizeof(short)  * 256);
    memcpy((void*)color_rescale_i, (void*)header.color_rescale_i, sizeof(short)  * 256);
}

//############################################################################
u_int LaserBoy_wav_header::lboy_chunk_size() const
{
    u_int LB_header_bytes = 20;
    if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
        LB_header_bytes += (4 * num_channels);
    if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED)
        LB_header_bytes += 32;
    if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
        LB_header_bytes += (4 * num_channels);
    if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
        LB_header_bytes += (2 * num_channels);
    if(LaserBoy_wav_mode & LASERBOY_WAVE_NUM_FRAMES)
        LB_header_bytes += 4;
    if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
        LB_header_bytes += 512;
    if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
        LB_header_bytes += 512;
    if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
        LB_header_bytes += 512;
    if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
        LB_header_bytes += 512;
    return LB_header_bytes;
}

//############################################################################
bool LaserBoy_wav_header::from_fstream_wav(std::fstream& in, const string& file)
{
    char    a,
            b,
            c,
            d;
    short   audio_format,
            block_align;
    u_int   i,
            chunk_size          = 0,
            sub_chunk_1_size    = 0,
            sub_chunk_2_size    = 0,
            sub_chunk_3_size    = 0,
            LaserBoy_chunk_size = 0,
            byte_rate;
    string  temp_string,
            version_guess;
    //------------------------------------------------------------------------
    file_name         = file;
    version           = "!LaserBoy!"; // assume the worst!
    version_guess     = "!LaserBoy!";
    LaserBoy_wav_mode =   LASERBOY_WAVE_POSITIVE;
    //------------------------------------------------------------------------
    in.get(a); if(a != 'R') return false;
    in.get(a); if(a != 'I') return false;
    in.get(a); if(a != 'F') return false;
    in.get(a); if(a != 'F') return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    chunk_size = (   (a & 0x000000ff)
                   | (b & 0x000000ff) << 8
                   | (c & 0x000000ff) << 16
                   | (d & 0x000000ff) << 24
                 );
    //------------------------------------------------------------------------
    in.get(a); if(a != 'W') return false;
    in.get(a); if(a != 'A') return false;
    in.get(a); if(a != 'V') return false;
    in.get(a); if(a != 'E') return false;
    //------------------------------------------------------------------------
    in.get(a);
    if(a == 'f')
    {
        in.get(a); if(a != 'm') return false;
        in.get(a); if(a != 't') return false;
        in.get(a); if(a != ' ') return false;
    }
    else if(a == 'L')
    {
        in.get(a); if(a != 'I') return false;
        in.get(a); if(a != 'S') return false;
        in.get(a); if(a != 'T') return false;
        in.get(a); in.get(b); in.get(c); in.get(d);
        sub_chunk_3_size = (   (a & 0x000000ff)
                             | (b & 0x000000ff) << 8
                             | (c & 0x000000ff) << 16
                             | (d & 0x000000ff) << 24
                           );
        in.seekg(sub_chunk_3_size, ios::cur); // jump over the whole LIST chunk!
        in.get(a); if(a != 'f') return false;
        in.get(a); if(a != 'm') return false;
        in.get(a); if(a != 't') return false;
        in.get(a); if(a != ' ') return false;
    }
    else
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sub_chunk_1_size = (   (a & 0x000000ff)
                         | (b & 0x000000ff) << 8
                         | (c & 0x000000ff) << 16
                         | (d & 0x000000ff) << 24
                       );
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    audio_format = ((b & 0x00ff) << 8 | (a & 0x00ff));
    if(audio_format != 1) return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    num_channels = ((b & 0x00ff) << 8 | (a & 0x00ff));
    if(num_channels < 2)
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sample_rate = (   (a & 0x000000ff)
                    | (b & 0x000000ff) << 8
                    | (c & 0x000000ff) << 16
                    | (d & 0x000000ff) << 24
                  );
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    byte_rate = (   (a & 0x000000ff)
                  | (b & 0x000000ff) << 8
                  | (c & 0x000000ff) << 16
                  | (d & 0x000000ff) << 24
                );
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    block_align = ((b & 0x00ff) << 8 | (a & 0x00ff));
    //------------------------------------------------------------------------
    in.get(a); in.get(b);
    bits_per_sample = ((b & 0x00ff) << 8 | (a & 0x00ff));
    if(bits_per_sample != 16)
        return false;
    //------------------------------------------------------------------------
    if(byte_rate != (sample_rate * num_channels * (bits_per_sample / 8)))
        return false;
    //------------------------------------------------------------------------
    if(block_align != (num_channels * (bits_per_sample / 8)))
        return false;
    //------------------------------------------------------------------------
    // look for LaserBoy
    //------------------------------------------------------------------------
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    in.get(a);    temp_string.push_back(a);
    //------------------------------------------------------------------------
    if(temp_string == "LBoy")
    {
        //--------------------------------------------------------------------
        in.get(a); in.get(b); in.get(c); in.get(d);
        LaserBoy_chunk_size = (   (a & 0x000000ff)
                                | (b & 0x000000ff) << 8
                                | (c & 0x000000ff) << 16
                                | (d & 0x000000ff) << 24
                              );
        //--------------------------------------------------------------------
        temp_string.clear();
        in.get(a);    temp_string.push_back(a); // L
        in.get(a);    temp_string.push_back(a); // a
        in.get(a);    temp_string.push_back(a); // s
        in.get(a);    temp_string.push_back(a); // e
        in.get(a);    temp_string.push_back(a); // r
        in.get(a);    temp_string.push_back(a); // B
        in.get(a);    temp_string.push_back(a); // o
        in.get(a);    temp_string.push_back(a); // y
    }
    else if(temp_string == "Lase")
    {
        in.get(a);    temp_string.push_back(a); // r
        in.get(a);    temp_string.push_back(a); // B
        in.get(a);    temp_string.push_back(a); // o
        in.get(a);    temp_string.push_back(a); // y
    }
    else
        in.seekg(-4, ios::cur); // Oops! put it back!
    //------------------------------------------------------------------------
    if(temp_string == "LaserBoy")
    {
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        in.get(a);    temp_string.push_back(a);
        //----------------------------------------------------------------
        if(    temp_string != "LaserBoy06072008"
            && temp_string != "LaserBoy07132008"
            && temp_string != "LaserBoy10102008"
            && temp_string != "LaserBoy11112008"
            && temp_string != "LaserBoy12082009"
            && temp_string != "LaserBoy06282010" // 13.5 years ago!
            && temp_string != LASERBOY_WAVE_VERSION // "LaserBoy20231223"
          )
            return false;
        //----------------------------------------------------------------
        version_guess = temp_string;
        //----------------------------------------------------------------
        in.get(a); in.get(b); in.get(c); in.get(d);
        LaserBoy_wav_mode = (   (a & 0x000000ff)
                              | (b & 0x000000ff) << 8
                              | (c & 0x000000ff) << 16
                              | (d & 0x000000ff) << 24
                            );
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
        {
            for(i = 0; i < num_channels; i++)
            {
                in.get(a); in.get(b); in.get(c); in.get(d);
                offset[i] = (   (a & 0x000000ff)
                              | (b & 0x000000ff) << 8
                              | (c & 0x000000ff) << 16
                              | (d & 0x000000ff) << 24
                            );
            }
        } // end if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            in.get(a); in.get(b); in.get(c); in.get(d);
            parms.lit_dwell_overhang = (   (a & 0x000000ff)
                                         | (b & 0x000000ff) << 8
                                         | (c & 0x000000ff) << 16
                                         | (d & 0x000000ff) << 24
                                       );
            //----------------------------------------------------------------
            if(version_guess == "LaserBoy06072008")
            {   // floats were stored backwards. Oops!
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                parms.blank_delta_max = parms.lit_delta_max;
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(a); in.get(b); in.get(c); in.get(d);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            } // end if(version_guess == "LaserBoy06072008")
            //----------------------------------------------------------------
            else if(version_guess == "LaserBoy10102008")
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                parms.blank_delta_max = parms.lit_delta_max;
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            } // end else if(version_guess == "LaserBoy10102008")
            //----------------------------------------------------------------
            else if(    version_guess == "LaserBoy11112008"
                     || version_guess == "LaserBoy12082009"
                   )
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.blank_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            } // end else if(version_guess == "LaserBoy11112008" || "LaserBoy12082009")
            //----------------------------------------------------------------
            else // since "LaserBoy06282010"
            {
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.lit_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.blank_delta_max = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_dwell_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.max_wag_microsec = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_distance = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.insignificant_angle = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
                in.get(d); in.get(c); in.get(b); in.get(a);
                parms.frames_per_second = (float)
                                           (   (a & 0x000000ff)
                                             | (b & 0x000000ff) << 8
                                             | (c & 0x000000ff) << 16
                                             | (d & 0x000000ff) << 24
                                           );
            }
        } // end if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED)
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
        {
            for(i = 0; i < num_channels; i++)
            {
                in.get(a); in.get(b);
                signal_id[i] = ((b & 0x00ff) << 8 | (a & 0x00ff));
                in.get(a); in.get(b);
                LSB_tag  [i] = ((b & 0x00ff) << 8 | (a & 0x00ff));
            }
        } // end if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
        else if(num_channels >= 6)
        {
            signal_id[0] = LASERBOY_SIGNAL_X_POSITION;
            signal_id[1] = LASERBOY_SIGNAL_Y_POSITION;
            signal_id[2] = LASERBOY_SIGNAL_RED_ANALOG;
            signal_id[3] = LASERBOY_SIGNAL_GREEN_ANALOG;
            signal_id[4] = LASERBOY_SIGNAL_BLUE_ANALOG;
            if(offset[5] == 0)
                signal_id[5] = LASERBOY_SIGNAL_Z_POSITION;
            LSB_tag  [2] = LASERBOY_LSB_END_OF_FRAME;
            LSB_tag  [3] = LASERBOY_LSB_UNIQUE_FRAME;
        }
        //----------------------------------------------------------------
        if(    temp_string != "LaserBoy06072008"
            && temp_string != "LaserBoy07132008"
            && temp_string != "LaserBoy10102008"
            && temp_string != "LaserBoy11112008"
            && temp_string != "LaserBoy12082009"
            && temp_string != "LaserBoy06282010" // 13.5 years ago!
          )
        { // it is later than "LaserBoy06282010"
            if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
                for(i = 0; i < num_channels; i++)
                {
                    in.get(a); in.get(b); // 16 bits!
                    resolution[i] = (u_char)a;
                }
        } // end later than "LaserBoy06282010"
        else // "LaserBoy06282010" or before
        {
            if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
                for(i = 0; i < num_channels; i++)
                {
                    in.get(a);
                    resolution[i] = (u_char)a;
                }
        } // end "LaserBoy06282010" or before
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            in.get(a); in.get(b); in.get(c); in.get(d);
            num_frames = (   (a & 0x000000ff)
                           | (b & 0x000000ff) << 8
                           | (c & 0x000000ff) << 16
                           | (d & 0x000000ff) << 24
                         );
        } // end if(LaserBoy_wav_mode & LASERBOY_WAVE_NUM_FRAMES)
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R && in.good())
            in.read((char*)color_rescale_r, 256 * sizeof(signed short));
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G && in.good())
            in.read((char*)color_rescale_g, 256 * sizeof(signed short));
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B && in.good())
            in.read((char*)color_rescale_b, 256 * sizeof(signed short));
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I && in.good())
            in.read((char*)color_rescale_i, 256 * sizeof(signed short));
        //----------------------------------------------------------------
        channel_map.clear();
        for(i = 0; i < num_channels; i++)
        {
            switch((int)abs(signal_id[i]))
            {
                case LASERBOY_SIGNAL_X_POSITION:
                    channel_map.push_back('X');
                    break;
                case LASERBOY_SIGNAL_Y_POSITION:
                    channel_map.push_back('Y');
                    break;
                case LASERBOY_SIGNAL_Z_POSITION:
                    channel_map.push_back('Z');
                    break;
                case LASERBOY_SIGNAL_RED_ANALOG:
                    channel_map.push_back('r');
                    break;
                case LASERBOY_SIGNAL_GREEN_ANALOG:
                    channel_map.push_back('g');
                    break;
                case LASERBOY_SIGNAL_BLUE_ANALOG:
                    channel_map.push_back('b');
                    break;
                case LASERBOY_SIGNAL_MONO_TTL:
                case LASERBOY_SIGNAL_MONO_OR_ANALOG:
                case LASERBOY_SIGNAL_MONO_WEIGHTED_ANALOG:
                case LASERBOY_SIGNAL_MONO_AVG_ANALOG:
                case LASERBOY_SIGNAL_MONO_O_SCOPE:
                    channel_map.push_back('i');
                    break;
                case LASERBOY_NO_SIGNAL:
                    channel_map.push_back('0');
                    break;
                default:
                    channel_map.push_back('?');
                    break;
            } // end switch((int)abs(signal_id[i]))
        } // end for(i = 0; i < num_channels; i++)
    } // end if(temp_string == "LaserBoy")
    //------------------------------------------------------------------------
    temp_string.clear();
    in.get(a);    temp_string.push_back(a); // d
    in.get(a);    temp_string.push_back(a); // a
    in.get(a);    temp_string.push_back(a); // t
    in.get(a);    temp_string.push_back(a); // a
    //------------------------------------------------------------------------
    if(temp_string != "data")
    {
        d = ' ';
        while(d != 'a' && in.good())
        {
            c = ' ';
            while(c != 't' && in.good())
            {
                b = ' ';
                while(b != 'a' && in.good())
                {
                    a = ' ';
                    while(a != 'd' && in.good())
                        in.get(a);
                    in.get(b);
                }
                in.get(c);
            }
            in.get(d);
        }
    }
    if(!in.good())
        return false;
    //------------------------------------------------------------------------
    in.get(a); in.get(b); in.get(c); in.get(d);
    sub_chunk_2_size = (   (a & 0x000000ff)
                         | (b & 0x000000ff) << 8
                         | (c & 0x000000ff) << 16
                         | (d & 0x000000ff) << 24
                       );
    //------------------------------------------------------------------------
    header_size = in.tellg();
    num_samples = sub_chunk_2_size / (num_channels * (bits_per_sample / 8));
    version     = version_guess; // Woohoo !!!
    //------------------------------------------------------------------------
    return true;
}

//############################################################################
void LaserBoy_wav_header::to_fstream_wav(std::fstream& out)
{
    short   audio_format          = 1,
            block_align           = num_channels * (bits_per_sample / 8);

    int     i,
            sub_chunk_1_size      = 16, // standard wav Subchunk1Size
            LaserBoy_chunk_size   = 0,
            sub_chunk_2_size      = num_samples * num_channels * (bits_per_sample / 8),
            byte_rate             = sample_rate * num_channels * (bits_per_sample / 8),
            chunk_size            = 20 + sub_chunk_1_size + sub_chunk_2_size;
    //------------------------------------------------------------------------
    if(version != "!LaserBoy!") // it is a LaserBoy wav.
    {
        LaserBoy_chunk_size = lboy_chunk_size();
        chunk_size += (   4 // sizeof "LBoy"
                        + sizeof(int) // sizeof (int)LaserBoy_chunk_size
                        + LaserBoy_chunk_size
                      );
    }
    //------------------------------------------------------------------------
    out.seekp(0, ios::beg);
    //------------------------------------------------------------------------
    out.put('R');
    out.put('I');
    out.put('F');
    out.put('F');

    out.put((char) (chunk_size & 0x000000ff)       );
    out.put((char)((chunk_size & 0x0000ff00) >> 8 ));
    out.put((char)((chunk_size & 0x00ff0000) >> 16));
    out.put((char)((chunk_size & 0xff000000) >> 24));

    out.put('W');
    out.put('A');
    out.put('V');
    out.put('E');

    out.put('f');
    out.put('m');
    out.put('t');
    out.put(' ');

    out.put((char) (sub_chunk_1_size & 0x000000ff)       );
    out.put((char)((sub_chunk_1_size & 0x0000ff00) >> 8 ));
    out.put((char)((sub_chunk_1_size & 0x00ff0000) >> 16));
    out.put((char)((sub_chunk_1_size & 0xff000000) >> 24));

    out.put((char) (audio_format & 0x00ff)      );
    out.put((char)((audio_format & 0xff00) >> 8)); // 2

    out.put((char) (num_channels & 0x00ff)      );
    out.put((char)((num_channels & 0xff00) >> 8)); // 4

    out.put((char) (sample_rate & 0x000000ff)       );
    out.put((char)((sample_rate & 0x0000ff00) >> 8 ));
    out.put((char)((sample_rate & 0x00ff0000) >> 16));
    out.put((char)((sample_rate & 0xff000000) >> 24)); // 8

    out.put((char) (byte_rate & 0x000000ff)       );
    out.put((char)((byte_rate & 0x0000ff00) >> 8 ));
    out.put((char)((byte_rate & 0x00ff0000) >> 16));
    out.put((char)((byte_rate & 0xff000000) >> 24)); // 12

    out.put((char) (block_align & 0x00ff)      );
    out.put((char)((block_align & 0xff00) >> 8)); // 14

    out.put((char) (bits_per_sample & 0x00ff)      );
    out.put((char)((bits_per_sample & 0xff00) >> 8)); // 16
//----------------------------------------------------------------------------
// information added for LaserBoy !!!
//----------------------------------------------------------------------------
    if(version != "!LaserBoy!")
    {
        out.put('L');
        out.put('B');
        out.put('o');
        out.put('y');

        out.put((char) (LaserBoy_chunk_size & 0x000000ff)       );
        out.put((char)((LaserBoy_chunk_size & 0x0000ff00) >> 8 ));
        out.put((char)((LaserBoy_chunk_size & 0x00ff0000) >> 16));
        out.put((char)((LaserBoy_chunk_size & 0xff000000) >> 24)); // 36

        out.put(version[ 0]); // L
        out.put(version[ 1]); // a
        out.put(version[ 2]); // s
        out.put(version[ 3]); // e
        out.put(version[ 4]); // r
        out.put(version[ 5]); // B
        out.put(version[ 6]); // o
        out.put(version[ 7]); // y
        out.put(version[ 8]); // C
        out.put(version[ 9]); // C
        out.put(version[10]); // Y
        out.put(version[11]); // Y
        out.put(version[12]); // M
        out.put(version[13]); // M
        out.put(version[14]); // D
        out.put(version[15]); // D

        out.put((char) (LaserBoy_wav_mode & 0x000000ff)       );
        out.put((char)((LaserBoy_wav_mode & 0x0000ff00) >> 8 ));
        out.put((char)((LaserBoy_wav_mode & 0x00ff0000) >> 16));
        out.put((char)((LaserBoy_wav_mode & 0xff000000) >> 24));

        if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
        {
            for(i = 0; i < num_channels; i++)
            {
                out.put((char)( offset[i] & 0x000000ff)       );
                out.put((char)((offset[i] & 0x0000ff00) >> 8 ));
                out.put((char)((offset[i] & 0x00ff0000) >> 16));
                out.put((char)((offset[i] & 0xff000000) >> 24));
            }
        }

        if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            out.put((char) (parms.lit_dwell_overhang & 0x000000ff)       ); // int
            out.put((char)((parms.lit_dwell_overhang & 0x0000ff00) >> 8 ));
            out.put((char)((parms.lit_dwell_overhang & 0x00ff0000) >> 16));
            out.put((char)((parms.lit_dwell_overhang & 0xff000000) >> 24));

            out.put((char)(((int)(parms.lit_delta_max) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.lit_delta_max) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.lit_delta_max) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.lit_delta_max) & 0x000000ff)       );

            out.put((char)(((int)(parms.blank_delta_max) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.blank_delta_max) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.blank_delta_max) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.blank_delta_max) & 0x000000ff)       );

            out.put((char)(((int)(parms.max_dwell_microsec) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.max_dwell_microsec) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.max_dwell_microsec) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.max_dwell_microsec) & 0x000000ff)       );

            out.put((char)(((int)(parms.max_wag_microsec) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.max_wag_microsec) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.max_wag_microsec) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.max_wag_microsec) & 0x000000ff)       );

            out.put((char)(((int)(parms.insignificant_distance) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.insignificant_distance) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.insignificant_distance) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.insignificant_distance) & 0x000000ff)       );

            out.put((char)(((int)(parms.insignificant_angle) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.insignificant_angle) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.insignificant_angle) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.insignificant_angle) & 0x000000ff)       );

            out.put((char)(((int)(parms.frames_per_second) & 0xff000000) >> 24)); // float
            out.put((char)(((int)(parms.frames_per_second) & 0x00ff0000) >> 16));
            out.put((char)(((int)(parms.frames_per_second) & 0x0000ff00) >> 8 ));
            out.put((char) ((int)(parms.frames_per_second) & 0x000000ff)       );
        }

        if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
            for(i = 0; i < num_channels; i++)
            {
                out.put((char) (signal_id[i] & 0x00ff)      );
                out.put((char)((signal_id[i] & 0xff00) >> 8));

                out.put((char) (LSB_tag[i] & 0x00ff)      );
                out.put((char)((LSB_tag[i] & 0xff00) >> 8));
            }

        if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
        {
            for(i = 0; i < num_channels; i++)
            {
                out.put(resolution[i]);
                out.put(0x00); // always an even number of bytes.
            }
        }

        if(LaserBoy_wav_mode & LASERBOY_WAVE_NUM_FRAMES)
        {
            out.put((char)( num_frames & 0x000000ff)       );
            out.put((char)((num_frames & 0x0000ff00) >> 8 ));
            out.put((char)((num_frames & 0x00ff0000) >> 16));
            out.put((char)((num_frames & 0xff000000) >> 24));
        }

        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_R)
            out.write((char*)color_rescale_r, 256 * sizeof(signed short));
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_G)
            out.write((char*)color_rescale_g, 256 * sizeof(signed short));
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_B)
            out.write((char*)color_rescale_b, 256 * sizeof(signed short));
        if(LaserBoy_wav_mode & LASERBOY_COLOR_RESCALE_I)
            out.write((char*)color_rescale_i, 256 * sizeof(signed short));
    } // end if(version != "!LaserBoy!")
//----------------------------------------------------------------------------
// end of report information added for LaserBoy !!!
//----------------------------------------------------------------------------
// data chunk!
//----------------------------------------------------------------------------
    out.put('d');
    out.put('a');
    out.put('t');
    out.put('a');

    out.put((char) (sub_chunk_2_size & 0x000000ff)       );
    out.put((char)((sub_chunk_2_size & 0x0000ff00) >> 8 ));
    out.put((char)((sub_chunk_2_size & 0x00ff0000) >> 16));
    out.put((char)((sub_chunk_2_size & 0xff000000) >> 24));

    header_size = out.tellp();
    return;
}

//############################################################################
void LaserBoy_wav_header::tell(string label) const
{
    cout << "----------------------------------------------------------" << ENDL;
    cout << "-- " << label                           << ENDL << ENDL;
    cout << "             file_name   : " << file_name       << ENDL;
    cout << "             header_size : " << header_size     << ENDL;
    cout << "             num_samples : " << num_samples     << ENDL;
    cout << "             sample_rate : " << sample_rate     << ENDL;
    cout << "             channel_map : " << channel_map     << ENDL;
    //------------------------------------------------------------------------
    if(version != "!LaserBoy!")
    {
        cout << "       LaserBoy_wav_mode : ";
        if(LaserBoy_wav_mode & LASERBOY_WAVE_POSITIVE        )
            cout << "LASERBOY_WAVE_POSITIVE" << ENDL;
        else
            cout << "LASERBOY_WAVE_NEGATIVE" << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_END_OF_FRAME    )
            cout << "                           LASERBOY_WAVE_END_OF_FRAME"     << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_UNIQUE_FRAME    )
            cout << "                           LASERBOY_WAVE_UNIQUE_FRAME"     << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_UNIQUE_VERTEX   )
            cout << "                           LASERBOY_WAVE_UNIQUE_VERTEX"    << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS         )
            cout << "                           LASERBOY_WAVE_OFFSETS"          << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED       )
            cout << "                           LASERBOY_WAVE_OPTIMIZED"        << ENDL;
        else
            cout << "                           --- no optimization ----"       << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX   )
            cout << "                           LASERBOY_WAVE_SIGNAL_MATRIX"    << ENDL;
        if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
            cout << "                           LASERBOY_SIGNAL_BIT_RESOLUTION" << ENDL;
        //----------------------------------------------------------------
        cout << "            num_channels : " << num_channels    << ENDL;
        cout << "         bits_per_sample : " << bits_per_sample << ENDL;
        cout << "                 version : " << version         << ENDL << ENDL;
        //----------------------------------------------------------------
        cout << " ch. :offset: bits :     : signal name  : LSB tag"          << ENDL;
        cout << "-----:------:------:-----:--------------:-----------------" << ENDL;
        for(int i = 0; i < num_channels; i++)
        {
            cout << setw(4)
                 << i
                 << " : ";
            if(LaserBoy_wav_mode & LASERBOY_WAVE_OFFSETS)
                 cout << setw(4) << (int)offset[i];
            else
                 cout << " na ";
            cout << " : ";

            if(LaserBoy_wav_mode & LASERBOY_SIGNAL_BIT_RESOLUTION)
                 cout << setw(4) << (int)resolution[i];
            else
                 cout << "    ";

            if(LaserBoy_wav_mode & LASERBOY_WAVE_SIGNAL_MATRIX)
                cout << " : "
                     << ((signal_id[i] >= 0) ? ("pos") : ("neg"))
                     << " : "
                     << wav_signal_id_to_name(signal_id[i])
                     << " : "
                     << wav_LSB_tag_to_name(LSB_tag[i])
                     << ENDL;
            else
                cout << ENDL;
        }
        //----------------------------------------------------------------
        if(LaserBoy_wav_mode & LASERBOY_WAVE_OPTIMIZED)
        {
            cout << ENDL;
            cout << "    lit_dwell_overhang : " << parms.lit_dwell_overhang     << ENDL;
            cout << "         lit_delta_max : " << parms.lit_delta_max          << ENDL;
            cout << "       blank_delta_max : " << parms.blank_delta_max        << ENDL;
            cout << "    max_dwell_microsec : " << parms.max_dwell_microsec     << ENDL;
            cout << "      max_wag_microsec : " << parms.max_wag_microsec       << ENDL;
            cout << "insignificant_distance : " << parms.insignificant_distance << ENDL;
            cout << "   insignificant_angle : " << parms.insignificant_angle * one_radian << ENDL;
            cout << "     frames_per_second : " << parms.frames_per_second      << ENDL;
        }
    }
    else
    {
        cout << "not a LaserBoy wav" << ENDL;
    }
    cout << "----------------------------------------------------------" << ENDL;
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
