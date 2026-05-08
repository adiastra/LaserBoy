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
// LaserBoy.cpp is part of LaserBoy.
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

#include <cstdlib>

//############################################################################
int main(int argc, char *argv[])
{
    int xres = 640,
        yres = 480;
    if(argc >= 3)
    {
        xres = atoi(argv[1]);
        yres = atoi(argv[2]);
    }
    if(xres < 0) return -1;
    if(yres < 0) return -1;
    LaserBoy_FLTK_GUI  LaserBoy_App(xres, yres);
    return LaserBoy_App.start_menu_loop();
}


//############################################################################
//////////////////////////////////////////////////////////////////////////////
//############################################################################




/*
//    string ascii = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    string ascii = "€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
//    string ascii = "ĄĆĘŁŃŚŹŻąćęłńśźż";
//    string ascii = "Ččđ";
//    string ascii = "€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿĄĆĘŁŃŚŹŻąćęłńśźżČčđ";
//    string ascii = "ЁЂЃЄЅІЇЈЉЊЋЎЏАБВГДЕЖЗИЙКЛМНОП";
//    string ascii = "РСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмноп";
//    string ascii = "РСТХаеорсухNoёѕіїј§";
//    string ascii = "ЂЃЄЉЊЋЎЏБГДЖИЙКЛПУФЦЧШЩЪЫЬЭЮЯбвгджзийклмнп";
//    string ascii = "αßΓπΣσμτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙∙√ⁿ²ЁЂЃЄЅІЇЈЉЊЋЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяNoёђѓєѕіїјљњћќ§ўџ";
//    string ascii = "αßΓπΣσμτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙∙√ⁿ²ЁЂЃЄЅІЇЈЉЊЋЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяNoёђѓєѕіїјљњћќ§ўџ";

//    string ascii = "αΓπΣσμτΦΘΩδ∞φε∩≡≥≤⌠⌡÷≈√ⁿЂЃЄЉЊЋЎЏБГДЕЖИЙКЛПУФЦЧШЩЪЫЬЭЮЯбвгджзийклмноптфцчшщъыьэюяђѓєљњћќўџ";
    string ascii = "# ß±°∙∙²ЁЅІЇЈАВЕЗМНОРСТХаеорсухNoёѕіїј§";
    wstring_convert<codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;
    char buffer[5];
    buffer[4] = '\0';
    u32string unicode = conv_utf8_utf32.from_bytes(ascii);
    cout << endl << endl;
    cout << ascii;
    cout << endl << endl;
    for(size_t i = 0; i < unicode.size(); i++)
    {
        char_utf32_to_utf8(unicode[i], buffer);
        cout << "case " << setw(5) << unicode[i] << ": return " << setw(5) << i + 234 << ";  //  " << buffer << endl;
    }
    cout << endl << endl;
*/
