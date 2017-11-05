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


using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment (lib, "D3DCompiler.lib")






class TexturesApp : public DXApp{
public:
	TexturesApp(HINSTANCE hInstance);
	~TexturesApp();

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


	//LAND
	Material mLandMat;
	
	ID3D11Buffer *	mpLandVB;
	ID3D11Buffer *  mpLandIB;

	UINT	 mLandIndexCount;

	XMFLOAT4X4  mLandWorld;

	//Adding textures
	ID3D11ShaderResourceView*	mpSRV_Grass; //SAVE THE TEXTURE
	ID3D11ShaderResourceView* mpSRV_GrassNMap;
	ID3D11SamplerState*			mpSamplerState;


	//waves
	Material mWavesMat;
	ID3D11Buffer* mWavesVB;
	ID3D11Buffer* mWavesIB;
	Waves mWaves;
	XMFLOAT4X4 mWavesWorld;
	ID3D11ShaderResourceView*	 mpSRV_Water;

	//alpha blending
	ID3D11BlendState*	mpTransparentBS;

	Fog mFog;

	

private:
	void InitShaders();		//loads and prepares the shaders
	void initPrecompiledShaders();
	void InitBuffers();		//creates the shape to render
	void InitLandBuffers();
	void InitWaveBuffers();
	void InitConstantBuffers();
	void InitRasterizerState();
	void InitTextures();

	float GetHillHeight(float x, float z) const;
	XMFLOAT3 GetHillNormal(float x, float z) const;


	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);
};

TexturesApp::TexturesApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "PhongApp";

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
	//Land material
	mLandMat.Ambient = XMFLOAT4(1, 1, 1, 1.0f);
	mLandMat.Diffuse = XMFLOAT4(0.5, 0.7, 0.5, 1.0f);
	mLandMat.Specular = XMFLOAT4(0.2, 0.2, 0.2, 1.0f);
	mLandMat.SpecularPower = 16.0f;
	mLandMat.UseTexture = 1;

	//Land material
	
	XMMATRIX W = XMMatrixTranslation(0, -3, 0);
	XMStoreFloat4x4(&mWavesWorld, W);

	mWavesMat.Ambient = XMFLOAT4(1, 1, 1, 1.0f);
	mWavesMat.Diffuse = XMFLOAT4(0.5, 0.7, 0.5, 0.6f);
	mWavesMat.Specular = XMFLOAT4(0.9, 0.9, 0.9, 1.0f);
	mWavesMat.SpecularPower = 96.0f;
	mWavesMat.UseTexture = 1;



	mAngle = 0.0f;

	XMMATRIX I = XMMatrixIdentity();

	XMStoreFloat4x4(&mSkullWorld, I);
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProjection, I);
	XMStoreFloat4x4(&mLandWorld, I);

	mFog.Color = GColors::Silver;
	mFog.Start = 10.0f;
	mFog.Range = 70.0f;
	mFog.Enable = 0;
	


}

TexturesApp::~TexturesApp()
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

	Memory::SafeRelease(mpWireframeRS);

	Memory::SafeRelease(mpSamplerState);
	Memory::SafeRelease(mpSRV_Grass);

	Memory::SafeRelease(mWavesVB);
	Memory::SafeRelease(mWavesIB);
}

void TexturesApp::OnResize(){
	DXApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	XMMATRIX P = mCam.Proj();
	//XMMATRIX P = mCam.get XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_ClientWidth / m_ClientHeight, 0.1f, 100.0f);
	XMStoreFloat4x4(&mProjection, P);

}

bool TexturesApp::Init()
{
	if (!DXApp::Init())
		return false;


	InitBuffers();
	InitLandBuffers();
	InitWaveBuffers();
	//InitShaders();
	initPrecompiledShaders();
	InitConstantBuffers();
	InitRasterizerState();
	InitTextures();
	OnResize();

	XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

	XMVECTOR pos = XMLoadFloat3(&mCam.GetPosition());
	mCam.LookAt(pos, focusPoint, upDirection);

	return true;
}



