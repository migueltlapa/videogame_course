#include "Framework\DXApp.h"

#include "D3DCompiler.h"

#pragma comment (lib, "D3DCompiler.lib")

struct VERTEX{ DirectX::XMFLOAT3 Pos; DirectX::XMFLOAT4 Color; };

class TriangleApp : public DXApp{
public:
	TriangleApp(HINSTANCE hInstance);
	~TriangleApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;

private:

	ID3D11InputLayout	*pLayout;           // the pointer to the input layout
	ID3D11VertexShader	*pVS;               // the pointer to the vertex shader
	ID3D11PixelShader	*pPS;               // the pointer to the pixel shader
	ID3D11Buffer		*pVBuffer;          // the pointer to the vertex buffer
	ID3D11Buffer		*pIBuffer;			
		
	void InitShaders();		//loads and prepares the shaders
	void InitGraphics();	//creates the shape to render
	void CreateLayout();	//create the layout 
};


TriangleApp::TriangleApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "TriangleApp";
}

TriangleApp::~TriangleApp()
{

}

bool TriangleApp::Init()
{
	if (!DXApp::Init())
		return false;


	InitGraphics();
	InitShaders();

	return true;

}



void TriangleApp::Update(float dt)
{

	using namespace DirectX;
	using namespace DirectX::PackedVector;

	if (GetAsyncKeyState('A') & 0x01)
	{

		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pImmediateContext->Map(pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

		VERTEX* v = reinterpret_cast<VERTEX*>(mappedData.pData);

		XMVECTOR vec;
		vec = DirectX::XMLoadFloat3(&v[0].Pos);
		vec = vec + DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.001, 0,0));
		XMStoreFloat3(&v[0].Pos, vec);
		

		/*for (int i = 0; i < 3; i++){
			v[i].Pos = DirectX::XMFLOAT3(v[i].Pos.x + 0.001f, v[i].Pos.y, v[i].Pos.z);

		}*/

		m_pImmediateContext->Unmap(pVBuffer, 0);


	}
		
}

void TriangleApp::Render(float dt)
{

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//***********************


	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	m_pImmediateContext->IASetInputLayout(pLayout);
	// select which primtive type we are using
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

	// draw the vertex buffer to the back buffer
	m_pImmediateContext->DrawIndexed(3, 0, 0);

	//***********************
	HR(m_pSwapChain->Present(1, 0));
}

void TriangleApp::InitGraphics(){
	// create a triangle using the VERTEX struct
	VERTEX tVertices[] =
	{
		{ DirectX::XMFLOAT3(0, 0.5f, 0.0f),		{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(0.45f, -0.5, 0.0f), { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f), { 0.0f, 0.0f, 1.0f, 1.0f } }
	};
	
	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	//bd.Usage = D3D11_USAGE_IMMUTABLE;             // we wont change this vertex
	bd.Usage = D3D11_USAGE_DYNAMIC;             // we wont change this vertex
	bd.ByteWidth = sizeof(VERTEX)* 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      // use as a vertex buffer
	bd.CPUAccessFlags = 0;						  // we don't need to allow CPU to write in buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA bdData;
	bdData.pSysMem = tVertices;
	
	HR(m_pDevice->CreateBuffer(&bd, &bdData, &pVBuffer));       // create the buffer

	UINT indices[] = { 0, 1, 2 };

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT)* 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &pIBuffer));


}

void TriangleApp::InitShaders(){
	// load and compile the two shaders
	ID3DBlob *VS, *PS;
	
	LPCSTR profileV = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
	LPCSTR profileP = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
		
	Utility::CompileShader(L"shadersV.hlsl", "VShader", profileV, &VS);
	Utility::CompileShader(L"shadersP.hlsl", "PShader", profileP, &PS);
		
	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(pVS, 0, 0);
	m_pImmediateContext->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	
	m_pDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	m_pImmediateContext->IASetInputLayout(pLayout);


	Memory::SafeRelease(VS);
	Memory::SafeRelease(PS);
}


int WINAPI WinMain2(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TriangleApp tApp(hInstance);
	if (!tApp.Init()) 
		return 1;


	return tApp.Run();
}