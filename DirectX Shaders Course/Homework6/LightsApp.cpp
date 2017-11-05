#include "Framework\DXApp.h"
#include "Framework\Camera.h"
#include "Framework\MathHelper.h"
#include "Framework\LightHelper.h"

#include "D3DCompiler.h"


#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <fstream>
#include <vector>
#include "Framework\GeometryGenerator.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")


struct VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal; 
};

struct EYE
{
	XMFLOAT4 EyePos;
	XMFLOAT4 EyeDir;
};

D3D11_INPUT_ELEMENT_DESC iedLights[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

class LightsApp : public DXApp{
public:
	LightsApp(HINSTANCE hInstance);
	~LightsApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
	void OnResize() override;

private:

	//input layout
	ID3D11InputLayout	*mpLayout;           // the pointer to the input layout

	//shaders
	ID3D11VertexShader	*mpVS;               // the pointer to the vertex shader
	ID3D11PixelShader	*mpPS;               // the pointer to the pixel shader

	//Buffers
	ID3D11Buffer		*mpModelVB;          // the pointer to the vertex buffer
	ID3D11Buffer		*mpSkullIB;

	UINT	mModelIndexCount;

	//rasterizer state
	ID3D11RasterizerState* mpWireframeRS;

	//constant buffers
	enum ConstantBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers   //must be the last
	};

	ID3D11Buffer* mpConstantBuffers[NumConstantBuffers];

	enum PSConstantBuffer
	{
		CB_EyeInfo,
		CB_LightInfo,
		CB_MaterialInfo,
		PSNumConstantBuffers   //must be the last
	};


	ID3D11Buffer* mpConstP[PSNumConstantBuffers];

	//Matrices
	XMFLOAT4X4 mSkullWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;

	//
	float mAngle;

	//camera
	Camera mCam;

	POINT mLastMousePos;

	//
	Material mSkullMat;


	//LAND
	Material mLandMat;
	
	ID3D11Buffer *	mpLandVB;
	ID3D11Buffer *  mpLandIB;

	UINT	 mLandIndexCount;

	XMFLOAT4X4  mLandWorld;

private:
	void InitShaders();		//loads and prepares the shaders
	void initPrecompiledShaders();
	void InitBuffers();		//creates the shape to render
	void InitLandBuffers();
	void InitConstantBuffers();
	void InitRasterizerState();

	float GetHillHeight(float x, float z) const;
	XMFLOAT3 GetHillNormal(float x, float z) const;


	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
};

LightsApp::LightsApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "PhongApp";

	//mouse
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//camera
	mCam.SetPosition(0.0f, 4.0f, -10.0f);

	//skull material
	mSkullMat.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mSkullMat.Specular= XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mSkullMat.SpecularPower = 120.0f;
	mSkullMat.UseTexture = 0;
	//Land material
	mLandMat.Ambient = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mLandMat.Diffuse = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mLandMat.Specular = XMFLOAT4(0.2, 0.2, 0.2, 1.0f);
	mLandMat.SpecularPower = 16.0f;
	mLandMat.UseTexture = 0;

	

	mAngle = 0.0f;

	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mSkullWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProjection, I);
	XMStoreFloat4x4(&mLandWorld, I);


	


}

LightsApp::~LightsApp()
{
	Memory::SafeRelease(mpLayout);

	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPS);

	Memory::SafeRelease(mpModelVB);
	Memory::SafeRelease(mpSkullIB);

	Memory::SafeRelease(mpConstantBuffers[CB_Application]);
	Memory::SafeRelease(mpConstantBuffers[CB_Frame]);
	Memory::SafeRelease(mpConstantBuffers[CB_Object]);

	Memory::SafeRelease(mpConstP[CB_EyeInfo]);
	Memory::SafeRelease(mpConstP[CB_LightInfo]);
	Memory::SafeRelease(mpConstP[CB_MaterialInfo]);

	Memory::SafeRelease(mpWireframeRS);
}

void LightsApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Application], 0, nullptr, &mProjection, 0, 0);
}

bool LightsApp::Init()
{
	if (!DXApp::Init())
		return false;


	InitBuffers();
	//InitLandBuffers();
	//InitShaders();
	initPrecompiledShaders();
	InitConstantBuffers();
	InitRasterizerState();
	OnResize();

	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR pos = XMLoadFloat3(&mCam.GetPosition());
	mCam.LookAt(pos, focusPoint, upDirection);

	return true;
}



