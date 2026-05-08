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
// LaserBoy_palette_set.cpp is part of LaserBoy.
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
LaserBoy_palette& LaserBoy_palette_set::palette_picker(int index)
{
    if(index == LASERBOY_TRUE_COLOR)
        return at(target_palette_index);
    if(index < 0)
        index = number_of_palettes() - 1;
    else if(index >= (int)number_of_palettes())
        index = 0;
    return at(index);
}

//############################################################################
LaserBoy_Error_Code LaserBoy_palette_set::palette_from_ifstream_txt(std::ifstream&  in,
                                                                    const u_int&    group_type,
                                                                    int&            line_number,
                                                                    const string&   name
                                                                   )
{
    int named_palette_index = name_to_palette_index(name.substr(0, 8));
    if(named_palette_index == LASERBOY_PALETTE_NOT_FOUND)
    {
        LaserBoy_palette palette;
        if(palette.from_ifstream_txt(in, group_type, line_number))
        {
            if(name.size())
                palette.name = name.substr(0, 8);
            else
                palette.name = GUID8char();
            push_back(palette);
            palette_index = p_space->number_of_palettes() - 1;

            return LASERBOY_OK; // we accepted a new palette from txt
        }
        return LASERBOY_TXT_PALETTE_FAILED; // we did not get a new palette
    }
    else
        palette_index = named_palette_index;
    return LASERBOY_PALETTE_NAME_IN_USE; // we already have this palette
}

