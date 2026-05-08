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
// LaserBoy_frame_set.hpp is part of LaserBoy.
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
#ifndef __LASERBOY_FRAME_SET_DEFINITIONS__
#define __LASERBOY_FRAME_SET_DEFINITIONS__

//############################################################################
#include "LaserBoy_frame.hpp"

//############################################################################
class LaserBoy_frame_set : public vector<LaserBoy_frame>
{
public:
    LaserBoy_frame_set()
                     : vector<LaserBoy_frame>()
                     , p_frame_set        (this)
                     , frame_set_error    (LASERBOY_OK)
                     , num_2D_frames      ( 0)
                     , num_3D_frames      ( 0)
                     , frame_index        ( 0)
                     , frame_select_start (-1)
                     , frame_select_end   (-1)
                     , null_frame         ()
                     , ild_file_GUID      ("--------")
    {
        null_frame.palette_index = LASERBOY_ILDA_DEFAULT;
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.is_2D();
        null_frame.quantity = null_frame.size();
        null_frame.shrink_to_fit();
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set(const LaserBoy_frame_set& fs)
                     : vector<LaserBoy_frame>()
                     , p_frame_set        (this)
                     , frame_set_error    (fs.frame_set_error)
                     , num_2D_frames      (fs.num_2D_frames)
                     , num_3D_frames      (fs.num_3D_frames)
                     , frame_index        (fs.frame_index)
                     , frame_select_start (fs.frame_select_start)
                     , frame_select_end   (fs.frame_select_end)
                     , null_frame         ()
                     , ild_file_GUID      (fs.ild_file_GUID)
    {
        null_frame.palette_index = LASERBOY_ILDA_DEFAULT;
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.is_2D();
        null_frame.quantity = null_frame.size();
        null_frame.shrink_to_fit();
        clear();
        reserve(fs.size());
        insert(begin(), fs.begin(), fs.end());
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set(const LaserBoy_real_segment_set& rss)
                     : vector<LaserBoy_frame>()
                     , p_frame_set        (this)
                     , frame_set_error    (rss.real_segment_set_error)
                     , frame_index        (rss.frame_index)
                     , frame_select_start (rss.frame_select_start)
                     , frame_select_end   (rss.frame_select_end)
                     , null_frame         ()
                     , ild_file_GUID      ("--------")
    {
        LaserBoy_frame frame;
        null_frame.palette_index = LASERBOY_ILDA_DEFAULT;
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.push_back(LaserBoy_vertex(0, 0, 0, 255, 255, 255, 64, 55));
        null_frame.is_2D();
        null_frame.quantity = null_frame.size();
        null_frame.shrink_to_fit();
        reserve(rss.size());
        for(size_t i = 0; i < rss.size(); i++)
        {
            frame = rss[i];
            push_back(frame);
        }
        count_frames();
    }
    //------------------------------------------------------------------------
   ~LaserBoy_frame_set()
    {}
    //------------------------------------------------------------------------
    LaserBoy_frame& frame(int index) //sets frame_index
    {
        if(size())
        {
            if(index < 0)
                index = size() - 1;
            else if(index >= (int)size())
                index = 0;
            frame_index = index;
            return at(frame_index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& next_frame() //sets frame_index
    {
        if(size())
        {
            frame_index++;
            if(frame_index >= size())
                frame_index = 0;
            return at(frame_index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& previous_frame() //sets frame_index
    {
        if(size())
        {
            if(frame_index == 0)
                frame_index = size() - 1;
            else
                frame_index--;
            return at(frame_index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& frame_picker(int index) // does not set frame_index
    {
        if(size())
        {
            if(index < 0)
                index = size() - 1;
            else if(index >= (int)size())
                index = 0;
            return at(index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& current_frame()
    {
        if(size())
            return at(frame_index);
        return null_frame;
    }
    //------------------------------------------------------------------------
    void  delete_current_frame()
    {
        if(size() > 1)
        {
            erase(begin() + frame_index);
            if(frame_index > 0)
                frame_index--;
        }
        else
        {
            clear();
            push_back(null_frame);
        }
        normalize_cursors();
        return;
    }
    //------------------------------------------------------------------------
    void  delete_frame(u_int index)
    {
        if(size() > 1)
        {
            if(index < size())
            {
                erase(begin() + index);
                if(frame_index >= index)
                {
                    if(frame_index == 0)
                        frame_index = size() - 1;
                    else
                        frame_index--;
                }
            }
        }
        else
        {
            clear();
            push_back(null_frame);
        }
        normalize_cursors();
        return;
    }
    //------------------------------------------------------------------------
    void  copy_frame()
    {
        insert(begin() + frame_index, frame_picker(frame_index));
        frame_index++;
        return;
    }
    //------------------------------------------------------------------------
    void  new_frame()
    {
        insert(begin() + frame_index, LaserBoy_frame());
        //frame_index++;
        return;
    }
    //------------------------------------------------------------------------
    void  push_back_frame(const LaserBoy_frame& frame)
    {
        push_back(frame);
        return;
    }
    //------------------------------------------------------------------------
    void  count_frames()
    {
        num_2D_frames = num_3D_frames = 0;
        for(size_t i = 0; i < size(); i++)
            at(i).is_2D() ? num_2D_frames++ : num_3D_frames++;
        return;
    }
    //------------------------------------------------------------------------
    int   total_vertices() const
    {
        size_t  i,
                vertices = 0;
        for(i = 0; i < size(); i++)
            vertices += (at(i).size());
        return vertices;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& pre_incr_current_frame()
    {
        if(size())
        {
            frame_index++;
            if(frame_index >= size())
                frame_index = 0;
            return at(frame_index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& pre_decr_current_frame()
    {
        if(size())
        {
            if(frame_index == 0)
                frame_index = size() - 1;
            else
                frame_index--;
            return at(frame_index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& post_incr_current_frame(int)
    {
        if(size())
        {
            u_int frame_return = frame_index;
            frame_index++;
            if(frame_index >= size())
                frame_index = 0;
            return at(frame_return);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& post_decr_current_frame(int)
    {
        if(size())
        {
            u_int frame_return = frame_index;
            if(frame_index == 0)
                frame_index = size() - 1;
            else
                frame_index--;
            return at(frame_return);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator  = (const LaserBoy_frame_set& fs)
    {
        clear();
        reserve(fs.size());
        insert(begin(), fs.begin(), fs.end());
        frame_set_error    = fs.frame_set_error;
        frame_index        = fs.frame_index;
        frame_select_start = fs.frame_select_start;
        frame_select_end   = fs.frame_select_end;
        num_2D_frames      = fs.num_2D_frames;
        num_3D_frames      = fs.num_3D_frames;
        ild_file_GUID      = fs.ild_file_GUID;
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator  = (const LaserBoy_real_segment_set& rss)
    {
        LaserBoy_frame frame;
        frame_set_error    = rss.real_segment_set_error;
        frame_index        = rss.frame_index;
        frame_select_start = rss.frame_select_start;
        frame_select_end   = rss.frame_select_end;
        clear();
        reserve(rss.size());
        for(size_t i = 0; i < rss.size(); i++)
        {
            frame = rss[i];
            push_back(frame);
        }
        count_frames();
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator += (const LaserBoy_real_segment_set& rss)
    {
        LaserBoy_frame frame;
        for(size_t i = 0; i < rss.size(); i++)
        {
            frame = rss[i];
            push_back(frame);
        }
        count_frames();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator += (const LaserBoy_frame_set& frames)
    {
        reserve(size() + frames.size());
        insert(end(), frames.begin(), frames.end());
        count_frames();
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator  = (const LaserBoy_frame& frame)
    {
        clear();
        frame_select_start = -1;
        frame_select_end   = -1;
        push_back(frame);
        count_frames();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator += (const LaserBoy_frame& frame);
    //------------------------------------------------------------------------
    LaserBoy_frame_set& operator += (const LaserBoy_segment& segment)
    {
        LaserBoy_ild_header header;
        header.format = segment.is_2D();
        push_back(LaserBoy_frame(header, segment));
        count_frames();
        shrink_to_fit();
        return *this;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set selected_frames()
    {
        LaserBoy_frame_set  selected;
        for(size_t i = 0; i < size(); i++)
            if(at(i).is_selected)
                selected.push_back(at(i));
        return selected;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame_set  operator +  (const LaserBoy_frame_set& frames) const
    {
        LaserBoy_frame_set sum(*this);
        sum += frames;
        sum.count_frames();
        return sum;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame& operator [] (u_int index) // does not set frame_index
    {
        if(size())
        {
            if(index >= size())
                index = 0;
            return at(index);
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_frame selected_frame_index(u_int index)
    {
        if(size())
        {
            u_int selected_count = 0,
                  i;
            if(index < size())
            {
                for(i = 0; i < size(); i++)
                {
                    if(at(i).is_selected)
                    {
                        if(index == selected_count)
                            return at(i);
                        selected_count++;
                    }
                }
            }
        }
        return null_frame;
    }
    //------------------------------------------------------------------------
    LaserBoy_segment sum_of_frames()
    {
        bool              same_palettes = true;
        size_t            i;
        LaserBoy_segment  segment(front().palette_index, false);
        for(i = 1; i < size(); i++)
            if(segment.palette_index != at(i).palette_index)
            {
                same_palettes = false;
                break;
            }
        if(!same_palettes)
        {
            segment.promote_to_true_color();
            promote_to_true_color();
        }
        for(i = 0; i < size(); i++)
            segment += at(i);
        return segment;
    }
    //------------------------------------------------------------------------
    size_t number_of_frames() const {   return size();   }
    //------------------------------------------------------------------------
    int index_of_name(const string& name) const
    {
        for(int i = 0; i < (int)size(); i++)
            if(at(i).name == name)
                return i;
        return -1;
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
    int number_of_ild_color_tables() const
    {
        int   true_color_count = 0;
        for(int i = 0; i < (int)size(); i++)
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
                true_color_count++;
        return true_color_count;
    }
    //------------------------------------------------------------------------
    int number_of_ild_palettes() const
    {
        int   palette_count = 0;
        bool  in_true_color          = false,
              default_palette_needed = false;
        for(int i = 0; i < (int)size(); i++)
        {
            if(at(i).palette_index == LASERBOY_TRUE_COLOR)
            {
                if(!in_true_color)
                {
                    palette_count++;
                    in_true_color = true;
                    default_palette_needed = true;
                }
            }
            else if(at(i).palette_index != LASERBOY_ILDA_DEFAULT)
            {
                palette_count++;
                in_true_color = false;
                default_palette_needed = true;
            }
            else if(default_palette_needed)
            {
                palette_count++;
                default_palette_needed = false;
            }
        }
        return palette_count;
    }
    //------------------------------------------------------------------------
    bool                 font_index_to_frames           (const string& utf8_path,
                                                         string&       report,
                                                         bool          cleanup = false
                                                        );
    LaserBoy_Error_Code  get_unicode_index              (u32string& characters);
    bool                 save_utf8_frame_index          (const string& utf8_path,
                                                         string&       report,
                                                         bool          cleanup = false
                                                        );
    LaserBoy_Error_Code  order_frames_by_name           (string&       report,
                                                         bool          cleanup = false
                                                        );
    //------------------------------------------------------------------------
    void                 render_frame                   (const int& index,
                                                         struct LaserBoy_bmp *bmp
                                                        )  const ;
    //------------------------------------------------------------------------
    u_short              max_dimension                  () const ;
    double               relative_volume                () const ;
    double               relative_volume_to_origin      () const ;
    size_t               closest_x_to_edge              () const ;
    size_t               closest_y_to_edge              () const ;
    size_t               closest_z_to_edge              () const ;
    int                  frame_set_palette_index        ()       ;
    bool                 true_color_frames              ()       ;
    //------------------------------------------------------------------------
    LaserBoy_frame_set&  reverse                        ();
    LaserBoy_frame_set&  make_current_first             ();
    LaserBoy_frame_set&  swap_frames                    (int i, int j);
    LaserBoy_frame_set&  reverse_selected               ();
    LaserBoy_frame_set&  random_order                   ();
    LaserBoy_frame_set&  frame_reverse                  ();
    //------------------------------------------------------------------------
    LaserBoy_frame_set&  center_x                       (bool individually = false);
    LaserBoy_frame_set&  center_y                       (bool individually = false);
    LaserBoy_frame_set&  center_z                       (bool individually = false);
    LaserBoy_frame_set&  center                         (bool individually = false);
    //------------------------------------------------------------------------
    LaserBoy_frame_set&  normalize                      (double scale);
    LaserBoy_frame_set&  normalize_vectors              (double scale);
    LaserBoy_frame_set&  normalize_vectors_with_origin  (double scale);
    //------------------------------------------------------------------------
    LaserBoy_real_segment_set to_real_segment_set       (bool add_origin) const ;
    //------------------------------------------------------------------------
    bool                 from_nothing                   ();
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ild_file                  (const string& file);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ild_file                  (const string&              file,
                                                         long int&                  bytes_skipped,
                                                         LaserBoy_ild_header_count& counter
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_txt_file                  (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ctn_file                  (const string& file,
                                                         long int&     bytes_skipped
                                                        );
    //------------------------------------------------------------------------
    void                 superimpose_frame_set          (LaserBoy_frame_set& overlay);
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  superimpose_from_txt_file      (const string& file,
                                                         int &new_frame_count,
                                                         int &new_palette_count
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_dxf_file                  (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_dxf_directory             (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_dxf_font                  (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ifstream_ild              (std::ifstream&             in,
                                                         long int&                  bytes_skipped,
                                                         LaserBoy_ild_header_count& counter
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_ifstream_ctn              (std::ifstream& in,
                                                         long int&      bytes_skipped
                                                        );
    //------------------------------------------------------------------------
    bool                 from_LaserBoy_wav_file         (const string& file,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_qm_wav_file               (const string& file,
                                                         bool          global_polarity,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_unformatted_wav_file      (const string& file,
                                                         bool          global_polarity,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    bool                 from_headerless_LB_wav_file    (const string& file,
                                                         bool          global_polarity,
                                                         bool          append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_fstream_LaserBoy_wav      (LaserBoy_wav_header header,
                                                         std::fstream&       in,
                                                         bool                append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_fstream_qm_wav            (LaserBoy_wav_header header,
                                                         std::fstream&       in,
                                                         bool                global_polarity,
                                                         bool                append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_fstream_unformatted_wav   (LaserBoy_wav_header header,
                                                         std::fstream&       in,
                                                         bool                global_polarity,
                                                         bool                append = false
                                                        );
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  from_fstream_headerless_LB_wav (LaserBoy_wav_header header,
                                                         std::fstream&       in,
                                                         bool                global_polarity,
                                                         bool                append = false
                                                        );
    //------------------------------------------------------------------------
    void normalize_cursors()
    {
        if(frame_index >= size())
            frame_index = 0;
        if(frame_select_start >= (int)size())
            frame_select_start = -1;
        if(frame_select_end >= (int)size())
            frame_select_end = -1;
    }
    //------------------------------------------------------------------------
    bool    is_ild_busted                   (                        )       ;
    bool    is_2D                           (bool silent = false     )       ;
    void    flatten_z                       (                        )       ;
    void    flat_to_2D                      (                        )       ;
    void    z_order_vertices                (unsigned short span     )       ;
    void    unblank_all_vertices            (                        )       ;
    void    convert_black_to_blank          (                        )       ;
    void    convert_black_to_color          (                        )       ;
    void    convert_blank_to_black          (                        )       ;
    void    impose_black_level              (                        )       ;
    void    reduce_blank_vectors            (                        )       ;
    void    remove_dots                     (                        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     enhance_dots    (                        )       ;
    //------------------------------------------------------------------------
    void    remove_dwell_vertices           (                        )       ;
    void    remove_short_vectors            (                        )       ;
    void    reduce_lit_vectors              (                        )       ;
    void    minimize                        (                        )       ;
    void    to_dots                         (                        )       ;
    void    impose_bit_resolution           (                        )       ;
    void    explode_current_frame           (                        )       ;
    void    explode_all_frames              (                        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  collapse_selected_frames(                   )       ;
    //------------------------------------------------------------------------
    void    delete_every_nth_frame          (unsigned short n        )       ;
    void    keep_every_nth_frame            (unsigned short n        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  fracture_segments  (                        )       ;
    //------------------------------------------------------------------------
    void    omit_equivalent_vectors         (                        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code     bond_segments   (                        )       ;
    LaserBoy_Error_Code     reorder_segments(                        )       ;
    //------------------------------------------------------------------------
    void    randomize_segments              (                        )       ;
    void    conglomerate_lit_segments       (                        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  add_dwell              (                    )       ;
    LaserBoy_Error_Code  add_lit_span_vertices  (                    )       ;
    LaserBoy_Error_Code  add_blank_span_vertices(                    )       ;
    //------------------------------------------------------------------------
    void    delete_redundant_frames         (                        )       ;
    //------------------------------------------------------------------------
    LaserBoy_Error_Code  optimize           (                        )       ;
    LaserBoy_Error_Code  color_from_bmp     (const string& file      )       ;
    void                 color_from_bmp     (struct LaserBoy_bmp* bmp)       ;
    LaserBoy_Error_Code  subtract_bmp       (const string& file      )       ;
    void                 subtract_bmp       (struct LaserBoy_bmp* bmp)       ;
    //------------------------------------------------------------------------
    void    sync_rgb_and_palette            (                        )       ;
    //------------------------------------------------------------------------
    void    index_palette                   (                        )       ;
    void    index_segments_palette          (                        )       ;
    void    span_palette                    (                        )       ;
    void    span_x_palette                  (                        )       ;
    void    span_y_palette                  (                        )       ;
    void    span_z_palette                  (                        )       ;
    void    span_radial_palette             (                        )       ;
    void    span_axial_palette              (                        )       ;
    void    span_segments_palette           (                        )       ;
    void    random_color_vectors_palette    (                        )       ;
    void    random_color_segments_palette   (                        )       ;
    //------------------------------------------------------------------------
    void    index_hues                      (                        )       ;
    void    index_segments_hues             (                        )       ;
    void    span_hues                       (                        )       ;
    void    span_x_hues                     (                        )       ;
    void    span_y_hues                     (                        )       ;
    void    span_z_hues                     (                        )       ;
    void    span_radial_hues                (                        )       ;
    void    span_axial_hues                 (                        )       ;
    void    span_segments_hues              (                        )       ;
    void    random_color_vectors_hues       (                        )       ;
    void    random_color_segments_hues      (                        )       ;
    //------------------------------------------------------------------------
    void    quarter_turn                    (u_int plane, u_int turns)       ;
    double  max_distance_from_origin        (                        ) const ;
    double  max_distance_from_origin_xy     (                        ) const ;
    double  max_distance_from_origin_zy     (                        ) const ;
    double  min_distance_from_origin        (                        ) const ;
    double  min_distance_from_origin_xy     (                        ) const ;
    //------------------------------------------------------------------------
    LaserBoy_Bounds  apply_view             (                        )       ;
    LaserBoy_Bounds  scale_around_origin    (LaserBoy_3D_double f    )       ;
    LaserBoy_Bounds  scale_around_origin    (                        )       ;
    LaserBoy_Bounds  rotate_around_origin   (LaserBoy_3D_double f    )       ;
    LaserBoy_Bounds  rotate_around_origin   (                        )       ;
    LaserBoy_Bounds  move                   (LaserBoy_3D_double f    )       ;
    LaserBoy_Bounds  move                   (                        )       ;
    //------------------------------------------------------------------------
    void    toggle_frame_select_start       (                        )       ;
    void    toggle_frame_select_end         (                        )       ;
    void    invert_frame_selections         (                        )       ;
    void    delete_selected_frames          (                        )       ;
    void    trim_to_selected_frames         (                        )       ;
    void    bit_reduce_to_palette           (                        )       ;
    void    best_reduce_to_palette          (                        )       ;
    void    best_reduce_all_to_palette      (                        )       ;
    void    promote_to_true_color           (                        )       ;
    void    best_match_palette              (u_int index             )       ;
    void    best_match_target_palette       (                        )       ;
    void    strip_color                     (                        )       ;
    void    strip_color_or                  (                        )       ;
    void    strip_color_avg                 (                        )       ;
    void    to_palette_by_index             (u_int index             )       ;
    void    to_target_palette_by_index      (                        )       ;
    void    flip                            (int plane               )       ;
    void    renumber                        (                        )       ;
    bool    save_as_ild                     (const string& file,
                                             LaserBoy_ild_header_count& counter);
    bool    save_as_ctn                     (const string& file      )       ;
    bool    save_as_txt                     (const string& file      )       ;
    void    to_ofstream_ild                 (std::ofstream& out,
                                             LaserBoy_ild_header_count& counter);
    void    to_ofstream_ctn                 (std::ofstream& out      )       ;
    void    to_ofstream_txt                 (std::ofstream& out      )       ;
    //------------------------------------------------------------------------
    bool    save_as_wav                     (const string& file,
                                             bool   optimized,
                                             bool   timed
                                            )                                ;
    //------------------------------------------------------------------------
    void    to_fstream_wav                  (std::fstream& out,
                                             bool          optimized,
                                             bool          timed
                                            )                                ;
    //------------------------------------------------------------------------
    void    save_as_bmp_directory           (const string& dir,
                                             const string& name      ) const ;
    //------------------------------------------------------------------------
    void    save_as_bmp_view                (u_int         frame_index,
                                             const string& file      ) const ;
    //------------------------------------------------------------------------
    void    save_as_bmp_directory_view      (const string& dir,
                                             const string& name      ) const ;
    //------------------------------------------------------------------------
    bool    save_as_dxf_directory           (const string& dir,
                                             const string& name
                                            ) const ;
    //------------------------------------------------------------------------
    LaserBoy_frame_set*   p_frame_set       ;
    LaserBoy_Error_Code   frame_set_error   ;
    u_int                 num_2D_frames     ,
                          num_3D_frames     ,
                          frame_index       ;
    int                   frame_select_start,
                          frame_select_end  ;
    LaserBoy_frame        null_frame        ;
    string                ild_file_GUID     ;
};

//############################################################################
typedef LaserBoy_Bounds(*LaserBoy_frame_effect)           (LaserBoy_frame*    ); // function prototyes
typedef LaserBoy_Bounds(*LaserBoy_selected_frames_effect) (LaserBoy_frame_set*);
typedef LaserBoy_Bounds(*LaserBoy_frame_set_effect)       (LaserBoy_frame_set*);

//############################################################################
#endif

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
