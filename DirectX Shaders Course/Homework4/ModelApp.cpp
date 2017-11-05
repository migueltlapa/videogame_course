#include "Framework\DXApp.h"
#include "Framework\Camera.h"
#include "Framework\MathHelper.h"
#include "D3DCompiler.h"

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include<fstream>
#include<vector>



using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")
struct VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 Color;

};




D3D11_INPUT_ELEMENT_DESC ied[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};



class ModelApp : public DXApp{
public:
	ModelApp(HINSTANCE hInstance);
	~ModelApp();

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
	ID3D11Buffer		*mpBoxVB;          // the pointer to the vertex buffer
	ID3D11Buffer		*mpBoxIB;

	UINT	mModelIndexCount;
	ID3D11RasterizerState* mpWireframeRS;

	//Pointer_Calaca *position_skull;
	float m_count;

	//constant buffers
	enum ConstanBuffer
	{
		CB_Application,
		CB_Frame,
		CB_Object,
		CB_WVP,
		NumConstantBuffers   //must be the last
	};

	ID3D11Buffer* mpConstantBuffers[NumConstantBuffers];

	//Matrices
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;
	XMFLOAT4X4 mWVP;
	
	//
	float mAngle;

	//camera
	Camera mCam;

	POINT mLastMousePos;

	// Position Mesh

	int mesh_x;
	int mesh_y;
	int mesh_z;
	int count_pos_mesh;
	
	//std::vector<VERTEX> vertices;
	
	// Init Rasterize State
	int count_rasterizer_state;


private:
	void InitShaders();		//loads and prepares the shaders
	void InitBuffers();		//creates the shape to render
	void InitConstantBuffers();

	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void InitPrecompiledShaders();
	void InitRasterizerState();

};

ModelApp::ModelApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "ModelApp";

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

ModelApp::~ModelApp()
{
	Memory::SafeRelease(mpLayout);

	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPS);

	Memory::SafeRelease(mpBoxVB);
	Memory::SafeRelease(mpBoxIB);

	Memory::SafeRelease(mpConstantBuffers[CB_Application]);
	Memory::SafeRelease(mpConstantBuffers[CB_Frame]);
	Memory::SafeRelease(mpConstantBuffers[CB_Object]);
	Memory::SafeRelease(mpConstantBuffers[CB_WVP]);	
}

void ModelApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);
	//@W
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Application], 0, nullptr, &mProjection, 0, 0);
}

bool ModelApp::Init()
{
	if (!DXApp::Init())
		return false;

	
	InitBuffers();
	//InitShaders();
	InitPrecompiledShaders();
	InitConstantBuffers();
	//InitRasterizerState();

	OnResize();

	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR pos = XMLoadFloat3(&mCam.GetPosition());
	mCam.LookAt(pos, focusPoint, upDirection);

	return true;
}



void ModelApp::Update(float dt)
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

	//position_skull->info->Pos.x = position_skull->info->Pos.x + 5;
	
	

	//XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	/*if (GetAsyncKeyState('A'))
	eyePosition = XMVectorSet(0, 0, -10 + ((int)(mAngle) % 100) + 0.7f, 1);*/



	mCam.UpdateViewMatrix();
	XMMATRIX V = mCam.View();
	//XMMATRIX V = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	XMStoreFloat4x4(&mView, V);
	//@W
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Frame], 0, nullptr, &mView, 0, 0);

	//mAngle += 90.0f * dt;
	XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);

	if (GetAsyncKeyState('I') & 0x01)
	{
		
		
		if (count_pos_mesh == 0)
		{
			mesh_x++;
		}
		
		if (count_pos_mesh == 1)
		{
			mesh_y++;
		}

		if (count_pos_mesh == 2)
		{
			mesh_z++;
			count_pos_mesh = 0;
		}

		count_pos_mesh++;
	}

	if (GetAsyncKeyState('K') & 0x01)
	{
		if (count_rasterizer_state == 0)
		{
			
			D3D11_RASTERIZER_DESC wireframeDesc;
			ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
			wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME; //
			wireframeDesc.CullMode = D3D11_CULL_NONE;
			wireframeDesc.FrontCounterClockwise = false;
			wireframeDesc.DepthClipEnable = true;
			HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));
		
		
		}

		if (count_rasterizer_state == 1)
		{
			
			D3D11_RASTERIZER_DESC wireframeDesc;
			ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
			wireframeDesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME; //
			wireframeDesc.CullMode = D3D11_CULL_NONE;
			wireframeDesc.FrontCounterClockwise = false;
			wireframeDesc.DepthClipEnable = true;
			HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));



			count_rasterizer_state = -1;
		}
		

		count_rasterizer_state++;
	}



	XMMATRIX W = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle)) + XMMatrixTranslation(mesh_x,mesh_y,mesh_z);
	
	XMStoreFloat4x4(&mWorld, W);
	//@W
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &mWorld, 0, 0);
	XMMATRIX P = XMLoadFloat4x4(&mProjection);
	XMMATRIX WVP = W * V * P;
	XMStoreFloat4x4(&mWVP,WVP);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_WVP], 0, nullptr, &mWVP, 0, 0);
	///
	


}

