#pragma once

#ifdef EMU_BUILD_DLL
	#define EMU_API _declspec(dllexport)
#else
	#define EMU_API _declspec(dllimport)
#endif