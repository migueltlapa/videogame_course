#include "DXApp.h"

struct VERTEX{ FLOAT X, Y, Z; DirectX::XMFLOAT4 Color; };



class TriangleApp : public DXApp
{
public:
	TriangleApp(HINSTANCE hInstance);
	~TriangleApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;

private:// Son privados no quiero heredar los atributos
	//Layout
	
	ID3D11InputLayout * mpLayout;  // Especifica com ose va mandar la informacion
	//Shader
	ID3D11VertexShader *mpVS;
	ID3D11PixelShader  *mpPs;
	//Vertex BUffer
	ID3D11Buffer      *mpVBuffer;

private:
	void InitShaders();
	void InitGraphics();

};


TriangleApp::TriangleApp(HINSTANCE hInstance) : DXApp(hInstance)
{
	m_appTitle = "TriangleApp";


}

TriangleApp::~TriangleApp()
{
	Memory::SafeRelease(mpLayout);
	Memory::SafeRelease(mpVS);
	Memory::SafeRelease(mpPs);
	Memory::SafeRelease(mpVBuffer);

}

bool TriangleApp::Init()
{
	DXApp::Init();
	InitShaders();
	InitGraphics();
	return true;
}

void TriangleApp::Update(float dt)
{
	using namespace DirectX;
	using namespace DirectX::PackedVector;

	if (GetAsyncKeyState('A') & 0x01)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(m_pImmediateContext->Map(mpVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

		VERTEX* v = reinterpret_cast<VERTEX*>(mappedData.pData);
		v[0].X = v[0].X + 0.001f;
		m_pImmediateContext->Unmap(mpVBuffer, 0);
	}





}

void TriangleApp::Render(float dt)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pImmediateContext->IASetInputLayout(mpLayout);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &mpVBuffer, &stride, &offset);
	m_pImmediateContext->Draw(3, 0);
	
	
	m_pSwapChain->Present(0, 0);
}

void TriangleApp::InitShaders()
{
	ID3DBlob *pVS, *pPS;


	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS; // sEA LO SUFICIENTEME ESTRICTO PARA SER LA COMPILACION

#if defined(DEBUG) || (_DEBUG)  // checa los dos DEBUG and _DEBUG
	flags |= D3DCOMPILE_DEBUG;

#endif

	LPCTSTR profileV = (m_FeatureLevel >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";

	LPCTSTR profileP = (m_FeatureLevel >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";

	HRESULT hr;
	
	hr = Utility::CompileShader(L"shaderV.hlsl", "main", profileV, &pVS);
	if (FAILED (hr))
	{
		//OutputDebugString("Failed compiling the vertex shader") Aqui imprime solo mensaje
		printf("Failed Compiling Vertex Shader %08x\n", hr);
		// Aqui imprime el error
	}


	hr = Utility::CompileShader(L"shaderP.hlsl", "PShader", profileP, &pPS);
	if (FAILED(hr))
	{
		//OutputDebugString("Failed compiling the vertex shader") Aqui imprime solo mensaje
		printf("Failed Compiling Vertex Shader %08x\n", hr);
		// Aqui imprime el error
	}


	m_pDevice->CreateVertexShader(pVS->GetBufferPointer(),pVS->GetBufferSize(),NULL,&mpVS);
	m_pDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), NULL, &mpPs);

	m_pImmediateContext->VSSetShader(mpVS, 0, 0);// Cargamos a Contexto
	m_pImmediateContext->PSSetShader(mpPs, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[]=
	{// Aqui describimos lo que vamos a mandar
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA },

		// Aqui la posicion es 4 x 3 en el offset del Color
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA }


	};
	m_pDevice->CreateInputLayout(ied, 2, pVS->GetBufferPointer(),pVS->GetBufferSize(), &mpLayout);
		// El 2 es por el numero de elementos de l aestructura
		
		Memory::SafeRelease(pVS);
		Memory::SafeRelease(pPS);	

}

void TriangleApp::InitGraphics()
{
	VERTEX tVertices[] ={

		{ 0.0f, 0.5f, 0.0f, {1.0f,0.0f,0.0f,1.0f} },
		{ 0.45f, -0.5f, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ -0.45f, -0.5f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } }


	};

	//Create the vertex buffer
	D3D11_BUFFER_DESC bdesc;
	ZeroMemory(&bdesc, sizeof(bdesc));
	bdesc.Usage = D3D11_USAGE_DYNAMIC;
	bdesc.ByteWidth = sizeof(VERTEX)*3;
	bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = tVertices;// Es el MemCopy
	HR(m_pDevice->CreateBuffer(&bdesc, &vData, &mpVBuffer));


}
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TriangleApp tApp(hInstance);
	if (!tApp.Init()) return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return tApp.Run();
}

