#include "Framework\DXApp.h"

class TestApp : public DXApp
{
public: 
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool Init() override;
	void Update(float dt) override;
	void Render(float dt) override;
};


TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}

TestApp::~TestApp()
{

}

bool TestApp::Init()
{
	return DXApp::Init();
}

void TestApp::Update(float dt)
{

}

void TestApp::Render(float dt)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pSwapChain->Present(0, 0);
}


int WINAPI WinMain0( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TestApp tApp(hInstance);
	if (!tApp.Init()) return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return tApp.Run(); 
}

