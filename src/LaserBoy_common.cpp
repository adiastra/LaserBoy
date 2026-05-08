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
// LaserBoy_common.cpp is part of LaserBoy.
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
#include "LaserBoy_common.hpp"
#include <boost/asio.hpp>

//############################################################################
LaserBoy_TUI* p_space = (LaserBoy_TUI*)0;

//############################################################################
const double pi                = (4 * atan(1.00));
const double two_pi            = (pi * 2.00 )    ;
const double quarter_pi        = (pi / 4.00 )    ;
const double half_pi           = (pi / 2.00 )    ;
const double three_quarters_pi = (pi * 0.75 )    ;
const double pi_and_a_half     = (pi * 1.50 )    ;
const double one_degree        = (pi / 180.0)    ;
const double one_radian        = (180.0 / pi)    ;
const double _e_               = std::exp(1.0)   ;

//############################################################################
const u_short short_bit_mask[16] =
{
    (u_short)0xffff, // 1111 1111 1111 1111
    (u_short)0xfffe, // 1111 1111 1111 1110
    (u_short)0xfffc, // 1111 1111 1111 1100
    (u_short)0xfff8, // 1111 1111 1111 1000

    (u_short)0xfff0, // 1111 1111 1111 0000
    (u_short)0xffe0, // 1111 1111 1110 0000
    (u_short)0xffc0, // 1111 1111 1100 0000
    (u_short)0xff80, // 1111 1111 1000 0000

    (u_short)0xff00, // 1111 1111 0000 0000
    (u_short)0xfe00, // 1111 1110 0000 0000
    (u_short)0xfc00, // 1111 1100 0000 0000
    (u_short)0xf800, // 1111 1000 0000 0000

    (u_short)0xf000, // 1111 0000 0000 0000
    (u_short)0xe000, // 1110 0000 0000 0000
    (u_short)0xc000, // 1100 0000 0000 0000
    (u_short)0x8000  // 1000 0000 0000 0000
};

//############################################################################
string GUID8char()
{
    time_t               now = time(NULL);
    string               P_name;
    static u_short       new_palette_id = 0;
    static const string  sixty_four_glyphs =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_0123456789";

// 1111 1100 0000 0000                      6 bits of new_palette_id
    P_name += sixty_four_glyphs.at(((new_palette_id & 0xfc00) >> 10));
// 0000 0011 1111 0000
    P_name += sixty_four_glyphs.at(((new_palette_id & 0x03f0) >> 4));

// 0000 0000 0000 1111                       4 bits of new_palette_id
// 1100 0000 0000 0000 0000 0000 0000 0000   2 bits of now
    P_name += sixty_four_glyphs.at(   ((new_palette_id & 0x000f) << 2)
                                    | ((now & 0xc0000000) >> 30)
                                  );

// 0011 1111 0000 0000 0000 0000 0000 0000   6 bits of now
    P_name += sixty_four_glyphs.at((now & 0x3f000000) >> 24);

// 0000 0000 1111 1100 0000 0000 0000 0000
    P_name += sixty_four_glyphs.at((now & 0x00fc0000) >> 18);

// 0000 0000 0000 0011 1111 0000 0000 0000
    P_name += sixty_four_glyphs.at((now & 0x0003f000) >> 12);

// 0000 0000 0000 0000 0000 1111 1100 0000
    P_name += sixty_four_glyphs.at((now & 0x00000fc0) >>  6);

// 0000 0000 0000 0000 0000 0000 0011 1111
    P_name += sixty_four_glyphs.at( now & 0x0000003f       );

    new_palette_id++;
    if(new_palette_id == 32768) // 1000 0000 0000 0000
    {   // 32768 new palettes names in one second is quite a lot!
        new_palette_id = 0;
        while(now == time(NULL)){/* wait for next second */};
    }
    return  P_name;
}

//############################################################################
//                 delta_02, delta_01, delta_12,
double delta_angle(double a, double b, double c) // sss triangle
{                       // returns the simple angle in the three vertices 0, 1, 2
    double angle;
    //------------------------------------------------------------------------
    if(    b == 0.0
        || c == 0.0
      )
        angle = 0;
    //------------------------------------------------------------------------
    else if(a == 0.0)
        angle = pi;
    //------------------------------------------------------------------------
    else if(a >= b + c)
        angle = 0.0;
    //------------------------------------------------------------------------
    else
        angle = pi - acos((b*b + c*c - a*a)/(2*b*c));
    //------------------------------------------------------------------------
    return angle;
}

