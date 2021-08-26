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

#ifndef _IRESHOLDER_H_
#define _IRESHOLDER_H_

namespace sys
{
    class GameEngine;
}

namespace app
{
    enum struct ResState : unsigned char
    {
        READY,
        LOADING,
        ERROR
    };

    class IResHolder
    {
      public:
        virtual ~IResHolder() = default;

        // If pEngine is null, getResState() MUST only return the current
        // resources state WITHOUT trying to load next resource.
        // If pEngine is valid, getResState() MUST load next resource and return
        // READY if all resources have been loaded, LOADING if there is still
        // resources to load, or ERROR if resource loading has failed.
        // In all cases, before returning ERROR, getResState() SHOULD call
        // cleanRes(true) to enforce resources cleaning.
        virtual ResState getResState(const sys::GameEngine* pEngine) = 0;

        // cleanRes() must be deeply transitive if this IResHolder contains any
        // embedded IResHolder.
        // If bForce is true, cleanRes() MUST clean all resources.
        // If bForce is false, cleanRes() MAY clean all or some of the resources
        // depending of the IResHolder usage.
        virtual void cleanRes(bool bForce) = 0;
    };
} // namespace app

#endif // _IRESHOLDER_H_