void TexturesApp::Update(float dt)
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
	XMMATRIX W = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(mAngle));
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
	
	

	
	///WAVES UPDATE
	static float t_base = 0.0f;
	if ((mTimer.TotalTime() - t_base) >= 0.25f)
	{
		t_base += 0.25f;

		DWORD i = 5 + rand() % (mWaves.RowCount() - 10);
		DWORD j = 5 + rand() % (mWaves.ColumnCount() - 10);

		float r = MathHelper::RandF(1.0f, 2.0f);

		mWaves.Disturb(i, j, r);
	}

	mWaves.Update(dt);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pImmediateContext->Map(mWavesVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	VertexPNTT* v = reinterpret_cast<VertexPNTT*>(mappedData.pData);
	for (UINT i = 0; i < mWaves.VertexCount(); ++i)
	{
		v[i].Pos = mWaves[i];
		v[i].Normal = mWaves.Normal(i);

		// Derive tex-coords in [0,1] from position.
		v[i].Tex.x = (0.5f + mWaves[i].x / mWaves.Width());
		v[i].Tex.y = (0.5f - mWaves[i].z / mWaves.Depth());
	}

	m_pImmediateContext->Unmap(mWavesVB, 0);



}

void TexturesApp::Render(float dt)
{

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Silver);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//***********************
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);


	m_pImmediateContext->IASetInputLayout(mpLayout);
	// select which primtive type we are using
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// select which vertex buffer to display
	UINT stride = sizeof(VertexPNTT);
	UINT offset = 0;
	
	//m_pImmediateContext->RSSetState(mpWireframeRS);
	//shaders
	m_pImmediateContext->VSSetShader(mpVS, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, NumConstantBuffers, mpConstantBuffers);
	m_pImmediateContext->PSSetShader(mpPS, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, PSNumConstantBuffers, mpConstP);


	ObjectMatrices objMat;


	// draw the vertex buffer to the back buffer
	//-----------------LAND-----------------------

	ID3D11ShaderResourceView *texturesSRV[] = { mpSRV_Grass, mpSRV_GrassNMap};
	
	//textures
	m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);
	m_pImmediateContext->PSSetShaderResources(0, 2, texturesSRV);

	//vertex and index buffers
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpLandVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpLandIB, DXGI_FORMAT_R32_UINT, 0);

	//constant buffers of the vertex shader
	objMat.worldMatrix = XMLoadFloat4x4(&mLandWorld);
	objMat.wvp = objMat.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMat.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr,objMat.worldMatrix));
	objMat.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMat, 0, 0);
	//constant buffers pixel shader
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mLandMat, 0, 0);

	m_pImmediateContext->DrawIndexed(mLandIndexCount, 0, 0);

	//--------------WAVES-----------------------
	m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);

	m_pImmediateContext->IASetVertexBuffers(0, 1, &mWavesVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mWavesIB, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->PSSetSamplers(0, 1, &mpSamplerState);
	m_pImmediateContext->PSSetShaderResources(0, 1, &mpSRV_Water);

	//constant buffers of the vertex shader
	objMat.worldMatrix = XMLoadFloat4x4(&mWavesWorld);
	objMat.wvp = objMat.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMat.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMat.worldMatrix));
	objMat.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMat, 0, 0);
	//constant buffers pixel shader
	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mWavesMat, 0, 0);
	//m_pImmediateContext->DrawIndexed(3 * mWaves.TriangleCount(), 0, 0);
	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	
	//-----------------------SKULL-------------------------
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpModelVB, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(mpSkullIB, DXGI_FORMAT_R32_UINT, 0);

	m_pImmediateContext->OMSetBlendState(mpTransparentBS, blendFactor, 0xffffffff);
	//constant buffers of the vertex shader
	objMat.worldMatrix = XMLoadFloat4x4(&mSkullWorld);
	objMat.wvp = objMat.worldMatrix*XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProjection);
	objMat.worldIT = XMMatrixTranspose(XMMatrixInverse(nullptr, objMat.worldMatrix));
	objMat.texTransform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	m_pImmediateContext->UpdateSubresource(mpConstantBuffers[CB_Object], 0, nullptr, &objMat, 0, 0);

	m_pImmediateContext->UpdateSubresource(mpConstP[CB_MaterialInfo], 0, nullptr, &mSkullMat, 0, 0);
	
	m_pImmediateContext->DrawIndexed(mModelIndexCount, 0, 0);

	m_pImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	//***********************
	HR(m_pSwapChain->Present(0, 0));
}