//############################################################################
double simple_2D_angle(double x1, double y1, double x2, double y2)
{
    return atan2(y2 - y1, x2 - x1);
}

//############################################################################
u_char rescale_to_index(u_short* rescale, u_short value)
{
    u_int i;
    value &= 0xfffe; // remove possible LSB tag
    for(i = 0; i < 256; i++)
        if(rescale[i] == value)
            return (u_char)i;
    return 0x00;
}


//############################################################################
int greatest_common_devisor(int x, int y)
{
    x = abs(x);
    y = abs(y);
    int z;
    while(y != 0)
    {
        z = x % y;
        x = y;
        y = z;
    }
    return x;
}

//############################################################################
int lowest_common_denominator(int x, int y)
{
  int z,
      min = x > y ? y : x;
  for(z = 2; z < min; z++)
    if(((x % z) == 0) && ((y % z) == 0))
       break;
  if(z == min)
    return y;
  return z;
}

//############################################################################
bool file_exists(const string& file)
{
    std::ifstream in;
    in.open(file.c_str(), ios::in);
    if(in.is_open())
    {
        in.close();
        return true;
    }
    return false;
}

//############################################################################
bool directory_exists(const string& dir, const string& home)
{
    if(chdir(home.c_str())) {};
    if(chdir(dir.c_str()) == 0) // success!
    {
        if(chdir(home.c_str())) {};
        return true;
    }
    if(chdir(home.c_str())) {};
    return false;
}

//############################################################################
string time_as_string(double seconds)
{
    if(seconds >= 0)
    {
        char   buffer[256];
        string time_string;
        int    hh,
               mm,
               ss;
        double sec;

        hh  = (int)(seconds / 3600);
        mm  = (int)((seconds - hh * 3600) / 60);
        ss  = (int)(seconds - (hh * 3600 + mm * 60));
        sec = seconds - (hh * 3600 + mm * 60 + ss);

        sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
        time_string = buffer;
        sprintf(buffer, "%lf", sec);
        time_string += &(buffer[1]); // ignore the leading zero!
        return time_string;
    }
    return string("??:??:??");
}

//############################################################################
double random_01() // returns a double between 0 & 1.
{
    return ((double)(rand()) / (double)RAND_MAX);
}

//############################################################################
double random_neg_to_pos_1() // returns a double between -1 & 1.
{
    return random_01() - random_01();
}

//############################################################################
void txt_tag(std::ofstream& out)
{
    out << "# This file was generated by " LASERBOY_VERSION " !!!\n"
           "#\n"
           "# the free, multiplatform laser display application\n"
           "# by James Lehman <james@akrobiz.com>\n"
           "# Extra Stimulus Inc., Akron, Ohio USA\n"
           "# http://laserboy.org/\n\n"

           "# ASCII format version: " LASERBOY_TXT_VERSION "\n"
        << ENDL;
    return;
}

//############################################################################
bool get_dxf_pair(std::ifstream& in, int& group_code, string& entity_string)
{
    char line[256];
    in >> group_code;
    if(group_code != -1)
    {
        while(isspace(in.peek()))
            in.get(); // eat the '\r''\n'

        in.getline(line, 255);
        entity_string = line;

        int i = entity_string.size() - 1;
        while(!isgraph(entity_string[i]))
            i--;

        entity_string = entity_string.substr(0, i + 1);
    }
    return (group_code != 0 && in.good());
}

//############################################################################
bool get_dxf_pair(std::ifstream& in, int& group_code, char entity_string[256])
{
    in >> group_code;
    if(group_code != -1)
    {
        while(isspace(in.peek()))
            in.get(); // eat the '\r''\n'
        in.getline(entity_string, 255);
    }
    return (group_code != 0 && in.good());
}

//############################################################################
bool clear_to_alpha(std::ifstream& in, int& line_number)
{
    u_int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(!isalpha(next_char))
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
            }
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is alpha
            break;
    }
    return in.good();
}

