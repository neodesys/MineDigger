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
#define	_TEXTUREDRAWER_H_

#include "ISpriteDrawer.h"
#include "../sys/sys.h"

namespace sys
{
	class Texture;
}

namespace app
{
	class TextureDrawer : public ISpriteDrawer
	{
	public:
		void setTexture(const sys::Texture* pTexture);
		void setTexture(const sys::Texture* pTexture, const sys::Rect& clip);

		virtual bool getSpriteSize(float scale, int& w, int& h) const override;
		virtual void drawSprite(sys::Renderer& rdr, const sys::Rect& rect) const override;

	protected:
		const sys::Texture* m_pTexture = nullptr;
		bool m_bSrcClip = false;
		sys::Rect m_srcClip;
	};
}

#endif //_TEXTUREDRAWER_H_
