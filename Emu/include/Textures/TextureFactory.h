#pragma once

#include <memory>
#include <string>

#include "../Core.h"

#include "ITexture.h"

namespace Engine
{
	EMU_API std::shared_ptr<ITexture> CreateTexture(int r, int g, int b);
	// EMU_API std::shared_ptr<ITexture> CreateTexture(const std::string& filePath);
}