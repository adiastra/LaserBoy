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
// LaserBoy_real_segment.cpp is part of LaserBoy.
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
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_real_vertex from,
                                             LaserBoy_real_vertex to
                                            ) // 3D line function
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{   // 3D line constructor
    int steps = linear_steps(to, from, (   (to.is_lit())
                                         ? (p_space->lit_delta_max)
                                         : (p_space->blank_delta_max)
                                       )
                            );
    if(steps)
    {
        LaserBoy_3D_double _to        (to),
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //----------------------------------------------------------------
        for(int i = 1; i < steps; i++)
            push_back(LaserBoy_real_vertex( LaserBoy_3D_double(_from + (delta * i)),
                                            (LaserBoy_color)to,
                                            to.k,
                                            to.c
                                          )
                     );
        //----------------------------------------------------------------
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_real_vertex from,
                                             LaserBoy_real_vertex to,
                                             const u_int&         max_d
                                            ) // 3D line function
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{   // 3D line constructor
    int steps = linear_steps(to, from, max_d);
    if(steps)
    {
        LaserBoy_3D_double _to        (to),
                           _from      (from),
                           difference (_to - _from),
                           delta      (difference / steps);
        //----------------------------------------------------------------
        for(int i = 1; i < steps; i++)
            push_back(LaserBoy_real_vertex( LaserBoy_3D_double(_from + (delta * i)),
                                            (LaserBoy_color)to,
                                            to.k,
                                            to.c
                                          )
                     );
        //----------------------------------------------------------------
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(const LaserBoy_3D_double& p,
                                             const LaserBoy_color      c
                                            ) // 3D cross function
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    push_back(LaserBoy_real_vertex(     p.x,      p.y,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
    push_back(LaserBoy_real_vertex(     p.x,      p.y,      p.z, c.r, c.g, c.b,                     0, 0));
    push_back(LaserBoy_real_vertex(-32767.0,      p.y,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
    push_back(LaserBoy_real_vertex( 32767.0,      p.y,      p.z, c.r, c.g, c.b,                     0, 0));
    push_back(LaserBoy_real_vertex(     p.x, -32767.0,      p.z, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
    push_back(LaserBoy_real_vertex(     p.x,  32767.0,      p.z, c.r, c.g, c.b,                     0, 0));
    push_back(LaserBoy_real_vertex(     p.x,      p.y, -32767.0, c.r, c.g, c.b, LASERBOY_BLANKING_BIT, 0));
    push_back(LaserBoy_real_vertex(     p.x,      p.y,  32767.0, c.r, c.g, c.b,                     0, 0));
    sync_rgb_and_palette();
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_frame_set&  font_frames,
                                             const u32string&     character_list,
                                             const string&        text,
                                             const double         char_width,
                                             const double         mono_font_space,
                                             const double         super_diacritic_gap,
                                             const bool           bond_word,
                                             const bool           bond_line
                                            )
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{   // fixed spaced font constructor
    if(text.size() == 0)
        real_segment_error = LASERBOY_EMPTY_STRING_ERROR;
    else
    {
        wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
        //--------------------------------------------------------------------
        bool                  first_glyph_rendered = false;
        size_t                text_index           = 0,
                              character_count      = 0,
                              vertex_index;
        LaserBoy_3D_double    float_3D;
        LaserBoy_real_vertex  vertex;
        LaserBoy_real_segment character(55, false),
                              diacritc (55, false);
        string                _text = text;
        u32string             u32_text;
        //--------------------------------------------------------------------
        for(text_index = 0; text_index < _text.size(); text_index++)
        {
            if(_text[text_index] == 0x0d)
                _text.erase(text_index);
        }
        text_index = 0;
        u32_text = conv_utf8_utf32.from_bytes(_text);
        //--------------------------------------------------------------------
        if(character_list.find(u32_text[text_index]) == string::npos) // character not found
            character = LaserBoy_real_segment(u32_text[text_index]);
        else
            character = font_frames[character_list.find(u32_text[text_index])].to_real_segment();
        //--------------------------------------------------------------------
        character.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
        while(    (text_index + 1) < u32_text.size()
               && is_super_diacritic(u32_text[text_index + 1])
             )
        {
            if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
            {
                diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
            }
            else
            {
                diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                diacritc.move(LaserBoy_3D_double(0.0,
                                                    -diacritc.min_y()
                                                  +  character.max_y()
                                                  +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                 0.0
                                                )
                            );
            }
            character += diacritc;
            character.convert_blank_to_black();
            text_index++;
        }
        //--------------------------------------------------------------------
        while(    (text_index + 1) < u32_text.size()
               && is_sub_diacritic(u32_text[text_index + 1])
             )
        {
            if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
            else
                diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
            diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
            character += diacritc;
            character.convert_blank_to_black();
            text_index++;
        }
        //--------------------------------------------------------------------
        float_3D = (LaserBoy_3D_double)character.at(0);
        vertex.blank();
        vertex.c = 0;
        vertex.r = 255;
        vertex.g = 0;
        vertex.b = 0;
        vertex = float_3D;
        push_back(vertex);
        vertex.unblank();
        //--------------------------------------------------------------------
        // UTF-8 Diacritic Range: Decimal 768-879. Hex 0300-036F
        //--------------------------------------------------------------------
        for(text_index = 0; text_index < u32_text.size(); text_index++)
        {
            if(u32_text[text_index] != 0x00000020) // [space]
            {
                if(character_list.find(u32_text[text_index]) == string::npos) // character not found
                    character = LaserBoy_real_segment(u32_text[text_index]);
                else
                    character = font_frames[character_list.find(u32_text[text_index])].to_real_segment();
                character.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                //------------------------------------------------------------
                while(    (text_index + 1) < u32_text.size()
                       && is_super_diacritic(u32_text[text_index + 1])
                     )
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                    {
                        diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                        diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                    }
                    else
                    {
                        diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                        diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                        diacritc.move(LaserBoy_3D_double(0.0,
                                                            -diacritc.min_y()
                                                          +  character.max_y()
                                                          +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                         0.0
                                                        )
                                    );
                    }
                    character += diacritc;
                    character.convert_blank_to_black();
                    text_index++;
                }
                //------------------------------------------------------------
                while(    (text_index + 1) < u32_text.size()
                       && is_sub_diacritic(u32_text[text_index + 1])
                     )
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                        diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                    else
                        diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                    character += diacritc;
                    character.convert_blank_to_black();
                    text_index++;
                }
                //------------------------------------------------------------
                for(vertex_index = 0; vertex_index < character.size(); vertex_index++)
                {
                    float_3D = character.at(vertex_index);
                    //--------------------------------------------------------
                    float_3D.x = float_3D.x + (   character_count
                                                * (   mono_font_space
                                                    * LASERBOY_MAX_SHORT
                                                  )
                                              );
                    //--------------------------------------------------------
                    if(vertex_index && character.at(vertex_index).is_black(p_space->black_level))
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(0, 0, 0),
                                                       vertex.k,
                                                       255
                                                      )
                                 );
                    else
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(255, 255, 255),
                                                       vertex.k,
                                                       55
                                                      )
                                 );
                    if(character.at(vertex_index).is_blank())
                    {
                        if(character_count && (bond_word || bond_line)) // not the first glyph
                        {
                            back().r = 0;
                            back().g = 0;
                            back().b = 0;
                            back().c = 255;
                        }
                        else
                            back().blank();
                    }
                }
                first_glyph_rendered = true;
                character_count++;
            } // end if(u32_text[text_index] != 0x00000020) // [space]
            else
            {
                character_count++;
                while(    (text_index + 1) < u32_text.size()
                       && u32_text[text_index + 1] == 0x00000020 // [space]
                     )
                {
                    text_index++;
                    character_count++;
                }
                //------------------------------------------------------------
                if((text_index + 1) < (u32_text.size() - 1))
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                        float_3D = LaserBoy_3D_double(0.0, 0.0, 0.0);
                    else
                        float_3D = font_frames[character_list.find(u32_text[text_index + 1])].at(0);
                    //--------------------------------------------------------
                    float_3D.x = float_3D.x + (   (   character_count
                                                    + 1
                                                  )
                                                * (   mono_font_space
                                                    * LASERBOY_MAX_SHORT
                                                  )
                                              );
                    //--------------------------------------------------------
                    if(first_glyph_rendered && bond_line)
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(0, 0, 0),
                                                       vertex.k,
                                                       255
                                                      )
                                 );
                    else
                    {
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(255, 255, 255),
                                                       vertex.k,
                                                       55
                                                      )
                                 );
                        back().blank();
                    }
                }
            }
        } // end for(text_index = 0; text_index < (int)u32_text.size(); text_index++)
        reduce_blank_vectors();
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_frame_set&  font_frames,
                                             const u32string&     character_list,
                                             const string&        text,
                                             const double         char_width,
                                             const double         vari_font_gap,
                                             const double         vari_font_space,
                                             const double         super_diacritic_gap,
                                             const bool           bond_word,
                                             const bool           bond_line
                                            )
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{   // variable spaced font constructor
    if(text.size() == 0)
        real_segment_error = LASERBOY_EMPTY_STRING_ERROR;
    else
    {
        wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
        //--------------------------------------------------------------------
        bool                  first_glyph_rendered = false;
        size_t                text_index           = 0,
                              vertex_index;
        double                glyph_offset,
                              width_of_a_space     = 0.0,
                              accumulated_offset   = 0.0;
        LaserBoy_3D_double    float_3D;
        LaserBoy_real_vertex  vertex;
        LaserBoy_real_segment character(55, false),
                              diacritc (55, false);
        string                _text = text;
        u32string             u32_text;
        //--------------------------------------------------------------------
        for(text_index = 0; text_index < _text.size(); text_index++)
        {
            if(_text[text_index] == 0x0d)
                _text.erase(text_index);
        }
        text_index = 0;
        u32_text = conv_utf8_utf32.from_bytes(_text);
        //--------------------------------------------------------------------
        if(character_list.find(u32_text[text_index]) == string::npos) // character not found
            character = LaserBoy_real_segment(u32_text[text_index]);
        else
            character = font_frames[character_list.find(u32_text[text_index])].to_real_segment();
        character.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
        //--------------------------------------------------------------------
        while(    (text_index + 1) < u32_text.size()
               && is_super_diacritic(u32_text[text_index + 1])
             )
        {
            if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
            {
                diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
            }
            else
            {
                diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                diacritc.move(LaserBoy_3D_double(   0.0,
                                                   -diacritc.min_y()
                                                 +  character.max_y()
                                                 +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                    0.0
                                                )
                            );
            }
            character += diacritc;
            character.convert_blank_to_black();
            text_index++;
        }
        //--------------------------------------------------------------------
        while(    (text_index + 1) < u32_text.size()
               && is_sub_diacritic(u32_text[text_index + 1])
             )
        {
            if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
            else
                diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
            diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
            character += diacritc;
            character.convert_blank_to_black();
            text_index++;
        }
        //--------------------------------------------------------------------
        float_3D = (LaserBoy_3D_double)character.at(0);
        vertex.blank();
        vertex.c = 0;
        vertex.r = 255;
        vertex.g = 0;
        vertex.b = 0;
        vertex = float_3D;
        push_back(vertex);
        vertex.unblank();
        //--------------------------------------------------------------------
        if(vari_font_space < 0.0)
        {
            for(size_t i = 0; i < font_frames.size(); i++)
                if(width_of_a_space < font_frames[i].width())
                    width_of_a_space = font_frames[i].width();
        }
        else
            width_of_a_space = (LASERBOY_MAX_SHORT * vari_font_space);
        //--------------------------------------------------------------------
        // UTF-8 Diacritic Range: Decimal 768-879. Hex 0300-036F
        //--------------------------------------------------------------------
        for(text_index = 0; text_index < u32_text.size(); text_index++)
        {
            if(    u32_text[text_index] != 0x00000020) // [space]
            {
                if(character_list.find(u32_text[text_index]) == string::npos) // character not found
                    character = LaserBoy_real_segment(u32_text[text_index]);
                else
                    character = font_frames[character_list.find(u32_text[text_index])].to_real_segment();
                character.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                //------------------------------------------------------------
                while(    (text_index + 1) < u32_text.size()
                       && is_super_diacritic(u32_text[text_index + 1])
                     )
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                    {
                        diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                        diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                    }
                    else
                    {
                        diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                        diacritc.scale(LaserBoy_3D_double(char_width, 1.0, 1.0));
                        diacritc.move(LaserBoy_3D_double(   0.0,
                                                           -diacritc.min_y()
                                                         +  character.max_y()
                                                         +  (super_diacritic_gap * LASERBOY_MAX_SHORT),
                                                            0.0
                                                        )
                                    );
                    }
                    character += diacritc;
                    character.convert_blank_to_black();
                    text_index++;
                }
                //------------------------------------------------------------
                while(    (text_index + 1) < u32_text.size()
                       && is_sub_diacritic(u32_text[text_index + 1])
                     )
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                        diacritc = LaserBoy_real_segment(u32_text[text_index + 1]);
                    else
                        diacritc = font_frames[character_list.find(u32_text[text_index + 1])].to_real_segment();
                    character += diacritc;
                    character.convert_blank_to_black();
                    text_index++;
                }
                //------------------------------------------------------------
                glyph_offset = character.segment_left().x;
                for(vertex_index = 0; vertex_index < character.size(); vertex_index++)
                {
                    float_3D   = character.at(vertex_index);
                    float_3D.x = (float_3D.x - glyph_offset) + accumulated_offset;
                    if(character.at(vertex_index).is_black(p_space->black_level))
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(0, 0, 0),
                                                       vertex.k,
                                                       255
                                                      )
                                 );
                    else
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(255, 255, 255),
                                                       vertex.k,
                                                       55
                                                      )
                                 );
                    if(character.at(vertex_index).is_blank())
                    {
                        if(text_index && (bond_word || bond_line)) // not the first glyph
                        {
                            back().r = 0;
                            back().g = 0;
                            back().b = 0;
                            back().c = 255;
                        }
                        else
                            back().blank();
                    }
                } // end for(vertex_index = 0; vertex_index < (int)font_frames[font_frame_index].size(); vertex_index++)
                accumulated_offset += (   character.width()
                                        + (   vari_font_gap
                                            * LASERBOY_MAX_SHORT // percent of square space
                                          )
                                      );
                first_glyph_rendered = true;
            } // end if(u32_text[text_index] != 0x00000020) // [space]
            else // it's a space
            {
                accumulated_offset += (   width_of_a_space
                                        + (   vari_font_gap
                                            * LASERBOY_MAX_SHORT // percent of square space
                                          )
                                      );
                while(    (text_index + 1) < u32_text.size()
                       && u32_text[text_index + 1] == 0x00000020 // [space]
                     )
                {
                    accumulated_offset += (   width_of_a_space
                                            + (   vari_font_gap
                                                * LASERBOY_MAX_SHORT // percent of square space
                                              )
                                          );
                    text_index++;
                }
                if((text_index + 1) < (u32_text.size() - 1))
                {
                    if(character_list.find(u32_text[text_index + 1]) == string::npos) // character not found
                    {
                        glyph_offset = 0.0;
                        float_3D = LaserBoy_3D_double(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glyph_offset = font_frames[character_list.find(u32_text[text_index + 1])].segment_left().x;
                        float_3D = font_frames[character_list.find(u32_text[text_index + 1])].at(0);
                    }
                    float_3D.x = (float_3D.x - glyph_offset) + accumulated_offset;
                    if(first_glyph_rendered && bond_line)
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(0, 0, 0),
                                                       vertex.k,
                                                       255
                                                      )
                                 );
                    else
                    {
                        push_back(LaserBoy_real_vertex(float_3D,
                                                       LaserBoy_color(255, 255, 255),
                                                       vertex.k,
                                                       55
                                                      )
                                 );
                        back().blank();
                    }
                }
            }
        } // end for(text_index = 0; text_index < (int)u32_text.size(); text_index++)
        reduce_blank_vectors();
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(char32_t unichar) // uncicode not found frame
{
    LaserBoy_real_segment  rs;
    if(p_space->font_frames.size())
    {
        string text = as_hex_string(unichar);
        rs.palette_index = LASERBOY_ILDA_DEFAULT;
        if(text.substr(0, 4) == "0000")
            text = text.substr(4, 4);
        rs = LaserBoy_real_segment(p_space->font_frames,
                                   p_space->font_index,
                                   text,
                                   1.0,
                                   1.0,
                                   0.05, // NA
                                   true,
                                   true
                                  );
        rs.convert_black_to_blank();
        rs.reduce_blank_vectors();
        rs.convert_blank_to_black();
        rs.scale(LaserBoy_3D_double(1.0 / text.size(), 1.0 / text.size(), 0.0));
        rs.scale(LaserBoy_3D_double(0.85, 0.85, 0.0));
        rs.center_y();
        rs.move(LaserBoy_3D_double(2600 - rs.min_x(), LASERBOY_HALF_SHORT, 0.0));
    }
    else
    {
        rs.push_back(LaserBoy_real_vertex(                 0,                  0, 0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, 0, 255, 255, 255,                     0, 55));
        rs.push_back(LaserBoy_real_vertex(LASERBOY_MAX_SHORT,                  0, 0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
        rs.push_back(LaserBoy_real_vertex(                 0, LASERBOY_MAX_SHORT, 0, 255, 255, 255,                     0, 55));
    }
    rs.push_back(LaserBoy_real_vertex(                 0,                  0, 0, 255, 255, 255, LASERBOY_BLANKING_BIT, 55));
    rs.push_back(LaserBoy_real_vertex(LASERBOY_MAX_SHORT,                  0, 0, 255, 255, 255,                     0, 55));
    rs.push_back(LaserBoy_real_vertex(LASERBOY_MAX_SHORT, LASERBOY_MAX_SHORT, 0, 255, 255, 255,                     0, 55));
    rs.push_back(LaserBoy_real_vertex(                 0, LASERBOY_MAX_SHORT, 0, 255, 255, 255,                     0, 55));
    rs.push_back(LaserBoy_real_vertex(                 0,                  0, 0, 255, 255, 255,                     0, 55));
    *this += rs;
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(const double      fixed_radius,
                                             const double      rhodonea_numerator,
                                             const double      rhodonea_denominator,
                                             const double      start,
                                             const double      duration,
                                             double            iterations
                                            ) // ::rhodonea
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating rhodonea");
    if(rhodonea_denominator != 0.0)
        ratio = rhodonea_numerator / rhodonea_denominator;
    vertex.x = fixed_radius * cos(ratio * start) * cos(start);
    vertex.y = fixed_radius * cos(ratio * start) * sin(start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = fixed_radius * cos(ratio * time) * cos(time);
            vertex.y = fixed_radius * cos(ratio * time) * sin(time);
            push_back(vertex);
        }
        vertex.x = fixed_radius * cos(ratio * end) * cos(end);
        vertex.y = fixed_radius * cos(ratio * end) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO4,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             LaserBoy_oscillator LBO4_,
                                             const double        fixed_radius,
                                             const double        rhodonea_numerator,
                                             const double        rhodonea_denominator,
                                             const double        start,
                                             const double        duration,
                                             double              iterations
                                            ) // ::oscillator_rhodonea
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_rhodonea");
    if(rhodonea_denominator != 0.0)
        ratio = rhodonea_numerator / rhodonea_denominator;
     LBO1.phase += (LBO1.phase_cycle  / 4.0);
     LBO2.phase += (LBO2.phase_cycle  / 4.0);
     LBO3.phase += (LBO3.phase_cycle  / 4.0);
    LBO1_.phase += (LBO1_.phase_cycle / 4.0);
    LBO2_.phase += (LBO2_.phase_cycle / 4.0);
    LBO3_.phase += (LBO3_.phase_cycle / 4.0);
    vertex.x =   fixed_radius
               * LBO1(ratio * start)
               * LBO2(start);
    vertex.y =   fixed_radius
               * LBO3(ratio * start)
               * LBO4(start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   fixed_radius
                       * LBO1.blend(LBO1_, blend)(ratio * time)
                       * LBO2.blend(LBO2_, blend)(time);
            vertex.y =   fixed_radius
                       * LBO3.blend(LBO3_, blend)(ratio * time)
                       * LBO4.blend(LBO4_, blend)(time);
            push_back(vertex);
        }
        vertex.x =   fixed_radius
                   * LBO1_(ratio * end)
                   * LBO2_(end);
        vertex.y =   fixed_radius
                   * LBO3_(ratio * end)
                   * LBO4_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(const double      fixed_radius,
                                             const double      roller_radius,
                                             const double      roller_offset,
                                             const double      start,
                                             const double      duration,
                                             double            iterations,
                                             const char        dud
                                            ) // ::epicycloid
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating epitrochoid / epicycloid");
    if(roller_radius != 0.0)
        ratio = fixed_radius / roller_radius;
    vertex.x =   roller_radius * (ratio + 1) * cos(start)
               - roller_offset * cos((ratio + 1) * start);
    vertex.y =   roller_radius * (ratio + 1) * sin(start)
               - roller_offset * sin((ratio + 1) * start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   roller_radius * (ratio + 1) * cos(time)
                       - roller_offset * cos((ratio + 1) * time);
            vertex.y =   roller_radius * (ratio + 1) * sin(time)
                       - roller_offset * sin((ratio + 1) * time);
            push_back(vertex);
        }
        vertex.x =   roller_radius * (ratio + 1) * cos(end)
                   - roller_offset * cos((ratio + 1) * end);
        vertex.y =   roller_radius * (ratio + 1) * sin(end)
                   - roller_offset * sin((ratio + 1) * end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations,
                                             const char          dud
                                            ) // ::oscillator_epicycloid
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_epitrochoid / oscillator_epicycloid");
     LBO1.phase += (LBO1.phase_cycle  / 4.0);
     LBO2.phase += (LBO2.phase_cycle  / 4.0);
    LBO1_.phase += (LBO1_.phase_cycle / 4.0);
    LBO2_.phase += (LBO2_.phase_cycle / 4.0);
    if(roller_radius != 0.0)
        ratio = fixed_radius / roller_radius;
    vertex.x =   roller_radius * (ratio + 1) * LBO1(start)
               - roller_offset * LBO2((ratio + 1) * start);
    vertex.y =   roller_radius * (ratio + 1) * LBO3(start)
               - roller_offset * LBO4((ratio + 1) * start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   roller_radius * (ratio + 1) * LBO1.blend(LBO1_, blend)(time)
                       - roller_offset * LBO2.blend(LBO2_, blend)((ratio + 1) * time);
            vertex.y =   roller_radius * (ratio + 1) * LBO3.blend(LBO3_, blend)(time)
                       - roller_offset * LBO4.blend(LBO4_, blend)((ratio + 1) * time);
            push_back(vertex);
        }
        vertex.x =   roller_radius * (ratio + 1) * LBO1_(end)
                   - roller_offset * LBO2_((ratio + 1) * end);
        vertex.y =   roller_radius * (ratio + 1) * LBO3_(end)
                   - roller_offset * LBO4_((ratio + 1) * end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(const double      fixed_radius,
                                             const double      roller_radius,
                                             const double      roller_offset,
                                             const double      start,
                                             const double      duration,
                                             double            iterations,
                                             const short       dud
                                            ) // ::hypocycloid
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating hypotrochoid / hypocycloid");
    if(roller_radius != 0.0)
        ratio = fixed_radius / roller_radius;
    vertex.x =    (fixed_radius - roller_radius) * cos(start)
                + roller_offset * cos((ratio - 1) * start);
    vertex.y =    (fixed_radius - roller_radius) * sin(start)
                - roller_offset * sin((ratio - 1) * start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =    (fixed_radius - roller_radius) * cos(time)
                        + roller_offset * cos((ratio - 1) * time);
            vertex.y =    (fixed_radius - roller_radius) * sin(time)
                        - roller_offset * sin((ratio - 1) * time);
            push_back(vertex);
        }
        vertex.x =    (fixed_radius - roller_radius) * cos(end)
                    + roller_offset * cos((ratio - 1) * end);
        vertex.y =    (fixed_radius - roller_radius) * sin(end)
                    - roller_offset * sin((ratio - 1) * end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations,
                                             const short         dud
                                            ) // ::oscillator_hypocycloid
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                ratio = 1.0,
                          blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_hypotrochoid / oscillator_hypocycloid");
     LBO1.phase += (LBO1.phase_cycle  / 4.0);
     LBO2.phase += (LBO2.phase_cycle  / 4.0);
    LBO1_.phase += (LBO1_.phase_cycle / 4.0);
    LBO2_.phase += (LBO2_.phase_cycle / 4.0);
    if(roller_radius != 0.0)
        ratio = fixed_radius / roller_radius;
    vertex.x =    (fixed_radius - roller_radius) * LBO1(start)
                + roller_offset * LBO2((ratio - 1) * start);
    vertex.y =    (fixed_radius - roller_radius) * LBO3(start)
                - roller_offset * LBO4((ratio - 1) * start);
    vertex.z = 0.0;
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =    (fixed_radius - roller_radius) * LBO1.blend(LBO1_, blend)(time)
                        + roller_offset * LBO2.blend(LBO2_, blend)((ratio - 1) * time);
            vertex.y =    (fixed_radius - roller_radius) * LBO3.blend(LBO3_, blend)(time)
                        - roller_offset * LBO4.blend(LBO4_, blend)((ratio - 1) * time);
            push_back(vertex);
        }
        vertex.x =    (fixed_radius - roller_radius) * LBO1_(end)
                    + roller_offset * LBO2_((ratio - 1) * end);
        vertex.y =    (fixed_radius - roller_radius) * LBO3_(end)
                    - roller_offset * LBO4_((ratio - 1) * end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO1_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations
                                            ) // ::oscillator
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator");
    vertex.x = start;
    vertex.y = LBO1(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = time;
            vertex.y = LBO1.blend(LBO1_, blend)(time);
            push_back(vertex);
        }
        vertex.x = end;
        vertex.y = LBO1_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations
                                            ) // ::oscillator_sum
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_sum");
    vertex.x = start;
    vertex.y = LBO1(start) + LBO2(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = time;
            vertex.y = LBO1.blend(LBO1_, blend)(time) + LBO2.blend(LBO2_, blend)(time);
            push_back(vertex);
        }
        vertex.x = end;
        vertex.y = LBO1_(end) + LBO2_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             double              acceleration,
                                             const char          dud
                                            ) // ::oscillator_xy
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    size_t                i,
                          i_iterations = (size_t)iterations;
    double                blend,
                          increment,
                          time,
                          deceleration,
                          reverse_i;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_xy");
    vertex.x = LBO1(start);
    vertex.y = LBO2(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        if(acceleration == 0.0)
        {
            increment = acceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                time     =   start
                           + (   duration
                               * (i / iterations)
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                push_back(vertex);
            }
        }
        else if(acceleration > 0.0)
        {
            increment = acceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                time     =   start
                           + (   duration
                               * (i / iterations)
                               * pow(_e_, -(acceleration - (i * increment)))
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                push_back(vertex);
            }
        }
        else // if(acceleration < 0.0)
        {
            deceleration = -acceleration;
            increment    = deceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                reverse_i = iterations - double(i);
                time     =   (start + duration)
                           - (   duration
                               * (reverse_i / iterations)
                               * pow(_e_, -(deceleration - (reverse_i * increment)))
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                push_back(vertex);
            }
        }
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             double              acceleration
                                            ) // ::oscillator_xyz
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    size_t                i,
                          i_iterations = (size_t)iterations;
    double                blend,
                          increment,
                          time,
                          deceleration,
                          reverse_i;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating oscillator_xyz");
    vertex.x = LBO1(start);
    vertex.y = LBO2(start);
    vertex.z = LBO3(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        if(acceleration == 0.0)
        {
            increment = acceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                time     =   start
                           + (   duration
                               * (i / iterations)
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                vertex.z = LBO3.blend(LBO3_, blend)(time);
                push_back(vertex);
            }
        }
        else if(acceleration > 0.0)
        {
            increment = acceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                time     =   start
                           + (   duration
                               * (i / iterations)
                               * pow(_e_, -(acceleration - (i * increment)))
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                vertex.z = LBO3.blend(LBO3_, blend)(time);
                push_back(vertex);
            }
        }
        else // if(acceleration < 0.0)
        {
            deceleration = -acceleration;
            increment    = deceleration / iterations;
            for (i = 1; i <= i_iterations; ++i)
            {
                reverse_i = iterations - double(i);
                time     =   (start + duration)
                           - (   duration
                               * (reverse_i / iterations)
                               * pow(_e_, -(deceleration - (reverse_i * increment)))
                             );
                blend    = (time - start) / duration;
                vertex.x = LBO1.blend(LBO1_, blend)(time);
                vertex.y = LBO2.blend(LBO2_, blend)(time);
                vertex.z = LBO3.blend(LBO3_, blend)(time);
                push_back(vertex);
            }
        }
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO4,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             LaserBoy_oscillator LBO4_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations
                                            ) // ::harmonograph
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating harmonograph");
    vertex.x = LBO1(start) + LBO2(start);
    vertex.y = LBO3(start) + LBO4(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   LBO1.blend(LBO1_, blend)(time)
                       + LBO2.blend(LBO2_, blend)(time);
            vertex.y =   LBO3.blend(LBO3_, blend)(time)
                       + LBO4.blend(LBO4_, blend)(time);
            push_back(vertex);
        }
        vertex.x =   LBO1_(end)
                   + LBO2_(end);
        vertex.y =   LBO3_(end)
                   + LBO4_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations
                                            ) // ::harmonograph_xyz
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating harmonograph_xyz");
    vertex.x = LBO1(start) + LBO2(start);
    vertex.y = LBO3(start) + LBO4(start);
    vertex.z = LBO5(start) + LBO6(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   LBO1.blend(LBO1_, blend)(time)
                       + LBO2.blend(LBO2_, blend)(time);
            vertex.y =   LBO3.blend(LBO3_, blend)(time)
                       + LBO4.blend(LBO4_, blend)(time);
            vertex.z =   LBO5.blend(LBO5_, blend)(time)
                       + LBO6.blend(LBO6_, blend)(time);
            push_back(vertex);
        }
        vertex.x =   LBO1_(end)
                   + LBO2_(end);
        vertex.y =   LBO3_(end)
                   + LBO4_(end);
        vertex.z =   LBO5_(end)
                   + LBO6_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const short         dud
                                            ) // ::amplitude_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating amplitude_mod");
    vertex.x = start;
    vertex.y = LBO1(start) * LBO2(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = time;
            vertex.y =   LBO1.blend(LBO1_, blend)(time)
                       * LBO2.blend(LBO2_, blend)(time);
            push_back(vertex);
        }
        vertex.x = end;
        vertex.y =   LBO1_(end)
                   * LBO2_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO4,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             LaserBoy_oscillator LBO4_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const char          dud
                                            ) // ::amplitude_mod_xy
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating amplitude_mod_xy");
    vertex.x = LBO1(start) * LBO2(start);
    vertex.y = LBO3(start) * LBO4(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   LBO1.blend(LBO1_, blend)(time)
                       * LBO2.blend(LBO2_, blend)(time);
            vertex.y =   LBO3.blend(LBO3_, blend)(time)
                       * LBO4.blend(LBO4_, blend)(time);
            push_back(vertex);
        }
        vertex.x =   LBO1_(end)
                   * LBO2_(end);
        vertex.y =   LBO3_(end)
                   * LBO4_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations,
                                             const char          dud
                                            ) // ::amplitude_mod_xyz
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating amplitude_mod_xyz");
    vertex.x = LBO1(start) * LBO2(start);
    vertex.y = LBO3(start) * LBO4(start);
    vertex.z = LBO5(start) * LBO6(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x =   LBO1.blend(LBO1_, blend)(time)
                       * LBO2.blend(LBO2_, blend)(time);
            vertex.y =   LBO3.blend(LBO3_, blend)(time)
                       * LBO4.blend(LBO4_, blend)(time);
            vertex.z =   LBO5.blend(LBO5_, blend)(time)
                       * LBO6.blend(LBO6_, blend)(time);
            push_back(vertex);
        }
        vertex.x =   LBO1_(end)
                   * LBO2_(end);
        vertex.y =   LBO3_(end)
                   * LBO4_(end);
        vertex.z =   LBO5_(end)
                   * LBO6_(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const int           dud
                                            ) // ::frequency_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating frequency_mod");
    vertex.x = start;
    LBO = LBO1;
    LBO.frequency = LBO1.frequency * LBO2(start);
    vertex.y = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = time;
            LBO = LBO1.blend(LBO1_, blend);
            LBO.frequency =   LBO1.blend(LBO1_, blend).frequency
                            * LBO2.blend(LBO2_, blend)(time);
            vertex.y = LBO(time);
            push_back(vertex);
        }
        LBO           = LBO1_;
        LBO.frequency =   LBO1_.frequency
                        * LBO2_(end);
        vertex.y  = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO4,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             LaserBoy_oscillator LBO4_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const short         dud
                                            ) // ::frequency_mod_xy
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating frequency_mod_xy");
    LBO = LBO1;
    LBO.frequency = LBO1.frequency * LBO2(start);
    vertex.x = LBO(start);
    LBO = LBO3;
    LBO.frequency = LBO3.frequency * LBO4(start);
    vertex.y = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO = LBO1.blend(LBO1_, blend);
            LBO.frequency =   LBO1.blend(LBO1_, blend).frequency
                            * LBO2.blend(LBO2_, blend)(time);

            vertex.x = LBO(time);
            LBO = LBO3.blend(LBO3_, blend);
            LBO.frequency =   LBO3.blend(LBO3_, blend).frequency
                            * LBO4.blend(LBO4_, blend)(time);
            vertex.y = LBO(time);
            push_back(vertex);
        }
        LBO           = LBO1_;
        LBO.frequency =   LBO1_.frequency
                        * LBO2_(end);
        vertex.x      = LBO(end);
        LBO           = LBO3_;
        LBO.frequency =   LBO3_.frequency
                        * LBO4_(end);
        vertex.y  = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations,
                                             const short         dud
                                            ) // ::frequency_mod_xyz
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating frequency_mod_xyz");
    LBO = LBO1;
    LBO.frequency = LBO1.frequency * LBO2(start);
    vertex.x = LBO(start);
    LBO = LBO3;
    LBO.frequency = LBO3.frequency * LBO4(start);
    vertex.y = LBO(start);
    LBO = LBO3;
    LBO.frequency = LBO5.frequency * LBO6(start);
    vertex.z = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO           = LBO1.blend(LBO1_, blend);
            LBO.frequency =   LBO1.blend(LBO1_, blend).frequency
                            * LBO2.blend(LBO2_, blend)(time);
            vertex.x = LBO(time);
            LBO = LBO3.blend(LBO3_, blend);
            LBO.frequency =   LBO3.blend(LBO3_, blend).frequency
                            * LBO4.blend(LBO4_, blend)(time);
            vertex.y = LBO(time);
            LBO = LBO5.blend(LBO5_, blend);
            LBO.frequency =   LBO5.blend(LBO5_, blend).frequency
                            * LBO6.blend(LBO6_, blend)(time);
            vertex.z = LBO(time);
            push_back(vertex);
        }
        LBO           = LBO1_;
        LBO.frequency =   LBO1_.frequency
                        * LBO2_(end);
        vertex.x      = LBO(end);
        LBO           = LBO3_;
        LBO.frequency =   LBO3_.frequency
                        * LBO4_(end);
        vertex.y      = LBO(end);
        LBO           = LBO5_;
        LBO.frequency =   LBO5_.frequency
                        * LBO6_(end);
        vertex.z      = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const float         dud
                                            ) // ::phase_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating phase_mod");
    vertex.x = start;
    LBO = LBO1;
    LBO.phase = LBO1.phase + LBO2(start) * (LBO1.phase_cycle / 2.0);
    vertex.y = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = time;
            LBO       = LBO1;
            LBO.phase =   LBO1.blend(LBO1_, blend).phase
                        + LBO2.blend(LBO2_, blend)(time)
                        * (LBO1.phase_cycle / 2.0);
            vertex.y = LBO(time);
            push_back(vertex);
        }
        vertex.x  = end;
        LBO       = LBO1_;
        LBO.phase = LBO1_.phase + LBO2_(end) * (LBO1_.phase_cycle / 2.0);
        vertex.y  = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO3,
                                             LaserBoy_oscillator LBO4,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             LaserBoy_oscillator LBO3_,
                                             LaserBoy_oscillator LBO4_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const int           dud
                                            ) // ::phase_mod_xy
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating phase_mod_xy");
    LBO = LBO1;
    LBO.phase = LBO1.phase + LBO2(start) * (LBO1.phase_cycle / 2.0);
    vertex.x = LBO(start);
    LBO = LBO3;
    LBO.phase = LBO3.phase + LBO4(start) * (LBO3.phase_cycle / 2.0);
    vertex.y = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO       = LBO1.blend(LBO1_, blend);
            LBO.phase =   LBO1.blend(LBO1_, blend).phase
                        + LBO2.blend(LBO2_, blend)(time) * (LBO1.phase_cycle / 2.0);
            vertex.x  = LBO(time);
            LBO       = LBO3.blend(LBO3_, blend);
            LBO.phase =   LBO3.blend(LBO3_, blend).phase
                        + LBO4.blend(LBO4_, blend)(time) * (LBO3.phase_cycle / 2.0);
            vertex.y  = LBO(time);
            push_back(vertex);
        }
        LBO = LBO1_;
        LBO.phase = LBO1_.phase + LBO2_(end) * (LBO1_.phase_cycle / 2.0);
        vertex.x  = LBO(end);
        LBO = LBO3;
        LBO.phase = LBO3_.phase + LBO4_(end) * (LBO3_.phase_cycle / 2.0);
        vertex.y  = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
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
                                             double              iterations,
                                             const int           dud
                                            ) // ::phase_mod_xyz
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating phase_mod_xyz");
    LBO = LBO1;
    LBO.phase = LBO1.phase + LBO2(start) * (LBO1.phase_cycle / 2.0);
    vertex.x = LBO(start);
    LBO = LBO3;
    LBO.phase = LBO3.phase + LBO4(start) * (LBO3.phase_cycle / 2.0);
    vertex.y = LBO(start);
    LBO = LBO3;
    LBO.phase = LBO5.phase + LBO6(start) * (LBO5.phase_cycle / 2.0);
    vertex.z = LBO(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO       = LBO1.blend(LBO1_, blend);
            LBO.phase =   LBO1.blend(LBO1_, blend).phase
                        + LBO2.blend(LBO2_, blend)(time) * (LBO1.phase_cycle / 2.0);
            vertex.x  = LBO(time);
            LBO       = LBO3.blend(LBO3_, blend);
            LBO.phase =   LBO3.blend(LBO3_, blend).phase
                        + LBO4.blend(LBO4_, blend)(time) * (LBO3.phase_cycle / 2.0);
            vertex.y  = LBO(time);
            LBO       = LBO5.blend(LBO5_, blend);
            LBO.phase =   LBO5.blend(LBO5_, blend).phase
                        + LBO6.blend(LBO6_, blend)(time) * (LBO5.phase_cycle / 2.0);
            vertex.z  = LBO(time);
            push_back(vertex);
        }
        LBO       = LBO1_;
        LBO.phase =   LBO1_.phase + LBO2_(end) * (LBO1_.phase_cycle / 2.0);
        vertex.x  = LBO(end);
        LBO       = LBO3_;
        LBO.phase =   LBO3_.phase + LBO4(end) * (LBO3_.phase_cycle / 2.0);
        vertex.y  = LBO(end);
        LBO       = LBO5_;
        LBO.phase = LBO5_.phase + LBO6_(end) * (LBO5_.phase_cycle / 2.0);
        vertex.z  = LBO(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO1_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const char          dud
                                            ) // ::polar
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating polar");
    vertex.x = LBO1(start) * cos(start);
    vertex.y = LBO1(start) * sin(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = LBO1.blend(LBO1_, blend)(time) * cos(time);
            vertex.y = LBO1.blend(LBO1_, blend)(time) * sin(time);
            push_back(vertex);
        }
        vertex.x = LBO1_(end) * cos(end);
        vertex.y = LBO1_(end) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const char          dud1,
                                             const char          dud2
                                            ) // ::polar_sum
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating polar_sum");
    vertex.x = (LBO1(start) + LBO2(start)) * cos(start);
    vertex.y = (LBO1(start) + LBO2(start)) * sin(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = (   LBO1.blend(LBO1_, blend)(time)
                         + LBO2.blend(LBO2_, blend)(time)) * cos(time);
            vertex.y = (   LBO1.blend(LBO1_, blend)(time)
                         + LBO2.blend(LBO2_, blend)(time)) * sin(time);
            push_back(vertex);
        }
        vertex.x = (   LBO1_(end)
                     + LBO2_(end)) * cos(end);
        vertex.y = (   LBO1_(end)
                     + LBO2_(end)) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const short         dud1,
                                             const char          dud2
                                            ) // ::polar_amplitude_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating polar_amplitude_mod");
    vertex.x = (LBO1(start) * LBO2(start)) * cos(start);
    vertex.y = (LBO1(start) * LBO2(start)) * sin(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            vertex.x = (   LBO1.blend(LBO1_, blend)(time)
                         * LBO2.blend(LBO2_, blend)(time)) * cos(time);
            vertex.y = (   LBO1.blend(LBO1_, blend)(time)
                         * LBO2.blend(LBO2_, blend)(time)) * sin(time);
            push_back(vertex);
        }
        vertex.x = (   LBO1_(end)
                     * LBO2_(end)) * cos(end);
        vertex.y = (   LBO1_(end)
                     * LBO2_(end)) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const int           dud1,
                                             const char          dud2
                                            ) // ::polar_frequency_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating polar_frequency_mod");
    LBO = LBO1;
    LBO.frequency = LBO1.frequency * LBO2(start);
    vertex.x = LBO(start) * cos(start);
    vertex.y = LBO(start) * sin(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO = LBO1;
            LBO.frequency =   LBO1.blend(LBO1_, blend).frequency
                            * LBO2.blend(LBO2_, blend)(time);
            vertex.x = LBO(time) * cos(time);
            vertex.y = LBO(time) * sin(time);
            push_back(vertex);
        }
        LBO           = LBO1_;
        LBO.frequency = LBO1_.frequency * LBO2_(end);
        vertex.x      = LBO(end) * cos(end);
        vertex.y      = LBO(end) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