void ModelApp::Render(float dt)
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
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpBoxVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpBoxIB, DXGI_FORMAT_R32_UINT, 0);

	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	//@WT
	m_pImmediateContext->VSSetConstantBuffers(0, 4, mpConstantBuffers);

	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);

	m_pImmediateContext->RSSetState(mpWireframeRS);

	// draw the vertex buffer to the back buffer
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	XMMATRIX t = XMMatrixTranslation(-2, -1, 2);
	
	XMStoreFloat4x4(&mWVP, t);

	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_WVP], 0, nullptr, &mWVP, 0, 0);
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	
	//***********************
	HR(m_pSwapChain->Present(0, 0));
}

void ModelApp::InitBuffers(){
	
	
	std::ifstream fin("Models/skull.txt");
	//std::ifstream fin("Models/car.txt");
	if (!fin)
	{
		MessageBox(0, "skull.txt not found", 0, 0);
	}


	//Box 
	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	// VertexCount: 31076
	fin >> ignore >> tcount;
	// TriangleCount: 60339
	fin >> ignore >> ignore >> ignore >> ignore; // VertexList (pos, normal)

	
	std::vector<VERTEX> vertices(vcount);
	//*pointer_array = vertices[0];
	for (UINT i = 0; i < vcount; ++i)
	{
	
		//0.592978 		1.92413 			- 2.62486
			fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
			vertices[i].Color = vertices[i].Pos;
			//vertices[i].Pos.x = vertices[i].Pos.x + 5;
			
		//0.572276 		0.816877 			0.0721907
			fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
			
	}

	fin >> ignore;//}

	fin >> ignore;// trianglelist
	fin >> ignore;//{

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
	
	// @T bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex

	bd.Usage = D3D11_USAGE_DYNAMIC;             // we wont change this vertex
	bd.ByteWidth = sizeof(VERTEX)* vcount;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	//  @T bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer

	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;						  // we don't need to allow CPU to write in buffer
	bd.MiscFlags = 0;
	
	bd.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = &vertices[0];
	//@T
	
	//position_skull->info = &vertices[0];

	//@T HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpBoxVB));       // create the buffer

	HR(m_pDevice->CreateBuffer(&bd, &bdData, &mpBoxVB));       // create the buffer
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

void ModelApp::InitConstantBuffers(){
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
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_WVP]));
}

void ModelApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	Utility::CompileShader(L"Shaders/VShaderModel.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"Shaders/PShaderModel.hlsl", "PShader", profileP, &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mpVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mpPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	// create the input layout object
	
	m_pDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &mpLayout);
	m_pImmediateContext->IASetInputLayout(mpLayout);

	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}

void ModelApp::InitPrecompiledShaders()
{
	ID3DBlob *pVS;
	ID3DBlob *pPS;

#if _DEBUG

	LPCWSTR compiledVShader = L"Debug/VShaderModeld.cso";
	LPCWSTR compiledPShader = L"Debug/PShaderModeld.cso";


#else

	LPCWSTR compiledVShader = L"Shader.cso";
	LPCWSTR compiledPShader = L"ShaderP.cso";
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


	m_pImmediateContext->VSSetShader(mpVS, 0, 0);// Cargamos a Contexto
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	
	m_pDevice->CreateInputLayout(ied, 3, pVS->GetBufferPointer(), pVS->GetBufferSize(), &mpLayout);
	// El 2 es por el numero de elementos de l aestructura

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);

}

void ModelApp::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME; //
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));

}

void ModelApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void ModelApp::OnMouseMove(WPARAM btnState, int x, int y)
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

void ModelApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();
}


int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	ModelApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}