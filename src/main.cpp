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

#include <SDL2/SDL_main.h>

#include "app/app.h"
#include "game/MineDigger.h"
#include "sys/Logger.h"

int main(int argc, char** argv)
{
    // Parse command line for log priority
    for (int i = 1; i < argc; ++i)
    {
        if ((!SDL_strcmp(argv[i], "-l") || !SDL_strcmp(argv[i], "--log")) && (i + 1 < argc))
        {
            const char* level = argv[i + 1];
            if (!SDL_strcasecmp(level, "info"))
            {
                sys::Logger::setLogPriority(sys::LogLevel::INFO);
                break;
            }
            else if (!SDL_strcasecmp(level, "warn") || !SDL_strcasecmp(level, "warning"))
            {
                sys::Logger::setLogPriority(sys::LogLevel::WARNING);
                break;
            }
            else if (!SDL_strcasecmp(level, "crit") || !SDL_strcasecmp(level, "critical"))
            {
                sys::Logger::setLogPriority(sys::LogLevel::CRITICAL);
                break;
            }
        }
    }

    // Run game
    game::MineDigger game;
    return app::run(game);
}
