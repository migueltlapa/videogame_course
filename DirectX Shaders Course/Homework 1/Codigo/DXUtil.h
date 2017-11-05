#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include "dxerr.h"

#pragma comment (lib, "d3d11.lib")


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
	HRESULT hr = x;                                      \
if (FAILED(hr))                                         \
		{                                                      \
		DXTraceW(__FILEW__, __LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) x
#endif
#endif 


namespace Memory
{
	template <class T> void SafeDelete(T & t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}

	template <class T> void SafeDeteleArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}

	template <class T> void SafeRelease(T & t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}

namespace Utility
{
	void GetTextureDim(ID3D11Resource*res, UINT* width, UINT* height);
}