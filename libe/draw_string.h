/*
* ES5932517 firmware
*
* Copyright (c) Antti Partanen 2014
*/

/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/


#ifndef _LIBE_DRAW_STRING_H_
#define _LIBE_DRAW_STRING_H_

#include <stdint.h>
#include <stdbool.h>
#include <libe/libe.h>
#include "fonts/fonts.h"

/**
 * The character printed by GrStringDraw in place of any character in the
 * string which does not appear in the font.  When using a font which does not
 * include this character, a space is left instead.
 */
#define EUI_ABSENT_CHAR_REPLACEMENT '.'

/** Indicates that the font data is stored in an uncompressed format. */
#define EUI_FONT_FMT_UNCOMPRESSED   0x00

/** Indicates that the font data is stored using a pixel-based RLE format. */
#define EUI_FONT_FMT_PIXEL_RLE      0x01

/**
 * A marker used in the format field of a font to indicates that the font
 * data is stored using the new tFontEx structure.
 */
#define EUI_FONT_EX_MARKER          0x80

/**
 * Indicates that the font data is stored in an uncompressed format and uses
 * the tFontEx structure format.
 */
#define EUI_FONT_FMT_EX_UNCOMPRESSED   (FONT_FMT_UNCOMPRESSED | FONT_EX_MARKER)

/**
 * Indicates that the font data is stored using a pixel-based RLE format and
 * uses the tFontEx structure format.
 */
#define EUI_FONT_FMT_EX_PIXEL_RLE      (FONT_FMT_PIXEL_RLE | FONT_EX_MARKER)

/**
 * This structure describes a font used for drawing text onto the screen.
 */
struct font {
	uint8_t format;         //!< The format of the font.  Can be one of FONT_FMT_UNCOMPRESSED or FONT_FMT_PIXEL_RLE.
	uint8_t maxWidth;       //!< The maximum width of a character; this is the width of the widest character in the font, though any individual character may be narrower than this width.
	uint8_t height;         //!< The height of the character cell; this may be taller than the font data for the characters (to provide inter-line spacing).
	uint8_t baseline;       //!< The offset between the top of the character cell and the baseline of  the glyph.  The baseline is the bottom row of a capital letter, below which only the descenders of the lower case letters occur.
	uint16_t offset[96];    //!< The offset within data to the data for each character in the font.
	const uint8_t *data;    //!< A pointer to the data for the font.
};

/**
 * This is a newer version of the structure which describes a font used
 * for drawing text onto the screen.  This variant allows a font to contain an
 * arbitrary, contiguous block of codepoints from the 256 basic characters in
 * an ISO8859-n font and allows support for accented characters in Western
 * European languages and any left-to-right typeface supported by an ISO8859
 * variant. Fonts encoded in this format may be used interchangeably with the
 * original fonts merely by casting the structure pointer when calling any
 * function or macro which expects a font pointer as a parameter.
 */
struct font_ex {
	uint8_t format;			//!< The format of the font.  Can be one of FONT_FMT_EX_UNCOMPRESSED or FONT_FMT_EX_PIXEL_RLE.
	uint8_t maxWidth;		//!< The maximum width of a character; this is the width of the widest character in the font, though any individual character may be narrower than this width.
	uint8_t height;			//!< The height of the character cell; this may be taller than the font data for the characters (to provide inter-line spacing).
	uint8_t baseline;		//!< The offset between the top of the character cell and the baseline of the glyph.  The baseline is the bottom row of a capital letter, below which only the descenders of the lower case letters occur.
	uint8_t first;		    //!< The codepoint number representing the first character encoded in the font.
	uint8_t last;			//!< The codepoint number representing the last character encoded in the font.
	const uint16_t *offset;	//!< A pointer to a table containing the offset within data to the data for each character in the font.
	const uint8_t *data;	//!< A pointer to the data for the font.
};

/**
 * @brief Determines the width of a string.
 *
 * This function determines the width of a string (or portion of the string)
 * when drawn with a particular font.  The \e length parameter allows a
 * portion of the string to be examined without having to insert a NULL
 * character at the stopping point (would not be possible if the string was
 * located in flash); specifying a length of -1 will cause the width of the
 * entire string to be computed.
 *
 * @param font is a pointer to the font to use.
 * @param string is the string in question.
 * @param length is the length of the string.
 * @return Returns the width of the string in pixels.
 */
int16_t eui_str_width(const struct font *font, char *string, int length);


/**
 * @brief Draws a string.
 *
 * This function draws a string of test on the screen.  The \e length
 * parameter allows a portion of the string to be examined without having to
 * insert a NULL character at the stopping point (which would not be possible
 * if the string was located in flash); specifying a length of -1 will cause
 * the entire string to be rendered (subject to clipping).
 *
 * @param context is a pointer to the drawing context to use.
 * @param string is a pointer to the string to be drawn.
 * @param length is the number of characters from the string that should be drawn on the screen.
 * @param x is the X coordinate of the upper left corner of the string position on the screen.
 * @param y is the Y coordinate of the upper left corner of the string position on the screen.
 * @param opaque is true if the background of each character should be drawn and false if it should not (leaving the background as is).
 */
void eui_str_draw(struct display *display, const struct font *font, char *string, int length, int16_t x, int16_t y, bool opaque);

/**
 * @brief Gets the baseline of a string.
 *
 * This function determines the baseline position of a string.  The baseline
 * is the offset between the top of the string and the bottom of the capital
 * letters.  The only string data that exists below the baseline are the
 * descenders on some lower-case letters (such as ``y'').
 *
 * @param font is a pointer to the font to use.
 * @return Returns the baseline of the string, in pixels.
 */
uint8_t eui_str_baseline(const struct font *font);

/**
 * @brief Draws a centered string.
 *
 * This function draws a string of test on the screen centered upon the
 * provided position.  The \e lLength parameter allows a portion of the
 * string to be examined without having to insert a NULL character at the
 * stopping point (which would not be possible if the string was located in
 * flash); specifying a length of -1 will cause the entire string to be
 * rendered (subject to clipping).
 *
 * @param context is a pointer to the drawing context to use.
 * @param string is a pointer to the string to be drawn.
 * @param length is the number of characters from the string that should be drawn on the screen.
 * @param x is the X coordinate of the center of the string position on the screen.
 * @param y is the Y coordinate of the center of the string position on the screen.
 * @param opaque is \b true if the background of each character should be drawn and \b false if it should not (leaving the background as is).
 */
// void eui_str_draw_centered(eui_context_t *context, char *string, int length, int16_t x, int16_t y, int opaque);

/**
 * @brief Gets the height of a string.
 *
 * This function determines the height of a string.  The height is the offset
 * between the top of the string and the bottom of the string, including any
 * ascenders and descenders.  Note that this will not account for the case
 * where the string in question does not have any characters that use
 * descenders but the font in the drawing context does contain characters with
 * descenders.
 *
 * @param font is a pointer to the font to use.
 * @return Returns the height of the string, in pixels.
 */
uint8_t eui_str_char_max_height(const struct font *font);

/**
 * @brief Gets the maximum width of a character in a string.
 *
 * This function determines the maximum width of a character in a string.  The
 * maximum width is the width of the widest individual character in the font
 * used to render the string, which may be wider than the widest character
 * that is used to render a particular string.
 *
 * @param font is a pointer to the font to use.
 * @return Returns the maximum width of a character in a string, in pixels.
 */
uint8_t eui_str_char_max_width(const struct font *font);


#endif /* _LIBE_DRAW_STRING_H_ */
