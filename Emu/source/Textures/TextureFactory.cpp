#pragma once

#include <memory>
#include <string>

#include "../../include/Core.h"

#include "../../include/Textures/TextureFactory.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	std::shared_ptr<ITexture> CreateTexture(int r, int g, int b)
	{
		return std::make_shared<Texture>(r, g, b);
	}
}