LaserBoy_real_segment::LaserBoy_real_segment(LaserBoy_oscillator LBO1,
                                             LaserBoy_oscillator LBO2,
                                             LaserBoy_oscillator LBO1_,
                                             LaserBoy_oscillator LBO2_,
                                             const double        start,
                                             const double        duration,
                                             double              iterations,
                                             const float         dud1,
                                             const char          dud2
                                            ) // ::polar_phase_mod
                : vector<LaserBoy_real_vertex>(),
                  is_selected                 (false),
                  palette_index               (LASERBOY_ILDA_DEFAULT),
                  real_segment_error          (LASERBOY_OK)
{
    bool                  iterations_is_int = close_to_int(iterations);
    double                blend,
                          increment,
                          time,
                          end = start + duration;
    LaserBoy_real_vertex  vertex;
    LaserBoy_oscillator   LBO;
    reserve((size_t)iterations + 2);
    p_space->p_GUI->display_state("calculating polar_phase_mod");
    LBO = LBO1;
    LBO.phase = LBO1.phase + LBO2(start) * (LBO1.phase_cycle / 2.0);
    vertex.x = LBO(start) * cos(start);
    vertex.y = LBO(start) * sin(start);
    push_back(vertex);
    vertex.unblank();
    if(iterations >= 2 && duration > 0.0)
    {
        increment = duration / (iterations - 1.0);
        for(time = start + increment; time <= end; time += increment)
        {
            blend    = (time - start) / duration;
            LBO       = LBO1;
            LBO.phase =   LBO1.blend(LBO1_, blend).phase
                        + LBO2.blend(LBO2_, blend)(time) * (LBO.phase_cycle / 2.0);
            vertex.x  = LBO(time) * cos(time);
            vertex.y  = LBO(time) * sin(time);
            push_back(vertex);
        }
        LBO       = LBO1_;
        LBO.phase =   LBO1_.phase
                    + LBO2_(end) * (LBO.phase_cycle / 2.0);
        vertex.x  = LBO(end) * cos(end);
        vertex.y  = LBO(end) * sin(end);
        if(    iterations_is_int
            && ((back() | vertex) > LASERBOY_EPSILON)
          )
            push_back(vertex);
    }
}

