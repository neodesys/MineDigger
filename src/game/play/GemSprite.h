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

#ifndef _GEMSPRITE_H_
#define	_GEMSPRITE_H_

#include "../../app/DynSprite.h"
#include "GemBoardModel.h"

namespace game
{
	namespace play
	{
		class GemSprite final : public app::DynSprite
		{
		public:
			struct Config
			{
				float selectedSpriteScale;

				float minMoveOffset;
				float maxMoveOffset;

				float initRowSpacingFactor;

				float thrownOutImpulseCoeff;
				float gravityCoeff;

				float anchorSpringConstant;
				float anchorSpringDampingCoeff;
			};

			GemSprite() = default;
			void attachView(GemBoardView* pView);

			enum struct State : unsigned char
			{
				RECYCLED = 0,
				FALLING,
				IN_PLACE,
				SPRING_ATTACHED,
				THROWN_OUT
			};

			State getState() const
			{
				return m_state;
			}

			int getModelRow() const
			{
				return m_modelRow;
			}

			int getModelCol() const
			{
				return m_modelCol;
			}

			const sys::Vec2& getPos() const
			{
				return m_pos;
			}

			const sys::Vec2& getTargetPos() const
			{
				return m_targetPos;
			}

			void setPos(const sys::Vec2& pos);
			bool select(const sys::Vec2& mousePos);
			void unselect();

			void initGem(GemType type, int row, int col);
			bool trySwapGem(int row, int col);
			void cancelMove();
			void validateMove(int row, int col);
			void fallGem(int lowerRow);
			void throwGemOut();

			void updatePos(float dtCoeff, float dt2) override final;
			void draw(sys::Renderer& rdr) override final;

		private:
			GemSprite(const GemSprite&) = delete;
			GemSprite& operator=(const GemSprite&) = delete;

			void interruptSwap();

			//Forbid access to base methods
			void setAcceleration(const sys::Vec2& v);
			void setImpulse(const sys::Vec2& v);
			void setScale(float scale);
			void setTexture(const sys::Texture* pTexture);
			void setTexture(const sys::Texture* pTexture, const sys::Rect& clip);

			GemBoardView* m_pView = nullptr;

			State m_state = State::RECYCLED;

			int m_modelRow = -1;
			int m_modelCol = -1;

			int m_swapModelRow = -1;
			int m_swapModelCol = -1;

			bool m_bSelected = false;
			sys::Vec2 m_selectMouseOffset;

			sys::Vec2 m_targetPos;
		};
	}
}

#endif //_GEMSPRITE_H_
