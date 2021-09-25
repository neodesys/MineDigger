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

#include "ResLoader.h"

#include <new>

#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_image.h>

namespace
{
	const char ASSETS_CHECK_FILE[] = "assets.dir";

#ifdef _WIN32
	const char ASSETS_RES_SUBDIR[] = "res\\";
#else
	const char ASSETS_RES_SUBDIR[] = "res/";
#endif //_WIN32

	const std::size_t ASSET_MAX_PATH_LENGTH = 256;
}

namespace sys
{
	const Logger ResLoader::s_log("ResLoader");

	ResLoader* ResLoader::createResLoader()
	{
		//Search for resources directory
		//* We first look for "assets.dir" file directly as on some platforms
		//(Android) resources may be loaded from a specific "assets" storage
		//place.
		//* Then we look relatively to the application executable:
		//  - once again looking for "assets.dir" file directly as on some
		//    platforms (OSX/iOS) SDL base path may be already set to the
		//    application bundle resources directory;
		//  - eventually by looking for a subdirectory "res/" in the same
		//    folder as the executable file (Windows/Linux).
		std::size_t basePathLength = 0;
		char* pPathBuffer = nullptr;

		SDL_RWops* pFile = SDL_RWFromFile(ASSETS_CHECK_FILE, "r");
		if (!pFile)
		{
			char* pBasePath = SDL_GetBasePath();
			if (!pBasePath)
			{
				s_log.critical("Cannot find assets directory (%s)", SDL_GetError());
				return nullptr;
			}

			basePathLength = SDL_strlen(pBasePath);
			pPathBuffer = static_cast<char*>(SDL_malloc(basePathLength + sizeof(ASSETS_RES_SUBDIR) + ASSET_MAX_PATH_LENGTH));
			if (!pPathBuffer)
			{
				SDL_free(pBasePath);
				s_log.critical("Out of memory");
				return nullptr;
			}

			SDL_memcpy(pPathBuffer, pBasePath, basePathLength + 1);
			SDL_free(pBasePath);

			SDL_memcpy(pPathBuffer + basePathLength, ASSETS_CHECK_FILE, sizeof(ASSETS_CHECK_FILE));
			pFile = SDL_RWFromFile(pPathBuffer, "r");
			if (!pFile)
			{
				SDL_memcpy(pPathBuffer + basePathLength, ASSETS_RES_SUBDIR, sizeof(ASSETS_RES_SUBDIR));
				basePathLength += sizeof(ASSETS_RES_SUBDIR) - 1;

				SDL_memcpy(pPathBuffer + basePathLength, ASSETS_CHECK_FILE, sizeof(ASSETS_CHECK_FILE));
				pFile = SDL_RWFromFile(pPathBuffer, "r");
				if (!pFile)
				{
					SDL_free(pPathBuffer);
					s_log.critical("Cannot find assets directory (%s)", SDL_GetError());
					return nullptr;
				}
			}
		}
		SDL_RWclose(pFile);

		ResLoader* pLoader = new(std::nothrow) ResLoader(basePathLength, pPathBuffer);
		if (!pLoader)
		{
			SDL_free(pPathBuffer);
			s_log.critical("Out of memory");
			return nullptr;
		}

		if (pPathBuffer)
		{
			pPathBuffer[basePathLength] = '\0';
			s_log.info("Resource loader created with base path %s", pPathBuffer);
		}
		else
			s_log.info("Resource loader created with specific assets storage");

		return pLoader;
	}

	ResLoader::~ResLoader()
	{
		SDL_free(m_pPathBuffer);
	}

	SDL_Surface* ResLoader::loadImage(const char* asset)
	{
		SDL_Surface* pSDLSurf = IMG_Load(getAssetFullPath(asset));
		if (pSDLSurf)
			return pSDLSurf;

		s_log.warning("Cannot load \"%s\" image (%s)", asset ? asset : "", IMG_GetError());
		return nullptr;
	}

	const char* ResLoader::getAssetFullPath(const char* asset)
	{
		if (asset && (asset[0] != '\0'))
		{
			std::size_t length = SDL_strlen(asset);
			if (length <= ASSET_MAX_PATH_LENGTH)
			{
				if (m_pPathBuffer)
				{
					SDL_memcpy(m_pPathBuffer + m_basePathLength, asset, length + 1);
					return m_pPathBuffer;
				}
				else
					return asset;
			}
		}

		return "";
	}
}
