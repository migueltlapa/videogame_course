#include "DXApp.h"
#include <WindowsX.h>
#include <vector>
#include <dxgi.h>
#include <sstream>
#include <iostream>

#pragma comment (lib, "dxgi.lib")

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
	m_ClientWidth = 800;
	m_ClientHeight = 600;
	m_appTitle = "Sesión 01: DXSetUP";
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

	mTimer.Reset();

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else
		{
			mTimer.Tick();

			if (!m_AppPaused)
			{
				CalculateFrameStats();
				//Update
				Update(mTimer.DeltaTime());
				//Render
				Render(mTimer.DeltaTime());
			}
			else
			{
				Sleep(100);
			}
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
		D3D_FEATURE_LEVEL_11_0, //first check d3d 11 support
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1;								// one back buffer
	swapDesc.BufferDesc.Width = m_ClientWidth;				// set the back buffer width
	swapDesc.BufferDesc.Height = m_ClientHeight;			// set the back buffer height
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// use 32-bit color
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;			
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;		
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	swapDesc.OutputWindow = m_hAppWnd;						// the window to be used
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;			
	swapDesc.Windowed = true;								//windowed/full-screen mode
	swapDesc.SampleDesc.Count = 1;							//how many multisamples
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //allow alt-enter full-screen

	HRESULT result;

	UINT j = 0;
	IDXGIAdapter * pAdapter;
	IDXGIFactory * pFactory;
	pAdapter = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);

	std::vector <IDXGIAdapter*> vAdapters;
	while (pFactory->EnumAdapters(j, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
		++j;
	}

	Memory::SafeRelease(pFactory);
	

	for (int i = 0; i < numDriverTypes; ++i)
	{
		IDXGIAdapter* pAdapt = vAdapters[0];
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


#pragma region CREATE THE DEPTH/STENCIL VIEW

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


#pragma endregion

#pragma region 2. CREATE THE RENDER TARGET VIEW
	//create the render target view
	//big image that is been rendered, we need a texture that we bind to the view

	ID3D11Texture2D* pBackBufferTex = 0;
	HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBufferTex)));
	HR(m_pDevice->CreateRenderTargetView(pBackBufferTex, nullptr, &m_pRenderTargetView));
	Memory::SafeRelease(pBackBufferTex);
	
	//Bind render target view (the render target is the back buffer)
	//m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
#pragma endregion

#pragma region 3. INITIALIZE THE VIEWPORT
	//viewport creation
	m_Viewport.Width = static_cast<float> (m_ClientWidth);
	m_Viewport.Height = static_cast<float> (m_ClientHeight);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	//Bind Viewport
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

#pragma endregion
	
	
	return true;
	
}

float DXApp::AspectRatio()const
{
	return static_cast<float>(m_ClientWidth) / m_ClientHeight;
}

void DXApp::OnResize(){

	assert(m_pImmediateContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pDepthStencilView);
	Memory::SafeRelease(m_pDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	HR(m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView));
	Memory::SafeRelease(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_ClientWidth;
	depthStencilDesc.Height = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	/*if (mEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{*/
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	//}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer));
	HR(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	// Set the viewport transform.

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = static_cast<float>(m_ClientWidth);
	m_Viewport.Height = static_cast<float>(m_ClientHeight);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;



	m_pImmediateContext->RSSetViewports(1, &m_Viewport);
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
	UINT height = r.bottom - r.top;

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
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_AppPaused = true;
			mTimer.Stop();
		}
		else
		{
			m_AppPaused = false;
			mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_ClientWidth = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);
		if (m_pDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					OnResize();
				}
				else if (m_Resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		mTimer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//This code has been copied from the book of Frank D. Luna
void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		//outs.precision(6);
		outs << m_appTitle << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(m_hAppWnd,  outs.str().c_str() );

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
