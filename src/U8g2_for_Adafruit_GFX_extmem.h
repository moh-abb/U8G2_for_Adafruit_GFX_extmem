/*

  U8g2_for_Adafruit_GFX_extmem.h

  Add unicode support and U8g2 fonts to Adafruit GFX libraries.

  U8g2 for Adafruit GFX Lib (https://github.com/olikraus/U8g2_for_Adafruit_GFX)

  Library modified by RobotGB (https://github.com/RobotGB/U8G2_for_Adafruit_GFX_extmem)

  Copyright (c) 2018, olikraus@gmail.com
  All rights reserved.
  Copyright (c) 2019, RobotGB (robot_m0@outlook.com)
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

//==========================================//
// uncomment the value that you WANT to use //
// and uncomment all other values           //
//==========================================//

#define USE_SPIFFSMEM
//#define USE_SDMEM

//============================================

#ifdef USE_SPIFFSMEM
  #include <FS.h>
  #define MEM fs
#endif

#ifdef USE_SDMEM
  #include <SD.h>
  #define MEM SD
#endif

#ifndef U8g2_for_Adafruit_GFX_extmem_h
#define U8g2_for_Adafruit_GFX_extmem_h

#include <Adafruit_GFX.h>

// #include "u8g2_fonts.h"


//========================================================



struct _u8g2_font_info_t
{
  /* offset 0 */
  uint8_t glyph_cnt;
  uint8_t bbx_mode;
  uint8_t bits_per_0;
  uint8_t bits_per_1;

  /* offset 4 */
  uint8_t bits_per_char_width;
  uint8_t bits_per_char_height;
  uint8_t bits_per_char_x;
  uint8_t bits_per_char_y;
  uint8_t bits_per_delta_x;

  /* offset 9 */
  int8_t max_char_width;
  int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
  int8_t x_offset;
  int8_t y_offset;

  /* offset 13 */
  int8_t  ascent_A;
  int8_t  descent_g;  /* usually a negative value */
  int8_t  ascent_para;
  int8_t  descent_para;

  /* offset 17 */
  uint16_t start_pos_upper_A;
  uint16_t start_pos_lower_a;

  /* offset 21 */
  uint16_t start_pos_unicode;
};
typedef struct _u8g2_font_info_t u8g2_font_info_t;

struct _u8g2_font_decode_t
{
  // const uint8_t *decode_ptr;      /* pointer to the compressed data */
  //^ this is no longer used, instead a combination of font and position locator is used
  MEM::File font;
  uint32_t decode_ptr;

  int16_t target_x;
  int16_t target_y;
  uint16_t fg_color;
  uint16_t bg_color;

  int8_t x;           /* local coordinates, (0,0) is upper left */
  int8_t y;
  int8_t glyph_width;
  int8_t glyph_height;

  uint8_t decode_bit_pos;     /* bitpos inside a byte of the compressed data */
  uint8_t is_transparent;
  uint8_t dir;        /* direction */
};
typedef struct _u8g2_font_decode_t u8g2_font_decode_t;


struct _u8g2_font_t
{
  Adafruit_GFX *gfx;

  // /* information about the current font */
  // const uint8_t *font;             /* current font for all text procedures */
  MEM::File font; //replaced pointer with file
  // ^ some functions require _u8g2_font_t, others require _u8g2_font_decode_t
  // so I added the font file on both and set them equal

  //u8g2_font_calc_vref_fnptr font_calc_vref;
  u8g2_font_decode_t font_decode;   /* new font decode structure */
  u8g2_font_info_t font_info;     /* new font info structure */

  //uint8_t font_height_mode;
  //int8_t font_ref_ascent;
  //int8_t font_ref_descent;

  int8_t glyph_x_offset;    /* set by u8g2_GetGlyphWidth as a side effect */
};
typedef struct _u8g2_font_t u8g2_font_t;


