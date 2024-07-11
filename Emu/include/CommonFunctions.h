#pragma once

#include <string>
#include <random>
#include <sstream>

namespace Engine
{
	// Not guaranteed to be unique, but good enough for our purposes.
	// Maybe switch to boost::uuids::uuid if we need a more robust solution.
	inline static std::string CreateUUID()
	{
		std::random_device rd;
		std::uniform_int_distribution<int> dist(0, 15);

		std::stringstream ss;
		for (int i = 0; i < 32; i++)
		{
			if (i == 8 || i == 12 || i == 16 || i == 20)
			{
				ss << "-";
			}
			ss << std::hex << dist(rd);
		}

		return ss.str();
	}

	inline static double Lerp(double a, double b, double f)
	{
		return (a * (1.0f - f)) + (b * f);
	}
}