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

#ifndef _TEXTUREDRAWER_H_
#define _TEXTUREDRAWER_H_

#include "../sys/Rect.h"
#include "ISpriteDrawer.h"

namespace sys
{
    class Texture;
}

namespace app
{
    class TextureDrawer : public ISpriteDrawer
    {
      public:
        // Setting the texture resets any sub-images configuration
        void setTexture(const sys::Texture* pTexture, const sys::Rect* pClip = nullptr);

        // If (uCount < 2) or (uStride == 0), the sub-images are disabled and
        // the method always returns true.
        // uStride is the number of sub-images per line in the attached texture
        // within its source clipping bounds.
        // A texture must be attached BEFORE calling configureSubImages(), else
        // the method fails and returns false.
        bool configureSubImages(unsigned int uCount, unsigned int uStride);

        unsigned int getSubImagesCount() const
        {
            return m_uSubImagesCount;
        }

        // Returns false if uIndex is out of bounds or if sub-images have not
        // been correctly configured
        bool selectSubImage(unsigned int uIndex);

        unsigned int getSelectedSubImage() const
        {
            return m_uSelectedSubImage;
        }

        virtual bool getSpriteSize(float scale, int& w, int& h) const override;
        virtual void drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const override;

      protected:
        const sys::Texture* m_pTexture = nullptr;
        bool m_bSrcClip = false;
        sys::Rect m_srcClip;

        unsigned int m_uSubImagesCount = 0;
        unsigned int m_uSubImagesStride = 0;
        unsigned int m_uSelectedSubImage = 0;
        sys::Rect m_subImageClip;
    };
} // namespace app

#endif // _TEXTUREDRAWER_H_