//############################################################################
bool clear_to_digit(std::ifstream& in, int& line_number)
{
    u_int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(    !isdigit(next_char)
            && next_char != '.'
            && next_char != '-'
            && next_char != '+'
          )
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
            {
                line_number++;
            }
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is digit '.' '+' or '-'
            break;
    }
    return in.good();
}

//############################################################################
bool clear_to_token(std::ifstream& in, int& line_number)
{
    while(in.good())
    {
        int next_char = in.peek();
        if(    !isalnum(next_char)
            && next_char != '.'
            && next_char != '-'
            && next_char != '+'
          )
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
                line_number++;
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is alpha, digit '.' '+' or '-'
            break;
    }
    return in.good();
}

//############################################################################
bool clear_to_token(std::ifstream& in, int& next_char, int& line_number)
{
    while(in.good())
    {
        next_char = in.peek();
        if(    !isalnum(next_char)
            && next_char != '.'
            && next_char != '-'
            && next_char != '+'
          )
        {
            in.get(); // eat it!
            if(next_char == '\n') // end of the line!
                line_number++;
            if(next_char == '#') // comment
            {
                in.ignore(99999, '\n');
                line_number++;
            }
        }
        else // next_char is alpha, digit '.' '+' or '-'
            break;
    }
    return in.good();
}

//############################################################################
bool clear_through_string(std::ifstream& in, const string& token, int& line_number)
{
    string  word;
    while(in.good() && word.substr(0, token.size()) != token)
        get_next_token(in, word, line_number);
    return in.good();
}

//############################################################################
bool get_next_token(std::ifstream& in, string& word, int& line_number)
{
    int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(!isgraph(next_char))
        {
            if(next_char == '\n') // end of the line!
                line_number++;
            in.get(); // eat it!
        }
        else // next_char isgraph
            break;
    }
    in >> word;
    return in.good();
}

//############################################################################
bool get_next_word(std::ifstream& in, string& word, int& line_number)
{
    u_int next_char = '\0';
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            if(next_char == '\n') // end of the line!
            {
                in.get(); // eat it!
                line_number++;
                return false;
            }
            in.get(); // eat it!
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(    isalpha(next_char)
        || next_char == '.'
        || next_char == '+'
        || next_char == '-'
        || next_char == '_'
        || next_char == '<'
      )
    {
        in >> word;
        return true;
    }
    return false;
}

//############################################################################
bool get_next_number(std::ifstream& in, double& number, int& line_number)
{
    bool   negate    = false;
    u_int  next_char = '\0';
    char*  nada      = {0};
    string numstr;
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            if(next_char == '\n') // end of the line!
            {
                in.get(); // eat it!
                line_number++;
                return false;
            }
            in.get(); // eat it!
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(next_char == '-')
    {
        in.get(); // eat it!
        negate = true;
        next_char = in.peek();
    }
    //------------------------------------------------------------------------
    if(    isdigit(next_char)
        || next_char == '+'
        || next_char == '.'
      )
    {
        in >> numstr;
        number = strtod(numstr.c_str(), &nada);
        if(negate)
            number = -number;
        return true;
    }
    //------------------------------------------------------------------------
    else if(isalpha(next_char))
    {
        in >> numstr;

        if     (numstr == "pi")
            number = pi;
        else if(numstr == "two_pi")
            number = two_pi;
        else if(numstr == "half_pi")
            number = half_pi;
        else if(numstr == "quarter_pi")
            number = quarter_pi;
        else if(numstr == "three_quarters_pi")
            number = three_quarters_pi;
        else if(numstr == "pi_and_a_half")
            number = pi_and_a_half;
        else if(numstr == "e")
            number = _e_;
        else if(numstr == "one_third")
            number = 1.0 / 3.0;
        else if(numstr == "two_thirds")
            number = 2.0 / 3.0;
        else if(numstr == "true")
            number = 1.0;
        else if(numstr == "yes")
            number = 1.0;
        else if(numstr == "on")
            number = 1.0;
        else if(numstr == "false")
            number = 0.0;
        else if(numstr == "no")
            number = 0.0;
        else if(numstr == "off")
            number = 0.0;
        else
            return false;

        if(negate)
            number = -number;
        return true;
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
bool get_next_hex(std::ifstream& in, double& number, int& line_number)
{
    u_int next_char = '\0';
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            if(next_char == '\n') // end of the line!
            {
                in.get(); // eat it!
                line_number++;
                return false;
            }
            in.get(); // eat it!
        }
        else // it is not a white space character
            break;
    }
    //------------------------------------------------------------------------
    if(    isxdigit(next_char)
        || next_char == '+'
        || next_char == '-'
      )
    {
        string token,
               chopped;
        char** junk = NULL;
        in >> token;
        if(token == "-1")
        {
            number = -1;
            return true;
        }
        chopped = token;
        if(    token.substr(0, 2) == "0x"
            || token.substr(0, 2) == "0X"
          )
        {
            chopped = token.substr(2);
        }
        for(size_t i = 0; i < chopped.size(); i++)
            if(!isxdigit(chopped[i]))
                return false;
        number = (int)(strtol(chopped.c_str(), junk, 16));
        return true;
    }
    //------------------------------------------------------------------------
    return false;
}