void TexturesApp::InitBuffers(){

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
	
	std::vector<VertexPNTT> vertices(vcount);
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
	bd.ByteWidth = sizeof(VertexPNTT)* vcount;             // size is the VERTEX struct * 3
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

void TexturesApp::InitLandBuffers()
{
	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;
	geoGen.CreateGrid(160.0f, 160.0f, 50, 50, grid);

	mLandIndexCount = grid.Indices.size();

	std::vector<VertexPNTT> vertices(grid.Vertices.size());
	for (UINT i = 0; i < grid.Vertices.size(); ++i)
	{
		XMFLOAT3 p = grid.Vertices[i].Position;
		//p.y = GetHillHeight(p.x, p.z);
		vertices[i].Pos = p;
		//vertices[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[i].Normal = GetHillNormal(p.x,p.z);
		vertices[i].Tex = XMFLOAT2(2*grid.Vertices[i].TexC.x, 2*grid.Vertices[i].TexC.y);
		vertices[i].Tangent = grid.Vertices[i].TangentU;
	}

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.ByteWidth = sizeof(VertexPNTT)* grid.Vertices.size();             // size is the VERTEX struct * 3
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
void TexturesApp::InitWaveBuffers()
{
	// Create the vertex buffer.  Note that we allocate space only, as
	// we will be updating the data every time step of the simulation.
	mWaves.Init(160, 160, 1.0f, 0.03f, 3.25f, 0.4f);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VertexPNTT)* mWaves.VertexCount();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	HR(m_pDevice->CreateBuffer(&vbd, 0, &mWavesVB));


	// Create the index buffer.  The index buffer is fixed, so we only 
	// need to create and set once.

	std::vector<UINT> indices(3 * mWaves.TriangleCount()); // 3 indices per face

	// Iterate over each quad.
	UINT m = mWaves.RowCount();
	UINT n = mWaves.ColumnCount();
	int k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (DWORD j = 0; j < n - 1; ++j)
		{
			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;

			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &mWavesIB));
}


void TexturesApp::InitConstantBuffers(){
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



void TexturesApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	Utility::CompileShader(L"Shaders/ShaderTexturesV.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"Shaders/ShaderTexturesP.hlsl", "PShader", profileP, &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &mpVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &mpPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(mpVS, 0, 0);
	m_pImmediateContext->PSSetShader(mpPS, 0, 0);

	// create the input layout object
	m_pDevice->CreateInputLayout(iedPNTT, ARRAYSIZE(iedPNTT), VS->GetBufferPointer(), VS->GetBufferSize(), &mpLayout);
	m_pImmediateContext->IASetInputLayout(mpLayout);


	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}

void TexturesApp::initPrecompiledShaders()
{
	ID3DBlob* pVS, *pPS;

#if _DEBUG
	LPCWSTR compiledVShader = L"Debug/ShaderTexturesVd.cso";
	LPCWSTR compiledPShader = L"Debug/ShaderTexturesPd.cso";
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

	m_pDevice->CreateInputLayout(iedPNTT,
		ARRAYSIZE(iedPNTT),
		pVS->GetBufferPointer(),
		pVS->GetBufferSize(),
		&mpLayout);

	Memory::SafeRelease(pVS);
	Memory::SafeRelease(pPS);

}

void TexturesApp::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));

	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME; // D3D11_FILL_SOLID;  //
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;
	HR(m_pDevice->CreateRasterizerState(&wireframeDesc, &mpWireframeRS));
	
	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(m_pDevice->CreateBlendState(&transparentDesc, &mpTransparentBS));


}

void TexturesApp::InitTextures()
{
	EffectFactory* pEffect = new EffectFactory(m_pDevice);

	pEffect->CreateTexture(L"Textures\\stones.dds",
							m_pImmediateContext,
							&mpSRV_Grass);
	pEffect->CreateTexture(L"Textures\\stones_nmap.dds",
		m_pImmediateContext,
		&mpSRV_GrassNMap);

	pEffect->CreateTexture(L"Textures\\water1.dds",
		m_pImmediateContext,
		&mpSRV_Water);

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
void TexturesApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		mLastMousePos.x = x;
		mLastMousePos.y = y;

		SetCapture(m_hAppWnd);
	}
}

void TexturesApp::OnMouseMove(WPARAM btnState, int x, int y)
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

void TexturesApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) == 0)
	{

	}
	ReleaseCapture();

}

float TexturesApp::GetHillHeight(float x, float z) const
{
	return  0.3*( z*sinf(0.1f * x) + x*cosf(0.1f*z) );
}

DirectX::XMFLOAT3 TexturesApp::GetHillNormal(float x, float z) const
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






int WINAPI WinMain777(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TexturesApp tApp(hInstance);
	if (!tApp.Init())
		return 1;


	return tApp.Run();
}