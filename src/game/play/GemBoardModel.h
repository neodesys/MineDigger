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

#ifndef _GEMBOARDMODEL_H_
#define	_GEMBOARDMODEL_H_

namespace game
{
	namespace play
	{
		enum GemType : unsigned char
		{
			BLUE_GEM = 0,
			GREEN_GEM,
			PURPLE_GEM,
			RED_GEM,
			YELLOW_GEM,
			NB_GEM_TYPES,
			NO_GEM = NB_GEM_TYPES
		};

		class GemBoardView;

		class GemBoardModel final
		{
		public:
			GemBoardModel(GemBoardView& view) : m_view(view)
			{
				clearBoard();
			}

			static constexpr int NB_ROWS = 8;
			static constexpr int NB_COLS = 8;
			static constexpr int COLLAPSE_SIZE = 3;

			GemType getGem(int row, int col) const
			{
				return m_board[row][col];
			}

			void resetBoard(bool bFillBoard);
			void swapGems(int rowA, int colA, int rowB, int colB);
			void tryCollapse(int row, int col);

		private:
			GemBoardModel(const GemBoardModel&) = delete;
			GemBoardModel& operator=(const GemBoardModel&) = delete;

			GemBoardView& m_view;

			GemType m_board[NB_ROWS][NB_COLS];

			void clearBoard();
			GemType generateGem(int row, int col) const;

			bool getHCollapseBounds(int row, int& minCol, int& maxCol, GemType type) const;
			bool getVCollapseBounds(int col, int& minRow, int& maxRow, GemType type) const;

			void executeVCollapsing(int col, int minRow, int maxRow);
		};
	}
}

#endif //_GEMBOARDMODEL_H_