//############################################################################
bool hex_string(const string& token, double& number)
{
    string chopped = token;
    char** junk = NULL;
    if(token == "-1")
    {
        number = -1;
        return true;
    }
    if(    token.substr(0, 2) == "0x"
        || token.substr(0, 2) == "0X"
      )
    {
        chopped = token.substr(2);
    }
    for(size_t i = 0; i < chopped.size(); i++)
        if(!isxdigit(chopped[i]))
            return false;
    number = (int)(strtol(chopped.c_str(), junk, 16));
    return true;
}

//############################################################################
string as_hex_string(const char32_t& utf32)
{
    std::stringstream ss;
    ss << setfill('0') << setw(8) << std::hex << utf32;
    return ss.str();
}

//############################################################################
bool get_next_line(std::ifstream& in, string& line, int& line_number)
{
    getline(in, line);
    line_number++;
    return in.good();
}

//############################################################################
bool get_next_visible_line(std::ifstream& in, string& line, int& line_number)
{
    u_int next_char = '\0';
    //------------------------------------------------------------------------
    while(in.good())
    {
        next_char = in.peek();
        if(isspace(next_char))
        {
            if(next_char == '\n') // end of the line!
                line_number++;
            in.get(); // eat it!
        }
        else // it is not a white space character
            break;
    }
    getline(in, line);
    line_number++;
    return in.good();
}

//############################################################################
char* char_utf32_to_utf8(const char32_t& utf32, const char* buffer)
{
    char* end = const_cast<char*>(buffer);
    if(utf32 < 0x7F)
        *(end++) = static_cast<unsigned>(utf32);
    else if(utf32 < 0x7FF)
    {
        *(end++) = 0b1100'0000 + static_cast<unsigned>(utf32 >> 6);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32         & 0b0011'1111);
    }
    else if(utf32 < 0x10000)
    {
        *(end++) = 0b1110'0000 + static_cast<unsigned>( utf32 >> 12);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6 ) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>( utf32        & 0b0011'1111);
    }
    else if(utf32 < 0x110000)
    {
        *(end++) = 0b1111'0000 + static_cast<unsigned>( utf32 >> 18);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 12) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6 ) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>( utf32        & 0b0011'1111);
    }
    *end = '\0';
    return end;
}

//############################################################################
bool is_orientation(const char32_t& utf32)
{
    switch(utf32)
    {
        case 0x00000001: return true;
        case 0x00000002: return true;
        case 0x00000003: return true;

                default: return false;
    }
    return false;
}

//############################################################################
bool is_super_diacritic(const char32_t& utf32)
{
    switch(utf32)
    {
        case 0x000002c6: return true;
        case 0x000002c7: return true;
        case 0x000002c9: return true;
        case 0x000002d8: return true;
        case 0x000002d9: return true;
        case 0x000002da: return true;
        case 0x000002dc: return true;
        case 0x000002dd: return true;
        case 0x00000300: return true;
        case 0x00000301: return true;
        case 0x00000302: return true;
        case 0x00000303: return true;
        case 0x00000304: return true;
        case 0x00000305: return true;
        case 0x00000306: return true;
        case 0x00000307: return true;
        case 0x00000308: return true;
        case 0x00000309: return true;
        case 0x0000030a: return true;
        case 0x0000030b: return true;
        case 0x0000030c: return true;
        case 0x0000030d: return true;
        case 0x0000030e: return true;
        case 0x0000030f: return true;
        case 0x00000310: return true;
        case 0x00000311: return true;
        case 0x00000313: return true;
        case 0x00000314: return true;
        case 0x00000315: return true;
        case 0x0000033d: return true;
        case 0x0000033e: return true;
        case 0x00000343: return true;
        case 0x0000034a: return true;
        case 0x0000034b: return true;
        case 0x0000034c: return true;
        case 0x00000352: return true;
        case 0x0000035b: return true;
        case 0x00000385: return true;

                default: return false;
    }
    return false;
}

