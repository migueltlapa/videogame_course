#include "Framework\DXApp.h"
#include "Framework\Camera.h"
#include "Framework\MathHelper.h"
#include "D3DCompiler.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <fstream>
#include <vector>

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")


struct VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal; 
};

D3D11_INPUT_ELEMENT_DESC iedPhong[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

class PhongApp : public DXApp{
public:
	PhongApp(HINSTANCE hInstance);
	~PhongApp();

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
	ID3D11Buffer		*mpBoxIB;

	UINT	mModelIndexCount;

	//rasterizer state
	ID3D11RasterizerState* mpWireframeRS;

	//constant buffers
	enum ConstanBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		NumConstantBuffers   //must be the last
	};

	ID3D11Buffer* mpConstantBuffers[NumConstantBuffers];
	ID3D11Buffer* mpConstP[1];

	//Matrices
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;

	//
	float mAngle;

	//camera
	Camera mCam;

	POINT mLastMousePos;

private:
	void InitShaders();		//loads and prepares the shaders
	void initPrecompiledShaders();
	void InitBuffers();		//creates the shape to render
	void InitConstantBuffers();
	void InitRasterizerState();


	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
};

PhongApp::PhongApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "PhongApp";

	//mouse
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//camera
	mCam.SetPosition(0.0f, 4.0f, -10.0f);



	mAngle = 0.0f;

	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mWorld, I);
	/*XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProjection, I);*/

}

PhongApp::~PhongApp()
{
	Memory::SafeRelease(mpLayout);

	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPS);

	Memory::SafeRelease(mpModelVB);
	Memory::SafeRelease(mpBoxIB);

	Memory::SafeRelease(mpConstantBuffers[CB_Application]);
	Memory::SafeRelease(mpConstantBuffers[CB_Frame]);
	Memory::SafeRelease(mpConstantBuffers[CB_Object]);
}

void PhongApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Application], 0, nullptr, &mProjection, 0, 0);
}

bool PhongApp::Init()
{
	if (!DXApp::Init())
		return false;


	InitBuffers();
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



void PhongApp::Update(float dt)
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

	
	XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);

	XMMATRIX W = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle));
	XMStoreFloat4x4(&mWorld, W);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &mWorld, 0, 0);
	
	XMFLOAT3 Epos = mCam.GetPosition();
	XMFLOAT4 EyePos = XMFLOAT4(Epos.x, Epos.y, Epos.z, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstP[0], 0, nullptr, &EyePos, 0, 0);

}

void PhongApp::Render(float dt)
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
	m_pImmediateContext->IASetIndexBuffer(mpBoxIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->RSSetState(mpWireframeRS);
	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 3, mpConstantBuffers);
	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, mpConstP);

	// draw the vertex buffer to the back buffer
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	//***********************
	HR(m_pSwapChain->Present(0, 0));
}

void PhongApp::InitBuffers(){

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
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mpBoxIB));


}

void PhongApp::InitConstantBuffers(){
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

	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[0]));

}

void PhongApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	Utility::CompileShader(L"Shaders/ShaderPhongV.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"Shaders/ShaderPhongP.hlsl", "PShader", profileP, &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mpVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mpPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	// create the input layout object
	


	m_pDevice->CreateInputLayout(iedPhong, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &mpLayout);
	m_pImmediateContext->IASetInputLayout(mpLayout);


	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}

void PhongApp::initPrecompiledShaders()
{
	ID3DBlob* pVS, *pPS;

#if _DEBUG
	LPCWSTR compiledVShader = L"Debug/ShaderPhongVd.cso";
	LPCWSTR compiledPShader = L"Debug/ShaderPhongPd.cso";
#else
	LPCWSTR compiledVShader = L"Release/ShaderPhongV.cso";
	LPCWSTR compiledPShader = L"Release/ShaderPhongP.cso";
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

	m_pDevice->CreateInputLayout(iedPhong, 2,
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		&mpLayout);

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);

}

void PhongApp::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));

	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // D3D11_FILL_SOLID;  //
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));
}

void PhongApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void PhongApp::OnMouseMove(WPARAM btnState, int x, int y)
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

void PhongApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();

}



int WINAPI WinMain5B(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	PhongApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}