//############################################################################
LaserBoy_Error_Code LaserBoy_palette_set::palettes_from_txt_file(const string& file_name)
{
    int                  line_number = 1;
    string               word;
    std::ifstream        in       (file_name.c_str(), ios::in );
    std::ofstream        error_log("txt_in_errors.txt", ios::out);
    //------------------------------------------------------------------------
    palette_set_error = LASERBOY_OK;
    if(!in.is_open())
    {
        error_log << file_name
                  << " error: file open failed"
                  << ENDL;
        error_log.close();
        palette_set_error |= LASERBOY_FILE_OPEN_FAILED;
        return palette_set_error;
    }
    //------------------------------------------------------------------------
    while(in.good())
    {
        clear_to_alpha(in, line_number);
        if(get_next_word(in, word, line_number))
        {
            if     (word == "frame"  ) {} // ignore frames
            else if(word == "table"  ) {} // ignore tables
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
                            switch(palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        palette_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_NAME_NOT_FOUND;
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
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
                            palette_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                            word.clear();
                            switch(palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        // this is impossible
                                        break;
                                case LASERBOY_OK:
                                        error_log << file_name
                                                  << " on line: "
                                                  << line_number
                                                  << " new named palette added to set."
                                                  << ENDL;
                                        palette_set_error |= LASERBOY_TXT_UNEXPECED_PALETTE;
                                        break;
                                case LASERBOY_TXT_PALETTE_FAILED:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " invalid palette data."
                                                  << ENDL;
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "rgb")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette rgb name
                            switch(palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        palette_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette rgb [no-name]
                        {
                            word.clear();
                            switch(palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                    }
                    else if(word == "hex")
                    {
                        if(get_next_word(in, word, line_number))
                        {
                            // palette hex name
                            switch(palette_from_ifstream_txt(in, LASERBOY_HEX, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
                                        error_log << file_name
                                                  << " error on line: "
                                                  << line_number
                                                  << " \'"
                                                  << word
                                                  << "\' is already named in the LaserBoy palette set."
                                                  << ENDL;
                                        palette_set_error |= LASERBOY_PALETTE_NAME_IN_USE;
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                        break;
                            }
                        }
                        else // palette hex [no-name]
                        {
                            word.clear();
                            switch(palette_from_ifstream_txt(in, LASERBOY_HEX, line_number, word))
                            {
                                case LASERBOY_PALETTE_NAME_IN_USE:
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
                                        palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
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
                        palette_set_error |= LASERBOY_TXT_SYNTAX_ERROR;
                    }
                } // end if(get_next_word(in, word, line_number))
                else // there was no word after palette
                {
                    // palette rgb [no-name]
                    word.clear();
                    switch(palette_from_ifstream_txt(in, LASERBOY_RGB, line_number, word))
                    {
                        case LASERBOY_PALETTE_NAME_IN_USE:
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
                                palette_set_error |= LASERBOY_TXT_PALETTE_FAILED;
                                break;
                    }
                }
            } // end else if(word == "palette")
        } // end if(get_next_word(in, word, line_number))
    } // end while(in.good())
    //------------------------------------------------------------------------
    in.close();
    error_log.close();
    return palette_set_error;
}

//############################################################################
LaserBoy_Error_Code LaserBoy_palette_set::save_new_palettes_as_txt(const string& file)
{
    if(number_of_palettes() > LASERBOY_BUILT_IN_PALETTES)
    {
        std::ofstream out(file.c_str(), ios::out);
        if(out.is_open())
        {
            txt_tag(out);
            for(int index = LASERBOY_BUILT_IN_PALETTES; index < number_of_palettes(); index++)
                at(index).to_ofstream_txt(out);
            out.close();
            return LASERBOY_OK;
        }
        else
            return LASERBOY_FILE_OPEN_FAILED;
    }
    else
        return LASERBOY_NO_PALETTES_FOUND;
}


//############################################################################
LaserBoy_Error_Code LaserBoy_palette_set::copy_palette_from_bmp(const string& file, const string& name)
{
    char   file_name[256];
    struct LaserBoy_bmp bmp = {0};
    strcpy(file_name, file.c_str());
    if(bmp_from_file(&bmp, file_name))
    {
        if(bmp.bpp <= 8)
        {
            int              palette_at_index = -1;
            LaserBoy_palette from_bmp;
            from_bmp.reserve((u_int)(0x01 << bmp.bpp));
            for(u_int i = 0; i < (u_int)(0x01 << bmp.bpp); i++)
                from_bmp.push_back(bmp_get_palette_index(&bmp, i));
            bmp_free(&bmp);
            palette_at_index = add_unique_palette(from_bmp);
            if(palette_at_index == -1)
            {
                u_int               number = 0;
                char                number_str[12];
                string              palette_name = name;
                palette_name += "________";
                palette_name = palette_name.substr(0, 8);
                while(is_palette_name(palette_name))
                {
                    sprintf(number_str, "%u", number++);
                    palette_name = palette_name.substr(0, 8 - strlen(number_str));
                    palette_name += number_str;
                }
                target_palette_index = size() - 1;
                target_palette().name = palette_name;
                return LASERBOY_OK;
            }
            else
            {
                target_palette_index = palette_at_index;
                return LASERBOY_PALETTE_ALREADY_DEFINED;
            }
        }
        return LASERBOY_NO_PALETTES_FOUND;
    }
    return LASERBOY_FILE_OPEN_FAILED;
}

//############################################################################
void LaserBoy_palette_set::populate_palette_set()
{
    LaserBoy_palette palette;
    u_char           r = 0,
                     g = 0,
                     b = 0,
                     shift;
    int              i,
                     hue;
    //------------------------------------------------------------------------
    palette.name = "Default_";
    palette.first = 0;
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x10, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x20, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x30, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x40, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x50, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x60, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x70, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x90, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xa0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xb0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xc0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xd0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xe0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xf0, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xe0, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xc0, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xa0, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x60, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x40, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x20, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x20));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x40));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x60));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xa0));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xc0));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xe0));
    palette.push_back(LaserBoy_color(0x00, 0x82, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x72, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x68, 0xff));
    palette.push_back(LaserBoy_color(0x0a, 0x60, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x52, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x4a, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x40, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x20, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x20, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x40, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x60, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xa0, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xe0, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x20, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x40, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x60, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xa0, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xc0, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xe0, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white 55
    palette.white = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0xff, 0xe0, 0xe0));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xa0, 0xa0));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0x60, 0x60));
    palette.push_back(LaserBoy_color(0xff, 0x40, 0x40));
    palette.push_back(LaserBoy_color(0xff, 0x20, 0x20)); // last unique
    palette.last  = palette.number_of_colors() - 1;
