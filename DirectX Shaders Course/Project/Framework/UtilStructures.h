#pragma once

#include "DXUtil.h"



struct ObjectMatrices
{
	XMMATRIX wvp;
	XMMATRIX worldMatrix;
	XMMATRIX worldIT;
	XMMATRIX texTransform;
};




struct VertexPNT
{
	VertexPNT() : Pos(0.0f, 0.0f, 0.0f),
		Normal(0.0f, 0.0f, 0.0f),
		Tex(0.0f, 0.0f){}
	VertexPNT(float px, float py, float pz,
		float nx, float ny, float nz,
		float u, float v) :
		Pos(px, py, pz),
		Normal(nx, ny, nz),
		Tex(u, v){}
	VertexPNT(const XMFLOAT3 &p,
		const XMFLOAT3 &n,
		const XMFLOAT2 &uv) :
		Pos(p), Normal(n), Tex(uv){}
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
};

D3D11_INPUT_ELEMENT_DESC iedPNT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct VertexPNTT
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
	XMFLOAT2 Tex;
};

D3D11_INPUT_ELEMENT_DESC iedPNTT[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};