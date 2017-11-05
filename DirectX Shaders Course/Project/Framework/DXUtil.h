#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "dxerr.h" // download the DXERR.lib http://blogs.msdn.com/b/chuckw/archive/2012/04/24/where-s-dxerr-lib.aspx

#pragma comment (lib, "d3d11.lib")

using namespace DirectX;
using namespace DirectX::PackedVector;


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
	{														\
	HRESULT hr = x;											\
if (FAILED(hr))												\
		{                                                   \
		DXTraceW(__FILEW__, __LINE__, hr, L#x, true);		\
		}                                                   \
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
	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob);
}

namespace GColors
{
	XMGLOBALCONST XMFLOAT4 White	= { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Black	= { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Red		= { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Green	= { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Blue		= { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Yellow	= { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Cyan		= { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Magenta	= { 1.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMFLOAT4 Silver	= { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMFLOAT4 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}