uint8_t u8g2_IsGlyph(u8g2_font_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_font_t *u8g2, uint16_t requested_encoding);
void u8g2_SetFontMode(u8g2_font_t *u8g2, uint8_t is_transparent);
void u8g2_SetFontDirection(u8g2_font_t *u8g2, uint8_t dir);
int16_t u8g2_DrawGlyph(u8g2_font_t *u8g2, int16_t x, int16_t y, uint16_t encoding);
int16_t u8g2_DrawStr(u8g2_font_t *u8g2, int16_t x, int16_t y, const char *s);
void u8g2_SetFont(u8g2_font_t *u8g2, MEM::File font);
void u8g2_SetForegroundColor(u8g2_font_t *u8g2, uint16_t fg);
void u8g2_SetBackgroundColor(u8g2_font_t *u8g2, uint16_t bg);


class U8G2_FOR_ADAFRUIT_GFX_EXTMEM : public Print {
  public:
    u8g2_font_t u8g2;
    int16_t tx, ty;       // current position for the print command
    uint16_t encoding;    // the unicode, detected by the utf-8 decoder
    uint8_t utf8_state;   // current state of the utf-8 decoder, contains the remaining bytes for a detected unicode glyph

    void home(void)
      { tx = 0; ty = 0;  utf8_state = 0; }
    void setCursor(int16_t x, int16_t y)
      { tx = x; ty = y; utf8_state = 0;  }
    int16_t getCursorX(void) { return tx; }
    int16_t getCursorY(void) { return ty; }

    U8G2_FOR_ADAFRUIT_GFX_EXTMEM(void) {u8g2.font_decode.fg_color = 1; u8g2.font_decode.is_transparent = 1; u8g2.font_decode.dir = 0; home(); }
    void begin(Adafruit_GFX &gfx) { u8g2.gfx = &gfx; }
    void setFontFile(MEM::File font)             // set u8g2 font
      { u8g2_SetFont(&u8g2, font); }
    void setFontMode(uint8_t is_transparent)      // is_transparent==0: Background is not drawn
      { u8g2_SetFontMode(&u8g2, is_transparent); }
    void setFontDirection(uint8_t d)              // 0; 0 degree, 1: 90 degree, 2: 180 degree, 3: 270 degree
      { u8g2_SetFontDirection(&u8g2, d); }
    void setForegroundColor(uint16_t fg)           // Use this color to draw the text
      { u8g2_SetForegroundColor(&u8g2, fg); }
    void setBackgroundColor(uint16_t bg)           // only used for setFontMode(0)
      { u8g2_SetBackgroundColor(&u8g2, bg); }
    int8_t getFontAscent(void)
      { return u8g2.font_info.ascent_A; }
    int8_t getFontDescent(void)
      { return u8g2.font_info.descent_g; }
    int16_t drawGlyph(int16_t x, int16_t y, uint16_t e)
      { return u8g2_DrawGlyph(&u8g2, x, y, e); }           // draw a signle char (e == Unicode)
    int16_t drawStr(int16_t x, int16_t y, const char *s)
      { return u8g2_DrawStr(&u8g2, x, y, s); }

    int16_t drawUTF8(int16_t x, int16_t y, const char *str);
    int16_t getUTF8Width(const char *str);


    uint16_t utf8_next(uint8_t b);

    size_t write(uint8_t v) {
      uint16_t e = utf8_next(v);
      int16_t delta;

      if ( e == '\n' )
      {
	tx = 0;
	ty += u8g2.font_info.ascent_para - u8g2.font_info.descent_para;
      }
      else if ( e == '\r' )
      {
	tx = 0;
      }
      else if ( e < 0x0fffe )
      {
        delta = drawGlyph(tx, ty, e);
        switch(u8g2.font_decode.dir)
        {
          case 0:
            tx += delta;
            break;
          case 1:
            ty += delta;
            break;
          case 2:
            tx -= delta;
            break;
          case 3:
            ty -= delta;
            break;
        }
      }
      return 1;
     }

    size_t write(const uint8_t *buffer, size_t size) {
      size_t cnt = 0;
      while( size > 0 ) {
          cnt += write(*buffer++);
          size--;
      }
      return cnt;
    }
};


/*==========================================*/
//
/* start font list */
// removed font list because any fonts are stored in external memory
/* end font list */

#endif
