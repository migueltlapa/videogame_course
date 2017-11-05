#include "Framework\DXApp.h"
#include "Framework\Camera.h"
#include "Framework\MathHelper.h"
#include "Framework\LightHelper.h"
#include "Framework\UtilStructures.h"
#include "Framework\Waves.h"
#include "D3DCompiler.h"
#include <Effects.h> 

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <fstream>
#include <vector>
#include "Framework\GeometryGenerator.h"
#include "RenderStates.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")






class MirrorApp : public DXApp{
public:
	MirrorApp(HINSTANCE hInstance);
	~MirrorApp();

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

	//Buffers skull
	ID3D11Buffer		*mpModelVB;          // the pointer to the vertex buffer
	ID3D11Buffer		*mpSkullIB;
	
	
	

	UINT	mModelIndexCount;

	
	//constant buffers
	enum ConstantBuffer
	{
		CB_Object,
		NumConstantBuffers   //must be the last
	};

	ID3D11Buffer* mpConstantBuffers[NumConstantBuffers];

	enum PSConstantBuffer
	{
		CB_EyeInfo,
		CB_LightInfo,
		CB_MaterialInfo,
		CB_FogInfo,
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

	Material mMirrorMat;
	
	Fog mFog;

	ID3D11SamplerState* mpSamplerState;
	//------------------
	//Buffer Room
	ID3D11Buffer		*mpRoomVB;
	XMFLOAT4X4			mRoomWorld;
	Material			mRoomMat;

	ID3D11ShaderResourceView *mpSRV_floor;
	ID3D11ShaderResourceView *mpSRV_wall;
	ID3D11ShaderResourceView *mpSRV_mirror;



private:
	void InitShaders();		//loads and prepares the shaders
	void initPrecompiledShaders();
	void InitBuffers();		//creates the shape to render
	void InitRoomBuffer();
	void InitConstantBuffers();
	void InitTextures();

	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
};

MirrorApp::MirrorApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "MirrorApp";

	//mouse
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;

	//camera
	mCam.SetPosition(0.0f, 4.0f, -10.0f);

	//skull material
	mSkullMat.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.6f);
	mSkullMat.Specular= XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mSkullMat.SpecularPower = 120.0f;
	mSkullMat.UseTexture = 0;

	//Mirror material
	mMirrorMat.Ambient = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	mMirrorMat.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.3f);
	mMirrorMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mMirrorMat.SpecularPower = 120.0f;
	mMirrorMat.UseTexture = 1;

	//RoomMat
	mRoomMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mRoomMat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mRoomMat.Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mRoomMat.SpecularPower = 16.0f;
	mRoomMat.UseTexture = 1;
	
	mAngle = 0.0f;

	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mSkullWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProjection, I);
	XMStoreFloat4x4(&mRoomWorld, I);

	mFog.Color = GColors::Silver;
	mFog.Start = 10.0f;
	mFog.Range = 70.0f;
	mFog.Enable = 0;
	


}

MirrorApp::~MirrorApp()
{
	Memory::SafeRelease(mpLayout);

	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPS);

	Memory::SafeRelease(mpModelVB);
	Memory::SafeRelease(mpSkullIB);

	Memory::SafeRelease(mpConstantBuffers[CB_Object]);

	Memory::SafeRelease(mpConstP[CB_EyeInfo]);
	Memory::SafeRelease(mpConstP[CB_LightInfo]);
	Memory::SafeRelease(mpConstP[CB_MaterialInfo]);
	
}

void MirrorApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);

}

bool MirrorApp::Init()
{
	if (!DXApp::Init())
		return false;


	InitBuffers();
	//InitShaders();
	InitRoomBuffer();
	initPrecompiledShaders();
	InitConstantBuffers();
	InitTextures();
	OnResize();
	RenderStates::InitAll(m_pDevice);

	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR pos = XMLoadFloat3(&mCam.GetPosition());
	mCam.LookAt(pos, focusPoint, upDirection);

	return true;
}



