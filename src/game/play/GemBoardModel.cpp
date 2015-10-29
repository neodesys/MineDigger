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

#include "GemBoardView.h"

#include <cassert>
#include <random>

namespace game
{
	namespace play
	{
		void GemBoardModel::resetBoard(bool bFillBoard)
		{
			if (bFillBoard)
			{
				for (int row = 0; row < NB_ROWS; ++row)
				{
					for (int col = 0; col < NB_COLS; ++col)
						m_board[row][col] = generateGem(row, col);
				}
			}
			else
				clearBoard();

			m_view.onBoardReset();
		}

		void GemBoardModel::swapGems(int rowA, int colA, int rowB, int colB)
		{
			if ((rowA >= 0) && (rowA < NB_ROWS) &&
				(colA >= 0) && (colA < NB_COLS) &&
				(rowB >= 0) && (rowB < NB_ROWS) &&
				(colB >= 0) && (colB < NB_COLS))
			{
				if (rowA == rowB)
				{
					//Horizontal swap
					if (std::abs(colB - colA) != 1)
					{
						//We can only swap contiguous gems
						m_view.onCancelSwap(rowA, colA, rowB, colB);
						return;
					}
				}
				else if (colA == colB)
				{
					//Vertical swap
					if (std::abs(rowB - rowA) != 1)
					{
						//We can only swap contiguous gems
						m_view.onCancelSwap(rowA, colA, rowB, colB);
						return;
					}
				}
				else
				{
					//We cannot swap diagonal gems
					m_view.onCancelSwap(rowA, colA, rowB, colB);
					return;
				}

				GemType typeA = m_board[rowA][colA];
				GemType typeB = m_board[rowB][colB];
				if ((typeA == GemType::NO_GEM) || (typeB == GemType::NO_GEM) || (typeA == typeB))
				{
					//We can only swap different valid gems
					m_view.onCancelSwap(rowA, colA, rowB, colB);
					return;
				}

				//Perform the swap itself
				m_board[rowB][colB] = typeA;
				m_board[rowA][colA] = typeB;

				int minRow = rowB;
				int maxRow = rowB;
				int minCol = colB;
				int maxCol = colB;
				if (!getHCollapseBounds(rowB, minCol, maxCol, typeA) &&
					!getVCollapseBounds(colB, minRow, maxRow, typeA))
				{
					//First gem doesn't collapse, let's try the second one
					minRow = maxRow = rowA;
					minCol = maxCol = colA;
					if (!getHCollapseBounds(rowA, minCol, maxCol, typeB) &&
						!getVCollapseBounds(colA, minRow, maxRow, typeB))
					{
						//Second gem doesn't collapse neither, swap has
						//failed. Let's restore model state
						m_board[rowB][colB] = typeB;
						m_board[rowA][colA] = typeA;

						m_view.onCancelSwap(rowA, colA, rowB, colB);
						return;
					}
				}

				//At least one gem produces a collapsing, swap has succeeded
				m_view.onValidateSwap(rowA, colA, rowB, colB);
			}
		}

		void GemBoardModel::tryCollapse(int row, int col)
		{
			if ((row >= 0) && (row < NB_ROWS) &&
				(col >= 0) && (col < NB_COLS))
			{
				GemType type = m_board[row][col];
				if (type != GemType::NO_GEM)
				{
					//Verify horizontal collapsing
					bool bCollapse = false;
					int min = col;
					int max = col;
					if (getHCollapseBounds(row, min, max, type))
					{
						bCollapse = true;

						for (int i = min; i <= max; ++i)
						{
							if (i != col)
								executeVCollapsing(i, row, row);
						}
					}

					//Verify vertical collapsing
					min = max = row;
					if (getVCollapseBounds(col, min, max, type))
						executeVCollapsing(col, min, max);
					else if (bCollapse)
						executeVCollapsing(col, row, row);
				}
			}
		}

		void GemBoardModel::clearBoard()
		{
			for (int row = 0; row < NB_ROWS; ++row)
			{
				for (int col = 0; col < NB_COLS; ++col)
					m_board[row][col] = GemType::NO_GEM;
			}
		}

		GemType GemBoardModel::generateGem(int row, int col) const
		{
			static std::mt19937 s_randGenerator(std::random_device{}());
			static std::uniform_int_distribution<unsigned short> s_randDistrib(0, GemType::NB_GEM_TYPES - 1);

			GemType type = static_cast<GemType>(s_randDistrib(s_randGenerator));
			assert(type < GemType::NB_GEM_TYPES);

			int minRow = row;
			int maxRow = row;
			int minCol = col;
			int maxCol = col;

			while (getHCollapseBounds(row, minCol, maxCol, type) || getVCollapseBounds(col, minRow, maxRow, type))
			{
				minRow = maxRow = row;
				minCol = maxCol = col;
				type = static_cast<GemType>((type + 1) % GemType::NB_GEM_TYPES);
			}

			return type;
		}

		bool GemBoardModel::getHCollapseBounds(int row, int& minCol, int& maxCol, GemType type) const
		{
			assert(type != GemType::NO_GEM);
			assert(minCol == maxCol);

			while ((--minCol >= 0) && (m_board[row][minCol] == type));
			while ((++maxCol < NB_COLS) && (m_board[row][maxCol] == type));
			return (--maxCol - ++minCol + 1 >= COLLAPSE_SIZE);
		}

		bool GemBoardModel::getVCollapseBounds(int col, int& minRow, int& maxRow, GemType type) const
		{
			assert(type != GemType::NO_GEM);
			assert(minRow == maxRow);

			while ((--minRow >= 0) && (m_board[minRow][col] == type));
			while ((++maxRow < NB_ROWS) && (m_board[maxRow][col] == type));
			return (--maxRow - ++minRow + 1 >= COLLAPSE_SIZE);
		}

		void GemBoardModel::executeVCollapsing(int col, int minRow, int maxRow)
		{
			//Destroy all vertical gems that are collapsing
			for (int row = minRow; row <= maxRow; ++row)
			{
				GemType type = m_board[row][col];
				if (type != GemType::NO_GEM)
				{
					m_board[row][col] = GemType::NO_GEM;
					m_view.onGemDestroyed(row, col);
				}
			}

			//Drop down all valid gems above
			while (++maxRow < NB_ROWS)
			{
				GemType type = m_board[maxRow][col];
				if (type != GemType::NO_GEM)
				{
					m_board[minRow][col] = type;
					m_board[maxRow][col] = GemType::NO_GEM;
					m_view.onGemFallen(maxRow, minRow, col);
					++minRow;
				}
			}

			//Generate new gems for the top of the column
			while (minRow < NB_ROWS)
			{
				GemType type = generateGem(minRow, col);
				m_board[minRow][col] = type;
				m_view.onGemCreated(type, minRow, col);
				++minRow;
			}
		}
	}
}
