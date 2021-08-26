/**
 * MineDigger
 *
 * Copyright (C) 2015, Loïc Le Page
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _FONT_H_
#define _FONT_H_

#include "Logger.h"

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

namespace sys
{
    class GameEngine;
    class Texture;
    class Rect;

    class Font final
    {
      public:
        static Font* loadFont(const GameEngine& engine, const char* asset, int pointSize = 12, long fontFaceIdx = 0);
        ~Font();

        const int m_pointSize;

        bool isFontMonospace() const;

        int getFontHeight() const;
        int getFontAscent() const;
        int getFontDescent() const;

        int getFontLineSkip() const;

        enum struct Hinting : unsigned char
        {
            NORMAL,
            LIGHT,
            MONO,
            NONE
        };

        void setFontHinting(Hinting hinting);
        Hinting getFontHinting() const;

        enum : int
        {
            STYLE_NORMAL = 0x00,
            STYLE_BOLD = 0x01,
            STYLE_ITALIC = 0x02,
            STYLE_UNDERLINE = 0x04,
            STYLE_STRIKETHROUGH = 0x08
        };

        void setFontStyle(int style);
        int getFontStyle() const;

        void setFontOutline(int outline);
        int getFontOutline() const;

        void enableKerning(bool bEnable);
        bool isKerningEnabled() const;

        // WARNING: glyph metrics are returned with a bottom-to-top y axis
        bool getGlyphMetrics(char16_t glyph, Rect* pRect, int* pAdvance) const;

        // txt must be encoded in UTF-8
        bool getTextSize(const char* txt, int& w, int& h) const;
        Texture* createTextTexture(const GameEngine& engine, const char* txt) const;

      private:
        Font(int pointSize, TTF_Font* pSDLFont) : m_pointSize(pointSize), m_pSDLFont(pSDLFont)
        {
        }
        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        TTF_Font* const m_pSDLFont;

        static const Logger s_log;
    };
} // namespace sys

#endif // _FONT_H_