void MirrorApp::Update(float dt)
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
	//m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Frame], 0, nullptr, &mView, 0, 0);

	
	XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);

	//mAngle += 90.0f*dt;
	XMMATRIX W = XMMatrixScaling(0.5f,0.5f,0.5f)*
		         XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle))*
				 XMMatrixTranslation(0.0f,1.0f, -5.0f);
	XMStoreFloat4x4(&mSkullWorld, W);
	
	XMFLOAT3 Epos = mCam.GetPosition();
	XMFLOAT4 EyePos = XMFLOAT4(Epos.x, Epos.y, Epos.z, 1.0f);
	
	XMFLOAT3 Edir = mCam.GetLook();
	XMFLOAT4 EyeDir = XMFLOAT4(Edir.x, Edir.y, Edir.z, 1.0f);

	EYE eyeInfo;
	eyeInfo.EyePos	= EyePos;
	eyeInfo.EyeDir	= EyeDir;
		
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_EyeInfo], 0, nullptr, &eyeInfo, 0, 0);

	// Defining the Light structure
	Light gLight;
	gLight.Position = EyePos;
	gLight.Direction = EyeDir;
	gLight.Color = XMFLOAT4(1, 1, 1, 1);
	gLight.Kc = 1.0f;
	gLight.Kl = 0.00f;
	gLight.Kq = 0.00f;
	gLight.SpotAngle = XMConvertToRadians(15);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_LightInfo], 0, nullptr, &gLight, 0, 0);
	
	
	
}

void MirrorApp::Render(float dt)
{

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//***********************
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);


	m_pImmediateContext->IASetInputLayout(mpLayout);
	// select which primtive type we are using
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// select which vertex buffer to display
	UINT stride = sizeof(VertexPNT);
	UINT offset = 0;
	
	//m_pImmediateContext->RSSetState(mpWireframeRS);
	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, NumConstantBuffers, mpConstantBuffers);
	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, PSNumConstantBuffers, mpConstP);

	m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);

	ObjectMatrices objMatrix;

	//-----------------------ROOM-------------------------
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	
	m_pImmediateContext->RSSetState(RenderStates::NoCullRS);
	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMatrix.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatrix.wvp = objMatrix.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatrix.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatrix.worldMatrix));
	objMatrix.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatrix, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mRoomMat, 0, 0);

	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_floor);
	m_pImmediateContext->Draw(6, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_wall);
	m_pImmediateContext->Draw(18, 6);
	m_pImmediateContext->RSSetState(nullptr);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	//***********************


	//-----------------------SKULL-------------------------
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMatrix.worldMatrix = XMLoadFloat4x4(&mSkullWorld);
	objMatrix.wvp = objMatrix.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatrix.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatrix.worldMatrix));
	objMatrix.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatrix, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);
	
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	
	//-------------DRAW THE MIRROR STENCIL------------------
	
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);
	
	//constant buffers of the vertex shader
	objMatrix.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatrix.wvp = objMatrix.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatrix.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatrix.worldMatrix));
	objMatrix.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatrix, 0, 0);

	//m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);

	m_pImmediateContext->OMSetBlendState(RenderStates::NoRenderTargetWritesBS,
									blendFactor, 0xffffffff);

	m_pImmediateContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);

	m_pImmediateContext->Draw(6, 24);

	m_pImmediateContext->OMSetDepthStencilState(0, 0);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);


	//------------SKULL REFLECTION------------------

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	//m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	XMVECTOR mirrorPlane = XMVectorSet(0, 0, 1.0f, 0.0);
	XMMATRIX R = XMMatrixReflect(mirrorPlane);
	objMatrix.worldMatrix = XMLoadFloat4x4(&mSkullWorld)*R;
	objMatrix.wvp = objMatrix.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatrix.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatrix.worldMatrix));
	objMatrix.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatrix, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);

	m_pImmediateContext->OMSetDepthStencilState(RenderStates::DrawReflectionDSS, 1);
	
	
	
	
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	m_pImmediateContext->OMSetDepthStencilState(0, 0);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	//------------------DRAW  MIRROR TO THE BACK BUFFER-------------------------------
	

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpRoomVB, &stride, &offset);

	//constant buffers of the vertex shader
	objMatrix.worldMatrix = XMLoadFloat4x4(&mRoomWorld);
	objMatrix.wvp = objMatrix.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMatrix.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMatrix.worldMatrix));
	objMatrix.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMatrix, 0, 0);
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mMirrorMat, 0, 0);
	
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_mirror);


	m_pImmediateContext->OMSetBlendState(RenderStates::TransparentBS,
										blendFactor,
										0xffffffff);

	//m_pImmediateContext->OMSetDepthStencilState(RenderStates::MarkMirrorDSS, 1);

	m_pImmediateContext->Draw(6, 24);

	//m_pImmediateContext->OMSetDepthStencilState(0, 0);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	
	
	
	//***********************
	HR(m_pSwapChain->Present(0, 0));
}

