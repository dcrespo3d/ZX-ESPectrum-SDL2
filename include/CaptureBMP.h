///////////////////////////////////////////////////////////////////////////////
//
// ZX-ESPectrum-IDF - Sinclair ZX Spectrum emulator for ESP32 / IDF
//
// BMP CAPTURE
//
// Copyright (c) 2023 Víctor Iborra [Eremus] and David Crespo [dcrespo3d]
// https://github.com/EremusOne/ZX-ESPectrum-IDF
//
// Based on ZX-ESPectrum-Wiimote
// Copyright (c) 2020, 2021 David Crespo [dcrespo3d]
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

#ifndef CaptureBMP_h
#define CaptureBMP_h

// Reference
//
// BITMAPFILEHEADER: 14 bytes
// typedef struct tagBITMAPFILEHEADER {
//   WORD  bfType;
//   DWORD bfSize;
//   WORD  bfReserved1;
//   WORD  bfReserved2;
//   DWORD bfOffBits;
// }
//
// BITMAPINFOHEADER: 40 bytes
// typedef struct tagBITMAPINFOHEADER {
//   DWORD biSize;
//   LONG  biWidth;
//   LONG  biHeight;
//   WORD  biPlanes;
//   WORD  biBitCount;
//   DWORD biCompression;
//   DWORD biSizeImage;
//   LONG  biXPelsPerMeter;
//   LONG  biYPelsPerMeter;
//   DWORD biClrUsed;
//   DWORD biClrImportant;
// }

// custom header for 8 bit indexed BMP with adapted palette
// which requires no translation between framebuffer values and indices
#define BMP_HEADER1_SIZE 18
#define BMP_HEADER2_SIZE 20
#define BMP_HEADER3_SIZE 1040

const unsigned char bmp_header1[BMP_HEADER1_SIZE] = {
    0x42,0x4d,0x36,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x04,0x00,0x00,0x28,0x00,
    0x00,0x00 };