//############################################################################
size_t LaserBoy_real_segment::number_of_color_vectors() const
{
    size_t count = 0;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                count++;
    return count;
}

//############################################################################
size_t LaserBoy_real_segment::number_of_dark_vectors() const
{
    size_t count = 0;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_dark(p_space->black_level))
                count++;
    return count;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::dimensions(LaserBoy_3D_double& real_offset, bool ignore_origin) const
{
    size_t             i,
                       start    = 0;
    LaserBoy_3D_double real_min =  DBL_MAX,
                       real_max = -DBL_MAX;
    if(ignore_origin)
        start = 2;
    for(i = start; i < size(); i++) // ignore the origin vector
    {
        if(at(i).x > real_max.x)
            real_max.x = at(i).x;
        if(at(i).x < real_min.x)
            real_min.x = at(i).x;
        if(at(i).y > real_max.y)
            real_max.y = at(i).y;
        if(at(i).y < real_min.y)
            real_min.y = at(i).y;
        if(at(i).z > real_max.z)
            real_max.z = at(i).z;
        if(at(i).z < real_min.z)
            real_min.z = at(i).z;
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
LaserBoy_3D_double LaserBoy_real_segment::segment_front() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double front_;
        front_.z = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z > front_.z)
                front_ = at(i);
        return front_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::segment_back() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double back_;
        back_.z = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).z < back_.z)
                back_ = at(i);
        return back_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::segment_top() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double top_;
        top_.y = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y > top_.y)
                top_ = at(i);
        return top_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::segment_bottom() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double bottom_;
        bottom_.y = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).y < bottom_.y)
                bottom_ = at(i);
        return bottom_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::segment_right() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double right_;
        right_.x = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x > right_.x)
                right_ = at(i);
        return right_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::segment_left() const
{
    if(size() > 1)
    {
        LaserBoy_3D_double left_;
        left_.x = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(at(i).x < left_.x)
                left_ = at(i);
        return left_;
    }
    else
        return LaserBoy_3D_double(0, 0, 0);
}

