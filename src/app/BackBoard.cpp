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

#include "BackBoard.h"

#include "../sys/Renderer.h"

namespace app
{
	void BackBoard::draw(sys::Renderer& rdr)
	{
		rdr.clearBackground(&m_backColor);

		if (m_pTexture)
			rdr.drawTexture(*m_pTexture, nullptr, nullptr);
	}
}
