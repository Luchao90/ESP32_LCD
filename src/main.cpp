/*

  ScrollingText.ino

  This will scroll text on the display. 
  Enable U8g2 16 bit mode (see FAQ) for larger text!

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U82glib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.
  
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18 /* A4 */ , /* data=*/ 16 /* A2 */, /* CS=*/ 17 /* A3 */, /* reset=*/ U8X8_PIN_NONE);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 14, /* data=*/ 13, /* CS=*/ 15, /* reset=*/ 16); // Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS
//U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);
//U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 15, /* reset=*/ 16); // Feather HUZZAH ESP8266, E=clock=14, RW=data=13, RS=CS

// End of constructor list

// This example shows a scrolling text.
// If U8G2_16BIT is not set (default), then the pixel width of the text must be lesser than 128
// If U8G2_16BIT is set, then the pixel width an be up to 32000

#define STP7920_CS_PIN 5
#define STP7920_RESET_PIN 19

U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/STP7920_CS_PIN, /* reset= */ STP7920_RESET_PIN);
//U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 18, /* data=*/ 23, /* CS=*/ STP7920_CS_PIN, /* reset=*/ U8X8_PIN_NONE);

u8g2_uint_t offset;            // current offset for the scrolling text
u8g2_uint_t width;             // pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "Novotec "; // scroll this text from right to left

void setup(void)
{
  pinMode(STP7920_CS_PIN, OUTPUT);
  digitalWrite(STP7920_CS_PIN, 0);
  pinMode(STP7920_CS_PIN, OUTPUT);
  digitalWrite(STP7920_CS_PIN, 0);
  pinMode(STP7920_RESET_PIN, OUTPUT);
  digitalWrite(STP7920_RESET_PIN, 0);
  pinMode(STP7920_RESET_PIN, OUTPUT);
  digitalWrite(STP7920_RESET_PIN, 0);

  u8g2.begin();

  u8g2.setFont(u8g2_font_inb30_mr); // set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);  // calculate the pixel width of the text

  u8g2.setFontMode(false); // enable transparent mode, which is faster
}

void loop(void)
{
  u8g2_uint_t x;

  u8g2.firstPage();
  do
  {

    // draw the scrolling text at current offset
    x = offset;
    u8g2.setFont(u8g2_font_inb30_mr); // set the target font
    do
    {                                     // repeated drawing of the scrolling text...
      u8g2.drawUTF8(x, 30, text);         // draw the scolling text
      x += width;                         // add the pixel width of the scrolling text
    } while (x < u8g2.getDisplayWidth()); // draw again until the complete display is filled

    u8g2.setFont(u8g2_font_inb16_mr); // draw the current pixel width
    u8g2.setCursor(0, 58);
    u8g2.print(width); // this value must be lesser than 128 unless U8G2_16BIT is set

  } while (u8g2.nextPage());

  offset -= 1; // scroll by one pixel
  if ((u8g2_uint_t)offset < (u8g2_uint_t)-width)
    offset = 0; // start over again

  delay(100); // do some small delay
}