//############################################################################
double LaserBoy_real_segment::max_x() const
{
    if(size() > 1)
    {
        double max = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(max <= at(i).x)
                max = at(i).x;
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::max_y() const
{
    if(size() > 1)
    {
        double max = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(max <= at(i).y)
                max = at(i).y;
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::max_z() const
{
    if(size() > 1)
    {
        double max = -DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(max <= at(i).z)
                max = at(i).z;
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_x() const
{
    if(size() > 1)
    {
        double min = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(min >= at(i).x)
                min = at(i).x;
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_y() const
{
    if(size() > 1)
    {
        double min = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(min >= at(i).y)
                min = at(i).y;
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_z() const
{
    if(size() > 1)
    {
        double min = DBL_MAX;
        for(size_t i = 0; i < size(); i++)
            if(min >= at(i).z)
                min = at(i).z;
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::height() const
{
    return max_y() - min_y();
}
//############################################################################
double LaserBoy_real_segment::width() const
{
    return max_x() - min_x();
}

//############################################################################
double LaserBoy_real_segment::depth() const
{
    return max_z() - min_z();
}

//############################################################################
double LaserBoy_real_segment::max_dimension() const
{
    double max = 0;
    if(width () > max)    max = width ();
    if(height() > max)    max = height();
    if(depth () > max)    max = depth ();
    return max;
}

//############################################################################
double LaserBoy_real_segment::max_distance_from_origin() const
{
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin;
        for(size_t i = 0; i < size(); i++)
        {
            d = origin | at(i);
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::max_distance_from_origin_xy() const
{
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::max_distance_from_origin_zy() const
{
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.x = 0;
            d = origin | vertex;
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::max_distance_from_origin_xz() const
{
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.y = 0;
            d = origin | vertex;
            if(d > max)
                max = d;
        }
        return max;
    }
    else
        return 0.0;
}

//############################################################################
size_t LaserBoy_real_segment::i_max_distance_from_origin_xy() const
{
    size_t index = 0;
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d > max)
            {
                index = i;
                max = d;
            }
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_real_segment::i_max_distance_from_origin_zy() const
{
    size_t index = 0;
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.x = 0;
            d = origin | vertex;
            if(d > max)
            {
                index = i;
                max = d;
            }
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_real_segment::i_max_distance_from_origin_xz() const
{
    size_t index = 0;
    if(size() > 1)
    {
        double             max = 0.0,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.y = 0;
            d = origin | vertex;
            if(d > max)
            {
                index = i;
                max = d;
            }
        }
    }
    return index;
}

//############################################################################
double LaserBoy_real_segment::min_distance_from_origin() const
{
    if(size() > 1)
    {
        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin;
        for(size_t i = 0; i < size(); i++)
        {
            d = origin | at(i);
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_distance_from_origin_xy() const
{
    if(size() > 1)
    {
        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_distance_from_origin_zy() const
{
    if(size() > 1)
    {
        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.x = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
double LaserBoy_real_segment::min_distance_from_origin_xz() const
{
    if(size() > 1)
    {
        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.y = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
                min = d;
        }
        return min;
    }
    else
        return 0.0;
}

//############################################################################
size_t LaserBoy_real_segment::i_min_distance_from_origin_xy() const
{
    size_t index = 0;
    if(size() > 1)
    {

        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.z = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
            {
                index = i;
                min = d;
            }
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_real_segment::i_min_distance_from_origin_zy() const
{
    size_t index = 0;
    if(size() > 1)
    {

        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.x = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
            {
                index = i;
                min = d;
            }
        }
    }
    return index;
}

//############################################################################
size_t LaserBoy_real_segment::i_min_distance_from_origin_xz() const
{
    size_t index = 0;
    if(size() > 1)
    {

        double             min = DBL_MAX,
                           d;
        LaserBoy_3D_double origin,
                           vertex;
        for(size_t i = 0; i < size(); i++)
        {
            vertex = at(i);
            vertex.y = 0;
            d = origin | vertex;
            if(d == 0.0)
                return 0.0;
            if(d < min)
            {
                index = i;
                min = d;
            }
        }
    }
    return index;
}

//############################################################################
bool LaserBoy_real_segment::segment_passes_through_origin(const double& granularity) const
{
    if(size() > 1)
    {
        LaserBoy_real_segment rs(*this);
        rs.normalize_vectors_with_origin();
        for(size_t i = 1; i < rs.size(); i++)
            if(vector_passes_through_origin(at(i - 1), at(i), granularity))
                return true;
    }
    return false;
}

//############################################################################
void LaserBoy_real_segment::blank_all_vertices()
{
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
            at(i).blank();
    return;
}

//############################################################################
void LaserBoy_real_segment::unblank_all_vertices()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            at(i).unblank();
    return;
}

//############################################################################
void LaserBoy_real_segment::strip_color() // to white in default palette
{
    palette_index = LASERBOY_ILDA_DEFAULT;
    if(size() > 1)
    {
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
            {
                at(i).c = 55;
                at(i).r = 255;
                at(i).g = 255;
                at(i).b = 255;
            }
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::strip_color_rgb(const LaserBoy_color& c)
{
    palette_index = LASERBOY_TRUE_COLOR;
    for(size_t i = 1; i < size(); i++)
    {
        if(at(i).is_color(p_space->black_level))
        {
            at(i).r = c.r;
            at(i).g = c.g;
            at(i).b = c.b;
        }
    }
    sync_rgb_and_palette();
    return;
}

//############################################################################
void LaserBoy_real_segment::index_hues(int index_multiple, int offset)
{
    u_short hue_index = 0;
    palette_index   = LASERBOY_TRUE_COLOR;
    for(size_t i = 1; i < size(); i += index_multiple)
    {
        for(int j = 0; j < index_multiple; j++)
        {
            if(    (i + j) < size()
                && at(i + j).is_color(p_space->black_level)
              )
            {
                hue_index = hue_index % 1530;
                at(i + j) = LaserBoy_color(hue_index);
            }
        }
        hue_index++;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::span_hues(double span_factor, int offset)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
            if(at(i).is_color(p_space->black_level))
                at(i) = LaserBoy_color((u_short)(((int)(((i * span_factor) / (double)(size() - 1)) * 1530) + offset) % 1530));
    return;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::reverse()
{
    if(size() > 1)
    {
        int                    i;
        LaserBoy_real_segment  reversed(palette_index, false);
        LaserBoy_real_vertex   vertex;
        //----------------------------------------------------------------
        reversed.reserve(size());
        //----------------------------------------------------------------
        i = size() - 1;
        //----------------------------------------------------------------
        vertex = at(i--);
        vertex.blank();
        reversed.push_back(vertex);
        //----------------------------------------------------------------
        while(i >= 0)
        {
            vertex   = at(i    );
            vertex.r = at(i + 1).r;
            vertex.g = at(i + 1).g;
            vertex.b = at(i + 1).b;
            vertex.c = at(i + 1).c;
            vertex.k = at(i + 1).k;
            reversed.push_back(vertex);
            i--;
        }
        //----------------------------------------------------------------
        *this = reversed;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::reorder_from(size_t vertex_index)
{
    if(    vertex_index
        && vertex_index < size()
        && size() > 1
      )
    {
        size_t                 i;
        LaserBoy_real_segment  reordered(palette_index, false);
        //----------------------------------------------------------------
        for(i = vertex_index; i < size(); i++)
            reordered += at(i);
        reordered.front().blank();
        for(i = 0; i <= vertex_index; i++)
            reordered += at(i);
        *this = reordered;
        //----------------------------------------------------------------
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::center_x()
{
    if(size() > 1)
    {
        LaserBoy_3D_double  center = rectangular_center_of();
        for(size_t i = 0; i < size(); i++)
            at(i).x = at(i).x - center.x;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::center_y()
{
    if(size() > 1)
    {
        LaserBoy_3D_double  center = rectangular_center_of();
        for(size_t i = 0; i < size(); i++)
            at(i).y = at(i).y - center.y;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::center_z()
{
    if(size() > 1)
    {
        LaserBoy_3D_double  center = rectangular_center_of();
        for(size_t i = 0; i < size(); i++)
            at(i).z = at(i).z - center.z;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::center()
{
    if(size() > 1)
    {
        LaserBoy_3D_double  center = rectangular_center_of();
        for(size_t i = 0; i < size(); i++)
            at(i) = at(i) - center;
    }
    return *this;
}

//############################################################################
void LaserBoy_real_segment::flip(u_int plane)
{
    if(size() > 1)
    {
        size_t i;
        switch(plane)
        {
           case 0: // X mirror
                   for(i = 0; i < size(); i++)
                       at(i).x = -at(i).x;
                   break;
           //-----------------------------------------------------------------
           case 1: // Y flip
                   for(i = 0; i < size(); i++)
                       at(i).y = -at(i).y;
                   break;
           //-----------------------------------------------------------------
           case 2: // Z
                   for(i = 0; i < size(); i++)
                       at(i).z = -at(i).z;
                   break;
           //-----------------------------------------------------------------
           case 3: // X, Y
                   for(i = 0; i < size(); i++)
                   {
                       at(i).x = -at(i).x;
                       at(i).y = -at(i).y;
                   }
                   break;
           //-----------------------------------------------------------------
           case 4: // X, Y, Z
                   for(i = 0; i < size(); i++)
                       at(i) = -at(i);
                   break;
           //-----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::quarter_turn(u_int plane, u_int turns)
{
    if(size() > 1)
    {
        size_t  i;
        u_int   j;
        double  temp;
        switch(plane)
        {
           case 0: // X Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 1: // Z Y
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).z;
                           at(i).z = at(i).y;
                           at(i).y = temp;
                       }
                   break;
           //-----------------------------------------------------------------
           case 2: // X Z
                   for(i = 0; i < size(); i++)
                       for(j = 0; j < turns; j++)
                       {
                           temp = -at(i).x;
                           at(i).x = at(i).z;
                           at(i).z = temp;
                       }
                   break;
           //-----------------------------------------------------------------
        }
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::flatten_z()
{
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
            at(i).z = 0;
    return;
}

//############################################################################
double LaserBoy_real_segment::vector_angle(size_t vertex_index) const
{
    double angle = 0.0;
    if(vertex_index != 0)
    {
        if(size() > 1)
        {
            int _0  = 0,
                _1  = 0,
                _2  = 0;
           //-----------------------------------------------------------------
            if(vertex_index > 0 && vertex_index < size() - 1)
            {
                _0 = vertex_index - 1;
                _1 = vertex_index    ;
                _2 = vertex_index + 1;
            }
           //-----------------------------------------------------------------
            else if(vertex_index == size() - 1)
            {
                _0 = size() - 2;
                _1 = 0                     ;
                _2 = 1                     ;
            }
            //----------------------------------------------------------------
            angle = delta_angle( at(_0) | at(_2), // distance between vertices 0,2
                                 at(_0) | at(_1),
                                 at(_1) | at(_2)
                               );
        }
    }
    return angle;
}

//############################################################################
double LaserBoy_real_segment::total_angle() const
{
    double  total = 0.0;
    size_t  i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            total += vector_angle(i);
    return total;
}

//############################################################################
double LaserBoy_real_segment::max_angle() const
{
    double  max = 0.0;
    size_t  i;
    if(size() > 1)
      for(i = 0; i < size() - 1; i++)
          if(max < vector_angle(i))
              max = vector_angle(i);
    return max;
}

//############################################################################
double LaserBoy_real_segment::total_magnitude() const
{
    double  total = 0.0;
    size_t  i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            total += (at(i) | at(i + 1));
    return total;
}

//############################################################################
double LaserBoy_real_segment::max_magnitude() const
{
    double  max = 0.0,
            d;
    size_t  i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
        {
            d = (at(i) | at(i + 1));
            if(max < d)
                max = d;
        }
    return max;
}

//############################################################################
double LaserBoy_real_segment::max_color_magnitude() const
{
    double  max = 0.0,
            d;
    size_t  i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            if(at(i + 1).is_color(p_space->black_level))
            {
                d = (at(i) | at(i + 1));
                if(max < d)
                    max = d;
            }
    return max;
}

//############################################################################
double LaserBoy_real_segment::max_dark_magnitude() const
{
    double  max = 0.0,
            d;
    size_t  i;
    if(size() > 1)
        for(i = 0; i < size() - 1; i++)
            if(at(i + 1).is_dark(p_space->black_level))
            {
                d = (at(i) | at(i + 1));
                if(max < d)
                    max = d;
            }
    return max;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::move_(LaserBoy_3D_double displacement,
                                                    double             move_acceleration
                                                   )
{
    if(size() > 1)
    {
        double step,
               dsize = size() - 1.0;
        if(move_acceleration == 0.0)
            for(size_t i = 0; i < size(); i++)
                at(i) += (displacement * (i / dsize));
        else if(move_acceleration > 0.0)
        {
            step = move_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(i) += (displacement * (i / dsize) * pow(_e_, -(move_acceleration - (i * step))));
        }
        else // if(move_acceleration < 0.0)
        {
            move_acceleration = -move_acceleration;
            step = move_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(dsize - i) += (displacement - (displacement * (i / dsize)) * pow(_e_, -(move_acceleration - (i * step))));
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::scale_(LaserBoy_3D_double factor,  // from
                                                     LaserBoy_3D_double factor_, // to
                                                     double             scale_acceleration
                                                    )
{
    if(    (    factor != 1.0
             || factor_   != 1.0
           )
        && size() > 1
      )
    {
        double             step,
                           dsize = size() - 1.0;
        LaserBoy_3D_double diff;
        if(scale_acceleration == 0.0)
        {
            diff = factor_ - factor;
            for(size_t i = 0; i < size(); i++)
                at(i) *= (factor + (diff * (i / dsize)));
        }
        else if(scale_acceleration > 0.0)
        {
            diff = factor_ - factor;
            step = scale_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(i) *= (factor + (diff * (i / dsize) * pow(_e_, -(scale_acceleration - (i * step)))));
        }
        else // if(scale_acceleration < 0.0)
        {
            diff = factor - factor_;
            scale_acceleration = -scale_acceleration;
            step = scale_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(dsize - i) *= (factor_ + (diff * (i / dsize) * pow(_e_, -(scale_acceleration - (i * step)))));
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::rotate_(LaserBoy_3D_double fulcrum,
                                                      LaserBoy_3D_double fulcrum_,
                                                      double             fulcrum_acceleration,
                                                      LaserBoy_3D_double rotation,
                                                      double             rotate_acceleration
                                                     )
{
    double                r_step,
                          f_step,
                          dsize = size() - 1.0;
    LaserBoy_3D_double    diff = fulcrum_ - fulcrum;
    if(size() > 1 && rotation != 0.0)
    {
        if(fulcrum_acceleration == 0.0)
        {
            if(rotate_acceleration == 0.0)
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum  + diff * (i / dsize),
                                                         rotation * (i / dsize)
                                                        );
            else if(rotate_acceleration > 0.0)
            {
                r_step = rotate_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum  + diff * (i / dsize),
                                                         rotation * (i / dsize) * pow(_e_, -(rotate_acceleration - (i * r_step)))
                                                        );
            }
            else // if(rotate_acceleration < 0.0)
            {
                rotate_acceleration = -rotate_acceleration;
                r_step = rotate_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(dsize - i) = rotate_vertex_on_coordinates(at(dsize - i),
                                                                 fulcrum  + diff * (i / dsize),
                                                                 rotation - (rotation * (i / dsize) * pow(_e_, -(rotate_acceleration - (i * r_step))))
                                                                );
          }
        } // end if(fulcrum_acceleration == 0.0)
        else if(fulcrum_acceleration > 0.0)
        {
            if(rotate_acceleration == 0.0)
                for(size_t i = 0; i < size(); i++)
                {
                    f_step = fulcrum_acceleration / size();
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                         rotation * (i / dsize)
                                                        );
            }
            else if(rotate_acceleration > 0.0)
            {
                r_step = rotate_acceleration  / size();
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                         rotation * (i / dsize) * pow(_e_, -(rotate_acceleration - (i * r_step)))
                                                        );
        }
            else // if(rotate_acceleration < 0.0)
            {
                rotate_acceleration = -rotate_acceleration;
                r_step = rotate_acceleration  / size();
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(dsize - i) = rotate_vertex_on_coordinates(at(dsize - i),
                                                                 fulcrum  + (diff     * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                                 rotation - (rotation * (i / dsize) * pow(_e_, -(rotate_acceleration  - (i * r_step))))
                                                                );
        }
        } // end else if(fulcrum_acceleration > 0.0)
        else //  (fulcrum_acceleration < 0.0)
        {
            if(rotate_acceleration == 0.0)
                for(size_t i = 0; i < size(); i++)
                {
                    fulcrum_acceleration = -fulcrum_acceleration;
                    f_step = fulcrum_acceleration / size();
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                         rotation * (i / dsize)
                                                        );
            }
            else if(rotate_acceleration > 0.0)
            {
                r_step = rotate_acceleration  / size();
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                         rotation * (i / dsize) * pow(_e_, -(rotate_acceleration - (i * r_step)))
                                                        );
            }
            else // if(rotate_acceleration < 0.0)
            {
                fulcrum_acceleration = -fulcrum_acceleration;
                rotate_acceleration  = -rotate_acceleration;
                f_step = fulcrum_acceleration / size();
                r_step = rotate_acceleration  / size();
                for(size_t i = 0; i < size(); i++)
                    at(dsize - i) = rotate_vertex_on_coordinates(at(dsize - i),
                                                                 fulcrum_ - (diff     * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                                 rotation - (rotation * (i / dsize) * pow(_e_, -(rotate_acceleration  - (i * r_step))))
                                                                );
            }
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_vertex LaserBoy_real_segment::interpolated_value_at(double relative_position) const
{
    LaserBoy_real_vertex value;
    if(    size() > 1
        && relative_position >= 0.0
        && relative_position <= 1.0
      )
    {
        double location = relative_position * (size() - 1.0);
        u_int  from = (u_int)location,
               to   = from + 1;
        if(to >= size())
            to = size() - 1;
        if(from >= size())
            from = size() - 1;
        value = at(from) + ((at(to) - at(from)) * (location - from));
        if(at(from).is_lit() || at(to).is_lit())
            value.unblank();
    }
    return value;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::add(LaserBoy_real_segment rs_displacements)
{
    if(    size() > 1
        && rs_displacements.size() > 1
      )
    {
        if(size() == rs_displacements.size())
        {
            for(size_t i = 0; i < size(); i++)
                at(i) += rs_displacements.at(i);
        }
        else if(size() > rs_displacements.size())
        {
            for(size_t i = 0; i < size(); i++)
                at(i) += rs_displacements.interpolated_value_at((double)i / (size() - 1));
        }
        else
        {
            for(size_t i = 0; i < rs_displacements.size(); i++)
                rs_displacements.at(i) =   rs_displacements.at(i)
                                         + interpolated_value_at((double)i / (rs_displacements.size() - 1));
            *this = rs_displacements;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::multiply(LaserBoy_real_segment rs_factors)
{
    if(    size() > 1
        && rs_factors.size() > 1
      )
    {
        if(size() == rs_factors.size())
        {
            for(size_t i = 0; i < size(); i++)
                at(i) *= rs_factors.at(i);
        }
        else if(size() > rs_factors.size())
        {
            for(size_t i = 0; i < size(); i++)
                at(i) *= rs_factors.interpolated_value_at((double)i / (size() - 1));
        }
        else
        {
            for(size_t i = 0; i < rs_factors.size(); i++)
                rs_factors.at(i) *= interpolated_value_at((double)i / (rs_factors.size() - 1));
            *this = rs_factors;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::warp(LaserBoy_3D_double    fulcrum,
                                                   LaserBoy_3D_double    fulcrum_,
                                                   double                fulcrum_acceleration,
                                                   LaserBoy_real_segment rs_rotations
                                                  )
{
    double                f_step,
                          dsize;
    LaserBoy_3D_double    diff = fulcrum_ - fulcrum;
    LaserBoy_real_segment rs;
    if(    size() > 1
        && rs_rotations.size() > 1
      )
    {
        if(fulcrum_acceleration == 0)
        {
            if(size() == rs_rotations.size())
            {
                dsize  = size() - 1;
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum  + diff * (i / dsize),
                                                         rs_rotations.at(i) * pi
                                                        );
            }
            else if(size() > rs_rotations.size())
            {
                dsize  = size() - 1;
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum  + diff * (i / dsize),
                                                         rs_rotations.interpolated_value_at(i / dsize) * pi
                                                        );
            }
            else // (size() < rs_rotations.size())
            {
                dsize = rs_rotations.size() - 1;
                for(size_t i = 0; i < rs_rotations.size(); i++)
                    rs_rotations.at(i) = rotate_vertex_on_coordinates(interpolated_value_at(i / dsize),
                                                                      fulcrum  + diff * (i / dsize),
                                                                      rs_rotations.at(i) * pi
                                                                     );
                *this = rs_rotations;
            }
        }
        else if(fulcrum_acceleration > 0.0)
        {
            if(size() == rs_rotations.size())
            {
                dsize  = size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                         rs_rotations.at(i) * pi
                                                        );
            }
            else if(size() > rs_rotations.size())
            {
                dsize  = size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                         rs_rotations.interpolated_value_at(i / dsize) * pi
                                                        );
            }
            else // (size() < rs_rotations.size())
            {
                dsize  = rs_rotations.size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < rs_rotations.size(); i++)
                    rs_rotations.at(i) = rotate_vertex_on_coordinates(interpolated_value_at(i / dsize),
                                                                      fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                                      rs_rotations.at(i) * pi
                                                                     );
                *this = rs_rotations;
            }
        }
        else
        {
            fulcrum_acceleration = -fulcrum_acceleration;
            if(size() == rs_rotations.size())
            {
                dsize  = size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                         rs_rotations.at(i) * pi
                                                        );
            }
            else if(size() > rs_rotations.size())
            {
                dsize  = size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < size(); i++)
                    at(i) = rotate_vertex_on_coordinates(at(i),
                                                         fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                         rs_rotations.interpolated_value_at(i / dsize) * pi
                                                        );
            }
            else // (size() < rs_rotations.size())
            {
                dsize  = rs_rotations.size() - 1;
                f_step = fulcrum_acceleration / size();
                for(size_t i = 0; i < rs_rotations.size(); i++)
                    rs_rotations.at(i) = rotate_vertex_on_coordinates(interpolated_value_at(i / dsize),
                                                                      fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                                      rs_rotations.at(i) * pi
                                                                     );
                *this = rs_rotations;
            }
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::xyz_rgb(LaserBoy_real_segment rs_colors)
{
    palette_index = LASERBOY_TRUE_COLOR;
    if(    size() > 1
        && rs_colors.size() > 1
      )
    {
        if(size() == rs_colors.size())
        {
            for(size_t i = 0; i < size(); i++)
            {
                at(i).r = (char)(255 * (rs_colors.at(i).x + 1.0) / 2.0);
                at(i).g = (char)(255 * (rs_colors.at(i).y + 1.0) / 2.0);
                at(i).b = (char)(255 * (rs_colors.at(i).z + 1.0) / 2.0);
            }
        }
        else if(size() > rs_colors.size())
        {
            double dsize = size() - 1;
            for(size_t i = 0; i < size(); i++)
            {
                at(i).r = (char)(255 * (rs_colors.interpolated_value_at(i / dsize).x + 1.0) / 2.0);
                at(i).g = (char)(255 * (rs_colors.interpolated_value_at(i / dsize).y + 1.0) / 2.0);
                at(i).b = (char)(255 * (rs_colors.interpolated_value_at(i / dsize).z + 1.0) / 2.0);
            }
        }
        else
        {
            for(size_t i = 0; i < rs_colors.size(); i++)
            {
                rs_colors.at(i)   = interpolated_value_at((double)i / (rs_colors.size() - 1));
                rs_colors.at(i).r = (char)(255 * (rs_colors.at(i).x + 1.0) / 2.0);
                rs_colors.at(i).g = (char)(255 * (rs_colors.at(i).y + 1.0) / 2.0);
                rs_colors.at(i).b = (char)(255 * (rs_colors.at(i).z + 1.0) / 2.0);
            }
            *this = rs_colors;
        }
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::to_polar()
{
    for(size_t i = 0; i < size(); i++)
        at(i) = LaserBoy_3D_double(at(i).y * cos(at(i).x),
                                   at(i).y * sin(at(i).x),
                                   at(i).z
                                  );
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::differences()
{
    LaserBoy_real_segment rs;
    rs.push_back(at(0));
    for(size_t i = 1; i < size(); i++)
        rs.push_back(at(i) - at(i - 1));
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::sums()
{
    LaserBoy_real_segment rs;
    rs.push_back(at(0));
    for(size_t i = 1; i < size(); i++)
    {
        rs.push_back(rs.back() + at(i));
        rs.back().unblank();
    }
    *this = rs;
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::to_rectangular()
{   // this doesn't work!
/*
    LaserBoy_real_segment rectangular;
    if(size() > 1)
    {
        u_int                 i,
                              least = 0;
        double                rho,
                              theta,
                              diff,
                              diff_a,
                              diff_b,
                              diff_c,
                              diff_d,
                              min = DBL_MAX;
        LaserBoy_real_vertex  a,
                              b,
                              c,
                              d;
        for(i = 0; i < size(); i++)
        {
            rho   = sqrt(at(i).x * at(i).x + at(i).y * at(i).y);
            theta = atan2(at(i).y, at(i).x);
            if(theta < 0.0)
                theta += two_pi;
            rectangular.push_back(LaserBoy_real_vertex(theta,
                                                       rho,
                                                       at(i).z,
                                                       at(i).r,
                                                       at(i).g,
                                                       at(i).b,
                                                       at(i).k,
                                                       at(i).c
                                                      )
                                 );
        }
        for(i = 1; i < size(); i++)
        {
            a = rectangular.at(i);
            a.x -= pi;
            a.y = -a.y;
            a.z = 0;

            b = rectangular.at(i);
            b.x += pi;
            b.y = -b.y;
            b.z = 0;

            c = rectangular.at(i);
            c.x += two_pi;
            c.z = 0;

            d = rectangular.at(i);
            d.x += (pi * 3);
            d.y = -d.y;
            d.z = 0;

            diff   = rectangular.at(i - 1) | rectangular.at(i);
            diff_a = rectangular.at(i - 1) | a;
            diff_b = rectangular.at(i - 1) | b;
            diff_c = rectangular.at(i - 1) | c;
            diff_d = rectangular.at(i - 1) | d;

            if(diff < min)
            {
                min = diff;
                least = 0;
            }
            if(diff_a < min)
            {
                min = diff_a;
                least = 1;
            }
            if(diff_b < min)
            {
                min = diff_b;
                least = 2;
            }
            if(diff_c < min)
            {
                min = diff_c;
                least = 3;
            }
            if(diff_d < min)
            {
                min = diff_d;
                least = 4;
            }
            switch(least)
            {
                case 0:
                    break;
                case 1:
                    rectangular.at(i).x = a.x;
                    rectangular.at(i).y = a.y;
                    break;
                case 2:
                    rectangular.at(i).x = b.x;
                    rectangular.at(i).y = b.y;
                    break;
                case 3:
                    rectangular.at(i).x = c.x;
                    rectangular.at(i).y = c.y;
                    break;
                case 4:
                    rectangular.at(i).x = d.x;
                    rectangular.at(i).y = d.y;
                    break;
            }
        }
    }
*/
    return *this;
}

//############################################################################
void LaserBoy_real_segment::rotate(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        LaserBoy_3D_double center = centroid_of_coordinates();
        for(size_t i = 0; i < size(); i++)
            at(i) = rotate_vertex_on_coordinates(at(i), center, a);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::rotate_around_origin(LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) = rotate_vertex(at(i), a);
    }
    return;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::rotate_on_coordinates(LaserBoy_3D_double fulcrum,
                                                                    LaserBoy_3D_double fulcrum_,
                                                                    double             fulcrum_acceleration,
                                                                    LaserBoy_3D_double rotation
                                                                   )
{
    if(size() > 1)
    {
        double             f_step,
                           dsize = size() - 1.0;
        LaserBoy_3D_double diff  = fulcrum_ - fulcrum;
        if(fulcrum_acceleration == 0.0)
            for(size_t i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates(at(i),
                                                     fulcrum + (diff * (i / dsize)),
                                                     rotation
                                                    );
        else if(fulcrum_acceleration > 0.0)
        {
            f_step = fulcrum_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates(at(i),
                                                     fulcrum + (diff * (i / dsize) * pow(_e_, -(fulcrum_acceleration - (i * f_step)))),
                                                     rotation
                                                    );
        }
        else // (fulcrum_acceleration < 0.0)
        {
            fulcrum_acceleration = -fulcrum_acceleration;
            f_step               = fulcrum_acceleration / size();
            for(size_t i = 0; i < size(); i++)
                at(i) = rotate_vertex_on_coordinates(at(i),
                                                     fulcrum_ - (diff * (i  / dsize) * pow(_e_, -(fulcrum_acceleration - (i  * f_step)))),
                                                     rotation
                                                    );
        }
    }
    return *this;
}

//############################################################################
void LaserBoy_real_segment::rotate_on_coordinates_x(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) = rotate_vertex_on_coordinates_x(at(i), p, a);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::rotate_on_coordinates_y(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
            at(i) = rotate_vertex_on_coordinates_y(at(i), p, a);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::rotate_on_coordinates_z(LaserBoy_3D_double p, double a)
{
    if(size() > 1)
    {
        for(size_t i = 0; i < size(); i++)
           at(i) = rotate_vertex_on_coordinates_z(at(i), p, a);
    }
    return;
}

//############################################################################
bool LaserBoy_real_segment::find_rgb_in_palette(const LaserBoy_palette& palette)
{
    bool all_colors_found = true;
    if(size() > 1)
    {
        size_t        i,
                      j;
        vector<bool>  match(size(), false);
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            for(j = 0; j < palette.number_of_colors(); j++)
                if((LaserBoy_color)at(i) == palette.at(j))
                {
                    at(i).c = (u_char)j;
                    match[i] = true;
                }
        //--------------------------------------------------------------------
        for(i = 0; i < size(); i++)
            all_colors_found &= match[i];
        //--------------------------------------------------------------------
    }
    return all_colors_found;
}

//############################################################################
void LaserBoy_real_segment::promote_to_true_color()
{
    if(palette_index != LASERBOY_TRUE_COLOR)
    {
        set_rgb_from_palette();
        palette_index = LASERBOY_TRUE_COLOR;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::set_rgb_from_palette()
{
    if(    palette_index != LASERBOY_TRUE_COLOR
        && size() > 1
      )
        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).is_color(p_space->black_level))
            {
                at(i).r = p_space->palette_picker(palette_index)[at(i).c].r;
                at(i).g = p_space->palette_picker(palette_index)[at(i).c].g;
                at(i).b = p_space->palette_picker(palette_index)[at(i).c].b;
            }
            else if(at(i).is_black(p_space->black_level))
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
        }
    return;
}

//############################################################################
void LaserBoy_real_segment::set_palette_to_332()
{
    if(size() > 1)
    {
        front().c = 0x00;
        for(size_t i = 1; i < size(); i++)
            at(i).c =    (at(i).r & 0xe0)
                      | ((at(i).g & 0xe0) >> 3)
                      | ((at(i).b & 0xc0) >> 6);
        // does NOT set palette_index to LASERBOY_REDUCED_332
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::sync_rgb_and_palette()
{
    if(size() > 1)
    {
        bool fits_in_default = true;
        int  i;
        if(palette_index != LASERBOY_TRUE_COLOR)
        {
            for(i = 0; i < (int)p_space->palette_picker(palette_index).size(); i++)
                fits_in_default &= p_space->palette_picker(LASERBOY_ILDA_DEFAULT).has_color(p_space->palette_picker(palette_index).at(i));
            if(fits_in_default)
            {
                best_match_palette(LASERBOY_ILDA_DEFAULT);
                return;
            }
            set_rgb_from_palette();
            return;
        }
        //----------------------------------------------------------------
        size_t            j;
        LaserBoy_palette  palette,
                          reordered;
        //----------------------------------------------------------------
        if(first_lit_vector_index() > -1) // there are lit vectors!
        {
            palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
            //----------------------------------------------------------------
            for(i = first_lit_vector_index() + 1; i < (int)size(); i++)
            {
                for(j = 0; j < palette.number_of_colors(); j++)
                    if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                        break;
                if(    j == palette.number_of_colors()
                    && at(i).is_lit()
                  )
                    palette.push_back((LaserBoy_color)at(i));
                if(palette.number_of_colors() > LASERBOY_PALETTE_MAX)
                {
                    set_palette_to_332();
                    return;
                }
            }
            //----------------------------------------------------------------
            for(i = 0; i < (int)palette.size(); i++)
                fits_in_default &= p_space->palette_picker(LASERBOY_ILDA_DEFAULT).has_color(palette.at(i));
            if(fits_in_default)
            {
                best_match_palette(LASERBOY_ILDA_DEFAULT);
                return;
            }
            palette.reorder();
            for(i = p_space->number_of_palettes() - 1; i >= 0; i--)
            {
                reordered = p_space->palette_picker(i);
                reordered.reorder();
                if(palette.is_in(reordered))
                {
                    palette_index = i;
                    find_rgb_in_palette(p_space->palette_picker(i));
                    return;
                }
            }
            palette.find_factors();
            find_rgb_in_palette(palette);
            palette.name = GUID8char();
            p_space->push_back_palette(palette);
            palette_index = p_space->number_of_palettes() - 1;
            return;
        } // end if(first_lit_vector_index() > -1) // there are lit vectors!
        palette_index = LASERBOY_ILDA_DEFAULT;
        //----------------------------------------------------------------
    } // end if(size() > 1)
    return;
}

//############################################################################
void LaserBoy_real_segment::best_match_palette(int index)
{
    if(    index != palette_index
        && index < p_space->number_of_palettes()
      )
    {
        if(size() > 1)
        {
            if(!p_space->allow_lit_black)
                convert_black_to_blank();
            else
                impose_black_level();

            for(size_t i = 1; i < size(); i++)
                 at(i).c = p_space->
                           palette_picker(index)
                           .best_match(at(i));
        }
        palette_index = index;
        set_rgb_from_palette();
        p_space->palette_index = palette_index;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::bit_reduce_to_palette()
{
    if(palette_index == LASERBOY_TRUE_COLOR)
    {
        set_palette_to_332();
        palette_index = LASERBOY_REDUCED_332;
        set_rgb_from_palette();
    }
    return;
}


//############################################################################
void LaserBoy_real_segment::best_reduce_to_palette()
{
    if(number_of_color_vectors() >= 1)
    {
        int               local_palette_index;
        size_t            i,
                          j;
        LaserBoy_palette  palette;
        //----------------------------------------------------------------
        if(!p_space->allow_lit_black)
            convert_black_to_blank();
        else
            impose_black_level();
        //----------------------------------------------------------------
        palette.push_back((LaserBoy_color)at(first_lit_vector_index()));
        //----------------------------------------------------------------
        for(i = first_lit_vector_index() + 1; i < size(); i++)
        {
            for(j = 0; j < palette.number_of_colors(); j++)
                if(at(i).is_lit() && (palette[j] == (LaserBoy_color)at(i)))
                    break;
            if(    j == palette.number_of_colors()
                && at(i).is_lit()
              )
                palette.push_back((LaserBoy_color)at(i));
        }
        //----------------------------------------------------------------
        local_palette_index = palette.best_reduction();
        if(local_palette_index == -1)
        {
            palette.name = GUID8char();
            p_space->push_back_palette(palette);
            best_match_palette(p_space->number_of_palettes() - 1);
        }
        else
            best_match_palette(local_palette_index);
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::convert_black_to_blank()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).is_black(p_space->black_level))
            {
                at(i).r = 255;
                at(i).g = 0;
                at(i).b = 0;
                at(i).c = 0;
                at(i).blank();
            }
        }
    return;
}

//############################################################################
void LaserBoy_real_segment::convert_blank_to_black()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).is_blank())
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
                at(i).c = p_space->palette_picker(palette_index).black;
                at(i).unblank();
            }
        }
    return;
}

//############################################################################
void LaserBoy_real_segment::impose_black_level()
{
    if(size() > 1)
        for(size_t i = 1; i < size(); i++)
        {
            if(at(i).as_color().average() < p_space->black_level)
            {
                at(i).r = 0;
                at(i).g = 0;
                at(i).b = 0;
            }
        }
    return;
}

//############################################################################
void LaserBoy_real_segment::reduce_blank_vectors()
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  minimum_blanking(palette_index, false);
        minimum_blanking.push_back(front());
        minimum_blanking.back().blank();
        for(i = 1; i < size(); i++)
        {
            if(    at(i).is_blank()
                && minimum_blanking.back().is_blank()
              )
                minimum_blanking.back() = at(i); // the new, last blank
            else
                minimum_blanking.push_back(at(i));
        }
        while(minimum_blanking.size() && minimum_blanking.back().is_blank())
            minimum_blanking.pop_back();
        *this = minimum_blanking;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::reduce_lit_vectors()
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  segment_1(palette_index, false),
                               segment_2(palette_index, false);
        //----------------------------------------------------------------
        segment_1.reserve(size());
        segment_2.reserve(size());
        //----------------------------------------------------------------
        segment_1.push_back(front());
        for(i = 1; i < size(); i++)
        {
            if(at(i) != segment_1.back())
                segment_1.push_back(at(i));
        }
        //----------------------------------------------------------------
        while(segment_1.size() && segment_1.back().is_blank())
            segment_1.pop_back();
        //----------------------------------------------------------------
        if(segment_1.size())
        {
            segment_2.push_back(segment_1.front());
            for(i = 1; i < segment_1.size() - 1; i++)
            {
                if(    (    segment_1.at(i    ).color_of(p_space->black_level)
                         != segment_1.at(i + 1).color_of(p_space->black_level)
                       )
                    || (    segment_1.vector_angle(i)
                          > p_space->insignificant_angle
                       )
                  )
                    segment_2.push_back(segment_1.at(i));
            }
            //----------------------------------------------------------------
            segment_2.push_back(segment_1.at(i));
            while(segment_2.size() && segment_2.back().is_blank())
                segment_2.pop_back();
        }
        //----------------------------------------------------------------
        *this = segment_2;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::omit_equivalent_vectors()
{
    if(number_of_segments() > 1)
    {
        LaserBoy_real_segment_set  segments;
        size_t                     i,
                                   j;
        //----------------------------------------------------------------
        reduce_blank_vectors();
        reduce_lit_vectors();
        fracture_segments();
        segments = explode_segments();
        //----------------------------------------------------------------
        for(i = 0; i < segments.size() - 1; i++)
        {
            for(j = i + 1; j < segments.size(); j++)
                if(    segments[i] == segments[j]
                    || segments[i] == segments[j].reverse()
                  )
                    segments[j].clear();
        }
        //----------------------------------------------------------------
        *this = segments.sum_of_frames();
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::move(LaserBoy_3D_double d)
{
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
            at(i) += d;
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::scale_around_origin(LaserBoy_3D_double s)
{
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
            at(i) *= s;
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::scale_on_coordinates(LaserBoy_3D_double p, LaserBoy_3D_double s)
{
    if(size() > 1)
        for(size_t i = 0; i < size(); i++)
            at(i) = ((at(i).as_3D_double() - p) * s) + p;
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::scale(LaserBoy_3D_double s)
{
    return scale_on_coordinates(centroid_of_coordinates(), s);
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment::add_lit_span_vertices()
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  distance_optimized(palette_index, false),
                               line              (palette_index, false);
        //----------------------------------------------------------------
        for(i = 0; i < (size() - 1); i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_lit()
                && (at(i) | at(i + 1)) > p_space->lit_delta_max
              )
            {
                line += LaserBoy_real_segment(at(i), at(i + 1));
                line.pop_back();
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(back());
        //----------------------------------------------------------------
        *this = distance_optimized;
    }
    return real_segment_error;
}

//############################################################################
void LaserBoy_real_segment::add_lit_span_vertices(const u_int& max_d)
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  distance_optimized(palette_index, false),
                               line              (palette_index, false);
        //----------------------------------------------------------------
        for(i = 0; i < (size() - 1); i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_lit()
                && (at(i) | at(i + 1)) > max_d
              )
            {
                line += LaserBoy_real_segment(at(i), at(i + 1), max_d);
                line.pop_back();
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(back());
        *this = distance_optimized;
    }
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment::add_blank_span_vertices()
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  distance_optimized(palette_index, false),
                               line              (palette_index, false);
        //----------------------------------------------------------------
        for(i = 0; i < (size() - 1); i++)
        {
            distance_optimized.push_back(at(i));
            if(    at(i + 1).is_blank()
                && (at(i) | at(i + 1)) > p_space->blank_delta_max
              )
            {
                line += LaserBoy_real_segment(at(i), at(i + 1));
                line.pop_back();
                distance_optimized += line;
                line.clear();
            }
        }
        distance_optimized.push_back(back());
        //----------------------------------------------------------------
        *this = distance_optimized;
    }
    return real_segment_error;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::rectangular_center_of() const
{
    LaserBoy_3D_double center;
    if(size() > 1)
    {
        double  max_x = -DBL_MAX,
                min_x =  DBL_MAX,
                max_y = -DBL_MAX,
                min_y =  DBL_MAX,
                max_z = -DBL_MAX,
                min_z =  DBL_MAX;

        for(size_t i = 0; i < size(); i++)
        {
            if(at(i).x > max_x)    max_x = at(i).x;
            if(at(i).x < min_x)    min_x = at(i).x;
            if(at(i).y > max_y)    max_y = at(i).y;
            if(at(i).y < min_y)    min_y = at(i).y;
            if(at(i).z > max_z)    max_z = at(i).z;
            if(at(i).z < min_z)    min_z = at(i).z;
        }
        center.x = (max_x - ((max_x - min_x) / 2.0));
        center.y = (max_y - ((max_y - min_y) / 2.0));
        center.z = (max_z - ((max_z - min_z) / 2.0));
    }
    return center;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::mean_of_coordinates() const
{
    LaserBoy_3D_double mean;
    if(size() > 1)
    {
        mean = front();
        for(size_t i = 1; i < size(); i++)
            mean += at(i);
        mean /= size();
    }
    return mean;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_coordinates_xy(double& area, LaserBoy_real_segment& rs) const
{
    LaserBoy_3D_double centroid;
    area = 0.0;
    rs.clear();
    if(size() > 1)
    {
        size_t i;
        rs = polygon_outline_xy(area);
        if(area == 0.0) // from this perspective the segment is either a dot or a line
            return centroid;
        for(i = 0; i < rs.size() - 1; i++)
        {
            centroid.x += (rs.at(i).x + rs.at(i + 1).x) * ((rs.at(i).x * rs.at(i + 1).y) - (rs.at(i + 1).x * rs.at(i).y));
            centroid.y += (rs.at(i).y + rs.at(i + 1).y) * ((rs.at(i).x * rs.at(i + 1).y) - (rs.at(i + 1).x * rs.at(i).y));
        }
        centroid *= (1.0 / (area * 6.0));
        return centroid;
    }
    return centroid;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_coordinates_zy(double& area, LaserBoy_real_segment& rs) const
{
    LaserBoy_3D_double centroid;
    area = 0.0;
    rs.clear();
    if(size() > 1)
    {
        size_t i;
        rs = polygon_outline_zy(area);
        if(area == 0.0) // from this perspective the segment is either a dot or a line
            return centroid;
        for(i = 0; i < rs.size() - 1; i++)
        {
            centroid.z += (rs.at(i).z + rs.at(i + 1).z) * ((rs.at(i).z * rs.at(i + 1).y) - (rs.at(i + 1).z * rs.at(i).y));
            centroid.y += (rs.at(i).y + rs.at(i + 1).y) * ((rs.at(i).z * rs.at(i + 1).y) - (rs.at(i + 1).z * rs.at(i).y));
        }
        centroid *= (1.0 / (area * 6.0));
        return centroid;
    }
    return centroid;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_coordinates_xz(double& area, LaserBoy_real_segment& rs) const
{
    LaserBoy_3D_double centroid;
    area = 0.0;
    rs.clear();
    if(size() > 1)
    {
        size_t i;
        rs = polygon_outline_xz(area);
        if(area == 0.0) // from this perspective the segment is either a dot or a line
            return centroid;
        for(i = 0; i < rs.size() - 1; i++)
        {
            centroid.x += (rs.at(i).x + rs.at(i + 1).x) * ((rs.at(i).x * rs.at(i + 1).z) - (rs.at(i + 1).x * rs.at(i).z));
            centroid.z += (rs.at(i).z + rs.at(i + 1).z) * ((rs.at(i).x * rs.at(i + 1).z) - (rs.at(i + 1).x * rs.at(i).z));
        }
        centroid *= (1.0 / (area * 6.0));
        return centroid;
    }
    return centroid;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_coordinates() const
{
    LaserBoy_real_segment rs(*this);
    rs.reduce_blank_vectors();
    rs.strip_color();
    rs.reduce_lit_vectors();
    LaserBoy_3D_double    centroid = rs.rectangular_center_of();
    if((rs.size() > 2) && rs.max_dimension())
    {
        double                area_xy,
                              area_xz,
                              area_zy;
        LaserBoy_real_segment polygon_xy,
                              polygon_xz,
                              polygon_zy;
        LaserBoy_3D_double centroid_xy = rs.centroid_of_coordinates_xy(area_xy, polygon_xy),
                           centroid_xz = rs.centroid_of_coordinates_xz(area_xz, polygon_xz),
                           centroid_zy = rs.centroid_of_coordinates_zy(area_zy, polygon_zy);
        if(area_xy != 0.0 || area_xz != 0.0)
            centroid.x =    (   centroid_xy.x * area_xy
                              + centroid_xz.x * area_xz
                            )
                          / (area_xy + area_xz);
        if(area_xy != 0.0 || area_zy != 0.0)
            centroid.y =    (   centroid_xy.y * area_xy
                              + centroid_zy.y * area_zy
                            )
                          / (area_xy + area_zy);
        if(area_zy != 0.0 || area_xz != 0.0)
            centroid.z =    (   centroid_zy.z * area_zy
                              + centroid_xz.z * area_xz
                            )
                          / (area_zy + area_xz);
    }
    return centroid;
}

//############################################################################
LaserBoy_real_segment LaserBoy_real_segment::polygon_outline_xy(double& area) const
{
    LaserBoy_real_segment rs;
    area = 0.0;
    if((width() == 0.0) || (height() == 0.0))
        return rs;
    if(size() > 2)
    {
        bool               cancave_found;
        size_t             i,
                           j;
        LaserBoy_3D_double origin,
                           center;
        rs = (*this);
        for(i = 0; i < rs.size(); i++)
            rs.at(i).z = 0.0; // 2D
        rs.reduce_blank_vectors();
        rs.strip_color();
        rs.reduce_lit_vectors();
        if(rs.size() < 3)
        {
            rs.clear();
            return rs;
        }
        for(i = 0; i < rs.size(); i++)
            rs.at(i).unblank();
        center = rs.rectangular_center_of();
        rs -= center;
        rs.sort_by_rotation_xy();
        rs.reorder_from(rs.i_max_distance_from_origin_xy());
        rs.push_back(rs.front());
        do
        {
            cancave_found = false;
            for(i = 0; i < rs.size() - 2; i++)
                if(three_point_angle_xy(rs.at(i + 1), rs.at(i), rs.at(i + 2)) <= pi)
                {
                    cancave_found = true;
                    rs.remove_vertex(i + 1);
                    break;
                }
        } while(cancave_found == true);
        if(rs.back().as_3D_double() == rs.front().as_3D_double())
            rs.pop_back();
        rs.sort_by_rotation_xy();
        rs.push_back(rs.front());
        rs.front().blank();
        j = rs.size() - 2;
        for(i = 0; i < rs.size() - 1; i++)
        {
            area +=   (rs.at(j).x + rs.at(i).x)
                    * (rs.at(j).y - rs.at(i).y);
            j = i;
        }
        area = abs(area / 2.0);
        if(area != 0.0)
            return rs + center;
        return LaserBoy_real_segment();
    }
    return rs;
}

//############################################################################
LaserBoy_real_segment LaserBoy_real_segment::polygon_outline_zy(double& area) const
{
    LaserBoy_real_segment rs;
    area = 0.0;
    if((height() == 0.0) || (depth() == 0.0))
        return rs;
    if(size() > 2)
    {
        bool               cancave_found;
        size_t             i,
                           j;
        LaserBoy_3D_double origin,
                           center;
        rs = (*this);
        for(i = 0; i < rs.size(); i++)
            rs.at(i).x = 0.0; // 2D
        rs.reduce_blank_vectors();
        rs.strip_color();
        rs.reduce_lit_vectors();
        if(rs.size() < 3)
        {
            rs.clear();
            return rs;
        }
        for(i = 0; i < rs.size(); i++)
            rs.at(i).unblank();
        center = rs.rectangular_center_of();
        rs -= center;
        rs.sort_by_rotation_zy();
        rs.reorder_from(rs.i_max_distance_from_origin_zy());
        rs.push_back(rs.front());
        do
        {
            cancave_found = false;
            for(i = 0; i < rs.size() - 2; i++)
                if(three_point_angle_zy(rs.at(i + 1), rs.at(i), rs.at(i + 2)) <= pi)
                {
                    cancave_found = true;
                    rs.remove_vertex(i + 1);
                    break;
                }
        } while(cancave_found == true);
        if(rs.back().as_3D_double() == rs.front().as_3D_double())
            rs.pop_back();
        rs.sort_by_rotation_zy();
        rs.push_back(rs.front());
        rs.front().blank();
        j = rs.size() - 2;
        for(i = 0; i < rs.size() - 1; i++)
        {
            area +=   (rs.at(j).z + rs.at(i).z)
                    * (rs.at(j).y - rs.at(i).y);
            j = i;
        }
        area = abs(area / 2.0);
        if(area != 0.0)
            return rs + center;
        return LaserBoy_real_segment();
    }
    return rs;
}

//############################################################################
LaserBoy_real_segment LaserBoy_real_segment::polygon_outline_xz(double& area) const
{
    LaserBoy_real_segment rs;
    area = 0.0;
    if((width() == 0.0) || (depth() == 0.0))
        return rs;
    if(size() > 2)
    {
        bool               cancave_found;
        size_t             i,
                           j;
        LaserBoy_3D_double origin,
                           center;
        rs = (*this);
        for(i = 0; i < rs.size(); i++)
            rs.at(i).y = 0.0; // 2D
        rs.reduce_blank_vectors();
        rs.strip_color();
        rs.reduce_lit_vectors();
        if(rs.size() < 3)
        {
            rs.clear();
            return rs;
        }
        for(i = 0; i < rs.size(); i++)
            rs.at(i).unblank();
        center = rs.rectangular_center_of();
        rs -= center;
        rs.sort_by_rotation_xz();
        rs.reorder_from(rs.i_max_distance_from_origin_xz());
        rs.push_back(rs.front());
        do
        {
            cancave_found = false;
            for(i = 0; i < rs.size() - 2; i++)
                if(three_point_angle_xz(rs.at(i + 1), rs.at(i), rs.at(i + 2)) <= pi)
                {
                    cancave_found = true;
                    rs.remove_vertex(i + 1);
                    break;
                }
        } while(cancave_found == true);
        if(rs.back().as_3D_double() == rs.front().as_3D_double())
            rs.pop_back();
        rs.sort_by_rotation_xz();
        rs.push_back(rs.front());
        rs.front().blank();
        j = rs.size() - 2;
        for(i = 0; i < rs.size() - 1; i++)
        {
            area +=   (rs.at(j).x + rs.at(i).x)
                    * (rs.at(j).z - rs.at(i).z);
            j = i;
        }
        area = abs(area / 2.0);
        if(area != 0.0)
            return rs + center;
        return LaserBoy_real_segment();
    }
    return rs;
}

//############################################################################
size_t LaserBoy_real_segment::number_of_segments() const // a segment is a series of lit verticies
{
    size_t  i,
            segment_count = 0;
    if(size() > 1)
    {
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                segment_count++;
            }
        }
    }
    return segment_count;
}

//############################################################################
bool LaserBoy_real_segment::find_segment_at_index(size_t segment_index, size_t& start, size_t& end) const
{   // the first segment is number zero!
    if(size() > 1)
    {
        size_t  i;
        int     segment_count = -1;
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                segment_count++;
                if(segment_count == (int)segment_index)
                    return true;
            }
        } // segment index either negative or out of range
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1;
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                return false; // and set start, end to first segment
            }
        }
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::fracture_segments()
{
    if(number_of_lit_vectors() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  fractured(palette_index, false);
        fractured.reserve(3 * size());
        reduce_blank_vectors();
        //----------------------------------------------------------------
        for(i = 0; i < size(); i++)
        {
            fractured.push_back(at(i));
            fractured.push_back(at(i));
            fractured.back().blank();
        }
        //----------------------------------------------------------------
        fractured.pop_back();
        fractured.reduce_blank_vectors();
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::bond_segments()
{
    if(size() > 1)
    {
        size_t                 i;
        LaserBoy_real_segment  bonded(palette_index, false);
        //----------------------------------------------------------------
        bonded.reserve(size());
        bonded.push_back(front());
        for(i = 1; i < size() - 1; i++)
        {
            if( !(     at(i).is_blank()
                   &&     (   at(i)
                            | at(i - 1)
                          )
                       <= p_space->insignificant_distance
              )  )
                bonded += at(i);
        }
        bonded.push_back(back());
        //----------------------------------------------------------------
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::conglomerate_lit_segments()
{
    if(number_of_segments() > 1)
    {
        bool                       match_found = false;
        int                        i,
                                   j,
                                   least_angle_index;
        double                     angle,
                                   least_angle;
        vector<int>                match_index;
        vector<char>               match_index_type;
        LaserBoy_real_segment      copy(*this);
        LaserBoy_real_segment      rs(palette_index, false);
        LaserBoy_real_segment_set  lit_vectors;
        //----------------------------------------------------------------
//        copy.fracture_segments();
        copy.reduce_blank_vectors();
        copy.reduce_lit_vectors();
        lit_vectors = copy.explode_segments();
//        lit_vectors.reduce_blank_vectors   ();
//        lit_vectors.reduce_lit_vectors      ();
//        lit_vectors.omit_equivalent_vectors ();
        //----------------------------------------------------------------
        do
        {
            for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            {
                match_found = false;
                match_index.clear();
                match_index_type.clear();
                for(j = i + 1; j < (int)lit_vectors.number_of_frames(); j++)
                {
                    if(         lit_vectors[i].last_lit_vector().as_3D_double()
                             == lit_vectors[j].first_lit_anchor().as_3D_double()
                      )
                    {
                        match_found = true;
                        match_index_type.push_back('a');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].last_lit_vector().as_3D_double()
                             == lit_vectors[j].last_lit_vector().as_3D_double()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('b');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_double()
                             == lit_vectors[j].last_lit_vector().as_3D_double()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('c');
                        match_index.push_back(j);
                    }
                    else if(    lit_vectors[i].first_lit_anchor().as_3D_double()
                             == lit_vectors[j].first_lit_anchor().as_3D_double()
                           )
                    {
                        match_found = true;
                        match_index_type.push_back('d');
                        match_index.push_back(j);
                    }
                } // end for(j = i; j < (int)lit_vectors.number_of_frames(); j++)
                //------------------------------------------------------------
                angle             = pi;
                least_angle       = pi;
                least_angle_index = 0;
                //------------------------------------------------------------
                if(match_index.size())
                {
                    for(j = 0; j < (int)match_index.size(); j++)
                    {
                        switch(match_index_type[j])
                        {
                            case 'a':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].first_lit_vertex()
                                                         );
                                break;
                            case 'b':
                                angle = three_point_angle(lit_vectors[i].last_lit_anchor(),
                                                          lit_vectors[i].last_lit_vector(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'c':
                                angle = three_point_angle(lit_vectors[i].first_lit_vertex(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].last_lit_anchor()
                                                         );
                                break;
                            case 'd':
                                angle = three_point_angle(lit_vectors[i].first_lit_vertex(),
                                                          lit_vectors[i].first_lit_anchor(),
                                                          lit_vectors[match_index[j]].first_lit_vertex()
                                                         );
                                break;
                        } // end switch(match_index_type[j])
                        if(angle <= least_angle)
                        {
                            least_angle = angle;
                            least_angle_index = j;
                        }
                    } // end for(j = 0; j < (int)match_index.size(); j++)
                    //--------------------------------------------------------
                    switch(match_index_type[least_angle_index])
                    {
                        case 'a':
                            break;
                        case 'b':
                            lit_vectors[match_index[least_angle_index]].reverse();
                            break;
                        case 'c':
                            lit_vectors.swap_frames(i, match_index[least_angle_index]);
                            break;
                        case 'd':
                            lit_vectors[i].reverse();
                            break;
                    } // end switch(match_index_type[least_angle_index])
                    //--------------------------------------------------------
                    lit_vectors[i].is_selected = true;
                    lit_vectors[match_index[least_angle_index]].is_selected = true;
                    lit_vectors.collapse_selected_frames();
                    lit_vectors[i].bond_segments();
                    //--------------------------------------------------------
                    break;
                } // end if(match_found)
                //------------------------------------------------------------
            } // end for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
        } while(match_found);
        //----------------------------------------------------------------
//        lit_vectors.reduce_blank_vectors();
//        lit_vectors.reduce_lit_vectors  ();
        for(i = 0; i < (int)lit_vectors.number_of_frames(); i++)
            rs += lit_vectors[i];
        *this = rs;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::sort_by_rotation_xy()
{
    if(size() > 1)
    {
        size_t                    i;
        LaserBoy_3D_double        origin,
                                  base_line(32767.0, 0.0, 0.0);
        LaserBoy_real_segment     rs1(*this),
                                  rs2;
        LaserBoy_4D_double_vector _v;
        for(i = 0; i < rs1.size(); i++)
        {
            rs1.at(i).z = 0.0;
            _v.push_back(LaserBoy_4D_double(rs1.at(i).as_3D_double(),
                                            three_point_angle_xy(origin, base_line, rs1.at(i))
                                           )
                        );
        }
        _v.sort();
        for(i = 0; i < _v.size(); i++)
            rs2.push_back(LaserBoy_real_vertex( _v.at(i).x,
                                                _v.at(i).y,
                                                _v.at(i).z,
                                                255,
                                                255,
                                                255,
                                                0,
                                                55
                                              )
                         );
        *this = rs2;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::sort_by_rotation_xz()
{
    if(size() > 1)
    {
        size_t                    i;
        LaserBoy_3D_double        origin,
                                  base_line(32767.0, 0.0, 0.0);
        LaserBoy_real_segment     rs1(*this),
                                  rs2;
        LaserBoy_4D_double_vector _v;
        for(i = 0; i < rs1.size(); i++)
        {
            rs1.at(i).y = 0.0;
            _v.push_back(LaserBoy_4D_double(rs1.at(i).as_3D_double(),
                                            three_point_angle_xz(origin, base_line, rs1.at(i))
                                           )
                        );
        }
        _v.sort();
        for(i = 0; i < _v.size(); i++)
            rs2.push_back(LaserBoy_real_vertex( _v.at(i).x,
                                                _v.at(i).y,
                                                _v.at(i).z,
                                                255,
                                                255,
                                                255,
                                                0,
                                                55
                                              )
                         );
        *this = rs2;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::sort_by_rotation_zy()
{
    if(size() > 1)
    {
        size_t                    i;
        LaserBoy_3D_double        origin,
                                  base_line(0.0, 32767.0, 0.0);
        LaserBoy_real_segment     rs1(*this),
                                  rs2;
        LaserBoy_4D_double_vector _v;
        for(i = 0; i < rs1.size(); i++)
        {
            rs1.at(i).x = 0.0;
            _v.push_back(LaserBoy_4D_double(rs1.at(i).as_3D_double(),
                                            three_point_angle_zy(origin, base_line, rs1.at(i))
                                           )
                        );
        }
        _v.sort();
        for(i = 0; i < _v.size(); i++)
            rs2.push_back(LaserBoy_real_vertex( _v.at(i).x,
                                                _v.at(i).y,
                                                _v.at(i).z,
                                                255,
                                                255,
                                                255,
                                                0,
                                                55
                                              )
                         );
        *this = rs2;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment_set LaserBoy_real_segment::explode_segments() const
{
    LaserBoy_real_segment_set rss;
    //------------------------------------------------------------------------
    if(size() > 1)
    {
        size_t                 i,
                               j,
                               start,
                               end;
        LaserBoy_real_segment  rs(palette_index, false);
        //----------------------------------------------------------------
        for(i = 1; i < size(); i++)
        {
            if(at(i).is_lit())
            {
                start = i - 1; // anchor to the first lit vertex
                while(at(i).is_lit() && i < (size() - 1))
                    i++;
                //------------------------------------------------------------
                end = i - 1;
                if(    i == (size() - 1)
                    && at(i).is_lit()
                  )
                    end = i;
                //------------------------------------------------------------
                rs.clear();
                for(j = start; j <= end; j++)
                    rs.push_back(at(j));
                rss.push_back(rs);
            }
        }
    }
    //------------------------------------------------------------------------
    return rss;
}

//############################################################################
LaserBoy_real_segment LaserBoy_real_segment::copy_segment(size_t segment_index) const
{
    LaserBoy_real_segment rs;
    rs.palette_index = palette_index;
    if(size() > 1)
    {
        size_t start, end;
        if(find_segment_at_index(segment_index, start, end))
        {
            rs.clear();
            rs.reserve(end - start + 1);
            for(size_t i = start; i <= end; i++)
                rs.push_back(at(i));
        }
    }
    return rs;
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::rectangular_center_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).rectangular_center_of();
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::mean_of_coordinates_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).mean_of_coordinates();
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_segment_xy(size_t segment_index, double& area, LaserBoy_real_segment& rs) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_xy(area, rs);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_segment_xz(size_t segment_index, double& area, LaserBoy_real_segment& rs) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_xz(area, rs);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_segment_zy(size_t segment_index, double& area, LaserBoy_real_segment& rs) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates_zy(area, rs);
}

//############################################################################
LaserBoy_3D_double LaserBoy_real_segment::centroid_of_segment(size_t segment_index) const
{
    return (copy_segment(segment_index)).centroid_of_coordinates();
}

//############################################################################
void LaserBoy_real_segment::move_segment(size_t segment_index, LaserBoy_3D_double d)
{
    if(size() > 1)
    {
        size_t i    ,
               start,
               end  ;
        find_segment_at_index(segment_index, start, end);
        //----------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) += d;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::center_segment_x(size_t index)
{
    if(size() > 1)
    {
        size_t              i,
                            start,
                            end;
        LaserBoy_3D_double  center = rectangular_center_of_segment(index);
        find_segment_at_index(index, start, end);
        for(i = start; i <= end; i++)
            at(i).x = at(i).x - center.x;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::center_segment_y(size_t index)
{
    if(size() > 1)
    {
        size_t              i,
                            start,
                            end;
        LaserBoy_3D_double  center = rectangular_center_of_segment(index);
        find_segment_at_index(index, start, end);
        for(i = start; i <= end; i++)
            at(i).y = at(i).y - center.y;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::center_segment_z(size_t index)
{
    if(size() > 1)
    {
        size_t              i,
                            start,
                            end;
        LaserBoy_3D_double  center = rectangular_center_of_segment(index);
        find_segment_at_index(index, start, end);
        for(i = start; i <= end; i++)
            at(i).z = at(i).z - center.z;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::center_segment(size_t index)
{
    if(size() > 1)
    {
        size_t              i,
                            start,
                            end;
        LaserBoy_3D_double  center = rectangular_center_of_segment(index);
        find_segment_at_index(index, start, end);
        for(i = start; i <= end; i++)
            at(i) = at(i) - center;
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::rotate_segment(size_t segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t              i,
                            start,
                            end;
        LaserBoy_3D_double  center = centroid_of_segment(segment_index);
        find_segment_at_index(segment_index, start, end);
        for(i = start; i <= end; i++)
            at(i) = rotate_vertex_on_coordinates(at(i), center, a);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::rotate_segment_around_origin(size_t segment_index, LaserBoy_3D_double a)
{
    if(size() > 1)
    {
        size_t i    ,
               start,
               end  ;
        find_segment_at_index(segment_index, start, end);
        //----------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = rotate_vertex(at(i), a);
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::scale_segment(size_t segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        size_t             i     ,
                           start ,
                           end   ;
        LaserBoy_3D_double center;
        //----------------------------------------------------------------
        find_segment_at_index(segment_index, start, end);
        center = centroid_of_segment(segment_index);
        //----------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) = scale_vertex_on_coordinates(at(i), center, m);
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
void LaserBoy_real_segment::scale_segment_around_origin(size_t segment_index, LaserBoy_3D_double m)
{
    if(size() > 1)
    {
        size_t i    ,
               start,
               end  ;
        //----------------------------------------------------------------
        find_segment_at_index(segment_index, start, end);
        //----------------------------------------------------------------
        for(i = start; i <= end; i++)
            at(i) *= m;
        //----------------------------------------------------------------
    }
    return;
}

//############################################################################
bool LaserBoy_real_segment::save_as_bmp(const string&      file,
                                        u_int              x_res,
                                        u_int              y_res,
                                        u_int              rendered_line_width,
                                        bool               bmp_render_vectors,
                                        bool               bmp_render_vertices,
                                        double             scale
                                       ) const
{
    int                   bmp_center_x = x_res / 2,
                          bmp_center_y = y_res / 2;
    char                  file_name[80];
    LaserBoy_3D_double    _0,
                          _1;
    struct LaserBoy_bmp   bmp_out = {0};

    LaserBoy_real_segment copy(*this);
    if(size() > 1)
    {
        p_space->p_GUI->display_state("rendering bmp file " + file);
        //--------------------------------------------------------------------
        if(copy.number_of_unique_colors() == 1)
        {
            bmp_init(&bmp_out, x_res, y_res, 1);
            bmp_clear(&bmp_out, 1);
            copy.promote_to_true_color();
            copy.best_reduce_to_palette();
            if(p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out))
            {
                bmp_set_palette_index(&bmp_out, 0, 255, 255, 255);
                bmp_set_palette_index(&bmp_out, 1,   0,   0,   0);
            }
        }
        else if(copy.number_of_unique_colors() <= 15)
        {
            bmp_init(&bmp_out, x_res, y_res, 4);
            bmp_clear(&bmp_out, 15);
            copy.promote_to_true_color();
            copy.best_reduce_to_palette();
            if(p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out))
                return false;
        }
        else if(copy.number_of_unique_colors() <= 255)
        {
            if(copy.palette_index == LASERBOY_TRUE_COLOR)
                copy.best_reduce_to_palette();
            bmp_init(&bmp_out, x_res, y_res, 8);
            bmp_clear(&bmp_out, 255);
            p_space->palette_picker(copy.palette_index).to_bmp_palette(&bmp_out);
        }
        else
        {
            bmp_init(&bmp_out, x_res, y_res, 24);
            bmp_clear(&bmp_out, 0x00000000);
        }
        if(scale == 0)
        {
            copy.to_unit(false);
            LaserBoy_3D_double WxHxD,
                               center;
            WxHxD = copy.dimensions(center, false);
            if(WxHxD.x != 0 && WxHxD.y != 0)
                scale =   (    bmp_out.xres / WxHxD.x
                            <= bmp_out.yres / WxHxD.y
                          )
                        ? (bmp_out.xres / WxHxD.x)
                        : (bmp_out.yres / WxHxD.y);
            copy.move(-center);
        }
        //----------------------------------------------------------------
        if(scale != 0)
        {
            for(size_t i = 1; i < copy.size(); i++)
            {
                if(copy.at(i).is_color(p_space->black_level))
                {
                    _0 = copy.at(i - 1);
                    _1 = copy.at(i    );
                    //------------------------------------------------------------
                    if(copy.palette_index == LASERBOY_TRUE_COLOR)
                    {
                        if(bmp_render_vectors)
                            bmp_put_thick_line_fast(    &bmp_out
                                                     ,  (int)(_0.x * scale) + bmp_center_x
                                                     , ((int)(_0.y * scale) + bmp_center_y)
                                                     ,  (int)(_1.x * scale) + bmp_center_x
                                                     , ((int)(_1.y * scale) + bmp_center_y)
                                                     , bmp_out.make_rgb(copy.at(i).r, copy.at(i).g, copy.at(i).b)
                                                     , p_space->rendered_line_width
                                                   );
                        if(bmp_render_vertices)
                            bmp_put_vertex(&bmp_out,
                                            (int)(_1.x * scale) + bmp_center_x,
                                           ((int)(_1.y * scale) + bmp_center_y),
                                           bmp_out.make_rgb(copy.at(i).r, copy.at(i).g, copy.at(i).b),
                                           rendered_line_width
                                          );
                    }
                    else
                    {
                        if(bmp_render_vectors)
                            bmp_put_thick_line_fast(   &bmp_out
                                                     ,  (int)(_0.x * scale) + bmp_center_x
                                                     , ((int)(_0.y * scale) + bmp_center_y)
                                                     ,  (int)(_1.x * scale) + bmp_center_x
                                                     , ((int)(_1.y * scale) + bmp_center_y)
                                                     , copy.at(i).c
                                                     , p_space->rendered_line_width
                                                   );
                        if(bmp_render_vertices)
                            bmp_put_vertex(&bmp_out,
                                            (int)(_1.x * scale) + bmp_center_x,
                                           ((int)(_1.y * scale) + bmp_center_y),
                                           copy.at(i).c,
                                           rendered_line_width
                                          );
                    }
                } // end if(copy.at(i).is_color(p_space->black_level))
            } // for(size_t i = 1; i < copy.size(); i++)
        } // end if(scale != 0)
    } // end if(size() > 1)
    else
    {
        bmp_init(&bmp_out, p_space->output_bmp_size, p_space->output_bmp_size,  1);
        bmp_set_palette_index(&bmp_out, 0, 255, 255, 255);
        bmp_set_palette_index(&bmp_out, 1,   0,   0,   0);
        bmp_clear(&bmp_out, 1);
    }
    sprintf(file_name, "%s", file.c_str());
    p_space->p_GUI->display_state("writing   bmp file " + string(file_name));
    bmp_save_as(&bmp_out, file_name);
    bmp_free(&bmp_out);
    return true;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment::from_ifstream_txt(std::ifstream& in,
                                                             const u_int&   group_type,
                                                             int&           line_number
                                                            )
{
    LaserBoy_real_vertex real_vertex;
    //------------------------------------------------------------------------
    if(    group_type == LASERBOY_3D_FRAME_PALETTE
        || group_type == LASERBOY_2D_FRAME_PALETTE
      )
        palette_index = p_space->palette_index;
    else
        palette_index = LASERBOY_TRUE_COLOR;
    //------------------------------------------------------------------------
    clear();
    push_back(LaserBoy_real_vertex());
    push_back(LaserBoy_real_vertex());
    //------------------------------------------------------------------------
    if(in.good())
    {
        real_segment_error = LASERBOY_OK;
        while(real_vertex.from_ifstream_txt(in, group_type, line_number))
            push_back(real_vertex);
        sync_rgb_and_palette();
    }
    else
        real_segment_error = LASERBOY_EOF;
    //------------------------------------------------------------------------
    return real_segment_error;
}

//############################################################################
void LaserBoy_real_segment::to_ofstream_txt_color_table(std::ofstream& out)
{
    out << "# ---------------------------------------------------------------"
        << ENDL
        << "table ";
    if(p_space->save_txt_color_hex)
        out << "hex";
    else
        out << "rgb";
    out << ENDL;
    //------------------------------------------------------------------------
    for(size_t i = 0; i < size(); i++)
        if(at(i).is_lit())
            ((LaserBoy_color)at(i)).to_ofstream_txt(out, p_space->save_txt_color_hex);
        else
            out << "    -1"
                << ENDL;
    //------------------------------------------------------------------------
    out << ENDL;
    return;
}

//############################################################################
void LaserBoy_real_segment::to_ofstream_txt(std::ofstream& out, int frame_index)
{
    int output_format = (   (is_2D())
                           ? (   (p_space->save_txt_with_color)
                               ? (LASERBOY_2D_FRAME_RGB)
                               : (   (palette_index == LASERBOY_TRUE_COLOR)
                                   ? (LASERBOY_2D_FRAME_TABLE)
                                   : (LASERBOY_2D_FRAME_PALETTE)
                                 )
                             ) // 3D
                           : (   (p_space->save_txt_with_color)
                               ? (LASERBOY_3D_FRAME_RGB)
                               : (   (palette_index == LASERBOY_TRUE_COLOR)
                                   ? (LASERBOY_3D_FRAME_TABLE)
                                   : (LASERBOY_3D_FRAME_PALETTE)
                                 )
                             )
                        );
    //------------------------------------------------------------------------
    if(frame_index >= 0)
        out << "# " << setw(6) << frame_index << " ---------------------------------------------------------" << ENDL;
    out << "frame ";
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_2D_FRAME_PALETTE:
            out << "xy ";
            break;
        case LASERBOY_3D_FRAME_RGB:
        case LASERBOY_3D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_PALETTE:
            out << "xyz ";
            break;
    }
    //------------------------------------------------------------------------
    switch(output_format)
    {
        case LASERBOY_2D_FRAME_RGB:
        case LASERBOY_3D_FRAME_RGB:
            if(p_space->save_txt_color_hex)
                out << "hex";
            else
                out << "rgb";
            break;
        case LASERBOY_2D_FRAME_TABLE:
        case LASERBOY_3D_FRAME_TABLE:
            out << "table";
            break;
        case LASERBOY_2D_FRAME_PALETTE:
        case LASERBOY_3D_FRAME_PALETTE:
            out << "palette";
            break;
    }
    //------------------------------------------------------------------------
    out << " real" << ENDL;
    switch(output_format)
    {
        case LASERBOY_3D_FRAME_PALETTE:
            for(size_t i = 0; i < size(); i++)
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << setw(20) << left << at(i).z
                    << noshowpos
                    << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                    << ENDL;
            break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_PALETTE:
            for(size_t i = 0; i < size(); i++)
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << noshowpos
                    << setw(7) << ((at(i).is_lit()) ? ((int)at(i).c) : (-1))
                    << ENDL;
            break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_TABLE: // nothing but x y z
            for(size_t i = 0; i < size(); i++)
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << setw(20) << left << at(i).z
                    << noshowpos
                    << ENDL;
            break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_TABLE: // nothing but x y
            for(size_t i = 0; i < size(); i++)
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << noshowpos
                    << ENDL;
            break;
        //--------------------------------------------------------------------
        case LASERBOY_3D_FRAME_RGB:
            for(size_t i = 0; i < size(); i++)
            {
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << setw(20) << left << at(i).z
                    << noshowpos;
                if(p_space->save_txt_color_hex)
                {
                    if(at(i).is_lit())
                        out << "    0x"
                            << hex
                            << setw(6)
                            << right
                            << setfill('0')
                            << at(i).as_uint()
                            << dec
                            << left
                            << setfill(' ');
                    else
                        out << "   -1";
                }
                else
                {
                    if(at(i).is_lit())
                        out << setw(7) << (int)at(i).r
                            << setw(7) << (int)at(i).g
                            << setw(7) << (int)at(i).b;
                    else
                        out << "   -1";
                }
                out << ENDL;
            }
            break;
        //--------------------------------------------------------------------
        case LASERBOY_2D_FRAME_RGB:
            for(size_t i = 0; i < size(); i++)
            {
                out << "    "
                    << showpos
                    << setw(20) << left << at(i).x
                    << setw(20) << left << at(i).y
                    << noshowpos;
                if(p_space->save_txt_color_hex)
                {
                    if(at(i).is_lit())
                        out << "    0x"
                            << hex
                            << setw(6)
                            << right
                            << setfill('0')
                            << at(i).as_uint()
                            << dec
                            << left
                            << setfill(' ');
                    else
                        out << "   -1";
                }
                else
                {
                    if(at(i).is_lit())
                        out << setw(7) << (int)at(i).r
                            << setw(7) << (int)at(i).g
                            << setw(7) << (int)at(i).b;
                    else
                        out << "   -1";
                }
                out << ENDL;
            }
        break;
        //--------------------------------------------------------------------
    }
    out << ENDL;
    //------------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_real_segment::from_ifstream_dxf(std::ifstream& in)
{
    u_char                    dxf_color_index     ,
                              prev_dxf_color_index;
    u_int                     arc_vertices  = 0         ,
                              group_70_flag = 0         ,
                              group_71_flag = 0         ,
                              group_72_flag = 0         ,
                              group_73_flag = 0         ;
    int                       group_code                ,
                              first_segment_vertex_index;
    double                    double_value,
                              radius      ,
                              arc_start   ,
                              arc_end     ,
                              arc_angle   ,
                              arc_step    ;
    string                    entity_string,
                              font         ,
                              text         ;
    LaserBoy_ild_header_count counter;
    LaserBoy_3D_double        double_3D_1,
                              double_3D_2,
                              double_3D_3,
                              double_3D_4;
    LaserBoy_color            color,
                              prev_color;
    LaserBoy_real_segment     real_vertices;
    vector<double>            vertex_x,
                              vertex_y,
                              vertex_z;
    bool                      closed_polyline;
    //------------------------------------------------------------------------
    palette_index = LASERBOY_DXF;
    push_back(LaserBoy_real_vertex()); // every LaserBoy_real_segment::from_ifstream_dxf
    push_back(LaserBoy_real_vertex()); // has an original vector this.size() not less than 2
    //------------------------------------------------------------------------
    while(in.good() && entity_string != "ENTITIES") // ignore everthing up to ENTITIES
        in >> entity_string;
    //------------------------------------------------------------------------
    if(in.good())
    {
        while(in.good())
        {
            dxf_color_index          = p_space->palette_picker(LASERBOY_DXF).white;
            prev_dxf_color_index     = p_space->palette_picker(LASERBOY_DXF).white;
            color                      = (u_int)0X00ffffff;
            prev_color                 = (u_int)0X00ffffff;
            first_segment_vertex_index = -1,
            closed_polyline            = false;
            group_70_flag              = 0;
            radius                     = 0;
            arc_vertices               = 0;
            arc_start                  = 0;
            arc_end                    = 360;
            group_code                 = -1;
            //----------------------------------------------------------------
            vertex_x.clear();
            vertex_y.clear();
            vertex_z.clear();
            //----------------------------------------------------------------
            if(entity_string == "POINT")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                            double_3D_1.x = double_value;
                            break;
                        case  20: // y1
                            double_3D_1.y = double_value;
                            break;
                        case  30: // z1
                            double_3D_1.z = double_value;
                            break;
                        case  62: // palette_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                    }
                }
                push_back(LaserBoy_real_vertex(double_3D_1, color).blank());
                push_back(LaserBoy_real_vertex(double_3D_1, color).unblank());
            }
            else if(entity_string == "LINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                            double_3D_1.x = double_value;
                            break;
                        case  20: // y1
                            double_3D_1.y = double_value;
                            break;
                        case  30: // z1
                            double_3D_1.z = double_value;
                            break;
                        case  11: // x2
                            double_3D_2.x = double_value;
                            break;
                        case  21: // y2
                            double_3D_2.y = double_value;
                            break;
                        case  31: // z2
                            double_3D_2.z = double_value;
                            break;
                        case  62: // dxf_color_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                    }
                }
                push_back(LaserBoy_real_vertex(double_3D_1, color).blank());
                push_back(LaserBoy_real_vertex(double_3D_2, color).unblank());
            }
            //----------------------------------------------------------------
            else if(    entity_string == "CIRCLE"
                     || entity_string == "ARC"
                   )
            {
                u_int a;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                            double_3D_1.x = double_value;
                            break;
                        case  20: // y1
                            double_3D_1.y = double_value;
                            break;
                        case  30: // z1
                            double_3D_1.z = double_value;
                            break;
                        case  40: // radius
                            radius = double_value;
                            break;
                        case  50: // start angle
                            arc_start = double_value;
                            break;
                        case  51: // end angle
                            arc_end = double_value;
                            break;
                        case  62: // dxf_color_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                    }
                }
                if(arc_start >= arc_end)
                    arc_end += 360.0;
                arc_angle  = arc_end - arc_start;
                arc_vertices = (int)ceil(arc_angle / p_space->rendered_arc_angle);
                arc_step   = arc_angle / arc_vertices;
                arc_start *= one_degree;
                arc_end   *= one_degree;
                arc_step  *= one_degree;
                double_3D_2.x = radius * cos(arc_start);
                double_3D_2.y = radius * sin(arc_start);
                double_3D_2.z = double_3D_1.z;
                push_back(LaserBoy_real_vertex(double_3D_1 + double_3D_2, color).blank());
                for(a = 1; a <= arc_vertices; a++)
                {
                    double_3D_2.x = radius * cos(a * arc_step + arc_start);
                    double_3D_2.y = radius * sin(a * arc_step + arc_start);
                    double_3D_2.z = double_3D_1.z;
                    push_back(LaserBoy_real_vertex(double_3D_1 + double_3D_2, color).unblank());
                }
            }
            //----------------------------------------------------------------
            else if(entity_string == "ELLIPSE")
            {
                u_int a,
                      vertex_index;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x1
                            double_3D_1.x = double_value;
                            break;
                        case  20: // y1
                            double_3D_1.y = double_value;
                            break;
                        case  30: // z1
                            double_3D_1.z = double_value;
                            break;
                        case  11: // x2
                            double_3D_2.x = double_value;
                            break;
                        case  21: // y2
                            double_3D_2.y = double_value;
                            break;
                        case  31: // z2
                            double_3D_2.z = double_value;
                            break;
                        case  40: // ratio between major and minor axis
                            radius = double_value;
                            break;
                        case  41: // start angle
                            arc_start = double_value;
                            break;
                        case  42: // end angle
                            arc_end = double_value;
                            break;
                        case  62: // dxf_color_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                    }
                }
                if(arc_start >= arc_end) arc_end += two_pi;
                arc_angle  = arc_end - arc_start;
                arc_vertices = (int)ceil(arc_angle / (p_space->rendered_arc_angle * one_degree));
                arc_step   = arc_angle / arc_vertices;
                double major_axis = double_3D_2.magnitude(),
                      minor_axis = major_axis * radius;
                double_3D_3   = 0.0;
                double_3D_3.z = -double_3D_2.direction().z;
                real_vertices.clear();
                double_3D_4.x = -major_axis * cos(arc_start);
                double_3D_4.y = -minor_axis * sin(arc_start);
                double_3D_4.z = double_3D_1.z;
                real_vertices.push_back(LaserBoy_real_vertex(double_3D_4, color).blank());
                for(a = 1; a <= arc_vertices; a++)
                {
                    double_3D_4.x = -major_axis * cos(a * arc_step + arc_start);
                    double_3D_4.y = -minor_axis * sin(a * arc_step + arc_start);
                    double_3D_4.z = double_3D_1.z;
                    real_vertices.push_back(LaserBoy_real_vertex(double_3D_4, color).unblank());
                }
                if(double_3D_3 != 0.0)
                    for(vertex_index = 0; vertex_index < real_vertices.size(); vertex_index++)
                        real_vertices[vertex_index] = rotate_vertex(real_vertices[vertex_index], double_3D_3);
                for(vertex_index = 0; vertex_index < real_vertices.size(); vertex_index++)
                    push_back(real_vertices[vertex_index] + double_3D_1);
            }
            //----------------------------------------------------------------
            else if(entity_string == "POLYLINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  62: // dxf_color_index
                            dxf_color_index = (u_char)double_value;
                            prev_dxf_color_index = dxf_color_index;
                            prev_color = p_space->palette_picker(LASERBOY_DXF)[dxf_color_index];
                            if(prev_color.is_black())
                                prev_color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            color = p_space->palette_picker(LASERBOY_DXF)[dxf_color_index];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            prev_color = color;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                        case  70:
                            if(((int)double_value) & 0x01)
                                closed_polyline = true;
                            break;
                    }
                }
                while(entity_string == "VERTEX")
                {
                    while(get_dxf_pair(in, group_code, entity_string))
                    {
                        sscanf(entity_string.c_str(), "%lf", &double_value);
                        switch(group_code)
                        {
                            case  10: // x1
                                double_3D_1.x = double_value;
                                break;
                            case  20: // y1
                                double_3D_1.y = double_value;
                                break;
                            case  30: // z1
                                double_3D_1.z = double_value;
                                break;
                            case  62: // dxf_color_index
                                prev_dxf_color_index = dxf_color_index;
                                dxf_color_index = (u_char)double_value;
                                prev_color = p_space->palette_picker(LASERBOY_DXF)[prev_dxf_color_index];
                                if(prev_color.is_black())
                                    prev_color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                                if(color.is_black())
                                    color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                                break;
                            case 420: // true color
                                prev_color = color;
                                color = (u_int)double_value;
                                if(color.is_black())
                                    color = (u_int)0X00ffffff;
                                palette_index = LASERBOY_TRUE_COLOR;
                                break;
                        }
                    }
                    if(first_segment_vertex_index == -1)
                    {
                        push_back(LaserBoy_real_vertex(double_3D_1, color).blank());
                        first_segment_vertex_index = (int)size() - 1;
                    }
                    else
                        push_back(LaserBoy_real_vertex(double_3D_1, prev_color).unblank());
                } // end while(entity_string == "VERTEX")
                if(closed_polyline)
                    push_back(LaserBoy_real_vertex((LaserBoy_3D_double)(at(first_segment_vertex_index)), back().as_color()).unblank());
            }
            //----------------------------------------------------------------
            else if(entity_string == "LWPOLYLINE")
            {
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case  10: // x
                            vertex_x.push_back(double_value);
                            break;
                        case  20: // y
                            vertex_y.push_back(double_value);
                            break;
                        case  62: // dxf_color_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                        case  70: // is closed polyline
                            group_70_flag = (int)double_value;
                            break;
                        case  90: // number of verticies
                            arc_vertices = (int)double_value;
                            break;
                    }
                }
                if(    (arc_vertices > 0)
                    && (arc_vertices == vertex_x.size())
                    && (arc_vertices == vertex_y.size())
                  )
                {
                    double_3D_1.x = vertex_x.front();
                    double_3D_1.y = vertex_y.front();
                    double_3D_1.z = 0;
                    push_back(LaserBoy_real_vertex(double_3D_1, color).blank());
                    for(u_int a = 0; a < arc_vertices; a++)
                    {
                        double_3D_1.x = vertex_x[a];
                        double_3D_1.y = vertex_y[a];
                        push_back(LaserBoy_real_vertex(double_3D_1, color).unblank());
                    }
                    if(group_70_flag & 0x01) // closed polyline
                    {
                        double_3D_1.x = vertex_x.front();
                        double_3D_1.y = vertex_y.front();
                        push_back(LaserBoy_real_vertex(double_3D_1, color).unblank());
                    }
                }
            }
            //----------------------------------------------------------------
            else if(entity_string == "3DFACE")
            {
                u_int vertex_count = 0;
                vertex_x.resize(10);
                vertex_y.resize(10);
                vertex_z.resize(10);
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    if     (group_code < 20 && group_code >  9)
                    {
                        vertex_x[group_code - 10] = double_value;
                        vertex_count++;
                    }
                    else if(group_code < 30 && group_code > 19)
                        vertex_y[group_code - 20] = double_value;
                    else if(group_code < 40 && group_code > 29)
                        vertex_z[group_code - 30] = double_value;
                }
                double_3D_1.x = vertex_x.front();
                double_3D_1.y = vertex_y.front();
                double_3D_1.z = vertex_z.front();
                push_back(LaserBoy_real_vertex(double_3D_1, color).blank());
                for(u_int a = 0; a < vertex_count; a++)
                {
                    double_3D_1.x = vertex_x[a];
                    double_3D_1.y = vertex_y[a];
                    double_3D_1.z = vertex_z[a];
                    push_back(LaserBoy_real_vertex(double_3D_1, color).unblank());
                }
                double_3D_1.x = vertex_x.front();
                double_3D_1.y = vertex_y.front();
                double_3D_1.z = vertex_z.front();
                push_back(LaserBoy_real_vertex(double_3D_1, color).unblank());
            }
            //----------------------------------------------------------------
            else if(entity_string == "TEXT")
            {
                int                text_index,
                                   vertex_index;
                long int           bytes_skipped;
                LaserBoy_frame_set font_frames;
                double_3D_1 = 0.0;
                double_3D_3 = 0.0;
                while(get_dxf_pair(in, group_code, entity_string))
                {
                    sscanf(entity_string.c_str(), "%lf", &double_value);
                    switch(group_code)
                    {
                        case   1: // the text
                            text = entity_string;
                            break;
                        case   7: // the font
                            font = entity_string;
                            break;
                        case  10: // x1
                            double_3D_1.x = double_value;
                            break;
                        case  20: // y1
                            double_3D_1.y = double_value;
                            break;
                        case  30: // z1
                            double_3D_1.z = double_value;
                            break;
                        case  40: // text height
                            radius = double_value;
                            break;
                        case  50: // flat rotation angle
                            double_3D_3.z = double_value * one_degree;
                            break;
                        case  51: // oblique rotation angle
                            double_3D_3.y = double_value * one_degree;
                            break;
                        case  62: // dxf_color_index
                            color = p_space->palette_picker(LASERBOY_DXF)[(u_char)double_value];
                            if(color.is_black())
                                color = p_space->palette_picker(LASERBOY_DXF)[p_space->palette_picker(LASERBOY_DXF).white];
                            break;
                        case 420: // true color
                            color = (u_int)double_value;
                            if(color.is_black())
                                color = (u_int)0X00ffffff;
                            palette_index = LASERBOY_TRUE_COLOR;
                            break;
                        case  71: // flipped or mirrored
                            group_71_flag = (int)double_value;
                            break;
                        case  72: // horizontal justification
                            group_72_flag = (int)double_value;
                            break;
                        case  73: // vertical justification
                            group_73_flag = (int)double_value;
                            break;
                    }
                }
                //------------------------------------------------------------
                if(font == "ARIAL")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "arial.ild", bytes_skipped, counter);
                else if(font == "COMIC_SANS_MS")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "comic_sans.ild", bytes_skipped, counter);
                else if(font == "COURIER_NEW")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "courier_new.ild", bytes_skipped, counter);
                else if(font == "LUCIDA_CONSOLE")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "lucida.ild", bytes_skipped, counter);
                else if(font == "IMPACT")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "impact.ild", bytes_skipped, counter);
                else if(font == "TIMES_NEW_ROMAN")
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "times_new_roman.ild", bytes_skipped, counter);
                else
                    font_frames.from_ild_file(LASERBOY_ILD_SHARE + string("fonts/") + "narrow_vector.ild", bytes_skipped, counter);
                if(    font_frames.frame_set_error != LASERBOY_OK
                    || font_frames.number_of_frames() < (('~' - '!') + 1)
                  )
                    real_segment_error = LASERBOY_BAD_FONT_FILE;
                else
                {
                    real_vertices.clear();
                    for(text_index = 0; text_index < (int)text.size(); text_index++)
                    {
                        if(text[text_index] >= '!' && text[text_index] <= '~')
                        {
                            for(vertex_index = 0; vertex_index < (int)font_frames[text[text_index] - '!'].size(); vertex_index++)
                            {
                                double_3D_2 = font_frames[text[text_index] - '!'].at(vertex_index);
                                //------------------------------------------------
                                double_3D_2.x = (   (   double_3D_2.x
                                                      / (   p_space->rendered_mono_font_space
                                                          * LASERBOY_MAX_SHORT
                                                        )
                                                      + (double)(text_index)
                                                    )
                                                ) * radius;
                                //------------------------------------------------
                                double_3D_2.y = (   (   double_3D_2.y
                                                      / (   p_space->rendered_mono_font_space
                                                          * LASERBOY_MAX_SHORT
                                                        )
                                                    )
                                                ) * radius;
                                //------------------------------------------------
                                real_vertices.push_back(   (font_frames[text[text_index] - '!'].at(vertex_index).is_blank())
                                                         ? (LaserBoy_real_vertex(double_3D_2, color).blank())
                                                         : (   (font_frames[text[text_index] - '!'].at(vertex_index).is_black(p_space->black_level))
                                                             ? (LaserBoy_real_vertex(double_3D_2, LaserBoy_color(0,0,0)).unblank())
                                                             : (LaserBoy_real_vertex(double_3D_2, color).unblank())
                                                           )
                                                       );
                            }
                        }
                    }
                    if(group_71_flag & 2) // mirrored
                        real_vertices.flip(0);
                    if(group_71_flag & 4) // flipped
                        real_vertices.flip(1);
                    if(double_3D_3 != 0.0)
                        for(vertex_index = 0; vertex_index < (int)real_vertices.size(); vertex_index++)
                            real_vertices[vertex_index] = rotate_vertex(real_vertices[vertex_index], double_3D_3);
                    for(vertex_index = 0; vertex_index < (int)real_vertices.size(); vertex_index++)
                        push_back(real_vertices[vertex_index] + double_3D_1);
                }
            } // end else if(entity_string == "TEXT")
            //----------------------------------------------------------------
            else if(entity_string == "ENDSEC")
                break;
            //----------------------------------------------------------------
            else
                get_dxf_pair(in, group_code, entity_string);
        } // end while(in.good())
//        sync_rgb_and_palette();
    } // end if(in.good())
    //------------------------------------------------------------------------
    else
        real_segment_error = LASERBOY_EOF;
    //------------------------------------------------------------------------
    return real_segment_error;
}

//############################################################################
bool LaserBoy_real_segment::save_as_dxf(const string& file, bool save_true_color_dxf) const
{
    std::ofstream out(file.c_str(), ios::out);
    if(out.is_open())
    {
        size_t                     segment_vector_index,
                                   vertex_index;
        LaserBoy_real_segment_set  rss;

        //----------------------------------------------------------------
        rss = explode_segments();
        //----------------------------------------------------------------
        out << " 0\n"
                "SECTION\n"
                " 2\n"
                "ENTITIES\n";
        //----------------------------------------------------------------
        for(segment_vector_index = 0; segment_vector_index < rss.size(); segment_vector_index++)
        {
            if(rss[segment_vector_index].number_of_color_vectors() == 1)
            {
                if(     (LaserBoy_3D_double)rss[segment_vector_index].first_lit_anchor()
                     == (LaserBoy_3D_double)rss[segment_vector_index].first_lit_vertex()
                  )
                {
                    out << " 0\n"
                           "POINT\n";
                    if(save_true_color_dxf)
                        out << " 420\n"
                            << rss[segment_vector_index].first_lit_vertex().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(rss[segment_vector_index].first_lit_vertex());
                    out << "\n 10\n"
                        << rss[segment_vector_index].first_lit_anchor().x
                        << "\n 20\n"
                        << rss[segment_vector_index].first_lit_anchor().y
                        << "\n 30\n"
                        << rss[segment_vector_index].first_lit_anchor().z
                        << "\n";
                }
                else
                {
                    out << " 0\n"
                           "LINE\n";
                    if(save_true_color_dxf)
                        out << " 420\n"
                            << rss[segment_vector_index].first_lit_vertex().color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(rss[segment_vector_index].first_lit_vertex());
                    out << "\n 10\n"
                        << rss[segment_vector_index].first_lit_anchor().x
                        << "\n 20\n"
                        << rss[segment_vector_index].first_lit_anchor().y
                        << "\n 30\n"
                        << rss[segment_vector_index].first_lit_anchor().z
                        << "\n 11\n"
                        << rss[segment_vector_index].first_lit_vertex().x
                        << "\n 21\n"
                        << rss[segment_vector_index].first_lit_vertex().y
                        << "\n 31\n"
                        << rss[segment_vector_index].first_lit_vertex().z
                        << "\n";
                }
            }
            //----------------------------------------------------------------
            else
            {
                out << " 0\n"
                        "POLYLINE\n"
                        " 70\n"
                    << rss[segment_vector_index].is_closed_polygon()
                    << "\n 10\n"
                       "0.00\n"
                       " 20\n"
                       "0.00\n"
                       " 30\n"
                       "0.00\n"
                       //---------
                       " 0\n"
                       "VERTEX\n";
                if(save_true_color_dxf)
                    out << " 420\n"
                        << rss[segment_vector_index].at(1).color_of(p_space->black_level);
                else
                    out << " 62\n"
                        << p_space->
                           palette_picker(LASERBOY_DXF)
                           .best_match(rss[segment_vector_index].at(1));
                out << "\n 10\n"
                    << rss[segment_vector_index].first_lit_anchor().x
                    << "\n 20\n"
                    << rss[segment_vector_index].first_lit_anchor().y
                    << "\n 30\n"
                    << rss[segment_vector_index].first_lit_anchor().z
                    << "\n";

                for(vertex_index = 1; vertex_index < rss[segment_vector_index].size() - 1; vertex_index++)
                {
                    out << " 0\n"
                        << "VERTEX\n";
                    if(save_true_color_dxf)
                        out << " 420\n"
                            << rss[segment_vector_index].at(vertex_index + 1).color_of(p_space->black_level);
                    else
                        out << " 62\n"
                            << p_space->
                               palette_picker(LASERBOY_DXF)
                               .best_match(rss[segment_vector_index].at(vertex_index + 1));
                    out << "\n 10\n"
                        << rss[segment_vector_index].at(vertex_index).x
                        << "\n 20\n"
                        << rss[segment_vector_index].at(vertex_index).y
                        << "\n 30\n"
                        << rss[segment_vector_index].at(vertex_index).z
                        << "\n";
                }
                out << " 0\n"
                       "VERTEX\n";
                if(save_true_color_dxf)
                    out << " 420\n";
                else
                    out << " 62\n";
                out << "0\n"
                       " 10\n"
                    << rss[segment_vector_index].at(vertex_index).x
                    << "\n 20\n"
                    << rss[segment_vector_index].at(vertex_index).y
                    << "\n 30\n"
                    << rss[segment_vector_index].at(vertex_index).z
                    << "\n";
            }
        }
        //----------------------------------------------------------------
        out << " 0\n"
               "ENDSEC\n"
               " 0\n"
               "EOF\n";
        //----------------------------------------------------------------
        out.close();
        return true;
    }
    return false;
}

//############################################################################
void LaserBoy_real_segment::normalize(bool ignore_origin) // if(p_space->maintain_real_origin)
{
    if(size() > 2)
    {
        int                 _x,
                            _y,
                            _z;
        size_t              i,
                            start       = 0;
        double              real_size   =  0.0,
                            real_scale  =  1.0;
        LaserBoy_3D_double  real_min    =  DBL_MAX,
                            real_max    = -DBL_MAX,
                            real_offset =  0.0;
        if(ignore_origin)
            start = 2;
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
        }
        if(p_space->maintain_real_origin)
        {
            if(fabs(real_max.x) > real_size)
                real_size = fabs(real_max.x);
            if(fabs(real_min.x) > real_size)
                real_size = fabs(real_min.x);
            if(fabs(real_max.y) > real_size)
                real_size = fabs(real_max.y);
            if(fabs(real_min.y) > real_size)
                real_size = fabs(real_min.y);
            if(fabs(real_max.z) > real_size)
                real_size = fabs(real_max.z);
            if(fabs(real_min.z) > real_size)
                real_size = fabs(real_min.z);
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
        real_scale = 65535.0 / real_size;
        //----------------------------------------------------------------
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            _x = (int)round((at(i).x - real_offset.x) * real_scale);
            _y = (int)round((at(i).y - real_offset.y) * real_scale);
            _z = (int)round((at(i).z - real_offset.z) * real_scale);
            if(_x >  32767)    _x =  32767;
            if(_x < -32767)    _x = -32767;
            if(_y >  32767)    _y =  32767;
            if(_y < -32767)    _y = -32767;
            if(_z >  32767)    _z =  32767;
            if(_z < -32767)    _z = -32767;
            at(i).x = (double)(_x);
            at(i).y = (double)(_y);
            at(i).z = (double)(_z);
        }
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_real_segment::normalize_vectors(bool ignore_origin)
{
    if(size() > 2)
    {
        int                 _x,
                            _y,
                            _z;
        size_t              i,
                            start       =  0;
        double              real_size   =  0.0,
                            real_scale  =  1.0;
        LaserBoy_3D_double  real_min    =  DBL_MAX,
                            real_max    = -DBL_MAX,
                            real_offset =  0.0;
        if(ignore_origin)
            start = 2;
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
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
        real_scale = 65535.0 / real_size;
        //----------------------------------------------------------------
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            _x = (int)round((at(i).x - real_offset.x) * real_scale);
            _y = (int)round((at(i).y - real_offset.y) * real_scale);
            _z = (int)round((at(i).z - real_offset.z) * real_scale);
            if(_x >  32767)    _x =  32767;
            if(_x < -32767)    _x = -32767;
            if(_y >  32767)    _y =  32767;
            if(_y < -32767)    _y = -32767;
            if(_z >  32767)    _z =  32767;
            if(_z < -32767)    _z = -32767;
            at(i).x = (double)(_x);
            at(i).y = (double)(_y);
            at(i).z = (double)(_z);
        }
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_real_segment::normalize_vectors_with_origin(bool ignore_origin)
{
    if(size() > 2)
    {
        int                 _x,
                            _y,
                            _z;
        size_t              i,
                            start       =  0;
        double              real_size   =  0.0,
                            real_scale  =  1.0;
        LaserBoy_3D_double  real_min    =  DBL_MAX,
                            real_max    = -DBL_MAX;
        if(ignore_origin)
            start = 2;
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
        }
        if(fabs(real_max.x) > real_size)
            real_size = fabs(real_max.x);
        if(fabs(real_min.x) > real_size)
            real_size = fabs(real_min.x);
        if(fabs(real_max.y) > real_size)
            real_size = fabs(real_max.y);
        if(fabs(real_min.y) > real_size)
            real_size = fabs(real_min.y);
        if(fabs(real_max.z) > real_size)
            real_size = fabs(real_max.z);
        if(fabs(real_min.z) > real_size)
            real_size = fabs(real_min.z);
        real_size *= 2;
        real_scale = 65535.0 / real_size;
        //----------------------------------------------------------------
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            _x = (int)round((at(i).x) * real_scale);
            _y = (int)round((at(i).y) * real_scale);
            _z = (int)round((at(i).z) * real_scale);
            if(_x >  32767)    _x =  32767;
            if(_x < -32767)    _x = -32767;
            if(_y >  32767)    _y =  32767;
            if(_y < -32767)    _y = -32767;
            if(_z >  32767)    _z =  32767;
            if(_z < -32767)    _z = -32767;
            at(i).x = (double)(_x);
            at(i).y = (double)(_y);
            at(i).z = (double)(_z);
        }
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_real_segment::to_unit(bool ignore_origin)
{
    if(size() > 2)
    {
        size_t              i,
                            start       =  0;
        double              real_size   =  0.0,
                            real_scale  =  1.0;
        LaserBoy_3D_double  real_min    =  DBL_MAX,
                            real_max    = -DBL_MAX,
                            real_offset =  0.0;
        if(ignore_origin)
            start = 2;
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
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
        real_scale = 2.0 / real_size;
        //----------------------------------------------------------------
        for(i = start; i < size(); i++) // ignore the origin vector
            at(i) = (at(i) - real_offset) * real_scale;
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
void LaserBoy_real_segment::to_unit_with_origin(bool ignore_origin)
{
    if(size() > 2)
    {
        size_t              i,
                            start       =  0;
        double              real_size   =  0.0,
                            real_scale  =  1.0;
        LaserBoy_3D_double  real_min    =  DBL_MAX,
                            real_max    = -DBL_MAX;
        if(ignore_origin)
            start = 2;
        for(i = start; i < size(); i++) // ignore the origin vector
        {
            if(at(i).x > real_max.x)    real_max.x = at(i).x;
            if(at(i).x < real_min.x)    real_min.x = at(i).x;
            if(at(i).y > real_max.y)    real_max.y = at(i).y;
            if(at(i).y < real_min.y)    real_min.y = at(i).y;
            if(at(i).z > real_max.z)    real_max.z = at(i).z;
            if(at(i).z < real_min.z)    real_min.z = at(i).z;
        }
        if(fabs(real_max.x) > real_size)
            real_size = fabs(real_max.x);
        if(fabs(real_min.x) > real_size)
            real_size = fabs(real_min.x);
        if(fabs(real_max.y) > real_size)
            real_size = fabs(real_max.y);
        if(fabs(real_min.y) > real_size)
            real_size = fabs(real_min.y);
        if(fabs(real_max.z) > real_size)
            real_size = fabs(real_max.z);
        if(fabs(real_min.z) > real_size)
            real_size = fabs(real_min.z);
        real_size *= 2;
        real_scale = 2.0 / real_size;
        //----------------------------------------------------------------
        for(i = start; i < size(); i++) // ignore the origin vector
            at(i) *= real_scale;
    }
    //--------------------------------------------------------------------
    return;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::clip(const LaserBoy_3D_double& max,
                                                   const LaserBoy_3D_double& min,
                                                   const double&             granularity
                                                  )
{
    if(size() > 1)
    {
        LaserBoy_real_segment clipped;
        LaserBoy_3D_double    c1,
                              c2;
        LaserBoy_real_vertex  blank;
        //----------------------------------------------------------------
        if(clip_check(max, min) == false)
        {
            clear();
            return *this;
        }
        //----------------------------------------------------------------
        clipped.palette_index = palette_index;
        reduce_blank_vectors();
        if(LaserBoy_bounds_check(at(0), max, min)) // 0 is in bounds
        {
            switch(clip_vector(at(0), at(1), max, min, granularity, c1, c2))
            {
                case no_line:
                    clipped.push_back(blank);
                    break;
                case p1_in_p2_in: // not possible
                case p1_in_p2_out:
                    clipped.push_back(at(0));
                    break;
                case p1_out_p2_in:
                case p1_out_p2_out:
                    clipped.push_back(at(0));
                    clipped.back() = c1;
                    break;
            }
        }
        else
            clipped.push_back(at(0));
        for(size_t i = 1; i < size(); i++)
        {
            if(LaserBoy_bounds_check(at(i - 1), max, min) || LaserBoy_bounds_check(at(i), max, min))
            {
                switch(clip_vector(at(i - 1), at(i), max, min, granularity, c1, c2))
                {
                    case no_line:
                        clipped.push_back(blank);
                        break;
                    case p1_in_p2_in: // not possible
                        clipped.push_back(at(i));
                        break;
                    case p1_in_p2_out:
                        clipped.push_back(at(i));
                        clipped.back() = c2;
                        break;
                    case p1_out_p2_in:
                        clipped.push_back(blank);
                        clipped.back() = c1;
                        clipped.push_back(at(i));
                        break;
                    case p1_out_p2_out:
                        clipped.push_back(blank);
                        if(at(i).is_lit())
                        {
                            clipped.back() = c1;
                            clipped.push_back(at(i));
                            clipped.back() = c2;
                        }
                        break;
                }
            } // if(LaserBoy_bounds_check(at(i - 1), max, min) || LaserBoy_bounds_check(at(i), max, min))
            else // at(i - 1) and at(i) are both in bounds
                clipped.push_back(at(i));
        } // end for(size_t i = 1; i < size(); i++)
        clipped.reduce_blank_vectors();
        if(clipped.size() > 1)
            *this = clipped;
        else
            clear();
        return *this;
    } // end if(size() > 1)
    clear();
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::clip()
{
    return clip(LaserBoy_3D_double( 32767.0,  32767.0,  32767.0), // max
                LaserBoy_3D_double(-32767.0, -32767.0, -32767.0), // min
                65536.0                                           // granularity
               );
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::clip_around_coordinate(const LaserBoy_3D_double& center,
                                                                     const LaserBoy_3D_double& max,
                                                                     const LaserBoy_3D_double& min,
                                                                     const double&             granularity
                                                                    )
{
    LaserBoy_real_segment rs(*this);
    if(size() > 1)
    {
        rs.clip(max + center, min + center, granularity);
        rs -= center;
        *this = rs;
    }
    return *this;
}

//############################################################################
LaserBoy_real_segment& LaserBoy_real_segment::clip_around_coordinate(const LaserBoy_3D_double& center,
                                                                     const double              range,
                                                                     const double&             granularity
                                                                    )
{
    LaserBoy_real_segment rs(*this);
    if(size() > 1)
    {
        rs.clip(LaserBoy_3D_double( range + center.x,  range + center.y,  range + center.z),
                LaserBoy_3D_double(-range + center.x, -range + center.y, -range + center.z),
                granularity
               );
        rs -= center;
        *this = rs;
    }
    return *this;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
