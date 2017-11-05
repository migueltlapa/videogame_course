#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>
#include "DXUtil.h"

#include "GameTimer.h"


class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

	//main application loop
	int Run();

	//Framework methods
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual void OnResize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	//mouse input
	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }


protected:
	//win32 attributes
	HWND		m_hAppWnd;
	HINSTANCE	m_AppInstance;
	UINT		m_ClientWidth;
	UINT		m_ClientHeight;
	
	std::string	m_appTitle;
	DWORD		m_WndStyle;

	//DirectX attributes

	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D11_VIEWPORT			m_Viewport;

	ID3D11Texture2D*		m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

	//timer
	GameTimer mTimer;
	bool      m_AppPaused;

	//window states
	bool      m_Minimized;
	bool      m_Maximized;
	bool      m_Resizing;

protected:
	bool InitWindow();
	

	float     AspectRatio()const;
	
	//Initialize direct3d
	bool InitDirect3D();

	//Frames per second
	void CalculateFrameStats();
};