//############################################################################
bool is_sub_diacritic(const char32_t& utf32)
{
    switch(utf32)
    {
        case 0x000000b8: return true;
        case 0x000002db: return true;
        case 0x00000316: return true;
        case 0x00000317: return true;
        case 0x00000318: return true;
        case 0x00000319: return true;
        case 0x0000031c: return true;
        case 0x0000031d: return true;
        case 0x0000031e: return true;
        case 0x0000031f: return true;
        case 0x00000320: return true;
        case 0x00000323: return true;
        case 0x00000324: return true;
        case 0x00000325: return true;
        case 0x00000326: return true;
        case 0x00000327: return true;
        case 0x00000328: return true;
        case 0x00000329: return true;
        case 0x0000032a: return true;
        case 0x0000032b: return true;
        case 0x0000032c: return true;
        case 0x0000032d: return true;
        case 0x0000032e: return true;
        case 0x0000032f: return true;
        case 0x00000330: return true;
        case 0x00000331: return true;
        case 0x00000339: return true;
        case 0x0000033a: return true;
        case 0x0000033b: return true;
        case 0x0000033c: return true;
        case 0x00000345: return true;
        case 0x00000347: return true;
        case 0x00000348: return true;
        case 0x00000349: return true;
        case 0x00000353: return true;
        case 0x00000354: return true;
        case 0x00000355: return true;
        case 0x00000356: return true;
        case 0x00000359: return true;
        case 0x0000035a: return true;

                default: return false;
    }
    return false;
}

//############################################################################
bool is_diacritic(const char32_t& utf32)
{
   return is_super_diacritic(utf32) || is_sub_diacritic(utf32);
}

//############################################################################
bool get_utf8_index(u32string& characters, const string &font_name)
{
    wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    string         word;
    u32string      unicode;
    std::ifstream  in(font_name, ios::in); // not binary!
    //------------------------------------------------------------------------
    while(in.good())
    {
        getline(in, word);
        unicode = conv_utf8_utf32.from_bytes(word);
        characters.clear();
        for(size_t i = 0; i < unicode.size(); i++)
            if(unicode.at(i) > 0x000020)
                characters += unicode[i];
        in.close();
        return true;
    }
    //------------------------------------------------------------------------
    in.close();
    return false;
}

//############################################################################
int LaserBoy_version_check(string& version_string, string install_GUID, u_int app_runs_count)
{
    using boost::asio::ip::tcp;
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("laserboy.org", "http");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        boost::asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "POST /cgi-bin/laserboy_version?"
                       << LASERBOY_VERSION
                       << '+'
                       << install_GUID
                       << '+'
                       << app_runs_count
                       << " HTTP/1.0\r\n";
        //----------------------------------------------------------------
        request_stream << "Host: laserboy.org\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";
        //----------------------------------------------------------------
        boost::asio::write(socket, request);
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        u_int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if(!response_stream || http_version.substr(0, 5) != "HTTP/")
            return 1;
        if(status_code != 200)
            return 1;
        boost::asio::read_until(socket, response, "\r\n\r\n");
        std::string header;
        while(std::getline(response_stream, header) && header != "\r")
        {}
        std::ostringstream ostringstream_buffer;
        string version_string_buffer;
        if(response.size() > 0)
        {
            ostringstream_buffer << &response;
            version_string_buffer = ostringstream_buffer.str();
        }
        for(size_t i = 0; i < version_string_buffer.size(); i++)
            if(!strchr(" \t\r\n\0", version_string_buffer.at(i)))
                version_string += version_string_buffer.at(i);
        boost::system::error_code error;
        while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
            std::cout << &response;
        if(error != boost::asio::error::eof)
            throw boost::system::system_error(error);
    }
    catch(std::exception& e)
    {
        return 1;
    }
    return 0;
}

//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################