void LightsApp::Update(float dt)
{
	//
	// Control the camera.
	//
	if (GetAsyncKeyState('W') & 0x8000)
		mCam.Walk(10.0f*dt);

	if (GetAsyncKeyState('S') & 0x8000)
		mCam.Walk(-10.0f*dt);

	if (GetAsyncKeyState('A') & 0x8000)
		mCam.Strafe(-10.0f*dt);

	if (GetAsyncKeyState('D') & 0x8000)
		mCam.Strafe(10.0f*dt);

	using namespace DirectX;
	using namespace DirectX::PackedVector;


	//XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	/*if (GetAsyncKeyState('A'))
	eyePosition = XMVectorSet(0, 0, -10 + ((int)(mAngle) % 100) + 0.7f, 1);*/

	mCam.UpdateViewMatrix();
	XMMATRIX V = mCam.View();
	//XMMATRIX V = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	XMStoreFloat4x4(&mView, V);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Frame], 0, nullptr, &mView, 0, 0);

	
	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);

	//mAngle += 90.0f*dt;
	XMMATRIX W = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle));
	XMStoreFloat4x4(&mSkullWorld, W);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &mSkullWorld, 0, 0);
	
	XMFLOAT3 Epos = mCam.GetPosition();
	XMFLOAT4 EyePos = XMFLOAT4(Epos.x, Epos.y, Epos.z, 1.0f);
	
	XMFLOAT3 Edir = mCam.GetLook();
	//Light.Position = XMFLOAT4(0.0f, 4.0f, -10.0f, 1.0f);
	XMFLOAT4 EyeDir = XMFLOAT4(Edir.x, Edir.y, Edir.z, 1.0f);

	EYE eyeInfo; 
	// Defino la posicion de la luz de manera constante
	
	// Defino la direccion de la luz de manera constante

	eyeInfo.EyePos	= EyePos;
	eyeInfo.EyeDir	= EyeDir;
	

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_EyeInfo], 0, nullptr, &eyeInfo, 0, 0);

	// Defining the Light structure
	Light gLight[3];
	//gLight.Position = EyePos;
	gLight[0].Position = XMFLOAT4(0.0f, 4.0f, -10.0f, 1.0f);
	gLight[0].Direction = EyeDir;
	gLight[0].Color = XMFLOAT4(1, 1, 1, 1);
	gLight[0].Kc = 1.0f;
	gLight[0].Kl = 0.01f;
	gLight[0].Kq = 0.02f;
	gLight[0].SpotAngle = XMConvertToRadians(15);

	//gLight.Position = EyePos;
	gLight[1].Position = XMFLOAT4(0.0f, 8.0f, -10.0f, 1.0f);
	gLight[1].Direction = EyeDir;
	gLight[1].Color = XMFLOAT4(1, 0, 0, 1);
	gLight[1].Kc = 1.0f;
	gLight[1].Kl = 0.01f;
	gLight[1].Kq = 0.02f;
	gLight[1].SpotAngle = XMConvertToRadians(5);

	//gLight.Position = EyePos;
	gLight[2].Position = XMFLOAT4(0.0f, 6.0f, -10.0f, 1.0f);
	gLight[2].Direction = EyeDir;
	gLight[2].Color = XMFLOAT4(0, 1, 0, 1);
	gLight[2].Kc = 1.0f;
	gLight[2].Kl = 0.01f;
	gLight[2].Kq = 0.02f;
	gLight[2].SpotAngle = XMConvertToRadians(5);


	m_pImmediateContext->UpdateSubresource(mpConstP[CB_LightInfo], 0, nullptr, &gLight[0], 0, 0);

	//material
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);




}