void MirrorApp::InitBuffers(){

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
	
	std::vector<VertexPNT> vertices(vcount);
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
	bd.ByteWidth = sizeof(VertexPNT)* vcount;             // size is the VERTEX struct * 3
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

void MirrorApp::InitConstantBuffers(){
	// Create the constant buffers for the variables defined in the vertex shader.
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(ObjectMatrices);
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstantBuffers[CB_Object]));

	constantBufferDesc.ByteWidth = sizeof(EYE);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_EyeInfo]));
	
	constantBufferDesc.ByteWidth = sizeof(Light);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_LightInfo]));

	constantBufferDesc.ByteWidth = sizeof(Material);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_MaterialInfo]));
	
	constantBufferDesc.ByteWidth = sizeof(Fog);
	HR(m_pDevice->CreateBuffer(&constantBufferDesc, nullptr, &mpConstP[CB_FogInfo]));
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_FogInfo], 0, nullptr, &mFog, 0, 0);


}



void MirrorApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	Utility::CompileShader(L"Shaders/ShaderMirrorV.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"Shaders/ShaderMirrorP.hlsl", "PShader", profileP, &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mpVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mpPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	// create the input layout object
	m_pDevice->CreateInputLayout(iedPNT, ARRAYSIZE(iedPNT), VS->GetBufferPointer(), VS->GetBufferSize(), &mpLayout);
	m_pImmediateContext->IASetInputLayout(mpLayout);


	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}

void MirrorApp::initPrecompiledShaders()
{
	ID3DBlob* pVS, *pPS;

#if _DEBUG
	LPCWSTR compiledVShader = L"Debug/ShaderMirrorVd.cso";
	LPCWSTR compiledPShader = L"Debug/ShaderMirrorPd.cso";
#else
	LPCWSTR compiledVShader = L"Release/ShaderTexturesV.cso";
	LPCWSTR compiledPShader = L"Release/ShaderTexturesP.cso";
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

	m_pDevice->CreateInputLayout(iedPNT,
		ARRAYSIZE(iedPNT),
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		&mpLayout);

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);

}

void MirrorApp::InitTextures()
{
	EffectFactory* pEffect = new EffectFactory(m_pDevice);

	pEffect->CreateTexture(L"Textures\\floor.dds",
							m_pImmediateContext,
							&mpSRV_floor);
	pEffect->CreateTexture(L"Textures\\bricks.dds",
		m_pImmediateContext,
		&mpSRV_wall);

	pEffect->CreateTexture(L"Textures\\water1.dds",
		m_pImmediateContext,
		&mpSRV_mirror);

	/*ID3D11Resource * resource;
	CreateDDSTextureFromFile(
		m_pDevice,
		L"Texturas\\water2.dds",
		&resource,
		&mpSRV_Water);*/

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	HR(m_pDevice->CreateSamplerState(&samplerDesc, &mpSamplerState));
}
void MirrorApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void MirrorApp::OnMouseMove(WPARAM btnState, int x, int y)
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

void MirrorApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();

}

void MirrorApp::InitRoomBuffer()
{
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |--------------|
	//   |              |
	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/


	VertexPNT v[30];

	v[0] = VertexPNT(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[1] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);

	v[3] = VertexPNT(-3.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f);
	v[4] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f);
	v[5] = VertexPNT(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f);

	// Wall: Observe we tile texture coordinates, and that we
	// leave a gap in the middle for the mirror.
	v[6] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[7] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);

	v[9] = VertexPNT(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[10] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f);
	v[11] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f);

	v[12] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[13] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);

	v[15] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f);
	v[16] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f);
	v[17] = VertexPNT(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f);

	v[18] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = VertexPNT(-3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = VertexPNT(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);

	v[21] = VertexPNT(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = VertexPNT(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f);
	v[23] = VertexPNT(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f);

	// Mirror
	v[24] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[25] = VertexPNT(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[26] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[27] = VertexPNT(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[28] = VertexPNT(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[29] = VertexPNT(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexPNT)* 30;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;
	HR(m_pDevice->CreateBuffer(&vbd, &vinitData, &mpRoomVB));
}


int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	MirrorApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}