//    palette.push_back(LaserBoy_color(0x80, 0x80, 0x80)); // gray
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.black = palette.number_of_colors() - 1;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "Dft_TTL_";
    palette.first = 0;
        palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0x00, 0xff, 0xe0));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
        palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white 55
    palette.white = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.last  = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.black = palette.number_of_colors() - 1;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "X29_____";
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.black = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.first = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white
    palette.white = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff)); //last unique
    palette.last  = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0x80, 0x80, 0x80)); // gray
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "ILDA_std";
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.black = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white
    palette.white = palette.number_of_colors() - 1;
    palette.first = palette.number_of_colors() - 1;  // first
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.first = palette.number_of_colors() - 1;
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0x8c, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0x97, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xa3, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xae, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xba, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xc5, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xd1, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xdc, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xe8, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xf3, 0x80));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xf3, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xe8, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xdc, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xd1, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xc5, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xba, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xae, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0xa3, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0x97, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0x8c, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0x80));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0x8c));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0x97));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xa3));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xae));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xba));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xc5));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xd1));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xdc));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xe8));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xf3));
    palette.push_back(LaserBoy_color(0x80, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xf3, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xe8, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xdc, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xd1, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xc5, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xba, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xae, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0xa3, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0x97, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0x8c, 0xff));
    palette.push_back(LaserBoy_color(0x80, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0x8c, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0x97, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xa3, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xae, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xba, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xc5, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xd1, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xdc, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xe8, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xf3, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xf3));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xe8));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xdc));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xd1));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xc5));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xba));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xae));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0xa3));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0x97));
    palette.push_back(LaserBoy_color(0xff, 0x80, 0x8c));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x17, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x2e, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x46, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x5d, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x74, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x8b, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xa2, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xb9, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xd1, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xe8, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xe8, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xd1, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xb9, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xa2, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x8b, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x74, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x5d, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x46, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x2e, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x17, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x17));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x2e));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x46));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x5d));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x74));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x8b));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xa2));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xb9));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xd1));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xe8));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xe8, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xd1, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xb9, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xa2, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x8b, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x74, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x5d, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x46, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x2e, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x17, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x17, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x2e, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x46, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x5d, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x74, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x8b, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xa2, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xb9, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xd1, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xe8, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xe8));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xd1));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xb9));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xa2));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x8b));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x74));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x5d));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x46));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x2e));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x17));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x0c, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x17, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x23, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x2f, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x3a, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x46, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x51, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x5d, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x69, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x74, 0x00));
    palette.push_back(LaserBoy_color(0x80, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x74, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x69, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x5d, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x51, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x46, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x3a, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x2f, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x23, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x17, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x0c, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x0c));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x17));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x23));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x2f));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x3a));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x46));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x51));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x5d));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x69));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x74));
    palette.push_back(LaserBoy_color(0x00, 0x80, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x74, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x69, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x5d, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x51, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x46, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x3a, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x2f, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x23, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x17, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x0c, 0x80));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x0c, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x17, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x23, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x2f, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x3a, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x46, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x51, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x5d, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x69, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x74, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x80));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x74));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x69));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x5d));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x51));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x46));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x3a));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x2f));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x23));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x17));
    palette.push_back(LaserBoy_color(0x80, 0x00, 0x0c));
    palette.push_back(LaserBoy_color(0xff, 0xc0, 0xc0));
    palette.push_back(LaserBoy_color(0xff, 0x40, 0x40));
    palette.push_back(LaserBoy_color(0xc0, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x40, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xc0));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x40));
    palette.push_back(LaserBoy_color(0xc0, 0xc0, 0x00));
    palette.push_back(LaserBoy_color(0x40, 0x40, 0x00));
    palette.push_back(LaserBoy_color(0xc0, 0xff, 0xc0));
    palette.push_back(LaserBoy_color(0x40, 0xff, 0x40));
    palette.push_back(LaserBoy_color(0x00, 0xc0, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0x40, 0x00));
    palette.push_back(LaserBoy_color(0xc0, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x40, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xc0, 0xc0));
    palette.push_back(LaserBoy_color(0x00, 0x40, 0x40));
    palette.push_back(LaserBoy_color(0xc0, 0xc0, 0xff));
    palette.push_back(LaserBoy_color(0x40, 0x40, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xc0));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x40));
    palette.push_back(LaserBoy_color(0xff, 0xc0, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x40, 0xff));
    palette.push_back(LaserBoy_color(0xc0, 0x00, 0xc0));
    palette.push_back(LaserBoy_color(0x40, 0x00, 0x40));
    palette.push_back(LaserBoy_color(0xff, 0x60, 0x60));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white
    palette.push_back(LaserBoy_color(0xf5, 0xf5, 0xf5));
    palette.push_back(LaserBoy_color(0xeb, 0xeb, 0xeb));
    palette.push_back(LaserBoy_color(0xe0, 0xe0, 0xe0));
    palette.push_back(LaserBoy_color(0xd5, 0xd5, 0xd5));
    palette.push_back(LaserBoy_color(0xcb, 0xcb, 0xcb));
    palette.push_back(LaserBoy_color(0xc0, 0xc0, 0xc0));
    palette.push_back(LaserBoy_color(0xb5, 0xb5, 0xb5));
    palette.push_back(LaserBoy_color(0xab, 0xab, 0xab));
    palette.push_back(LaserBoy_color(0xa0, 0xa0, 0xa0));
    palette.push_back(LaserBoy_color(0x95, 0x95, 0x95));
    palette.push_back(LaserBoy_color(0x8b, 0x8b, 0x8b));
    palette.push_back(LaserBoy_color(0x80, 0x80, 0x80)); // gray
    palette.push_back(LaserBoy_color(0x75, 0x75, 0x75));
    palette.push_back(LaserBoy_color(0x6b, 0x6b, 0x6b));
    palette.push_back(LaserBoy_color(0x60, 0x60, 0x60));
    palette.push_back(LaserBoy_color(0x55, 0x55, 0x55));
    palette.push_back(LaserBoy_color(0x4b, 0x4b, 0x4b));
    palette.push_back(LaserBoy_color(0x40, 0x40, 0x40));
    palette.push_back(LaserBoy_color(0x35, 0x35, 0x35));
    palette.push_back(LaserBoy_color(0x2b, 0x2b, 0x2b));
    palette.push_back(LaserBoy_color(0x20, 0x20, 0x20));
    palette.push_back(LaserBoy_color(0x15, 0x15, 0x15));
    palette.push_back(LaserBoy_color(0x0b, 0x0b, 0x0b));
    palette.last  = palette.number_of_colors() - 1;  // last
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "DXF_____";
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.black = palette.number_of_colors() - 1;  // black
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.first = palette.number_of_colors() - 1;  // first
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0x7f, 0x7f));
    palette.push_back(LaserBoy_color(0xbf, 0xbf, 0xbf));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0x7f));
    palette.push_back(LaserBoy_color(0xa5, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0x52));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x3f));
    palette.push_back(LaserBoy_color(0x4c, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x13));
    palette.push_back(LaserBoy_color(0xff, 0x3f, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0x9f, 0x7f));
    palette.push_back(LaserBoy_color(0xa5, 0x29, 0x00));
    palette.push_back(LaserBoy_color(0xa5, 0x67, 0x52));
    palette.push_back(LaserBoy_color(0x7f, 0x1f, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0x4f, 0x3f));
    palette.push_back(LaserBoy_color(0x4c, 0x13, 0x00));
    palette.push_back(LaserBoy_color(0x4c, 0x2f, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x09, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x17, 0x13));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xbf, 0x7f));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0x00));
    palette.push_back(LaserBoy_color(0xa5, 0x7c, 0x52));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0x5f, 0x3f));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x4c, 0x39, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x1c, 0x13));
    palette.push_back(LaserBoy_color(0xff, 0xbf, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xdf, 0x7f));
    palette.push_back(LaserBoy_color(0xa5, 0x7c, 0x00));
    palette.push_back(LaserBoy_color(0xa5, 0x91, 0x52));
    palette.push_back(LaserBoy_color(0x7f, 0x5f, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0x6f, 0x3f));
    palette.push_back(LaserBoy_color(0x4c, 0x39, 0x00));
    palette.push_back(LaserBoy_color(0x4c, 0x42, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x1c, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x21, 0x13));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0xa5, 0xa5, 0x00));
    palette.push_back(LaserBoy_color(0xa5, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x7f, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x4c, 0x4c, 0x00));
    palette.push_back(LaserBoy_color(0x4c, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0xbf, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xdf, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0x7c, 0xa5, 0x00));
    palette.push_back(LaserBoy_color(0x91, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x5f, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0x6f, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x39, 0x4c, 0x00));
    palette.push_back(LaserBoy_color(0x42, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x1c, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x21, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0xbf, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0x00));
    palette.push_back(LaserBoy_color(0x7c, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0x5f, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x00));
    palette.push_back(LaserBoy_color(0x39, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x1c, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0x3f, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x9f, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0x29, 0xa5, 0x00));
    palette.push_back(LaserBoy_color(0x67, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x1f, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0x4f, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x13, 0x4c, 0x00));
    palette.push_back(LaserBoy_color(0x2f, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x09, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x17, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x00));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0xa5, 0x00));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0x00));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x00, 0x4c, 0x00));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x00));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x3f));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0x9f));
    palette.push_back(LaserBoy_color(0x00, 0xa5, 0x29));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0x67));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0x1f));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x4f));
    palette.push_back(LaserBoy_color(0x00, 0x4c, 0x13));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x2f));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x09));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x17));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0x7f));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0xbf));
    palette.push_back(LaserBoy_color(0x00, 0xa5, 0x52));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0x7c));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0x3f));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x5f));
    palette.push_back(LaserBoy_color(0x00, 0x4c, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x39));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x13));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x1c));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xbf));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0xdf));
    palette.push_back(LaserBoy_color(0x00, 0xa5, 0x7c));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0x91));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0x5f));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x6f));
    palette.push_back(LaserBoy_color(0x00, 0x4c, 0x39));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x42));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x1c));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x21));
    palette.push_back(LaserBoy_color(0x00, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0xff, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0xa5, 0xa5));
    palette.push_back(LaserBoy_color(0x52, 0xa5, 0xa5));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0x7f));
    palette.push_back(LaserBoy_color(0x3f, 0x7f, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0x4c, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x4c, 0x4c));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x26, 0x26));
    palette.push_back(LaserBoy_color(0x00, 0xbf, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0xdf, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x7c, 0xa5));
    palette.push_back(LaserBoy_color(0x52, 0x91, 0xa5));
    palette.push_back(LaserBoy_color(0x00, 0x5f, 0x7f));
    palette.push_back(LaserBoy_color(0x3f, 0x6f, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0x39, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x42, 0x4c));
    palette.push_back(LaserBoy_color(0x00, 0x1c, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x21, 0x26));
    palette.push_back(LaserBoy_color(0x00, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0xbf, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x52, 0x7c, 0xa5));
    palette.push_back(LaserBoy_color(0x00, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x3f, 0x5f, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x39, 0x4c));
    palette.push_back(LaserBoy_color(0x00, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x1c, 0x26));
    palette.push_back(LaserBoy_color(0x00, 0x3f, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0x9f, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x29, 0xa5));
    palette.push_back(LaserBoy_color(0x52, 0x67, 0xa5));
    palette.push_back(LaserBoy_color(0x00, 0x1f, 0x7f));
    palette.push_back(LaserBoy_color(0x3f, 0x4f, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0x13, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x2f, 0x4c));
    palette.push_back(LaserBoy_color(0x00, 0x09, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x17, 0x26));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x7f, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0xa5));
    palette.push_back(LaserBoy_color(0x52, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0x3f, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x13, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0x3f, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0x9f, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0x29, 0x00, 0xa5));
    palette.push_back(LaserBoy_color(0x67, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x1f, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0x4f, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x13, 0x00, 0x4c));
    palette.push_back(LaserBoy_color(0x2f, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x09, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x17, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xbf, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0x52, 0x00, 0xa5));
    palette.push_back(LaserBoy_color(0x7c, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x3f, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0x5f, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x4c));
    palette.push_back(LaserBoy_color(0x39, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x13, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x1c, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0xbf, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xdf, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0x7c, 0x00, 0xa5));
    palette.push_back(LaserBoy_color(0x91, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x5f, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0x6f, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x39, 0x00, 0x4c));
    palette.push_back(LaserBoy_color(0x42, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x1c, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x21, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xff));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0xff));
    palette.push_back(LaserBoy_color(0xa5, 0x00, 0xa5));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0xa5));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x7f));
    palette.push_back(LaserBoy_color(0x4c, 0x00, 0x4c));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x4c));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x26));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0xbf));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0xdf));
    palette.push_back(LaserBoy_color(0xa5, 0x00, 0x7c));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0x91));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0x5f));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x6f));
    palette.push_back(LaserBoy_color(0x4c, 0x00, 0x39));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x42));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x1c));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x21));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x7f));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0xbf));
    palette.push_back(LaserBoy_color(0xa5, 0x00, 0x52));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0x7c));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0x3f));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x5f));
    palette.push_back(LaserBoy_color(0x4c, 0x00, 0x26));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x39));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x13));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x1c));
    palette.push_back(LaserBoy_color(0xff, 0x00, 0x3f));
    palette.push_back(LaserBoy_color(0xff, 0x7f, 0x9f));
    palette.push_back(LaserBoy_color(0xa5, 0x00, 0x29));
    palette.push_back(LaserBoy_color(0xa5, 0x52, 0x67));
    palette.push_back(LaserBoy_color(0x7f, 0x00, 0x1f));
    palette.push_back(LaserBoy_color(0x7f, 0x3f, 0x4f));
    palette.push_back(LaserBoy_color(0x4c, 0x00, 0x13));
    palette.push_back(LaserBoy_color(0x4c, 0x26, 0x2f));
    palette.push_back(LaserBoy_color(0x26, 0x00, 0x09));
    palette.push_back(LaserBoy_color(0x26, 0x13, 0x17));
    palette.push_back(LaserBoy_color(0x54, 0x54, 0x54));
    palette.push_back(LaserBoy_color(0x76, 0x76, 0x76));
    palette.push_back(LaserBoy_color(0x98, 0x98, 0x98));
    palette.push_back(LaserBoy_color(0xba, 0xba, 0xba));
    palette.push_back(LaserBoy_color(0xdc, 0xdc, 0xdc));
    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff));
    palette.white = palette.number_of_colors() - 1;  //white
    palette.last  = palette.number_of_colors() - 1;  //last
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "ezfbHues";
    for(i = 0; i < LASERBOY_PALETTE_MAX - 1; i++)
    {
        hue = i * 1528 / LASERBOY_PALETTE_MAX;
        shift = hue % 255;
        r = g = b = 0;
        if(hue < 255)
        {
            r = 255;
            g = shift;
        }
        else if(hue < 510)
        {
            r = 256 - shift;
            g = 255;
        }
        else if(hue < 765)
        {
            g = 255;
            b = shift;
        }
        else if(hue < 1020)
        {
            g = 256 - shift;
            b = 255;
        }
        else if(hue < 1275)
        {
            r = shift;
            b = 255;
        }
        else
        {
            r = 255;
            b = 256 - shift;
        }
        palette.push_back(LaserBoy_color(r, g, b));
    }