const unsigned char bmp_header3[BMP_HEADER3_SIZE] = { 
    0x23,0x2e,0x00,0x00,0x23,0x2e,0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,
    0xbf,0x00,0x00,0x00,0xff,0x00,0x00,0x7f,0x00,0x00,0x00,0x7f,0x7f,0x00,0x00,0x7f,
    0xbf,0x00,0x00,0x7f,0xff,0x00,0x00,0xbf,0x00,0x00,0x00,0xbf,0x7f,0x00,0x00,0xbf,
    0xbf,0x00,0x00,0xbf,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x7f,0x00,0x00,0xff,
    0xbf,0x00,0x00,0xff,0xff,0x00,0x7f,0x00,0x00,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,
    0xbf,0x00,0x7f,0x00,0xff,0x00,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x7f,0x7f,
    0xbf,0x00,0x7f,0x7f,0xff,0x00,0x7f,0xbf,0x00,0x00,0x7f,0xbf,0x7f,0x00,0x7f,0xbf,
    0xbf,0x00,0x7f,0xbf,0xff,0x00,0x7f,0xff,0x00,0x00,0x7f,0xff,0x7f,0x00,0x7f,0xff,
    0xbf,0x00,0x7f,0xff,0xff,0x00,0xbf,0x00,0x00,0x00,0xbf,0x00,0x7f,0x00,0xbf,0x00,
    0xbf,0x00,0xbf,0x00,0xff,0x00,0xbf,0x7f,0x00,0x00,0xbf,0x7f,0x7f,0x00,0xbf,0x7f,
    0xbf,0x00,0xbf,0x7f,0xff,0x00,0xbf,0xbf,0x00,0x00,0xbf,0xbf,0x7f,0x00,0xbf,0xbf,
    0xbf,0x00,0xbf,0xbf,0xff,0x00,0xbf,0xff,0x00,0x00,0xbf,0xff,0x7f,0x00,0xbf,0xff,
    0xbf,0x00,0xbf,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x7f,0x00,0xff,0x00,
    0xbf,0x00,0xff,0x00,0xff,0x00,0xff,0x7f,0x00,0x00,0xff,0x7f,0x7f,0x00,0xff,0x7f,
    0xbf,0x00,0xff,0x7f,0xff,0x00,0xff,0xbf,0x00,0x00,0xff,0xbf,0x7f,0x00,0xff,0xbf,
    0xbf,0x00,0xff,0xbf,0xff,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x7f,0x00,0xff,0xff,
    0xbf,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,
    0xbf,0x00,0x00,0x00,0xff,0x00,0x00,0x7f,0x00,0x00,0x00,0x7f,0x7f,0x00,0x00,0x7f,
    0xbf,0x00,0x00,0x7f,0xff,0x00,0x00,0xbf,0x00,0x00,0x00,0xbf,0x7f,0x00,0x00,0xbf,
    0xbf,0x00,0x00,0xbf,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x7f,0x00,0x00,0xff,
    0xbf,0x00,0x00,0xff,0xff,0x00,0x7f,0x00,0x00,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,
    0xbf,0x00,0x7f,0x00,0xff,0x00,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x7f,0x7f,
    0xbf,0x00,0x7f,0x7f,0xff,0x00,0x7f,0xbf,0x00,0x00,0x7f,0xbf,0x7f,0x00,0x7f,0xbf,
    0xbf,0x00,0x7f,0xbf,0xff,0x00,0x7f,0xff,0x00,0x00,0x7f,0xff,0x7f,0x00,0x7f,0xff,
    0xbf,0x00,0x7f,0xff,0xff,0x00,0xbf,0x00,0x00,0x00,0xbf,0x00,0x7f,0x00,0xbf,0x00,
    0xbf,0x00,0xbf,0x00,0xff,0x00,0xbf,0x7f,0x00,0x00,0xbf,0x7f,0x7f,0x00,0xbf,0x7f,
    0xbf,0x00,0xbf,0x7f,0xff,0x00,0xbf,0xbf,0x00,0x00,0xbf,0xbf,0x7f,0x00,0xbf,0xbf,
    0xbf,0x00,0xbf,0xbf,0xff,0x00,0xbf,0xff,0x00,0x00,0xbf,0xff,0x7f,0x00,0xbf,0xff,
    0xbf,0x00,0xbf,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x7f,0x00,0xff,0x00,
    0xbf,0x00,0xff,0x00,0xff,0x00,0xff,0x7f,0x00,0x00,0xff,0x7f,0x7f,0x00,0xff,0x7f,
    0xbf,0x00,0xff,0x7f,0xff,0x00,0xff,0xbf,0x00,0x00,0xff,0xbf,0x7f,0x00,0xff,0xbf,
    0xbf,0x00,0xff,0xbf,0xff,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x7f,0x00,0xff,0xff,
    0xbf,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,
    0xbf,0x00,0x00,0x00,0xff,0x00,0x00,0x7f,0x00,0x00,0x00,0x7f,0x7f,0x00,0x00,0x7f,
    0xbf,0x00,0x00,0x7f,0xff,0x00,0x00,0xbf,0x00,0x00,0x00,0xbf,0x7f,0x00,0x00,0xbf,
    0xbf,0x00,0x00,0xbf,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x7f,0x00,0x00,0xff,
    0xbf,0x00,0x00,0xff,0xff,0x00,0x7f,0x00,0x00,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,
    0xbf,0x00,0x7f,0x00,0xff,0x00,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x7f,0x7f,
    0xbf,0x00,0x7f,0x7f,0xff,0x00,0x7f,0xbf,0x00,0x00,0x7f,0xbf,0x7f,0x00,0x7f,0xbf,
    0xbf,0x00,0x7f,0xbf,0xff,0x00,0x7f,0xff,0x00,0x00,0x7f,0xff,0x7f,0x00,0x7f,0xff,
    0xbf,0x00,0x7f,0xff,0xff,0x00,0xbf,0x00,0x00,0x00,0xbf,0x00,0x7f,0x00,0xbf,0x00,
    0xbf,0x00,0xbf,0x00,0xff,0x00,0xbf,0x7f,0x00,0x00,0xbf,0x7f,0x7f,0x00,0xbf,0x7f,
    0xbf,0x00,0xbf,0x7f,0xff,0x00,0xbf,0xbf,0x00,0x00,0xbf,0xbf,0x7f,0x00,0xbf,0xbf,
    0xbf,0x00,0xbf,0xbf,0xff,0x00,0xbf,0xff,0x00,0x00,0xbf,0xff,0x7f,0x00,0xbf,0xff,
    0xbf,0x00,0xbf,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x7f,0x00,0xff,0x00,
    0xbf,0x00,0xff,0x00,0xff,0x00,0xff,0x7f,0x00,0x00,0xff,0x7f,0x7f,0x00,0xff,0x7f,
    0xbf,0x00,0xff,0x7f,0xff,0x00,0xff,0xbf,0x00,0x00,0xff,0xbf,0x7f,0x00,0xff,0xbf,
    0xbf,0x00,0xff,0xbf,0xff,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x7f,0x00,0xff,0xff,
    0xbf,0x00,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,
    0xbf,0x00,0x00,0x00,0xff,0x00,0x00,0x7f,0x00,0x00,0x00,0x7f,0x7f,0x00,0x00,0x7f,
    0xbf,0x00,0x00,0x7f,0xff,0x00,0x00,0xbf,0x00,0x00,0x00,0xbf,0x7f,0x00,0x00,0xbf,
    0xbf,0x00,0x00,0xbf,0xff,0x00,0x00,0xff,0x00,0x00,0x00,0xff,0x7f,0x00,0x00,0xff,
    0xbf,0x00,0x00,0xff,0xff,0x00,0x7f,0x00,0x00,0x00,0x7f,0x00,0x7f,0x00,0x7f,0x00,
    0xbf,0x00,0x7f,0x00,0xff,0x00,0x7f,0x7f,0x00,0x00,0x7f,0x7f,0x7f,0x00,0x7f,0x7f,
    0xbf,0x00,0x7f,0x7f,0xff,0x00,0x7f,0xbf,0x00,0x00,0x7f,0xbf,0x7f,0x00,0x7f,0xbf,
    0xbf,0x00,0x7f,0xbf,0xff,0x00,0x7f,0xff,0x00,0x00,0x7f,0xff,0x7f,0x00,0x7f,0xff,
    0xbf,0x00,0x7f,0xff,0xff,0x00,0xbf,0x00,0x00,0x00,0xbf,0x00,0x7f,0x00,0xbf,0x00,
    0xbf,0x00,0xbf,0x00,0xff,0x00,0xbf,0x7f,0x00,0x00,0xbf,0x7f,0x7f,0x00,0xbf,0x7f,
    0xbf,0x00,0xbf,0x7f,0xff,0x00,0xbf,0xbf,0x00,0x00,0xbf,0xbf,0x7f,0x00,0xbf,0xbf,
    0xbf,0x00,0xbf,0xbf,0xff,0x00,0xbf,0xff,0x00,0x00,0xbf,0xff,0x7f,0x00,0xbf,0xff,
    0xbf,0x00,0xbf,0xff,0xff,0x00,0xff,0x00,0x00,0x00,0xff,0x00,0x7f,0x00,0xff,0x00,
    0xbf,0x00,0xff,0x00,0xff,0x00,0xff,0x7f,0x00,0x00,0xff,0x7f,0x7f,0x00,0xff,0x7f,
    0xbf,0x00,0xff,0x7f,0xff,0x00,0xff,0xbf,0x00,0x00,0xff,0xbf,0x7f,0x00,0xff,0xbf,
    0xbf,0x00,0xff,0xbf,0xff,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x7f,0x00,0xff,0xff,
    0xbf,0x00,0xff,0xff,0xff,0x00
};

void CaptureToBmp();

#endif