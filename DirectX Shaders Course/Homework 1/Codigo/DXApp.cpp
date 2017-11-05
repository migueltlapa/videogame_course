#include "DXApp.h"


namespace
{
	//used to forward msgs to user define proc function
	DXApp* g_App = nullptr;

}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_App) return g_App->MsgProc(hwnd, msg, wParam, lParam);
	else return DefWindowProc(hwnd, msg, wParam, lParam);

}

DXApp::DXApp(HINSTANCE hInstance)
{
	m_AppInstance = hInstance;
	m_hAppWnd = NULL;
	m_ClientWidth = 600;
	m_ClientHeight = 600;
	m_appTitle = "Demo 02: DXSetUPHR";
	m_WndStyle = WS_OVERLAPPEDWINDOW;
	g_App = this;


	//DirectX attributes
	m_pDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;


}


DXApp::~DXApp()
{

	//Cleanup direct3D

	if (m_pImmediateContext)  m_pImmediateContext->ClearState();

	//Remove all COM Objectos (ID* : ID3D11, IDXGI,...)
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);

}


int DXApp::Run()
{
	//main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else
		{
			//Update
			Update(0.0f);
			//Render
			Render(0.0f);
		}
	}

	return static_cast<int>(msg.wParam);
}

bool DXApp::Init()
{
	if (!InitWindow())
		return false;

	//INITIALIZE DIRECT3D
	if (!InitDirect3D())
		return false;

	return true;
}


bool DXApp::InitDirect3D()
{
#pragma region 1. CREATE THE DEVICE AND SWAP CHAIN
	

	UINT createDeviceFlags = 0;  
#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; //create a device that supports the debug layer
#endif //Debug

	//driver types
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	//nvidia, intel graphics 
		D3D_DRIVER_TYPE_WARP,		//High performance software rasterizer
		D3D_DRIVER_TYPE_REFERENCE	//very slow, sofware implementation that supports every Direct3D feature, used for testing and debugging other drivers.
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);


	//Feature levels: which version of D3D is supported by the device
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // color
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hAppWnd;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //allow alt-enter fullscreen

	HRESULT result;

	for (int i = 0; i < numDriverTypes; ++i)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice,
			&m_FeatureLevel, &m_pImmediateContext);

		if (SUCCEEDED(result))
		{
			m_DriverType = driverTypes[i];
			break;
		}

	}

	if (FAILED(result))
	{
		OutputDebugString("Failed to create device and swap chain");
		return false;
	}

	if (FAILED(result))
	{
		OutputDebugString("Failed to create device and swap chain");
		return false;
	}
#pragma endregion  


#pragma region 2. CREATE THE RENDER TARGET VIEW
	//create the render target view
	//big image that is been rendered, we need a texture that we bind to the view

	ID3D11Texture2D* pBackBufferTex = 0;
	HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBufferTex)));
	HR(m_pDevice->CreateRenderTargetView(pBackBufferTex, nullptr, &m_pRenderTargetView));
	Memory::SafeRelease(pBackBufferTex);
	
	//Bind render target view
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
#pragma endregion

#pragma region 3. INITIALIZE THE VIEWPORT
	//viewport creation
	// Original Start
	//m_Viewport.Width = static_cast<float> (m_ClientWidth);
	//m_Viewport.Height = static_cast<float> (m_ClientHeight);
	//m_Viewport.TopLeftX = 0;
	//m_Viewport.TopLeftY = 0;
	//m_Viewport.MinDepth = 0.0f;
	//m_Viewport.MaxDepth = 1.0f;
	// Original EnD
	//Bind Viewport
	
	m_Viewport[0].Width = static_cast<float> (300);
	m_Viewport[0].Height = static_cast<float> (300);
	m_Viewport[0].TopLeftX = 0;
	m_Viewport[0].TopLeftY = 0;
	m_Viewport[0].MinDepth = 0.0f;
	m_Viewport[0].MaxDepth = 1.0f;

	m_Viewport[1].Width = static_cast<float> (300);
	m_Viewport[1].Height = static_cast<float> (300);
	m_Viewport[1].TopLeftX = 300;
	m_Viewport[1].TopLeftY = 300;
	m_Viewport[1].MinDepth = 0.0f;
	m_Viewport[1].MaxDepth = 1.0f;


	
	
	// Original BEg
	//m_pImmediateContext->RSSetViewports(1, &m_Viewport);
	// Original END

	m_pImmediateContext->RSSetViewports(2, &m_Viewport[0]);
	
#pragma endregion
	
	
	return true;
	
}

bool DXApp::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //horizontal and vertical redraw really old
	//important
	wcex.hInstance = m_AppInstance;
	wcex.lpfnWndProc = MainWndProc;  //like a pointer
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //(HBRUSH)(COLOR_WINDOW + 1); //
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "DXAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		OutputDebugString("\nFAILDED TO CREATE WINDOW CLASS\n");
		return false;
	}

	RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.right - r.top;

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_appTitle.c_str(), m_WndStyle,
		x, y, width, height, NULL, NULL, m_AppInstance, NULL);

	if (!m_hAppWnd){
		OutputDebugString("\nFAILDED TO CREATE WINDOW\n");
		return false;
	}

	ShowWindow(m_hAppWnd, SW_SHOW);

	return true;

}

LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); //end the app
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);

	}
}