//    palette.push_back(LaserBoy_color(0xff, 0xff, 0xff)); // white
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.first = 0;
    palette.black = 255;
    palette.white = 254;
    palette.last  = 254;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "RedGreen";
    palette.first = 0;
    for(i = 0; i < LASERBOY_PALETTE_MAX - 1; i++)
    {
        hue = i * 510 / (LASERBOY_PALETTE_MAX - 1);
        shift = hue % 255;
        if(hue < 255)
        {
            r = 255;
            g = shift;
        }
        else if(hue < 510)
        {
            r = 253 - shift;
            g = 255;
        }
        palette.push_back(LaserBoy_color(r, g, 0));
    }
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00)); // black
    palette.last  = 254;
    palette.white = 254;
    palette.black = 255;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "ezfb_332";
    for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
        palette.push_back(LaserBoy_color(  i & 0xe0,
                                      (i & 0x1c) << 3,
                                      (i & 0x03) << 6
                                    )
                         );
    palette.first =   1;
    palette.last  = 255;
    palette.white = 255;
    palette.black =   0;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "_Reds___";
    for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
        palette.push_back(LaserBoy_color(i, 0, 0));
    palette.first =   0;
    palette.black =   0;
    palette.white = 255;
    palette.last  = 255;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "_Greens_";
    for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
        palette.push_back(LaserBoy_color(0, i, 0));
    palette.first =   0;
    palette.black =   0;
    palette.white = 255;
    palette.last  = 255;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "_Blues__";
    for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
        palette.push_back(LaserBoy_color(0, 0, i));
    palette.first =   0;
    palette.black =   0;
    palette.white = 255;
    palette.last  = 255;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    palette.name = "_Grays__";
    for(i = 0; i < LASERBOY_PALETTE_MAX; i++)
        palette.push_back(LaserBoy_color(i, i, i));
    palette.first =   0;
    palette.black =   0;
    palette.white = 255;
    palette.last  = 255;
    push_back(palette);
    palette.clear();
    //------------------------------------------------------------------------
    for(i = LASERBOY_ILDA_DEFAULT; i < LASERBOY_BUILT_IN_PALETTES; i++)
        at(i).in_use = true;
    return;
}

