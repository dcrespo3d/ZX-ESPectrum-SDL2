///////////////////////////////////////////////////////////////////////////////
//
// ZX-ESPectrum-IDF - Sinclair ZX Spectrum emulator for ESP32 / IDF
//
// PHYSICAL MEMBRANE KEYBOARD READ
//
// Copyright (c) 2023 Víctor Iborra [Eremus] and David Crespo [dcrespo3d]
// https://github.com/EremusOne/ZX-ESPectrum-IDF
//
// Based on ZX-ESPectrum-Wiimote
// Copyright (c) 2020, 2022 David Crespo [dcrespo3d]
// https://github.com/dcrespo3d/ZX-ESPectrum-Wiimote
//
// Based on previous work by Ramón Martinez and Jorge Fuertes
// https://github.com/rampa069/ZX-ESPectrum
//
// Original project by Pete Todd
// https://github.com/retrogubbins/paseVGA
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef ZXKEYB_h
#define ZXKEYB_h

#include <inttypes.h>

class ZXKeyb
{
public:

    // setup pins for physical keyboard
    static void setup();

    // process physical keyboard
    static void process();

    static uint8_t ZXcols[8];
    
    static uint8_t PrevFkeyOSD;

private:
    static void putRows(uint8_t row_pattern);
    static uint8_t getCols();
};

#endif // ZXKEYB_h
