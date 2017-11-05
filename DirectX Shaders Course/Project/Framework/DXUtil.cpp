
#include "DXUtil.h"
#include <D3Dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")


void Utility::GetTextureDim(ID3D11Resource*res, UINT* width, UINT* height)
{
	D3D11_RESOURCE_DIMENSION dim;
	res->GetType(&dim);

	switch (dim)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto texture = reinterpret_cast<ID3D11Texture2D*>(res);
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			if (width)*width = desc.Width; //width of texture in pixels
			if (height)*height = desc.Height;//height of texture in pixels
		}
		break;
	default:
		{
				if (width)*width = 0; //width of texture in pixels
				if (height)*height = 0;//height of texture in pixels
		}
	break;

	}
}

HRESULT Utility::CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}