//############################################################################
void LaserBoy_palette_set::add_palette()
{
    LaserBoy_palette palette;
    palette.reserve(256);
    palette.push_back(LaserBoy_color(0x00, 0x00, 0x00));
    palette.name = GUID8char();
    palette.first = 0;
    palette.last  = 0;
    palette.white = 0;
    palette.black = 0;
    push_back(palette);
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
void LaserBoy_palette_set::copy_palette(int index)
{
    LaserBoy_palette palette(palette_picker(index));
    palette.name = GUID8char();
    push_back(palette);
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
void LaserBoy_palette_set::copy_palette_reverse(int index)
{
    LaserBoy_palette palette(palette_picker(index));
    palette.reverse_order();
    if(palette.number_of_colors())
    {
        for(int i = number_of_palettes() - 1; i >= 0; i--)
            if(palette_picker(i) == palette)
            {
                target_palette_index = i;
                return;
            }
        palette.name = GUID8char();
        push_back(palette);
    }
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
void LaserBoy_palette_set::copy_palette_reorder(int index)
{
    LaserBoy_palette palette(palette_picker(index));
    palette.reorder();
    if(palette.number_of_colors())
    {
        for(int i = number_of_palettes() - 1; i >= 0; i--)
            if(palette_picker(i) == palette)
            {
                target_palette_index = i;
                return;
            }
        palette.name = GUID8char();
        push_back(palette);
    }
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
void LaserBoy_palette_set::copy_palette_negative(int index)
{
    LaserBoy_palette palette(palette_picker(index));
    palette.negative();
    if(palette.number_of_colors())
    {
        for(int i = number_of_palettes() - 1; i >= 0; i--)
            if(palette_picker(i) == palette)
            {
                target_palette_index = i;
                return;
            }
        palette.name = GUID8char();
        push_back(palette);
    }
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
void LaserBoy_palette_set::copy_palette_expanded(int index)
{
    LaserBoy_palette palette(palette_picker(index));
    palette.expand();
    if(palette.number_of_colors())
    {
        for(int i = number_of_palettes() - 1; i >= 0; i--)
            if(palette_picker(i) == palette)
            {
                target_palette_index = i;
                return;
            }
        palette.name = GUID8char();
        push_back(palette);
    }
    target_palette_index = number_of_palettes() - 1;
    return;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