void LightsApp::Render(float dt)
{

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//***********************
	m_pImmediateContext->IASetInputLayout(mpLayout);
	// select which primtive type we are using
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->RSSetState(mpWireframeRS);
	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, NumConstantBuffers, mpConstantBuffers);
	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, PSNumConstantBuffers, mpConstP);

	// draw the vertex buffer to the back buffer
	//draw the SKULL
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	XMMATRIX t = XMMatrixTranslation(0, 6, -10);
	XMStoreFloat4x4(&mSkullWorld, t);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &mSkullWorld, 0, 0);
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	//draw the LAND
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpLandVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpLandIB, DXGI_FORMAT_R32_UINT, 0);

	//update the vertex shader: world matrix
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &mLandWorld, 0, 0);
	//update the pixel shader: material info
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mLandMat, 0, 0);
	m_pImmediateContext->DrawIndexed(mLandIndexCount, 0, 0);



	//***********************
	HR(m_pSwapChain->Present(0, 0));
}

void LightsApp::InitBuffers(){

	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, "skull.txt not found.", 0, 0);
	}
	
	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;  	// VertexCount: 31076
	fin >> ignore >> tcount;  	// TriangleCount: 60339
	fin >> ignore >> ignore >> ignore >> ignore; // VertexList (pos, normal)
	
	std::vector<VERTEX> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore; //}
	fin >> ignore; //triangleList
	fin >> ignore; //{

	mModelIndexCount = 3 * tcount;
	std::vector<UINT> indices(mModelIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

		
	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	
	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VERTEX)* vcount;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];

	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpModelVB));       // create the buffer

	//indices
	
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* mModelIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpSkullIB));


}

void LightsApp::InitLandBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;
	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	mLandIndexCount = grid.Indices.size();

	std::vector<VERTEX> vertices(grid.Vertices.size());
	for (UINT i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;
		p.y = GetHillHeight(p.x, p.z);
		vertices[i].Pos = p;
		//vertices[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[i].Normal = GetHillNormal(p.x,p.z);
		//grid.Vertices[i].TexC;
	}

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VERTEX)* grid.Vertices.size(); // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpLandVB));       // create the buffer

	//indices

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* mLandIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &grid.Indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpLandIB));


}
void LightsApp::InitConstantBuffers(){
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_Application]));
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_Frame]));
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_Object]));

	constantBufferDesc.ByteWidth = sizeof(EYE);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_EyeInfo]));
	
	constantBufferDesc.ByteWidth = sizeof(Light)*3;
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_LightInfo]));

	constantBufferDesc.ByteWidth = sizeof(Material);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_MaterialInfo]));

}



void LightsApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	Utility::CompileShader(L"Shaders/ShaderLightsV.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"Shaders/ShaderLightsP.hlsl", "PShader", profileP, &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mpVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mpPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	// create the input layout object
	m_pDevice->CreateInputLayout(iedLights, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &mpLayout);
	m_pImmediateContext->IASetInputLayout(mpLayout);


	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}

void LightsApp::initPrecompiledShaders()
{
	ID3DBlob* pVS, *pPS;

#if _DEBUG
	LPCWSTR compiledVShader = L"Debug/ShaderLightsVd.cso";
	LPCWSTR compiledPShader = L"Debug/ShaderLightsPd.cso";
#else
	LPCWSTR compiledVShader = L"Release/ShaderLightsV.cso";
	LPCWSTR compiledPShader = L"Release/ShaderLightsP.cso";
#endif

	HR(D3DReadFileToBlob(compiledVShader, &pVS));
	HR(D3DReadFileToBlob(compiledPShader, &pPS));

	m_pDevice->CreateVertexShader(pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		nullptr,
		&mpVS);

	m_pDevice->CreatePixelShader(pPS->GetBufferPointer(),
		pPS->GetBufferSize(),
		nullptr,
		&mpPS);

	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	m_pDevice->CreateInputLayout(iedLights, 2,
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		&mpLayout);

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);

}

void LightsApp::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));

	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // D3D11_FILL_SOLID;  //
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));
}

void LightsApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void LightsApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mCam.Pitch(dy);
		mCam.RotateY(dx);

	}


	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void LightsApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();

}

float LightsApp::GetHillHeight(float x, float z) const
{
	return  0.3*( z*sinf(0.1f * x) + x*cosf(0.1f*z) );
}

DirectX::XMFLOAT3 LightsApp::GetHillNormal(float x, float z) const
{
  //n = (-df/dx, 1, -df/dz)

	XMFLOAT3 n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z)
		);
	
	XMVECTOR  unitNormal = XMVector3Normalize(XMLoadFloat3(&n));
	XMStoreFloat3(&n, unitNormal);
	return n;

}






int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	LightsApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}