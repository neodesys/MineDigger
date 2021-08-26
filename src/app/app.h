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

#ifndef _APP_H_
#define _APP_H_

namespace app
{
    class IGame;

    enum RetCode : short
    {
        SUCCESS = 0,
        ERR_NO_ENGINE = -1,
        ERR_NO_LOADING_SCREEN = -2,
        ERR_GAME_RES_LOADING = -3,
        ERR_SCREEN_RES_LOADING = -4
    };

    RetCode run(IGame& game);
} // namespace app

#endif // _APP_H_
